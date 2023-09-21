#include "Pch.h"
#include "Animation.h"

namespace IS {
	void Animation::initAnimation(int rows, int columns, float animation_time) {
        x_frames = columns;
        y_frames = rows;
        animation_duration = animation_time;
        time_per_frame = animation_time / (x_frames * y_frames);
	}

    // Update function to advance the animation
    void Animation::updateAnimation(float deltaTime) {
        frame_timer += deltaTime;

        while (frame_timer >= time_per_frame) {
            frame_timer -= time_per_frame;
            ++x_frame_index;
            if (x_frame_index == x_frames) {
                x_frame_index = 0;
                ++y_frame_index;
                if (y_frame_index == y_frames) y_frame_index = 0;
            }
        }
    }

    float Animation::getFrameWidth() const {
        return 1.f / x_frames;
    }

    float Animation::getFrameHeight() const {
        return 1.f / y_frames;
    }

    void Animation::resetAnimation() {
        x_frame_index = 0;
        y_frame_index = 0;
        frame_timer = 0;
    }
}