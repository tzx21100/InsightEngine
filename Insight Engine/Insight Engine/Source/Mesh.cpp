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
    void Mesh::setupInstancedQuadVAO() {
        // Define the vertices of the quad as a triangle strip
        std::array<Vertex, 4> vertices{
            Vertex{glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            Vertex{glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            Vertex{glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
        };

        // Define an array of instance data for a single quad
        std::array<InstanceData, 1> instanceData{
            InstanceData{
                glm::mat4(1.0f),    // Identity transformation matrix
                glm::vec3(1.0f),    // White color
                0                   // Texture index (if applicable)
            }
        };

        // Generate a VAO handle to encapsulate the VBO
        GLuint vao_hdl;
        glCreateVertexArrays(1, &vao_hdl);
        glBindVertexArray(vao_hdl);

        // Create and bind a VBO to store the vertex data
        GLuint vbo_hdl;
        glCreateBuffers(1, &vbo_hdl);
        glNamedBufferStorage(vbo_hdl, sizeof(Vertex) * vertices.size(), vertices.data(), 0);

        // Create and bind a VBO to store the instance data
        GLuint instance_vbo_hdl;
        glCreateBuffers(1, &instance_vbo_hdl);
        glNamedBufferStorage(instance_vbo_hdl, sizeof(InstanceData) * instanceData.size(), instanceData.data(), 0);

        // Bind the VBO for vertices to the VAO
        glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(Vertex));

        // Bind the VBO for instance data to the VAO
        glVertexArrayVertexBuffer(vao_hdl, 1, instance_vbo_hdl, 0, sizeof(InstanceData));

        // Enable the position attribute for vertices
        glEnableVertexArrayAttrib(vao_hdl, 0);
        glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribBinding(vao_hdl, 0, 0);

        // Enable the texture coordinate attribute for vertices
        glEnableVertexArrayAttrib(vao_hdl, 2);
        glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(vao_hdl, 2, 0);

        // Enable the instance attributes
        // The model matrix is a 4x4 matrix (mat4), color is a vec3, and texIndex is an int
        glEnableVertexArrayAttrib(vao_hdl, 3); // Model Matrix
        glEnableVertexArrayAttrib(vao_hdl, 4); // Color
        glEnableVertexArrayAttrib(vao_hdl, 5); // Texture Index

        // Specify the format and bindings for instance attributes
        glVertexArrayAttribFormat(vao_hdl, 3, 4, GL_FLOAT, GL_FALSE, offsetof(InstanceData, modelXformMatrix));
        glVertexArrayAttribFormat(vao_hdl, 4, 3, GL_FLOAT, GL_FALSE, offsetof(InstanceData, color));
        glVertexArrayAttribIFormat(vao_hdl, 5, 1, GL_INT, offsetof(InstanceData, texIndex));

        // Specify which buffer binding point the instance attributes will use
        glVertexArrayAttribBinding(vao_hdl, 3, 1);
        glVertexArrayAttribBinding(vao_hdl, 4, 1);
        glVertexArrayAttribBinding(vao_hdl, 5, 1);

        // Set the divisor for the instance attributes to 1 (one set of attributes per instance)
        glVertexArrayBindingDivisor(vao_hdl, 1, 1);

        // Unbind the VAO (not necessary to unbind buffers individually)
        glBindVertexArray(0);

        // Save VAO and draw count
        vao_ID = vao_hdl;
        vbo_ID = vbo_hdl;
        instance_vbo_ID = instance_vbo_hdl;
        draw_count = static_cast<GLuint>(vertices.size());
    }

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
        //quad_mesh.setupInstancedQuadVAO();
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

    void Mesh::uploadInstanceData(const std::vector<InstanceData>& instanceData, Mesh const& mesh_used) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh_used.instance_vbo_ID);

        // Map the buffer for writing
        InstanceData* bufferData = (InstanceData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        if (bufferData) {
            // Copy the instance data to the mapped buffer
            std::memcpy(bufferData, instanceData.data(), instanceData.size() * sizeof(InstanceData));

            // Unmap the buffer
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        else {
            // Handle buffer mapping error
            std::cerr << "Failed to map the instance buffer for writing." << std::endl;
        }

        // Unbind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}