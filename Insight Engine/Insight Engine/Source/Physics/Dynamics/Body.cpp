/*!
 * \file Body.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 28-09-2023
 * \brief
 * This header file contains the definitions of the `RigidBody` class, which is used to represent 
 * and manipulate rigid bodies within a physics simulation. The RigidBody class provides 
 * functionality to create body, apply forces, update physical states, and manage transformations 
 * in game world.
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
#include "Body.h"

namespace IS
{    
    // Default constructor for the RigidBody class
    RigidBody::RigidBody() {
        mPosition = Vector2D();
        mRotation = 0.f;
        mAngularVelocity = 0.f;
        mVelocity = Vector2D(); // (0,0)
        mBodyType = BodyType::Dynamic;
        mForce = Vector2D();
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = 99999999999999995.f;
        mInvMass = 1.f / mMass;
        mRestitution = 0.5f;
        mArea = 0.f;
        mState = BodyState::IDLE;
        mInertia = 1.f;
        mInvInertia = 1.f / mInertia;
        mStaticFriction = 0.6f;
        mDynamicFriction = 0.4f;
        mGravityScale = 1.f;
    }

    // Parameterized constructor to initialize rigid body properties.
	RigidBody::RigidBody(Vector2D position, BodyType body_type, float mass, float restitution) {
        mPosition = position;
        mRotation = 0.f;
        mAngularVelocity = 0.f;
        mVelocity = Vector2D(); // (0,0)
        mBodyType = body_type;
        mForce = Vector2D(); // (0,0)
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = mass;
        mInvMass = 1.f / mMass;
        mRestitution = restitution;
        mArea = 0.f;
        mState = BodyState::IDLE;
        mInertia = 1.f;
        mInvInertia = 1.f / mInertia;
        mStaticFriction = 0.6f;
        mDynamicFriction = 0.4f;
        mGravityScale = 1.f;

	}

    void RigidBody::CreateStaticBody(Vector2D const& position, float restitution) {
        mPosition = position;
        mRotation = 0.f;
        mAngularVelocity = 0.f;
        mVelocity = Vector2D();
        mBodyType = BodyType::Static;
        mForce = Vector2D();
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = 0.f;
        mInvMass = 0.f;
        mRestitution = restitution;
        mArea = 0.f;
        mInertia = 0.f;
        mInvInertia = 0.f;
        mStaticFriction = 0.6f;
        mDynamicFriction = 0.4f;
        mGravityScale = 1.f;
    }

    // updates the rigid body's transformation data to match the texture sprite Transform
    void RigidBody::BodyFollowTransform(Transform const& trans) {
        //UpdateBoxBody(trans);
        //mBodyTransform = trans;
        mPosition = trans.world_position;
        mRotation = trans.rotation;
        mAngularVelocity = trans.angle_speed;
        CreateBoxBody(trans.scaling.x / 1, trans.scaling.y / 1, mMass, mRestitution);

    }
#if 0

    // Update the position, rotation, and force of the rigid body based on real-world gravity
    void RigidBody::BodyUpdate(float dt, Vector2D const& gravity) {
        if (!(mBodyType == BodyType::Dynamic)) {
            return;
        }
        mVelocity += gravity * dt;
        mBodyTransform.world_position += mVelocity * dt;
        mBodyTransform.rotation += mAngularVelocity * dt;

        mTransformUpdateRequired = true;
    }
#endif
    // Move the game object by a specified vector
    void RigidBody::Move(Vector2D const& val) {
        mPosition += val;
        /*mTransformUpdateRequired = true;
        UpdateTransformedVertices();*/
    }

    // Rotate the game object by a specified angle
    void RigidBody::Rotate(float val) {
        mRotation += val;
    }

    // Add a force to the rigid body
    void RigidBody::AddForce(Vector2D const& val) {
        mForce += val;
    }
    
    // Set a force to the rigid body
    void RigidBody::SetForce(Vector2D const& val) {
        mForce = val;
    }

    // Add a velocity to the rigid body
    void RigidBody::AddVelocity(Vector2D const& val) {
        mVelocity += val;
    }
    
    // Add a velocity to the rigid body
    void RigidBody::SetVelocity(Vector2D const& val) {
        mVelocity = val;
    }

    // Create a box-shaped rigid body with specified parameters
    void RigidBody::CreateBoxBody(float width, float height, float mass, float restitution) {
        mArea = fabs(width * height);
        // set the range to be [0,1]
        mRestitution = std::max(0.0f, std::min(restitution, 1.0f));
        
        mMass = 0.f;
        mInertia = 0.f;

        if (mBodyType == BodyType::Dynamic) {
            mMass = mass;
            mInertia = (1.f / 12.f) * mass * (width * width + height * height);
        }

        // p = m * v
        mDensity = mMass * mArea; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
        mInvMass = mMass > 0.f ? (1.f / mMass) : 0.f;
        mInvInertia = mInertia > 0.f ? (1.f / mInertia) : 0.f;
    }

    // Create a circle-shaped rigid body with specified parameters
    void RigidBody::CreateCircleBody(float radius, float mass, float restitution) {
        mArea = fabs(radius * radius * PI);
        // set the range to be [0,1]
        mRestitution = std::max(0.0f, std::min(restitution, 1.0f));

        mMass = 0.f;
        mInertia = 0.f;

        if (mBodyType == BodyType::Dynamic) {
            mMass = mass;
            // static obj should have 0 mass and inertia
            mInertia = (1.f / 2.f) * mass * radius * radius;
        }

        // p = m * v
        mDensity = mMass * mArea; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
        mInvMass = mMass > 0.f ? (1.f / mMass) : 0.f;
        mInvInertia = mInertia > 0.f ? (1.f / mInertia) : 0.f;
    }
#if 0 
    // Update the parameters of a box-shaped rigid body based on its current Transform
    void RigidBody::UpdateBoxBody(Transform const& body_transform) {
        if (mBodyTransform.scaling != body_transform.scaling) { 
            // the box scaling will be updating only when it is different from transform scaling
            CreateBoxBody(body_transform.scaling.x, body_transform.scaling.y, mMass, mRestitution);
            CreateBoxVertices(body_transform.scaling.x, body_transform.scaling.y); // for vertices
        }
        mTransformUpdateRequired = true;
        UpdateTransformedVertices();
    }
#endif
}