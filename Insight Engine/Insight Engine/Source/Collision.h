#pragma once

#include "Pch.h"

namespace IS
{
	struct AABB
	{
		Vector2D	min;
		Vector2D	max;
	};

	// AABB Collision for static and dynamic, return true if they colliding
	bool CollisionIntersection_RectRect(const AABB& aabb1, const Vector2D& vel1,
		const AABB& aabb2, const Vector2D& vel2, const float& dt);
}