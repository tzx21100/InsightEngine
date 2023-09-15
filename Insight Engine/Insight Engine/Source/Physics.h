#pragma once

#include "Pch.h"

namespace IS 
{
	class Physics : public ParentSystem {
	public:
        Physics();
        void Update(float dt) override;
        std::string getName() override { return "Physics"; }
        void Initialize() override;
        void HandleMessage(const Message& message) override {}
        //void setGravity(float g) { Gravity = g; }
        Vector2D getGravity() { return Gravity; }
        ~Physics() {}
	private:

        // gravity of the world
        Vector2D Gravity;

        // max velocity for the game body
        float MaxVelocity;
	};

    void rigidBodyCallUpdate(RigidBody body, Vector2D gravity, float dt);

    //void collisionCallUpdate(Collider collider, float dt, RigidBody rigidBody, auto const& entity, std::set<Entity> mEntities);
    void collisionCallUpdate(RigidBody rigidBody, float dt,  auto const& entity, std::set<Entity> mEntities);

    void ResolveCollision(RigidBody bodyA, RigidBody bodyB, Vector2D normal, float depth);

    extern Physics* PHYSICS;
}