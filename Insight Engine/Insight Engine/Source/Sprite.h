/*!
 * \file Sprite.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
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
        // Instance data of quad entity (static color, textured, animated)
        struct instanceData {
            glm::vec4 color{};
            float tex_index{-1.f};
            glm::mat3 model_to_ndc_xform{};
            glm::vec2 anim_frame_dimension{ 1.f, 1.f }; // default UV size
            glm::vec2 anim_frame_index{ 0.f, 0.f };
            float entID{}; // initialize with invalid entity id
            int layer{};
        };

        struct instanceData3D {
            glm::vec4 color{};
            float tex_index{-1};
            glm::mat4 model_to_ndc_xform{};
            glm::vec2 anim_frame_dimension{ 1.f, 1.f }; // default UV size
            glm::vec2 anim_frame_index{ 0.f, 0.f };
            float entID{}; // initialize with invalid entity id
            int layer{};
        };
        
        // Instance data of debug non-quad entity (line, circle)
        struct nonQuadInstanceData {
            glm::vec4 color{};
            glm::mat4 model_to_ndc_xform{};
        };

        // Member variables
        GLenum primitive_type{};        // The rendering primitive type for the sprite (e.g., GL_TRIANGLE_STRIP).
        Transform model_TRS{};          // Transformation values for the sprite.
        int animation_index{};          // The current texture index for switching animations (0 is the default texture).
        Image img{};                    // Texture Object
        std::vector<Animation> anims{}; // Vector of animations attached to this sprite
        bool toRender{ true };          // Flag to control rendering
        int layer{ DrawLayer::DEFAULT_LAYER };                    // Layer value (lower is further back)
        // ImGui properties
        std::string name;               // The name of the sprite.
        glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };              // The color of the sprite.

        // For layering //
        enum DrawLayer : int
        {
            BACKGROUND_LAYER = 0,
            DEFAULT_LAYER,
            FOREGROUND_LAYER,
            UI_LAYER,
            INVALID_LAYER // always the last
        };
        // Static data member for layers management
        static std::unordered_set<int> layersToIgnore;
        static void toggleLayer(DrawLayer layer);
        static std::string LayerToString(DrawLayer layer)
        {
            switch (layer)
            {
            case BACKGROUND_LAYER:  return "Background";
            case DEFAULT_LAYER:     return "Default";
            case FOREGROUND_LAYER:  return "Foreground";
            case UI_LAYER:          return "UI";
            default: break;
            }
            return "Invalid";
        }

        // Unordered set insert comparator
        struct GfxLayerComparator {
            bool operator()(instanceData const& a, instanceData const& b) const {
                return a.layer < b.layer;
            }

            bool operator()(instanceData3D const& a, instanceData3D const& b) const {
                return a.layer < b.layer;
            }
        };

        /*!
         * \brief Default constructor for the Sprite class.
         *
         * Initializes the sprite with default values.
         */
        Sprite();

        /*!
         * \brief Constructor for the Sprite class with specified parameters.
         *
         * Initializes the sprite with a specified name and primitive type.
         *
         * \param model_name The name of the sprite.
         * \param primitive The rendering primitive type for the sprite.
         */
        Sprite(std::string const& model_name, GLenum primitive); 

        /*!
         * \brief Copy constructor for the Sprite class.
         *
         * This constructor creates a copy of an existing Sprite object, copying its attributes and properties to a new
         * Sprite object.
         *
         * \param other The Sprite object to be copied.
         */
        Sprite(const Sprite& other);

        /*!
         * \brief Gets the type of the Sprite component.
         * \return The name of the Sprite component.
         */
        static std::string GetType() {
            return "Sprite";
        }

        /*!
         * \brief Applies the transformation to the sprite.
         *
         * This function returns the transformation matrix for the sprite based on its position, rotation, and scaling.
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
         * \brief Renders instances of quads efficiently.
         *
         * This static member function binds the instance Vertex Buffer Object (VBO),
         * uploads the instance data to the GPU, and draws the instances as quads using
         * the provided shader program and mesh. It's a highly efficient method for rendering
         * multiple sprites with different transformations in a single draw call.
         */
        static void draw_instanced_quads();
        static void draw_instanced_3D_quads();

        static void draw_colored_quad(Vector2D const& pos, float rotation, Vector2D const& scale, Vector4D const& color, int layer = DrawLayer::DEFAULT_LAYER);
        static void draw_textured_quad(Vector2D const& pos, float rotation, Vector2D const& scale, Image const& texture, float alpha, int layer = DrawLayer::DEFAULT_LAYER);

        static void draw_lights();

        static void draw_picked_entity_border();

        /*!
         * \brief "Draws" a debug line between two specified points.
         *
         * This static member function draws a debug line between two given points.
         * It provides the option to specify the color, length, and angle of the line.
         *
         * \param p0 The first point of the line.
         * \param p1 The second point of the line.
         * \param color The color of the line, specified as a tuple of three floats (r, g, b).
         * \param length The length of the line (optional, defaults to -1.f).
         * \param angleInDegrees The angle of the line in degrees (optional, defaults to -1.f).
         */
        static void drawDebugLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& color,
            float length = -1.f, float angleInDegrees = -1.f); // Use non-default values only when values will never change!

        /*!
         * \brief Renders instances of lines efficiently.
         *
         * This static member function binds the instance Vertex Buffer Object (VBO),
         * uploads the instance data to the GPU, and draws the instances as lines using
         * the provided shader program and mesh. It's a highly efficient method for rendering
         * multiple lines with different transformations in a single draw call.
         */
        static void draw_instanced_lines();

        /*!
         * \brief "Draws" a debug circle at a specified position with a given scale and color.
         *
         * This static member function draws a debug circle at the specified world position.
         * You can specify the circle's scale and color as parameters.
         *
         * \param worldPos The world position where the center of the circle should be drawn.
         * \param scale The scale of the circle (width and height).
         * \param color The color of the circle, specified as a tuple of three floats (r, g, b).
         */
        static void drawDebugCircle(Vector2D const& worldPos, Vector2D const& scale, std::tuple<float, float, float> const& color);

        /*!
         * \brief Renders instances of circles efficiently.
         *
         * This static member function binds the instance Vertex Buffer Object (VBO),
         * uploads the instance data to the GPU, and draws the instances as circles using
         * the provided shader program and mesh. It's a highly efficient method for rendering
         * multiple circles with different transformations in a single draw call.
         */
        static void draw_instanced_circles();

        /*!
         * \brief Adds a new animation to the sprite.
         *
         * This member function allows you to add a new animation to the sprite. You can specify the name of the
         * animation, the number of columns and rows in the sprite sheet, and the total animation time. This function
         * creates an Animation object and adds it to the sprite's list of animations.
         *
         * \param name The name of the animation.
         * \param columns The number of columns in the sprite sheet.
         * \param rows The number of rows in the sprite sheet.
         * \param animation_time The total time (in seconds) for the animation.
         */
        void AddAnimation(std::string const& name, int columns, int rows, float animation_time);

        /*!
         * \brief Removes an animation from the sprite by index.
         *
         * This member function removes an animation from the sprite's list of animations based on the specified index.
         * If the index is invalid, this function logs a warning message and does nothing.
         *
         * \param animIndex The index of the animation to remove.
         */
        void removeAnimation(int animIndex);

        /*!
         * \brief Removes all animations from the sprite.
         *
         * This member function removes all animations from the sprite. After calling this function, the sprite will
         * no longer have any animations associated with it.
         */
        void removeAllAnimations();

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
         * \brief Deserializes the sprite data
         *
         * \param Json::Value the data to deserialize
         */
        void Deserialize(Json::Value data) override;

        /*!
         * \brief Serializes the sprite data
         */
        Json::Value Serialize() override;


        /// BELOW FUNCTIONS ARE UNUSED AFTER ADDING INSTANCING ///
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
         * \brief Draws a line between two points.
         *
         * This static function draws a line between two specified points.
         *
         * \param p0 The first point of the line.
         * \param p1 The second point of the line.
         * \param color The color of the line, white by default. TODO: Replace with Vector3D
         * \param thickness The thickness of the line.
         */
        void drawLine(Vector2D const& p0, Vector2D const& p1, std::tuple<float, float, float> const& color, float thickness);
    };
}
#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_SPRITE_H