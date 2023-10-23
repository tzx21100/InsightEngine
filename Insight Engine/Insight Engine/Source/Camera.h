#pragma once
/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "CoreEngine.h"
#include <glm/glm.hpp>

namespace IS {
	class Camera {
	public:
		static Camera& Instance() { static Camera instance; return instance; }

		void UpdateCamPos(float newX, float newY);
		void UpdateCamDim(float newWidth);
		void UpdateCamRotation(float newAngle);

		glm::vec2 GetCamPos();
		glm::vec2 GetCamDim();
		glm::vec2 GetUVector();
		glm::vec2 GetVVector();

	private:
		Camera();
		~Camera() = default;

		glm::vec2 world_position{};
		glm::vec2 camera_dim{};
		glm::vec2 uVector{}; // camera x/y axis
		glm::vec2 vVector{};
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H