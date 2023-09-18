#pragma once
#include <GL/glew.h>
#include "Transform.h"
#include "Shader.h"
#include "Animation.h"

namespace IS {
    class Sprite : public IComponent {
    public:
        GLenum primitive_type{};
        Transform model_TRS{}; // transformation values
        uint8_t texture{};
        uint32_t texture_width{};
        uint32_t texture_height{};
        GLuint tex_ID{};

        // imgui
        std::string name;
        glm::vec3 color{};
        bool drawing{ true };

        Sprite() {
            name = "Box";
            primitive_type = GL_TRIANGLE_STRIP;
            PRNG prng;
            for (int i{}; i < 3; ++i) {
                color[i] = prng.generate();
            }
            //give it a default size of 1 by 1
            setSpriteSize(1, 1);
            setWorldPos(0, 0);

        }

        Sprite(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive) {
            PRNG prng;
            for (int i{}; i < 3; ++i) {
                color[i] = prng.generate();
            }
        }
        void transform(float delta_time);
        //either manually set or follow transforms
        void followTransform(Transform trans) { model_TRS = trans; }
        void setWorldPos(Transform trans) { model_TRS.world_position = trans.world_position;}
        void setWorldPos(float x, float y) { model_TRS.world_position = glm::vec2(x, y); }
        void setSpriteSize(Transform trans) { model_TRS.scaling = trans.scaling; }
        void setSpriteSize(float width, float height) { model_TRS.scaling = glm::vec2(width, height); }
        // for M1 rubrics showcase
        void drawSpecial(const Mesh& mesh_used, Shader shader, GLuint texture_id = 0);
        void drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& animPointer, GLuint texture_id = 0);
    };
}