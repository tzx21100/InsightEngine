#include "Pch.h"


namespace IS
{
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
}