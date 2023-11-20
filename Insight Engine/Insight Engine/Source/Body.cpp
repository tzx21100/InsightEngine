/*!
 * \file Body.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
 * \brief
 * This header file contains the definitions of the `RigidBody` class, which is used to represent 
 * and manipulate rigid bodies within a physics simulation. The RigidBody class provides 
 * functionality to create body, apply forces, update physical states, and manage transformations 
 * in game world.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
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
        //mBodyTransform = Transform(Vector2D(), 0.f, Vector2D());
        mBodyType = BodyType::Dynamic;
        mForce = Vector2D();
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = 1.f;
        mInvMass = 1.f / mMass;
        mRestitution = 0.5f;
        mArea = 0.f;
        mState = BodyState::IDLE;
        //mBodyShape = BodyShape::Box;
        //mTransformUpdateRequired = false;
        //mGridState = GridState::Uninitialized;
        mInertia = 1.f;
        mInvInertia = 1.f / mInertia;
        mStaticFriction = 0.6f;
        mDynamicFriction = 0.4f;
        mGravityScale = 1.f;
        //CreateBoxBody(0.f, 0.f, mMass, mRestitution);
        //if (mBodyShape == BodyShape::Box) {
        //    CreateBoxBody(mBodyTransform.scaling.x, mBodyTransform.scaling.y, mMass, mRestitution);
        //    CreateBoxVertices(mBodyTransform.scaling.x, mBodyTransform.scaling.y); // for vertices
        //    // making the transform mVertices same size as the mVertices
        //    mTransformedVertices = mVertices;
        //}
        //else if (mBodyShape == BodyShape::Circle) {
        //    CreateCircleBody(mBodyTransform.scaling.x/2.f, mMass, mRestitution);
        //}
        //else {
        //    // make sure the vector array is empty
        //    mVertices.clear();
        //    mTransformedVertices.clear();
        //}

        //mTransformUpdateRequired = true;
    }

    // Parameterized constructor to initialize rigid body properties.
	RigidBody::RigidBody(Vector2D position, BodyType body_type, float mass, float restitution) {
        mPosition = position;
        mRotation = 0.f;
        mAngularVelocity = 0.f;
        mVelocity = Vector2D(); // (0,0)
        //mBodyTransform.world_position = position;
        //mBodyTransform.rotation = 0.f;
        //mBodyTransform.scaling = Vector2D(width, height);
        mBodyType = body_type;
        mForce = Vector2D(); // (0,0)
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = mass;
        mInvMass = 1.f / mMass;
        mRestitution = restitution;
        mArea = 0.f;
        mState = BodyState::IDLE;
        //mBodyShape = body_shape;
        //mTransformUpdateRequired = false;
        //mGridState = GridState::Uninitialized;
        mInertia = 1.f;
        mInvInertia = 1.f / mInertia;
        mStaticFriction = 0.6f;
        mDynamicFriction = 0.4f;
        mGravityScale = 1.f;
        //if (mBodyShape == BodyShape::Box) {
        //    CreateBoxBody(mBodyTransform.scaling.x, mBodyTransform.scaling.y, mMass, mRestitution);
        //    CreateBoxVertices(mBodyTransform.scaling.x, mBodyTransform.scaling.y); // for vertices
        //    // making the transform mVertices same size as the mVertices
        //    mTransformedVertices = mVertices;
        //}
        //else if (mBodyShape == BodyShape::Circle) {
        //    CreateCircleBody(mBodyTransform.scaling.x/2.f, mMass, mRestitution);
        //}
        //else {
        //    // make sure the vector array is empty
        //    mVertices.clear();
        //    mTransformedVertices.clear();
        //}

        //mTransformUpdateRequired = true;

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
    // Calculate all the vertices for a 2D axis-aligned bounding box from origin (Box shape) based on origin (0,0)
    void RigidBody::CreateBoxVertices(float width, float height) {
        // the vertices are calculated based on origin (not transform yet)
        float left = - width / 2.f;
        float right = left + width;
        float bottom = - height / 2.f;
        float top = bottom + height;

        std::vector<Vector2D> box_vertices;
        box_vertices.emplace_back(Vector2D(left,top)); // 0 top left
        box_vertices.emplace_back(Vector2D(right, top)); // 1 top right
        box_vertices.emplace_back(Vector2D(right, bottom)); // 2 bottom right
        box_vertices.emplace_back(Vector2D(left, bottom)); // 3 bottom left

        mVertices = box_vertices;
    }

    // Calculate all the updated transformed vertices based on the transform center position
    std::vector<Vector2D> RigidBody::GetTransformedVertices()
    {
        // if true means the game object had move from previous place
        if (mTransformUpdateRequired)
        {
            //BodyTransform transform(this->position, this->rotation);
            Transform currentTransform = Transform(mBodyTransform.world_position, mBodyTransform.rotation, mBodyTransform.scaling);

            // loop for each vertice
            for (int i = 0; i < mVertices.size(); i++)
            {
                Vector2D v = mVertices[i];
                // apply transform function to calculate correct mVertices
                mTransformedVertices[i] = RigidBody::TransformRigidBody(v, currentTransform);
            }
        }/*
        else {
            this->mTransformedVertices = this->mVertices;
        }*/

        mTransformUpdateRequired = false;
        return mTransformedVertices;
    }

    void RigidBody::UpdateTransformedVertices() {
        // if true means the game object had move from previous place
        if (mTransformUpdateRequired)
        {
            //BodyTransform transform(this->position, this->rotation);
            Transform currentTransform = Transform(mBodyTransform.world_position, mBodyTransform.rotation, mBodyTransform.scaling);

            // loop for each vertice
            for (int i = 0; i < mVertices.size(); i++)
            {
                Vector2D v = mVertices[i];
                // apply transform function to calculate correct mVertices
                mTransformedVertices[i] = RigidBody::TransformRigidBody(v, currentTransform);
            }
        }/*
        else {
            this->mTransformedVertices = this->mVertices;
        }*/

        mTransformUpdateRequired = false;
    }

    // Transform a vector using a given transformation matrix
    Vector2D RigidBody::TransformRigidBody(Vector2D v, Transform transform) {
        Vector2D ret;
        float angle = glm::radians(transform.rotation);
        /*if (angle < 0.f) { angle += 360.f; }
        angle *= (PI / 180.f);*/
        ret.x = cosf(angle) * v.x - sinf(angle) * v.y + transform.world_position.x;
        ret.y = sinf(angle) * v.x + cosf(angle) * v.y + transform.world_position.y;
        return ret;
    }

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

    // Get the axis-aligned bounding box (AABB) of the rigid body.
    Box RigidBody::GetAABB() {
            float minX = std::numeric_limits<float>::max();
            float minY = std::numeric_limits<float>::max();
            float maxX = -std::numeric_limits<float>::max();
            float maxY = -std::numeric_limits<float>::max();

            if (mBodyShape == BodyShape::Box)
            {
                // loop through the vertices to get a bigger(if necessary) box for grid cell calculation
                for (int i = 0; i < mTransformedVertices.size(); i++)
                {
                    Vector2D v = mTransformedVertices[i];

                    if (v.x < minX) { minX = v.x; }
                    if (v.x > maxX) { maxX = v.x; }
                    if (v.y < minY) { minY = v.y; }
                    if (v.y > maxY) { maxY = v.y; }
                }
            }
            else if (mBodyShape == BodyShape::Circle)
            {
                float radius = mBodyTransform.scaling.x / 2;

                minX = mBodyTransform.world_position.x - radius;
                minY = mBodyTransform.world_position.y - radius;
                maxX = mBodyTransform.world_position.x + radius;
                maxY = mBodyTransform.world_position.y + radius;
            }

            Box aabb = Box(minX, minY, maxX, maxY);

        return aabb;
    }
#endif
}