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

    class RigidBody : public IComponent{
        public:
        Vector2D velocity;
        float angular_velocity;
        Transform bodyTransform; // position, orientation, scaling
        //Vector2D position;
        BodyType bodyType;
        //float rotation;
        //float rotationVelocity;
        Vector2D force;
        Vector2D acceleration;
        float density;
        float mass;
        float InvMass;
        float restitution; // bounciness/elasticity, 0 - objects stick tgt with no bounce, 1 - objects bounces off with no loss of energy
        float area;       
        //float radius; // for circle
        //Vector2D dimension; // for box's width and height
        Shape bodyShape;
        std::vector<Vector2D> vertices; // vertices for polygon (based on origin)
        std::vector<Vector2D> transformedVertices; // vertices for polygon after transfromation
        bool transformUpdateRequired; // default false, for calculating the transformed vertices when this is true

        // default and copy constructor
        RigidBody();
        RigidBody(glm::vec2 position, BodyType bodyType, float mass, float restitution,
            float width, float height,  Shape bodyShape);
        void bodyFollowTransform(Transform const& trans);
        // calculate all the vertices for a box
        std::vector<Vector2D> CreateBoxVertices(float const& width, float const& height);
        // calculate all the updated transformed vertices based center position
        std::vector<Vector2D> GetTransformedVertices();
        // calculting the updated vertices by sin & cos
        //Vector2D Transform(Vector2D v, BodyTransform transform);
        Vector2D TransformRigidBody(Vector2D v, Transform transform);

        // update postion, rotation, force based on real world gravity
        void BodyUpdate(float const& dt, Vector2D const& gravity);
        // move the game object
        void Move(Vector2D const& val);
        // rotate the game object
        void Rotate(float const& val);
        // adding force
        void AddForce(Vector2D const& val);
        // adding velocity
        void AddVelocity(Vector2D const& val);
        // create box rigidbody
        void CreateBoxBody(float const& width, float const& height, float const& mass, float const& restitution);
        // create circle rigidbody
        void CreateCircleBody(float const& radius, float const& mass, float const& restitution);

        void updateBoxBody(Transform const& bodyTransform, float const& mass, float const& restitution);

        Json::Value Serialize() override {
            Json::Value prefab;

            // Serializing velocity
            prefab["RigidBodyVelocityX"] = velocity.x;
            prefab["RigidBodyVelocityY"] = velocity.y;

            // Serializing angular_velocity
            prefab["RigidBodyAngularVelocity"] = angular_velocity;

            // Serializing bodyType
            prefab["RigidBodyBodyType"] = static_cast<int>(bodyType);

            // Serializing force
            prefab["RigidBodyForceX"] = force.x;
            prefab["RigidBodyForceY"] = force.y;

            // Serializing acceleration
            prefab["RigidBodyAccelerationX"] = acceleration.x;
            prefab["RigidBodyAccelerationY"] = acceleration.y;

            // Serializing other scalar properties
            prefab["RigidBodyDensity"] = density;
            prefab["RigidBodyMass"] = mass;
            prefab["RigidBodyInvMass"] = InvMass;
            prefab["RigidBodyRestitution"] = restitution;
            prefab["RigidBodyArea"] = area;

            // Serializing bodyShape (casted to int)
            prefab["RigidBodyShapeType"] = static_cast<int>(bodyShape);

            // Serializing vertices and transformedVertices
            Json::Value verticesArray(Json::arrayValue);
            for (const auto& vertex : vertices) {
                Json::Value v;
                v["x"] = vertex.x;
                v["y"] = vertex.y;
                verticesArray.append(v);
            }
            prefab["RigidBodyVertices"] = verticesArray;

            Json::Value transformedVerticesArray(Json::arrayValue);
            for (const auto& vertex : transformedVertices) {
                Json::Value v;
                v["x"] = vertex.x;
                v["y"] = vertex.y;
                transformedVerticesArray.append(v);
            }
            prefab["RigidBodyTransformedVertices"] = transformedVerticesArray;

            // Serializing transformUpdateRequired
            prefab["RigidBodyTransformUpdateRequired"] = transformUpdateRequired;

            return prefab;
        }
        void Deserialize(Json::Value data) override {
            // Deserializing velocity
            velocity.x = data["RigidBodyVelocityX"].asFloat();
            velocity.y = data["RigidBodyVelocityY"].asFloat();

            // Deserializing angular_velocity
            angular_velocity = data["RigidBodyAngularVelocity"].asFloat();

            // Deserializing bodyType
            bodyType = static_cast<BodyType>(data["RigidBodyBodyType"].asInt());

            // Deserializing force
            force.x = data["RigidBodyForceX"].asFloat();
            force.y = data["RigidBodyForceY"].asFloat();

            // Deserializing acceleration
            acceleration.x = data["RigidBodyAccelerationX"].asFloat();
            acceleration.y = data["RigidBodyAccelerationY"].asFloat();

            // Deserializing other scalar properties
            density = data["RigidBodyDensity"].asFloat();
            mass = data["RigidBodyMass"].asFloat();
            InvMass = data["RigidBodyInvMass"].asFloat();
            restitution = data["RigidBodyRestitution"].asFloat();
            area = data["RigidBodyArea"].asFloat();

            // Deserializing bodyShape (assuming it's an enum or similar that can be cast from int)
            bodyShape = static_cast<Shape>(data["RigidBodyShapeType"].asInt());

            // Deserializing vertices and transformedVertices
            const Json::Value verticesArray = data["RigidBodyVertices"];
            vertices.clear();
            for (const auto& v : verticesArray) {
                Vector2D vertex;
                vertex.x = v["x"].asFloat();
                vertex.y = v["y"].asFloat();
                vertices.push_back(vertex);
            }

            const Json::Value transformedVerticesArray = data["RigidBodyTransformedVertices"];
            transformedVertices.clear();
            for (const auto& v : transformedVerticesArray) {
                Vector2D vertex;
                vertex.x = v["x"].asFloat();
                vertex.y = v["y"].asFloat();
                transformedVertices.push_back(vertex);
            }

            // Deserializing transformUpdateRequired
            transformUpdateRequired = data["RigidBodyTransformUpdateRequired"].asBool();
        }


    };

    
}