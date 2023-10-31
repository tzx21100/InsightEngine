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

	enum aCameraType { CAMERA_TYPE_GAME = 0, CAMERA_TYPE_EDITOR };

	class Camera {
	public:		
		Camera();
		~Camera() = default;

		void UpdateCamPos(float newX, float newY);
		void UpdateCamDim(float newWidth);
		void UpdateCamRotation(float newAngle);
		void UpdateCamXform();
		void ZoomCamera(float yoffset);
		void PanCamera(float dt, float delta_x, float delta_y);
		
		float GetZoomLevel() const { return mZoomLevel; }
		void SetZoomLevel(float zoom_level)
		{
			mZoomLevel = std::min(Camera::mMaxZoom, std::max(zoom_level, Camera::mMinZoom));
		}

		glm::vec2 GetCamPos();
		glm::vec2 GetCamDim();
		glm::vec2 GetUVector();
		glm::vec2 GetVVector();

		static aCameraType mActiveCamera;

		glm::mat3 xform{};
		static float mMinZoom; ///< minimum zoom level
		static float mMaxZoom; ///< maximum zoom level
		static float mZoomSpeed; ///< rate of zoom
		static float mMoveSpeed; ///< rate of camera pan

	private:
		float mZoomLevel;
		glm::vec2 world_position{};
		glm::vec2 camera_dim{};
		glm::vec2 uVector{}; // camera x/y axis
		glm::vec2 vVector{};

		static float mMinX; ///< minimum x bound of camera
		static float mMaxX; ///< maximum x bound of camera
		static float mMinY; ///< minimum y bound of camera
		static float mMaxY; ///< maximum y bound of camera
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H