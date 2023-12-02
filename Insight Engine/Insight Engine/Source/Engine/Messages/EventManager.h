/*!
 * \file EventManager.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This source file declares the implementation of a EventManager class
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_EVENTMANAGER_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_EVENTMANAGER_H_

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/Core/Core.h"
#include <vector>
#include <unordered_map>
#include "Message.h"

/*this class is to listen to messages and allow for specific systems to subscribe to specific messages
It stores who the subscribers are for the specific message and when the engine broadcasts the message,
all subscribed Systems will recieved the message.
*/
namespace IS {
    /**
     * \class EventManager
     * \brief Manages the subscription and broadcasting of messages to various listeners in the system.
     *
     * The EventManager follows the Singleton design pattern, ensuring that there is only one instance of the manager in the system.
     * It provides methods for systems to subscribe to specific message types and for broadcasting messages to all subscribed systems.
     */
    class IS_API EventManager {
    public:
        /**
         * \brief Returns the singleton instance of the EventManager.
         *
         * This method follows the "Meyer's Singleton" pattern, ensuring thread safety and lazy initialization.
         *
         * \return A reference to the single instance of EventManager.
         */
        static EventManager& Instance() {
            static EventManager instance;
            return instance;
        }

        /**
         * \brief Allows systems to subscribe to specific message types.
         *
         * When a system wishes to listen for a specific message type, it can use this method to register itself.
         * When that message type is broadcast, the system will be notified.
         *
         * \param type The type of message to subscribe to.
         * \param listener A pointer to the system or entity that wants to listen to the message.
         */
        void Subscribe(MessageType type, MessageListener* listener);

        /**
         * \brief Broadcasts a message to all subscribed listeners of that message type.
         *
         * When this method is called with a message, all systems that have subscribed to that particular message type will be notified.
         *
         * \param message The message to be broadcast.
         */
        void Broadcast(const Message& message);

    private:
        /**
         * \brief Map that stores message listeners indexed by message type.
         *
         * For each message type, there can be multiple systems/entities that wish to listen.
         * Thus, for each message type, we maintain a vector of listeners.
         */
        std::unordered_map<MessageType, std::vector<MessageListener*>> handlers;

        /**
         * \brief Private constructor to ensure that no external entities can create an instance.
         *
         * The Singleton pattern is enforced by making the constructor private.
         */
        EventManager();

        /**
         * \brief Destructor to handle any cleanup if necessary.
         */
        ~EventManager();
    };
}

#define BROADCAST_MESSAGE(message) ::IS::EventManager::Instance().Broadcast(message)

#endif
