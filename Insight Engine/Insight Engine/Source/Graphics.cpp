//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include "Asset.h"

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
        // Initialize entry points to OpenGL functions and extensions
        if (GLenum err = glewInit(); GLEW_OK != err) {
            std::ostringstream oss;
            oss << glewGetErrorString(err);
            IS_CORE_CRITICAL("Unable to initialize GLEW - error: {} - abort program", oss.str());
            std::exit(EXIT_FAILURE);
        }
        if (GLEW_VERSION_4_5) {
            std::ostringstream oss;
            oss << glewGetString(GLEW_VERSION);
            IS_CORE_INFO("Using glew version: {}", oss.str());
            IS_CORE_INFO("Driver supports OpenGL 4.5");
        } else {
            IS_CORE_ERROR("Driver doesn't support OpenGL 4.5 - abort program");
            std::exit(EXIT_FAILURE);
        }

        glClearColor(0.f, 0.f, 0.f, 0.f);

        glViewport(0, 0, WIDTH, HEIGHT);

        initMeshes();
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

        if (InsightEngine::Instance().mUsingGUI)
            mFramebuffer->Bind();
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
                    InsightEngine& engine = InsightEngine::Instance();
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
                    Physics::drawOutLine(body, sprite);
                }
            }

        }

        // Render FPS and Entities alive
        InsightEngine& engine = InsightEngine::Instance();
        std::ostringstream fps_text_oss;
        fps_text_oss << "FPS: " << std::fixed << std::setprecision(0) << 1 / delta_time;
        std::string fps_text = fps_text_oss.str();
        std::ostringstream entities_alive_text_oss;
        entities_alive_text_oss << "Entities Alive: " << engine.EntitiesAlive();
        std::string entities_alive_text = entities_alive_text_oss.str();

        float scale = 10.f;
        float pos_x = -(WIDTH / 2.f) + scale;
        float pos_y = (HEIGHT / 2.f) - (scale * 3.f);
        const glm::vec3 islamic_green = { 0.f, .56f, .066f };
        const glm::vec3 malachite = { 0.f, 1.f, .25f };
        static glm::vec3 color = islamic_green;
        color = (!(engine.FrameCount() % 180)) ? ((color == islamic_green) ? malachite : islamic_green) : color;
        Text::renderText(text_shader_pgm, fps_text, pos_x, pos_y, scale, color);
        Text::renderText(text_shader_pgm, entities_alive_text, pos_x, pos_y - (scale * 3.f), scale, color);


        //Text::drawTextAnimation("  Welcome To \nInsight Engine,", "Enjoy your stay!", delta_time, text_shader_pgm);
        //Text::renderText(text_shader_pgm, "  Welcome To \nInsight Engine!", -130.f, 400.f, 12.f, glm::vec3(0.529f, 0.808f, 0.922f));

        if (InsightEngine::Instance().mUsingGUI)
            mFramebuffer->Unbind();

    }

    void ISGraphics::cleanup() {
        //Mesh::cleanup4Meshes();
        //glDeleteTextures(1, &placeholder_tex.tex_ID);
    }

    void ISGraphics::initMeshes() {
        Mesh quad_mesh, point_mesh, line_mesh, circle_mesh;
        quad_mesh.setupQuadVAO();
        point_mesh.setupNonQuadVAO(GL_POINTS);
        line_mesh.setupNonQuadVAO(GL_LINES);
        circle_mesh.setupNonQuadVAO(GL_TRIANGLE_FAN);

        meshes.emplace_back(quad_mesh);
        meshes.emplace_back(point_mesh);
        meshes.emplace_back(line_mesh);
        meshes.emplace_back(circle_mesh);
    }

    GLuint ISGraphics::initTextures(Image& image) {

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        int width{ image.width }, height{ image.height }; // channels{ image.channels };
        unsigned char* image_data = image.data;

        if (!image_data) {
            IS_CORE_ERROR("Failed to load image: {}", image.file_name.empty() ? "No filepath provided!" : image.file_name);
            return 0; // Return 0 to indicate failure
        }


        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

        stbi_image_free(image_data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        IS_CORE_INFO("Using Texture: {}", textureID);
        return textureID;
    }

    GLuint ISGraphics::GetScreenTexture() { return mFramebuffer->GetColorAttachment(); }
    void ISGraphics::ResizeFramebuffer(GLuint width, GLuint height) { mFramebuffer->Resize(width, height); }

} // end namespace IS
