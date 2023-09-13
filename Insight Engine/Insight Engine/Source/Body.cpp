#include "Pch.h"
#include "CoreEngine.h"

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
            this->vertices = CreateBoxVertices(this->dimension);
            // making the transform vertices same size as the vertices, not neccessary
            //this->transformedVertices(this->vertices.size());
        }
        else {
            // make sure the vector array is empty
            this->vertices.clear();
            this->transformedVertices.clear();
        }

        this->transformUpdateRequired = true;
    }

	RigidBody::RigidBody(Vector2D position, BodyType bodyType, float density, float mass, float restitution, float area,
        float radius, Vector2D dimension, Shape bodyShape) {
        this->velocity = Vector2D(); // (0,0)
		this->position = position;
        this->bodyType = bodyType;
        this->rotation = 0.f;
        this->rotationVelocity = 0.f;
        this->force = Vector2D(); // (0,0)
        this->density = density;
        this->mass = mass;
        this->restitution = restitution;
        this->area = area;
        this->radius = radius;
        this->dimension = dimension;
        this->bodyShape = bodyShape;
        // if body dynamic, calculate the inverse mass, otherwise set as 0.f
        this->InvMass = (this->bodyType == BodyType::Dynamic) ? (1.f / this->mass) : 0.f;
        
        if (this->bodyShape == Shape::Box) {
            this->vertices = CreateBoxVertices(this->dimension);
            // making the transform vertices same size as the vertices, not neccessary
            //this->transformedVertices(this->vertices.size());
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

            // loop for each vertice
            for (int i = 0; i < this->vertices.size(); i++)
            {
                Vector2D v = this->vertices[i];
                // apply transform function to calculate correct vertices
                this->transformedVertices[i] = RigidBody::Transform(v, transform);
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
        return ret;
    }

    void RigidBody::Move(Vector2D amount) {
        this->position += amount;
        this->transformUpdateRequired = true;
    }

    void RigidBody::Rotate(float amount) {
        this->rotation += amount;
        this->transformUpdateRequired = true;
    }

    bool RigidBody::CreateBoxBody(float width, float height, Vector2D position, float density, float restitution) {
        float area = width * height;

        restitution = std::max(0.0f, std::min(restitution, 1.0f)); // set the range to be [0,1]

        float mass = density / area; // m=p/v => m=p/A*depth => assume the depth for all objects are same in 2D world

        return true;
    }
}