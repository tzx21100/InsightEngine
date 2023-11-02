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
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "CoreEngine.h"
#include <glm/glm.hpp>

namespace IS {
	// Enumeration for camera type
	enum aCameraType { CAMERA_TYPE_GAME = 0, CAMERA_TYPE_EDITOR };

	class Camera {
	public:		
		/**
		 * Constructor for the Camera class.
		 */
		Camera();

		/**
		 * Destructor for the Camera class.
		 */
		~Camera() = default;

		/**
		 * Update the camera position.
		 *
		 * @param newX - The new X-coordinate of the camera position.
		 * @param newY - The new Y-coordinate of the camera position.
		 */
		void UpdateCamPos(float newX, float newY);


		/**
		 * Update the camera dimensions.
		 *
		 * @param newWidth - The new width of the camera's view.
		 */
		void UpdateCamDim(float newWidth);

		/**
		 * Update the camera rotation.
		 *
		 * @param newAngle - The new rotation angle in degrees.
		 */
		void UpdateCamRotation(float newAngle);

		/**
		* Update the camera transformation matrix.
		*/
		void UpdateCamXform();

		/**
		 * Zoom the camera in or out.
		 *
		 * @param yoffset - The zoom factor.
		 */
		void ZoomCamera(float yoffset);

		/**
		 * Pan the camera.
		 *
		 * @param delta_x - The change in X-coordinate for panning.
		 * @param delta_y - The change in Y-coordinate for panning.
		 */
		void PanCamera(float delta_x, float delta_y);
		
		/**
		 * Get the current zoom level of the camera.
		 *
		 * @return The current zoom level.
		 */
		float GetZoomLevel() const { return mZoomLevel; }

		/**
		 * Set the zoom level for the camera, clamped within the specified bounds.
		 *
		 * @param zoom_level - The desired zoom level to set.
		 */
		void SetZoomLevel(float zoom_level)
		{
			mZoomLevel = std::min(Camera::CAMERA_ZOOM_MAX, std::max(zoom_level, Camera::CAMERA_ZOOM_MIN));
		}

		/**
		 * Get the current camera position.
		 *
		 * @return A glm::vec2 containing the camera's position (X and Y coordinates).
		 */
		glm::vec2 GetCamPos();

		/**
		 * Get the current camera dimensions.
		 *
		 * @return A glm::vec2 containing the camera's dimensions (width and height).
		 */
		glm::vec2 GetCamDim();

		/**
		 * Get the U vector of the camera.
		 *
		 * @return A glm::vec2 representing the U vector.
		 */
		glm::vec2 GetUVector();

		/**
		 * Get the V vector of the camera.
		 *
		 * @return A glm::vec2 representing the V vector.
		 */
		glm::vec2 GetVVector();

		/**
		* The transformation matrix of the camera.
		*/
		glm::mat3 xform{};

		/// Static members
		static aCameraType mActiveCamera;

		/**
		* Constants defining camera properties and limits.
		*/
		static const float CAMERA_X_MIN; ///< minimum x bound of camera
		static const float CAMERA_X_MAX; ///< maximum x bound of camera
		static const float CAMERA_Y_MIN; ///< minimum y bound of camera
		static const float CAMERA_Y_MAX; ///< maximum y bound of camera
		static const float CAMERA_ZOOM_MIN; ///< minimum zoom level
		static const float CAMERA_ZOOM_MAX; ///< maximum zoom level
		static const float CAMERA_ZOOM_SPEED_MIN; ///< minimum rate of zoom
		static const float CAMERA_ZOOM_SPEED_MAX; ///< maximum rate of zoom
		static const float CAMERA_MOVE_SPEED_MIN; ///< minimum rate of pan
		static const float CAMERA_MOVE_SPEED_MAX; ///< maximum rate of pan

		static float mZoomSpeed; ///< rate of zoom
		static float mMoveSpeed; ///< rate of camera pan

	private:
		float mZoomLevel;
		glm::vec2 world_position{};
		glm::vec2 camera_dim{};

		// camera's x/y axis
		glm::vec2 uVector{};
		glm::vec2 vVector{};
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_CAMERA_H