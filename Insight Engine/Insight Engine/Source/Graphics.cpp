//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include <vector>
#include <array>
#include <string>

namespace IS {
    ISGraphics::ISModel ISGraphics::test_model("Rectangle");
    ISGraphics::ISModel ISGraphics::test_points_model("Points");
    ISGraphics::ISModel ISGraphics::test_lines_model("Lines");
    ISGraphics::ISModel ISGraphics::test_circle_model("Circle");

    void ISGraphics::init() {
        glClearColor(1.f, 1.f, 1.f, 1.f); // set color buffer to white

        glViewport(0, 0, WIDTH, HEIGHT);

        test_model.setupVAO();
        test_model.setupShaders();

        test_points_model.primititiveType = GL_POINTS;
        test_points_model.setupSpecialVAO();
        test_points_model.setupShaders();

        test_lines_model.primititiveType = GL_LINES;
        test_lines_model.setupSpecialVAO();
        test_lines_model.setupShaders();

        test_circle_model.primititiveType = GL_TRIANGLE_FAN;
        test_circle_model.setupSpecialVAO();
        test_circle_model.setupShaders();

        test_model.worldPosition = glm::vec2(-200.f, 300.f); // somewhere top-left (initially)
        test_model.angle = 30.f; // 30 degrees tilted
        test_model.scaling = glm::vec2(400.f, 200.f); // max scaling (fit entire screen x: 1280, y: 720)

        test_points_model.scaling = glm::vec2(1066.67f, 600.f);
        test_lines_model.scaling = glm::vec2(1000.00f, 500.f); 
        test_circle_model.scaling = glm::vec2(300.00f, 300.f);
    }

    void ISGraphics::update(float delta_time) {
        glClearColor(1.f, 1.f, 1.f, 1.f); // set color buffer to white

        test_model.transform(delta_time);
        test_points_model.transform(delta_time);
        test_lines_model.transform(delta_time);
        test_circle_model.transform(delta_time);
    }

    void ISGraphics::ISModel::transform(float delta_time) {
        // xform
        auto wrap_angle = [](float angle) {
            angle = fmod(angle, 360.f);
            if (angle < 0.f)
                angle += 360.f;
            return angle;
            };

        float angle_speed = 0.f; // 0 degree rotation per second
        angle += angle_speed * delta_time;
        angle = wrap_angle(angle);
        float angle_rad = glm::radians(angle);

        float sin_angle = sinf(angle_rad);
        float cos_angle = cosf(angle_rad);

        glm::mat3 world_to_NDC_xform = { scaling.x * cos_angle, scaling.x * -sin_angle, 0,   // column 1
                                         scaling.y * sin_angle, scaling.y * cos_angle,  0,   // column 2
                                         worldPosition.x,       worldPosition.y,        1 }; // column 3


        float map_scale_x = 0.00078125f; // 1/1280 (DEPENDANT ON WORLD SIZE)
        float map_scale_y = 0.00138889f; // 1/720

        glm::mat3 map_scale_xform = { map_scale_x, 0,         0,   // column 1
                                      0,         map_scale_y, 0,   // column 2
                                      0,         0,           1 }; // column 3

        mdl_to_ndl_xform = map_scale_xform * world_to_NDC_xform;
    }

    void ISGraphics::draw() {
        glClear(GL_COLOR_BUFFER_BIT);

        if (test_model.drawing)
            test_model.draw();

        test_circle_model.drawSpecial();
        test_lines_model.drawSpecial();
        test_points_model.drawSpecial();
    }

    void ISGraphics::cleanup() {
        // WIP
    }

    void ISGraphics::ISModel::setupVAO() {
        struct Vertex {
            glm::vec2 position;
            glm::vec2 texCoord;
        };

        // Define the vertices of the quad
        std::array<Vertex, 4> vertices{
            Vertex{glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
            Vertex{glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}
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
        primititiveType = GL_TRIANGLES;
        vao_ID = vao_hdl;
        draw_count = static_cast<GLuint>(indices.size());
    }

    void ISGraphics::ISModel::setupShaders() {
        // vertex shader
        std::string vtx_shdr
        {
            "#version 450 core\n"
            "layout(location = 0) in vec2 aVertexPosition;\n\n"
            "uniform mat3 uModel_to_NDC;\n\n"
            "void main() {\n"
            "	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);\n"
            "}\n"

            /*
            "#version 450 core\n"
            "layout(location = 0) in vec2 aVertexPosition;\n"
            "layout(location = 1) in vec2 aTexCoord;\n"
            "layout(location = 0) out vec2 vTexCoord;\n"
            "void main()\n"
            "{\n"
            "	gl_Position = vec4(aVertexPosition, 0.0, 1.0);\n"
            "	vTexCoord = aTexCoord;\n"
            "}\n"
            */
        };

        // fragment shader
        std::string frag_shdr
        {
            // for colors only, no texture
            "#version 450 core\n"
            "layout (location=0) out vec4 fFragColor;\n"
            "uniform vec3 uColor;"
            "void main() {\n"
            "	fFragColor = vec4(uColor, 1.0);\n"
            "}\n"

            // for texture
            /*
            "#version 450 core\n"
            "layout (location=0) in vec2 vTexCoord;\n"
            "layout (location=0) out vec4 fFragColor;\n"
            "uniform sampler2D uTex2d;\n"
            "void main()\n"
            "{\n"
            "	fFragColor = texture(uTex2d, vTexCoord);\n"
            "}\n"
            */
        };

        // Compile and link the shaders into a shader program
        shader_program.compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        shader_program.compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        shader_program.link();
        shader_program.validate();

        // Print the active attribute and uniform information
        /*shader_program.PrintActiveAttribs();
        shader_program.PrintActiveUniforms();*/

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

        shader_program.setUniform("uColor", test_model.color[0], test_model.color[1], test_model.color[2]);
        shader_program.setUniform("uModel_to_NDC", test_model.mdl_to_ndl_xform);

        glDrawElements(primititiveType, draw_count, GL_UNSIGNED_INT, NULL);

        glBindVertexArray(0);
        shader_program.unUse();
    }

    // for M1 rubrics (to be removed after!)
    void ISGraphics::ISModel::setupSpecialVAO() {
        // Define the vertices of the quad

        std::vector<glm::vec2> pos_vtx;

        if (primititiveType == GL_POINTS || primititiveType == GL_LINES) {
            pos_vtx = {
                glm::vec2(-1.0f, -1.0f),
                glm::vec2(1.0f, -1.0f),
                glm::vec2(-1.0f, 1.0f),
                glm::vec2(1.0f, 1.0f)
            };
            draw_count = static_cast<GLuint>(pos_vtx.size());
        }
        else if (primititiveType == GL_TRIANGLE_FAN) {
            pos_vtx.emplace_back(glm::vec2(0.f, 0.f)); // center of circle

            int slices{ 30 };
            float angle{ (2 * static_cast<float>(PI) / slices) }; // 15 slices

            for (int i{}; i < slices + 1; ++i) {
                float x = (cosf(i * angle));
                float y = (sinf(i * angle));

                pos_vtx.emplace_back(glm::vec2(x, y));
            }
            color[0] = 0.f, color[1] = 0.f, color[2] = 1.f;
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
        shader_program.setUniform("uColor", color[0], color[1], color[2]); // same colour as test_model for now
        shader_program.setUniform("uModel_to_NDC", mdl_to_ndl_xform);

        switch (primititiveType) {
        case GL_POINTS:
            glPointSize(5.f);
            glDrawArrays(primititiveType, 0, draw_count);
            glPointSize(1.f);
            break;
        case GL_LINES:
            glLineWidth(3.f);
            glDrawArrays(primititiveType, 0, draw_count);
            glLineWidth(1.f);
            break;
        case GL_TRIANGLE_FAN:
            glDrawArrays(primititiveType, 0, draw_count);
        }

        glBindVertexArray(0);
        shader_program.unUse();
    }
}
