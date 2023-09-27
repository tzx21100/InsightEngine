#include "Pch.h"


namespace IS {
	Transform::Transform() {
		this->world_position = glm::vec2();
		this->rotation = 0.f;
		this->scaling = glm::vec2();
	}

	Transform::Transform(glm::vec2 world_position, float rotation, glm::vec2 scaling) {
		this->world_position = world_position;
		this->rotation = rotation;
		this->scaling = scaling;
	}

	Vector2D Transform::getWorldPosition() {
		return Vector2D(world_position.x, world_position.y);
	}

	float Transform::getRotation() {
		return rotation;
	}

	Vector2D Transform::getScaling() {
		return Vector2D(scaling.x, scaling.y);
	}

	void Transform::setWorldPosition(float x, float y) {
		world_position.x = x;
		world_position.y = y;
	}

	void Transform::setRotation(float angle, float angleSpeed) {
		rotation = angle;
		angle_speed = angleSpeed;
	}

	void Transform::setScaling(float width, float height) {
		scaling.x = width;
		scaling.y = height;
	}

	Json::Value Transform::Serialize() {
		Json::Value transformData;

		// Serializing world_position
		transformData["TransformWorldPositionX"] = world_position.x;
		transformData["TransformWorldPositionY"] = world_position.y;

		// Serializing rotation
		transformData["TransformRotation"] = rotation;

		// Serializing scaling
		transformData["TransformScalingX"] = scaling.x;
		transformData["TransformScalingY"] = scaling.y;

		// Note: Not serializing mdl_to_ndc_xform since matrix serialization can be complex and depends on specifics

		return transformData;
	}

	void Transform::Deserialize(Json::Value data) {
		// Deserializing world_position
		world_position.x = data["TransformWorldPositionX"].asFloat();
		world_position.y = data["TransformWorldPositionY"].asFloat();

		// Deserializing rotation
		rotation = data["TransformRotation"].asFloat();

		// Deserializing scaling
		scaling.x = data["TransformScalingX"].asFloat();
		scaling.y = data["TransformScalingY"].asFloat();

		// Note: Not deserializing mdl_to_ndc_xform since matrix deserialization can be complex and depends on specifics
	}
}
