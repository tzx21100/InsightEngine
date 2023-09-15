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

    class BodyTransform {
    public:
        float PositionX;
        float PositionY;
        float Sin;
        float Cos;

        BodyTransform();
        BodyTransform(Vector2D position, float angle);
        BodyTransform(float x, float y, float angle);
    };

    class RigidBody : public IComponent{
        public:
        Vector2D velocity;
        //Vector2D acceleration;
        Vector2D position;
        BodyType bodyType;
        float rotation;
        float rotationVelocity;
        Vector2D force;
        //float gravity;
        float density;
        float mass;
        float InvMass;
        //float friction;
        float restitution; // bounciness/elasticity, 0 - objects stick tgt with no bounce, 1 - objects bounces off with no loss of energy
        float area;       
        float radius; // for circle
        Vector2D dimension; // for box's width and height
        Shape bodyShape;
        std::vector<Vector2D> vertices; // vertices for polygon
        std::vector<Vector2D> transformedVertices;
        bool transformUpdateRequired; // default false, for calculating the transformed vertices when this is true

        // default and copy constructor
        RigidBody();
        RigidBody(Vector2D position, BodyType bodyType, float mass, float restitution,
            float radius, Vector2D dimension,  Shape bodyShape);

        // getting all the vertices from a box
        std::vector<Vector2D> CreateBoxVertices(Vector2D dimension);
        // getting the updated transformed vertices once object moving
        std::vector<Vector2D> GetTransformedVertices();
        // calculting the updated vertices by sin & cos
        Vector2D Transform(Vector2D v, BodyTransform transform);

        // update postion, rotation, force based on real world gravity
        void BodyUpdate(float dt, Vector2D gravity);
        // move the game object
        void Move(Vector2D val);
        // rotate the game object
        void Rotate(float val);
        // adding force
        void AddForce(Vector2D val);

        void CreateBoxBody(float width, float height, float mass, float restitution);
        void CreateCircleBody(float radius, float mass, float restitution);

        Json::Value Serialize() override {
            Json::Value prefab;
            prefab["RigidBodyVelocityX"] = velocity.x;
            prefab["RigidBodyVelocityY"] = velocity.y;
            //prefab["RigidBodyAccelerationX"] = acceleration.x;
            //prefab["RigidBodyAccelerationY"] = acceleration.y;
            prefab["RigidBodyPositionX"] =position.x;
            prefab["RigidBodyPositionY"] = position.y;
            prefab["RigidBodyBodyType"] = static_cast<int>(bodyType);
            //prefab["RigidBodyGravity"] = gravity;
            prefab["RigidBodyMass"] = mass;
            //prefab["RigidBodyFriction"] = friction;
            prefab["RigidBodyRestitution"] = restitution;
            return prefab;
        }
    };

    
}