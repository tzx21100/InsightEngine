#pragma once

#include "Pch.h"

namespace IS 
{
	class Physics : public ParentSystem {
	public:
        Physics();
        //sendMessage will use the InsightEngine to broadcast all the messages to everyone
        void sendMessage(Message* message) { EventManager::Instance().Broadcast(*message); };
        void Subscribe(MessageType type) { EventManager::Instance().Subscribe(type, this); };
        void Update(float dt) override;
        std::string GetName() { return "Physics"; }
        void Initialize() override;
        ~Physics() {}
	private:

        // gravity of the world
        Vector2D Gravity;

        // max velocity for the game body
        float MaxVelocity;
	};

    extern Physics* PHYSICS;
}