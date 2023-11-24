#pragma once

#ifndef GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H
#define GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H

#include "Transform.h"
#include <glm/glm.hpp>

namespace IS {

	class Light : public IComponent {
	public:
		static std::string GetType() {
			return "Light";
		}
		
		Light() = default;
		Light(Vector2D offset, Vector3D const& hue, float intensity, float size, bool render)
			: mOffset{ offset }, mHue{ hue }, mIntensity{ intensity }, mSize{ size }, mRender{ render } {};

		void draw(float attachedEntID);
		void FollowTransform(Vector2D position);

		Json::Value Serialize() override;
		void Deserialize(Json::Value data) override;

		Vector2D mPosition{};
		Vector2D mOffset{};
		Vector3D mHue{ 1.f, 1.f, 1.f };
		float mIntensity{ .5f }; // 0 - 1.f
		float mSize{ 200.f };
		bool mRender{ true };
	};

}// end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_GRAPHICS_SYSTEM_LIGHT_H