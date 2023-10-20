 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Entities.h"
#include "Component.h"

namespace IS {
	class AIState : public IComponent
	{
	public:
		virtual std::string GetType() const override {
			return "AIState";
		}
		void(*Enter)();
		void(*Update)();
		void(*Exit)();
	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H  