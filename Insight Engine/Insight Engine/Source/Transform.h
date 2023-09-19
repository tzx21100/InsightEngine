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
	};
}