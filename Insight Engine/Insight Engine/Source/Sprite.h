/*!
 * \file Sprite.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file defines the Sprite class, which represents a 2D sprite with transformation, rendering properties,
 * and animation capabilities.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SPRITE_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SPRITE_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <glad/glad.h>
#include "Transform.h"
#include "Shader.h"
#include "Animation.h"
#include "Vector2D.h"
#include <vector>

namespace IS {
    /*!
     * \brief The Sprite class represents a 2D sprite with transformation and rendering properties.
     *
     * The Sprite class encapsulates a 2D sprite with various properties such as transformation, rendering settings,
     * and animation capabilities. It allows you to create and manage 2D sprites for use in OpenGL applications.
     */

    class Sprite : public IComponent {
    public:
        static std::string GetType(){
            return "Sprite";
        }

        struct instanceData {
            glm::vec3 color{};
            float tex_index{};
            glm::mat3 model_to_ndc_xform{};
            glm::vec2 anim_frame_dimension{ 1.f, 1.f }; // default UV size
            glm::vec2 anim_frame_index{ 0.f, 0.f };
            float entID{}; // initialize with invalid entity id
        };

        struct lineInstanceData {
            glm::vec3 color{};
            glm::mat3 model_to_ndc_xform{};
        };

        static int texture_count;

        GLenum primitive_type{};   // The rendering primitive type for the sprite (e.g., GL_TRIANGLE_STRIP).
        Transform model_TRS{};     // Transformation values for the sprite.
        //uint8_t texture_id{};      // The texture ID for the sprite. (randomly given by OpenGL)
        int animation_index{};     // The current texture index for switching animations (0 is the default texture).
        // std::vector<Animation> anim_vect{};
        Image img{};
        // Animation anim{};
        std::vector<Animation> anims{};


        // ImGui properties
        std::string name;          // The name of the sprite.
        glm::vec3 color{};         // The color of the sprite.
        bool drawing{ true };      // Whether the sprite is currently set to be drawn.

        /*!
         * \brief Default constructor for the Sprite class.
         *
         * Initializes the sprite with default values.
         */
        int sscounter=0;
        Sprite() {
            name = "Box";
            primitive_type = GL_TRIANGLE_STRIP;
            PRNG& prng = PRNG::Instance();
            for (int i{}; i < 3; ++i) {
                color[i] = prng.generate();
            }
            // Give it a default size of 1 by 1
            setSpriteSize(1, 1);
            setWorldPos(0, 0);
        }

        /*!
         * \brief Constructor for the Sprite class with specified parameters.
         *
         * Initializes the sprite with a specified name and primitive type.
         *
         * \param model_name The name of the sprite.
         * \param primitive The rendering primitive type for the sprite.
         */
        Sprite(std::string const& model_name, GLenum primitive) : name(model_name), primitive_type(primitive) {
            PRNG& prng = PRNG::Instance();
            for (int i{}; i < 3; ++i) {
                color[i] = prng.generate();
            }
        }

        // Copy constructor
        Sprite(const Sprite& other)
            : primitive_type(other.primitive_type),
            model_TRS(other.model_TRS),
            animation_index(other.animation_index),
            img(other.img), // Directly using the default copy for Image. You might need to adjust this if you add a custom copy constructor for Image.
            anims(other.anims),
            name(other.name),
            color(other.color),
            drawing(other.drawing),
            sscounter(other.sscounter)
        {
            img.texture_id = other.img.texture_id;
        }


        /*!
         * \brief Applies the transformation to the sprite.
         *
         * This function calculates the transformation matrix for the sprite based on its position, rotation, and scaling.
         */
        void transform();

        /*!
         * \brief Sets the sprite's transformation to follow another transformation.
         *
         * \param trans The transformation to follow.
         */
        void followTransform(Transform trans) { model_TRS = trans; }

        /*!
         * \brief Sets the world position of the sprite.
         *
         * \param trans The world position to set.
         */
        void setWorldPos(Transform trans) { model_TRS.world_position = trans.world_position; }
        void setWorldPos(float x, float y) { model_TRS.world_position = Vec2D(x, y); }

        /*!
         * \brief Sets the size of the sprite.
         *
         * \param trans The size to set.
         */
        void setSpriteSize(Transform trans) { model_TRS.scaling = trans.scaling; }
        void setSpriteSize(float width, float height) { model_TRS.scaling = Vec2D(width, height); }

        /*!
         * \brief Draws a special sprite.
         *
         * This function draws a special sprite using the provided mesh, shader, and texture ID.
         *
         * \param mesh_used The mesh to use for rendering.
         * \param shader The shader to use for rendering.
         * \param texture_id The texture ID to use for rendering.
         */
        void drawSprite(const Mesh& mesh_used, Shader shader, GLuint texture_id = 0);

        /*!
         * \brief Draws an animated sprite.
         *
         * This function draws an animated sprite using the provided mesh, shader, animation, and texture ID.
         *
         * \param mesh_used The mesh to use for rendering.
         * \param shader The shader to use for rendering.
         * \param animPointer The pointer to the animation to use for rendering.
         * \param texture_id The texture ID to use for rendering.
         */
        void drawAnimation(const Mesh& mesh_used, Shader shader, Animation const& animPointer, GLuint texture_id = 0);

        /*!
         * \brief Draws a line between two points.
         *
         * This static function draws a line between two specified points.
         *
         * \param p0 The first point of the line.
         * \param p1 The second point of the line.
         * \param color The color of the line, white by default. TODO: Replace with Vector3D
         * \param thickness The thickness of the line.
         */
        static void drawLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& color = { 1.f, 1.f, 1.f }, float thickness = 1.f);

        /*!
         * \brief Calculates the transformation matrix for a line sprite.
         *
         * This static function calculates the transformation matrix for a line sprite based on
         * midpoint translation, rotation angle, and length scale.
         *
         * \param midpoint_translate The midpoint translation.
         * \param rotate_angle_rad The rotation angle in radians.
         * \param length_scale The length scaling factor.
         * \return The transformation matrix for the line sprite.
         */
        // static glm::mat3 lineTransform(Vector2D const& midpoint_translate, float rotate_angle_rad, float length_scale);

        /*!
         * \brief Deserializes the sprite data
         *
         * \param Json::Value the data to deserialize
         */
        void Deserialize(Json::Value data) override;

        /*!
         * \brief Serializes the sprite data
         */
        Json::Value Serialize() override;


        static void draw_instanced_lines();
        static void draw_instanced_quads();
    };
}
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SPRITE_H