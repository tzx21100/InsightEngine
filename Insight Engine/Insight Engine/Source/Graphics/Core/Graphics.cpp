
/*!
 * \file Graphics.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
 * \date 25-11-2023
 * \brief
 * This source file defines the implementation for class ISGraphics which
 * encapsulates the functionalities of a Graphics Engine.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Graphics.h"
#include "Graphics/System/Light.h"
#include "Physics/Dynamics/Body.h"
#include "Physics/Collision/Collider.h"
#include "Physics/System/Physics.h"

#include <stb_image.h>

namespace IS {
    void ClearOpenGLError() { while (glGetError() != GL_NO_ERROR); }
    bool GotError() { return glGetError() != GL_NO_ERROR; }

    /// Static objects ///

    // Frame Buffer
    std::shared_ptr<Framebuffer> ISGraphics::mFramebuffer;

    Framebuffer ISGraphics::mShaderFrameBuffer;

    // Meshes vector
    std::vector<Mesh> ISGraphics::meshes;

    // Shaders
    Shader ISGraphics::fb_shader_pgm;

    Shader ISGraphics::main_quad_shader;

    Shader ISGraphics::quad_shader_pgm;
    Shader ISGraphics::non_quad_shader_pgm;
    Shader ISGraphics::glitched_quad_shader_pgm;
    Shader ISGraphics::quad_border_shader_pgm;
    Shader ISGraphics::light_shader_pgm;
    Shader ISGraphics::videoShader;

    // Texture vector
    std::vector<Image> ISGraphics::textures;

    // instance data containers
    std::multiset<Sprite::instanceData, Layering::GfxLayerComparator> ISGraphics::layeredQuadInstances;
    std::vector<Sprite::nonQuadInstanceData> ISGraphics::lineInstances;
    std::vector<Sprite::nonQuadInstanceData> ISGraphics::circleInstances;
    std::vector<Sprite::instanceData> ISGraphics::lightInstances;
    std::vector<float> ISGraphics::lightRadius;

    // Editor and entity camera
    // Camera ISGraphics::cameras[2];
    Camera3D ISGraphics::cameras3D[2];
    
    // Text Objects
    std::unordered_map<std::string, Text> ISGraphics::mTexts;

    bool ISGraphics::mGlitched = false;
    bool ISGraphics::mLightsOn = true;
    bool ISGraphics::mDisplayFPS = false;

    VideoPlayer ISGraphics::videoplayer;
    std::vector<VideoPlayer> ISGraphics::videos;


    // Layering
    std::vector<Layering> ISGraphics::mLayers;


    void ISGraphics::Initialize() {
        ISGraphics::Instance().Subscribe(MessageType::SpriteAdded);
        ISGraphics::Instance().Subscribe(MessageType::SpriteRemoved);
        ISGraphics::ClearLayers();

        glClearColor(0.f, 0.f, 0.f, 0.f); // set background to black

        int width, height;
        InsightEngine::Instance().GetWindowSize(width, height);
        glViewport(0, 0, width, height); // set viewport to window size

        // init graphics systems
        Mesh::initMeshes(meshes); // init 3 meshes
        
        // init quad shader
        Shader::compileAllShaders();
        Shader::setMainQuadShader(quad_shader_pgm);
        
        // init all fonts
        InitFonts();

        // create framebuffer
        Framebuffer::FramebufferProps props{ 0, 0, static_cast<GLuint>(width), static_cast<GLuint>(height) }; 
        mFramebuffer = std::make_shared<Framebuffer>(props);

        mShaderFrameBuffer = Framebuffer(props);

        // initialize cameras
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

        //videoplayer.initVideoPlayer(0.1f, 0.1f, 0.9f, -0.9f);
        //videoplayer.loadVideo("Assets/Videos/SG_handsomest.mp4");

       /* VideoPlayer::createAndLoadVideo("Assets/Videos/SG_handsomest.mp4", 0.1f, 0.1f, -0.8f, 0.9f, true);
        VideoPlayer::createAndLoadVideo("Assets/Videos/Insight_LevelEditorVideo.mp4", 0.1f, 0.1f, -0.5f, 0.33f, true);
        VideoPlayer::createAndLoadVideo("Assets/Videos/SG_handsomest.mp4", 0.1f, 0.1f, 0.4f, -0.9f, true);
        VideoPlayer::createAndLoadVideo("Assets/Videos/Insight_LevelEditorVideo.mp4", 0.1f, 0.1f, 0.2f, 0.2f, true);
        VideoPlayer::createAndLoadVideo("Assets/Videos/SG_handsomest.mp4", 0.1f, 0.1f, -0.7f, -0.3f, true);
        VideoPlayer::createAndLoadVideo("Assets/Videos/Insight_LevelEditorVideo.mp4", 0.1f, 0.1f, 0.6f, 0.5f, true);*/
    }

    std::string ISGraphics::GetName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
        if (msg.GetType() == MessageType::SpriteAdded) {
          auto& spr=InsightEngine::Instance().GetComponent<Sprite>(msg.GetInt());
            AddEntityToLayer(spr.layer, msg.GetInt());
           
        }
        if (msg.GetType() == MessageType::SpriteRemoved) {
            RemoveEntityFromLayer(msg.int_value);
        }
    }

    void ISGraphics::Update(float delta_time) {
        // macro to catch graphics error
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            IS_CORE_ERROR("OpenGL Error: {}", error);
        }

        InsightEngine& engine = InsightEngine::Instance(); // get engine instance

    #ifdef USING_IMGUI

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

                mFramebuffer->Unbind();
            }
        }
    #endif // USING_IMGUI


            // for each entity
        for (int i = 0; i < static_cast<int>(mLayers.size()); i++)
        {
            Layering& layers = mLayers[i];
            if (layers.mLayerActive == false) { 
                continue; 
            }
            for (auto& entity : layers.mLayerEntities) {
                // get sprite and transform components
                auto& sprite = engine.GetComponent<Sprite>(entity);
                auto& trans = engine.GetComponent<Transform>(entity);
                if (&trans == nullptr) { continue; }

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
                    //if (Sprite::layersToIgnore.find(sprite.layer) == Sprite::layersToIgnore.end() && sprite.toRender) {
                    // empty instance data
                    Sprite::instanceData instData;

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
                        instData.color = sprite.color;
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

                    if (engine.HasComponent<Light>(entity) && mLightsOn)
                    {
                        auto& light = engine.GetComponent<Light>(entity);
                        light.FollowTransform(trans.world_position);
                        light.draw(static_cast<float>(entity));
                    }
                }

                // Debug draw
                // if they have a collider component
                if (engine.HasComponent<Collider>(entity))
                {
                    auto& collider = engine.GetComponent<Collider>(entity);
                    // draw their outline if activated
                    Physics::DrawOutLine(collider);
                }
                // if they have a collider component
                if (engine.HasComponent<RigidBody>(entity))
                {
                    auto& body = engine.GetComponent<RigidBody>(entity);
                    // draw their outline if activated
                    if (Physics::mShowVelocity) Sprite::drawDebugLine(body.mPosition, body.mPosition + body.mVelocity, { 1.f, 0.f, 0.f });
                }
            }
        }

        // update active camera
        cameras3D[Camera3D::mActiveCamera].Update();

        // ShaderEffect::shader_effect_update(delta_time); // not working yet

        videoplayer.update(delta_time);
    }

    void ISGraphics::Draw([[maybe_unused]] float delta_time) {
        // get engine instance
        InsightEngine& engine = InsightEngine::Instance();
        
        // loading fb texture onto quad
        //mFramebuffer->Bind();

         //mLightsOn = false; //// TESTINGGGGGGGGGGGGGGGGGGGGGGGGGGGGG

        if (mLightsOn)
        {
            mShaderFrameBuffer.Bind();
            // set clear color
            GL_CALL(glClearColor(0.f, 0.f, 0.f, 0.f));

            // clear color buffer
            glClear(GL_COLOR_BUFFER_BIT);

            videoplayer.render(); // render video before quads (M5 presentation duct tape)
            VideoPlayer::playVideos(delta_time);

            // quads will be drawn first
            Sprite::draw_instanced_quads();

            mShaderFrameBuffer.Unbind();
        }

        
        // bind fb
        if (engine.mRenderGUI)
        {
            mFramebuffer->Bind();
        }

        if (!mLightsOn)
        {
            // set clear color
            glClearColor(0.f, 0.f, 0.f, 0.f);

            // clear color buffer
            glClear(GL_COLOR_BUFFER_BIT);

            /// get width and height, set viewport size
            if (!engine.mRenderGUI) {
                int width, height;
                engine.GetWindowSize(width, height);
                glViewport(0, 0, width, height);
            }

            

            // quads will be drawn first
            if (mGlitched) // glitch effect
                Sprite::draw_instanced_glitched_quads();
            else
                Sprite::draw_instanced_quads();

            videoplayer.render(); // render video before quads (M5 presentation duct tape)
            VideoPlayer::playVideos(delta_time);
        }
        else
        {
            // set clear color
            glClearColor(0.f, 0.f, 0.f, 0.f);

            // clear color buffer
            glClear(GL_COLOR_BUFFER_BIT);
            /// get width and height, set viewport size
            if (!engine.mRenderGUI) {
                int width, height;
                engine.GetWindowSize(width, height);
                glViewport(0, 0, width, height);
            }
            // render lighting
            Sprite::draw_lights();
        }

    #ifdef USING_IMGUI
        if (!engine.mRuntime)
        {
            setLineWidth(3.f);
            Sprite::draw_picked_entity_border();
            setLineWidth(2.f);
        }
    #endif // USING_IMGUI


        // followed by debugging circles and lines
        Sprite::draw_instanced_circles();
        Sprite::draw_instanced_lines();

        // Render text when GUI is disabled
#ifdef USING_IMGUI
        if (!engine.mRenderGUI)
        {
        }
#endif // !USING_IMGUI
            // Shared Attributes
        // const float scale = 20.f;
        //const float x_padding = scale;
        //const float y_padding = (scale * 3.f);
        int width, height;
        engine.GetWindowSize(width, height);
        // const float pos_x = .5f;
        // const float pos_y = .5f;
        const glm::vec4 islamic_green = { 0.f, .56f, .066f, 1.f };
        const glm::vec4 malachite = { 0.f, 1.f, .25f, 1.f };
        static glm::vec4 color = islamic_green;
        color = (0 == (engine.FrameCount() % 180)) ? ((color == islamic_green) ? malachite : islamic_green) : color;

        // Text Attribute
        std::ostringstream render_text;
        render_text << "FPS: " << std::fixed << std::setprecision(2) << 1 / engine.mDeltaTime << "\n";
        //render_text << "Delta Time: " << std::fixed << std::setprecision(4) << engine.mDeltaTime << '\n';

        // Render Text

        if (mDisplayFPS) {
            

            mTexts["EBGaramond"].addTextRenderCall(render_text.str(), 0.04f, 0.98f, 8.f, color);
        }
        //North_Forest_font.renderText(render_text.str(), pos_x, pos_y, scale, color);
        
        // render all text
        Text::renderAllText(mTexts);

        // first font renders everything and clears. the rest no more

        // for (auto& [font, text] : mTexts)
        // {
        //     text.renderAllText();
        // }

        //mFramebuffer->Unbind();
        //if (!engine.mRenderGUI)
        //{
        //    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        //    glClear(GL_COLOR_BUFFER_BIT);
        //    fb_shader_pgm.use();
        //    glBindVertexArray(meshes[5].vao_ID);
        //    glBindTexture(GL_TEXTURE_2D, mFramebuffer->GetColorAttachment());	// use the color attachment texture as the texture of the quad plane
        //    glDrawArrays(GL_TRIANGLES, 0, 6);
        //    fb_shader_pgm.unUse();
        //}

        // if using ImGui, unbind fb at the end of draw
        if (engine.mRenderGUI)
        {
            mFramebuffer->Unbind();
        }
    }

    void ISGraphics::cleanup()
    {
        Mesh::cleanupMeshes(meshes); // delete array and buffers
    }

    ImageData ISGraphics::loadImageData(const std::string& filepath) {
        // load texture
        int width{}, height{}, channels{};
        uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        ImageData img_data;
        img_data.width = width; img_data.height = height; img_data.channels = channels;
        img_data.data = data;
        return img_data;
    }

    void ISGraphics::initTextures(const std::string& filepath, Image& image , ImageData img_data)
    {

        // get asset manager
        InsightEngine& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");
        int width{img_data.width}, height{img_data.height}, channels{img_data.channels};
        // load texture
        uint8_t* data = img_data.data;

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

        // Set texture wrapping parameters to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Use stbi_image_free instead of delete[]
        stbi_image_free(data);  

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

    void ISGraphics::initFastTextures(const std::string& filepath, Image& image)
    {
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

        // Set texture wrapping parameters to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Use stbi_image_free instead of delete[]
        stbi_image_free(data);

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


    void ISGraphics::InitFonts()
    {
        Text temp;
        mTexts.insert({ "EBGaramond", temp });
        mTexts.insert({ "Poiret_One_Regular", temp });
        mTexts.insert({ "Cinzel_SemiBold", temp });
        mTexts.insert({ "Semplicita_Light", temp });

        mTexts["EBGaramond"].initText("Assets/Fonts/EBGaramond-VariableFont_wght.ttf");
        mTexts["Poiret_One_Regular"].initText("Assets/Fonts/PoiretOne-Regular.ttf");
        mTexts["Cinzel_SemiBold"].initText("Assets/Fonts/Cinzel-SemiBold.ttf");
        mTexts["Semplicita_Light"].initText("Assets/Fonts/Semplicita-Light.otf");

        // init text objects
        //Times_New_Roman_font.initText("Assets/Fonts/EBGaramond-VariableFont_wght.ttf");
        ////North_Forest_font.initText("Assets/Fonts/NORTH FOREST.ttf");
        //Poreit_One_Regular_font.initText("Assets/Fonts/PoiretOne-Regular.ttf");
    }

    GLuint ISGraphics::GetScreenTexture() { return mFramebuffer->GetColorAttachment(); }

    void ISGraphics::ResizeFramebuffer(GLuint width, GLuint height) { mFramebuffer->Resize(width, height); }


    //delete textures
    void ISGraphics::deleteTexture(Image& image) {
        if (image.texture_id != 0) {
            glDeleteTextures(1, &image.texture_id);
            image.texture_id = 0; // Reset the texture ID to indicate that the texture has been deleted
        }
    }

    void ISGraphics::Shutdown()
    {
        VideoPlayer::unloadVideos();
        mFramebuffer->Destroy();
        mShaderFrameBuffer.Destroy();
    }

} // end namespace IS
