/*!
 * \file Transform.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 27-09-2023
 * \brief
 * This header file defines the Transform class, which represents transformation properties
 * for game objects in an OpenGL-based application.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TRANSFORM_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TRANSFORM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "CoreEngine.h"
#include <glm/glm.hpp>

namespace IS {
	class Transform : public IComponent {
	public:
		/*!
		 * \brief Default constructor for the Transform class.
		 */
		Transform();

		/*!
		 * \brief Constructor for the Transform class with initial values.
		 *
		 * \param world_position The initial world position as a glm::vec2.
		 * \param rotation The initial rotation angle in degrees.
		 * \param scaling The initial scaling factors as a glm::vec2.
		 */
		Transform(glm::vec2 world_position, float rotation, glm::vec2 scaling);

		/*!
		 * \brief Get the world position.
		 *
		 * \return The world position as a Vector2D.
		 */
		Vector2D getWorldPosition();

		/*!
		 * \brief Get the rotation angle.
		 *
		 * \return The rotation angle in degrees.
		 */
		float getRotation();

		/*!
		 * \brief Get the scaling factors.
		 *
		 * \return The scaling factors as a Vector2D.
		 */
		Vector2D getScaling();

		/*!
		 * \brief Set the world position.
		 *
		 * \param x The X-coordinate of the world position.
		 * \param y The Y-coordinate of the world position.
		 */
		void setWorldPosition(float x, float y);

		/*!
		 * \brief Set the rotation angle.
		 *
		 * \param angle The new rotation angle in degrees.
		 * \param angleSpeed The angular speed for rotation (default is 0.0).
		 */
		void setRotation(float angle, float angleSpeed = 0.f);

		/*!
		 * \brief Set the scaling factors.
		 *
		 * \param width The new width scaling factor.
		 * \param height The new height scaling factor.
		 */
		void setScaling(float width, float height);

		// members
		glm::vec2 world_position{};					 // world coordinates (if world is 1280 pixels, valid world positions: [-640, 640]
		float rotation{};							 // z-axis rotation in degrees
		float angle_speed{};						 // rotation speed
		glm::vec2 scaling{};						 // x = width, y = height
		glm::mat3 mdl_to_ndc_xform = glm::imat3x3{}; // identity matrix default

		/*!
		 * \brief Serialize the Transform object to a JSON representation.
		 *
		 * This function serializes the Transform object's properties to a JSON Value.
		 *
		 * \return A JSON Value containing the serialized data.
		 */
		Json::Value Serialize() override;

		/*!
		 * \brief Deserialize the Transform object from a JSON representation.
		 *
		 * This function deserializes the Transform object's properties from a JSON Value.
		 *
		 * \param data The JSON Value containing the serialized data.
		 */
		void Deserialize(Json::Value data) override;
	};
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TRANSFORM_H