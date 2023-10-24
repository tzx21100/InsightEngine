#include "Pch.h"
#include "CoreEngine.h"

namespace IS 
{
	/*Manifold::Manifold(Manifold const& other) {
		mBodyA = other.mBodyA;
		mBodyB = other.mBodyB;
		mNormal = Vector2D();
		mDepth = 0.f;
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;
	}*/

	Manifold::Manifold() {

		mBodyA = nullptr;
		mBodyB = nullptr;
		mNormal = Vector2D();
		mDepth = 0.f;
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;
	}

	Manifold::Manifold(RigidBody* bodyA, RigidBody* bodyB,
		Vector2D const& normal, float const& depth,
		Vector2D const& contact1, Vector2D const& contact2, int const& contact_count) {

		mBodyA = bodyA;
		mBodyB = bodyB;
		mNormal = normal;
		mDepth = depth;
		mContact1 = contact1;
		mContact2 = contact2;
		mContactCount = contact_count;
	}

	void Manifold::FindContactPoints(RigidBody & bodyA, RigidBody & bodyB) {
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;

		BodyShape shapeA = bodyA.mBodyShape;
		BodyShape shapeB = bodyB.mBodyShape;

		switch (shapeA)
		{
		case BodyShape::Box:
			switch (shapeB)
			{
			case BodyShape::Box: // box vs box
				FindPolygonsContactPoints(bodyA.GetTransformedVertices(), bodyB.GetTransformedVertices(), mContact1, mContact2, mContactCount);
				break;
			case BodyShape::Circle: // box vs circle
				break;
			default:
				break;
			}
			break;
		case BodyShape::Circle:
			switch (shapeB)
			{
			case BodyShape::Box: // circle vs box
				break;
			case BodyShape::Circle: // circle vs circle
				break;
			default:
				break;
			}
			break;
		case BodyShape::Line:
			break;
		default:
			break;
		}
	}

	void Manifold::FindPolygonsContactPoints(std::vector<Vector2D> const& verticesA, std::vector<Vector2D> const& verticesB, Vector2D& contact1, Vector2D& contact2, int& contactCount) {
		
		contact1 = Vector2D();
		contact2 = Vector2D();
		contactCount = 0;

		Vector2D closest_point = Vector2D();
		float dis_sq = 0.f;
		float min_dis_sq = std::numeric_limits<float>::max();

		for (int i = 0; i < verticesA.size(); i++)
		{
			Vector2D check_point = verticesA[i];

			for (int j = 0; j < verticesB.size(); j++)
			{
				Vector2D va = verticesB[j];
				Vector2D vb = verticesB[(j + 1) % verticesB.size()]; // avoid out of range

				// calculate distance squared and closest point
				PointSegmentDistance(check_point, va, vb, dis_sq, closest_point);

				if (NearlyEqual(dis_sq, min_dis_sq)) // if the dis is same as the min dis, add second contact point
				{
					if (!NearlyEqual(closest_point, contact1))
					{ // only add the second contact point when it is different from the first contact point
						contact2 = closest_point;
						contactCount = 2;
					}
				}
				else if (dis_sq < min_dis_sq) // add first contact point
				{
					min_dis_sq = dis_sq;
					contactCount = 1;
					contact1 = closest_point;
				}
			}
		}

		for (int i = 0; i < verticesB.size(); i++)
		{
			Vector2D check_point = verticesB[i];

			for (int j = 0; j < verticesA.size(); j++)
			{
				Vector2D va = verticesA[j];
				Vector2D vb = verticesA[(j + 1) % verticesA.size()]; // avoid out of range

				PointSegmentDistance(check_point, va, vb, dis_sq, closest_point);

				if (NearlyEqual(dis_sq, min_dis_sq)) // if the dis is same as the min dis, add second contact point
				{
					if (!NearlyEqual(closest_point, contact1))
					{ // only add the second contact point when it is different from the first contact point
						contact2 = closest_point;
						contactCount = 2;
					}
				}
				else if (dis_sq < min_dis_sq) // add first contact point
				{
					min_dis_sq = dis_sq;
					contactCount = 1;
					contact1 = closest_point;
				}
			}
		}
	}

	bool Manifold::NearlyEqual(float const& a, float const& b) {
		return std::abs(a - b) < 0.05f; // less than 5 centimeter
	}

	bool Manifold::NearlyEqual(Vector2D const& a, Vector2D const& b) {
		return ISVector2DSquareDistance(a, b) < 0.5f * 0.5f ; // less than half meter square
	}

}