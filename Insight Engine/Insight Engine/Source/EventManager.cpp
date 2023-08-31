#include "Pch.h"
#include "EventManager.h"
namespace IS {

    //adding the key and system to the handler
    void EventManager::Subscribe(MessageType type, MessageListener* listener) {
        handlers[type].push_back(listener);
    }

    //first find if there are any handlers with the message type then broadcast
    void EventManager::Broadcast(const Message& message) {
        //it is a unorderedmap so i find the vector of messagelistener* here
        //auto is used because its convenient and I don't see restrictions against it
        auto it = handlers.find(message.GetType());
        if (it != handlers.end()) {
            //i iterate over the array of messagelisteners*
            for (const auto& listener : it->second) {
                listener->HandleMessage(message);
            }
        }
    }
}