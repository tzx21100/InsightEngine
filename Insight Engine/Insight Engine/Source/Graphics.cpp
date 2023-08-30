//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include <vector>
#include <array>
#include <string>



ISGraphics::ISModel ISGraphics::test_model{};

void ISGraphics::init() {
	glClearColor(1.f, 0.f, 0.f, 1.f); // set color buffer to red

	glViewport(0, 0, WIDTH, HEIGHT);

	test_model.setupVAO();
	test_model.setupShaders();
}

void ISGraphics::update() {
	glClearColor(1.f, 0.f, 0.f, 1.f); // set color buffer to red
}

void ISGraphics::draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	test_model.draw();
}

void ISGraphics::cleanup() {
	// WIP
}

void ISGraphics::ISModel::setupVAO() {
    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    // Define the vertices of the quad
    std::array<Vertex, 4> vertices{
        Vertex{glm::vec2(-1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec2(1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec2(-1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}
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
    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
    glVertexArrayAttribBinding(vao_hdl, 1, 0);

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
    vao_ID = vao_hdl;
    draw_count = static_cast<GLuint>(indices.size());
}

void ISGraphics::ISModel::setupShaders() {
    // vertex shader
    std::string vtx_shdr
    {
        "#version 450 core\n"
        "layout(location = 0) in vec2 aVertexPosition;\n"
        "layout(location = 1) in vec3 aVertexColor;\n"
        "layout(location = 0) out vec3 vColor;\n"
        "void main() {\n"
        "	gl_Position = vec4(aVertexPosition, 0.0, 1.0);\n"
        "	vColor = aVertexColor;\n"
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
        "layout (location=0) in vec3 vInterpColor;\n"
        "layout (location=0) out vec4 fFragColor;\n\n"

        "void main() {\n"
        "	fFragColor = vec4(vInterpColor, 1.0);\n"
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
    shader_program.CompileShaderFromString(GL_VERTEX_SHADER, vtx_shdr);
    shader_program.CompileShaderFromString(GL_FRAGMENT_SHADER, frag_shdr);
    shader_program.Link();
    shader_program.Validate();

    // Print the active attribute and uniform information
    shader_program.PrintActiveAttribs();
    shader_program.PrintActiveUniforms();

    // Check if the shader program compilation and linking was successful
    if (GL_FALSE == shader_program.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs\n";
        std::cout << shader_program.GetLog() << "\n";
        exit(EXIT_FAILURE);
    }
}

void ISGraphics::ISModel::draw() { // documentation soon
    shader_program.Use();

    glBindVertexArray(vao_ID);

    glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    shader_program.UnUse();
}