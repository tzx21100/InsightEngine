#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_

#include"Message.h"
#include <string>
/* create a virtual system class for all systems used in the game
this class should be the parent class for all systems used E.g. Graphics */

namespace Zx_Framework {

    class ParentSystem {
    public:
        virtual void sendMessage(Message* message){};
        virtual void updateSystem(float time) = 0;
        virtual std::string getName() = 0;
        virtual void initializeSystem() {};
        virtual ~ParentSystem(){}

    };
}


#endif //GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_