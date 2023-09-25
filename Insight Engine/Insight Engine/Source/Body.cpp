#include "Pch.h"
#include "CoreEngine.h"
#include "Graphics.h"
#include "Shader.h"

namespace IS
{
    // default and copy constructor for BodyTransform class
    // for calculating the transformed game objects' position & angle
    /*BodyTransform::BodyTransform() {
        this->PositionX = 0.f;
        this->PositionY = 0.f;
        this->Sin = 0.f;
        this->Cos = 0.f;

    }
    BodyTransform::BodyTransform(Vector2D position, float angle) {
        this->PositionX = position.x;
        this->PositionY = position.y;
        this->Sin = std::sin(angle);
        this->Cos = std::cos(angle);
    }
    BodyTransform::BodyTransform(float x, float y, float angle) {
        this->PositionX = x;
        this->PositionY = y;
        this->Sin = std::sin(angle);
        this->Cos = std::cos(angle);
    }*/
    
    // define RigidBody class
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
        mBodyShape = Shape::Box;
        mTransformUpdateRequired = false;

        if (mBodyShape == Shape::Box) {
            CreateBoxBody(mBodyTransform.scaling.x, mBodyTransform.scaling.y, mMass, mRestitution);
            mVertices = CreateBoxVertices(mBodyTransform.scaling.x, mBodyTransform.scaling.y);
            // making the transform mVertices same size as the mVertices, not neccessary
            mTransformedVertices = mVertices;
        }
        else if (mBodyShape == Shape::Circle) {
            CreateCircleBody(mBodyTransform.scaling.x/2, mMass, mRestitution);
        }
        else {
            // make sure the vector array is empty
            mVertices.clear();
            mTransformedVertices.clear();
        }

        mTransformUpdateRequired = true;
    }

	RigidBody::RigidBody(glm::vec2 my_position, BodyType my_body_type, float my_mass, float my_restitution,
        float my_width, float my_height, Shape my_body_shape) {
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

        if (mBodyShape == Shape::Box) {
            CreateBoxBody(mBodyTransform.scaling.x, mBodyTransform.scaling.y, mMass, mRestitution);
            mVertices = CreateBoxVertices(mBodyTransform.scaling.x, mBodyTransform.scaling.y);
            
            // making the transform mVertices same size as the mVertices, not neccessary
            mTransformedVertices = mVertices;
        }
        else if (mBodyShape == Shape::Circle) {
            CreateCircleBody(mBodyTransform.scaling.x/2, mMass, mRestitution);
        }
        else {
            // make sure the vector array is empty
            mVertices.clear();
            mTransformedVertices.clear();
        }

        mTransformUpdateRequired = true;
	}

    void RigidBody::BodyFollowTransform(Transform const& my_trans) {
        mBodyTransform = my_trans;
        UpdateBoxBody(my_trans);
    }

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

    Vector2D RigidBody::TransformRigidBody(Vector2D my_v, Transform my_transform) {
        Vector2D ret;
        float angle = fmod(my_transform.rotation, 360.f);
        if (angle < 0.f) { angle += 360.f; }
        angle *= (PI / 180.f);
        ret.x = cosf(angle) * my_v.x - sinf(angle) * my_v.y + my_transform.world_position.x;
        ret.y = sinf(angle) * my_v.x + cosf(angle) * my_v.y + my_transform.world_position.y;
        return ret;
    }

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

    void RigidBody::Move(Vector2D const& my_val) {
        mBodyTransform.world_position.x += my_val.x;
        mBodyTransform.world_position.y += my_val.y;
        mTransformUpdateRequired = true;
        mTransformedVertices = GetTransformedVertices();
    }

    void RigidBody::Rotate(float my_val) {
        mBodyTransform.rotation += my_val;
        mTransformUpdateRequired = true;
        mTransformedVertices = GetTransformedVertices();
    }

    void RigidBody::AddForce(Vector2D const& my_val) {
        mForce = my_val;
    }

    void RigidBody::AddVelocity(Vector2D const& my_val) {
        mVelocity += my_val;
    }

    void RigidBody::CreateBoxBody(float my_width, float my_height, float my_mass, float my_restitution) {
        mArea = fabs(my_width * my_height);
        // set the range to be [0,1]
        mRestitution = std::max(0.0f, std::min(my_restitution, 1.0f));
        // p = m * v
        mDensity = my_mass * mArea; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
    }

    void RigidBody::CreateCircleBody(float my_radius, float my_mass, float my_restitution) {
        mArea = fabs(my_radius * my_radius * PI);
        // set the range to be [0,1]
        mRestitution = std::max(0.0f, std::min(my_restitution, 1.0f));
        // p = m * v
        mDensity = my_mass * mArea; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
    }

    void RigidBody::UpdateBoxBody(Transform const& my_body_transform) {
        CreateBoxBody(my_body_transform.scaling.x, my_body_transform.scaling.y, mMass, mRestitution);
        mVertices = CreateBoxVertices(my_body_transform.scaling.x, my_body_transform.scaling.y);
        mTransformUpdateRequired = true;
        mTransformedVertices = GetTransformedVertices();
    }
}