/* Start Header **************************************************************/
/*!
\file	AIState.h
\author Matthew Ng, matthewdeen.ng@digipen.edu
\par Course: CSD2451
\date 02-11-2023
\brief
Component of AIFSM system

All content (C) 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/
/*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Engine/ECS/Entities.h"
#include "Engine/ECS/Component.h"

namespace IS {
	class AIState : public IComponent
	{
	public:
		static std::string GetType() {
			return "AIState";
		}

		/* Function pointers */
		void(*Enter)();
		void(*Update)();
		void(*Exit)();

		/* to store substates that the main state may have
		   e.g attack state can have two substates. Smash and swinging.
		NOTE: Substate implementation WIP */
		std::unordered_map<std::string, AIState*> substates{};
	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H  