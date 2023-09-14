#include "Pch.h"
#include "CoreEngine.h"
#include "Graphics.h"

namespace IS
{
    // default and copy constructor for BodyTransform class
    // for calculating the transformed game objects' position & angle
    BodyTransform::BodyTransform() {
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
    }
    
    // define RigidBody class
    RigidBody::RigidBody() {
        this->velocity = Vector2D(); // (0,0)
        this->position = Vector2D(); // (0,0)
        this->bodyType = BodyType::Static;
        this->rotation = 0.f;
        this->rotationVelocity = 0.f;
        this->force = Vector2D();
        this->density = 0.f;
        this->mass = 0.f;
        this->restitution = 0.f;
        this->area = 0.f;
        this->radius = 0.f;
        this->dimension = Vector2D();
        this->bodyShape = Shape::Box;
        // if body dynamic, calculate the inverse mass, otherwise set as 0.f
        this->InvMass = (this->bodyType == BodyType::Dynamic) ? (1.f / this->mass) : 0.f;

        if (this->bodyShape == Shape::Box) {
            CreateBoxBody(this->dimension.x, this->dimension.y, this->mass, this->restitution);
            this->vertices = CreateBoxVertices(this->dimension);
            // making the transform vertices same size as the vertices, not neccessary
            //this->transformedVertices(this->vertices.size());
        }
        else if (this->bodyShape == Shape::Circle) {
            CreateCircleBody(this->radius, this->mass, this->restitution);
        }
        else {
            // make sure the vector array is empty
            this->vertices.clear();
            this->transformedVertices.clear();
        }

        this->transformUpdateRequired = true;
    }

	RigidBody::RigidBody(Vector2D position, BodyType bodyType, float mass, float restitution,
        float radius, Vector2D dimension, Shape bodyShape) {
        this->velocity = Vector2D(); // (0,0)
		this->position = position;
        this->bodyType = bodyType;
        this->rotation = 0.f;
        this->rotationVelocity = 0.f;
        this->force = Vector2D(); // (0,0)
        this->density = 0.f;
        this->mass = mass;
        this->restitution = restitution;
        this->area = 0.f;
        this->radius = radius;
        this->dimension = dimension;
        this->bodyShape = bodyShape;
        // if body dynamic, calculate the inverse mass, otherwise set as 0.f
        this->InvMass = (this->bodyType == BodyType::Dynamic) ? (1.f / this->mass) : 0.f;
        
        if (this->bodyShape == Shape::Box) {
            CreateBoxBody(this->dimension.x, this->dimension.y, this->mass, this->restitution);
            this->vertices = CreateBoxVertices(this->dimension);
            
            this->transformedVertices = this->vertices;
            // making the transform vertices same size as the vertices, not neccessary
            //this->transformedVertices(this->vertices.size());
        }
        else if (this->bodyShape == Shape::Circle) {
            CreateCircleBody(this->radius, this->mass, this->restitution);
        }
        else {
            // make sure the vector array is empty
            this->vertices.clear();
            this->transformedVertices.clear();
        }

        this->transformUpdateRequired = true;

	}

    std::vector<Vector2D> RigidBody::CreateBoxVertices(Vector2D dimension) {
        float left = - dimension.x / 2.f;
        float right = left + dimension.x;
        float bottom = - dimension.y / 2.f;
        float top = bottom + dimension.y;

        std::vector<Vector2D> vertices;
        vertices.emplace_back(Vector2D(left,top)); //0
        vertices.emplace_back(Vector2D(right, top)); //1
        vertices.emplace_back(Vector2D(right, bottom)); //2
        vertices.emplace_back(Vector2D(left, bottom)); //3

        return vertices;
    }

    std::vector<Vector2D> RigidBody::GetTransformedVertices()
    {
        // if true means the game object had move from previous place
        if (this->transformUpdateRequired)
        {
            BodyTransform transform(this->position, this->rotation);
            //std::cout << this->position.x << ", " << this->position.y << std::endl;

            // loop for each vertice
            for (int i = 0; i < this->vertices.size(); i++)
            {
                Vector2D v = this->vertices[i];
                // apply transform function to calculate correct vertices
                this->transformedVertices[i] = RigidBody::Transform(v, transform);
                //std::cout << this->transformedVertices[i].x << "- " << this->transformedVertices[i].y << std::endl;
            }
        }

        this->transformUpdateRequired = false;
        return this->transformedVertices;
    }


    Vector2D RigidBody::Transform(Vector2D v, BodyTransform transform) {
        /*return new FlatVector(
            transform.Cos * v.X - transform.Sin * v.Y + transform.PositionX,
            transform.Sin * v.X + transform.Cos * v.Y + transform.PositionY);*/

        Vector2D ret;
        // math stuff
        ret.x = transform.Cos * v.x - transform.Sin * v.y + transform.PositionX;
        ret.y = transform.Sin * v.x + transform.Cos * v.y + transform.PositionY;
        //std::cout << transform.PositionX << "- " << transform.PositionY << std::endl;
        //std::cout << ret.x << "- " << ret.y << std::endl;
        return ret;
    }

    void RigidBody::Move(Vector2D val) {
        this->position += val;
        this->transformUpdateRequired = true;
        this->transformedVertices = GetTransformedVertices();
    }

    void RigidBody::Rotate(float val) {
        this->rotation += val;
        //this->rotation = fmod(this->rotation, (2.0 * PI));
        this->transformUpdateRequired = true;
    }

    void RigidBody::AddForce(Vector2D val) {
        this->force = val;
    }

    void RigidBody::CreateBoxBody(float width, float height, float mass, float restitution) {
        this->radius = 0.f;
        this->area = width * height;
        // set the range to be [0,1]
        this->restitution = std::max(0.0f, std::min(restitution, 1.0f));

        this->density = mass * area; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
    }

    void RigidBody::CreateCircleBody(float radius, float mass, float restitution) {
        this->dimension = Vector2D(); //(0, 0);
        this->area = radius * radius * PI;
        // set the range to be [0,1]
        this->restitution = std::max(0.0f, std::min(restitution, 1.0f));

        this->density = mass * area; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world
    }
}