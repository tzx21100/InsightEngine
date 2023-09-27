/*!
 * \file Animation.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file defines the Animation class, which manages sprite animations.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_ANIMATION_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_ANIMATION_H

#include <glad/glad.h>
#include "Mesh.h"
#include "Shader.h"

namespace IS {
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
        int x_frame_index{};         // The current column (X-axis) frame index.
        int y_frame_index{};         // The current row (Y-axis) frame index.

    private:
        int x_frames{};             // The total number of columns in the sprite sheet.
        int y_frames{};             //The total number of rows in the sprite sheet.
        float animation_duration{}; // The total duration of the animation in seconds.
        float time_per_frame{};     // The duration of each frame in seconds.
        float frame_timer{};        // Timer to keep track of elapsed time for frame updates.
    };
} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_ANIMATION_H
