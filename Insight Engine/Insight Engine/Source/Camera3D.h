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

namespace IS {
	// Enumeration for camera type

	class Camera3D {
	public:
		void init_camera(int width, int height, float fieldOfView) {
			initialFov = fieldOfView;

			fov = initialFov;
			aspectRatio = static_cast<float>(width) / static_cast<float>(height);

			// calculate camDist to see the entire game world of 1920 units width
			float camDist = ((static_cast<float>(width) / 2.f) / tanf(glm::radians(fov) / 2.f));

			position = glm::vec3(0.f, 0.f, camDist);
			near = 0.1f;
			far = camDist * 2.f; // to cover zooming in and out
		}

		// can use camera_keyboard_callback instead
		void update_camera_pos(float newX, float newY) {
			position.x = newX;
			position.y = newY;
		}

		void update_camera_zoom(float zoomLevel) {
			fov = initialFov / zoomLevel;
		}

		void update_camera_aspect_ratio(float ar) {
			aspectRatio = ar; 
		}

		void update_camera_xform() {
			// new direction camera is facing
			front = glm::vec3{
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))
			};

			// normalize
			front = glm::normalize(front);


			view = glm::lookAt(position, position + front, up);
			projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		}
		
		/*void camera_keyboard_callback(float cameraSpeed) {
			auto& engine = InsightEngine::Instance();
			auto input = engine.GetSystem<InputManager>("Input");

			if (input->IsKeyPressed(GLFW_KEY_W)) {
				position += front * cameraSpeed;
			}
			if (input->IsKeyPressed(GLFW_KEY_S)) {
				position -= front * cameraSpeed;
			}
			if (input->IsKeyPressed(GLFW_KEY_A)) {
				position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
			}
			if (input->IsKeyPressed(GLFW_KEY_D)) {
				position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
			}
		}*/

		void camera_mouse_callback(float xOffset, float yOffset) {
			const float sensitivity = 0.1f;
			
			xOffset *= sensitivity;
			yOffset *= sensitivity;

			yaw += xOffset;
			pitch -= yOffset;

			// clamp pitch
			if (pitch > 89.0f) pitch = 89.0f;
			else if (pitch < -89.0f) pitch = -89.0f;

			// new direction camera is facing
			front = glm::vec3{
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))
			};

			// normalize
			front = glm::normalize(front);
		}

		glm::mat4 getViewMatrix() { return view; }

		glm::mat4 getPerspectiveMatrix() { return projection; }
		
		glm::mat4 getCameraToNDCXform() { return projection * view; }

		/// Static members
		static aCameraType mActiveCamera;

		static float mZoomSpeed; ///< rate of zoom
		static float mMoveSpeed; ///< rate of camera pan

		glm::vec3 position{};

	private:
		const glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);	

		float aspectRatio{};
		float initialFov{};
		float fov{};
		float near{};
		float far{};
		float pitch{ 0.f };
		float yaw{ -90.f };

		glm::mat4 view{};
		glm::mat4 projection{};
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA3D_H