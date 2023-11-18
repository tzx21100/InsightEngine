/*!
 * \file Graphics.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This source file defines the implementation for class ISGraphics which
 * encapsulates the functionalities of a Graphics Engine.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Graphics.h"
#include "Body.h"
#include "Collider.h"
#include "Physics.h"

#include <stb_image.h>

namespace IS {

    /// Static objects ///

    // Frame Buffer
    std::shared_ptr<Framebuffer> ISGraphics::mFramebuffer;

    // Meshes vector
    std::vector<Mesh> ISGraphics::meshes;

    // Shaders
    Shader ISGraphics::inst_quad_shader_pgm;
    Shader ISGraphics::inst_3d_quad_shader_pgm;
    Shader ISGraphics::inst_non_quad_shader_pgm;
    Shader ISGraphics::quad_border_shader_pgm;
    Shader ISGraphics::light_shader_pgm;

    // Texture vector
    std::vector<Image> ISGraphics::textures;

    // Animation objects
    Animation ISGraphics::idle_ani;
    Animation ISGraphics::walking_ani;
    Animation ISGraphics::ice_cream_truck_ani;

    // instance data containers
    std::multiset<Sprite::instanceData, Sprite::GfxLayerComparator> ISGraphics::layeredQuadInstances;
    std::multiset<Sprite::instanceData3D, Sprite::GfxLayerComparator> ISGraphics::layered3DQuadInstances;
    std::vector<Sprite::nonQuadInstanceData> ISGraphics::lineInstances;
    std::vector<Sprite::nonQuadInstanceData> ISGraphics::circleInstances;
    std::vector<Sprite::instanceData3D> ISGraphics::lightInstances;

    // Editor and entity camera
    Camera ISGraphics::cameras[2];
    Camera3D ISGraphics::cameras3D[2];
    
    // Text Objects
    Text ISGraphics::Times_New_Roman_font;
    Text ISGraphics::Brush_Script_font;

    // Text animation boolean
    bool ISGraphics::mShowTextAnimation = true;

    Light testLight, testLight2, testLight3, testLight4;

    void ISGraphics::Initialize() {
        glClearColor(0.f, 0.f, 0.f, 0.f); // set background to white

        auto [width, height] = InsightEngine::Instance().GetWindowSize();
        glViewport(0, 0, width, height); // set viewport to window size

        // init graphics systems
        Mesh::initMeshes(meshes); // init 3 meshes
        
        // init quad shader
        inst_quad_shader_pgm.setupInstSpriteShaders();
        inst_3d_quad_shader_pgm.setup3DInstSpriteShaders();
        quad_border_shader_pgm.setupPickedQuadShaders();

        // init debugging lines and circles shaders
        inst_non_quad_shader_pgm.setupInstNonQuadShaders();

        light_shader_pgm.setupLightingShaders();

        // init font shaders
        Times_New_Roman_font.shader.setupTextShaders();
        Brush_Script_font.shader.setupTextShaders();

        // init text object
        Times_New_Roman_font.initText("Assets/Fonts/Times-New-Roman.ttf");
        Brush_Script_font.initText("Assets/Fonts/BRUSHSCI.ttf");
        
        // init 3 animations
        walking_ani.initAnimation(1, 4, 1.f); 
        idle_ani.initAnimation(1, 8, 3.f);
        ice_cream_truck_ani.initAnimation(1, 6, 2.f);

        // create framebuffer
        Framebuffer::FramebufferProps props{ 0, 0, static_cast<GLuint>(width), static_cast<GLuint>(height) }; 
        mFramebuffer = std::make_shared<Framebuffer>(props);
        //std::for_each_n(cameras, 2, [width](Camera& camera)
        //{
        //    camera.UpdateCamPos(0, 0);
        //    camera.UpdateCamDim(static_cast<float>(width),static_cast<float>(InsightEngine::Instance().GetWindowHeight()));
        //});

        for (int i{}; i < 2; ++i) {
            cameras3D[i].Init(width, height, 60.f);
        }

    #ifdef USING_IMGUI
        Camera3D::mActiveCamera = CAMERA_TYPE_EDITOR;
    #else
        Camera3D::mActiveCamera = CAMERA_TYPE_GAME;
    #endif // USING_IMGUI


        // set line width for all GL_LINES and GL_LINE_LOOP
        setLineWidth(2.f);

        //testLight.init({ 0.f, 0.f }, { 1.f, 1.f, 1.f }, 0.8f, 1000.f);
        //testLight2.init({ -100.f, 100.f }, { 1.f, 0.f, 0.f }, 1.0f, 300.f);
        //testLight3.init({ 0.f, 100.f }, { 0.f, 1.f, 0.f }, 1.0f, 300.f);
        //testLight4.init({ 100.f, 100.f }, { 0.f, 0.f, 1.f }, 1.0f, 300.f);
    }

    std::string ISGraphics::GetName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
    }

    void ISGraphics::Update(float delta_time) {
        // macro to catch graphics error
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            IS_CORE_ERROR("OpenGL Error: {}", error);
        }

        InsightEngine& engine = InsightEngine::Instance(); // get engine instance

        if (engine.mRenderGUI)
        {
            if (auto const& [fb_width, fb_height] = mFramebuffer->GetSize();
                engine.GetEditorLayer()->GetViewportSize().x > 0.f && engine.GetEditorLayer()->GetViewportSize().y > 0.f &&
                (fb_width != engine.GetEditorLayer()->GetViewportSize().x || fb_height != engine.GetEditorLayer()->GetViewportSize().y))
            {
                // resize framebuffer based on panel size
                ResizeFramebuffer(static_cast<GLuint>(engine.GetEditorLayer()->GetViewportSize().x),
                                  static_cast<GLuint>(engine.GetEditorLayer()->GetViewportSize().y));

                // bind framebuffer after resize
                mFramebuffer->Bind();

                // set clear color
                glClearColor(0.f, 0.f, 0.f, 0.f);

                // clear color buffer
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }

        for (int step = 0; step < InsightEngine::currentNumberOfSteps; ++step) { // fixed dt
            /*
            // empty quad instance data
            layeredQuadInstances.clear();

            // for each entity
            for (auto& entity : mEntities) {
                // get sprite and transform components
                auto& sprite = engine.GetComponent<Sprite>(entity);
                auto& trans = engine.GetComponent<Transform>(entity);

                // update sprite's transform [will be changed]
                sprite.followTransform(trans);
                sprite.transform();

                // if a sprite has animations, update their values with dt
                if (!sprite.anims.empty()) {
                    sprite.anims[sprite.animation_index].updateAnimation(delta_time);
                }

                // quad entities
                if (sprite.primitive_type == GL_TRIANGLE_STRIP) { 
                    // if sprite and it's layer is to be rendered
                    if (Sprite::layersToIgnore.find(sprite.layer) == Sprite::layersToIgnore.end() && sprite.toRender) {
                        // empty instance data
                        Sprite::instanceData instData; 
                        
                        // all quads will have xform, entityID, layer
                        instData.model_to_ndc_xform = ISMtx33ToGlmMat3(sprite.model_TRS.mdl_to_ndc_xform);
                        instData.entID = static_cast<float>(entity);
                        instData.layer = sprite.layer;

                        // quads with no texture
                        if (sprite.img.texture_id == 0) {
                            // copy sprite's color to instance data
                            instData.color = sprite.color;
                            // set to -1 to represent no texture
                            instData.tex_index = -1.f;
                        }
                        // quad has a texture (animation too)
                        else {
                            // copy texture index
                            instData.tex_index = static_cast<float>(sprite.img.texture_index);
                        }

                        // if sprite is an animation
                        if (!sprite.anims.empty()) {
                            // copy animation data
                            instData.anim_frame_dimension = sprite.anims[sprite.animation_index].frame_dimension;
                            instData.anim_frame_index = sprite.anims[sprite.animation_index].frame_index;
                        }
                        // insert to multiset with comparator function
                        layeredQuadInstances.insert(instData);
                    }
                }
            }
            */

            // empty quad instance data
            layered3DQuadInstances.clear();

            // for each entity
            for (auto& entity : mEntities) {
                // get sprite and transform components
                auto& sprite = engine.GetComponent<Sprite>(entity);
                auto& trans = engine.GetComponent<Transform>(entity);

                // to remove, just testing
                if (engine.GetEntityName(entity) == "Player") testLight.position = { trans.getWorldPosition().x, trans.getWorldPosition().y }; 

                // update sprite's transform [will be changed]
                sprite.followTransform(trans);
                sprite.transform();

                // if a sprite has animations, update their values with dt
                if (!sprite.anims.empty()) {
                    sprite.anims[sprite.animation_index].updateAnimation(delta_time);
                }

                // quad entities
                if (sprite.primitive_type == GL_TRIANGLE_STRIP) {
                    // if sprite and it's layer is to be rendered
                    if (Sprite::layersToIgnore.find(sprite.layer) == Sprite::layersToIgnore.end() && sprite.toRender) {
                        // empty instance data
                        Sprite::instanceData3D instData;

                        // all quads will have xform, entityID, layer
                        instData.model_to_ndc_xform = sprite.model_TRS.mdl_to_3dcam_to_ndc_xform;
                        instData.entID = static_cast<float>(entity);
                        instData.layer = sprite.layer;

                        // quads with no texture
                        if (sprite.img.texture_id == 0) {
                            // copy sprite's color to instance data
                            instData.color = sprite.color;
                            // set to -1 to represent no texture
                            instData.tex_index = -1.f;
                        }
                        // quad has a texture (animation too)
                        else {
                            // copy texture index
                            instData.tex_index = static_cast<float>(sprite.img.texture_index);
                        }

                        // if sprite is an animation
                        if (!sprite.anims.empty()) {
                            // copy animation data
                            instData.anim_frame_dimension = sprite.anims[sprite.animation_index].frame_dimension;
                            instData.anim_frame_index = sprite.anims[sprite.animation_index].frame_index;
                        }
                        // insert to multiset with comparator function
                        layered3DQuadInstances.insert(instData);
                    }
                }
            }

            // @YIMING TO CHANGE ACCORDINGLY
            //cameras3D[1].update_camera_pos(cameras[1].GetCamPos().x, cameras[1].GetCamPos().y); // follows 2d camera's position for now
            //cameras3D[1].update_camera_zoom(cameras[1].GetZoomLevel());
            // cameras3D[1].camera_keyboard_callback(1500.f * delta_time); // follows keyboard's input to change pos, must comment line 263 coz it overrides
            cameras3D[Camera3D::mActiveCamera].Update();
            // Graphics system's draw
            Draw(delta_time);
        }
    }

    void ISGraphics::Draw([[maybe_unused]] float delta_time) {
        // get engine instance
        InsightEngine& engine = InsightEngine::Instance();

        // bind fb
        if (engine.mRenderGUI) mFramebuffer->Bind();

        // set clear color
        glClearColor(0.f, 0.f, 0.f, 0.f);

        // clear color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        /// get width and height, set viewport size
        if (auto const& window = engine.GetSystem<WindowSystem>("Window"); !engine.mRenderGUI) {
            auto const& [width, height] = window->IsFullScreen() ? window->GetMonitorSize() : window->GetWindowSize();
            glViewport(0, 0, width, height);
        }

        // can enable to test drawing of debug line / circles
        // Sprite::drawDebugLine({ 0.f, 0.f }, { 200.f, 0.f }, { 1.0f, 0.0f, 0.0f });
        // Sprite::drawDebugCircle({ 0.f, 0.f }, { 500.f, 500.f }, { 0.0f, 1.0f, 0.0f });
        //Sprite::draw_colored_quad({ 200.f, 200.f }, 20.f, { 500.f, 500.f }, { 1.f, 1.f, 0.5f, 0.5f }, 4);

        //auto system = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        //Image* img = system->GetImage("icecream_truck_frame.png");
        //Sprite::draw_textured_quad({ -200.f, 200.f }, 340.f, { 500.f, 500.f }, *img, 4);


        // quads will be drawn first
        // Sprite::draw_instanced_quads();
        Sprite::draw_instanced_3D_quads();
        setLineWidth(3.f);
    #ifdef USING_IMGUI
        Sprite::draw_picked_entity_border();
    #endif // USING_IMGUI
        setLineWidth(2.f);

        // followed by debugging circles and lines
        Sprite::draw_instanced_circles();
        Sprite::draw_instanced_lines();
        testLight.draw();
        testLight2.draw();
        testLight3.draw();
        testLight4.draw();
        Sprite::draw_lights();

        // for each entity
        for (auto& entity : mEntities) {
            // if they have a collider component
            if (engine.HasComponent<Collider>(entity)) {
                auto& collider = engine.GetComponent<Collider>(entity);
                // draw their outline if activated
                Physics::DrawOutLine(collider);
            }
            // if they have a collider component
            if (engine.HasComponent<RigidBody>(entity)) {
                auto& body = engine.GetComponent<RigidBody>(entity);
                // draw their outline if activated
                if (Physics::mShowVelocity) Sprite::drawDebugLine(body.mPosition, body.mPosition + body.mVelocity, { 1.f, 0.f, 0.f });
            }
        }

        // Render text when GUI is disabled
        if (!engine.mRenderGUI) {
            // Shared Attributes
            const float scale = 20.f;
            const float x_padding = scale;
            const float y_padding = (scale * 3.f);
            auto [width, height] = InsightEngine::Instance().GetWindowSize();
            const float pos_x = -(width / 2.f) + x_padding;
            const float pos_y = (height / 2.f) - y_padding;
            const glm::vec3 islamic_green = { 0.f, .56f, .066f };
            const glm::vec3 malachite = { 0.f, 1.f, .25f };
            static glm::vec3 color = islamic_green;
            color = (0 == (engine.FrameCount() % 180)) ? ((color == islamic_green) ? malachite : islamic_green) : color;

            // Text Attribute
            std::ostringstream render_text;
            render_text << "FPS: " << std::fixed << std::setprecision(0) << 1 / engine.mDeltaTime << '\n';
            render_text << "Delta Time: " << std::fixed << std::setprecision(6) << engine.mDeltaTime << '\n';

            // Render Text
            Times_New_Roman_font.renderText(render_text.str(), pos_x, pos_y, scale, color);
        }

        // Double font animation
        if (mShowTextAnimation)
            Text::drawTextAnimation("  Welcome To \n Insight Engine,", "Enjoy your stay!", delta_time, Times_New_Roman_font, Brush_Script_font);

        // if using ImGui, unbind fb at the end of draw
        if (engine.mRenderGUI) mFramebuffer->Unbind();
    }

    void ISGraphics::cleanup() {
        Mesh::cleanupMeshes(meshes); // delete array and buffers
    }

    void ISGraphics::initTextures(const std::string& filepath, Image& image) {
        // get asset manager
        InsightEngine& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        int width, height, channels;
        // load texture
        uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0); 

        // error checking
        if (!data) {
            IS_CORE_ERROR("Failed to load image: {}", filepath.empty() ? "No filepath provided!" : filepath);
            return;
        }

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // for different png formats
        GLuint format = GL_RGBA;
        if (channels == 1) {
            format = GL_RED;
        }
        else if (channels == 2) {
            format = GL_RG;
        }
        else if (channels == 3) {
            format = GL_RGB;
        }
        else if (channels == 4) {
            format = GL_RGBA;
        }

        // bind texture
        GLuint textureID;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // create texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            IS_CORE_DEBUG("OpenGL error: {}", err);
        }

        // Use stbi_image_free instead of delete[]
        stbi_image_free(data);  

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // unbind
        glBindTexture(GL_TEXTURE_2D, 0);

        // update asset members
        image.width = width;
        image.height = height;
        image.channels = channels;
        image.mFileName = std::filesystem::path(filepath).filename().string();
        image.texture_id = textureID;
    }

    GLuint ISGraphics::GetScreenTexture() { return mFramebuffer->GetColorAttachment(); }

    void ISGraphics::ResizeFramebuffer(GLuint width, GLuint height) { mFramebuffer->Resize(width, height); }

    void ISGraphics::DrawOutLine(Sprite const& sprite, std::tuple<float, float, float> const& color)
    {
        // Define the four corners of the sprite's bounding box (rectangular shape) relative to its center
        Vector2D TL{ 0.f - (sprite.model_TRS.scaling.x / 2.f), 0.f + (sprite.model_TRS.scaling.y / 2.f) };
        Vector2D TR{ 0.f + (sprite.model_TRS.scaling.x / 2.f), 0.f + (sprite.model_TRS.scaling.y / 2.f) };
        Vector2D BR{ 0.f + (sprite.model_TRS.scaling.x / 2.f), 0.f - (sprite.model_TRS.scaling.y / 2.f) };
        Vector2D BL{ 0.f - (sprite.model_TRS.scaling.x / 2.f), 0.f - (sprite.model_TRS.scaling.y / 2.f) };
        
        // Transform the corners using the sprite's transformation (model_TRS) to account for any rotation or translation
        sprite.model_TRS.getTransformedPoint(TL);
        sprite.model_TRS.getTransformedPoint(TR);
        sprite.model_TRS.getTransformedPoint(BR);
        sprite.model_TRS.getTransformedPoint(BL);

        std::array<Vector2D, 4> vertices { TL, TR, BR, BL };

        // Iterate through the vertices and draw lines to create the outline
        for (size_t i = 0; i < vertices.size(); i++)
        {
            Vector2D va = vertices[i];
            Vector2D vb = vertices[(i + 1) % vertices.size()]; // modules by the size of the vector to avoid going out of the range

            // Determine whether to draw a horizontal or vertical line based on the vertex index
            if (!(i % 2)) {
                sprite.drawDebugLine(va, vb, color, sprite.model_TRS.scaling.x, sprite.model_TRS.rotation);
            }
            else {
                sprite.drawDebugLine(va, vb, color, sprite.model_TRS.scaling.y, sprite.model_TRS.rotation - 90.f);
            }
        }
    }
} // end namespace IS
