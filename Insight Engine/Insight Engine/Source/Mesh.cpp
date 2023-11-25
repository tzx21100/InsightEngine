/*!
 * \file Mesh.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * This source file defines the Mesh class, which represents OpenGL vertex array objects and buffers for rendering.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"

namespace IS {
    void Mesh::initMeshes(std::vector<Mesh>& meshes) {
        // Create, init and emplace into mesh vector
        Mesh inst_quad_mesh, inst_line_mesh, inst_circle_mesh, inst_3d_quad_mesh, outline_mesh, fb_mesh;

        // inst_quad_mesh.setupInstancedQuadVAO();
        inst_line_mesh.setupInstancedLineVAO();
        inst_circle_mesh.setupInstancedCircleVAO();
        inst_3d_quad_mesh.setupInstanced3DQuadVAO();
        outline_mesh.setupOutlineVAO();
        fb_mesh.setupFBVAO();

        meshes.emplace_back(inst_quad_mesh);
        meshes.emplace_back(inst_line_mesh);
        meshes.emplace_back(inst_circle_mesh);
        meshes.emplace_back(inst_3d_quad_mesh);
        meshes.emplace_back(outline_mesh);
        meshes.emplace_back(fb_mesh);
    }

    void Mesh::setupInstanced3DQuadVAO() {
        // Define the vertices of the quad as a triangle strip
        std::array<Vertex, 4> vertices{
            Vertex{glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.f)},
            Vertex{ glm::vec2(0.5f, -0.5f),  glm::vec2(1.f, 1.f) },
            Vertex{ glm::vec2(-0.5f, 0.5f),  glm::vec2(0.0f, 0.0f) },
            Vertex{ glm::vec2(0.5f, 0.5f),   glm::vec2(1.f, 0.0f) }
        };

        // Generate a VAO handle to encapsulate the VBO
        glCreateVertexArrays(1, &vao_ID);
        glBindVertexArray(vao_ID);

        // Create and bind a VBO to store the vertex data
        glCreateBuffers(1, &vbo_ID);
        glNamedBufferStorage(vbo_ID, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(vao_ID, pos_attrib);
        glEnableVertexArrayAttrib(vao_ID, tex_coord_attrib);

        // Bind attributes
        glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texCoord)));

        // Create Instance Buffer Object
        glCreateBuffers(1, &instance_vbo_ID);
        glNamedBufferStorage(instance_vbo_ID, sizeof(Sprite::instanceData) * 100'000, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(vao_ID, color_attrib);
        glEnableVertexArrayAttrib(vao_ID, tex_index_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row1_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row2_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row3_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row4_attrib);
        glEnableVertexArrayAttrib(vao_ID, anim_dim_attrib);
        glEnableVertexArrayAttrib(vao_ID, anim_index_attrib);
        glEnableVertexArrayAttrib(vao_ID, ent_ID_attib);

        // Specify instance data layout
        glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), 0);
        glVertexAttribPointer(tex_index_attrib, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 4));
        glVertexAttribPointer(x_form_row1_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 5));
        glVertexAttribPointer(x_form_row2_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 9));
        glVertexAttribPointer(x_form_row3_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 13));
        glVertexAttribPointer(x_form_row4_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 17));
        glVertexAttribPointer(anim_dim_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 21));
        glVertexAttribPointer(anim_index_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 23));
        glVertexAttribPointer(ent_ID_attib, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 25));

        // Specify instance data divisor for attribute instancing
        glVertexAttribDivisor(color_attrib, 1);
        glVertexAttribDivisor(tex_index_attrib, 1);
        glVertexAttribDivisor(x_form_row1_attrib, 1);
        glVertexAttribDivisor(x_form_row2_attrib, 1);
        glVertexAttribDivisor(x_form_row3_attrib, 1);
        glVertexAttribDivisor(x_form_row4_attrib, 1);
        glVertexAttribDivisor(anim_dim_attrib, 1);
        glVertexAttribDivisor(anim_index_attrib, 1);
        glVertexAttribDivisor(ent_ID_attib, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        draw_count = static_cast<GLuint>(vertices.size());
    }

    void Mesh::setupInstancedLineVAO() {
        // Define the vertices of the quad as a triangle strip
        std::vector<glm::vec2> vertices = { // start horizontal
            glm::vec2(-0.5f, 0.f),
            glm::vec2(0.5f, 0.f)
        };

        // Generate a VAO handle to encapsulate the VBO
        glCreateVertexArrays(1, &vao_ID);
        glBindVertexArray(vao_ID);

        // Create and bind a VBO to store the vertex data
        glCreateBuffers(1, &vbo_ID);
        glNamedBufferStorage(vbo_ID, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(vao_ID, pos_attrib);

        // Bind attributes
        glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

        // Create Instance Buffer Object
        glCreateBuffers(1, &instance_vbo_ID);
        glNamedBufferStorage(instance_vbo_ID, sizeof(Sprite::nonQuadInstanceData) * MAX_ENTITIES, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(vao_ID, color_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row1_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row2_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row3_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row4_attrib);

        // Specify instance data layout
        glVertexAttribPointer(color_attrib,       4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), 0);
        glVertexAttribPointer(x_form_row1_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 4));
        glVertexAttribPointer(x_form_row2_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 8));
        glVertexAttribPointer(x_form_row3_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 12));
        glVertexAttribPointer(x_form_row4_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 16));

        // Specify instance data divisor for attribute instancing
        glVertexAttribDivisor(color_attrib,       1);
        glVertexAttribDivisor(x_form_row1_attrib, 1);
        glVertexAttribDivisor(x_form_row2_attrib, 1);
        glVertexAttribDivisor(x_form_row3_attrib, 1);
        glVertexAttribDivisor(x_form_row4_attrib, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        draw_count = static_cast<GLuint>(vertices.size());
    }

    void Mesh::setupInstancedCircleVAO() {
        // Define the vertices of the quad as a triangle strip
        std::vector<glm::vec2> pos_vtx;

        // Generate circle vertices using GL_LINE_LOOP
        const int segments = 50;
        const float radius = 0.5f;
        for (int i = 0; i < segments; i++) {
            float theta = static_cast<float>(i) / static_cast<float>(segments) * 2.0f * PI;
            float x = radius * std::cos(theta);
            float y = radius * std::sin(theta);
            pos_vtx.emplace_back(glm::vec2(x, y));
        }

        // Generate a VAO handle to encapsulate the VBO
        glCreateVertexArrays(1, &vao_ID);
        glBindVertexArray(vao_ID);

        // Create and bind a VBO to store the vertex data
        glCreateBuffers(1, &vbo_ID);
        glNamedBufferStorage(vbo_ID, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(vao_ID, pos_attrib);

        // Bind attributes
        glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

        // Create Instance Buffer Object
        glCreateBuffers(1, &instance_vbo_ID);
        glNamedBufferStorage(instance_vbo_ID, sizeof(Sprite::nonQuadInstanceData) * MAX_ENTITIES, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_ID);

        // Enable attributes
        glEnableVertexArrayAttrib(vao_ID, color_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row1_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row2_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row3_attrib);
        glEnableVertexArrayAttrib(vao_ID, x_form_row4_attrib);

        // Specify instance data layout
        glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), 0);
        glVertexAttribPointer(x_form_row1_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 4));
        glVertexAttribPointer(x_form_row2_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 8));
        glVertexAttribPointer(x_form_row3_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 12));
        glVertexAttribPointer(x_form_row4_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::nonQuadInstanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 16));

        // Specify instance data divisor for attribute instancing
        glVertexAttribDivisor(color_attrib, 1);
        glVertexAttribDivisor(x_form_row1_attrib, 1);
        glVertexAttribDivisor(x_form_row2_attrib, 1);
        glVertexAttribDivisor(x_form_row3_attrib, 1);
        glVertexAttribDivisor(x_form_row4_attrib, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        draw_count = static_cast<GLuint>(pos_vtx.size());
    }

    void Mesh::setupOutlineVAO() {
        // Define the vertices of the quad as a triangle strip
        std::array<Vertex, 4> vertices{
            Vertex{glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.f)},
            Vertex{ glm::vec2(0.5f, -0.5f),  glm::vec2(1.f, 1.f) },
            Vertex{ glm::vec2(0.5f, 0.5f),   glm::vec2(1.f, 0.0f) },
            Vertex{ glm::vec2(-0.5f, 0.5f),  glm::vec2(0.0f, 0.0f) }
        };

        // Generate a VAO handle to encapsulate the VBO
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

        // Unbind the VAO (not necessary to unbind buffers individually)
        glBindVertexArray(0);

        // save VAO, VBO and draw count
        vao_ID = vao_hdl;
        vbo_ID = vbo_hdl;
        draw_count = static_cast<GLuint>(vertices.size());
    }

    void Mesh::setupFBVAO() {
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        // screen quad VAO
        unsigned int quadVAO, quadVBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        vao_ID = quadVAO;
        vbo_ID = quadVBO;
        draw_count = 6;
    }

    void Mesh::cleanupMeshes(std::vector<Mesh>& meshes) {
        // delete arrays and buffers
        for (auto& mesh : meshes) {
            glDeleteVertexArrays(1, &mesh.vao_ID);
            glDeleteBuffers(1, &mesh.vbo_ID);
        }
    }

    // now unused
 //   void Mesh::setupInstancedQuadVAO() {
 //       // Define the vertices of the quad as a triangle strip
 //       std::array<Vertex, 4> vertices{
 //           Vertex{glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.f)},
 //           Vertex{glm::vec2(0.5f, -0.5f),  glm::vec2(1.f, 1.f)},
 //           Vertex{glm::vec2(-0.5f, 0.5f),  glm::vec2(0.0f, 0.0f)},
 //           Vertex{glm::vec2(0.5f, 0.5f),   glm::vec2(1.f, 0.0f)}
 //       };

 //       // Generate a VAO handle to encapsulate the VBO
 //       glCreateVertexArrays(1, &vao_ID);
 //       glBindVertexArray(vao_ID);

 //       // Create and bind a VBO to store the vertex data
 //       glCreateBuffers(1, &vbo_ID);
 //       glNamedBufferStorage(vbo_ID, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
 //       glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);

 //       // Enable attributes
 //       glEnableVertexArrayAttrib(vao_ID, pos_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, tex_coord_attrib);

 //       // Bind attributes
 //       glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
 //       glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texCoord)));

 //       // Create Instance Buffer Object
 //       glCreateBuffers(1, &instance_vbo_ID);
 //       glNamedBufferStorage(instance_vbo_ID, sizeof(Sprite::instanceData) * MAX_ENTITIES, NULL, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
 //       glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_ID);

 //       // Enable attributes
 //       glEnableVertexArrayAttrib(vao_ID, color_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, tex_index_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, x_form_row1_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, x_form_row2_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, x_form_row3_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, anim_dim_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, anim_index_attrib);
 //       glEnableVertexArrayAttrib(vao_ID, ent_ID_attib);

 //       // Specify instance data layout
 //       glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), 0);
 //       glVertexAttribPointer(tex_index_attrib, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 4));
 //       glVertexAttribPointer(x_form_row1_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 5));
 //       glVertexAttribPointer(x_form_row2_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 8));
 //       glVertexAttribPointer(x_form_row3_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 11));
 //       glVertexAttribPointer(anim_dim_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 14));
 //       glVertexAttribPointer(anim_index_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 16));
 //       glVertexAttribPointer(ent_ID_attib, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite::instanceData), reinterpret_cast<GLvoid*>(sizeof(float) * 18));

 //       // Specify instance data divisor for attribute instancing
 //       glVertexAttribDivisor(color_attrib, 1);
 //       glVertexAttribDivisor(tex_index_attrib, 1);
 //       glVertexAttribDivisor(x_form_row1_attrib, 1);
 //       glVertexAttribDivisor(x_form_row2_attrib, 1);
 //       glVertexAttribDivisor(x_form_row3_attrib, 1);
 //       glVertexAttribDivisor(anim_dim_attrib, 1);
 //       glVertexAttribDivisor(anim_index_attrib, 1);
 //       glVertexAttribDivisor(ent_ID_attib, 1);

 //       glBindBuffer(GL_ARRAY_BUFFER, 0);
 //       glBindVertexArray(0);
 //       draw_count = static_cast<GLuint>(vertices.size());
 //   }

	//void Mesh::setupQuadVAO() {
 //       // Define the vertices of the quad as a triangle strip
 //       std::array<Vertex, 4> vertices{
 //           Vertex{glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.f)},
 //           Vertex{glm::vec2(0.5f, -0.5f), glm::vec2(1.f, 1.f)},
 //           Vertex{glm::vec2(-0.5f, 0.5f), glm::vec2(0.0f, 0.0f)},
 //           Vertex{glm::vec2(0.5f, 0.5f), glm::vec2(1.f, 0.0f)}
 //       };

 //       // Generate a VAO handle to encapsulate the VBO
 //       GLuint vao_hdl;
 //       glCreateVertexArrays(1, &vao_hdl);
 //       glBindVertexArray(vao_hdl);

 //       // Create and bind a VBO to store the vertex data
 //       GLuint vbo_hdl;
 //       glCreateBuffers(1, &vbo_hdl);
 //       glNamedBufferStorage(vbo_hdl, sizeof(Vertex) * vertices.size(), vertices.data(), 0);

 //       // Bind the VBO to the VAO
 //       glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(Vertex));

 //       // Enable the position attribute
 //       glEnableVertexArrayAttrib(vao_hdl, 0);
 //       glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
 //       glVertexArrayAttribBinding(vao_hdl, 0, 0);

 //       // Enable the texture coordinate attribute
 //       glEnableVertexArrayAttrib(vao_hdl, 2);
 //       glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
 //       glVertexArrayAttribBinding(vao_hdl, 2, 0);

 //       // Unbind the VAO (not necessary to unbind buffers individually)
 //       glBindVertexArray(0);

 //       // save VAO, VBO and draw count
 //       vao_ID = vao_hdl;
 //       vbo_ID = vbo_hdl;
 //       draw_count = static_cast<GLuint>(vertices.size());
	//}

 //   void Mesh::setupNonQuadVAO(GLenum mesh_primitive_type) {

 //       std::vector<glm::vec2> pos_vtx; // vector for vertice positions

 //       if (mesh_primitive_type == GL_POINTS) { // point mesh
 //           pos_vtx = {
 //               glm::vec2(0.f, 0.f)
 //           };
 //           draw_count = static_cast<GLuint>(pos_vtx.size());
 //       }
 //       else if (mesh_primitive_type == GL_LINES) { // line mesh
 //           pos_vtx = { // start horizontal
 //               glm::vec2(-1.f, 0.f),
 //               glm::vec2(1.f, 0.f)
 //           };
 //           draw_count = static_cast<GLuint>(pos_vtx.size());
 //       }
 //       else if (mesh_primitive_type == GL_TRIANGLE_FAN) { // circle mesh
 //           pos_vtx.emplace_back(glm::vec2(0.f, 0.f)); // center of circle

 //           int slices{ 30 }; // 30 slices
 //           float angle{ (2 * static_cast<float>(PI) / slices) }; // angle between each slice

 //           for (int i{}; i < slices + 1; ++i) {
 //               float x = (cosf(i * angle));
 //               float y = (sinf(i * angle));

 //               // emplace calculated vertices
 //               pos_vtx.emplace_back(glm::vec2(x, y));
 //           }
 //           draw_count = static_cast<GLuint>(slices + 2);
 //       }

 //       // Create and bind a VBO to store the vertex data
 //       GLuint vbo_hdl;
 //       glCreateBuffers(1, &vbo_hdl);
 //       glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);

 //       // vertex array object
 //       GLuint vao_hdl;
 //       glCreateVertexArrays(1, &vao_hdl);
 //       glEnableVertexArrayAttrib(vao_hdl, 0);
 //       glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
 //       glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
 //       glVertexArrayAttribBinding(vao_hdl, 0, 0);
 //       glBindVertexArray(0);

 //       // save VAO and VBO to free
 //       vao_ID = vao_hdl;
 //       vbo_ID = vbo_hdl;
 //   }
}