#include "Pch.h"
#include "CoreEngine.h"
#include "Sprite.h"
using namespace IS;

namespace IS 
{
	Physics* PHYSICS = NULL;

	Physics::Physics() 
	{
		//ErrorIf(PHYSICS != NULL, "Physics already initialized");
		//PHYSICS = this;
		mGravity = Vector2D(0, -981.f);
		mExertingGravity = false;
		//isDebugDraw = false;
		mMaxVelocity = 1000.f;
		mMinVelocity = -1000.f;
		iterations = 10;

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

	bool Physics::isDebugDraw = false;

	

	void Physics::Update(float dt)
	{
		
		for (size_t i = 0; i < iterations; i++) {
			Step(dt, mEntities);
			CollisionDetect(mEntities);
		}

	}

		
	void rigidBodyCallUpdate(RigidBody body, [[maybe_unused]] Vector2D gravity, [[maybe_unused]] float dt)
	{
		// if the body is dynamic, adding gravity
		switch (body.mBodyType)
		{
		case IS::BodyType::Static:
			return;
			break;
		case IS::BodyType::Dynamic:
			//apply force here
			//setDynamicObject(body)
			//body.mVelocity.y += gravity.y * dt;
			//body.position = body.position + body.mVelocity * dt;
			break;
		case IS::BodyType::Kinematic:
			return;
			break;
		default:
			break;
		}
	}

	void CollisionDetect(std::set<Entity> const& mEntities) {
		//loops through all Entities registered by the System this mEntities map
		for (auto const& entityA : mEntities) {
			if (InsightEngine::Instance().HasComponent<RigidBody>(entityA)) { // if entityA has rigidbody component
				//getting rigidbody component for each entity
				auto& rigidBodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entityA);
				auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entityA);
				rigidBodyA.BodyFollowTransform(trans);

				for (auto const& entityB : mEntities) {
					if (entityA == entityB) { // if self checking, continue
						continue;
					}
					if (InsightEngine::Instance().HasComponent<RigidBody>(entityB)) { // if entityB has rigidbody component
						auto& rigidBodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
						auto& trans2 = InsightEngine::Instance().GetComponent<Transform>(entityB);
						auto& sprite2 = InsightEngine::Instance().GetComponent<Sprite>(entityB);
						rigidBodyB.BodyFollowTransform(trans2);
						// for calculate collision response
						Vector2D normal = Vector2D();
						float depth = std::numeric_limits<float>::max();
						bool isColliding = false;
						switch (rigidBodyA.mBodyShape)
						{
						case BodyShape::Box:
							switch (rigidBodyB.mBodyShape)
							{
								// box vs box
							case BodyShape::Box:
								isColliding = IntersectionPolygons(rigidBodyA.GetTransformedVertices(), rigidBodyA.mBodyTransform.getWorldPosition(), rigidBodyB.GetTransformedVertices(), rigidBodyB.mBodyTransform.getWorldPosition(), normal, depth);
								break;
								// box vs circle
							case BodyShape::Circle:
								break;
							default:
								break;
							}
							break;
						case BodyShape::Circle:
							switch (rigidBodyB.mBodyShape)
							{
								// circle vs box
							case BodyShape::Box:
								break;
								// circle vs circle
							case BodyShape::Circle:
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
							Vector2D vec = normal * depth;
							// if body A is static 
							if (rigidBodyA.mBodyType != BodyType::Dynamic)
							{
								//rigidBody2.Move(normal * depth);
								trans2.world_position.x += vec.x;
								trans2.world_position.y += vec.y;
								rigidBodyB.mState = BodyState::GROUNDED;
							}
							// if body B is static 
							else if (rigidBodyB.mBodyType != BodyType::Dynamic)
							{
								//rigidBody.Move(-normal * depth);
								trans.world_position.x += -vec.x;
								trans.world_position.y += -vec.y;
								rigidBodyA.mState = BodyState::GROUNDED;
							}
							else // both are dynamic
							{
								//rigidBody.Move(-normal * depth / 2.f);
								//rigidBody.Move(normal * depth / 2.f);
								trans.world_position.x += -vec.x / 2.f;
								trans.world_position.y += -vec.y / 2.f;
								trans2.world_position.x += vec.x / 2.f;
								trans2.world_position.y += vec.y / 2.f;
							}
							sprite.color = glm::vec3(1.f, 0.f, 1.f);
							ResolveCollision(rigidBodyA, rigidBodyB, normal, depth);
						}
						else { //not colliding
							sprite2.color = glm::vec3(0.f, 0.f, 1.f);
						}
					}

				}
			}
		}
	}
#if 0
	void CollisionCallUpdate(RigidBody body, float dt, auto const& entity, std::set<Entity> mEntities)
	{
		switch (body.mBodyShape)
		{
		case Shape::Box:
			for (auto const& entity2 : mEntities) {
				//skip self comparison or the second entity do not have collider component
				//if (entity == entity2 || !(InsightEngine::Instance().HasComponent<Collider>(entity2))) {
					//auto& collider2 = InsightEngine::Instance().GetComponent<Collider>(entity2);
					auto& body2 = InsightEngine::Instance().GetComponent<RigidBody>(entity2);
					Vector2D test{ 1,1 };
					float test2 = 0.f;
					switch (body2.mBodyShape)
					{
					case Shape::Box:
						//box to box collision
						if (Intersection_Polygons(body.transformedVertices, body.mBodyTransform.getWorldPosition(), body2.transformedVertices, body2.mBodyTransform.getWorldPosition(), test, test2)) {
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
					switch (body2.mBodyShape)
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
					if (body2.mBodyShape == Shape::Line)
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
	void ResolveCollision(RigidBody& bodyA, RigidBody& bodyB, Vector2D const& normal, [[maybe_unused]] float depth) {
		Vector2D relativeVelocity = bodyB.mVelocity - bodyA.mVelocity;

		if (ISVector2DDotProduct(relativeVelocity, normal) > 0.f) //
		{
			return;
		}

		float e = std::min(bodyA.mRestitution, bodyB.mRestitution);

		float j = -(1.f + e) * ISVector2DDotProduct(relativeVelocity, normal);
		j /= bodyA.mInvMass + bodyB.mInvMass;

		Vector2D impulse = j * normal;

		bodyA.mVelocity -= impulse * bodyA.mInvMass;
		bodyB.mVelocity += impulse * bodyB.mInvMass;
	}

	void Physics::DrawOutLine(RigidBody & body, Sprite const& sprite) {
		for (size_t i = 0; i < body.mTransformedVertices.size(); i++) {
			Vector2D va = body.mTransformedVertices[i];
			Vector2D vb = body.mTransformedVertices[(i + 1) % body.mTransformedVertices.size()]; // modules by the size of the vector to avoid going out of the range
			sprite.drawLine(va, vb);
		}
		// draw the velocity line
		sprite.drawLine(body.mBodyTransform.getWorldPosition(), body.mBodyTransform.getWorldPosition() + body.mVelocity);
	}

	void Physics::UpdateGravity(auto const& key_input) {
		if (key_input->IsKeyPressed(GLFW_KEY_G)) {
			mExertingGravity = true;
			IS_CORE_DEBUG("mGravity Enabled!");
		}
		else if (key_input->IsKeyPressed(GLFW_KEY_F)) {
			mExertingGravity = false;
			IS_CORE_DEBUG("mGravity Disabled!");
		}
	}

	void Physics::UpdateDebugDraw(auto const& key_input) {
		if (key_input->IsKeyPressed(GLFW_KEY_2)) {
			Physics::isDebugDraw = true;
			IS_CORE_DEBUG("Draw Collision Boxes Enabled!");
		}
		else if (key_input->IsKeyPressed(GLFW_KEY_1)) {
			Physics::isDebugDraw = false;
			IS_CORE_DEBUG("Draw Collision Boxes Disabled!");
		}
	}

	void Physics::Step(float time, std::set<Entity> const& entities) {
		time /= static_cast<float>(iterations);

		for (auto const& entity : entities) {
			auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
			// check input for applying gravity
			UpdateGravity(input);

			// check input for drawing lines
			UpdateDebugDraw(input);

			// update gravity
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
				auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
				if (body.mBodyType != BodyType::Dynamic) {
					continue;
				}
				//freeze
				if (InsightEngine::Instance().mFreezeFrame) {
					if (!InsightEngine::Instance().mContinueFrame)
						return;
				}
				//std::cout << time << std::endl;
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				if (mExertingGravity) {
					body.mVelocity += mGravity * time;
				}

				body.mVelocity.x = std::min(body.mVelocity.x, mMaxVelocity);
				body.mVelocity.y = std::min(body.mVelocity.y, mMaxVelocity);
				body.mVelocity.x = std::max(body.mVelocity.x, mMinVelocity);
				body.mVelocity.y = std::max(body.mVelocity.y, mMinVelocity);
				trans.world_position.x += body.mVelocity.x * time;
				trans.world_position.y += body.mVelocity.y * time;
			}
		}
	}
}

