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
    GLuint ISGraphics::placeholder_tex;
    GLuint ISGraphics::fbo_id;
    GLuint ISGraphics::tex_id; // ?
    GLuint ISGraphics::vao_id;
    Shader ISGraphics::shader_pgm;
    Shader ISGraphics::mesh_shader_pgm;
    Mesh ISGraphics::quad_mesh;

    void ISGraphics::Initialize() {
        glClearColor(0.2f, 0.2f, 0.2f, 1.f); // set color buffer to dark grey

        glViewport(0, 0, WIDTH, HEIGHT);
        placeholder_tex = initTextures("Assets/placeholder_background.png");

        initSprites();
        //Mesh::init4Meshes();
        quad_mesh.setupQuadVAO();
        setupScreenFBO();
        //setupQuadVAO();
        setupScreenShaders();
        setupShaders();
    }

    void ISGraphics::Update(float delta_time) {
        for (Sprite& sprite : sprites) {
            sprite.transform(delta_time);
        }
        Draw();
    }

    std::string ISGraphics::getName() { return "Graphics"; };

    void ISGraphics::HandleMessage(const Message& msg) {
        if (msg.GetType() == MessageType::DebugInfo) {
            IS_CORE_INFO("Handling Graphics");
        }
    }

    void ISGraphics::Draw() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
        glClearColor(0.2f, 0.2f, 0.2f, 1.f); // set color buffer to dark grey

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT);

        // Render scene
        // Loop through all entities
        // find sprite dimensions and model type (box,line,triangle,circle)??
        // get the transform
        // render onto screen from ndc to world




        for (auto& entity : mEntities) {
            auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entity);
            //auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
            if (sprite.primitive_type == GL_TRIANGLES) {
                sprite.transform(1 / 60);
                //ISModel ModelCopy = models[static_cast<int>(ModelType::Box)];
                //set the model copy's transform
                //ModelCopy.draw();
                sprite.model_TRS.world_position = glm::vec2(0.f, -640.f); // somewhere top-left (initially)
                sprite.model_TRS.scaling = glm::vec2(400.f, 200.f); // max scaling (fit entire screen x: 1280, y: 720)
                sprite.drawSpecial(quad_mesh, mesh_shader_pgm);
            }
        }

        //for (Sprite &sprite : sprites) {
            //if (sprite.drawing) {
            //    Mesh meshUsed{};
            //    switch (sprite.primitive_type) {
            //    case GL_TRIANGLES:
            //        meshUsed = quad_mesh;
            //        break;
            //    case GL_POINTS:
            //        meshUsed = points_mesh;
            //        break;
            //    case GL_LINES:
            //        meshUsed = lines_mesh;
            //        break;
            //    case GL_TRIANGLE_FAN:
            //        meshUsed = circle_mesh;
            //        break;
            //    }
            //    //sprite.drawSpecial(meshUsed, mesh_shader_pgm);
            //} 
        //}
        //std::cout << "pri type: " << sprites[0].primitive_type << "draw cnt: " << quad_mesh.draw_count << std::endl;

        //sprites[0].drawSpecial(quad_mesh, mesh_shader_pgm, placeholder_tex);

        // switch back to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST);

        shader_pgm.use();
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glBindVertexArray(vao_id);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader_pgm.unUse();
    }

    void ISGraphics::cleanup() {
        //Mesh::cleanup4Meshes();

        glDeleteFramebuffers(1, &fbo_id);
        glDeleteVertexArrays(1, &vao_id);
        glDeleteTextures(1, &tex_id);
        glDeleteTextures(1, &placeholder_tex);
    }

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

    void ISGraphics::setupScreenFBO() {
        GLuint fbo_hdl, tex_hdl, rbo_hdl;

        // Create framebuffer object
        glGenFramebuffers(1, &fbo_hdl);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_hdl);

        // Create texture object (color attachment)
        glGenTextures(1, &tex_hdl);
        glBindTexture(GL_TEXTURE_2D, tex_hdl);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        // Allow rescaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_hdl, 0);

        // Create renderbuffer object (depth attachment)
        glGenRenderbuffers(1, &rbo_hdl);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_hdl);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
        glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_hdl);

        // Validate whether framebuffer object is complete
        GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        IS_CORE_ASSERT_MESG((fbo_status == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is incomplete!");

        fbo_id = fbo_hdl;
        tex_id = tex_hdl;
    }

    void ISGraphics::setupScreenShaders() {
        // vertex shader
        std::string vtx_shdr = R"(
            #version 450 core
            layout (location = 0) in vec2 aVertexPosition;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 0) out vec2 vTexCoord;

            void main()
            {
                gl_Position = vec4(aVertexPosition, 0.0, 1.0);
                vTexCoord = aTexCoord;
            }
        )";

        // fragment shader
        std::string frag_shdr = R"(
            #version 450 core

            layout (location = 0) in vec2 vTexCoord;
            layout (location = 0) out vec4 fFragColor;
            uniform sampler2D uScreenTex;
            
            void main() {
                fFragColor = texture(uScreenTex, vTexCoord);
            }
        )";

        // Compile and link the shaders into a shader program
        shader_pgm.compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        shader_pgm.compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        shader_pgm.link();
        shader_pgm.validate();

        // Check if the shader program compilation and linking was successful
        IS_CORE_ASSERT_MESG(GL_TRUE == shader_pgm.isLinked(), "Unable to compile/link/validate shader programs {}", shader_pgm.getLog());

    }

    GLuint ISGraphics::initTextures(std::string const& image_path) {
        int width{}, height{}, channels{};
        unsigned char* image_data = stbi_load(image_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (!image_data) {
            std::cerr << "Failed to load the texture image: " << image_path << std::endl;
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

        return textureID;
    }

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

            void main()
            {
                if (uTexture == 0)
                {
                    fFragColor = vec4(uColor, 1.0); // Use uColor if no texture is bound
                }
                else
                {
                    fFragColor = texture(uTex2d, vTexCoord); // Multiply texture color with uColor
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
}