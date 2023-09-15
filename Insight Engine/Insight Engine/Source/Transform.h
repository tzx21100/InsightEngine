#pragma once
#include "CoreEngine.h"
#include <glm/glm.hpp>

namespace IS {
	class Transform : public IComponent {
	public:
		// transformation
		glm::vec2 world_position{}; // world coordinates (if world is 1280 pixels, valid world positions: [-640, 640]
		glm::vec2 orientation{};   // x = angle displacement, y = angle speed
		glm::vec2 scaling{}; // x = width, y = height
		glm::mat3 mdl_to_ndl_xform = glm::imat3x3{};

		Transform();
		Transform(glm::vec2 world_position, glm::vec2 orientation, glm::vec2 scaling);

		Vector2D getWorldPosition();
		Vector2D getOrientation();
		Vector2D getScaling();

		void setWorldPosition(float x, float y);
		void setOrientation(float angle, float angle_speed);
		void setScaling(float width, float height);
	};
}