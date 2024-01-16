
#include "Pch.h"
#include "Shadow.h"
#include "Sprite.h"

namespace IS {
    Mesh Shadow::shadowMesh;

    void Shadow::setupShadowQuadMesh() {
        // Define the vertices of the quad as a triangle strip
        std::array<Mesh::Vertex, 4> vertices {
            Mesh::Vertex{glm::vec2(-1.0f, -1.0f)},
            Mesh::Vertex{ glm::vec2(1.0f, -1.0f)},
            Mesh::Vertex{ glm::vec2(-1.0f, 1.0f),},
            Mesh::Vertex{ glm::vec2(1.0f, 1.0f)}
        };

        // Generate a VAO handle to encapsulate the VBO
        glCreateVertexArrays(1, &shadowMesh.vao_ID);
        glBindVertexArray(shadowMesh.vao_ID);

        // Create and bind a VBO to store the vertex data
        glCreateBuffers(1, &shadowMesh.vbo_ID);
        glNamedBufferStorage(shadowMesh.vbo_ID, sizeof(Mesh::Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, shadowMesh.vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, Mesh::pos_attrib);

        // Bind attributes
        glVertexAttribPointer(Mesh::pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 0);

        // Create Instance Buffer Object
        glCreateBuffers(1, &shadowMesh.instance_vbo_ID);
        glNamedBufferStorage(shadowMesh.instance_vbo_ID, sizeof(Shadow::shadowInstanceData) * 100'000, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, shadowMesh.instance_vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, color_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, x_form_row1_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, x_form_row2_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, x_form_row3_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, x_form_row4_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, TL_coord_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, TR_coord_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, BR_coord_attrib);
        glEnableVertexArrayAttrib(shadowMesh.vao_ID, BL_coord_attrib);

        // Specify instance data layout
        glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), 0);
        glVertexAttribPointer(x_form_row1_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 4));
        glVertexAttribPointer(x_form_row2_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 8));
        glVertexAttribPointer(x_form_row3_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 12));
        glVertexAttribPointer(x_form_row4_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 16));
        glVertexAttribPointer(TL_coord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 18));
        glVertexAttribPointer(TR_coord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 20));
        glVertexAttribPointer(BR_coord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 22));
        glVertexAttribPointer(BL_coord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Shadow::shadowInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 24));

        // Specify instance data divisor for attribute instancing
        glVertexAttribDivisor(color_attrib, 1);
        glVertexAttribDivisor(x_form_row1_attrib, 1);
        glVertexAttribDivisor(x_form_row2_attrib, 1);
        glVertexAttribDivisor(x_form_row3_attrib, 1);
        glVertexAttribDivisor(x_form_row4_attrib, 1);
        glVertexAttribDivisor(TL_coord_attrib, 1);
        glVertexAttribDivisor(TR_coord_attrib, 1);
        glVertexAttribDivisor(BR_coord_attrib, 1);
        glVertexAttribDivisor(BL_coord_attrib, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        shadowMesh.draw_count = static_cast<GLuint>(vertices.size());
    }
}