/*!
 * \file Animation.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
 * \date 25-11-2023
 * \brief
 * This header file defines the Animation class, which manages sprite animations.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_ANIMATION_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_ANIMATION_H

#include <glad/glad.h>
#include "Mesh.h"
#include "Shader.h"

namespace IS {

    struct Image;

    /*!
     * \brief The Animation class manages sprite animations.
     *
     * The Animation class is responsible for handling sprite animations by specifying the number of rows and columns
     * in a sprite sheet and the animation's duration. It can update the animation based on the elapsed time and provide
     * information about the current frame's dimensions.
     */
    class Animation {
    public:
        /*!
         * \brief Initializes the animation settings.
         *
         * Initializes the animation with the given number of rows and columns in the sprite sheet and the total
         * animation time in seconds.
         *
         * \param rows The number of rows in the sprite sheet.
         * \param columns The number of columns in the sprite sheet.
         * \param animation_time The total duration of the animation in seconds.
         */
        void initAnimation(int rows, int columns, float animation_time);

        /*!
         * \brief Updates the animation frame.
         *
         * Updates the animation based on the elapsed time (deltaTime). This function advances the animation frame
         * according to the specified animation time.
         *
         * \param deltaTime The time difference since the last update.
         */
        void updateAnimation(float deltaTime);

        /*!
         * \brief Draws non-entity animation.
         *
         * Draws the non-entity animation at the specified position, rotation, scale, and alpha with the provided texture.
         *
         * \param deltaTime The time difference since the last update.
         * \param pos The position of the animation.
         * \param rotation The rotation angle of the animation.
         * \param scale The scale of the animation.
         * \param texture The texture used for the animation.
         * \param alpha The alpha (transparency) value of the animation.
         * \param layer The layer on which to draw the animation.
         */
        void drawNonEntityAnimation(float deltaTime, Vector2D const& pos, float rotation, Vector2D const& scale, Image const& texture, float alpha, int layer);

        /*!
         * \brief Gets the width of a single frame.
         *
         * Calculates and returns the width of a single frame in the sprite sheet as a fraction of the total width.
         *
         * \return The width of a single frame as a fraction of the total width (normalized value).
         */
        float getFrameWidth() const;

        /*!
         * \brief Gets the height of a single frame.
         *
         * Calculates and returns the height of a single frame in the sprite sheet as a fraction of the total height.
         *
         * \return The height of a single frame as a fraction of the total height (normalized value).
         */
        float getFrameHeight() const;

        /*!
         * \brief Resets the animation to the initial frame.
         *
         * Resets the animation to the first frame, setting the frame index and timer to their initial values.
         */
        void resetAnimation();

        // need to be accessed by shader
        std::string name{};
        glm::vec2 frame_dimension{1.f, 1.f};
        glm::vec2 frame_index{0.f, 0.f};
        int x_frames{};             // The total number of columns in the sprite sheet.
        int y_frames{};             //The total number of rows in the sprite sheet.
        float animation_duration{}; // The total duration of the animation in seconds.
        float time_per_frame{};     // The duration of each frame in seconds.
        float frame_timer{};        // Timer to keep track of elapsed time for frame updates.

    private:

    };
} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_ANIMATION_H