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
	class ParticleEmitter : public IComponent
	{
	public:
		static std::string GetType() {
			return "ParticleEmitter";
		}

		Vec2D velocity;
		int shape;
		int color;
		float size;
		float lifespan;
		bool active;
		float alpha;

	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLEEMITTER_H  