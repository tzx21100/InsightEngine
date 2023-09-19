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
        // create box rigidbody
        void CreateBoxBody(float const& width, float const& height, float const& mass, float const& restitution);
        // create circle rigidbody
        void CreateCircleBody(float const& radius, float const& mass, float const& restitution);

        void updateBoxBody(Transform const& bodyTransform, float const& mass, float const& restitution);

        Json::Value Serialize() override {
            Json::Value prefab;
            prefab["RigidBodyVelocityX"] = velocity.x;
            prefab["RigidBodyVelocityY"] = velocity.y;
            //prefab["RigidBodyAccelerationX"] = acceleration.x;
            //prefab["RigidBodyAccelerationY"] = acceleration.y;
            //prefab["RigidBodyPositionX"] =position.x;
            //prefab["RigidBodyPositionY"] = position.y;
            prefab["RigidBodyBodyType"] = static_cast<int>(bodyType);
            //prefab["RigidBodyGravity"] = gravity;
            prefab["RigidBodyMass"] = mass;
            //prefab["RigidBodyFriction"] = friction;
            prefab["RigidBodyRestitution"] = restitution;
            return prefab;
        }
    };

    
}