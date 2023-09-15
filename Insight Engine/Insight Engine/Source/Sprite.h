#pragma once
#include <GL/glew.h>
#include "Transform.h"
#include "Shader.h"

//enum class SpriteType {
//    Quad,
//    Point,
//    Line,
//    Circle
//};

namespace IS {
    class Sprite : public IComponent {
    public:
        GLenum primitive_type{};
        Transform model_TRS{}; // transformation values

        // imgui
        std::string name;
        glm::vec3 color{};
        bool drawing{ true };

        Sprite() {
            name = "Box";
            primitive_type = GL_TRIANGLES;
        }

        Sprite(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive) {
            PRNG prng;
            for (int i{}; i < 3; ++i) {
                color[i] = prng.generate();
            }
        }
        void transform(float delta_time);
        // for M1 rubrics showcase
        void drawSpecial(Mesh mesh_used, Shader shader, GLuint texture_id = 0);
    };
}