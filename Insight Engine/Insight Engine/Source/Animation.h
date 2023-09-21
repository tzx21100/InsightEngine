#pragma once
#include <GL/glew.h>
#include "Mesh.h"
#include "Shader.h"

namespace IS {
	class Animation {
	public:
		void initAnimation(int rows, int columns, float animation_time);
        void updateAnimation(float deltaTime);
        float getFrameWidth() const;
        float getFrameHeight() const;
        void resetAnimation();

        int x_frame_index{};
        int y_frame_index{};


    private:
        int x_frames{};
        int y_frames{};
        float animation_duration{};
        float time_per_frame{};
        float frame_timer{};
	};
}