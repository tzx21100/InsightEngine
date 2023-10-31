#include "Pch.h"
#include "Camera.h"

namespace IS {

	aCameraType Camera::mActiveCamera = CAMERA_TYPE_EDITOR;
	float Camera::mMinX = -16000;
	float Camera::mMaxX =  16000.f;
	float Camera::mMinY = -9000.f;
	float Camera::mMaxY =  9000.f;
	float Camera::mMinZoom = .1f;
	float Camera::mMaxZoom = 10.f;
	float Camera::mZoomSpeed = 0.1f;
	float Camera::mMoveSpeed = 200.f;

	Camera::Camera() {
		mZoomLevel = 1.f;
		world_position = { 0.f, 0.f };
		camera_dim = { 0.f, 0.f };
		uVector = { 1.f, 0.f };
		vVector = { 0.f, 1.f };
	}

	void Camera::UpdateCamPos(float newX, float newY) {
		world_position.x = std::clamp(newX, mMinX, mMaxX);
		world_position.y = std::clamp(newY, mMinY, mMaxY);
		UpdateCamXform();
	}

	void Camera::UpdateCamDim(float newWidth) {
		camera_dim = glm::vec2(newWidth, newWidth * 0.5625f); // follow [16:9] aspect ratio
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
		float scaled_width = camera_dim.x / mZoomLevel;
		float scaled_height = camera_dim.y / mZoomLevel;

		float twoOverWidth = 2.f / (scaled_width);
		float twoOverHeight = 2.f / (scaled_height);

		xform = glm::mat3 { 
			(twoOverWidth * uVector.x),							 (twoOverHeight * vVector.x),						   0.f, // column 1
			(twoOverWidth * uVector.y),							 (twoOverHeight * vVector.y),						   0.f, // column 2
			(twoOverWidth * -glm::dot(uVector, world_position)), (twoOverHeight * -glm::dot(vVector, world_position)), 1.f  // column 3
		}; 
	}

	void Camera::ZoomCamera(float yoffset)
	{
		mZoomLevel += yoffset * mZoomSpeed;
		mZoomLevel = std::max(mMinZoom, std::min(10.f, mZoomLevel));
		UpdateCamXform();
		//auto& engine = InsightEngine::Instance();
		//auto input = engine.GetSystem<InputManager>("Input");
		//input->setCenterPos(world_position.x, world_position.y);
		//input->setRatio(camera_dim.x, camera_dim.y);
	}

	void Camera::PanCamera(float dt, float delta_x, float delta_y)
	{
		float new_position_x = world_position.x - delta_x * mMoveSpeed * dt;
		float new_position_y = world_position.y + delta_y * mMoveSpeed * dt;
		UpdateCamPos(new_position_x, new_position_y);
		//auto& engine = InsightEngine::Instance();
		//auto input = engine.GetSystem<InputManager>("Input");
		//input->setCenterPos(world_position.x, world_position.y);
		//input->setRatio(camera_dim.x, camera_dim.y);
	}
}