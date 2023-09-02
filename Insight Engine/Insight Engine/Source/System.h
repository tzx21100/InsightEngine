#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_

#include"Message.h"
#include "EventManager.h"
#include <string>


/* create a virtual system class for all systems used in the game
this class should be the parent class for all systems used E.g. Graphics
all are virtual so they can be overwritten by the other systems when initing*/

namespace IS {

    //all systems should be able to listen to messages as well
    class ParentSystem :public MessageListener {
    public:
        //sendMessage will use the InsightEngine to broadcast all the messages to everyone
        void sendMessage(Message* message) { EventManager::Instance().Broadcast(*message); };
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };
        virtual void Update(float time) = 0;
        virtual std::string getName() = 0;
        virtual void Initialize() {};
        virtual ~ParentSystem(){}

    };
}


#endif //GAM200_INSIGHT_ENGINE_SOURCE_SYSTEM_H_