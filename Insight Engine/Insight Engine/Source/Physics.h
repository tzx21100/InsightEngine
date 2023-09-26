#pragma once

#include "Pch.h"
#include "Sprite.h"

namespace IS 
{
	class Physics : public ParentSystem {
	public:
        Physics();
        void Update(float dt) override;
        std::string GetName() override { return "Physics"; }
        void Initialize() override;
        void HandleMessage(const Message&) override {}
        //void setGravity(float g) { Gravity = g; }
        //Vector2D getGravity() { return Gravity; }
        static bool isDebugDraw;
        ~Physics() {}

        static void DrawOutLine(RigidBody & body, Sprite const& sprite);
	private:

        // gravity of the world
        Vector2D mGravity;
        bool mExertingGravity;
        // max velocity for the game body
        float mMaxVelocity;
        float mMinVelocity;
	};

    // not used
    void RigidBodyCallUpdate(RigidBody body, Vector2D gravity, float dt);

    //void collisionCallUpdate(Collider collider, float dt, RigidBody rigidBody, auto const& entity, std::set<Entity> mEntities);
    void CollisionCallUpdate(RigidBody rigidBody, float dt,  auto const& entity, std::set<Entity> mEntities);

    void CollisionDetect(std::set<Entity> const& mEntities);

    void ResolveCollision(RigidBody& bodyA, RigidBody& bodyB, Vector2D const& normal, float depth);

    

    extern Physics* PHYSICS;
}