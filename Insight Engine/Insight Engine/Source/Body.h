#pragma once
#include "Pch.h"
#include "Entities.h"
#include "Component.h"

namespace IS
{
    enum class BodyType : int {
        Static,
        Dynamic,
        Kinematic
    };

    class RigidBody : public IComponent{
        public:
        Vector2D velocity;
        Vector2D acceleration;
        Vector2D position;
        BodyType bodyType;
        float gravity;
        float mass;
        float friction;
        float restitution; // bounciness/elasticity, 0 - objects stick tgt with no bounce, 1 - objects bounces off with no loss of energy

        Json::Value Serialize() override {
            Json::Value prefab;
            prefab["RigidBodyVelocityX"] = velocity.x;
            prefab["RigidBodyVelocityY"] = velocity.y;
            prefab["RigidBodyAccelerationX"] = acceleration.x;
            prefab["RigidBodyAccelerationY"] = acceleration.y;
            prefab["RigidBodyPositionX"] =position.x;
            prefab["RigidBodyPositionY"] = position.y;
            prefab["RigidBodyBodyType"] = static_cast<int>(bodyType);
            prefab["RigidBodyGravity"] = gravity;
            prefab["RigidBodyMass"] = mass;
            prefab["RigidBodyFriction"] = friction;
            prefab["RigidBodyRestitution"] = restitution;
            return prefab;
        }
    };
}