/*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Entities.h"
#include "Component.h"

namespace IS {
	enum class Color : int
	{
		BLACK,
		RED,
		BLUE,
		GREEN,
		NUM_COLORS //for randomising colors
	};
	
	Color RandomColor() {
		return static_cast<Color>(rand() % static_cast<int>(Color::NUM_COLORS));
	}

	class ParticleEmitter : public IComponent
	{
	public:
		static std::string GetType() {
			return "ParticleEmitter";
		}

		// Particle Attributes
		Vec2D mVelocity;
		float mSize;
		float mLifespan;
		Color mColor;
		float mAlpha;

		// Emission properties
		float mEmissionRate;         // Rate at which particles are emitted
		float mTimeSinceLastEmission;

		// Constructor to initialize default values
		ParticleEmitter(): mVelocity(Vec2D(0,0)), mSize(0.f), mLifespan(0.f), mColor(Color::BLACK), mAlpha(0.f), mEmissionRate(0.f), mTimeSinceLastEmission(0.f) {}

		//// Method to set velocity
		//void SetVelocity(const Vec2D& velocity) {
		//	mVelocity = velocity;
		//}

		//// Method to set size
		//void SetSize(const float& size) {
		//	mSize = size;
		//}

		//// Method to set lifespan
		//void SetLifespan(const float& lifespan) {
		//	mLifespan = lifespan;
		//}

		//// Method to set color
		//void SetColor(const Color& color) {
		//	mColor = color;
		//}

		//// Method to set alpha
		//void SetAlpha(const float& alpha) {
		//	mAlpha = alpha;
		//}

	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H  