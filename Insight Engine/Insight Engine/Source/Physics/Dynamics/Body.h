/*!
 * \file Body.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 26-11-2023
 * \brief
 * This header file contains the declaration of the `RigidBody` class, which represents
 * the rigid body components for entities in the game engine. It provides functions to
 * update body data based on transformations, and perform physics-related operations.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_BODY_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_BODY_H

 /*                                                                   includes
  ----------------------------------------------------------------------------- */
#include "Engine/ECS/Entities.h"
#include "Engine/ECS/Component.h"
#include "Graphics/System/Transform.h"
#include "Physics/Collision/Grid.h"
#include "Physics/Collision/Collision.h"

namespace IS
{
    // Enumerations for body types and states

    /**
     * \enum BodyType
     * \brief Enumerates the types of rigid bodies.
     */
    enum class BodyType : short {
        Static,
        Dynamic,
        Kinematic,
        Ghost,
        WallClimb,
        Spikes,
        Ignore,
        Stone,
        Grass
    };

    /**
     * \enum BodyState
     * \brief Enumerates the states of the rigid body
     */
    enum class BodyState : short
    {
        IDLE,
        WALKING,
        JUMP,
        GROUNDED
    };

    /**
     * \class RigidBody
     * \brief Represents a rigid body component for game entities.
     */
    class RigidBody : public IComponent{
    public:
        static std::string GetType() {
            return "RigidBody";
        }
        // Member variables for rigid body properties

        Vector2D mPosition;
        float mRotation;
        float mAngularVelocity;                     /**< Angular velocity of the rigid body. */

        Vector2D mVelocity;                         /**< Linear velocity of the rigid body. */
        //Transform mBodyTransform;                   /**< Transformation data (position, ratation, angle speed, scaling). */
        BodyType mBodyType;                         /**< Type of the rigid body (Static, Dynamic, Kinematic). */
        Vector2D mForce;                            /**< Force applied to the rigid body. */
        Vector2D mAcceleration;                     /**< Acceleration of the rigid body. */
        float mDensity;                             /**< Density of the rigid body. */
        float mMass;                                /**< Mass of the rigid body. */
        float mInvMass;                             /**< Inverse mass of the rigid body. */
        float mRestitution;                         /**< Restitution coefficient (bounciness 0 to 1). */
        float mArea;                                /**< Area of the rigid body. */
        BodyState mState;                           /**< State of the rigid body. */
        float mInertia;                             /**< Inertia of the body. */
        float mInvInertia;                          /**< Inverse Inertia of the body. */
        float mStaticFriction;                      /**< Static friction of the body. */
        float mDynamicFriction;                     /**< Dynamic friction of the body. */
        float mGravityScale;                        /**< Gravity Scale of the body. */

        /*Dynamic friction is the friction which occurs once the static friction is overcome.
        The static friction conceptually is what is keeping our objects in place while grounded 
        and what brings moving objects to rest as the linear velocity is absorbed in contact by 
        the friction.In order for an object to be moved from rest, the static friction force must 
        be overcome with some kinetic force.once the static friction has been over come the body 
        begins to apply its dynamic friction which attempts to consume the excess kinetic energy 
        in an "angular" manner which induces spin on the body.*/

        // Constructors and member functions

        /**
         * \brief Default constructor for the RigidBody class.
         */
        RigidBody();

        /**
         * \brief Parameterized constructor to initialize rigid body properties.
         * \param position The initial position of the rigid body.
         * \param body_type The type of the rigid body (Static, Dynamic, Kinematic).
         * \param mass The mass of the rigid body.
         * \param restitution The restitution coefficient (bounciness).
         * \param width The width of the body (if applicable).
         * \param height The height of the body (if applicable).
         * \param body_shape The shape of the rigid body (Box, Circle, etc.).
         */
        RigidBody(Vector2D position, BodyType body_type, float mass, float restitution);

        /**
         * \brief Initializes a static body with a specific position and restitution value.
         * \param position The initial position of the static body.
         * \param restitution The restitution coefficient for the static body.
         */
        void CreateStaticBody(Vector2D const& position, float restitution);

        // Member functions for rigid body operations

        /**
         * \brief Updates the rigid body's transformation data to match the texture sprite Transform.
         * \param trans The Transform representing the new position and orientation.
         */
        void BodyFollowTransform(Transform const& trans);

        /*!
         * \brief Update the position, rotation, and force of the rigid body based on real-world gravity.
         * \param dt The time step.
         * \param gravity The gravity vector.
         */
        void BodyUpdate(float dt, Vector2D const& gravity); // not in use

        /*!
         * \brief Move the game object by a specified vector.
         * \param val The vector by which to move the game object.
         */
        void Move(Vector2D const& val);

        /*!
         * \brief Rotate the game object by a specified angle.
         * \param val The angle (in radians) by which to rotate the game object.
         */
        void Rotate(float val); // not in use

        /*!
         * \brief Add a force to the rigid body.
         * \param val The force vector to add.
         */
        void AddForce(Vector2D const& val);

        /*!
         * \brief Set the force to the rigid body.
         * \param val The force vector to add.
         */
        void SetForce(Vector2D const& val);

        /*!
         * \brief Add a velocity to the rigid body.
         * \param val The velocity vector to add.
         */
        void AddVelocity(Vector2D const& val);

        /*!
         * \brief Set the velocity to the rigid body.
         * \param val The velocity vector to add.
         */
        void SetVelocity(Vector2D const& val);

        /*!
        * \brief Create a box-shaped rigid body with specified parameters.
        * \param width The width of the box.
        * \param height The height of the box.
        * \param mass The mass of the rigid body.
        * \param restitution The restitution (bounciness/elasticity) of the rigid body.
        */
        void CreateBoxBody(float width, float height, float mass, float restitution);

        /*!
         * \brief Create a circle-shaped rigid body with specified parameters.
         * \param radius The radius of the circle.
         * \param mass The mass of the rigid body.
         * \param restitution The restitution (bounciness/elasticity) of the rigid body.
         */
        void CreateCircleBody(float radius, float mass, float restitution);

        /*!
         * \brief Update the parameters of a box-shaped rigid body based on its current Transform.
         * \param body_transform The Transform of the rigid body.
         */
        void UpdateBoxBody(Transform const& body_transform); // not in use


        /*!
         * \brief Serialize the RigidBody component to a JSON format.
         * \return A JSON object representing the serialized RigidBody.
         */
        Json::Value Serialize() override {
            Json::Value prefab;

            // Serializing velocity
            prefab["RigidBodyVelocityX"] = mVelocity.x;
            prefab["RigidBodyVelocityY"] = mVelocity.y;

            // Serializing angular_velocity
            prefab["RigidBodyAngularVelocity"] = mAngularVelocity;

            // Serializing bodyType
            prefab["RigidBodyBodyType"] = static_cast<int>(mBodyType);

            // Serializing force
            prefab["RigidBodyForceX"] = mForce.x;
            prefab["RigidBodyForceY"] = mForce.y;

            // Serializing acceleration
            prefab["RigidBodyAccelerationX"] = mAcceleration.x;
            prefab["RigidBodyAccelerationY"] = mAcceleration.y;

            // Serializing other scalar properties
            prefab["RigidBodyDensity"] = mDensity;
            prefab["RigidBodyMass"] = mMass;
            prefab["RigidBodyInvMass"] = mInvMass;
            prefab["RigidBodyRestitution"] = mRestitution;
            prefab["RigidBodyArea"] = mArea;

            // Serializing gravity scale
            prefab["RigidbodyGravityScale"] = mGravityScale;

            //prefab["mInertia"] = mInertia;
            //prefab["mInvInertia"] = mInvInertia;
            prefab["mStaticFriction"] = mStaticFriction;
            prefab["mDynamicFriction"] = mDynamicFriction;


            return prefab;
        }

        /*!
         * \brief Deserialize a JSON object to populate the RigidBody component.
         * \param data A JSON object containing serialized RigidBody data.
         */
        void Deserialize(Json::Value data) override {
            // Deserializing velocity
            mVelocity.x = data["RigidBodyVelocityX"].asFloat();
            mVelocity.y = data["RigidBodyVelocityY"].asFloat();

            // Deserializing angular_velocity
            mAngularVelocity = data["RigidBodyAngularVelocity"].asFloat();

            // Deserializing bodyType
            mBodyType = static_cast<BodyType>(data["RigidBodyBodyType"].asInt());

            // Deserializing force
            mForce.x = data["RigidBodyForceX"].asFloat();
            mForce.y = data["RigidBodyForceY"].asFloat();

            // Deserializing acceleration
            mAcceleration.x = data["RigidBodyAccelerationX"].asFloat();
            mAcceleration.y = data["RigidBodyAccelerationY"].asFloat();

            // Deserializing other scalar properties
            mDensity = data["RigidBodyDensity"].asFloat();
            mMass = data["RigidBodyMass"].asFloat();
            mInvMass = data["RigidBodyInvMass"].asFloat();
            mRestitution = data["RigidBodyRestitution"].asFloat();
            mArea = data["RigidBodyArea"].asFloat();

            // Deserializing gravity scale
            mGravityScale = data["RigidbodyGravityScale"].asFloat();

            mInertia= data["mInertia"].asFloat();
            mInvInertia = data["mInvInertia"].asFloat();
            mStaticFriction = data["mStaticFriction"].asFloat();
            mDynamicFriction = data["mDynamicFriction"].asFloat();

        }

    };

    
}

#endif