//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include "Asset.h"

#include <vector>
#include <array>
#include <string>
#include <stb_image.h>

namespace IS {

    std::vector<Sprite> ISGraphics::sprites;
    Texture ISGraphics::placeholder_tex;
    Texture ISGraphics::animation1;
    Animation ISGraphics::idle_ani;
    std::shared_ptr<Framebuffer> ISGraphics::framebuffer;
    Shader ISGraphics::mesh_shader_pgm;
    std::vector<Mesh> ISGraphics::meshes;

    void ISGraphics::Initialize() {
        // Initialize entry points to OpenGL functions and extensions
        if (GLenum err = glewInit(); GLEW_OK != err) {
            std::ostringstream oss;
            oss << glewGetErrorString(err);
            IS_CORE_ERROR("Unable Unable to initialize GLEW - error: {} - abort program", oss.str());
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

        glClearColor(1.f, 1.f, 1.f, 1.f);

        glViewport(0, 0, WIDTH, HEIGHT);
<<<<<<< Updated upstream
        //placeholder_tex = initTextures("Assets/placeholder_background.png");
        initMeshes();
        initSprites();
=======
        placeholder_tex.init_texture("Assets/placeholder_background.png");
        animation1.init_texture("Assets/player_idle.png");
        initMeshes();
        // initSprites();
        idle_ani.initAnimation(1, 8, 3.f);
>>>>>>> Stashed changes

        setupShaders();

        Framebuffer::FramebufferProps props{ 0, 0, WIDTH, HEIGHT };
        framebuffer = std::make_shared<Framebuffer>(props);
    }

    void ISGraphics::Update(float delta_time) {
<<<<<<< Updated upstream
        glClearColor(1.f, 1.f, 1.f, 1.f);
=======
        idle_ani.updateAnimation(delta_time);
>>>>>>> Stashed changes
        Draw(delta_time);
    }

    std::string ISGraphics::getName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
    }

    void ISGraphics::Draw(float delta_time) {
        framebuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT);

        // Render scene
        // Loop through all entities
        // find sprite dimensions and model type (box,line,triangle,circle)??
        // get the transform
        // render onto screen from ndc to world
        for (auto& entity : mEntities) {
            auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entity);
            auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
            sprite.followTransform(trans);
            sprite.transform(delta_time);
            GLuint texture{};
            switch (sprite.primitive_type) {
<<<<<<< Updated upstream
            case GL_TRIANGLES:
                sprite.transform(delta_time);
                sprite.drawSpecial(meshes[0], mesh_shader_pgm,sprite.texture);
                break;
            case GL_POINTS:
                sprite.transform(delta_time);
                sprite.drawSpecial(meshes[1], mesh_shader_pgm, sprite.texture);
                break;
            case GL_LINES:
                sprite.transform(delta_time);
                sprite.drawSpecial(meshes[2], mesh_shader_pgm, sprite.texture);
                break;
            case GL_TRIANGLE_FAN:
                sprite.transform(delta_time);
                sprite.drawSpecial(meshes[3], mesh_shader_pgm, sprite.texture);
=======
            case GL_TRIANGLE_STRIP:
                if (sprite.name == "textured_box") {
                    texture = animation1.tex_ID;
                    sprite.drawAnimation(meshes[0], mesh_shader_pgm, idle_ani, texture);
;                }
                else {
                    glBindVertexArray(meshes[0].vao_ID);
                    sprite.drawSpecial(meshes[0], mesh_shader_pgm);
                }
                break;
            case GL_POINTS:
                sprite.drawSpecial(meshes[1], mesh_shader_pgm);
                break;
            case GL_LINES:
                sprite.drawSpecial(meshes[2], mesh_shader_pgm);
                break;
            case GL_TRIANGLE_FAN:
                sprite.drawSpecial(meshes[3], mesh_shader_pgm);
>>>>>>> Stashed changes
                break;
            }
        }

        framebuffer->Unbind();
    
    }

    void ISGraphics::cleanup() {
        //Mesh::cleanup4Meshes();
        glDeleteTextures(1, &placeholder_tex.tex_ID);
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

<<<<<<< Updated upstream


    void ISGraphics::initSprites() { // mainly for M1
        Sprite test_box_sprite("Box", GL_TRIANGLES);
        Sprite test_points_sprite("Points", GL_POINTS);
        Sprite test_lines_sprite("Lines", GL_LINES);
        Sprite test_circle_sprite("Circle", GL_TRIANGLE_FAN);

        /* test_box_model.setupVAO();
         test_box_model.setupShaders();*/
        test_box_sprite.model_TRS.world_position = glm::vec2(0.f, -640.f); // somewhere top-left (initially)
        test_box_sprite.model_TRS.scaling = glm::vec2(400.f, 200.f); // max scaling (fit entire screen x: 1280, y: 720)
        //test_box_model.orientation.y = 30.f;

        /*test_points_model.setupSpecialVAO();
        test_points_model.setupShaders();*/
        test_points_sprite.model_TRS.scaling = glm::vec2(1066.67f, 600.f);
        test_points_sprite.model_TRS.orientation.y = 10.f;

        /*test_lines_model.setupSpecialVAO();
        test_lines_model.setupShaders();*/
        test_lines_sprite.model_TRS.scaling = glm::vec2(1000.00f, 500.f);
        test_lines_sprite.model_TRS.orientation.y = -10.f;

        /*test_circle_model.setupSpecialVAO();
        test_circle_model.setupShaders();*/
        test_circle_sprite.model_TRS.scaling = glm::vec2(100.f, 100.f);
        //test_circle_model.orientation.y = -30.f;

        //based on this i create the enum
        sprites.emplace_back(test_box_sprite); //ur box is 0
        sprites.emplace_back(test_points_sprite);//ur point is 1
        sprites.emplace_back(test_lines_sprite);//ur line is 2
        sprites.emplace_back(test_circle_sprite);//ur circle is 3
    }

    GLuint ISGraphics::initTextures(Image& image) {
        int width{image.width}, height{image.height}, channels{image.channels};
        unsigned char* image_data = image.data;

        if (!image_data) {
            std::cerr << "Failed to load the texture image: " << image.file_name << std::endl;
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

        std::cout << "TEXURE: " << textureID;
        return textureID;
    }

=======
>>>>>>> Stashed changes
    void ISGraphics::setupShaders() {
        // vertex shader
        std::string vtx_shdr = R"(
            #version 450 core
            layout(location = 0) in vec2 aVertexPosition;
            layout(location = 2) in vec2 aTexCoord;
            out vec2 vTexCoord;

            uniform mat3 uModel_to_NDC;

            void main()
            {
                gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.0)), 0.0, 1.0);
                vTexCoord = aTexCoord;
            }
        )";

        // fragment shader
        std::string frag_shdr = R"(
            #version 450 core

            layout(location = 0) out vec4 fFragColor;
            uniform vec3 uColor;
            uniform sampler2D uTex2d;
            in vec2 vTexCoord; // Input variable for texture coordinates
            uniform int uTexture; // Flag to indicate whether to use texture or color

            // animation uniforms - default unless drawing animation
            uniform vec2 uFrameDim = vec2(1.0, 1.0); 
            uniform vec2 uFrameIndex = vec2(0.0, 0.0);

            void main()
            {
                if (uTexture == 0)
                {
                    fFragColor = vec4(uColor, 1.0); // Use uColor if no texture is bound
                }
                else
                {
                    //fFragColor = texture(uTex2d, vTexCoord); // Multiply texture color with uColor
                    fFragColor = texture(uTex2d, vec2(vTexCoord.x * uFrameDim.x, vTexCoord.y * uFrameDim.y) + vec2(uFrameDim.x * uFrameIndex.x, uFrameDim.y * uFrameIndex.y)); // Multiply texture color with uColor
                }
            }
        )";

        // Compile and link the shaders into a shader program
        mesh_shader_pgm.compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        mesh_shader_pgm.compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        mesh_shader_pgm.link();
        mesh_shader_pgm.validate();

        // Check if the shader program compilation and linking was successful
        if (GL_FALSE == mesh_shader_pgm.isLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << mesh_shader_pgm.getLog() << "\n";
            exit(EXIT_FAILURE);
        }
    }

    GLuint ISGraphics::getScreenTexture() {
        return framebuffer->GetColorAttachment();
    }

    void ISGraphics::resizeFramebuffer(GLuint w, GLuint h) {
        framebuffer->Resize(w, h);
    }
}