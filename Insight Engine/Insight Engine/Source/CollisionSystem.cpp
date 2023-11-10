
#include "Pch.h"

namespace IS
{

	CollisionSystem::CollisionSystem() {
		mContactPair = std::vector<std::pair<Entity, Entity>>();// vector list of each two contact entities
		mManifoldInfo;
	}

	void CollisionSystem::Initialize() {

	}

	void CollisionSystem::Update(float dt)
	{
		BroadPhase();

		NarrowPhase();
	}

	void CollisionSystem::BroadPhase() 
	{
		CollisionDetect(mEntities);
	}

	void CollisionSystem::NarrowPhase() 
	{
		for (int i = 0; i < mContactPair.size(); i++)
		{
			std::pair<Entity, Entity> pair = mContactPair[i];
			Entity entityA = pair.first;
			Entity entityB = pair.second;

			auto& transA = InsightEngine::Instance().GetComponent<Transform>(entityA);
			auto& transB = InsightEngine::Instance().GetComponent<Transform>(entityB);
			auto& colliderA = InsightEngine::Instance().GetComponent<Collider>(entityA);
			auto& colliderB = InsightEngine::Instance().GetComponent<Collider>(entityB);
			colliderA.UpdateBoxCollider(transA);
			colliderB.UpdateBoxCollider(transB);

			bool boxes_colliding = false;

			if (colliderA.mSelectedCollider.test(ColliderShape::BOX)) {
				if (colliderB.mSelectedCollider.test(ColliderShape::BOX)) { // box vs box
					boxes_colliding = IntersectionPolygons(colliderA.mBoxCollider.transformedVertices, colliderA.mBoxCollider.center, colliderB.mBoxCollider.transformedVertices, colliderB.mBoxCollider.center, mManifoldInfo.mNormal, mManifoldInfo.mDepth);
				}
				if (colliderB.mSelectedCollider.test(ColliderShape::CIRCLE)) { // box vs circle

				}
				if (colliderB.mSelectedCollider.test(ColliderShape::LINE)) { // box vs line

				}
			}

			// TO DO:
			// circle collider check
			// line collider check

			// if collider A and collider B colliding
			if (boxes_colliding) {

				BodyType typeA = BodyType::Static;
				BodyType typeB = BodyType::Static;

				if (InsightEngine::Instance().HasComponent<RigidBody>(entityA)) {
					auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
					typeA = bodyA.mBodyType;
				}

				if (InsightEngine::Instance().HasComponent<RigidBody>(entityB)) {
					auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
					typeB = bodyB.mBodyType;
				}

				// vector of penetration depth to move entities apart
				SeparateColliders(typeA, typeB, transA, transB, mManifoldInfo.mNormal * mManifoldInfo.mDepth);

				// calculate the contact point information
				//mManifoldInfo.FindContactPoints(colliderA, colliderB);
				mManifoldInfo.FindPolygonsContactPoints(colliderA);
				Manifold contact = Manifold(&colliderA, &colliderB, mManifoldInfo.mNormal, mManifoldInfo.mDepth, mManifoldInfo.mContact1, mManifoldInfo.mContact2, mManifoldInfo.mContactCount);


			}
		}
	}

	void CollisionSystem::CollisionDetect(std::set<Entity> const& entities) {
		for (auto const& entityA : mEntities)
		{
			if (InsightEngine::Instance().HasComponent<Collider>(entityA)) {

				for (auto const& entityB : mEntities) {

					if (entityA == entityB) { // if self checking, continue
						continue;
					}

					if (InsightEngine::Instance().HasComponent<Collider>(entityB)) {
						auto& transA = InsightEngine::Instance().GetComponent<Transform>(entityA);
						auto& transB = InsightEngine::Instance().GetComponent<Transform>(entityB);
						//auto& colliderA = InsightEngine::Instance().GetComponent<Collider>(entityA);
						//auto& colliderB = InsightEngine::Instance().GetComponent<Collider>(entityB);
						auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
						auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);

						if (bodyA.mBodyType != BodyType::Dynamic && bodyB.mBodyType != BodyType::Dynamic)
						{
							// continue if collision happens between two non dynamic entities
							continue;
						}

						mContactPair.emplace_back(std::make_pair(entityA, entityB));

						
					}
				}

				for (bits that are set in collider)
				{
					// update respective collider
					//somecollider.update(trans.world_position);
				}

			}
		}
	}

	void CollisionSystem::SeparateColliders(BodyType typeA, BodyType typeB, Transform& transA, Transform& transB, Vector2D const& vec) {



		// if body A is static 
		if (typeA != BodyType::Dynamic)
		{
			transB.Move(vec);
			// to be optimize
			//bodyB.mState = BodyState::GROUNDED;
		}
		// if body B is static 
		else if (typeB != BodyType::Dynamic)
		{
			transA.Move(-vec);
			// to be optimize
			//bodyA.mState = BodyState::GROUNDED;
		}
		else // both are dynamic
		{
			transA.Move(-vec / 2.f);
			transB.Move(vec / 2.f);
		}
		
	}

	

	void CollisionSystem::ResolveCollision(Manifold& contact) 
	{


		
	}

}