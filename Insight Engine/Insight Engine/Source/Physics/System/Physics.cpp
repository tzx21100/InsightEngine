/*!
 * \file Physics.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the Physics class and related functions. It is responsible
 * for managing physics simulations and collision handling, including collision
 * detection, collision response, and updating entity states based on physics
 * calculations for game entities.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Physics.h"
#include "Graphics/System/Sprite.h"
#include "Graphics/System/Light.h"

namespace IS {
		
	bool Physics::mShowColliders = false;								// Flag indicating whether the collider will be drawn
	bool Physics::mShowVelocity = false;								// Flag indicating whether the velocity will be drawn
	bool Physics::mShowGrid = false;									// Flag indicating whether the grid will be drawn
	bool Physics::mEnableImplicitGrid = false;							// Flag indicating whether implicit grid is enable
	bool Physics::mExertingGravity = true;								// Flag indicating whether gravity is currently exerted
	Vector2D Physics::mGravity = Vector2D(0.f, -981.f);					// Gravity of the world
	std::set<Entity> Physics::PhysicsEnableList = std::set<Entity>();	// Enable entities list for physics
	// Constructs a Physics instance
	Physics::Physics()
	{
		mMaxVelocity = 20000.f;									// Maximum velocity for game bodies
		mMinVelocity = -20000.f;								// Minimum velocity for game bodies
		mCurrentIterations = 0;									// Number of current iterations for physics step
		mTotalIterations = 1;									// Number of iterations for physics step
	}

	// Initializes the physics system
	void Physics::Initialize()
	{
	}

	// Updates the physics simulation for the given time step
	void Physics::Update(float dt)
	{
		if (InsightEngine::Instance().mRuntime == false)
		{
			return;
		}

		//// for testing grid
		//auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
		//if (input->IsKeyPressed(GLFW_KEY_3))
		//{
		//	for (int i = 0; i < ImplicitGrid::mRows; i++)
		//	{
		//		std::cout << "[ " << i << " row ]" << mImplicitGrid.mRowsBitArray[i].count() << std::endl;

		//	}
		//	for (int j = 0; j < ImplicitGrid::mCols; j++)
		//	{
		//		std::cout << "[ " << j << " col ]" << mImplicitGrid.mColsBitArray[j].count() << std::endl;
		//	}
		//	IS_CORE_INFO({ "InGridSize - {}" }, mImplicitGrid.mInGridList.size());
		//	IS_CORE_INFO({ "OverlapSize - {}" }, mImplicitGrid.mOverlapGridList.size());
		//	IS_CORE_INFO({ "OutSize - {}" }, mImplicitGrid.mOutsideGridList.size());
		//}

		/*if (InsightEngine::currentNumberOfSteps > 1) {
			mTotalIterations = 1;
		}
		else {
			mTotalIterations = 10;
		}*/
		//Loop using Fixed DT
		//dt = static_cast<float>(InsightEngine::Instance().mFixedDeltaTime);
		for (int step = 0; step < InsightEngine::Instance().GetCurrentNumberOfSteps(); ++step)
		{
			// physics update iteration

				// Performs a physics step for the set of entities with dt, updates velocities and positions for game entities
				Step(dt, PhysicsEnableList);
			
		}
			// set it back to 0 for next iteration loop
			mCurrentIterations = 0;
		
	}
#if 0

	// collision detect for implicit grid
	void Physics::ImplicitGridCollisionDetect()
	{
		// loop through each row with each col
		for (int row = 0; row < ImplicitGrid::mRows; row++)
		{
			for (int col = 0; col < ImplicitGrid::mCols; col++)
			{
				// check the existence of the objects in a cell, simply perform a bitwise AND operation
				std::bitset<MAX_ENTITIES> test_cell;

				for (Entity i = 0; i < InsightEngine::Instance().EntitiesAlive() && i < MAX_ENTITIES; ++i)
				{
					test_cell[i] = mImplicitGrid.mRowsBitArray[row][i] && mImplicitGrid.mColsBitArray[col][i];
				}

				if (test_cell == 0)
				{ // no entity in current cell, continue
					continue;
				}


				// getting entity number in this cell
				size_t totalEntities = 0;

				for (Entity i = 0; i < InsightEngine::Instance().EntitiesAlive() && i < MAX_ENTITIES; ++i)
				{
					if (test_cell[i])
					{
						totalEntities++;
					}
				}

				// at least more than 1 entity to avoid self checking
				// in case one entity overlaps on the grid check collide with one in grid entity
				if (totalEntities > 1)
				{
					// emplace all the entities in current cell
					for (Entity e = 0; e < InsightEngine::Instance().EntitiesAlive(); ++e)
					{
						if (test_cell.test(e))
						{ // if the current bit entity is true
	// emplace into InGridList
							mImplicitGrid.mInGridList.emplace_back(e);
							totalEntities--;
							if (totalEntities <= 0)
							{
								// if there is no more entities in this cell, break
								break;
							}
						}
					}

					// need check with all the overlap entities, in case the entities having outrageous width / heigth				
					//mImplicitGrid.EmplaceEntity(mImplicitGrid.mInGridList, mImplicitGrid.mOverlapGridList);

					if (mImplicitGrid.mInGridList.size() > 1)
					{
						CollisionDetect(mImplicitGrid.mInGridList);

						// empty the list
						mImplicitGrid.mInGridList.clear();
					}
				}


			}
		}

		// for collision outside or overlap of the grid
		if (mImplicitGrid.mOutsideGridList.size() > 1)
		{
			CollisionDetect(mImplicitGrid.mOutsideGridList);
		}
	}

	// Detects collisions among a set of entities, running different collision detect function based on the body shape (box, circle or line)
	void Physics::CollisionDetect(std::vector<Entity> const& entities)
	{

		// Loop through all Entities registered by the System
		for (size_t i = 0; i < entities.size() - 1; ++i)
		{
			//for (size_t i = 0; i < entities.size(); ++i) {
			const Entity& entityA = entities[i];

			if (InsightEngine::Instance().HasComponent<RigidBody>(entityA))
			{

				for (size_t j = i + 1; j < entities.size(); ++j)
				{
					//for (size_t j = 0; j < entities.size(); ++j) {
					const Entity& entityB = entities[j];

					if (entityA == entityB)
					{ // if self checking, continue
						continue;
					}

					if (InsightEngine::Instance().HasComponent<RigidBody>(entityB))
					{

						auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
						auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);

						if (bodyA.mBodyType != BodyType::Dynamic && bodyB.mBodyType != BodyType::Dynamic)
						{
							// continue if collision happens between two non dynamic entities
							continue;
						}
						// static AABB collision check, continue to the next loop if not colliding
						if (!StaticIntersectAABB(bodyA.GetAABB(), bodyB.GetAABB()))
						{
							continue;
						}
						mContactPair.emplace_back(std::make_pair(entityA, entityB));
					}
				}
			}
		}
	}

	// Detects collisions among all the possible entities
	void Physics::BroadPhase()
	{
		// if using implicit grid
		if (mEnableImplicitGrid)
		{
			// detect collision through Implicit Grid
			ImplicitGridCollisionDetect();
		} else
		{ // not using implict grid (Brute Force)
			std::vector<Entity> list;
			for (auto const& entity : mEntities)
			{
				list.emplace_back(entity);
			}
			CollisionDetect(list);
		}
	}

	// Resloves collisions among all the entities
	void Physics::NarrowPhase()
	{

		for (int i = 0; i < mContactPair.size(); i++)
		{
			std::pair<Entity, Entity> pair = mContactPair[i];
			Entity entityA = pair.first;
			Entity entityB = pair.second;

			auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
			auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
			auto& transA = InsightEngine::Instance().GetComponent<Transform>(entityA);
			auto& transB = InsightEngine::Instance().GetComponent<Transform>(entityB);
			bodyA.BodyFollowTransform(transA);
			bodyB.BodyFollowTransform(transB);

			bool isColliding = false;
			switch (bodyA.mBodyShape)
			{
			case BodyShape::Box:
				switch (bodyB.mBodyShape)
				{
					// box vs box
				case BodyShape::Box:
					isColliding = IntersectionPolygons(bodyA.mTransformedVertices, bodyA.mBodyTransform.getWorldPosition(), bodyB.mTransformedVertices, bodyB.mBodyTransform.getWorldPosition(), mManifoldInfo.mNormal, mManifoldInfo.mDepth);
					break;
					// box vs circle
				case BodyShape::Circle:
					break;
				default:
					break;
				}
				break;
			case BodyShape::Circle:
				switch (bodyB.mBodyShape)
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
			if (isColliding)
			{
				// vector of penetration depth to move entities apart
				SeparateBodies(&bodyA, &bodyB, &transA, &transB, mManifoldInfo.mNormal * mManifoldInfo.mDepth);

				// calculate the contact point information
				mManifoldInfo.FindContactPoints(bodyA, bodyB);
				Manifold contact = Manifold(&bodyA, &bodyB, mManifoldInfo.mNormal, mManifoldInfo.mDepth, mManifoldInfo.mContact1, mManifoldInfo.mContact2, mManifoldInfo.mContactCount);
				ResolveCollisionWithRotationAndFriction(contact);

			}
		}
	}

	// separate body when penetrating during colliding
	void Physics::SeparateBodies(RigidBody* bodyA, RigidBody* bodyB, Transform* transA, Transform* transB, Vector2D const& vec)
	{
		// vector of penetration depth to move entities apart

		// if body A is static 
		if (bodyA->mBodyType != BodyType::Dynamic)
		{
			transB->Move(vec);
			// to be optimize
			bodyB->mState = BodyState::GROUNDED;
		}
		// if body B is static 
		else if (bodyB->mBodyType != BodyType::Dynamic)
		{
			transA->Move(-vec);
			// to be optimize
			bodyA->mState = BodyState::GROUNDED;
		} else // both are dynamic
		{
			transA->Move(-vec / 2.f);
			transB->Move(vec / 2.f);
		}
	}
#endif

	// Draws the velocity and an outline around the specified rigid body using the provided sprite based on vertices for polygons.
	void Physics::DrawOutLine(Collider& collider, std::tuple<float, float, float> const& color)
	{
		// draw colliders in green
		if (mShowColliders)
		{
			if (collider.IsBoxColliderEnable()) { // if box collider is enable, draw it out
				for (size_t i = 0; i < collider.mBoxCollider.transformedVertices.size(); i++)
				{
					Vector2D va = collider.mBoxCollider.transformedVertices[i];
					Vector2D vb = collider.mBoxCollider.transformedVertices[(i + 1) % collider.mBoxCollider.transformedVertices.size()]; // modules by the size of the vector to avoid going out of the range

					Sprite::drawDebugLine(va, vb, color);
				}
			}
			if (collider.IsCircleColliderEnable()) { // if circle collider is enable, draw it out
				Sprite::drawDebugCircle(collider.mCircleCollider.center, { collider.mCircleCollider.radius * 2.f, collider.mCircleCollider.radius * 2.f }, color);
			}
			// to be done

		}

		// draw grid cell line in white
		if (mShowGrid) ImplicitGrid::DrawGrid();
		
		// draw the velocity line in blue
		//if (mShowVelocity) Sprite::drawDebugLine(body.mBodyTransform.getWorldPosition(), body.mBodyTransform.getWorldPosition() + body.mVelocity, { 1.f, 0.f, 0.f });
	}


	void Physics::AddLineSegementsForLights(Entity const& entity)
	{
		//for (auto const& entity: Physics::PhysicsEnableList) {
			
		//}

		auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
		auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
		if (collider.IsBoxColliderEnable()
			&& collider.mResponseEnable
			&& body.mBodyType == BodyType::Static) { // if check

			std::vector<Vector2D> vertices = collider.mBoxCollider.transformedVertices;
			for (int i = 0; i < vertices.size(); i++) {

				glm::vec4 vec = {	vertices[i].x,
									vertices[i].y,
									vertices[(i + 1) % vertices.size()].x,
									vertices[(i + 1) % vertices.size()].y };
				// emplace into the vector list
				Light::shadowLineSegments.emplace_back(vec);

			}
		}
	}


	// Performs a physics step for the specified time and set of entities, updates velocities and positions for game entities
	void Physics::Step(float time, std::set<Entity> const& entities)
	{
		// divide by iterations to increase precision
		time /= static_cast<float>(mTotalIterations);
		
		for (auto const& entity : entities)
		{
			auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);

			if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
				auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
				collider.UpdateCollider(trans);
			}

			// check if having rigidbody component
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity))
			{
				auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);

				body.BodyFollowTransform(trans);

				AddLineSegementsForLights(entity); // render shadow lights

				if (body.mBodyType == BodyType::Static) {
					continue; // skip the update for static entity
				}

				//freeze
				if (InsightEngine::Instance().mFreezeFrame)
				{
					if (!InsightEngine::Instance().mContinueFrame)
						return;
				}

				// update the velocity if gravity exists
				if (mExertingGravity)
				{
					body.mAcceleration = body.mForce * body.mInvMass + (mGravity * body.mGravityScale);
					body.mVelocity += body.mAcceleration * time;
				}

				// set range
				body.mVelocity.x = std::min(body.mVelocity.x, mMaxVelocity);
				body.mVelocity.y = std::min(body.mVelocity.y, mMaxVelocity);
				body.mVelocity.x = std::max(body.mVelocity.x, mMinVelocity);
				body.mVelocity.y = std::max(body.mVelocity.y, mMinVelocity);

				// if the entity is static or kinematic, skip
				if (body.mBodyType != BodyType::Dynamic)
				{
					body.mVelocity = Vector2D(0.f, 0.f);
				}

				// update position
				//mImplicitGrid.UpdateCell(entity, time);
				body.mPosition += body.mVelocity * time;
				trans.world_position = body.mPosition;
				auto& sprite = InsightEngine::Instance().GetComponent<Sprite>(entity);
				sprite.followTransform(trans);

				//body.mRotation = trans.getRotation();
				body.mRotation += body.mAngularVelocity * time * 15.f;
				trans.setRotation(body.mRotation, body.mAngularVelocity);
				//trans.world_position += body.mVelocity * time;
			}
		}
	}
}

