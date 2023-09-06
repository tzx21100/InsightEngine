//pch has to go to the top of every cpp
#include "Pch.h"
#include "Graphics.h"
#include <vector>
#include <array>
#include <string>

ISGraphics::ISModel ISGraphics::test_model("Rectangle");

void ISGraphics::init() {
	glClearColor(1.f, 1.f, 1.f, 1.f); // set color buffer to black

	glViewport(0, 0, WIDTH, HEIGHT);

	test_model.setupVAO();
	test_model.setupShaders();

    test_model.worldPosition = glm::vec2(-200.f, 300.f); // somewhere top-left (initially)
    test_model.angle = 30.f; // 30 degrees tilted
    test_model.scaling = glm::vec2(400.f, 200.f); // max scaling (fit entire screen x: 1280, y: 720)
}

void ISGraphics::update(float delta_time) {
    glClearColor(1.f, 1.f, 1.f, 1.f); // set color buffer to black

    // xform
    auto wrap_angle = [](float angle) {
        angle = fmod(angle, 360.f);
        if (angle < 0.f)
            angle += 360.f;
        return angle;
    };
    float angle_speed = 30.f; // 30 degrees per
    test_model.angle += angle_speed * delta_time;
    test_model.angle = wrap_angle(test_model.angle);
    float angle_rad = glm::radians(test_model.angle);

    float sin_angle = sinf(angle_rad);
    float cos_angle = cosf(angle_rad);

    glm::mat3 world_to_NDC_xform = { test_model.scaling.x * cos_angle, test_model.scaling.x * -sin_angle, 0,   // column 1
                                     test_model.scaling.y * sin_angle, test_model.scaling.y * cos_angle,  0,   // column 2
                                     test_model.worldPosition.x,       test_model.worldPosition.y,        1 }; // column 3


    float map_scale_x = 0.00078125f; // 1/1280 (DEPENDANT ON WORLD SIZE)
    float map_scale_y = 0.00138889f; // 1/720

    glm::mat3 map_scale_xform = { map_scale_x, 0,         0,   // column 1
                                  0,         map_scale_y, 0,   // column 2
                                  0,         0,           1 }; // column 3

    test_model.mdl_to_ndl_xform = map_scale_xform * world_to_NDC_xform;
}
void ISGraphics::draw() {
	glClear(GL_COLOR_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    if (test_model.drawing)
	    test_model.draw();

    // Render imgui window
    ImGui::Begin(test_model.name.c_str());
    ImGui::Text("Control the attributes of %s.", test_model.name.c_str());
    std::string label{ "Draw " + test_model.name };
    ImGui::Checkbox(label.c_str(), &test_model.drawing);
    ImGui::SliderFloat("Width", &test_model.scaling.x, 2.f, 2.f * WIDTH);
    ImGui::SliderFloat("Height", &test_model.scaling.y, 1.f, 2.f * HEIGHT);
    ImGui::SliderFloat("Rotation", &test_model.angle, 0.f, 360.f);
    ImGui::ColorEdit3("Color", test_model.color);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ISGraphics::cleanup() {
	// WIP

    // Shutdown imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

    //shader_program.SetUniform("uSize", test_model.size);
    shader_program.SetUniform("uColor", test_model.color[0], test_model.color[1], test_model.color[2]);
    shader_program.SetUniform("uModel_to_NDC", test_model.mdl_to_ndl_xform);


    glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    shader_program.UnUse();
}