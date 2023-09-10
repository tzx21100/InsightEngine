//pch has to go to the top of every cpp
#include "Pch.h"
#include "Shader.h"

namespace IS {
    GLboolean Shader::compileShaderString(GLenum shader_type, std::string const& shader_src) {
        if (pgm_hdl <= 0) {
            pgm_hdl = glCreateProgram();
            if (!pgm_hdl) {
                log = "Cannot create program handle.";
                return GL_FALSE;
            }
        }

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

        char const* shader_cstr[] {shader_src.c_str()};
        glShaderSource(shader_hdl, 1, shader_cstr, NULL);

        glCompileShader(shader_hdl);

        GLint compile_result;
        glGetShaderiv(shader_hdl, GL_COMPILE_STATUS, &compile_result);

        if (compile_result == GL_FALSE) {
            GLint log_len;
            glGetShaderiv(shader_hdl, GL_INFO_LOG_LENGTH, &log_len);

            if (log_len > 0) {
                std::vector<GLchar> log_vect(log_len);
                GLsizei written_log_len;
                glGetShaderInfoLog(shader_hdl, log_len, &written_log_len, log_vect.data());

                log = "Vertex shader compilation failed\n" + std::string(log_vect.begin(), log_vect.begin() + written_log_len);
            }
            glDeleteShader(shader_hdl);
            return GL_FALSE;
        }
        glAttachShader(pgm_hdl, shader_hdl);
        glDeleteShader(shader_hdl);
        return GL_TRUE;
    }

    GLboolean Shader::link() {
        if (linked == GL_TRUE) return GL_TRUE;
        if (pgm_hdl <= 0) return GL_FALSE;

        glLinkProgram(pgm_hdl);

        GLint link_result;
        glGetProgramiv(pgm_hdl, GL_LINK_STATUS, &link_result);

        if (link_result == GL_FALSE) {
            GLint log_len;
            glGetProgramiv(pgm_hdl, GL_INFO_LOG_LENGTH, &log_len);

            if (log_len > 0) {
                std::vector<GLchar> log_vect(log_len);
                GLsizei written_log_len;
                glGetProgramInfoLog(pgm_hdl, log_len, &written_log_len, log_vect.data());

                log = "Failed to link shader program\n" + std::string(log_vect.begin(), log_vect.begin() + written_log_len);
            }
            return GL_FALSE;
        }

        linked = GL_TRUE;
        return linked;
    }

    void Shader::use() {
        if (pgm_hdl > 0 && linked == GL_TRUE) glUseProgram(pgm_hdl);
    }

    void Shader::unUse() {
        glUseProgram(0);
    }

    GLboolean Shader::validate() {
        if (pgm_hdl <= 0 || linked == GL_FALSE) return GL_FALSE;

        glValidateProgram(pgm_hdl);
        GLint status;
        glGetProgramiv(pgm_hdl, GL_VALIDATE_STATUS, &status);

        if (status == GL_FALSE) {
            GLint log_len;
            glGetProgramiv(pgm_hdl, GL_INFO_LOG_LENGTH, &log_len);

            if (log_len > 0) {
                std::vector<GLchar> log_vect(log_len);
                GLsizei written_log_len;
                glGetProgramInfoLog(pgm_hdl, log_len, &written_log_len, log_vect.data());

                log = "Failed to validate shader program for current OpenGL context\n" + std::string(log_vect.begin(), log_vect.begin() + written_log_len);
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

    void Shader::setUniform(GLchar const* name, glm::mat3 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
        else std::cout << "Uniform variable " << name << " doesn't exist.\n";
    }

    void Shader::printAttribs() const {
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