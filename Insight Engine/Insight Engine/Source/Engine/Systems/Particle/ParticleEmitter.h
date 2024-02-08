/*!
 * \file ParticleEmitter.h
 * \author  Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date  25-11-2023
 * \brief
 * This is a simple header that defines the Particle Emitter Component.
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/ECS/Entities.h"
#include "Engine/ECS/Component.h"
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