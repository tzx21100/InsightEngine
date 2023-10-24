

#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MANIFOLD_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MANIFOLD_H

#include "Pch.h"

namespace IS 
{
    // Manifold refers to a mathematical construct used to represent 
    // contact information between two objects in a simulation

	class Manifold {
	public:
        RigidBody* mBodyA;
        RigidBody* mBodyB;
        Vector2D mNormal;
        float mDepth;
        Vector2D mContact1;
        Vector2D mContact2;
        int mContactCount;

        Manifold();

       /* Manifold(RigidBody* bodyA, RigidBody* bodyB,
            Vector2D const& normal, float const& depth,
            Vector2D const& contact1, Vector2D const& contact2, int const& contactCount) : mBodyA(bodyA), mBodyB(bodyB), 
            mNormal(normal), mDepth(depth), mContact1(contact1), mContact2(contact2), mContactCount(contactCount) {}*/

        Manifold(RigidBody* bodyA, RigidBody* bodyB,
            Vector2D const& normal, float const& depth,
            Vector2D const& contact1, Vector2D const& contact2, int const& contactCount);

        void FindContactPoints(RigidBody & bodyA, RigidBody & bodyB);

        void FindPolygonsContactPoints(std::vector<Vector2D> const& verticesA, std::vector<Vector2D> const& verticesB, Vector2D & contact1, Vector2D & contact2, int & contactCount);

        bool NearlyEqual(float const& a, float const& b);

        bool NearlyEqual(Vector2D const& a, Vector2D const& b);
	};

}

#endif