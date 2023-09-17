#include "Pch.h"
#include "CoreEngine.h"
using namespace IS;

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
		//InsightEngine::Instance().RegisterComponent<Collider>();
		//std::cout << "PhysicsSystem initialized." << std::endl;
		//The system can also subscribe to the message type it wants here
		Subscribe(MessageType::Collide);
	}
	bool addG = false;
	void Physics::Update(float dt)
	{
		Vector2D v = { 0.f, -9.8f };
		
		for (auto const& entity : mEntities) {
			InputManager& input = InputManager::Instance();
			if (input.IsKeyPressed(GLFW_KEY_G)) {
				addG = true;
			}
			else if(input.IsKeyPressed(GLFW_KEY_F)) {
				addG = false;
			}
			if (addG) {
				v += v * dt;
			}
			else {
				v = Vector2D();
			}
			// update gravity
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
				auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
				if (body.bodyType != BodyType::Dynamic) {
					continue;
				}
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				trans.world_position.x += v.x;
				trans.world_position.y += v.y;
			}
			
			
		}
		collisionCheck(dt, mEntities);
#if 0
		//loops through all Entities registered by the System this mEntities map is written in the parent class
		for (auto const& entity : mEntities) {
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
				//getting rigidbody component for each entit
				auto& rigidBody = InsightEngine::Instance().GetComponent<RigidBody>(entity);
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entity);
				rigidBody.bodyFollowTransform(trans);
				//std::cout << "body1:" << rigidBody.bodyTransform.world_position.x << " - " << rigidBody.bodyTransform.world_position.y << std::endl;

				for (auto const& entity2 : mEntities) {
					if (entity == entity2) {
						continue;
					}
					if (InsightEngine::Instance().HasComponent<RigidBody>(entity2)) {
						auto& rigidBody2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
						auto& trans2 = InsightEngine::Instance().GetComponent<Transform>(entity2);
						auto& sprite2 = InsightEngine::Instance().GetComponent<Sprite>(entity2);
						rigidBody2.bodyFollowTransform(trans2);
						//std::cout << "body:" << rigidBody.transformedVertices[0].x << " - " << rigidBody.transformedVertices[0].y << std::endl;
						//std::cout << "body2:" << rigidBody2.transformedVertices[0].x << " - " << rigidBody2.transformedVertices[0].y << std::endl;
						//std::cout << "body1:" << rigidBody.transformedVertices[0].x << " - " << rigidBody.transformedVertices[0].y << std::endl;
						Vector2D normal{ 1,1 };
						float depth = 0.f;
						//float adt += dt;
						if (Intersection_Polygons(rigidBody.GetTransformedVertices(), rigidBody.bodyTransform.getWorldPosition(), rigidBody2.GetTransformedVertices(), rigidBody2.bodyTransform.getWorldPosition(), normal, depth)) {
							//std::cout << adt << "Colliding" << std::endl;
							//sprite.color = glm::vec3(1.f, 1.f, 0.f);
							//sprite2.color = glm::vec3(1.f, 0.f, 0.f);
							Vector2D v = normal * depth;
							if (rigidBody.bodyType != BodyType::Dynamic)
							{
								//rigidBody2.Move(normal * depth);
								trans2.world_position.x += v.x;
								trans2.world_position.y += v.y;
							}
							else if (rigidBody2.bodyType != BodyType::Dynamic)
							{
								//rigidBody.Move(-normal * depth);
								trans.world_position.x += v.x;
								trans.world_position.y += v.y;
							}
							else
							{
								//rigidBody.Move(-normal * depth / 2.f);
								//rigidBody.Move(normal * depth / 2.f);
								trans.world_position.x += -v.x/2.;
								trans.world_position.y += -v.y/2.f;
								trans2.world_position.x += v.x/2.f;
								trans2.world_position.y += v.y/2.f;
							}

							ResolveCollision(rigidBody, rigidBody2, normal, depth);
						}
						else {
							sprite.color = glm::vec3(1.f, 0.f, 1.f);
							sprite2.color = glm::vec3(0.f, 0.f, 1.f);
						}
					}

				}
			}

		}
#endif

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
			//body.velocity.y += gravity.y * dt;
			//body.position = body.position + body.velocity * dt;
			break;
		case IS::BodyType::Kinematic:
			return;
			break;
		default:
			break;
		}
	}

	void collisionCheck(float dt, std::set<Entity> mEntities) {
		//loops through all Entities registered by the System this mEntities map
		for (auto const& entityA : mEntities) {
			if (InsightEngine::Instance().HasComponent<RigidBody>(entityA)) { // if entityA has rigidbody component
				//getting rigidbody component for each entity
				auto& rigidBodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entityA);
				auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entityA);
				rigidBodyA.bodyFollowTransform(trans);

				for (auto const& entityB : mEntities) {
					if (entityA == entityB) { // if self checking, continue
						continue;
					}
					if (InsightEngine::Instance().HasComponent<RigidBody>(entityB)) { // if entityB has rigidbody component
						auto& rigidBodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
						auto& trans2 = InsightEngine::Instance().GetComponent<Transform>(entityB);
						auto& sprite2 = InsightEngine::Instance().GetComponent<Sprite>(entityB);
						rigidBodyB.bodyFollowTransform(trans2);
						// for calculate collision response
						Vector2D normal;
						float depth;
						bool isColliding = false;
						switch (rigidBodyA.bodyShape)
						{
						case Shape::Box:
							switch (rigidBodyB.bodyShape)
							{
								// box vs box
							case Shape::Box:
								isColliding = Intersection_Polygons(rigidBodyA.GetTransformedVertices(), rigidBodyA.bodyTransform.getWorldPosition(), rigidBodyB.GetTransformedVertices(), rigidBodyB.bodyTransform.getWorldPosition(), normal, depth);
								break;
								// box vs circle
							case Shape::Circle:
								break;
							default:
								break;
							}
							break;
						case Shape::Circle:
							switch (rigidBodyB.bodyShape)
							{
								// circle vs box
							case Shape::Box:
								break;
								// circle vs circle
							case Shape::Circle:
								break;
							default:
								break;
							}
							break;
						default:
							break;
						}
						// if body A and body B colliding
						if (isColliding) {
							Vector2D v = normal * depth;
							// if body A is static 
							if (rigidBodyA.bodyType != BodyType::Dynamic)
							{
								//rigidBody2.Move(normal * depth);
								trans2.world_position.x += v.x;
								trans2.world_position.y += v.y;
							}
							// if body B is static 
							else if (rigidBodyB.bodyType != BodyType::Dynamic)
							{
								//rigidBody.Move(-normal * depth);
								trans.world_position.x += v.x;
								trans.world_position.y += v.y;
							}
							else // both are dynamic
							{
								//rigidBody.Move(-normal * depth / 2.f);
								//rigidBody.Move(normal * depth / 2.f);
								trans.world_position.x += -v.x / 2.;
								trans.world_position.y += -v.y / 2.f;
								trans2.world_position.x += v.x / 2.f;
								trans2.world_position.y += v.y / 2.f;
							}
							sprite.color = glm::vec3(1.f, 0.f, 1.f);
							sprite2.color = glm::vec3(0.f, 0.f, 1.f);
							ResolveCollision(rigidBodyA, rigidBodyB, normal, depth);
						}
					}

				}
			}
		}
	}
#if 0
	void collisionCallUpdate(RigidBody body, float dt, auto const& entity, std::set<Entity> mEntities)
	{
		switch (body.bodyShape)
		{
		case Shape::Box:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				//if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					//auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					Vector2D test{ 1,1 };
					float test2 = 0.f;
					switch (body2.bodyShape)
					{
					case Shape::Box:
						//box to box collision
						if (Intersection_Polygons(body.transformedVertices, body.bodyTransform.getWorldPosition(), body2.transformedVertices, body2.bodyTransform.getWorldPosition(), test, test2)) {
							std::cout << "Colliding" << std::endl;
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
				//}
			}
			break;
		case Shape::Circle:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				//if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					//auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
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
				//}
			}
			break;
		case Shape::Line:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				//if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					//auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					//line to box and line to circle collision defined above
					if (body2.bodyShape == Shape::Line)
					{
						//box to line collision
					}
			//	}
			}
			break;
		default:
			break;
		}
	}
#endif
	void ResolveCollision(RigidBody bodyA, RigidBody bodyB, Vector2D normal, float depth) {
		Vector2D relativeVelocity = bodyB.velocity - bodyA.velocity;

		if (ISVector2DDotProduct(relativeVelocity, normal) > 0.f) //
		{
			return;
		}

		float e = std::min(bodyA.restitution, bodyB.restitution);

		float j = -(1.f + e) * ISVector2DDotProduct(relativeVelocity, normal);
		j /= bodyA.InvMass + bodyB.InvMass;

		Vector2D impulse = j * normal;

		bodyA.velocity -= impulse * bodyA.InvMass;
		bodyB.velocity += impulse * bodyB.InvMass;
	}



}