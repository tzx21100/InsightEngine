/*!
 * \file Transform.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
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
#include "Engine/Core/Core.h"
#include "Engine/Core/CoreEngine.h"
#include <glm/glm.hpp>

namespace IS {

	class IS_API Transform : public IComponent {
	public:
		// Member variables
		Vec2D world_position{};	// world coordinates (if world is 1280 pixels, valid world positions: [-640, 640]
		float rotation{};		// z-axis rotation in degrees
		float angle_speed{};	// rotation speed
		Vec2D scaling{};		// x = width, y = height
		Mtx33 mdl_to_ndc_xform{}; // identity matrix default

		// 3D cam test
		glm::mat4 mdl_to_3dcam_to_ndc_xform{};

		/*!
		 * \brief Gets the type of the Transform component.
		 * \return The name of the Transform component.
		 */
		static std::string GetType() {
			return "Transform";
		}

		/*!
		 * \brief != operator overload.
		 * \param other The transform to compare with.
		 * \return Boolean flag indicating the outcome.
		 */
		bool operator!=(Transform const& other)
		{
			return !(world_position == other.world_position && rotation == other.rotation && angle_speed && other.angle_speed && scaling == other.scaling);
		}

		/*!
		 * \brief == operator overload.
		 * \param other The transform to compare with.
		 * \return Boolean flag indicating the outcome.
		 */
		bool operator==(Transform const& other)
		{
			return (world_position == other.world_position && rotation == other.rotation && angle_speed && other.angle_speed && scaling == other.scaling);
		}

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
		Transform(Vec2D world_position, float rotation, Vec2D scaling);

		/*!
		 * \brief Get the world position.
		 *
		 * \return The world position as a Vector2D.
		 */
		Vector2D getWorldPosition();

		/**
		 * Transforms a 2D point `v` into world coordinates using the transformation 
		 * represented by this Transform object.
		 * The transformation includes rotation and translation.
		 *
		 * @param v A reference to a Vector2D point to be transformed.
		 */
		void getTransformedPoint(Vector2D& v) const;

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

		/*!
		 * \brief Set the scaling factors.
		 *
		 * \param vec the new Vector for scaling factor.
		 */
		void setScaling(Vector2D vec);

		/*!
		 * \brief force change/update/move the transform position by specific value (for collision response)
		 *
		 * \param value adding with type Vector2D
		 */
		void Move(Vector2D const& val);

		/**
		 * @brief Returns the 3D transformation matrix for the object.
		 *
		 * This function calculates and returns the 3D transformation matrix for the object based on its
		 * world position, rotation, scaling, and the active 3D camera's transformation to NDC space.
		 *
		 * @return The 3D transformation matrix for the object.
		 */
		glm::mat4 Return3DXformMatrix();

		/**
		 * @brief Gets the transformation matrix from camera space to world space.
		 *
		 * This function calculates and returns the transformation matrix from camera space to world space
		 * based on the object's world position, rotation, and scaling.
		 *
		 * @return The transformation matrix from camera space to world space.
		 */
		glm::mat4 GetCameraToWorldTransform();

		/**
		 * \brief Gets the current mouse position in world coordinates.
		 *
		 * \return A pair representing the x and y coordinates of the mouse cursor.
		 */
		static std::pair<double, double> GetMousePosition();

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

		// not in use
		// std::vector<Vector2D> GetSquareTransformVertices();
	};

	/**
	 * Converts a custom 3x3 matrix (Matrix3x3) to a glm 3x3 matrix (glm::mat3).
	 *
	 * @param mat The custom 3x3 matrix to be converted.
	 * @return A glm 3x3 matrix that represents the same transformation as the custom matrix.
	 */
	glm::mat3 ISMtx33ToGlmMat3(Matrix3x3 const& mat);

	/**
	 * Converts a glm 3x3 matrix (glm::mat3) to a custom 3x3 matrix (Matrix3x3).
	 *
	 * @param mat The glm 3x3 matrix to be converted.
	 * @return A custom 3x3 matrix that represents the same transformation as the glm matrix.
	 */
	Matrix3x3 GlmMat3ToISMtx33(glm::mat3 const& mat);

	/**
	 * @brief Converts a 4x4 InsightMatrix to a glm::mat4.
	 *
	 * This function converts a 4x4 InsightMatrix to a glm::mat4.
	 *
	 * @param mat The input 4x4 InsightMatrix.
	 * @return The resulting glm::mat4.
	 */
	glm::mat4 ISMtx44ToGlmMat4(Matrix4x4 const& mat);

	/**
	 * @brief Converts a glm::mat4 to a 4x4 InsightMatrix.
	 *
	 * This function converts a glm::mat4 to a 4x4 InsightMatrix.
	 *
	 * @param mat The input glm::mat4.
	 * @return The resulting 4x4 InsightMatrix.
	 */
	Matrix4x4 GlmMat4ToISMtx44(glm::mat4 const& mat);
}

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_TRANSFORM_H