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

    struct RigidBody {
        Vector2D velocity;
        Vector2D acceleration;
        Vector2D position;
        BodyType bodyType;
        float gravity;
        float mass;
        float friction;
        float restitution; // bounciness/elasticity, 0 - objects stick tgt with no bounce, 1 - objects bounces off with no loss of energy
    };
}