#include "Pch.h"
#include "Camera.h"

namespace IS {

	aCameraType Camera::mActiveCamera;

	Camera::Camera() {
		//auto [width, height] = InsightEngine::Instance().GetWindowSize();
		world_position = { 0.f, 0.f };
		camera_dim = { 0.f, 0.f };
		uVector = { 1.f, 0.f };
		vVector = { 0.f, 1.f };
	}

	void Camera::UpdateCamPos(float newX, float newY) {
		world_position = { newX, newY };
	}

	void Camera::UpdateCamDim(float newWidth) {
		camera_dim = glm::vec2(newWidth, newWidth * 0.5625f); // follow aspect ratio
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

	void Camera::updateCamXform() {
		float twoOverWidth = 2.f / camera_dim.x;
		float twoOverHeight = 2.f / camera_dim.y;

		xform = glm::mat3 { 
			(twoOverWidth * uVector.x),							 (twoOverHeight * vVector.x),						   0.f, // column 1
			(twoOverWidth * uVector.y),							 (twoOverHeight * vVector.y),						   0.f, // column 2
			(twoOverWidth * -glm::dot(uVector, world_position)), (twoOverHeight * -glm::dot(vVector, world_position)), 1.f  // column 3
		}; 
	}
}