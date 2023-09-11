#pragma once
/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
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
    /*  _________________________________________________________________________ */
    class Shader
    {
    public:
        // default ctor required to initialize GLSLShader object to safe state
        Shader() : pgm_hdl(0), linked(GL_FALSE) {}

        GLboolean compileShaderString(GLenum shader_type, std::string const& shader_src);

        // Link shader objects attached to handle pgm_handle. This member function
        // will also verify the status of the link operation (successful or not?).
        // If the shader objects did not link into a program object, then the
        // member function must retrieve and write the program object's information
        // log to data member log_string. 
        GLboolean link();

        // Install the shader program object whose handle is encapsulated
        // by member pgm_handle
        void use();

        // De-install previously installed shader program object using Use().
        // More correctly, after the execution of this member function, the
        // current rendering state is referring to an invalid program object.
        void unUse();

        // check whether the executable shader program object can execute given the
        // current OpenGL state ...
        // See the glValidateProgram() reference page for more information
        // The function returns true if validatation succeeded 
        GLboolean validate();

        // return the handle to the shader program object
        GLuint getHandle() const;

        // have the different object code linked into a shader program?
        GLboolean isLinked() const;

        // return logged information from the GLSL compiler and linker and
        // validation information obtained after calling Validate() ...
        std::string getLog() const;

        // Use an OpenGL API function to dynamically associate a generic vertex 
        // attribute index with a named in attribute variable.
        // Note that labs and assignment sample code did not use the OpenGL API to
        // provide a connection between an input value to a shader and the slot in
        // which these values are fetched from buffer object to shaders. Instead, 
        // the layout qualifier was used in our vertex shaders to statically
        // provide indices to attribute values 
        // But, if users decide to forego the use of the layout qualifier, they'll
        // instead use this function to provide the association between a generic
        // vertex attribute index with a named attribute variable.
        void bindAttribLocation(GLuint index, GLchar const* name);

        // OpenGL is C-based API and therefore doesn't understand function
        // overloading or templates
        // Therefore, we need a family of functions to specify values of uniform
        // variables of different types for the current program object
        void setUniform(GLchar const* name, GLboolean val);
        void setUniform(GLchar const* name, GLint val);
        void setUniform(GLchar const* name, GLfloat val);
        void setUniform(GLchar const* name, GLfloat x, GLfloat y);
        void setUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
        void setUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void setUniform(GLchar const* name, glm::vec2 const& val);
        void setUniform(GLchar const* name, glm::vec3 const& val);
        void setUniform(GLchar const* name, glm::mat3 const& val);

        // display the list of active vertex attributes used by vertex shader
        void printAttribs() const;

        // display the list of active uniform variables
        void printUniforms() const;

    private:

        GLuint pgm_hdl = 0;  // handle to linked shader program object
        GLboolean linked = GL_FALSE; // has the program successfully linked?
        std::string log; // log for OpenGL compiler and linker messages
    };
}
