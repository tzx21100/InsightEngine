/*!
 * \file Camera.cpp
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

#include "Pch.h"
#include "Camera.h"

namespace IS {
	// Static members initialization
	aCameraType Camera::mActiveCamera = CAMERA_TYPE_EDITOR;
	const float Camera::CAMERA_X_MIN = -16000;
	const float Camera::CAMERA_X_MAX =  16000.f;
	const float Camera::CAMERA_Y_MIN = -9000.f;
	const float Camera::CAMERA_Y_MAX =  9000.f;
	const float Camera::CAMERA_ZOOM_MIN = .1f;
	const float Camera::CAMERA_ZOOM_MAX = 10.f;
	const float Camera::CAMERA_ZOOM_SPEED_MIN = 0.01f;
	const float Camera::CAMERA_ZOOM_SPEED_MAX = 0.2f;
	const float Camera::CAMERA_MOVE_SPEED_MIN = 1.f;
	const float Camera::CAMERA_MOVE_SPEED_MAX = 10.f;

	float Camera::mZoomSpeed = 0.1f;
	float Camera::mMoveSpeed = 5.f;

	Camera::Camera() {
		// default values for construction
		mZoomLevel = 1.f;
		world_position = { 0.f, 0.f };
		camera_dim = { 0.f, 0.f };
		uVector = { 1.f, 0.f };
		vVector = { 0.f, 1.f };
	}

	void Camera::UpdateCamPos(float newX, float newY) {
		// Update the camera position while clamping it within specified limits.
		world_position.x = std::clamp(newX, CAMERA_X_MIN, CAMERA_X_MAX);
		world_position.y = std::clamp(newY, CAMERA_Y_MIN, CAMERA_Y_MAX);
		UpdateCamXform();
	}

	void Camera::UpdateCamDim(float newWidth) {
		// follow [16:9] aspect ratio
		camera_dim = glm::vec2(newWidth, newWidth * 0.5625f); 
		UpdateCamXform();
	}

	void Camera::UpdateCamRotation(float newAngle) {
		uVector = { cosf(glm::radians(newAngle)), sinf(glm::radians(newAngle)) };
		vVector = { -uVector.y, uVector.x };
	}

	glm::vec2 Camera::GetCamPos() {
		return world_position;
	}

	glm::vec2 Camera::GetCamDim() {
		return camera_dim;
	}

	glm::vec2 Camera::GetUVector() {
		return uVector;
	}

	glm::vec2 Camera::GetVVector() {
		return vVector;
	}

	void Camera::UpdateCamXform() {
		// map scaling
		float scaled_width = camera_dim.x / mZoomLevel;
		float scaled_height = camera_dim.y / mZoomLevel;

		// save on calculations
		float twoOverWidth = 2.f / (scaled_width);
		float twoOverHeight = 2.f / (scaled_height);

		// camera xform calculation
		xform = glm::mat3 { 
			(twoOverWidth * uVector.x),							 (twoOverHeight * vVector.x),						   0.f, // column 1
			(twoOverWidth * uVector.y),							 (twoOverHeight * vVector.y),						   0.f, // column 2
			(twoOverWidth * -glm::dot(uVector, world_position)), (twoOverHeight * -glm::dot(vVector, world_position)), 1.f  // column 3
		}; 
	}

	void Camera::ZoomCamera(float yoffset) {
		// zoom based on positive or negative yOffset
		mZoomLevel += yoffset * mZoomSpeed;
		mZoomLevel = std::max(CAMERA_ZOOM_MIN, std::min(10.f, mZoomLevel));
		UpdateCamXform();
	}

	void Camera::PanCamera(float delta_x, float delta_y) {
		// pan camera by adjusting pos in world coords
		float new_position_x = world_position.x - delta_x * mMoveSpeed;
		float new_position_y = world_position.y + delta_y * mMoveSpeed;
		UpdateCamPos(new_position_x, new_position_y);
	}
}