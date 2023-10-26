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
		Camera();
		~Camera() = default;


		void UpdateCamPos(float newX, float newY);
		void UpdateCamDim(float newWidth);
		void UpdateCamRotation(float newAngle);
		void updateCamXform();

		glm::vec2 GetCamPos();
		glm::vec2 GetCamDim();
		glm::vec2 GetUVector();
		glm::vec2 GetVVector();

		enum camera_target {
			entity,
			editor
		};

		static int camera_in_use; // 0 for entity, 1 for engine
		glm::mat3 xform{};
	private:
		glm::vec2 world_position{};
		glm::vec2 camera_dim{};
		glm::vec2 uVector{}; // camera x/y axis
		glm::vec2 vVector{};
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H