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
#include "CoreEngine.h"
#include "Graphics.h"
#include "Shader.h"

namespace IS
{    
    // Default constructor for the RigidBody class
    RigidBody::RigidBody() {
        mVelocity = Vector2D(); // (0,0)
        mAngularVelocity = 0.f;
        mBodyTransform.world_position = glm::vec2();
        mBodyTransform.rotation = 0.f;
        mBodyTransform.scaling = glm::vec2();
        mBodyType = BodyType::Dynamic;
        mForce = Vector2D();
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = (mBodyType == BodyType::Dynamic) ? 2.f : 999999.f;
        mInvMass = 1.f / mMass;
        mRestitution = 0.5f;
        mArea = 0.f;
        mState = BodyState::IDLE;
        mBodyShape = BodyShape::Box;
        mTransformUpdateRequired = false;

        if (mBodyShape == BodyShape::Box) {
            CreateBoxBody(mBodyTransform.scaling.x, mBodyTransform.scaling.y, mMass, mRestitution);
            mVertices = CreateBoxVertices(mBodyTransform.scaling.x, mBodyTransform.scaling.y);
            // making the transform mVertices same size as the mVertices
            mTransformedVertices = mVertices;
        }
        else if (mBodyShape == BodyShape::Circle) {
            CreateCircleBody(mBodyTransform.scaling.x/2.f, mMass, mRestitution);
        }
        else {
            // make sure the vector array is empty
            mVertices.clear();
            mTransformedVertices.clear();
        }

        mTransformUpdateRequired = true;
    }

    // Parameterized constructor to initialize rigid body properties.
	RigidBody::RigidBody(glm::vec2 my_position, BodyType my_body_type, float my_mass, float my_restitution,
        float my_width, float my_height, BodyShape my_body_shape) {
        mVelocity = Vector2D(); // (0,0)
        mAngularVelocity = 0.f;
        mBodyTransform.world_position = my_position;
        mBodyTransform.rotation = 0.f;
        mBodyTransform.scaling = glm::vec2(my_width, my_height);
        mBodyType = my_body_type;
        mForce = Vector2D(); // (0,0)
        mAcceleration = Vector2D();
        mDensity = 0.f;
        mMass = my_mass;
        mInvMass = 1.f / mMass;
        mRestitution = my_restitution;
        mArea = 0.f;
        mState = BodyState::IDLE;
        mBodyShape = my_body_shape;
        mTransformUpdateRequired = false;

        if (mBodyShape == BodyShape::Box) {
            CreateBoxBody(mBodyTransform.scaling.x, mBodyTransform.scaling.y, mMass, mRestitution);
            mVertices = CreateBoxVertices(mBodyTransform.scaling.x, mBodyTransform.scaling.y);
            
            // making the transform mVertices same size as the mVertices
            mTransformedVertices = mVertices;
        }
        else if (mBodyShape == BodyShape::Circle) {
            CreateCircleBody(mBodyTransform.scaling.x/2.f, mMass, mRestitution);
        }
        else {
            // make sure the vector array is empty
            mVertices.clear();
            mTransformedVertices.clear();
        }

        mTransformUpdateRequired = true;
	}

    // Updates the rigid body's transformation data to match the texture sprite Transform
    void RigidBody::BodyFollowTransform(Transform const& my_trans) {
        mBodyTransform = my_trans;
        UpdateBoxBody(my_trans);
    }

    // Calculate all the vertices for a 2D axis-aligned bounding box from origin (Box shape) based on origin (0,0)
    std::vector<Vector2D> RigidBody::CreateBoxVertices(float my_width, float my_height) {
        // the vertices are calculated based on origin (not transform yet)
        float left = - my_width / 2.f;
        float right = left + my_width;
        float bottom = - my_height / 2.f;
        float top = bottom + my_height;

        std::vector<Vector2D> box_vertices;
        box_vertices.emplace_back(Vector2D(left,top)); // 0 top left
        box_vertices.emplace_back(Vector2D(right, top)); // 1 top right
        box_vertices.emplace_back(Vector2D(right, bottom)); // 2 bottom right
        box_vertices.emplace_back(Vector2D(left, bottom)); // 3 bottom left

        return box_vertices;
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

    // Transform a vector using a given transformation matrix
    Vector2D RigidBody::TransformRigidBody(Vector2D my_v, Transform my_transform) {
        Vector2D ret;
        float angle = fmod(my_transform.rotation, 360.f);
        if (angle < 0.f) { angle += 360.f; }
        angle *= (PI / 180.f);
        ret.x = cosf(angle) * my_v.x - sinf(angle) * my_v.y + my_transform.world_position.x;
        ret.y = sinf(angle) * my_v.x + cosf(angle) * my_v.y + my_transform.world_position.y;
        return ret;
    }

    // Update the position, rotation, and force of the rigid body based on real-world gravity
    void RigidBody::BodyUpdate(float my_dt, Vector2D const& my_gravity) {
        if (!(mBodyType == BodyType::Dynamic)) {
            return;
        }
        mVelocity += my_gravity * my_dt;
        mBodyTransform.world_position.x += mVelocity.x * my_dt;
        mBodyTransform.world_position.y += mVelocity.y * my_dt;
        mBodyTransform.rotation += mAngularVelocity * my_dt;

        mTransformUpdateRequired = true;
    }

    // Move the game object by a specified vector
    void RigidBody::Move(Vector2D const& my_val) {
        mBodyTransform.world_position.x += my_val.x;
        mBodyTransform.world_position.y += my_val.y;
        mTransformUpdateRequired = true;
        mTransformedVertices = GetTransformedVertices();
    }

    // Rotate the game object by a specified angle
    void RigidBody::Rotate(float my_val) {
        mBodyTransform.rotation += my_val;
        mTransformUpdateRequired = true;
        mTransformedVertices = GetTransformedVertices();
    }

    // Add a force to the rigid body
    void RigidBody::AddForce(Vector2D const& my_val) {
        mForce = my_val;
    }

    // Add a velocity to the rigid body
    void RigidBody::AddVelocity(Vector2D const& my_val) {
        mVelocity += my_val;
    }

    // Create a box-shaped rigid body with specified parameters
    void RigidBody::CreateBoxBody(float my_width, float my_height, float my_mass, float my_restitution) {
        mArea = fabs(my_width * my_height);
        // set the range to be [0,1]
        mRestitution = std::max(0.0f, std::min(my_restitution, 1.0f));
        // p = m * v
        mDensity = my_mass * mArea; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
    }

    // Create a circle-shaped rigid body with specified parameters
    void RigidBody::CreateCircleBody(float my_radius, float my_mass, float my_restitution) {
        mArea = fabs(my_radius * my_radius * PI);
        // set the range to be [0,1]
        mRestitution = std::max(0.0f, std::min(my_restitution, 1.0f));
        // p = m * v
        mDensity = my_mass * mArea; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
    }

    // Update the parameters of a box-shaped rigid body based on its current Transform
    void RigidBody::UpdateBoxBody(Transform const& my_body_transform) {
        CreateBoxBody(my_body_transform.scaling.x, my_body_transform.scaling.y, mMass, mRestitution);
        mVertices = CreateBoxVertices(my_body_transform.scaling.x, my_body_transform.scaling.y);
        mTransformUpdateRequired = true;
        mTransformedVertices = GetTransformedVertices();
    }

    Box RigidBody::GetAABB() {
            float minX = std::numeric_limits<float>::max();
            float minY = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::min();
            float maxY = std::numeric_limits<float>::min();

            if (mBodyShape == BodyShape::Box)
            {
                //FlatVector[] vertices = this.GetTransformedVertices();

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
}