 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */

namespace IS {
    class AIState {
    public:
        virtual void Enter() = 0;
        virtual void Update() = 0;
        virtual void Exit() = 0;
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_AISTATE_H  