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
#include "Camera3D.h"

namespace IS {

	aCameraType Camera3D::mActiveCamera = CAMERA_TYPE_EDITOR;
	const float Camera3D::CAMERA_ZOOM_MIN = .34f;
	const float Camera3D::CAMERA_ZOOM_MAX = 10.f;
	const float Camera3D::CAMERA_ZOOM_SPEED_MIN = 0.01f;
	const float Camera3D::CAMERA_ZOOM_SPEED_MAX = 0.2f;
	const float Camera3D::CAMERA_MOVE_SPEED_MIN = 1.f;
	const float Camera3D::CAMERA_MOVE_SPEED_MAX = 10.f;
	const float Camera3D::CAMERA_FOV_MIN = 0.f;
	const float Camera3D::CAMERA_FOV_MAX = 176.f;

	float Camera3D::mZoomSpeed = 0.1f;
	float Camera3D::mMoveSpeed = 5.f;

	aCameraProjection Camera3D::mProjectionType = ProjectionType_Othographic;

	void Camera3D::Init(int width, int height, float fov)
	{
		mFOV = fov;
		mAspectRatio = static_cast<float>(width) / static_cast<float>(height);

		// calculate camDist to see the entire game world of 1920 units width
		float distance = ((static_cast<float>(width) / 2.f) / tanf(glm::radians(mFOV) / 2.f));

		mPosition = glm::vec3(0.f, 0.f, distance);
		mNear = 0.1f;
		mFar = distance * 2.f; // to cover zooming in and out
	}

	void Camera3D::Update()
	{
		float adjusted_fov = std::clamp(mFOV / mZoomLevel, CAMERA_FOV_MIN, CAMERA_FOV_MAX);
		mView = glm::lookAt(mPosition, mPosition + mFront, mUp);

		switch (mProjectionType)
		{
		case ProjectionType_Othographic:
		{
			auto [width, height] = InsightEngine::Instance().GetWindowSize();
			float fWidth = static_cast<float>(width) / GetZoomLevel();
			float fHeight = fWidth / GetAspectRatio();
			mProjection = glm::ortho(-fWidth, fWidth, -fHeight, fHeight, mNear, mFar);
			break;
		}
		case ProjectionType_Perspective:
			mProjection = glm::perspective(glm::radians(adjusted_fov), mAspectRatio, mNear, mFar);
			break;
		}
	}

	void Camera3D::PanCamera(float delta_x, float delta_y)
	{
		// pan camera by adjusting pos in world coords
		float new_position_x = mPosition.x - delta_x * mMoveSpeed / mZoomLevel;
		float new_position_y = mPosition.y + delta_y * mMoveSpeed / mZoomLevel;
		SetPosition(new_position_x, new_position_y);
	}

	void Camera3D::MoveCamera(float move_speed)
	{
		auto& engine = InsightEngine::Instance();
		auto input = engine.GetSystem<InputManager>("Input");
		float camera_speed = move_speed / mZoomLevel;

		if (input->IsKeyHeld(GLFW_KEY_UP))
		{
			mPosition += mFront * camera_speed;
		}
		if (input->IsKeyHeld(GLFW_KEY_DOWN))
		{
			mPosition -= mFront * camera_speed;
		}
		if (input->IsKeyHeld(GLFW_KEY_LEFT))
		{
			mPosition -= glm::normalize(glm::cross(mFront, mUp)) * camera_speed;
		}
		if (input->IsKeyHeld(GLFW_KEY_RIGHT))
		{
			mPosition += glm::normalize(glm::cross(mFront, mUp)) * camera_speed;
		}

		mPosition.z = std::clamp(mPosition.z, 10.f, 3300.f);
	}

	void Camera3D::Rotate(float xoffset, float yoffset)
	{
		const float sensitivity = 0.1f;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		mYaw += xoffset;
		mPitch -= yoffset;

		// clamp pitch
		mPitch = std::clamp(mPitch, -90.f, 90.f);

		// new direction camera is facing
		mFront = glm::vec3{
			cos(glm::radians(mYaw)) * cos(glm::radians(mPitch)),
			sin(glm::radians(mPitch)),
			sin(glm::radians(mYaw)) * cos(glm::radians(mPitch))
		};

		// normalize
		mFront = glm::normalize(mFront);
	}

	Vector2D Camera3D::mouseToWorld(Vector2D const& screenMousePos){
		InsightEngine& engine = InsightEngine::Instance();
		auto [width, height] = engine.IsFullScreen() ? engine.GetMonitorSize() : engine.GetWindowSize();

		float ndcX = ((screenMousePos.x * 2.f / static_cast<float>(width)) - 1.f);
		float ndcY = ((screenMousePos.y * 2.f / static_cast<float>(height)) - 1.f);

		glm::vec4 ndcCoords{ ndcX, ndcY, 1.f, 1.f };

		auto cameraInUse = ISGraphics::cameras3D[Camera3D::mActiveCamera];
		glm::mat4 ndcToCam = glm::inverse(cameraInUse.GetViewMatrix()) * glm::inverse(cameraInUse.GetProjectionMatrix());

		float fWidth = static_cast<float>(width) / cameraInUse.GetZoomLevel();
		float fHeight = fWidth / cameraInUse.GetAspectRatio();
		Transform camXform({ cameraInUse.mPosition.x, cameraInUse.mPosition.y }, 0.f, { fWidth, fHeight });
		glm::mat4 camXformMat = camXform.Return3DXformMatrix();

		glm::vec4 worldPos = ndcToCam * ndcCoords;
		worldPos /= worldPos.w;

		//IS_CORE_ERROR("mouse to world: {}, {}", worldPos.x, -worldPos.y);
		return { worldPos.x, -worldPos.y };
	}

} // end namespace IS
