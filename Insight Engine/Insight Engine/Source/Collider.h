
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLIDER_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLIDER_H

/*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Vector2D.h"
#include <vector>
#include <bitset>

#define MAX_COLLIDER 4 // 3 colliders

namespace IS
{
	//enum ColliderShape
	//{
	//	invalid = 0,
	//	box = 1 << 1,
	//	circle = 1 << 2,
	//	line = 1 << 3,
	//	//sat = 1 << 4,
	//	//obb = 1 << 5
	//};

	enum ColliderShape : int
	{
		INVALID = 0,
		BOX,
		CIRCLE,
		LINE
	};

	struct BoxCollider
	{
		Vector2D center;
		//Vector2D min; // Minimum point of the bounding box
		//Vector2D max; // Maximum point of the bounding box
		Vector2D offset;
		Vector2D sizeScale;
		std::vector<Vector2D> vertices;
		std::vector<Vector2D> transformedVertices;

		BoxCollider();
		BoxCollider(Vector2D const& center_, Vector2D const& offset_, Vector2D const& sizeScale_, std::vector<Vector2D> const& vertices_, std::vector<Vector2D> const& transformedVertices_);


	};

	/*!
	 * \struct Circle
	 * \brief A structure representing a circle defined by its center and radius.
	 */
	struct CircleCollider
	{
		Vector2D center; // Center of the circle
		float    radius; // Radius of the circle
		Vector2D offset;
		float radiusScale;

		CircleCollider();
		CircleCollider(Vector2D const& center_, float const& radius_, Vector2D const& offset_);
	};

	/*!
	 * \struct Line
	 * \brief A structure representing a line segment defined by its start and end points, along with a normal vector.
	 */
	struct LineCollider
	{
		Vector2D start; // Starting point of the line segment
		Vector2D end; // Ending point of the line segment
		Vector2D normal; // Normal vector of the line segment
		Vector2D offset;
		Vector2D sizeScale;
	};

	class Collider : public IComponent
	{
	public:
		BoxCollider mBoxCollider;
		CircleCollider mCircleCollider;
		//LineCollider mLineCollider;
		std::bitset<MAX_COLLIDER> mSelectedCollider;
		bool mIsColliding;
		Entity mCollidingEntity;
		bool mResponseEnable;
		float mCollidedObjectAngle;

		Json::Value Serialize() override;
		void Deserialize(Json::Value data) override;

		static std::string GetType() { return "Collider"; }

		Collider();

		void CreateCollider();

		void UpdateCollider(Transform const& trans);

		void CreateBoxCollider();

		void CreateBoxVertices(float width, float height);

		void UpdateBoxCollider(Transform const& trans);

		void UpdateCircleCollider(Transform const& trans);

		// set all colliders active
		void EnableAllColliders();

		// set all colliders deactive
		void DisableAllColliders();

		// HELPER FUNCTION FOR BOX COLLIDER
		// set box collider active
		void EnableBoxCollider();

		// set box collider deactive
		void DisableBoxCollider();

		// check if box collider is enable or not
		bool IsBoxColliderEnable();

		// HELPER FUNCTION FOR CIRCLE COLLIDER
		// set circle collider active
		void EnableCircleCollider();

		// set circle collider deactive
		void DisableCircleCollider();

		// check if circle collider is enable or not
		bool IsCircleColliderEnable();

	};

}
#endif // !GAM200_INSIGHT_ENGINE_
