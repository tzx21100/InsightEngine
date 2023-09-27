/*!
 * \file Animation.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This source file defines the Animation class, which manages sprite animations.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"
#include "Animation.h"

namespace IS {
	void Animation::initAnimation(int rows, int columns, float animation_time) {
        // init animation object with values
        x_frames = columns;
        y_frames = rows;
        animation_duration = animation_time;
        time_per_frame = animation_time / (x_frames * y_frames);
	}

    void Animation::updateAnimation(float deltaTime) {
        // Update function to advance the animation
        frame_timer += deltaTime;

        while (frame_timer >= time_per_frame) {
            frame_timer -= time_per_frame;
            ++x_frame_index; // go to next column
            if (x_frame_index == x_frames) { // after last column
                x_frame_index = 0; // return to first
                ++y_frame_index; // go to next row
                if (y_frame_index == y_frames) y_frame_index = 0; // after last frame, reset to first
            }
        }
    }

    float Animation::getFrameWidth() const {
        return 1.f / x_frames; // returns width of each frame
    }

    float Animation::getFrameHeight() const {
        return 1.f / y_frames; // returns height of each frame
    }

    void Animation::resetAnimation() {
        // set indexes and timer to 0
        x_frame_index = 0;
        y_frame_index = 0;
        frame_timer = 0;
    }
}