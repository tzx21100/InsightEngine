#include "Pch.h"


namespace IS {
	Transform::Transform() {
		this->world_position = glm::vec2();
		this->orientation = glm::vec2();
		this->scaling = glm::vec2();
	}

	Transform::Transform(glm::vec2 world_position, glm::vec2 orientation, glm::vec2 scaling) {
		this->world_position = world_position;
		this->orientation = orientation;
		this->scaling = scaling;
	}

	Vector2D Transform::getWorldPosition() {
		return Vector2D(world_position.x, world_position.y);
	}

	Vector2D Transform::getOrientation() {
		return Vector2D(orientation.x, orientation.y);
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