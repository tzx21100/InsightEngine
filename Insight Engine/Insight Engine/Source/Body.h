/*!
 * \file Body.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
 * \brief
 * This header file contains the declaration of the `RigidBody` class, which represents
 * the rigid body components for entities in the game engine. It provides functions to
 * create rigid bodies in different shapes (box or circle), update body data based on
 * transformations, and perform physics-related operations.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
  ----------------------------------------------------------------------------- */
#pragma once
#include "Pch.h"
#include "Entities.h"
#include "Component.h"
#include "Transform.h"
#include "Grid.h"

namespace IS
{
    // Enumerations for body types and states

    /**
     * \enum BodyType
     * \brief Enumerates the types of rigid bodies.
     */
    enum class BodyType : int {
        Static,
        Dynamic,
        Kinematic
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
        virtual std::string GetType() const override {
            return "RigidBody";
        }
        // Member variables for rigid body properties

        Vector2D mVelocity;                         /**< Linear velocity of the rigid body. */
        float mAngularVelocity;                     /**< Angular velocity of the rigid body. */
        Transform mBodyTransform;                   /**< Transformation data (position, ratation, ratation speed, scaling). */
        BodyType mBodyType;                         /**< Type of the rigid body (Static, Dynamic, Kinematic). */
        Vector2D mForce;                            /**< Force applied to the rigid body. */
        Vector2D mAcceleration;                     /**< Acceleration of the rigid body. */
        float mDensity;                             /**< Density of the rigid body. */
        float mMass;                                /**< Mass of the rigid body. */
        float mInvMass;                             /**< Inverse mass of the rigid body. */
        float mRestitution;                         /**< Restitution coefficient (bounciness 0 to 1). */
        float mArea;                                /**< Area of the rigid body. */
        BodyState mState;                           /**< State of the rigid body. */
        BodyShape mBodyShape;                           /**< Shape of the rigid body (Box, Circle, etc.). */
        std::vector<Vector2D> mVertices;            /**< Vertices defining the polygon shape (for polygons). */
        std::vector<Vector2D> mTransformedVertices; /**< Transformed vertices of the polygon shape. */
        bool mTransformUpdateRequired;              /**< Flag to indicate if transformed vertices need to be updated. */
        int mId; // To compare then add/remove from the grid cell
        static int mNextId;
        GridState mGridState;
        bool mFirstTransform;

        // Constructors and member functions

        /**
         * \brief Default constructor for the RigidBody class.
         */
        RigidBody();

        /**
         * \brief Parameterized constructor to initialize rigid body properties.
         * \param my_position The initial position of the rigid body.
         * \param my_body_type The type of the rigid body (Static, Dynamic, Kinematic).
         * \param my_mass The mass of the rigid body.
         * \param my_restitution The restitution coefficient (bounciness).
         * \param my_width The width of the body (if applicable).
         * \param my_height The height of the body (if applicable).
         * \param my_body_shape The shape of the rigid body (Box, Circle, etc.).
         */
        RigidBody(Vector2D my_position, BodyType my_body_type, float my_mass, float my_restitution,
            float my_width, float my_height, BodyShape my_body_shape);

        /**
         * \brief Destructor for the RigidBody class.
         */
        //~RigidBody() { mNextId--; }

        // Member functions for rigid body operations

        /**
         * \brief Updates the rigid body's transformation data to match the texture sprite Transform.
         * \param my_trans The Transform representing the new position and orientation.
         */
        void BodyFollowTransform(Transform const& my_trans);

        /*!
         * \brief Calculate all the vertices for a 2D axis-aligned bounding box from origin (Box shape).
         * \param my_width The width of the box.
         * \param my_height The height of the box.
         * \return A vector containing the vertices of the box.
         */
        std::vector<Vector2D> CreateBoxVertices(float my_width, float my_height);

        /*!
         * \brief Calculate all the updated transformed vertices based on the transform center position.
         * \return A vector containing the transformed vertices of the rigid body.
         */
        std::vector<Vector2D> GetTransformedVertices();

        /*!
         * \brief Transform a vector using a given transformation matrix.
         * \param my_v The vector to transform.
         * \param my_transform The transformation matrix.
         * \return The transformed vector.
         */
        Vector2D TransformRigidBody(Vector2D my_v, Transform my_transform);

        /*!
         * \brief Update the position, rotation, and force of the rigid body based on real-world gravity.
         * \param my_dt The time step.
         * \param my_gravity The gravity vector.
         */
        void BodyUpdate(float my_dt, Vector2D const& my_gravity); // not in use

        /*!
         * \brief Move the game object by a specified vector.
         * \param my_val The vector by which to move the game object.
         */
        void Move(Vector2D const& my_val); // not in use

        /*!
         * \brief Rotate the game object by a specified angle.
         * \param my_val The angle (in radians) by which to rotate the game object.
         */
        void Rotate(float my_val);

        /*!
         * \brief Add a force to the rigid body.
         * \param my_val The force vector to add.
         */
        void AddForce(Vector2D const& my_val); // not in use

        /*!
         * \brief Add a velocity to the rigid body.
         * \param my_val The velocity vector to add.
         */
        void AddVelocity(Vector2D const& my_val);

        /*!
        * \brief Create a box-shaped rigid body with specified parameters.
        * \param my_width The width of the box.
        * \param my_height The height of the box.
        * \param my_mass The mass of the rigid body.
        * \param my_restitution The restitution (bounciness/elasticity) of the rigid body.
        */
        void CreateBoxBody(float my_width, float my_height, float my_mass, float my_restitution);

        /*!
         * \brief Create a circle-shaped rigid body with specified parameters.
         * \param my_radius The radius of the circle.
         * \param my_mass The mass of the rigid body.
         * \param my_restitution The restitution (bounciness/elasticity) of the rigid body.
         */
        void CreateCircleBody(float my_radius, float my_mass, float my_restitution);

        /*!
         * \brief Update the parameters of a box-shaped rigid body based on its current Transform.
         * \param my_body_transform The Transform of the rigid body.
         */
        void UpdateBoxBody(Transform const& my_body_transform);

        Box GetAABB();

        // Define the equality operator
        bool operator==(const RigidBody& other) const;


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

            // Serializing bodyShape (casted to int)
            prefab["RigidBodyShapeType"] = static_cast<int>(mBodyShape);

            // Serializing vertices and transformedVertices
            Json::Value verticesArray(Json::arrayValue);
            for (const auto& vertex : mVertices) {
                Json::Value v;
                v["x"] = vertex.x;
                v["y"] = vertex.y;
                verticesArray.append(v);
            }
            prefab["RigidBodyVertices"] = verticesArray;

            Json::Value transformedVerticesArray(Json::arrayValue);
            for (const auto& vertex : mTransformedVertices) {
                Json::Value v;
                v["x"] = vertex.x;
                v["y"] = vertex.y;
                transformedVerticesArray.append(v);
            }
            prefab["RigidBodyTransformedVertices"] = transformedVerticesArray;

            // Serializing transformUpdateRequired
            prefab["RigidBodyTransformUpdateRequired"] = mTransformUpdateRequired;

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

            // Deserializing bodyShape (assuming it's an enum or similar that can be cast from int)
            mBodyShape = static_cast<BodyShape>(data["RigidBodyShapeType"].asInt());

            // Deserializing vertices and transformedVertices
            const Json::Value verticesArray = data["RigidBodyVertices"];
            mVertices.clear();
            for (const auto& v : verticesArray) {
                Vector2D vertex;
                vertex.x = v["x"].asFloat();
                vertex.y = v["y"].asFloat();
                mVertices.push_back(vertex);
            }

            const Json::Value transformedVerticesArray = data["RigidBodyTransformedVertices"];
            mTransformedVertices.clear();
            for (const auto& v : transformedVerticesArray) {
                Vector2D vertex;
                vertex.x = v["x"].asFloat();
                vertex.y = v["y"].asFloat();
                mTransformedVertices.push_back(vertex);
            }

            // Deserializing transformUpdateRequired
            mTransformUpdateRequired = data["RigidBodyTransformUpdateRequired"].asBool();
        }


    };

    
}