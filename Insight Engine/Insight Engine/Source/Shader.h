/*!
 * \file Shader.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file defines the Shader class, which encapsulates shader compilation,
 * linking, and usage in OpenGL applications.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADER_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

namespace IS {
    /*!
     * \brief The Shader class encapsulates shader compilation, linking, and usage.
     *
     * The Shader class provides a convenient interface for managing OpenGL shaders.
     * It allows the compilation and linking of vertex and fragment shaders, setting
     * uniform values, and using shader programs.
     */
    class Shader {
    public:
        void setupInstSpriteShaders();
        void setupInstLineShaders();
        /*!
         * \brief Creates and sets up shaders for sprite rendering.
         *
         * This function creates and sets up shaders for rendering sprites. It compiles
         * both the vertex and fragment shaders and links them into a program.
         */
        void setupSpriteShaders();

        /*!
         * \brief Creates and sets up shaders for text rendering.
         *
         * This function creates and sets up shaders for rendering text. It compiles both
         * the vertex and fragment shaders and links them into a program.
         */
        void setupTextShaders();

        /*!
         * \brief Default constructor required to initialize Shader object to a safe state.
         */
        Shader() : pgm_hdl(0), linked(GL_FALSE) {}

        /*!
         * \brief Compiles a shader from a provided source string.
         *
         * \param shader_type The type of shader to compile (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
         * \param shader_src The source code of the shader as a string.
         * \return GL_TRUE if compilation was successful, GL_FALSE otherwise.
         */
        GLboolean compileShaderString(GLenum shader_type, std::string const& shader_src);

        /*!
         * \brief Links the shader program.
         *
         * This function links the shader program object with the attached shader objects.
         * It also verifies the status of the link operation and sets the log string accordingly.
         * \return GL_TRUE if linking was successful, GL_FALSE otherwise.
         */
        GLboolean link();

        /*!
         * \brief Installs the shader program for use in rendering.
         */
        void use();

        /*!
         * \brief De-installs the currently used shader program.
         *
         * After this function is called, the rendering state refers to an invalid program object.
         */
        void unUse();

        /*!
         * \brief Validates the shader program.
         *
         * This function validates the shader program against the current OpenGL state.
         * \return GL_TRUE if validation was successful, GL_FALSE otherwise.
         */
        GLboolean validate();

        /*!
         * \brief Retrieves the handle of the shader program.
         * \return The handle of the shader program.
         */
        GLuint getHandle() const;

        /*!
         * \brief Checks if the shader program is successfully linked.
         * \return GL_TRUE if the program is linked, GL_FALSE otherwise.
         */
        GLboolean isLinked() const;

        /*!
         * \brief Retrieves the log information from shader compilation and linking.
         * \return A string containing the compilation and linking log.
         */
        std::string getLog() const;

        /*!
         * \brief Binds a vertex attribute location.
         *
         * This function binds a generic vertex attribute index with a named attribute variable.
         * \param index The index to bind.
         * \param name The name of the attribute variable.
         */
        void bindAttribLocation(GLuint index, GLchar const* name);

        /*!
         * \brief Overloaded function to set various uniform variable types.
         *
         * This function is overloaded to set uniform variables of different types for the current program object.
         * \param name The name of the uniform variable.
         * \param val The value to set (varies based on the type).
         */
        void setUniform(GLchar const* name, GLboolean val);
        void setUniform(GLchar const* name, GLint val);
        void setUniform(GLchar const* name, GLfloat val);
        void setUniform(GLchar const* name, GLfloat x, GLfloat y);
        void setUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
        void setUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void setUniform(GLchar const* name, glm::vec2 const& val);
        void setUniform(GLchar const* name, glm::vec3 const& val);
        void setUniform(GLchar const* name, glm::mat3 const& val);

        /*!
         * \brief Prints the list of active vertex attributes used by the vertex shader.
         */
        void printAttribs() const;

        /*!
         * \brief Prints the list of active uniform variables.
         */
        void printUniforms() const;

    private:
        GLuint pgm_hdl = 0;          // Handle to the linked shader program object.
        GLboolean linked = GL_FALSE; // Flag indicating whether the program is successfully linked.
        std::string log;             // Log for OpenGL compiler and linker messages.
    };
}

#endif // GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SHADER_H
