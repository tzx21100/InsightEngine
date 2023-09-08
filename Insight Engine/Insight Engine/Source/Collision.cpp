#include "Pch.h"
#include "CoreEngine.h"

namespace IS
{

	/*bool CollisionIntersection_RectRect(const Collider& collider1, const Collider& collider2, const float& dt) {

	}*/

	// AABB Collision for static and dynamic, return true if they colliding
	bool CollisionIntersection_RectRect(const AABB& aabb1, const Vector2D& vel1,
		const AABB& aabb2, const Vector2D& vel2, const float& dt)
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

	//line segment reference, p0 - start, p1 - end
	void BuildLineSegment(Line& lineSegment, const Vector2D& p0, const Vector2D& p1) {
		Vector2D normal = { (p1.y - p0.y), -(p1.x - p0.x) };
		ISVector2DNormalize(normal, normal);

		lineSegment.start = p0;
		lineSegment.end = p1;
		lineSegment.normal = normal;
	}

	//check whether the ball is going to collide with the line segment and check whether the ball is inside or outside of the line segement
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


	//moving the ball when it is going to collide with the line edge and reflecting the ball accordingly
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

		return 0;//no collision
	}
	//reflecting the ball according to the hit direction with the line segment based on the normal of V
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