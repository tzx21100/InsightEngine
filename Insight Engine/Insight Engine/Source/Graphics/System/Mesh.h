/*!
 * \file Mesh.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * This header file defines the Mesh class, which represents OpenGL vertex array objects and buffers for rendering.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_MESH_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_MESH_H

#include "Engine/Core/Core.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace IS {
    /*!
     * \brief The Mesh class represents OpenGL vertex array objects and buffers for rendering.
     *
     * The Mesh class encapsulates the OpenGL vertex array object (VAO) and associated vertex buffer object (VBO) used
     * for rendering. It provides functions for setting up different types of meshes and initializing and cleaning up
     * meshes for rendering.
     */
    class IS_API Mesh {
    public:
        /*!
         * \brief Enumeration of attribute locations within the shader program.
         */
        enum attribs {
            pos_attrib,
            tex_coord_attrib,
            color_attrib,
            tex_index_attrib,
            x_form_row1_attrib, // shader only able to take in max 4 values at a time
            x_form_row2_attrib, // so we will have to break a 3x3 matrix into 3 vec3
            x_form_row3_attrib,
            x_form_row4_attrib,
            anim_dim_attrib,
            anim_index_attrib,
            ent_ID_attib
        };

        GLuint vao_ID{};            // The OpenGL vertex array object ID.
        GLuint vbo_ID{};            // The OpenGL vertex buffer object ID.
        GLuint instance_vbo_ID{};   // The OpenGL instance vertex buffer object ID.
        GLuint draw_count{};        // The number of vertices to be drawn.

        /*!
         * \brief Represents a vertex with position and texture coordinates.
         */
        struct Vertex {
            glm::vec2 position; // The vertex position.
            glm::vec2 texCoord; // The texture coordinates.
        };

        /*!
         * \brief Initializes various types of meshes for rendering.
         *
         * Initializes the necessary VAOs, VBOs abd IVBOs for rendering a quad, line, and circle mesh.
         *
         * \param meshes A vector of Mesh objects to store the initialized meshes.
         */
        static void initMeshes(std::vector<Mesh>& meshes);

        /**
         * @brief Set up an instanced 3D quad mesh.
         */
        void setupInstanced3DQuadVAO();

        /*!
         * \brief Set up an instanced line mesh.
         *
         * This function initializes the vertex and instance data for rendering an instanced line.
         */
        void setupInstancedLineVAO();

        /*!
        * \brief Set up an instanced circle mesh.
        *
        * This function initializes the vertex and instance data for rendering an instanced circle.
        */
        void setupInstancedCircleVAO();

        /**
         * @brief Set up an outline mesh.
         */
        void setupOutlineVAO();

        /**
         * @brief Set up a frame buffer mesh.
         */
        void setupFBVAO();

    
        /*!
         * \brief Cleans up the OpenGL resources associated with the initialized meshes.
         *
         * Deletes the vertex arrays and buffers used for rendering the initialized meshes.
         *
         * \param meshes A vector of Mesh objects containing the initialized meshes to be cleaned up.
         */
        static void cleanupMeshes(std::vector<Mesh>& meshes);

        /// BELOW FUNCTIONS ARE UNUSED AFTER ADDING INSTANCING ///

        /*!
        * \brief Set up an instanced quad mesh.
        *
        * This function initializes the vertex and instance data for rendering an instanced quad.
        */
        void setupInstancedQuadVAO();

        /*!
         * \brief Sets up a quad mesh with predefined vertices and texture coordinates.
         *
         * Sets up a quad mesh with vertices and texture coordinates for rendering.
         */
        void setupQuadVAO();

        /*!
         * \brief Sets up a non-quad mesh (points, lines, or circle) based on the specified primitive type.
         *
         * Sets up a non-quad mesh (points, lines, or circle) with vertices for rendering based on the specified
         * primitive type.
         *
         * \param mesh_primitive_type The OpenGL primitive type for the non-quad mesh.
         */
        void setupNonQuadVAO(GLenum mesh_primitive_type);
    };
} // end namespace IS
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_MESH_H
