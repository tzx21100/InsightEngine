#pragma once
#include "CoreEngine.h"
#include <glm/glm.hpp>

namespace IS {
	class Transform : public IComponent {
	public:
		// transformation
		glm::vec2 world_position{}; // world coordinates (if world is 1280 pixels, valid world positions: [-640, 640]
		float rotation{}; // z-axis rotation in degrees
		glm::vec2 scaling{}; // x = width, y = height
		glm::mat3 mdl_to_ndc_xform = glm::imat3x3{};

		Transform();
		Transform(glm::vec2 world_position, float rotation, glm::vec2 scaling);

		Vector2D getWorldPosition();
		float getRotation();
		Vector2D getScaling();

		void setWorldPosition(float x, float y);
		void setRotation(float angle);
		void setScaling(float width, float height);


		Json::Value Serialize() override {
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

		void Deserialize(Json::Value data) override {
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
	};




}