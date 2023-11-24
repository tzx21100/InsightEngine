/*!
 * \file Physics.cpp
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
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

namespace IS {

	bool Physics::mShowColliders = false;
	bool Physics::mShowVelocity = false;
	bool Physics::mShowGrid = false;
	bool Physics::mEnableImplicitGrid = false;
	bool Physics::mExertingGravity = true;				// Flag indicating whether gravity is currently exerted
	Vector2D Physics::mGravity = Vector2D(0.f, -981.f);							// Gravity of the world

	// Constructs a Physics instance
	Physics::Physics()
	{
		mMaxVelocity = 20000.f;									// Maximum velocity for game bodies
		mMinVelocity = -20000.f;									// Minimum velocity for game bodies
		mCurrentIterations = 0;									// Number of current iterations for physics step
		mTotalIterations = 1;									// Number of iterations for physics step
		mContactPair = std::vector<std::pair<Entity, Entity>>();// vector list of each two contact entities
		mManifoldInfo;											// instance of Manifold
		mImplicitGrid;											// instance of ImplicitGrid
	}

	// Initializes the physics system
	void Physics::Initialize()
	{
		mImplicitGrid.ClearGrid();
	}

	// Updates the physics simulation for the given time step
	void Physics::Update(float dt)
	{
		if (InsightEngine::Instance().mRuntime == false)
		{
			return;
		}

		// add new entity inside grid
		//mImplicitGrid.AddIntoCell(mEntities);

		// for testing grid
		auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
		if (input->IsKeyPressed(GLFW_KEY_3))
		{
			for (int i = 0; i < ImplicitGrid::mRows; i++)
			{
				std::cout << "[ " << i << " row ]" << mImplicitGrid.mRowsBitArray[i].count() << std::endl;

			}
			for (int j = 0; j < ImplicitGrid::mCols; j++)
			{
				std::cout << "[ " << j << " col ]" << mImplicitGrid.mColsBitArray[j].count() << std::endl;
			}
			IS_CORE_INFO({ "InGridSize - {}" }, mImplicitGrid.mInGridList.size());
			IS_CORE_INFO({ "OverlapSize - {}" }, mImplicitGrid.mOverlapGridList.size());
			IS_CORE_INFO({ "OutSize - {}" }, mImplicitGrid.mOutsideGridList.size());
		}

		/*if (InsightEngine::currentNumberOfSteps > 1) {
			mTotalIterations = 1;
		}
		else {
			mTotalIterations = 10;
		}*/

		//Loop using Fixed DT
		for (int step = 0; step < InsightEngine::currentNumberOfSteps; ++step)
		{

			// physics update iteration
			for (; mCurrentIterations < mTotalIterations; mCurrentIterations++)
			{

				// empty contact pair before going into collision step
				//mContactPair.clear();

				// Performs a physics step for the set of entities with dt, updates velocities and positions for game entities
				Step(dt, mEntities);

				// Collision Detection
				//BroadPhase();

				// Collision Resolution
				//NarrowPhase();
			}

			// set it back to 0 for next iteration loop
			mCurrentIterations = 0;
		}
		//mImplicitGrid.ClearGrid();
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



	// Resolves collisions between two rigid bodies by calculating and applying the impulse force to update the velocities of collding entities
	void Physics::ResolveCollision(Manifold& contact)
	{

		RigidBody& bodyA = *(contact.mBodyA);
		RigidBody& bodyB = *(contact.mBodyB);
		Vector2D normal = contact.mNormal;

		// calculate the relative velocity of two bodies
		Vector2D relativeVelocity = bodyB.mVelocity - bodyA.mVelocity;

		if (ISVector2DDotProduct(relativeVelocity, normal) > 0.f) // moving away
		{
			return;
		}
		// getting the lower restitution
		float e = std::min(bodyA.mRestitution, bodyB.mRestitution);

		// apply formula to calculate impulse
		float j = -(1.f + e) * ISVector2DDotProduct(relativeVelocity, normal);
		j /= bodyA.mInvMass + bodyB.mInvMass;

		Vector2D impulse = j * normal;

		// increment the velocity by impulse force
		bodyA.mVelocity += -impulse * bodyA.mInvMass;
		bodyB.mVelocity += impulse * bodyB.mInvMass;
	}

	// Resolves collisions between two rigid bodies, including rotation effects, by calculating and applying the impulse force to update the velocities and angular velocities of colliding entities.
	void Physics::ResolveCollisionWithRotation(Manifold& contact)
	{
		// init
		RigidBody* bodyA = contact.mBodyA;
		RigidBody* bodyB = contact.mBodyB;
		Vector2D normal = contact.mNormal;
		Vector2D contact1 = contact.mContact1;
		Vector2D contact2 = contact.mContact2;
		int contactCount = contact.mContactCount;

		std::vector<Vector2D> temp_contact_list;
		std::vector<Vector2D> temp_impulse_list;
		std::vector<Vector2D> temp_ra_list;
		std::vector<Vector2D> temp_rb_list;

		temp_contact_list.emplace_back(contact1);
		temp_contact_list.emplace_back(contact2);

		for (int i = 0; i < contactCount; i++)
		{
			temp_impulse_list.emplace_back(Vector2D());
			temp_ra_list.emplace_back(Vector2D());
			temp_rb_list.emplace_back(Vector2D());
		}

		// getting the lower restitution
		float e = std::min(bodyA->mRestitution, bodyB->mRestitution);

		for (int i = 0; i < contactCount; i++)
		{
			// for calculating angular linear velocity in Vector2D
			Vector2D ra = temp_contact_list[i] - bodyA->mBodyTransform.world_position;
			Vector2D rb = temp_contact_list[i] - bodyB->mBodyTransform.world_position;

			temp_ra_list[i] = ra;
			temp_rb_list[i] = rb;

			Vector2D ra_perp = Vector2D(-ra.y, ra.x);
			Vector2D rb_perp = Vector2D(-rb.y, rb.x);

			Vector2D angular_linear_velocityA = ra_perp * bodyA->mAngularVelocity;
			Vector2D angular_linear_velocityB = rb_perp * bodyB->mAngularVelocity;

			Vector2D relative_velocity =
				(bodyB->mVelocity + angular_linear_velocityB) -
				(bodyA->mVelocity + angular_linear_velocityA);

			float contact_velocity_mag = ISVector2DDotProduct(relative_velocity, normal);

			if (contact_velocity_mag > 0.f) // bodys moving away each other
			{
				continue;
			}

			float ra_perp_dotN = ISVector2DDotProduct(ra_perp, normal);
			float rb_perp_dotN = ISVector2DDotProduct(rb_perp, normal);

			// follow the formula
			float denom = bodyA->mInvMass + bodyB->mInvMass +
				(ra_perp_dotN * ra_perp_dotN) * bodyA->mInvInertia +
				(rb_perp_dotN * rb_perp_dotN) * bodyB->mInvInertia;

			float j = -(1.f + e) * contact_velocity_mag;
			j /= denom;
			j /= static_cast<float>(contactCount);

			// save into impulse list
			Vector2D impulse = j * normal;
			temp_impulse_list[i] = impulse;
		}

		for (int i = 0; i < contactCount; i++)
		{
			Vector2D impulse = temp_impulse_list[i];
			Vector2D ra = temp_ra_list[i];
			Vector2D rb = temp_rb_list[i];

			bodyA->mVelocity += -impulse * bodyA->mInvMass;
			bodyA->mAngularVelocity += -ISVector2DCrossProductMag(ra, impulse) * bodyA->mInvInertia;
			bodyB->mVelocity += impulse * bodyB->mInvMass;
			bodyB->mAngularVelocity += ISVector2DCrossProductMag(rb, impulse) * bodyB->mInvInertia;
		}

	}

	// Resolves collisions between two rigid bodies, including rotation and friction effects, by calculating and applying the impulse force to update the velocities, angular velocities, and angular impulses of colliding entities.
	void Physics::ResolveCollisionWithRotationAndFriction(Manifold& contact)
	{
		// init
		RigidBody* bodyA = contact.mBodyA;
		RigidBody* bodyB = contact.mBodyB;
		Vector2D normal = contact.mNormal;
		Vector2D contact1 = contact.mContact1;
		Vector2D contact2 = contact.mContact2;
		int contactCount = contact.mContactCount;

		std::vector<Vector2D> temp_contact_list;
		std::vector<Vector2D> temp_impulse_list;
		std::vector<Vector2D> temp_ra_list;
		std::vector<Vector2D> temp_rb_list;
		std::vector<Vector2D> temp_friction_impulse_list;
		std::vector<float> temp_j_list;

		temp_contact_list.emplace_back(contact1);
		temp_contact_list.emplace_back(contact2);

		for (int i = 0; i < contactCount; i++)
		{
			temp_impulse_list.emplace_back(Vector2D());
			temp_ra_list.emplace_back(Vector2D());
			temp_rb_list.emplace_back(Vector2D());
			temp_friction_impulse_list.emplace_back(Vector2D());
			temp_j_list.emplace_back(0.f);
		}

		// getting the lower restitution
		float e = std::min(bodyA->mRestitution, bodyB->mRestitution);

		float sf = (bodyA->mStaticFriction + bodyB->mStaticFriction) * 0.5f;
		float df = (bodyA->mDynamicFriction + bodyB->mDynamicFriction) * 0.5f;

		// calculation for rotational velocity
		for (int i = 0; i < contactCount; i++)
		{
			// for calculating angular linear velocity in Vector2D
			Vector2D ra = temp_contact_list[i] - bodyA->mBodyTransform.world_position;
			Vector2D rb = temp_contact_list[i] - bodyB->mBodyTransform.world_position;

			temp_ra_list[i] = ra;
			temp_rb_list[i] = rb;

			Vector2D ra_perp = Vector2D(-ra.y, ra.x);
			Vector2D rb_perp = Vector2D(-rb.y, rb.x);

			Vector2D angular_linear_velocityA = ra_perp * bodyA->mAngularVelocity;
			Vector2D angular_linear_velocityB = rb_perp * bodyB->mAngularVelocity;

			Vector2D relative_velocity =
				(bodyB->mVelocity + angular_linear_velocityB) -
				(bodyA->mVelocity + angular_linear_velocityA);

			float contact_velocity_mag = ISVector2DDotProduct(relative_velocity, normal);

			if (contact_velocity_mag > 0.f) // moving away
			{
				continue;
			}

			float ra_perp_dotN = ISVector2DDotProduct(ra_perp, normal);
			float rb_perp_dotN = ISVector2DDotProduct(rb_perp, normal);

			// follow the formula
			float denom = bodyA->mInvMass + bodyB->mInvMass +
				(ra_perp_dotN * ra_perp_dotN) * bodyA->mInvInertia +
				(rb_perp_dotN * rb_perp_dotN) * bodyB->mInvInertia;

			float j = -(1.f + e) * contact_velocity_mag;
			j /= denom;
			j /= static_cast<float>(contactCount);

			// for friction impulse below
			temp_j_list[i] = j;

			Vector2D impulse = j * normal;
			temp_impulse_list[i] = impulse;
		}

		for (int i = 0; i < contactCount; i++)
		{
			Vector2D impulse = temp_impulse_list[i];
			Vector2D ra = temp_ra_list[i];
			Vector2D rb = temp_rb_list[i];

			bodyA->mVelocity += -impulse * bodyA->mInvMass;
			bodyA->mAngularVelocity += -ISVector2DCrossProductMag(ra, impulse) * bodyA->mInvInertia;
			bodyB->mVelocity += impulse * bodyB->mInvMass;
			bodyB->mAngularVelocity += ISVector2DCrossProductMag(rb, impulse) * bodyB->mInvInertia;
		}

		// calculation for static and dynamic friction
		for (int i = 0; i < contactCount; i++)
		{
			Vector2D ra = temp_contact_list[i] - bodyA->mBodyTransform.world_position;
			Vector2D rb = temp_contact_list[i] - bodyB->mBodyTransform.world_position;

			temp_ra_list[i] = ra;
			temp_rb_list[i] = rb;

			Vector2D ra_perp = Vector2D(-ra.y, ra.x);
			Vector2D rb_perp = Vector2D(-rb.y, rb.x);

			Vector2D angular_linear_velocityA = ra_perp * bodyA->mAngularVelocity;
			Vector2D angular_linear_velocityB = rb_perp * bodyB->mAngularVelocity;

			Vector2D relative_velocity =
				(bodyB->mVelocity + angular_linear_velocityB) -
				(bodyA->mVelocity + angular_linear_velocityA);

			Vector2D tangent = relative_velocity - ISVector2DDotProduct(relative_velocity, normal) * normal;

			if (mManifoldInfo.NearlyEqual(tangent, Vector2D()))
			{
				continue; // continue if the friction tangent is close to 0, no friction will be applied
			} else
			{
				ISVector2DNormalize(tangent, tangent);
			}

			float ra_perp_dotT = ISVector2DDotProduct(ra_perp, tangent);
			float rb_perp_dotT = ISVector2DDotProduct(rb_perp, tangent);

			// formula ustage
			float denom = bodyA->mInvMass + bodyB->mInvMass +
				(ra_perp_dotT * ra_perp_dotT) * bodyA->mInvInertia +
				(rb_perp_dotT * rb_perp_dotT) * bodyB->mInvInertia;

			float jt = -ISVector2DDotProduct(relative_velocity, tangent);
			jt /= denom;
			jt /= static_cast<float>(contactCount);

			Vector2D friction_impulse;
			float j = temp_j_list[i];

			// Coulomb's law, clamp the friction to a certain value
			if (std::abs(jt) <= j * sf)
			{
				friction_impulse = jt * tangent;
			} else
			{
				friction_impulse = -j * tangent * df;
			}

			temp_impulse_list[i] = friction_impulse;
		}

		// apply on velocity and angular velocity
		for (int i = 0; i < contactCount; i++)
		{
			Vector2D friction_impulse = temp_impulse_list[i];
			Vector2D ra = temp_ra_list[i];
			Vector2D rb = temp_rb_list[i];

			bodyA->mVelocity += -friction_impulse * bodyA->mInvMass;
			bodyA->mAngularVelocity += -ISVector2DCrossProductMag(ra, friction_impulse) * bodyA->mInvInertia;
			bodyB->mVelocity += friction_impulse * bodyB->mInvMass;
			bodyB->mAngularVelocity += ISVector2DCrossProductMag(rb, friction_impulse) * bodyB->mInvInertia;
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

				//body.mRotation = trans.getRotation();
				body.mRotation += body.mAngularVelocity * time * 15.f;
				trans.setRotation(body.mRotation, body.mAngularVelocity);
				//trans.world_position += body.mVelocity * time;
			}
		}
	}
}

