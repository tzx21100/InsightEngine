/* Start Header **************************************************************/
/*!
\file	Pathfinder.h
\author Matthew Ng, matthewdeen.ng@digipen.edu
\par Course: CSD2401
\date 02-11-2023
\brief
Component of Pathfinding system

All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/
/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Engine/ECS/Component.h"

namespace IS {
	class Pathfinder : public IComponent
	{
	public:
		static std::string GetType() {
			return "Pathfinder";
		}
		Vector2D mInitPos;
		Vector2D mEndPos;

		Pathfinder() {
			//Initial position of AIs
			mInitPos = Vector2D(); //0,0
			mEndPos = Vector2D(); //0,0
		}
	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDER_H  