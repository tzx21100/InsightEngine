/*!
 * \file Mesh.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
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
	void Mesh::setupQuadVAO() {
        // Define the vertices of the quad as a triangle strip
        std::array<Vertex, 4> vertices{
            Vertex{glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
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

        // Enable the texture coordinate attribute
        glEnableVertexArrayAttrib(vao_hdl, 2);
        glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(vao_hdl, 2, 0);

        // Unbind the VAO (not necessary to unbind buffers individually)
        glBindVertexArray(0);

        // save VAO, VBO and draw count
        vao_ID = vao_hdl;
        vbo_ID = vbo_hdl;
        draw_count = static_cast<GLuint>(vertices.size());
	}

    void Mesh::setupNonQuadVAO(GLenum mesh_primitive_type) {

        std::vector<glm::vec2> pos_vtx; // vector for vertice positions

        if (mesh_primitive_type == GL_POINTS) { // point mesh
            pos_vtx = {
                glm::vec2(0.f, 0.f)
            };
            draw_count = static_cast<GLuint>(pos_vtx.size());
        }
        else if (mesh_primitive_type == GL_LINES) { // line mesh
            pos_vtx = { // start horizontal
                glm::vec2(-1.f, 0.f),
                glm::vec2(1.f, 0.f)
            };
            draw_count = static_cast<GLuint>(pos_vtx.size());
        }
        else if (mesh_primitive_type == GL_TRIANGLE_FAN) { // circle mesh
            pos_vtx.emplace_back(glm::vec2(0.f, 0.f)); // center of circle

            int slices{ 30 }; // 30 slices
            float angle{ (2 * static_cast<float>(PI) / slices) }; // angle between each slice

            for (int i{}; i < slices + 1; ++i) {
                float x = (cosf(i * angle));
                float y = (sinf(i * angle));

                // emplace calculated vertices
                pos_vtx.emplace_back(glm::vec2(x, y));
            }
            draw_count = static_cast<GLuint>(slices + 2);
        }

        // Create and bind a VBO to store the vertex data
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

        // save VAO and VBO to free
        vao_ID = vao_hdl;
        vbo_ID = vbo_hdl;
    }

    void Mesh::initMeshes(std::vector<Mesh>& meshes) {
        // 4 meshes for 4 different models
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

    void Mesh::cleanupMeshes(std::vector<Mesh>& meshes) {
        // delete arrays and buffers
        for (auto& mesh : meshes) {
            glDeleteVertexArrays(1, &mesh.vao_ID);
            glDeleteBuffers(1, &mesh.vbo_ID);
        }
    }
}