/*!
 * \file Camera.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 02-11-2023
 * \brief
 * This file defines the Camera class used for managing view and projection in graphics systems.
 *
 * The Camera class provides functions to update the camera's position, dimensions, rotation, and transformation.
 * It also supports zooming and panning operations, and allows access to various camera properties like zoom level
 * and camera vectors. This file defines the Camera class and its associated constants.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                      guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA3D_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA3D_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Input.h"


namespace IS {

	// Enumeration for camera type
	enum aCameraType { CAMERA_TYPE_GAME = 0, CAMERA_TYPE_EDITOR };

	// Enumeration for camera projection
	enum aCameraProjection { ProjectionType_Othographic = 0, ProjectionType_Perspective };

	class Camera3D {
	public:
		void Init(int width, int height, float fov);

		void Update();

		// can use camera_keyboard_callback instead
		void SetPosition(float x, float y) { mPosition.x = x; mPosition.y = y; }
		void SetPosition(float x, float y, float z) { mPosition = { x, y, z }; }

		glm::vec2 GetPosition2D() const { return { mPosition.x, mPosition.y }; }
		glm::vec3 GetPosition3D() const { return mPosition; }

		void SetZoomLevel(float zoom_level) { mZoomLevel = std::clamp(zoom_level, CAMERA_ZOOM_MIN, CAMERA_ZOOM_MAX); }
		float GetZoomLevel() const { return mZoomLevel; }

		void PanCamera(float delta_x, float delta_y);

		void SetAspectRatio(float ar) { mAspectRatio = ar;  }
		
		void MoveCamera(float move_speed);

		void Rotate(float xoffset, float yoffset);

		glm::mat4 GetViewMatrix() { return mView; }

		glm::mat4 GetProjectionMatrix() { return mProjection; }
		
		glm::mat4 getCameraToNDCXform() { return mProjection * mView; }
		
		float GetAspectRatio() { return mAspectRatio; }

		static Vector2D mouseToWorld(Vector2D const& screenMousePos);

		/// Static members
		static aCameraType mActiveCamera;

		static float mZoomSpeed; ///< rate of zoom
		static float mMoveSpeed; ///< rate of camera pan
		static aCameraProjection mProjectionType; ///< Type of camera projection

		glm::vec3 mPosition{};

	private:

		/**
		* Constants defining camera properties and limits.
		*/
		static const float CAMERA_ZOOM_MIN; ///< minimum zoom level
		static const float CAMERA_ZOOM_MAX; ///< maximum zoom level
		static const float CAMERA_ZOOM_SPEED_MIN; ///< minimum rate of zoom
		static const float CAMERA_ZOOM_SPEED_MAX; ///< maximum rate of zoom
		static const float CAMERA_MOVE_SPEED_MIN; ///< minimum rate of pan
		static const float CAMERA_MOVE_SPEED_MAX; ///< maximum rate of pan
		static const float CAMERA_FOV_MIN; ///< minimum fov
		static const float CAMERA_FOV_MAX; ///< maximum fov

		const glm::vec3 mUp = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 mFront = glm::vec3(0.f, 0.f, -1.f);	

		float mAspectRatio{};
		float mFOV{};
		float mNear{};
		float mFar{};
		float mPitch{ 0.f };
		float mYaw{ -90.f };

		float mZoomLevel = 1.f;

		glm::mat4 mView{};
		glm::mat4 mProjection{};

		friend class InspectorPanel;
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA3D_H