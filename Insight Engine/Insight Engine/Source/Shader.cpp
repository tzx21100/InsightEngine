/*!
 * \file Shader.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
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
#include "Shader.h"

namespace IS {
    void Shader::setupInstSpriteShaders() {
        // vertex shader
        std::string vtx_shdr = R"(
            #version 450 core
            layout(location = 0) in vec2  aVertexPosition;
            layout(location = 1) in vec2  aVertexTexCoord;
            layout(location = 2) in vec3  aVertexColor;
            layout(location = 3) in float aTexIndex;
            layout(location = 4) in vec3  aMtxRow1;
            layout(location = 5) in vec3  aMtxRow2;
            layout(location = 6) in vec3  aMtxRow3;
            layout(location = 7) in vec2  aAnimDim;
            layout(location = 8) in vec2  aAnimIndex;
            layout(location = 9) in float aEntityID;

            layout(location = 0) out vec3  vColor;
            layout(location = 1) out vec2  vTexCoord;
            layout(location = 2) out float vTexID;
            layout(location = 3) out vec2  vAnimDim;
            layout(location = 4) out vec2  vAnimIndex;
            layout(location = 5) out flat float vEntityID;

            void main()
            {
                mat3 model_to_NDC_xform = mat3(aMtxRow1, aMtxRow2, aMtxRow3);
                //model_to_NDC_xform = transpose(model_to_NDC_xform);
                gl_Position = vec4(vec2(model_to_NDC_xform * vec3(aVertexPosition, 1.0)), 0.0, 1.0);
		        vColor = aVertexColor;  
                vTexCoord = aVertexTexCoord;
                vTexID = aTexIndex;
                vAnimDim = aAnimDim;
                vAnimIndex = aAnimIndex;
                vEntityID = aEntityID;
            }
        )";

        // fragment shader
        std::string frag_shdr = R"(
            #version 450 core
            layout(location = 0) in vec3  vColor;
            layout(location = 1) in vec2  vTexCoord;
            layout(location = 2) in float vTexID;
            layout(location = 3) in vec2  vAnimDim;
            layout(location = 4) in vec2  vAnimIndex;
            layout(location = 5) in flat float vEntityID;

            layout(location = 0) out vec4 fFragColor;
            layout(location = 1) out int fEntityID;

            uniform sampler2D uTex2d[32];
  
            void main()
            {
                bool textured = false;
                if (vTexID >= 0) textured = true;
                if (!textured)
                {
                    fFragColor = vec4(vColor, 1.0); // Use vColor if no texture is bound
                }
                else
                {
                    int texIdx = int(vTexID);
                    fFragColor = texture(uTex2d[texIdx], vec2(vTexCoord.x * vAnimDim.x, vTexCoord.y * vAnimDim.y) + vec2(vAnimDim.x * vAnimIndex.x, vAnimDim.y * vAnimIndex.y));
                    //fFragColor = texture(uTex2d[texIdx], vec2(vTexCoord.x * 1, vTexCoord.y * 1) + vec2(1 * vAnimIndex.x, 1 * vAnimIndex.y));
                    //fFragColor = texture(uTex2d[texIdx], vTexCoord);
                }
                int id = int(vEntityID);
                fEntityID = id;
            }
        )";


        // Compile and link the shaders into a shader program
        compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        link();
        validate();

        // Check if the shader program compilation and linking was successful
        if (GL_FALSE == isLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << getLog() << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void Shader::setupInstLineShaders() {
        // vertex shader
        std::string vtx_shdr = R"(
            #version 450 core
            layout(location = 0) in vec2  aVertexPosition;
            layout(location = 2) in vec3  aVertexColor;
            layout(location = 4) in vec3  aMtxRow1;
            layout(location = 5) in vec3  aMtxRow2;
            layout(location = 6) in vec3  aMtxRow3;

            layout(location = 0) out vec3  vColor;

            void main()
            {
                mat3 model_to_NDC_xform = mat3(aMtxRow1, aMtxRow2, aMtxRow3);
                gl_Position = vec4(vec2(model_to_NDC_xform * vec3(aVertexPosition, 1.0)), 0.0, 1.0);
				vColor = aVertexColor;
            }
        )";

        // fragment shader
        std::string frag_shdr = R"(
            #version 450 core
            layout(location = 0) in vec3 vColor;
            layout(location = 0) out vec4 fFragColor;

            void main()
            {
                fFragColor = vec4(vColor, 1.0); // Use vColor if no texture is bound
            }
        )";

        // Compile and link the shaders into a shader program
        compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        link();
        validate();

        // Check if the shader program compilation and linking was successful
        if (GL_FALSE == isLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << getLog() << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void Shader::setupSpriteShaders() {
        // vertex shader
        std::string vtx_shdr = R"(
            #version 450 core
            layout(location = 0) in vec2 aVertexPosition;
            layout(location = 2) in vec2 aTexCoord;
            out vec2 vTexCoord;

            uniform mat3 uModel_to_NDC;

            void main()
            {
                gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.0)), 0.0, 1.0);
                vTexCoord = aTexCoord;
            }
        )";

        // fragment shader
        std::string frag_shdr = R"(
            #version 450 core

            layout(location = 0) out vec4 fFragColor;
            uniform vec3 uColor;
            uniform sampler2D uTex2d;
            in vec2 vTexCoord; // Input variable for texture coordinates
            uniform int uTexture; // Flag to indicate whether to use texture or color

            // animation uniforms - default unless drawing animation
            uniform vec2 uFrameDim = vec2(1.0, 1.0); 
            uniform vec2 uFrameIndex = vec2(0.0, 0.0);

            void main()
            {
                if (uTexture == 0)
                {
                    fFragColor = vec4(uColor, 1.0); // Use uColor if no texture is bound
                }
                else
                {
                    //fFragColor = texture(uTex2d, vTexCoord); // Multiply texture color with uColor
                    fFragColor = texture(uTex2d, vec2(vTexCoord.x * uFrameDim.x, vTexCoord.y * uFrameDim.y) + vec2(uFrameDim.x * uFrameIndex.x, uFrameDim.y * uFrameIndex.y)); // Multiply texture color with uColor
                }
            }
        )";

        // Compile and link the shaders into a shader program
        compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        link();
        validate();

        // Check if the shader program compilation and linking was successful
        if (GL_FALSE == isLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << getLog() << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void Shader::setupTextShaders() {
        // vertex shader
        std::string vtx_shdr = R"(
            #version 450 core
            layout (location = 0) in vec2 vertex;

            out VS_OUT{
                vec2 TexCoords;
                flat int index;
            } vs_out;

            uniform mat4 transforms[100]; // 400 chars per render
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * transforms[gl_InstanceID] * vec4(vertex.xy, 0.0, 1.0);
                vs_out.index = gl_InstanceID; // get index to feed to frag shader
                vs_out.TexCoords = vertex.xy;
                vs_out.TexCoords.y = 1.0f-vs_out.TexCoords.y; // flip
            }
        )";

        // fragment shader
        std::string frag_shdr = R"(
            #version 450 core

            out vec4 color;

            in VS_OUT{
                vec2 TexCoords;
                flat int index;
            } fs_in;

            uniform sampler2DArray text; // texture array
            uniform int letterMap[400];
            uniform vec3 textColor;

            void main()
            {    
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vec3(fs_in.TexCoords.xy,letterMap[fs_in.index])).r);
                color = vec4(textColor, 1.0) * sampled;
            }
        )";

        // Compile and link the shaders into a shader program
        compileShaderString(GL_VERTEX_SHADER, vtx_shdr);
        compileShaderString(GL_FRAGMENT_SHADER, frag_shdr);
        link();
        validate();

        // Check if the shader program compilation and linking was successful
        if (GL_FALSE == isLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << getLog() << "\n";
            exit(EXIT_FAILURE);
        }
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

    void Shader::setUniform(GLchar const* name, glm::mat3 const& val) {
        GLint loc = glGetUniformLocation(pgm_hdl, name);
        if (loc >= 0) glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
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