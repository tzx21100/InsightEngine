
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLIDER_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLIDER_H

/*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#define MAX_COLLIDER 2
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
		static std::string GetType() { return "Collider"; }


		/*uint16_t mShape;
		Vector2D mOffset;
		Vector2D mSizeScale;
		std::vector<Vector2D> mVertices;
		std::vector<Vector2D> mTransformedVertices;
		GridState mGridState;*/
		BoxCollider mBoxCollider;
		//CircleCollider mCircleCollider;
		//LineCollider mLineCollider;
		std::bitset<MAX_COLLIDER> mSelectedCollider;

		Collider();

		void CreateCollider();

		void UpdateCollider(Transform const& trans);

		void CreateBoxCollider();

		void CreateBoxVertices(float width, float height);

		void UpdateBoxCollider(Transform const& trans);


	};

	
}
#endif
