#include "Pch.h"

namespace IS {
	void Mesh::setupQuadVAO() {
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

        // Enable the texture coordinate attribute
        glEnableVertexArrayAttrib(vao_hdl, 2);
        glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));
        glVertexArrayAttribBinding(vao_hdl, 2, 0);

        // Set up the element buffer object (EBO) for indexing
        GLuint ebo_hdl;
        glCreateBuffers(1, &ebo_hdl);
        glNamedBufferStorage(ebo_hdl, sizeof(unsigned int) * indices.size(), indices.data(), 0);

        // Bind the EBO to the VAO
        glVertexArrayElementBuffer(vao_hdl, ebo_hdl);

        // Unbind the VAO (not necessary to unbind buffers individually)
        glBindVertexArray(0);

        vao_ID = vao_hdl;
        draw_count = static_cast<GLuint>(indices.size());
	}

	void Mesh::setupNonQuadVAO(GLenum mesh_primitive_type) {
        // Define the vertices of the quad

        std::vector<glm::vec2> pos_vtx;

        if (mesh_primitive_type == GL_POINTS) {
            pos_vtx = {
                glm::vec2(0.f, 0.f)
            };
            draw_count = static_cast<GLuint>(pos_vtx.size());
        }
        else if (mesh_primitive_type == GL_LINES) {
            pos_vtx = {
                glm::vec2(-1.f, 0.f),
                glm::vec2(1.f, 0.f)
            };
            draw_count = static_cast<GLuint>(pos_vtx.size());
        }
        else if (mesh_primitive_type == GL_TRIANGLE_FAN) {
            pos_vtx.emplace_back(glm::vec2(0.f, 0.f)); // center of circle

            int slices{ 30 };
            float angle{ (2 * static_cast<float>(PI) / slices) }; // 15 slices

            for (int i{}; i < slices + 1; ++i) {
                float x = (cosf(i * angle));
                float y = (sinf(i * angle));

                pos_vtx.emplace_back(glm::vec2(x, y));
            }
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

    /*void Mesh::init4Meshes() {
        quad_mesh.setupQuadVAO();
        points_mesh.setupNonQuadVAO(GL_POINTS);
        lines_mesh.setupNonQuadVAO(GL_LINES);
        circle_mesh.setupNonQuadVAO(GL_TRIANGLE_FAN);
    }*/

   /* void Mesh::cleanup4Meshes() {
        glDeleteVertexArrays(1, &quad_mesh.vao_ID);
        glDeleteVertexArrays(1, &points_mesh.vao_ID);
        glDeleteVertexArrays(1, &lines_mesh.vao_ID);
        glDeleteVertexArrays(1, &circle_mesh.vao_ID);
    }*/
}