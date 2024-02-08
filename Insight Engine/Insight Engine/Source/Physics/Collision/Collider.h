/*!
 * \file Collider.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the Collider class and associated structures for 
 * the Insight Engine's physics system. It includes definitions for BoxCollider, 
 * CircleCollider, and LineCollider structures, as well as the Collider class.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLIDER_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLIDER_H

/*                                                                   includes
 ----------------------------------------------------------------------------- */
#include <vector>
#include <bitset>

#include "Math/Vector2D.h"
#include "Collision.h"
#include "Graphics/System/Transform.h"

#define MAX_COLLIDER 4 // 3 colliders

namespace IS
{
	/*!
	 * \enum ColliderShape
	 * \brief Enum for identifying the shape of a collider.
	 */
	enum ColliderShape : int
	{
		INVALID = 0,
		BOX,
		CIRCLE,
		LINE
	};

	/*!
	 * \struct BoxCollider
	 * \brief Structure representing a box collider with center, offset, size scaling, and vertices.
	 */
	struct BoxCollider
	{
		Vector2D center;								// Center point of the box collider
		Vector2D offset;								// Offset of the collider from the center
		Vector2D sizeScale;								// Scale of the collider size
		std::vector<Vector2D> vertices;					// Vertices of the box
		std::vector<Vector2D> transformedVertices;		// Transformed vertices after applying transformations

		BoxCollider();
		BoxCollider(Vector2D const& center_, Vector2D const& offset_, Vector2D const& sizeScale_, std::vector<Vector2D> const& vertices_, std::vector<Vector2D> const& transformedVertices_);


	};

	/*!
	 * \struct CircleCollider
	 * \brief Structure representing a circle collider.
	 */
	struct CircleCollider
	{
		Vector2D center;								// Center of the circle
		float radius;									// Radius of the circle
		Vector2D offset;								// Offset of the collider from the center
		float radiusScale;								// Scale of the collider radius

		CircleCollider();
		CircleCollider(Vector2D const& center_, float const& radius_, Vector2D const& offset_);
	};

	/*!
	 * \struct LineCollider
	 * \brief Structure representing a line collider.
	 */
	struct LineCollider
	{
		Vector2D start;									// Starting point of the line segment
		Vector2D end;									// Ending point of the line segment
		Vector2D normal;								// Normal vector of the line segment
		Vector2D offset;								// Offset of the collider from the start point
		Vector2D sizeScale;								// Scale of the line segment
	};

	/*!
	 * \class Collider
	 * \brief Class handling colliders in the physics system.
	 *
	 * This class is responsible for managing box and circle colliders, including their creation, updating, and enabling/disabling.
	 */
	class Collider : public IComponent
	{
	public:
		BoxCollider mBoxCollider;						// Box collider struct
		CircleCollider mCircleCollider;					// Circle collider struct
		// line collider to be implemented
		std::bitset<MAX_COLLIDER> mSelectedCollider;	// Bitset for selected colliders
		bool mIsColliding;								// Flag indicating if the collider is currently in a collision
		std::vector<Entity> mCollidingEntity;			// List of entities currently colliding with this collider
		bool mResponseEnable;							// Flag to enable or disable collision response
		float mCollidedObjectAngle;						// Angle of the collided object

		Json::Value Serialize() override;
		void Deserialize(Json::Value data) override;

		static std::string GetType() { return "Collider"; }

		/*!
		 * \brief Default constructor for Collider.
		 */
		Collider();

		/*!
		 * \brief Updates the Collider based on the Transform.
		 * \param trans The Transform object to update the Collider with.
		 */
		void UpdateCollider(Transform const& trans);

		/*!
		 * \brief Calculate all the vertices for a 2D axis-aligned bounding box from origin (Box shape).
		 * \param width The width of the collider.
		 * \param height The height of the collider.
		 * \return A vector containing the vertices of the box collider.
		 */
		void CreateBoxVertices(float width, float height);

		/*!
		 * \brief Updates the box collider's transformed vertices based on the provided Transform.
		 * \param trans The Transform object that includes position, scale, and rotation data.
		 */
		void UpdateBoxCollider(Transform const& trans);

		/*!
		 * \brief Updates the circle collider based on the provided Transform.
		 * \param trans The Transform object to update the circle collider with.
		 */
		void UpdateCircleCollider(Transform const& trans);

		/*!
		 * \brief Activates all collider types (box, circle, etc.) in this Collider object.
		 */
		void EnableAllColliders();

		/*!
		 * \brief Deactivates all collider types in this Collider object.
		 */
		void DisableAllColliders();

		/*											HELPER FUNCTION FOR BOX COLLIDER
		----------------------------------------------------------------------------- */
		/*!
		 * \brief Activates the box collider component of this Collider object.
		 */
		void EnableBoxCollider();

		/*!
		 * \brief Deactivates the box collider component of this Collider object.
		 */
		void DisableBoxCollider();

		/*!
		 * \brief Checks if the box collider component is active.
		 * \return True if the box collider is active, false otherwise.
		 */
		bool IsBoxColliderEnable();

		/*											HELPER FUNCTION FOR CIRCLE COLLIDER
		---------------------------------------------------------------------------- - */
		/*!
		 * \brief Activates the circle collider component of this Collider object.
		 */
		void EnableCircleCollider();

		/*!
		 * \brief Deactivates the circle collider component of this Collider object.
		 */
		void DisableCircleCollider();

		/*!
		 * \brief Checks if the circle collider component is active.
		 * \return True if the circle collider is active, false otherwise.
		 */
		bool IsCircleColliderEnable();

		/**
		 * \brief Get the axis-aligned bounding box (AABB) of the collider.
		 * \return The AABB box of the collider.
		 */
		Box GetAABB();

	};

}
#endif // !GAM200_INSIGHT_ENGINE_
