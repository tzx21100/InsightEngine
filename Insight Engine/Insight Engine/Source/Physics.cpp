#include "Pch.h"
#include "CoreEngine.h"

namespace IS 
{
	Physics* PHYSICS = NULL;

	Physics::Physics() 
	{
		//ErrorIf(PHYSICS != NULL, "Physics already initialized");
		PHYSICS = this;
		//Gravity = Vector2D(0, -400);
		Gravity = 9.8f;
		MaxVelocity = 1000;

	}

	void Physics::Initialize()
	{
		// Initialization logic here
		//RegisterComponent(Body);
		InsightEngine::Instance().RegisterComponent<RigidBody>();
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
				rigidBodyCallUpdate(rigidBody, dt);
				if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
					// check collision
					collisionCallUpdate(collider, dt, rigidBody, entity, mEntities);
				}
			}

		}

	}

		
	void rigidBodyCallUpdate(RigidBody body, float dt) 
	{
		switch (body.bodyType)
		{
		case IS::BodyType::Static:
			return;
			break;
		case IS::BodyType::Dynamic:
			//apply force here
			//setDynamicObject(body)
			body.velocity.y -= body.gravity * dt;
			body.position = body.position + body.velocity * dt;
			break;
		case IS::BodyType::Kinematic:
			return;
			break;
		default:
			break;
		}
	}

	void collisionCallUpdate(Collider collider, float dt, RigidBody body, auto const& entity, std::set<Entity> mEntities)
	{
		switch (collider.colliderType)
		{
		case ColliderType::AABB:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					switch (collider2.colliderType)
					{
					case ColliderType::AABB:
						//box to box collision
						if (CollisionIntersection_RectRect(std::get<AABB>(collider.myCollider), body.velocity, std::get<AABB>(collider2.myCollider), body2.velocity, dt)) {
							// do collision effect here
						}
						break;
					case ColliderType::Circle:
						//box to circle collision
						break;
					case ColliderType::Line:
						//box to line collision
						break;
					default:
						break;
					}
				}
			}
			break;
		case ColliderType::Circle:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					switch (collider2.colliderType)
					{
					//circle to box collision defined above
					case ColliderType::Circle:
						//circle to circle collision
						break;
					case ColliderType::Line:
						//circle to line collision
						break;
					default:
						break;
					}
				}
			}
			break;
		case ColliderType::Line:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					//line to box and line to circle collision defined above
					if (collider2.colliderType == ColliderType::Line) 
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