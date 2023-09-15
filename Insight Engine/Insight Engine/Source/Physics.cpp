#include "Pch.h"
#include "CoreEngine.h"

namespace IS 
{
	Physics* PHYSICS = NULL;

	Physics::Physics() 
	{
		//ErrorIf(PHYSICS != NULL, "Physics already initialized");
		//PHYSICS = this;
		//Gravity = Vector2D(0, -400);
		//Gravity = 9.8f;
		//MaxVelocity = 1000;

	}

	void Physics::Initialize()
	{
		// Initialization logic here
		//RegisterComponent(Body);
		//InsightEngine::Instance().RegisterComponent<RigidBody>();
		InsightEngine::Instance().RegisterComponent<Collider>();
		std::cout << "PhysicsSystem initialized." << std::endl;
		//The system can also subscribe to the message type it wants here
		Subscribe(MessageType::Collide);
	}

	void Physics::Update(float dt)
	{
		//const float TimeStep = 1.0f / 60.0f;
		//loops through all Entities registered by the System this mEntities map is written in the parent class
		for (auto const& entity : mEntities) {

			//getting rigidbody component for each entit
			auto& rigidBody = InsightEngine::Instance().GetComponent<RigidBody>(entity);
			//getting collider component for each entity
			auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);

			//if the entity has a certain component do something with it
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
				// update rigidbody, such as gravity/position

				//rigidBodyCallUpdate(rigidBody, Gravity, dt);
				//if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
					// check collision
					collisionCallUpdate(rigidBody, dt, entity, mEntities);
				//}
			}

		}

	}

		
	void rigidBodyCallUpdate(RigidBody body, Vector2D gravity, float dt)
	{
		// if the body is dynamic, adding gravity
		switch (body.bodyType)
		{
		case IS::BodyType::Static:
			return;
			break;
		case IS::BodyType::Dynamic:
			//apply force here
			//setDynamicObject(body)
			body.velocity.y += gravity.y * dt;
			body.position = body.position + body.velocity * dt;
			break;
		case IS::BodyType::Kinematic:
			return;
			break;
		default:
			break;
		}
	}

	void collisionCallUpdate(RigidBody body, float dt, auto const& entity, std::set<Entity> mEntities)
	{
		switch (body.bodyShape)
		{
		case Shape::Box:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					Vector2D test{ 1,1 };
					float test2 = 0.f;
					switch (body2.bodyShape)
					{
					case Shape::Box:
						//box to box collision
						if (Intersection_Polygons(body.transformedVertices, body2.transformedVertices, test, test2)) {
							std::cout << "Colliding" << std::endl;
						//if (CollisionIntersection_RectRect(std::get<Box>(collider.myCollider), body.velocity, std::get<Box>(collider2.myCollider), body2.velocity, dt)) {
							// do collision effect here
						}
						break;
					case Shape::Circle:
						//box to circle collision
						break;
					case Shape::Line:
						//box to line collision
						break;
					default:
						break;
					}
				}
			}
			break;
		case Shape::Circle:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					switch (body2.bodyShape)
					{
					//circle to box collision defined above
					case Shape::Circle:
						//circle to circle collision
						break;
					case Shape::Line:
						//circle to line collision
						break;
					default:
						break;
					}
				}
			}
			break;
		case Shape::Line:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					//line to box and line to circle collision defined above
					if (body2.bodyShape == Shape::Line)
					{
						//box to line collision
					}
				}
			}
			break;
		default:
			break;
		}
	}



}