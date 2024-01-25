/*!
 * \file Shader.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * This source file defines the Shader class, which encapsulates shader compilation,
 * linking, and usage in OpenGL applications.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"
#include "Graphics/Core/Graphics.h"
#include "Shader.h"

namespace IS {
    void Shader::compileAllShaders() {
        std::string directory = "Assets/Shaders/"; // directory shaders are placed in

        // Initializing all required shaders
        ISGraphics::quad_shader_pgm.compileShaderFromFile(GL_VERTEX_SHADER, directory + "InstQuad.vert");
        ISGraphics::quad_shader_pgm.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "InstQuad.frag");
        ISGraphics::quad_shader_pgm.link();
        ISGraphics::quad_shader_pgm.validate();

        ISGraphics::glitched_quad_shader_pgm.compileShaderFromFile(GL_VERTEX_SHADER, directory + "InstQuad.vert");
        ISGraphics::glitched_quad_shader_pgm.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "GlitchEffect.frag");
        ISGraphics::glitched_quad_shader_pgm.link();
        ISGraphics::glitched_quad_shader_pgm.validate();

        ISGraphics::non_quad_shader_pgm.compileShaderFromFile(GL_VERTEX_SHADER, directory + "InstNonQuad.vert");
        ISGraphics::non_quad_shader_pgm.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "InstNonQuad.frag");
        ISGraphics::non_quad_shader_pgm.link();
        ISGraphics::non_quad_shader_pgm.validate();

        ISGraphics::quad_border_shader_pgm.compileShaderFromFile(GL_VERTEX_SHADER, directory + "Lighting.vert");
        ISGraphics::quad_border_shader_pgm.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "Lighting.frag");
        ISGraphics::quad_border_shader_pgm.link();
        ISGraphics::quad_border_shader_pgm.validate();

        ISGraphics::light_shader_pgm.compileShaderFromFile(GL_VERTEX_SHADER, directory + "shadowQuad.vert");
        ISGraphics::light_shader_pgm.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "shadowQuad.frag");
        ISGraphics::light_shader_pgm.link();
        ISGraphics::light_shader_pgm.validate();

        Text::textShader.compileShaderFromFile(GL_VERTEX_SHADER, directory + "Text.vert");
        Text::textShader.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "Text.frag");
        Text::textShader.link();
        Text::textShader.validate();

        ISGraphics::fb_shader_pgm.compileShaderFromFile(GL_VERTEX_SHADER, directory + "fb.vert");
        ISGraphics::fb_shader_pgm.compileShaderFromFile(GL_FRAGMENT_SHADER, directory + "fb.frag");
        ISGraphics::fb_shader_pgm.link();
        ISGraphics::fb_shader_pgm.validate();
    }

    void Shader::setMainQuadShader(Shader const& shader) {
        ISGraphics::main_quad_shader = shader;
    }

    GLboolean Shader::compileShaderString(GLenum shader_type, std::string const& shader_src) {
        // Check if the shader program handle is already created; if not, create it
        if (pgm_hdl <= 0) {
            pgm_hdl = glCreateProgram();
            if (!pgm_hdl) {
                log = "Cannot create program handle.";
                return GL_FALSE;
            }
        }

        // Create a shader object of the specified type (vertex or fragment)
        GLuint shader_hdl{};
        switch (shader_type) {
        case GL_VERTEX_SHADER: 
            shader_hdl = glCreateShader(GL_VERTEX_SHADER); 
            break;
        case GL_FRAGMENT_SHADER: 
            shader_hdl = glCreateShader(GL_FRAGMENT_SHADER); 
            break;
        default:
            log = "Incorrect shader type.";
            return GL_FALSE;
        }
        // Load the shader source code
        char const* shader_cstr[] {shader_src.c_str()};
        glShaderSource(shader_hdl, 1, shader_cstr, NULL);

        // Compile the shader
        glCompileShader(shader_hdl);

        // Check the compilation result
        GLint compile_result;
        glGetShaderiv(shader_hdl, GL_COMPILE_STATUS, &compile_result);

        // If compilation failed, retrieve and log error messages
        if (compile_result == GL_FALSE) {
            GLint log_len;
            glGetShaderiv(shader_hdl, GL_INFO_LOG_LENGTH, &log_len);

            if (log_len > 0) {
                std::vector<GLchar> log_vect(log_len);
                GLsizei written_log_len;
                glGetShaderInfoLog(shader_hdl, log_len, &written_log_len, log_vect.data());

                log = "Vertex shader compilation failed\n" + std::string(log_vect.begin(), log_vect.begin() + written_log_len);
                IS_CORE_ERROR("Shader compilation failed!");
                IS_CORE_ERROR("{}", log);
            }
            // Clean up the shader object and return GL_FALSE
            glDeleteShader(shader_hdl);
            return GL_FALSE;
        }
        // Attach the compiled shader to the program and clean up the shader object
        glAttachShader(pgm_hdl, shader_hdl);
        glDeleteShader(shader_hdl);
        return GL_TRUE;
    }

    GLboolean FileExists(std::string const& file_name) {
        std::ifstream infile(file_name); return infile.good();
    }

    GLboolean Shader::compileShaderFromFile(GLenum shader_type, std::string const& file_name) {
        // Check if the file exists
        if (GL_FALSE == FileExists(file_name)) {
            IS_CORE_ERROR("Shader File not found.");
            return GL_FALSE;
        }
        // Check if the program handle is already created; if not, create it
        if (pgm_hdl <= 0) {
            pgm_hdl = glCreateProgram();
            if (0 == pgm_hdl) {
                IS_CORE_ERROR("Cannot create program handle.");
                return GL_FALSE;
            }
        }
        // Open the shader file
        std::ifstream shader_file(file_name, std::ifstream::in);
        if (!shader_file) {
            IS_CORE_ERROR("Error opening file: {}", file_name);
            return GL_FALSE;
        }
        // Read the shader source code from the file
        std::stringstream buffer;
        buffer << shader_file.rdbuf();
        shader_file.close();

        // Compile the shader using the source code
        return compileShaderString(shader_type, buffer.str());
    }

    GLboolean Shader::link() {
        // Check if the shader program has already been linked
        if (linked == GL_TRUE) return GL_TRUE;
        if (pgm_hdl <= 0) return GL_FALSE;

        // Link the shader program
        glLinkProgram(pgm_hdl);

        // Check the linking result
        GLint link_result;
        glGetProgramiv(pgm_hdl, GL_LINK_STATUS, &link_result);

        // If linking failed, retrieve and log error messages
        if (link_result == GL_FALSE) {
            GLint log_len;
            glGetProgramiv(pgm_hdl, GL_INFO_LOG_LENGTH, &log_len);

            if (log_len > 0) {
                std::vector<GLchar> log_vect(log_len);
                GLsizei written_log_len;
                glGetProgramInfoLog(pgm_hdl, log_len, &written_log_len, log_vect.data());

                log = "Failed to link shader program\n" + std::string(log_vect.begin(), log_vect.begin() + written_log_len);
                IS_CORE_ERROR({}, log);
            }
            return GL_FALSE;
        }

        // Set the linked flag to GL_TRUE
        linked = GL_TRUE;
        return linked;
    }

    void Shader::use() {
        // Activates the shader program for rendering
        if (pgm_hdl > 0 && linked == GL_TRUE) glUseProgram(pgm_hdl);
    }

    void Shader::unUse() {
        // Deactivates the currently active shader program
        glUseProgram(0);
    }

    GLboolean Shader::validate() {
        // Check if the shader program handle is valid and if it has been linked successfully
        if (pgm_hdl <= 0 || linked == GL_FALSE) return GL_FALSE;

        // Validate the shader program.
        glValidateProgram(pgm_hdl);
        GLint status;
        glGetProgramiv(pgm_hdl, GL_VALIDATE_STATUS, &status);

        // If validation fails, retrieve and log error messages
        if (status == GL_FALSE) {
            GLint log_len;
            glGetProgramiv(pgm_hdl, GL_INFO_LOG_LENGTH, &log_len);

            if (log_len > 0) {
                std::vector<GLchar> log_vect(log_len);
                GLsizei written_log_len;
                glGetProgramInfoLog(pgm_hdl, log_len, &written_log_len, log_vect.data());

                log = "Failed to validate shader program for current OpenGL context\n" + std::string(log_vect.begin(), log_vect.begin() + written_log_len);
                IS_CORE_ERROR({}, log);
            }
            return GL_FALSE;
        }
        return GL_TRUE;
    }

    GLuint Shader::getHandle() const {
        return pgm_hdl;
    }

    GLboolean Shader::isLinked() const {
        return linked;
    }

    std::string Shader::getLog() const {
        return log;
    }

    void Shader::bindAttribLocation(GLuint index, GLchar const* name) {
        glBindAttribLocation(pgm_hdl, index, name);
    }

    // checks if uniform location is available, then sets the value as provided
    void Shader::setUniform(GLchar const* name, GLboolean val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform1i(loc, val);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, GLint val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform1i(loc, val);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, GLfloat val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform1f(loc, val);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, GLfloat x, GLfloat y) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform2f(loc, x, y);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform3f(loc, x, y, z);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform4f(loc, x, y, z, w);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, glm::vec2 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform2f(loc, val.x, val.y);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, glm::vec3 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform3f(loc, val.x, val.y, val.z);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, glm::vec4 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniform4f(loc, val.x, val.y, val.z, val.w);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }


    void Shader::setUniform(GLchar const* name, glm::mat3 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::setUniform(GLchar const* name, glm::mat4 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::printAttribs() const {
        // Prints the names and indices of active vertex attributes in the shader program.
        GLint max_length{}, num_attribs{};
        glGetProgramiv(pgm_hdl, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
        glGetProgramiv(pgm_hdl, GL_ACTIVE_ATTRIBUTES, &num_attribs);

        std::cout << "Index\t|\tName\n";
        std::cout << "----------------------------------------------------------------------\n";

        std::vector<GLchar> pname(max_length);

        for (GLint i{}; i < num_attribs; ++i) {
            GLsizei written{};
            GLint size{};
            GLenum type{};
            glGetActiveAttrib(pgm_hdl, i, max_length, &written, &size, &type, pname.data());
            GLint loc = glGetAttribLocation(pgm_hdl, pname.data());
            std::cout << loc << "\t\t" << pname.data() << std::endl;
        }
        std::cout << "----------------------------------------------------------------------\n";
    }

    void Shader::printUniforms() const {
        // Prints the names and locations of active uniform variables in the shader program
        GLint max_length{};
        glGetProgramiv(pgm_hdl, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

        std::vector<GLchar> pname(max_length);
        GLint num_uniforms{};
        glGetProgramiv(pgm_hdl, GL_ACTIVE_UNIFORMS, &num_uniforms);

        std::cout << "Location\t|\tName\n";
        std::cout << "----------------------------------------------------------------------\n";

        for (GLint i = 0; i < num_uniforms; ++i) {
            GLsizei written{};
            GLint size{};
            GLenum type{};
            glGetActiveUniform(pgm_hdl, i, max_length, &written, &size, &type, pname.data());
            GLint loc = glGetUniformLocation(pgm_hdl, pname.data());
            std::cout << loc << "\t\t" << pname.data() << std::endl;
        }
        std::cout << "----------------------------------------------------------------------\n";
    }
}