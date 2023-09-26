//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include "Asset.h"
#include "WindowSystem.h"

#include <vector>
#include <array>
#include <string>
#include <stb_image.h>

#include <ft2build.h>

namespace IS {

    std::vector<Sprite> ISGraphics::sprites;
    Animation ISGraphics::idle_ani;
    Animation ISGraphics::walking_ani;
    Animation ISGraphics::ice_cream_truck_ani;
    std::shared_ptr<Framebuffer> ISGraphics::mFramebuffer;
    Shader ISGraphics::mesh_shader_pgm;
    Shader ISGraphics::text_shader_pgm;
    std::vector<Mesh> ISGraphics::meshes;

    GLuint font_texture;
    Text ISGraphics::cascadia_text;


    void ISGraphics::Initialize() {
        glClearColor(0.f, 0.f, 0.f, 0.f);

        glViewport(0, 0, WIDTH, HEIGHT);

        Mesh::initMeshes(meshes);
        walking_ani.initAnimation(1, 4, 1.f);
        idle_ani.initAnimation(1, 8, 3.f);
        ice_cream_truck_ani.initAnimation(1, 6, 2.f);

        mesh_shader_pgm.setupSpriteShaders();

        text_shader_pgm.setupTextShaders();
        Text::initText("Assets/Fonts/Cascadia.ttf", text_shader_pgm);

        Framebuffer::FramebufferProps props{ 0, 0, WIDTH, HEIGHT };
        mFramebuffer = std::make_shared<Framebuffer>(props);
    }

    void ISGraphics::Update(float delta_time) {
        idle_ani.updateAnimation(delta_time);
        walking_ani.updateAnimation(delta_time);
        ice_cream_truck_ani.updateAnimation(delta_time);
        Draw(delta_time);
    }

    std::string ISGraphics::GetName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
    }

    void ISGraphics::Draw([[maybe_unused]] float delta_time) {
        InsightEngine& engine = InsightEngine::Instance();
        if (engine.mUsingGUI)
            mFramebuffer->Bind();

        if (auto const& window = engine.GetSystem<WindowSystem>("Window"); window->IsFullScreen() && !engine.mUsingGUI)
            glViewport(0, 0, window->GetMonitorWidth(), window->GetMonitorHeight());

        glClear(GL_COLOR_BUFFER_BIT);

        //Sprite::drawLine(Vector2D(0.f, 0.f), Vector2D(0.f, 200.f), delta_time);

        // Render scene
        // Loop through all entities
        // find sprite dimensions and model type (box,line,triangle,circle)??
        // get the transform
        // render onto screen from ndc to world
        for (auto& entity : mEntities) {
            auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entity);
            auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
            sprite.followTransform(trans);
            sprite.transform();
            switch (sprite.primitive_type) {
            case GL_TRIANGLE_STRIP:
                if (sprite.name == "textured_box") {
                    auto input = engine.GetSystem<InputManager>("Input");

                    if (sprite.current_tex_index == 0) sprite.drawAnimation(meshes[0], mesh_shader_pgm, idle_ani, sprite.texture);
                    else sprite.drawAnimation(meshes[0], mesh_shader_pgm, walking_ani, sprite.texture);
                } else if (sprite.name == "textured_box2") {
                    sprite.drawAnimation(meshes[0], mesh_shader_pgm, ice_cream_truck_ani, sprite.texture);
                } else {
                    sprite.drawSpecial(meshes[0], mesh_shader_pgm, sprite.texture);
                }
                break;
            case GL_POINTS:
                sprite.drawSpecial(meshes[1], mesh_shader_pgm, sprite.texture);
                break;
            case GL_LINES:
                sprite.drawSpecial(meshes[2], mesh_shader_pgm, sprite.texture);
                break;
            case GL_TRIANGLE_FAN:
                sprite.drawSpecial(meshes[3], mesh_shader_pgm, sprite.texture);
                break;
            }
            if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
                auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
                if (Physics::isDebugDraw) {
                    Physics::DrawOutLine(body, sprite);
                }
            }

        }

        // Render some text when GUI is disabled
        if (!engine.mUsingGUI){
            // Shared Attributes
            const float scale = 5.f;
            const float x_padding = scale;
            const float y_padding = (scale * 3.f);
            const float pos_x = -(WIDTH / 2.f) + x_padding;
            float pos_y = (HEIGHT / 2.f) - y_padding;
            const glm::vec3 islamic_green = { 0.f, .56f, .066f };
            const glm::vec3 malachite = { 0.f, 1.f, .25f };
            static glm::vec3 color = islamic_green;
            color = (!(engine.FrameCount() % 180)) ? ((color == islamic_green) ? malachite : islamic_green) : color;

            // Text Attribute
            std::ostringstream fps_text_oss;
            std::ostringstream entities_alive_text_oss;
            std::ostringstream entities_max_text_oss;
            fps_text_oss << "FPS: " << std::fixed << std::setprecision(0) << 1 / delta_time;
            entities_alive_text_oss << "Entities Alive: " << engine.EntitiesAlive();
            entities_max_text_oss.imbue(std::locale("")); // comma separated
            entities_max_text_oss << "Max Entities: " << std::fixed << MAX_ENTITIES;

            std::vector<std::string> render_texts;
            render_texts.emplace_back(fps_text_oss.str());
            render_texts.emplace_back(entities_alive_text_oss.str());
            render_texts.emplace_back(entities_max_text_oss.str());
            render_texts.emplace_back("");
            render_texts.emplace_back("General Controls");
            render_texts.emplace_back("- Press 'Tab' to toggle GUI");
            render_texts.emplace_back("- Press 'F11' to toggle fullscreen/windowed");
            render_texts.emplace_back("- Click mouse scrollwheel to spawn entity");
            render_texts.emplace_back("- Click right mouse button to spawn rigidbody entity");
            render_texts.emplace_back("");
            render_texts.emplace_back("Player Controls");
            render_texts.emplace_back("- Press 'WASD' to move in the four directions");
            render_texts.emplace_back("- Press 'Q' to rotate clockwise, 'E' to rotate counter-clockwise");
            render_texts.emplace_back("");
            render_texts.emplace_back("Physics Controls");
            render_texts.emplace_back("- Press '2' to enable draw collision boxes, '1' to disable");
            render_texts.emplace_back("- Press 'G' to enable gravity, 'F' to disable");
            render_texts.emplace_back("- Press 'Shift' + 'Space' to freeze frame, 'Space' to step frame");
            render_texts.emplace_back("");
            render_texts.emplace_back("Audio Controls");
            render_texts.emplace_back("- Press 'Z' to play sfx");
            render_texts.emplace_back("- Press 'X' to play music");
            render_texts.emplace_back("");
            render_texts.emplace_back("Scene Controls");
            render_texts.emplace_back("- Press 'J' to load testscene");
            render_texts.emplace_back("- Press 'R' to save testscene");

            // Render Text
            for (std::string const& render_text : render_texts) {
                Text::renderText(text_shader_pgm, render_text, pos_x, pos_y, scale, color);
                pos_y -= y_padding;
            }
        }


        Text::drawTextAnimation("  Welcome To \nInsight Engine,", "Enjoy your stay!", delta_time, text_shader_pgm);
        //Text::renderText(text_shader_pgm, "  Welcome To \nInsight Engine!", -130.f, 400.f, 12.f, glm::vec3(0.529f, 0.808f, 0.922f));

        if (InsightEngine::Instance().mUsingGUI)
            mFramebuffer->Unbind();

    }

    void ISGraphics::cleanup() {
        //Mesh::cleanup4Meshes();
        //glDeleteTextures(1, &placeholder_tex.tex_ID);
    }


    void ISGraphics::initTextures(const std::string& filepath, Image& image) {
        int width, height, channels;
        uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!data) {
            IS_CORE_ERROR("Failed to load image: {}", filepath.empty() ? "No filepath provided!" : filepath);
            return;
        }

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLuint format=GL_RGBA;
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

        GLuint textureID;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

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

        glBindTexture(GL_TEXTURE_2D, 0);

        image.width = width;
        image.height = height;
        image.channels = channels;
        image.mFileName = filepath;
        image.mTextureData = textureID;
    }


    GLuint ISGraphics::GetScreenTexture() { return mFramebuffer->GetColorAttachment(); }
    void ISGraphics::ResizeFramebuffer(GLuint width, GLuint height) { mFramebuffer->Resize(width, height); }

} // end namespace IS
