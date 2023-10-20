/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Component.h"

namespace IS {
	class Pathfinder : public IComponent
	{
	public:
		virtual std::string GetType() const override {
			return "Pathfinder";
		}
		Vector2D mInitPos;
		Vector2D mEndPos;

		Pathfinder() {
			mInitPos = Vector2D(); //0,0
			mEndPos = Vector2D(); //0,0
		}
	};
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PATHFINDER_H  