#pragma once

#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H

#include <glm/glm.hpp>

namespace IS {
	class Light {
	public:
		Light(glm::vec3 clr, float inten, float sz) : color{ clr }, intensity{ inten }, size{ sz } {};
		void draw();

		glm::vec3 color{};
		float intensity{}; // 0 - 1.f
		float size{};
	};
}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H