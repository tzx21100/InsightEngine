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
#include <stb_image.h>

namespace IS {
    /// Static objects ///
    std::vector<Image> ISGraphics::textures;
    std::vector<Sprite::instanceData> ISGraphics::quadInstances;
    Camera ISGraphics::camera{};

    // Sprites (models) to render
    std::vector<Sprite> ISGraphics::sprites;
    // Animation objects
    Animation ISGraphics::idle_ani;
    Animation ISGraphics::walking_ani;
    Animation ISGraphics::ice_cream_truck_ani;
    // Shaders
    Shader ISGraphics::mesh_shader_pgm;
    Shader ISGraphics::mesh_inst_shader_pgm;
    Shader ISGraphics::text_shader_pgm;
    // Mesh vector
    std::vector<Mesh> ISGraphics::meshes;
    // Text objects
    GLuint font_texture;
    Text ISGraphics::cascadia_text;
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
        text_shader_pgm.setupTextShaders();
        
        walking_ani.initAnimation(1, 4, 1.f); // init 3 animations
        idle_ani.initAnimation(1, 8, 3.f);
        ice_cream_truck_ani.initAnimation(1, 6, 2.f);
        Text::initText("Assets/Fonts/Cascadia.ttf", text_shader_pgm); // init text system

        Framebuffer::FramebufferProps props{ 0, 0, static_cast<GLuint>(width), static_cast<GLuint>(height) }; // create framebuffer
        mFramebuffer = std::make_shared<Framebuffer>(props);
    }

    std::string ISGraphics::GetName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
    }

    void ISGraphics::Update(float delta_time) {
        InsightEngine& engine = InsightEngine::Instance(); // get engine instance

        // update animations
        /*idle_ani.updateAnimation(delta_time);
        walking_ani.updateAnimation(delta_time);
        ice_cream_truck_ani.updateAnimation(delta_time);*/
        
        // fill up instancing vector
        quadInstances.clear();
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

            if (sprite.primitive_type == GL_TRIANGLE_STRIP) {
                Sprite::instanceData instData;
                instData.model_to_ndc_xform = ISMtx33ToGlmMat3(sprite.model_TRS.mdl_to_ndc_xform);
                if (sprite.img.texture_id == 0) { // no texture
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

                quadInstances.push_back(instData);
            }
        }

        // draw
        Draw(delta_time);
    }

    void ISGraphics::Draw([[maybe_unused]] float delta_time) {
        InsightEngine& engine = InsightEngine::Instance(); // get engine instance
        if (engine.mUsingGUI)
            mFramebuffer->Bind(); // bind fb

        if (auto const& window = engine.GetSystem<WindowSystem>("Window"); !engine.mUsingGUI) {
            auto const& [width, height] = window->IsFullScreen() ? window->GetMonitorSize() : window->GetWindowSize();
            glViewport(0, 0, width, height);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        // Bind the instance VBO
        glBindBuffer(GL_ARRAY_BUFFER, meshes[4].instance_vbo_ID);
        // Upload the quadInstances data to the GPU
        Sprite::instanceData* buffer = reinterpret_cast<Sprite::instanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

        if (buffer) {
            // Copy the instance data to the mapped buffer
            std::memcpy(buffer, quadInstances.data(), quadInstances.size() * sizeof(Sprite::instanceData));
            
            // Unmap the buffer
            if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
                // Handle the case where unmap was not successful
                std::cerr << "Failed to unmap the buffer." << std::endl;
            }
        }
        else {
            // Handle the case where mapping the buffer was not successful
            std::cerr << "Failed to map the buffer for writing." << std::endl;
        }

        glUseProgram(mesh_inst_shader_pgm.getHandle());
        glBindVertexArray(meshes[4].vao_ID);

        
        std::vector<int> tex_array_index_vect;
        for (auto const& texture : textures) {
            glBindTextureUnit(texture.texture_index, texture.texture_id);
            tex_array_index_vect.emplace_back(texture.texture_index);
        }

        auto tex_arr_uniform = glGetUniformLocation(mesh_inst_shader_pgm.getHandle(), "uTex2d");
        if (tex_arr_uniform >= 0)
            glUniform1iv(tex_arr_uniform, static_cast<int>(tex_array_index_vect.size()), &tex_array_index_vect[0]);
        else
            std::cout << "uTex2d Uniform not found" << std::endl;
        

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, meshes[4].draw_count, static_cast<GLsizei>(quadInstances.size()));


        for (auto& entity : mEntities) { // for each intentity
            // get sprite and transform components
            auto& sprite = engine.GetComponent<Sprite>(entity);
            //auto& trans = engine.GetComponent<Transform>(entity);

            //// update sprite's transform
            //sprite.followTransform(trans);
            //sprite.transform();

            //Mesh::InstanceData instanceData;
            //instanceData.modelXformMatrix = sprite.model_TRS.mdl_to_ndc_xform;
            //instanceData.color = sprite.color;
            //instanceData.texIndex = sprite.texture; // Set the appropriate texture index

            //// Add the instance data to the array
            //quadInstances.push_back(instanceData);

            // for each type
            switch (sprite.primitive_type) {
            case GL_TRIANGLE_STRIP: // quads
                //if (sprite.name == "player_sprite") { // if drawing player
                //    // swap animations based on index
                //    if (sprite.animation_index == 0) sprite.drawAnimation(meshes[0], mesh_shader_pgm, idle_ani, sprite.texture);
                //    else sprite.drawAnimation(meshes[0], mesh_shader_pgm, walking_ani, sprite.texture);
                //}
                //else if (sprite.name == "ice_cream_truck") { // if drawing truck
                //    sprite.drawAnimation(meshes[0], mesh_shader_pgm, ice_cream_truck_ani, sprite.texture); // draw animation
                //}
                //else { // every other textured box
                //    sprite.drawSprite(meshes[0], mesh_shader_pgm, sprite.texture);
                //}
                //break;
            case GL_POINTS: // points
                sprite.drawSprite(meshes[1], mesh_shader_pgm, sprite.img.texture_id);
                break;
            case GL_LINES: // lines
                sprite.drawSprite(meshes[2], mesh_shader_pgm, sprite.img.texture_id);
                break;
            case GL_TRIANGLE_FAN: // circle
                sprite.drawSprite(meshes[3], mesh_shader_pgm, sprite.img.texture_id);
                break;
            }
            if (engine.HasComponent<RigidBody>(entity)) { // for sprites with rigidBody
                auto& body = engine.GetComponent<RigidBody>(entity);
                Physics::DrawOutLine(body, sprite);
            }

        }

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
            Text::renderText(text_shader_pgm, render_text.str(), pos_x, pos_y, scale, color);
        }

        Text::drawTextAnimation("  Welcome To \nInsight Engine,", "Enjoy your stay!", delta_time, text_shader_pgm);

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



} // end namespace IS
