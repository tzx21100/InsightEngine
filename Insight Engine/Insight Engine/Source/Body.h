#pragma once
#include "Pch.h"
#include "Entities.h"
#include "Component.h"
#include "Transform.h"
#include <glm/glm.hpp>

namespace IS
{
    enum class BodyType : int {
        Static,
        Dynamic,
        Kinematic
    };

   /* class BodyTransform {
    public:
        float PositionX;
        float PositionY;
        float Sin;
        float Cos;

        BodyTransform();
        BodyTransform(Vector2D position, float angle);
        BodyTransform(float x, float y, float angle);
    };*/

    enum class BodyState : short
    {
        IDLE,
        WALKING,
        JUMP,
        GROUNDED
    };

    class RigidBody : public IComponent{
        public:
        Vector2D mVelocity;
        float mAngularVelocity;
        Transform mBodyTransform; // position, orientation, scaling
        BodyType mBodyType;
        Vector2D mForce;
        Vector2D mAcceleration;
        float mDensity;
        float mMass;
        float mInvMass;
        float mRestitution; // bounciness/elasticity, 0 - objects stick tgt with no bounce, 1 - objects bounces off with no loss of energy
        float mArea;    
        BodyState mState;
        Shape mBodyShape;
        std::vector<Vector2D> mVertices; // vertices for polygon (based on origin)
        std::vector<Vector2D> mTransformedVertices; // vertices for polygon after transfromation
        bool mTransformUpdateRequired; // default false, for calculating the transformed vertices when this is true

        // default and copy constructor
        RigidBody();

        RigidBody(glm::vec2 my_position, BodyType my_body_type, float my_mass, float my_restitution,
            float my_width, float my_height,  Shape my_body_shape);


        void BodyFollowTransform(Transform const& my_trans);

        // calculate all the vertices for a box
        std::vector<Vector2D> CreateBoxVertices(float my_width, float my_height);

        // calculate all the updated transformed vertices based center position
        std::vector<Vector2D> GetTransformedVertices();


        Vector2D TransformRigidBody(Vector2D my_v, Transform my_transform);

        // update postion, rotation, force based on real world gravity
        void BodyUpdate(float my_dt, Vector2D const& my_gravity);

        // move the game object
        void Move(Vector2D const& my_val);

        // rotate the game object
        void Rotate(float my_val);

        // adding force
        void AddForce(Vector2D const& my_val);

        // adding velocity
        void AddVelocity(Vector2D const& my_val);

        // create box rigidbody
        void CreateBoxBody(float my_width, float my_height, float my_mass, float my_restitution);

        // create circle rigidbody
        void CreateCircleBody(float my_radius, float my_mass, float my_restitution);


        void UpdateBoxBody(Transform const& my_body_transform);

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
            mBodyShape = static_cast<Shape>(data["RigidBodyShapeType"].asInt());

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