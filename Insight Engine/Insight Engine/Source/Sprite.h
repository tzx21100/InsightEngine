#pragma once
#include <GL/glew.h>
#include "Transform.h"
#include "Shader.h"
#include "Animation.h"
#include "Vector2D.h"

namespace IS {
    class Sprite : public IComponent {
    public:
        GLenum primitive_type{};
        Transform model_TRS{}; // transformation values
        uint8_t texture{};
        uint32_t texture_width{};
        uint32_t texture_height{};
        GLuint tex_ID{};
        int current_tex_index{}; // 0 is texture sprite initialized with

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
        void transform();
        //either manually set or follow transforms
        void followTransform(Transform trans) { model_TRS = trans; }
        void setWorldPos(Transform trans) { model_TRS.world_position = trans.world_position;}
        void setWorldPos(float x, float y) { model_TRS.world_position = glm::vec2(x, y); }
        void setSpriteSize(Transform trans) { model_TRS.scaling = trans.scaling; }
        void setSpriteSize(float width, float height) { model_TRS.scaling = glm::vec2(width, height); }
        // for M1 rubrics showcase
        void drawSpecial(const Mesh& mesh_used, Shader shader, GLuint texture_id = 0);
        void drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& animPointer, GLuint texture_id = 0);

        static void drawLine(Vector2D const& p0, Vector2D const& p1);
        static glm::mat3 lineTransform(Vector2D const& midpoint_translate, float rotate_angle_rad, float length_scale);


        Json::Value Serialize() override {
            Json::Value spriteData;

            // Serializing primitive_type
            spriteData["SpritePrimitiveType"] = primitive_type;

            // Serializing model_TRS (using the details of the Transform class)
            spriteData["SpriteTransformWorldPositionX"] = model_TRS.world_position.x;
            spriteData["SpriteTransformWorldPositionY"] = model_TRS.world_position.y;
            spriteData["SpriteTransformRotation"] = model_TRS.rotation;
            spriteData["SpriteTransformScalingX"] = model_TRS.scaling.x;
            spriteData["SpriteTransformScalingY"] = model_TRS.scaling.y;

            // Note: Not serializing mdl_to_ndc_xform as it's a matrix and the specific serialization might depend on further needs

            // Serializing texture-related properties
            spriteData["SpriteTexture"] = texture;
            spriteData["SpriteTextureWidth"] = texture_width;
            spriteData["SpriteTextureHeight"] = texture_height;
            spriteData["SpriteTexID"] = tex_ID;
            spriteData["SpriteCurrentTexIndex"] = current_tex_index;

            // Serializing imgui-related properties
            spriteData["SpriteName"] = name;
            spriteData["SpriteColorX"] = color.x;
            spriteData["SpriteColorY"] = color.y;
            spriteData["SpriteColorZ"] = color.z;
            spriteData["SpriteDrawing"] = drawing;

            return spriteData;
        }

        void Deserialize(Json::Value data) override {
            // Deserializing primitive_type
            primitive_type = data["SpritePrimitiveType"].asUInt();

            // Deserializing model_TRS
            model_TRS.world_position.x = data["SpriteTransformWorldPositionX"].asFloat();
            model_TRS.world_position.y = data["SpriteTransformWorldPositionY"].asFloat();
            model_TRS.rotation = data["SpriteTransformRotation"].asFloat();
            model_TRS.scaling.x = data["SpriteTransformScalingX"].asFloat();
            model_TRS.scaling.y = data["SpriteTransformScalingY"].asFloat();

            // Note: Not deserializing mdl_to_ndc_xform as it's a matrix and the specific deserialization might depend on further needs

            // Deserializing texture-related properties
            texture = static_cast<uint8_t>(data["SpriteTexture"].asUInt());
            texture_width = data["SpriteTextureWidth"].asUInt();
            texture_height = data["SpriteTextureHeight"].asUInt();
            tex_ID = data["SpriteTexID"].asUInt();
            current_tex_index = data["SpriteCurrentTexIndex"].asInt();

            // Deserializing imgui-related properties
            name = data["SpriteName"].asString();
            color.x = data["SpriteColorX"].asFloat();
            color.y = data["SpriteColorY"].asFloat();
            color.z = data["SpriteColorZ"].asFloat();
            drawing = data["SpriteDrawing"].asBool();
        }



    };
}