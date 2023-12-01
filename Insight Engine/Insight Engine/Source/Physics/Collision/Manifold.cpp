/*!
 * \file Manifold.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
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
#include "Manifold.h"
#include "Physics/System/CollisionSystem.h"
#include "Collision.h"

namespace IS 
{
	// Default constructor for the Manifold class.
	Manifold::Manifold() {
		mBodyA = nullptr;
		mBodyB = nullptr;
		mColliderA = nullptr;
		mColliderB = nullptr;
		mNormal = Vector2D();
		mDepth = 0.f;
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;
	}

	// Constructor for the Manifold class that initializes its members.
	Manifold::Manifold(RigidBody* bodyA, RigidBody* bodyB,
		Collider* colliderA, Collider* colliderB,
		Vector2D const& normal, float const& depth,
		Vector2D const& contact1, Vector2D const& contact2, int const& contact_count) {
		mBodyA = bodyA;
		mBodyB = bodyB;
		mColliderA = colliderA;
		mColliderB = colliderB;
		mNormal = normal;
		mDepth = depth;
		mContact1 = contact1;
		mContact2 = contact2;
		mContactCount = contact_count;
	}


	// Calculates the contact points for the collision between two rigid bodies.
	void Manifold::FindContactPoints(Collider& colliderA, Collider& colliderB, std::bitset<MAX_COLLIDING_CASE> colliding_collection) {
		mContact1 = Vector2D();
		mContact2 = Vector2D();
		mContactCount = 0;

		/*Collider colliderA = colliderA;
		Collider colliderB = colliderB;*/

		if (colliding_collection.test(CollidingStatus::BOX_A_BOX_B)) { // box vs box
			FindPolygonsContactPoints(colliderA.mBoxCollider.transformedVertices, colliderB.mBoxCollider.transformedVertices, mContact1, mContact2, mContactCount);
			return; // instantly return once find the contact points
		}
		if (colliding_collection.test(CollidingStatus::BOX_A_CIRCLE_B)) { // box vs circle
			FindCirclePolygonContactPoints(colliderB.mCircleCollider.center, colliderB.mCircleCollider.radius, colliderA.mBoxCollider.center, colliderA.mBoxCollider.transformedVertices, mContact1);
			mContactCount = 1;
			return; // instantly return once find the contact points
		}
		if (colliding_collection.test(CollidingStatus::CIRCLE_A_BOX_B)) { // circle vs box
			FindCirclePolygonContactPoints(colliderA.mCircleCollider.center, colliderA.mCircleCollider.radius, colliderB.mBoxCollider.center, colliderB.mBoxCollider.transformedVertices, mContact1);
			mContactCount = 1;
			return; // instantly return once find the contact points
		}
		if (colliding_collection.test(CollidingStatus::CIRCLE_A_CIRCLE_B)) { // circle vs circle
			FindCirlcesContactPoints(colliderA.mCircleCollider.center, colliderA.mCircleCollider.radius, colliderB.mCircleCollider.center, mContact1);
			mContactCount = 1;
			return; // instantly return once find the contact points
		}

	}

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

	// Calculates contact points for a collision between circle and polygon.
	void Manifold::FindCirclePolygonContactPoints(Vector2D const& circle_center, [[maybe_unused]] float const& circle_radius, [[maybe_unused]] Vector2D const& polygon_center, std::vector<Vector2D> const& polygon_vertices, Vector2D& contact_point) {
		Vector2D closest_point = Vector2D();
		float dis_sq = 0.f;
		float minDistSq = std::numeric_limits<float>::max();

		for (int i = 0; i < polygon_vertices.size(); i++)
		{
			Vector2D va = polygon_vertices[i];
			Vector2D vb = polygon_vertices[(i + 1) % polygon_vertices.size()]; // avoid out of range

			PointSegmentDistance(circle_center, va, vb, dis_sq, closest_point);

			if (dis_sq < minDistSq)
			{
				minDistSq = dis_sq;
				contact_point = closest_point;
			}
		}
	}

	// Calculates contact points for a collision between two circles.
	void Manifold::FindCirlcesContactPoints(Vector2D const& center_a, float const& radius_a, Vector2D const& center_b, Vector2D& contact_point) {
		Vector2D ab = center_b - center_a;
		ISVector2DNormalize(ab, ab);
		contact_point = center_a + ab * radius_a;
	}

	// Compares two floating - point values for near equality.
	bool Manifold::NearlyEqual(float const& a, float const& b) {
		//return std::abs(a - b) < 0.005f; // less than half cm
		return std::abs(a - b) < 1.f;
	}

	// Compares two 2D vectors for near equality.
	bool Manifold::NearlyEqual(Vector2D const& a, Vector2D const& b) {
		//return ISVector2DSquareDistance(a, b) < 0.005f * 0.005f ; // less than half cm square
		return ISVector2DSquareDistance(a, b) < 1.f * 1.f ;
	}

}