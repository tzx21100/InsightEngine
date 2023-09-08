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
        std::string getName() { return "Physics"; }
        void Initialize() override;
        void setGravity(float g) { Gravity = g; }
        float getGravity() { return Gravity; }
        ~Physics() {}
	private:

        // gravity of the world
        float Gravity;

        // max velocity for the game body
        float MaxVelocity;
	};

    void rigidBodyCallUpdate(RigidBody body, float dt);

    void collisionCallUpdate(Collider collider, float dt, RigidBody rigidBody, auto const& entity, std::set<Entity> mEntities);

    extern Physics* PHYSICS;
}