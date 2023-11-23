#pragma once

#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H

#include <glm/glm.hpp>

namespace IS {
	class Light : public IComponent {
	public:
		static std::string GetType() {
			return "Light";
		}
		
		Light() = default;
		Light(Vector2D pos, Vector3D clr, float inten, float sz) : position{ pos }, color { clr }, intensity{ inten }, size{ sz } {};

		void init(Vector2D pos, Vector3D clr, float inten, float sz);
		void draw();

		Vector2D position{};
		Vector3D color{};
		float intensity{}; // 0 - 1.f
		float size{};
	};
}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H