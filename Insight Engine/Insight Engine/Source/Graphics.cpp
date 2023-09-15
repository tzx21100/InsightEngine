//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include "Asset.h"

#include <vector>
#include <array>
#include <string>
#include <stb_image.h>

namespace IS {

    std::vector<ISGraphics::ISModel> ISGraphics::models;
    GLuint ISGraphics::placeholder_tex;
    GLuint ISGraphics::fbo_id;
    GLuint ISGraphics::tex_id;
    GLuint ISGraphics::vao_id;
    Shader ISGraphics::shader_pgm;

    void ISGraphics::Update(float deltaTime) { update(deltaTime); Draw(); };
    std::string ISGraphics::getName() { return "Graphics"; };
    void ISGraphics::Initialize() { init(); };
    void ISGraphics::HandleMessage(const Message& message) {};

    void ISGraphics::init() {
        glClearColor(0.2f, 0.2f, 0.2f, 1.f); // set color buffer to dark grey

        glViewport(0, 0, WIDTH, HEIGHT);
        placeholder_tex = initTextures("Assets/placeholder_background.png");

        initModels();
        setupScreenFBO();
        setupQuadVAO();
        setupScreenShaders();
    }

    void ISGraphics::update(float delta_time) {
        for (ISModel& model : models) {
            model.transform(delta_time);
        }
    }

    void ISGraphics::ISModel::transform(float delta_time) {
        // xform
        if (name == "Circle") 
            model_TRS.scaling.y = model_TRS.scaling.x;

        auto wrap_angle = [](float angle) {
            angle = fmod(angle, 360.f);
            if (angle < 0.f)
                angle += 360.f;
            return angle;
            };

        model_TRS.orientation.x += model_TRS.orientation.y * delta_time;
        model_TRS.orientation.x = wrap_angle(model_TRS.orientation.x);
        float angle_rad = glm::radians(model_TRS.orientation.x);

        float sin_angle = sinf(angle_rad);
        float cos_angle = cosf(angle_rad);

        glm::mat3 world_to_NDC_xform = { model_TRS.scaling.x * cos_angle, model_TRS.scaling.x * -sin_angle, 0,   // column 1
                                         model_TRS.scaling.y * sin_angle, model_TRS.scaling.y * cos_angle,  0,   // column 2
                                         model_TRS.world_position.x,       model_TRS.world_position.y,        1 }; // column 3


        float map_scale_x = 0.00078125f; // 1/1280 (DEPENDANT ON WORLD SIZE)
        float map_scale_y = 0.00138889f; // 1/720

        glm::mat3 map_scale_xform = { map_scale_x, 0,         0,   // column 1
                                      0,         map_scale_y, 0,   // column 2
                                      0,         0,           1 }; // column 3

        model_TRS.mdl_to_ndl_xform = map_scale_xform * world_to_NDC_xform;
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
            if (sprite.model == ModelType::Box) {
                ISModel ModelCopy = models[static_cast<int>(ModelType::Box)];
                //set the model copy's transform
                ModelCopy.draw();
                if (ModelCopy.drawing) { ModelCopy.drawSpecial(); }
            }

            
        }
        //for (ISModel &model : models) {
        //    if (model.drawing)
        //        model.drawSpecial();
        //}

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
        for (ISModel& model : models) {
            glDeleteVertexArrays(1, &model.vao_ID);
        }

        glDeleteFramebuffers(1, &fbo_id);
        glDeleteVertexArrays(1, &vao_id);
        glDeleteTextures(1, &tex_id);
        glDeleteTextures(1, &placeholder_tex);
    }

    void ISGraphics::initModels() {
        ISModel test_box_model("Box", GL_TRIANGLES);
        ISModel test_points_model("Points", GL_POINTS);
        ISModel test_lines_model("Lines", GL_LINES);
        ISModel test_circle_model("Circle", GL_TRIANGLE_FAN);

        test_box_model.setupVAO();
        test_box_model.setupShaders();
        test_box_model.model_TRS.world_position = glm::vec2(0.f, -640.f); // somewhere top-left (initially)
        test_box_model.model_TRS.scaling = glm::vec2(400.f, 200.f); // max scaling (fit entire screen x: 1280, y: 720)
        //test_box_model.orientation.y = 30.f;

        test_points_model.setupSpecialVAO();
        test_points_model.setupShaders();
        test_points_model.model_TRS.scaling = glm::vec2(1066.67f, 600.f);
        test_points_model.model_TRS.orientation.y = 10.f;

        test_lines_model.setupSpecialVAO();
        test_lines_model.setupShaders();
        test_lines_model.model_TRS.scaling = glm::vec2(1000.00f, 500.f);
        test_lines_model.model_TRS.orientation.y = -10.f;

        test_circle_model.setupSpecialVAO();
        test_circle_model.setupShaders();
        test_circle_model.model_TRS.scaling = glm::vec2(100.f, 100.f);
        //test_circle_model.orientation.y = -30.f;

        //based on this i create the enum
        models.emplace_back(test_box_model); //ur box is 0
        models.emplace_back(test_points_model);//ur point is 1
        models.emplace_back(test_lines_model);//ur line is 2
        models.emplace_back(test_circle_model);//ur circle is 3
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

    void ISGraphics::setupQuadVAO() {
        struct Vertex {
            glm::vec2 position;
            glm::vec2 texCoord;
        };

        // Define the vertices of the quad
        std::array<Vertex, 4> vertices{
            Vertex{ { -1.0f, -1.0f }, { 0.0f, 1.0f } },
            Vertex{ {  1.0f, -1.0f }, { 1.0f, 1.0f } },
            Vertex{ { -1.0f,  1.0f }, { 0.0f, 0.0f } },
            Vertex{ {  1.0f,  1.0f }, { 1.0f, 0.0f } }
        };

        std::array<GLuint, 6> indices{ 0, 1, 2, 2, 1, 3 };

        // Generate a VAO handle to encapsulate the VBO and EBO
        GLuint vao_hdl;
        glCreateVertexArrays(1, &vao_hdl);
        glBindVertexArray(vao_hdl);

        // Create and bind a VBO to store the vertex data
        GLuint vbo_hdl;
        glCreateBuffers(1, &vbo_hdl);
        glNamedBufferStorage(vbo_hdl, sizeof(Vertex) * vertices.size(), vertices.data(), 0);

        // Bind the VBO to the VAO
        glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(Vertex));

        // Enable the position attribute
        glEnableVertexArrayAttrib(vao_hdl, 0);
        glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribBinding(vao_hdl, 0, 0);

        // Enable the texture coordinate attribute
        glEnableVertexArrayAttrib(vao_hdl, 1);
        glVertexArrayAttribFormat(vao_hdl, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(vao_hdl, 1, 0);

        // Set up the element buffer object (EBO) for indexing
        GLuint ebo_hdl;
        glCreateBuffers(1, &ebo_hdl);
        glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * indices.size(), indices.data(), 0); // GL_DYNAMIC_STORAGE_BIT removed

        // Bind the EBO to the VAO
        glVertexArrayElementBuffer(vao_hdl, ebo_hdl);

        // Unbind the VAO (not necessary to unbind buffers individually)
        glBindVertexArray(0);

        // Store the VAO handle in the GLPbo object
        vao_id = vao_hdl;
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

    void ISGraphics::ISModel::setupVAO() {
        struct Vertex {
            glm::vec2 position;
            glm::vec2 texCoord;
        };

        // Define the vertices of the quad
        std::array<Vertex, 4> vertices{
            Vertex{glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, .0f)}
        };


        std::array<unsigned int, 6> indices{ 0, 1, 2, 2, 1, 3 };

        // Generate a VAO handle to encapsulate the VBO and EBO
        GLuint vao_hdl;
        glCreateVertexArrays(1, &vao_hdl);
        glBindVertexArray(vao_hdl);

        // Create and bind a VBO to store the vertex data
        GLuint vbo_hdl;
        glCreateBuffers(1, &vbo_hdl);
        glNamedBufferStorage(vbo_hdl, sizeof(Vertex) * vertices.size(), vertices.data(), 0);

        // Bind the VBO to the VAO
        glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(Vertex));

        // Enable the position attribute
        glEnableVertexArrayAttrib(vao_hdl, 0);
        glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribBinding(vao_hdl, 0, 0);

        // Enable the color attribute
        /*glEnableVertexArrayAttrib(vao_hdl, 1);
        glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
        glVertexArrayAttribBinding(vao_hdl, 1, 0);*/

        // Enable the texture coordinate attribute
        glEnableVertexArrayAttrib(vao_hdl, 2);
        glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(vao_hdl, 2, 0);

        // Set up the element buffer object (EBO) for indexing
        GLuint ebo_hdl;
        glCreateBuffers(1, &ebo_hdl);
        glNamedBufferStorage(ebo_hdl, sizeof(unsigned int) * indices.size(), indices.data(), 0); // GL_DYNAMIC_STORAGE_BIT removed

        // Bind the EBO to the VAO
        glVertexArrayElementBuffer(vao_hdl, ebo_hdl);

        // Unbind the VAO (not necessary to unbind buffers individually)
        glBindVertexArray(0);

        // Store the VAO handle in the GLPbo object
        primitive_type = GL_TRIANGLES;
        vao_ID = vao_hdl;
        draw_count = static_cast<GLuint>(indices.size());
    }

    void ISGraphics::ISModel::setupShaders() {
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
        shader_program.compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        shader_program.compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        shader_program.link();
        shader_program.validate();

        // Check if the shader program compilation and linking was successful
        if (GL_FALSE == shader_program.isLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << shader_program.getLog() << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void ISGraphics::ISModel::draw() { // documentation soon
        shader_program.use();

        glBindVertexArray(vao_ID);
        
        shader_program.setUniform("uColor", color);
        shader_program.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndl_xform);

        glDrawElements(primitive_type, draw_count, GL_UNSIGNED_INT, NULL);

        glBindVertexArray(0);
        shader_program.unUse();
    }

    // for M1 rubrics (to be removed after!)
    void ISGraphics::ISModel::setupSpecialVAO() {
        // Define the vertices of the quad

        std::vector<glm::vec2> pos_vtx;

        if (primitive_type == GL_POINTS || primitive_type == GL_LINES) {
            pos_vtx = {
                glm::vec2(-1.0f, -1.0f),
                glm::vec2(1.0f, -1.0f),
                glm::vec2(-1.0f, 1.0f),
                glm::vec2(1.0f, 1.0f)
            };
            draw_count = static_cast<GLuint>(pos_vtx.size());
        }
        else if (primitive_type == GL_TRIANGLE_FAN) {
            pos_vtx.emplace_back(glm::vec2(0.f, 0.f)); // center of circle

            int slices{ 30 };
            float angle{ (2 * static_cast<float>(PI) / slices) }; // 15 slices

            for (int i{}; i < slices + 1; ++i) {
                float x = (cosf(i * angle));
                float y = (sinf(i * angle));

                pos_vtx.emplace_back(glm::vec2(x, y));
            }
            //color[0] = 0.f, color[1] = 0.f, color[2] = 1.f;
            draw_count = static_cast<GLuint>(slices + 2);
        }

        GLuint vbo_hdl;
        glCreateBuffers(1, &vbo_hdl);
        glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);

        // vertex array object
        GLuint vao_hdl;
        glCreateVertexArrays(1, &vao_hdl);
        glEnableVertexArrayAttrib(vao_hdl, 0);
        glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
        glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao_hdl, 0, 0);
        glBindVertexArray(0);

        vao_ID = vao_hdl;
    }

    void ISGraphics::ISModel::drawSpecial() {
        shader_program.use();

        glBindVertexArray(vao_ID);

        //shader_program.SetUniform("uSize", test_model.size);
        shader_program.setUniform("uColor", color); // same colour as test_model for now
        shader_program.setUniform("uModel_to_NDC", model_TRS.mdl_to_ndl_xform);

        // Bind the texture to the uniform sampler2D
        if (this->name == "Box") {
            GLuint textureUniformLoc = glGetUniformLocation(shader_program.getHandle(), "uTex2d");
            if (textureUniformLoc != -1) {
                glUniform1i(textureUniformLoc, 0); // Bind to texture unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, placeholder_tex);
            }
            shader_program.setUniform("uTexture", 1);
        }
        else shader_program.setUniform("uTexture", 0);

        switch (primitive_type) {
        case GL_TRIANGLES:
            glDrawElements(primitive_type, draw_count, GL_UNSIGNED_INT, NULL);
            break;
        case GL_POINTS:
            glPointSize(5.f);
            glDrawArrays(primitive_type, 0, draw_count);
            glPointSize(1.f);
            break;
        case GL_LINES:
            glLineWidth(3.f);
            glDrawArrays(primitive_type, 0, draw_count);
            glLineWidth(1.f);
            break;
        case GL_TRIANGLE_FAN:
            glDrawArrays(primitive_type, 0, draw_count);
            break;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader_program.unUse();
    }
}
