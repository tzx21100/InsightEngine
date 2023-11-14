#include "Pch.h"

namespace IS {

    std::string Particle::GetName() {
        return "Particle";
    }

    void Particle::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void Particle::Update([[maybe_unused]] float deltaTime) {

    }

    void Particle::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }
}