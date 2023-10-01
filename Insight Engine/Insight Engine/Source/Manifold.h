#pragma once
#include "Pch.h"

namespace IS 
{
	class Manifold {
	public:
        RigidBody& mBodyA;
        RigidBody& mBodyB;
        Vector2D mNormal;
        float mDepth;
        Vector2D mContact1;
        Vector2D mContact2;
        int mContactCount;

        //Manifold();

        Manifold(RigidBody & bodyA, RigidBody & bodyB,
            Vector2D normal, float depth,
            Vector2D contact1, Vector2D contact2, int contactCount) : mBodyA(bodyA), mBodyB(bodyB), 
            mNormal(normal), mDepth(depth), mContact1(contact1), mContact2(contact2), mContactCount(contactCount) {}
	};

}