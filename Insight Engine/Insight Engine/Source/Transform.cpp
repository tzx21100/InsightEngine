#include "Pch.h"


namespace IS {
	Vector2D Transform::getWorldPosition() {
		return Vector2D(world_position.x, world_position.y);
	}

	Vector2D Transform::getOrientation() {
		return Vector2D(orientation.x, world_position.y);
	}

	Vector2D Transform::getScaling() {
		return Vector2D(scaling.x, scaling.y);
	}

	void Transform::setWorldPosition(float x, float y) {
		world_position.x = x;
		world_position.y = y;
	}

	void Transform::setOrientation(float angle, float angle_speed) {
		orientation.x = angle;
		orientation.y = angle_speed;
	}

	void Transform::setScaling(float width, float height) {
		scaling.x = width;
		scaling.y = height;
	}
}