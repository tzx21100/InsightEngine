/*!
 * \file EventManager.cpp
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This source file defines the implementation of a EventManager class
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "EventManager.h"

namespace IS {
    EventManager::EventManager() {
        //empty
    }

    EventManager::~EventManager() {
        handlers.clear();
    }
    //adding the key and system to the handler
    void EventManager::Subscribe(MessageType type, MessageListener* listener) {
        handlers[type].push_back(listener);
    }

    //first find if there are any handlers with the message type then broadcast
    void EventManager::Broadcast(const Message& message) {
        //it is a unorderedmap so i find the vector of messagelistener* here
        auto it = handlers.find(message.GetType());
        if (it != handlers.end()) {
            //i iterate over the array of messagelisteners*
            for (const auto& listener : it->second) {
                listener->HandleMessage(message);
            }
        }

    }
}