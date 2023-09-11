#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EVENTMANAGER_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_EVENTMANAGER_H_

#include<vector>
#include <unordered_map>
#include "Message.h"

/*this class is to listen to messages and allow for specific systems to subscribe to specific messages
It stores who the subscribers are for the specific message and when the engine broadcasts the message,
all subscribed Systems will recieved the message.
*/
namespace IS {
    class EventManager {
    public:
        //singleton as well
        static EventManager& Instance() {
            static EventManager instance;
            return instance;
        }
        void Subscribe(MessageType type, MessageListener* listener);
        void Broadcast(const Message& message);

    private:
        //i create an unorderedmap with MessageType as key and it should store an array of a pointer to message listeners (systems)
        std::unordered_map<MessageType, std::vector<MessageListener*>> handlers;
        //singleton
        EventManager();
        ~EventManager();
    };
}

#define BROADCAST_MESSAGE(message) ::IS::EventManager::Instance().Broadcast(message)

#endif
