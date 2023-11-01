/*!
 * \file Graphics.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
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
#include "Editor.h"
#include <stb_image.h>

namespace IS {

    /// Static objects ///
    bool ISGraphics::mShowTextAnimation = true;
    std::vector<Image> ISGraphics::textures;
    std::multiset<Sprite::instanceData, Sprite::GfxLayerComparator> ISGraphics::layeredQuadInstances;
    std::vector<Sprite::nonQuadInstanceData> ISGraphics::lineInstances;
    std::vector<Sprite::nonQuadInstanceData> ISGraphics::circleInstances;
    Camera ISGraphics::cameras[2];
    
    // Sprites (models) to render
    std::vector<Sprite> ISGraphics::sprites;
    // Animation objects
    Animation ISGraphics::idle_ani;
    Animation ISGraphics::walking_ani;
    Animation ISGraphics::ice_cream_truck_ani;
    // Shaders
    Shader ISGraphics::mesh_shader_pgm;
    Shader ISGraphics::mesh_inst_shader_pgm;
    Shader ISGraphics::mesh_inst_line_shader_pgm;
    /*Shader ISGraphics::TNR_text_shader_pgm;
    Shader ISGraphics::CS_text_shader_pgm;*/

    Text ISGraphics::Times_New_Roman_font;
    Text ISGraphics::Brush_Script_font;

    // Mesh vector
    std::vector<Mesh> ISGraphics::meshes;
    // Frame Buffer
    std::shared_ptr<Framebuffer> ISGraphics::mFramebuffer;

    void ISGraphics::Initialize() {
        glClearColor(0.f, 0.f, 0.f, 0.f); // set background to white

        auto [width, height] = InsightEngine::Instance().GetWindowSize();
        glViewport(0, 0, width, height); // set viewport to window size

        // init graphics systems
        Mesh::initMeshes(meshes); // init 4 meshes
        
        mesh_shader_pgm.setupSpriteShaders(); // init 2 shaders
        mesh_inst_shader_pgm.setupInstSpriteShaders();
        mesh_inst_line_shader_pgm.setupInstLineShaders();
        Times_New_Roman_font.shader.setupTextShaders();
        Brush_Script_font.shader.setupTextShaders();
        
        walking_ani.initAnimation(1, 4, 1.f); // init 3 animations
        idle_ani.initAnimation(1, 8, 3.f);
        ice_cream_truck_ani.initAnimation(1, 6, 2.f);

        // init text system
        Times_New_Roman_font.initText("Assets/Fonts/Times-New-Roman.ttf");
        Brush_Script_font.initText("Assets/Fonts/BRUSHSCI.ttf");

        Framebuffer::FramebufferProps props{ 0, 0, static_cast<GLuint>(width), static_cast<GLuint>(height) }; // create framebuffer
        mFramebuffer = std::make_shared<Framebuffer>(props);
        std::for_each_n(cameras, 2, [width](Camera& camera)
        {
            camera.UpdateCamPos(0, 0);
            camera.UpdateCamDim(static_cast<float>(width));
        });
    }

    std::string ISGraphics::GetName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
    }

    void ISGraphics::Update(float delta_time) {
        for (int step = 0; step < InsightEngine::currentNumberOfSteps; ++step) {
            InsightEngine& engine = InsightEngine::Instance(); // get engine instance


            GLenum error;
            while ((error = glGetError()) != GL_NO_ERROR) {
                IS_CORE_ERROR("OpenGL Error: {}", error);
            }

        // update animations
        /*idle_ani.updateAnimation(delta_time);
        walking_ani.updateAnimation(delta_time);
        ice_cream_truck_ani.updateAnimation(delta_time);*/
        
        // fill up instancing vector
        layeredQuadInstances.clear();
        for (auto& entity : mEntities) { // for each intentity
            // get sprite and transform components
            auto& sprite = engine.GetComponent<Sprite>(entity);
            auto& trans = engine.GetComponent<Transform>(entity);

                // update sprite's transform
                sprite.followTransform(trans);
                sprite.transform();

                if (!sprite.anims.empty()) {
                    sprite.anims[sprite.animation_index].updateAnimation(delta_time);
                }

                /* if (sprite.primitive_type == GL_LINES || sprite.primitive_type == GL_LINE_LOOP) {
                     Sprite::nonQuadInstanceData instLineData;
                     instLineData.color = sprite.color;
                     instLineData.model_to_ndc_xform = ISMtx33ToGlmMat3(sprite.model_TRS.mdl_to_ndc_xform);
                     lineInstances.emplace_back(instLineData);
                 }*/

            if (sprite.primitive_type == GL_TRIANGLE_STRIP) {
                Sprite::instanceData instData;
                instData.model_to_ndc_xform = ISMtx33ToGlmMat3(sprite.model_TRS.mdl_to_ndc_xform);
                instData.entID = static_cast<float>(entity);
                instData.layer = sprite.layer;
                if (sprite.img.texture_id == 0) { // no texture
                    instData.color = sprite.color;
                    instData.tex_index = -1.f;
                }
                else {
                    instData.tex_index = static_cast<float>(sprite.img.texture_index);
                }
                if (!sprite.anims.empty()) {
                    instData.anim_frame_dimension = sprite.anims[sprite.animation_index].frame_dimension;
                    instData.anim_frame_index = sprite.anims[sprite.animation_index].frame_index;
                }
                // no need for else as default values of instData will stay

                layeredQuadInstances.insert(instData);
            }
        }

        // draw
        Draw(delta_time);
    }

    }

    void ISGraphics::Draw([[maybe_unused]] float delta_time) {
        InsightEngine& engine = InsightEngine::Instance(); // get engine instance
        if (engine.mUsingGUI)
            mFramebuffer->Bind(); // bind fb
        glClear(GL_COLOR_BUFFER_BIT);

        //int entityID{};
        //// Read the entityID value at the specified pixel coordinates
        //glReadBuffer(GL_COLOR_ATTACHMENT1);
        //glReadPixels(0, 0, 1, 1, GL_RED_INTEGER, GL_INT, &entityID);
        ////GLuint entityIDR = entityID.x;
        //std::cout << entityID << std::endl;


        if (auto const& window = engine.GetSystem<WindowSystem>("Window"); !engine.mUsingGUI) {
            auto const& [width, height] = window->IsFullScreen() ? window->GetMonitorSize() : window->GetWindowSize();
            glViewport(0, 0, width, height);
        }

        //Sprite::drawDebugLine({ 0.f, 0.f }, { 200.f, 0.f }, 0.f, { 1.0f, 0.0f, 0.0f });
        //Sprite::drawDebugCircle({ 0.f, 0.f }, { 500.f, 500.f }, { 0.0f, 1.0f, 0.0f });

        Sprite::draw_instanced_quads();
        Sprite::draw_instanced_circles(); // layering how?
        Sprite::draw_instanced_lines();

        // Draw outline for selected entity
        auto const editor = engine.GetSystem<Editor>("Editor");
        auto const editor_layer = editor->GetEditorLayer();
        if (!editor_layer->IsGamePanelFocused() && Camera::mActiveCamera == CAMERA_TYPE_EDITOR)
            editor_layer->RenderSelectedEntityOutline();

        // Render text when GUI is disabled
        if (!engine.mUsingGUI) {
            // Shared Attributes
            const float scale = 5.f;
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
            render_text << "FPS: " << std::fixed << std::setprecision(0) << 1 / delta_time << '\n';
            render_text << "Entities Alive: " << engine.EntitiesAlive() << '\n';
            render_text.imbue(std::locale("")); // comma separated numbers
            render_text << "Max Entities: " << std::fixed << MAX_ENTITIES << "\n\n";
            render_text << "General Controls\n"
                           "- Press 'Tab' to toggle GUI\n"
                           "- Click mouse scrollwheel to spawn entity\n"
                           "- Click right mouse button to spawn rigidbody entity\n\n";
            render_text << "Player Controls\n"
                "- Press 'WASD' to move in the four directions\n"
                "- Press 'Q' to rotate clockwise, 'E' to rotate counter-clockwise\n\n";
            render_text << "Physics Controls\n"
                "- Press '2' to enable draw collision boxes, '1' to disable\n"
                "- Press 'G' to enable gravity, 'F' to disable\n"
                "- Press 'Shift' + 'Enter' to freeze frame, 'Enter' to step frame\n\n";
            render_text << "Audio Controls\n"
                "- Press 'Z' to play sfx\n"
                "- Press 'X' to play music";

            // Render Text
            Times_New_Roman_font.renderText(render_text.str(), pos_x, pos_y, scale, color);
        }

        if (mShowTextAnimation)
            Text::drawTextAnimation("  Welcome To \n Insight Engine,", "Enjoy your stay!", delta_time, Times_New_Roman_font, Brush_Script_font);

        // if using ImGui, unbind fb at the end of draw
        if (engine.mUsingGUI)
            mFramebuffer->Unbind();
    }

    void ISGraphics::cleanup() {
        Mesh::cleanupMeshes(meshes); // delete array and buffers
    }

    void ISGraphics::initTextures(const std::string& filepath, Image& image) {
        InsightEngine& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        int width, height, channels;
        uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0); // load texture

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

        stbi_image_free(data);  // Use stbi_image_free instead of delete[]

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
        image.mFileName = filepath;
        image.texture_id = textureID;
    }

    GLuint ISGraphics::GetScreenTexture() { return mFramebuffer->GetColorAttachment(); }
    void ISGraphics::ResizeFramebuffer(GLuint width, GLuint height) { mFramebuffer->Resize(width, height); }

    void ISGraphics::DrawOutLine(Sprite const& sprite, std::tuple<float, float, float> const& color, float thickness)
    {
        Vector2D TL{ sprite.model_TRS.world_position.x - (sprite.model_TRS.scaling.x / 2.f), sprite.model_TRS.world_position.y + (sprite.model_TRS.scaling.y / 2.f) };
        Vector2D TR{ sprite.model_TRS.world_position.x + (sprite.model_TRS.scaling.x / 2.f), sprite.model_TRS.world_position.y + (sprite.model_TRS.scaling.y / 2.f) };
        Vector2D BR{ sprite.model_TRS.world_position.x + (sprite.model_TRS.scaling.x / 2.f), sprite.model_TRS.world_position.y - (sprite.model_TRS.scaling.y / 2.f) };
        Vector2D BL{ sprite.model_TRS.world_position.x - (sprite.model_TRS.scaling.x / 2.f), sprite.model_TRS.world_position.y - (sprite.model_TRS.scaling.y / 2.f) };

        std::array<Vector2D, 4> vertices { TL, TR, BR, BL };


        for (size_t i = 0; i < vertices.size(); i++)
        {
            Vector2D va = vertices[i];
            Vector2D vb = vertices[(i + 1) % vertices.size()]; // modules by the size of the vector to avoid going out of the range

            if (!(i % 2)) {
                sprite.drawDebugLine(va, vb, color, sprite.model_TRS.scaling.x);
            }
            else {
                sprite.drawDebugLine(va, vb, color, sprite.model_TRS.scaling.y);
            }
            thickness = thickness;
            //sprite.drawLine(va, vb, color, thickness);
        }
    }



} // end namespace IS
