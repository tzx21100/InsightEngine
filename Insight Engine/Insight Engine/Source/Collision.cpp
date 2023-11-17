/*!
 * \file Collision.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
 * \brief
 * This header file contains functions needed for handling collision detection between
 * different shapes like circles, polygons, and axis-aligned bounding boxes (AABB).
 * It includes functionality to detect and respond to collisions between these shapes,
 * considering their velocities, positions, and normals.
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
#include "Collision.h"

namespace IS
{

	// FOR BOX

	// AABB Collision for static and dynamic, return true if they colliding
	bool CollisionIntersectionAABB(const Box& aabb1, const Vector2D& vel1,
		const Box& aabb2, const Vector2D& vel2, const float& dt)
	{
		/*
		Implement the collision intersection over here.

		The steps are:
		Step 1: Check for collision detection between rectangles (assuming static here).
				If the check returns no overlap you continue with the following steps.

		Step 2: Initialize and calculate the new velocity of Vb
				tFirst = 0
				tLast = dt

		Step 3: Working with one dimension (x-axis).
				if(Vb < 0)
					case 1
					case 4
				if(Vb > 0)
					case 2
					case 3
				case 5
		Step 4: Repeat step 3 on the y-axis
		Step 5: Otherwise the rectangles intersect
		*/

		// check static collision
		if (aabb1.min.x > aabb2.max.x || aabb2.min.x > aabb1.max.x || aabb1.max.y < aabb2.min.y || aabb2.max.y < aabb1.min.y) {
			// no static intersection then continue check the dynamic collision

			// dt first and last
			float tFirst = 0;
			float tLast = dt;

			Vector2D Va = vel1, Vb = vel2;
			// Vb - Va then assgin to Va, now Va is Vreal
			//AEVec2Sub(&Va, &Vb, &Va);
			Va = Vb - Va;

			// dynamic collision
			// check x-axis
			if (Va.x < 0) {
				// case 1
				if (aabb1.min.x > aabb2.max.x) {
					return 0; // No intersection and Bis moving away
				}
				// case 4
				if (aabb1.max.x < aabb2.min.x) {
					tFirst = std::max((aabb1.max.x - aabb2.min.x) / Va.x, tFirst);
				}
				if (aabb1.min.x < aabb2.max.x) {
					tLast = std::min((aabb1.min.x - aabb2.max.x) / Va.x, tLast);
				}
			}
			if (Va.x > 0) {
				// case 2
				if (aabb1.min.x > aabb2.max.x) {
					tFirst = std::max((aabb1.min.x - aabb2.max.x) / Va.x, tFirst);
				}
				if (aabb1.max.x > aabb2.min.x) {
					tLast = std::min((aabb1.max.x - aabb2.min.x) / Va.x, tLast);
				}
				// case 3
				if (aabb1.max.x < aabb2.min.x) {
					return 0;
				}
			}
			// case 5
			if (tFirst > tLast) {
				return 0;
			}

			// check y-axis
			if (Va.y < 0) {
				// case 1
				if (aabb1.min.y > aabb2.max.y) {
					return 0; // No intersection and Bis moving away
				}
				// case 4
				if (aabb1.max.y < aabb2.min.y) {
					tFirst = std::max((aabb1.max.y - aabb2.min.y) / Va.y, tFirst);
				}
				if (aabb1.min.y < aabb2.max.y) {
					tLast = std::min((aabb1.min.y - aabb2.max.x) / Va.y, tLast);
				}
			}
			if (Va.y > 0) {
				// case 2
				if (aabb1.min.y > aabb2.max.y) {
					tFirst = std::max((aabb1.min.y - aabb2.max.y) / Va.y, tFirst);
				}
				if (aabb1.max.y > aabb2.min.y) {
					tLast = std::min((aabb1.max.y - aabb2.min.y) / Va.y, tLast);
				}
				// case 3
				if (aabb1.max.y < aabb2.min.y) {
					return 0;
				}
			}
			// case 5
			if (tFirst > tLast) {
				return 0;
			}
		}
		else {
			return true;
		}
		return 0;
	}

	// calculate the shortest distance from a point to the line segment. (for getting contact points)
	void PointSegmentDistance(Vector2D const& point, Vector2D const& a, Vector2D const& b, float& distance_squared, Vector2D& closest_point) {
		Vector2D line = b - a; // line segment
		Vector2D ap = point - a; // for use of projecting onto ab

		float proj = ISVector2DDotProduct(ap, line); // project ap onto line segement
		float ab_lenSq = ISVector2DSquareLength(line);
		float d = proj / ab_lenSq; // d is the portion of proj on abLensq with range from 0 to 1

		if (d <= 0.0f) { // heading opposite of line from a
			closest_point = a;
		}
		else if (d >= 1.0f) { // out of line from b
			closest_point = b;
		}
		else { // between a and b
			closest_point = a + line * d;
		}

		// getting the distance from point to the line segment
		distance_squared = ISVector2DSquareDistance(point, closest_point);
	}

	// static AABB collision, return false if not colliding
	bool StaticIntersectAABB(const Box& a, const Box& b) {
		if (a.max.x < b.min.x || b.max.x < a.min.x ||
			a.max.y < b.min.y || b.max.y < a.min.y) {
			return false;
		}
		
		return true;
	}

	// Uses the Axis Separation Theorem (AST) to check collision between two polygons and calculates collision response data normal and depth
	bool IntersectionPolygons(std::vector<Vector2D> const& trans_verticesA, Vector2D centerA, std::vector<Vector2D> trans_verticesB, Vector2D centerB, Vector2D& normal, float& depth){

		// init
		normal = Vector2D();
		depth = std::numeric_limits<float>::max();
		// check seperating vertices on object A
		for (int i = 0; i < trans_verticesA.size(); i++) {
			Vector2D va = trans_verticesA[i];
			Vector2D vb = trans_verticesA[(i + 1) % trans_verticesA.size()]; // modules by the size of the vector to avoid going out of the range

			Vector2D edge = vb - va; // getting the vector of the side
			Vector2D axis(-edge.y, edge.x); // getting the left normal
			Vector2D axisnormalized;
			ISVector2DNormalize(axisnormalized, axis); // normalize the axis

			// init min & max for Object A & B
			float minA = std::numeric_limits<float>::max();
			float maxA = -std::numeric_limits<float>::max(); // negative max
			float minB = std::numeric_limits<float>::max();
			float maxB = -std::numeric_limits<float>::max(); // negative max

			// projecting all the vertices onto the normal
			ProjectVertices(trans_verticesA, axisnormalized, minA, maxA);
			ProjectVertices(trans_verticesB, axisnormalized, minB, maxB);

			if (minA >= maxB || minB >= maxA) { // if not interest or leaving gaps between, return false
				return false;
			}

			// calculate the depth of intersection on normal
			float intersectDepth = std::min(maxB - minA, maxA - minB);

			if (intersectDepth < depth) // updating the depth and normal
			{
				depth = intersectDepth;
				normal = axisnormalized;
			}
		}

		// check seperating vertices on object B
		for (int i = 0; i < trans_verticesB.size(); i++) {
			Vector2D va = trans_verticesB[i];
			Vector2D vb = trans_verticesB[(i + 1) % trans_verticesB.size()]; // modules by the size of the vector to avoid going out of the range

			Vector2D edge = vb - va; // getting the vector of the side
			Vector2D axis(-edge.y, edge.x); // getting the left normal
			Vector2D axisnormalized;
			ISVector2DNormalize(axisnormalized, axis); // normalize the axis

			// init min & max for Object A & B
			float minA = std::numeric_limits<float>::max();
			float maxA = -std::numeric_limits<float>::max(); // negative max
			float minB = std::numeric_limits<float>::max();
			float maxB = -std::numeric_limits<float>::max(); // negative max

			// projecting all the vertices onto the normal
			ProjectVertices(trans_verticesA, axisnormalized, minA, maxA);
			ProjectVertices(trans_verticesB, axisnormalized, minB, maxB);

			if (minA >= maxB || minB >= maxA) { // if not interest or leaving gaps between, return false
				return false;
			}

			// calculate the depth of intersection on normal
			float intersectDepth = std::min(maxB - minA, maxA - minB);

			if (intersectDepth < depth) // updating the depth and normal
			{
				depth = intersectDepth;
				normal = axisnormalized;
			}
		}

		//calculate norml
		Vector2D direction = centerB - centerA;

		// check the direction of direction vector and normal vector, make sure they moving to same direction for collision response
		if (ISVector2DDotProduct(direction, normal) < 0.f) // => bigger than 90 degree, means moving apart (different direction)
		{
			// reverse the direction of normal
			normal = -normal;
		}

		return true;
	}

	// Projects all vertices of a polygon onto a given normal vector and finds the minimum and maximum projections
	void ProjectVertices(std::vector<Vector2D> const& vertices, Vector2D const& normal, float& min, float& max)
	{

		for (int i = 0; i < vertices.size(); i++)
		{
			Vector2D v = vertices[i];
			float proj = ISVector2DDotProduct(v, normal); // dot product for projecting vertices onto normal

			if (proj < min) { min = proj; }
			if (proj > max) { max = proj; }
		}
	}

	// FOR CIRCLE VS POLYGON

	// Uses the Axis Separation Theorem (AST) to check collision between a circle and a polygon and calculates collision response data normal and depth
	bool IntersectionCirlcecPolygon(Vector2D circle_center, float radius, Vector2D polygon_center, std::vector<Vector2D> trans_vertices, Vector2D& normal, float& depth) {
		normal = Vector2D();
		depth = std::numeric_limits<float>::max();

		Vector2D axis;
		float axisDepth = 0.f;

		for (int i = 0; i < trans_vertices.size(); i++)
		{
			// getting two vertices to form a edge vector
			Vector2D va = trans_vertices[i];
			Vector2D vb = trans_vertices[(i + 1) % trans_vertices.size()]; // modules by the size of the vector to avoid going out of the range

			Vector2D edge = vb - va;
			axis = Vector2D(-edge.y, edge.x);
			ISVector2DNormalize(axis, axis); // normalize the axis

			//float minA, maxA, minB, maxB;
			float minA = std::numeric_limits<float>::max();
			float maxA = -std::numeric_limits<float>::max(); // negative max
			float minB = std::numeric_limits<float>::max();
			float maxB = -std::numeric_limits<float>::max(); // negative max
			ProjectVertices(trans_vertices, axis, minA, maxA);
			ProjectCircle(circle_center, radius, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA) // not intersecting
			{
				return false;
			}

			axisDepth = std::min(maxB - minA, maxA - minB);

			if (axisDepth < depth)
			{
				depth = axisDepth;
				normal = axis;
			}
		}

		// another case
		// getting the closest point on polygon to the circle
		Vector2D cp = trans_vertices[FindClosestPointOnPolygon(circle_center, trans_vertices)];

		axis = cp - circle_center;
		ISVector2DNormalize(axis, axis);

		//float minA, maxA, minB, maxB;
		float minA = std::numeric_limits<float>::max();
		float maxA = -std::numeric_limits<float>::max(); // negative max
		float minB = std::numeric_limits<float>::max();
		float maxB = -std::numeric_limits<float>::max(); // negative max
		ProjectVertices(trans_vertices, axis, minA, maxA);
		ProjectCircle(circle_center, radius, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
		{
			return false;
		}

		axisDepth = std::min(maxB - minA, maxA - minB);

		if (axisDepth < depth)
		{
			depth = axisDepth;
			normal = axis;
		}

		Vector2D direction = polygon_center - circle_center;

		if (ISVector2DDotProduct(direction, normal) < 0.f)
		{
			normal = -normal;
		}

		return true;

	}


	// Projects the left and right edges of a circle onto a given normal vector and finds the minimum and maximum projections
	void ProjectCircle(Vector2D const& center, float const& radius, Vector2D const& normal, float& min, float& max)
	{
		Vector2D directionRadius = normal * radius;

		Vector2D p1 = center + directionRadius;
		Vector2D p2 = center - directionRadius;

		min = ISVector2DDotProduct(p1, normal);
		max = ISVector2DDotProduct(p2, normal);

		if (min > max) {
			// swap min and max
			float tmp = min;
			min = max;
			max = tmp;
		}
	}

	// Finds the closest point on a polygon to a given circle center
	int FindClosestPointOnPolygon(Vector2D circle_center, std::vector<Vector2D> vertices)
	{
		int result = -1;
		float minDistance = std::numeric_limits<float>::max();

		for (int i = 0; i < vertices.size(); i++)
		{
			Vector2D v = vertices[i];
			float distance = ISVector2DDistance(v, circle_center);

			if (distance < minDistance)
			{
				minDistance = distance;
				result = i;
			}
		}

		return result;
	}


	// FOR CIRCLE

	// Checks if two circles are intersecting
	bool IntersectionCircles(Vector2D centerA, float radiusA, Vector2D centerB, float radiusB, Vector2D& normal, float& depth) {
		normal = Vector2D();
		depth = std::numeric_limits<float>::max(); // issue

		float distance = ISVector2DDistance(centerA, centerB);
		float totalRadius = radiusA + radiusB;

		if (distance >= totalRadius) {
			return false;
		}
		//calculate normal
		ISVector2DNormalize(normal, (centerB - centerA));
		depth = totalRadius - distance;
		return true;
	}

	//line segment reference, p0 - start, p1 - end
	void BuildLineSegment(Line& lineSegment, const Vector2D& p0, const Vector2D& p1) {
		Vector2D normal = { (p1.y - p0.y), -(p1.x - p0.x) };
		ISVector2DNormalize(normal, normal);

		lineSegment.start = p0;
		lineSegment.end = p1;
		lineSegment.normal = normal;
	}

	// check whether the ball is going to collide with the line segment and check whether the ball is inside or outside of the line segement
	int CollisionIntersection_CircleLineSegment(
		const Circle& circle,													//Circle data - input
		const Vector2D& ptEnd,													//End circle position - input
		const Line& lineSeg,												    //Line segment - input
		Vector2D& interPt,														//Intersection point - output
		Vector2D& normalAtCollision,											//Normal vector at collision time - output
		float& interTime,														//Intersection time ti - output
		bool& checkLineEdges)
	{
		Vector2D vVector = ptEnd - circle.center;
		Vector2D mVector{ vVector.y, -vVector.x };

		// if(N^.Bs - N^.P0 <= -R), Bs starting from the inside/left half plane
		if (ISVector2DDotProduct(lineSeg.normal, circle.center) - ISVector2DDotProduct(lineSeg.normal, lineSeg.start) <= -(circle.radius)) {

			// simulate LNS1 lline edge points
			Vector2D p0Prime{ lineSeg.start - (circle.radius * lineSeg.normal) };
			Vector2D p1Prime{ lineSeg.end - (circle.radius * lineSeg.normal) };

			// if(M.BsP0' * M.BsP1' < 0)
			if ((ISVector2DDotProduct(mVector, (p0Prime - circle.center)) *
				ISVector2DDotProduct(mVector, (p1Prime - circle.center))) < 0) {

				interTime = (ISVector2DDotProduct(lineSeg.normal, lineSeg.start) -
					ISVector2DDotProduct(lineSeg.normal, circle.center) -
					circle.radius) /
					(ISVector2DDotProduct(lineSeg.normal, vVector));

				if (0.f <= interTime && interTime <= 1.f) {
					interPt = circle.center + (vVector * interTime);
					//B'e = ApplyReflection(-N^, BiBe);
					normalAtCollision = -lineSeg.normal;
					return 1;
				}
				else {
					checkLineEdges = false;
					return CheckMovingCircleToLineEdge(checkLineEdges, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
				}
			}
		}
		// if(N^.Bs - N^.P0 >= R), Bs starting from the outside/right half plane
		else if (ISVector2DDotProduct(lineSeg.normal, circle.center) - ISVector2DDotProduct(lineSeg.normal, lineSeg.start) >= (circle.radius)) {

			// simulate LNS2 line edge points
			Vector2D p0Prime{ lineSeg.start + (circle.radius * lineSeg.normal) };
			Vector2D p1Prime{ lineSeg.end + (circle.radius * lineSeg.normal) };

			if ((ISVector2DDotProduct(mVector, (p0Prime - circle.center))
				* ISVector2DDotProduct(mVector, (p1Prime - circle.center)))
				< 0) { // if(M.BsP0' * M.BsP1' < 0)

				interTime = (ISVector2DDotProduct(lineSeg.normal, lineSeg.start) -
					ISVector2DDotProduct(lineSeg.normal, circle.center) +
					circle.radius) /
					(ISVector2DDotProduct(lineSeg.normal, vVector));

				if (interTime >= 0.f && interTime <= 1.f) {
					interPt = circle.center + (vVector * interTime);
					//B'e = ApplyReflection(N^, BiBe);
					normalAtCollision = lineSeg.normal;
					return 1;
				}
				else {
					checkLineEdges = false;
					return CheckMovingCircleToLineEdge(checkLineEdges, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
				}
			}
		}
		else {
			checkLineEdges = true;
			return CheckMovingCircleToLineEdge(checkLineEdges, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
		}
		return 0;
	}


	// moving the ball when it is going to collide with the line edge and reflecting the ball accordingly
	int CheckMovingCircleToLineEdge(
		bool withinBothLines,	                            					//Flag stating that the circle is starting from between 2 imaginary line segments distant +/- Radius respectively - input
		const Circle& circle,													//Circle data - input
		const Vector2D& ptEnd,													//End circle position - input
		const Line& lineSeg,												    //Line segment - input
		Vector2D& interPt,														//Intersection point - output
		Vector2D& normalAtCollision,											//Normal vector at collision time - output
		float& interTime)														//Intersection time ti - output
	{
		// v vector of the circile, Be - Bs
		Vector2D vVector = ptEnd - circle.center;
		Vector2D vVectorNormalized;
		ISVector2DNormalize(vVectorNormalized, vVector);
		Vector2D mVectorNormalized{ vVector.y, -(vVector.x) };
		ISVector2DNormalize(mVectorNormalized, mVectorNormalized);
		//ISVector2DNormalize(mVector, mVector);

		if (withinBothLines) {
			// if collide with P0 side first
			if (ISVector2DDotProduct((lineSeg.start - circle.center), (lineSeg.end - lineSeg.start)) > 0.f) {
				float m = ISVector2DDotProduct((lineSeg.start - circle.center), vVectorNormalized);
				if (m > 0) { // do check collision
					float dist0 = ISVector2DDotProduct((lineSeg.start - circle.center), mVectorNormalized);
					if (abs(dist0) > circle.radius) {
						return 0;
					}

					float s = sqrtf((circle.radius * circle.radius) - (dist0 * dist0));
					interTime = (m - s) / ISVector2DLength(vVector);

					if (interTime <= 1.f) {
						interPt = circle.center + (vVector * interTime);
						//B'e = ApplyReflection(N^, BiBe);
						ISVector2DNormalize(normalAtCollision, (interPt - lineSeg.start));
						return 1;
					}
				}
			}
			else { // (BsP1.P0P1 < 0), P1 side
				float m = ISVector2DDotProduct((lineSeg.end - circle.center), vVectorNormalized);
				if (m > 0) {
					float dist1 = ISVector2DDotProduct((lineSeg.end - circle.center), mVectorNormalized);
					if (abs(dist1) > circle.radius) {
						return 0;
					}

					float s = sqrtf((circle.radius * circle.radius) - (dist1 * dist1));
					interTime = (m - s) / ISVector2DLength(vVector);

					if (interTime <= 1) {
						interPt = circle.center + (vVector * interTime);
						//B'e = ApplyReflection(N^, BiBe);
						ISVector2DNormalize(normalAtCollision, (interPt - lineSeg.end));
						return 1;
					}
				}
			}
		}
		else {
			// check which line edge P0 or P1 is closer to V Vector
			bool P0Side = false;
			float dist0 = ISVector2DDotProduct((lineSeg.start - circle.center), mVectorNormalized);
			float dist1 = ISVector2DDotProduct((lineSeg.end - circle.center), mVectorNormalized);

			float dist0_absoluteValue = abs(dist0);
			float dist1_absoluteValue = abs(dist1);
			if ((dist0_absoluteValue > circle.radius) && (dist1_absoluteValue > circle.radius)) {
				return 0;
			}
			else if ((dist0_absoluteValue <= circle.radius) && (dist1_absoluteValue <= circle.radius)) {
				float m0 = ISVector2DDotProduct((lineSeg.start - circle.center), vVectorNormalized);
				float m1 = ISVector2DDotProduct((lineSeg.end - circle.center), vVectorNormalized);
				float m0_absoluteValue = abs(m0);
				float m1_absoluteValue = abs(m1);
				if (m0_absoluteValue < m1_absoluteValue) P0Side = true;
				else P0Side = false;
			}
			else if (dist0_absoluteValue <= circle.radius) {
				P0Side = true;
			}
			else { // if(dist1_absoluteValue <= R)
				P0Side = false;
			}

			if (P0Side) { // circle is closer to P0
				float m = ISVector2DDotProduct((lineSeg.start - circle.center), vVectorNormalized);
				if (m < 0) {
					return 0; // moving away
				}
				else {
					float s = sqrtf((circle.radius * circle.radius) - (dist0 * dist0));
					interTime = (m - s) / ISVector2DLength(vVector);
					if (interTime <= 1) {
						interPt = circle.center + (vVector * interTime);
						//B'e = ApplyReflection(N^, BiBe), Normal of reflection P0Bi normalized
						ISVector2DNormalize(normalAtCollision, (interPt - lineSeg.start));
						return 1;
					}
				}
			}
			else { // circle is closer to P1
				float m = ISVector2DDotProduct((lineSeg.end - circle.center), vVectorNormalized);
				if (m < 0) {
					return 0;
				}
				else {
					float s = sqrtf((circle.radius * circle.radius) - (dist1 * dist1));
					interTime = (m - s) / ISVector2DLength(vVector);
					if (interTime <= 1) {
						interPt = circle.center + (vVector * interTime);
						//B'e = ApplyReflection(N^, BiBe), P1Bi normalized
						ISVector2DNormalize(normalAtCollision, (interPt - lineSeg.end));
						return 1;
					}
				}
			}
		}

		return 0;// no collision
	}
	// reflecting the ball according to the hit direction with the line segment based on the normal of V
	void CollisionResponse_CircleLineSegment(
		const Vector2D& ptInter,		                                    		//Intersection position of the circle - input
		const Vector2D& normal,														//Normal vector of reflection on collision time - input
		Vector2D& ptEnd,															//Final position of the circle after reflection - output
		Vector2D& reflected)														//Normalized reflection vector direction - output
	{
		Vector2D iVector = ptEnd - ptInter;
		reflected = iVector - (2 * (ISVector2DDotProduct(iVector, normal)) * normal);

		ptEnd = ptInter + reflected;

		ISVector2DNormalize(reflected, reflected);
	}

}