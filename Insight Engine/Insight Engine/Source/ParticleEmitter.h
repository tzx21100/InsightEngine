/*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Entities.h"
#include "Component.h"
#include "Particle.h"
#include <vector>

namespace IS {


	class ParticleEmitter : public IComponent
	{
	public:
		static std::string GetType() {
			return "ParticleEmitter";
		}

		// Emitter properties
		int mParticlesAmount =1; //particles per second
		float mDirectionMin =0.f; //direction of emission
		float mDirectionMax =0.f; // direciont max

		Particle mParticle;
		// Constructor to initialize default values
		/*ParticleEmitter() : mVelocity(Vec2D(0, 0)), mSize(0.f), mLifespan(0.f), mColor(Color(1.f,1.f,1.f,1.f)), mAlpha(0.f), mEmissionRate(0.f), mTimeSinceLastEmission(0.f) {}*/

	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H  