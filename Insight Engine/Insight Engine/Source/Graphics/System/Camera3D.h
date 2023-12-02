/*!
 * \file Camera3D.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * This file defines the Camera3D class, which is responsible for managing the view and projection in mock 3D graphics systems.
 *
 * The Camera3D class provides essential functions to update the camera's position, dimensions, rotation, and transformation.
 * It supports zooming, panning operations, and allows access to various camera properties such as zoom level and camera vectors.
 * This file establishes the Camera3D class and its associated constants.
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
#include "Engine/Core/Core.h"
#include <glm/glm.hpp>

namespace IS {

	/*!
	 * \enum aCameraType
	 * \brief Enumeration representing the type of camera.
	 */
	enum aCameraType { CAMERA_TYPE_GAME = 0, CAMERA_TYPE_EDITOR };

	/*!
	 * \enum aCameraProjection
	 * \brief Enumeration representing the type of camera projection.
	 */
	enum aCameraProjection { ProjectionType_Othographic = 0, ProjectionType_Perspective };

	/*!
	 * \class Camera3D
	 * \brief The Camera3D class manages 3D camera operations in graphics systems.
	 *
	 * The Camera3D class provides functions to update the camera's position, dimensions, rotation, and transformation.
	 * It supports zooming and panning operations, and allows access to various camera properties like zoom level
	 * and camera vectors.
	 */
	class IS_API Camera3D {
	public:
		/*!
		 * \brief Initializes the Camera3D instance with the specified width, height, and field of view.
		 *
		 * \param width The width of the camera's view.
		 * \param height The height of the camera's view.
		 * \param fov The field of view (FOV) for perspective projection.
		 */
		void Init(int width, int height, float fov);

		void Reset();

		/*!
		 * \brief Updates the view and projection matrices of the Camera3D instance.
		 */
		void Update();

		/*!
		 * \brief Sets the position of the Camera3D instance.
		 *
		 * \param x The x-coordinate of the new position.
		 * \param y The y-coordinate of the new position.
		 */
		void SetPosition(float x, float y) { mPosition.x = x; mPosition.y = y; } // can use camera_keyboard_callback instead
		void SetPosition(float x, float y, float z) { mPosition = { x, y, z }; }

		/*!
		 * \brief Gets the position of the Camera3D instance.
		 *
		 * \return The position of the Camera3D instance.
		 */
		glm::vec2 GetPosition2D() const { return { mPosition.x, mPosition.y }; }
		glm::vec3 GetPosition3D() const { return mPosition; }

		/*!
		 * \brief Gets the near/far clipping plane of the Camera3D instance.
		 *
		 * \return The near/far clipping plane value.
		 */
		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }

		/*!
		 * \brief Sets the zoom level of the Camera3D instance.
		 *
		 * \param zoom_level The new zoom level.
		 */
		void SetZoomLevel(float zoom_level) { mZoomLevel = std::clamp(zoom_level, CAMERA_ZOOM_MIN, CAMERA_ZOOM_MAX); }

		/*!
		 * \brief Gets the current zoom level of the Camera3D instance.
		 *
		 * \return The current zoom level.
		 */
		float GetZoomLevel() const { return mZoomLevel; }

		/*!
		 * \brief Pans the Camera3D instance by a specified delta in x and y.
		 *
		 * \param delta_x The delta movement in the x-axis.
		 * \param delta_y The delta movement in the y-axis.
		 */
		void PanCamera(float delta_x, float delta_y);

		/*!
		 * \brief Sets the aspect ratio of the Camera3D instance.
		 *
		 * \param ar The new aspect ratio.
		 */
		void SetAspectRatio(float ar) { mAspectRatio = ar;  }
		
		/*!
		 * \brief Moves the Camera3D instance based on keyboard input and move speed.
		 *
		 * \param move_speed The speed at which the Camera3D instance moves.
		 */
		void MoveCamera(float move_speed);

		/*!
		 * \brief Rotates the Camera3D instance based on the specified x and y offsets.
		 *
		 * \param xoffset The x offset for rotation.
		 * \param yoffset The y offset for rotation.
		 */
		void Rotate(float xoffset, float yoffset);

		/*!
		 * \brief Gets the view matrix of the Camera3D instance.
		 *
		 * \return The view matrix.
		 */
		glm::mat4 GetViewMatrix() { return mView; }

		/*!
		 * \brief Gets the projection matrix of the Camera3D instance.
		 *
		 * \return The projection matrix.
		 */
		glm::mat4 GetProjectionMatrix() { return mProjection; }
		
		/*!
		 * \brief Gets the camera-to-normalized device coordinates (NDC) transformation matrix.
		 *
		 * \return The camera-to-NDC transformation matrix.
		 */
		glm::mat4 getCameraToNDCXform() { return mProjection * mView; }
		
		/*!
		 * \brief Gets the aspect ratio of the Camera3D instance.
		 *
		 * \return The aspect ratio of the Camera3D instance.
		 */
		float GetAspectRatio() { return mAspectRatio; }

		/// Static objects
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