/*!
 * \file Manifold.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 01-11-2023
 * \brief
 * This header file contains the definitions of the Manifold class, which represents contact information
 * between two objects in a physics simulation.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "CoreEngine.h"

namespace IS 
{
	// Default constructor for the Manifold class.
	Manifold::Manifold() {
		mColliderA = nullptr;
		mColliderB = nullptr;
		mNormal = Vector2D();
		mDepth = 0.f;
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;
	}

	// Constructor for the Manifold class that initializes its members.
	Manifold::Manifold(Collider* colliderA, Collider* colliderB,
		Vector2D const& normal, float const& depth,
		Vector2D const& contact1, Vector2D const& contact2, int const& contact_count) {
		mColliderA = colliderA;
		mColliderB = colliderB;
		mNormal = normal;
		mDepth = depth;
		mContact1 = contact1;
		mContact2 = contact2;
		mContactCount = contact_count;
	}

#if 0
	// Calculates the contact points for the collision between two rigid bodies.
	void Manifold::FindContactPoints(Collider& colliderA, Collider& colliderB) {
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;

		Collider colliderA = colliderA;
		Collider colliderB = colliderB;

		if (colliderA.mSelectedCollider.test(ColliderShape::BOX)) {
			if (colliderB.mSelectedCollider.test(ColliderShape::BOX)) { // box vs box
				FindPolygonsContactPoints(bodyA.GetTransformedVertices(), bodyB.GetTransformedVertices(), mContact1, mContact2, mContactCount);
			}
			if () {

			}
			if () {

			}
		}

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
#endif
	// Calculates contact points for a collision between two polygons.
	void Manifold::FindPolygonsContactPoints(std::vector<Vector2D> const& verticesA, std::vector<Vector2D> const& verticesB, Vector2D& contact1, Vector2D& contact2, int& contactCount) {

		Vector2D closest_point = Vector2D();
		float dis_sq = 0.f;
		float min_dis_sq = std::numeric_limits<float>::max();

		// each vertice on A need to do check with each line segment of B
		for (int i = 0; i < verticesA.size(); i++)
		{
			Vector2D check_point = verticesA[i];

			for (int j = 0; j < verticesB.size(); j++)
			{
				Vector2D va = verticesB[j];
				Vector2D vb = verticesB[(j + 1) % verticesB.size()]; // avoid out of range

				// calculate distance squared and closest point
				PointSegmentDistance(check_point, va, vb, dis_sq, closest_point);

				if (NearlyEqual(dis_sq, min_dis_sq)) // if the dis is nearly same as the min dis, means they close enough, add second contact point
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

	// Compares two floating - point values for near equality.
	bool Manifold::NearlyEqual(float const& a, float const& b) {
		return std::abs(a - b) < 0.005f; // less than half cm
	}

	// Compares two 2D vectors for near equality.
	bool Manifold::NearlyEqual(Vector2D const& a, Vector2D const& b) {
		return ISVector2DSquareDistance(a, b) < 0.005f * 0.005f ; // less than half cm square
	}

}