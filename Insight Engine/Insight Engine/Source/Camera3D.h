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
#include "CoreEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace IS {
	// Enumeration for camera type

	class Camera3D {
	public:
		void init3DCamera(int width, int height, float fieldOfView) {
			initialFov = fieldOfView;
			fov = initialFov;
			aspectRatio = static_cast<float>(width) / static_cast<float>(height);

			// calculate camDist to see the entire game world of 1920 units width
			float camDist = ((static_cast<float>(width) / 2.f) / tanf(fov / 2.f));

			position = glm::vec3(0.f, 0.f, camDist);
			near = 0.1f;
			far = camDist * 2.f; // to cover zooming in and out

			projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
			view = glm::lookAt(position, position + target, up);
		}

		void update3DCameraPos(float newX, float newY) {
			position.x += newX;
			position.y += newY;

			target.x += newX;
			target.y += newY;

			view = glm::lookAt(position, position + target, up);
		}

		void update3DCameraZoom(float zoomLevel) {
			fov = initialFov / zoomLevel;
			projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		}

		void update3DCameraAspectRatio(float ar) {
			aspectRatio = ar; 
			projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		}

		glm::mat4 getViewMatrix() { return view; }

		glm::mat4 getPerspectiveMatrix() { return projection; }
		
		glm::mat4 getCameraToNDCXform() { return view * projection; }

		/// Static members
		static aCameraType mActiveCamera;

		static float mZoomSpeed; ///< rate of zoom
		static float mMoveSpeed; ///< rate of camera pan

	private:
		glm::vec3 position{};
		glm::vec3 target = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);;

		float aspectRatio{};
		float initialFov{};
		float fov{};
		float near{};
		float far{};
		
		glm::mat4 view{};
		glm::mat4 projection{};
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA3D_H