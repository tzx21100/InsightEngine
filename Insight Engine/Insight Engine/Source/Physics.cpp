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
#include "CoreEngine.h"
#include "Sprite.h"

namespace IS {

	bool Physics::mShowColliders = false;
	bool Physics::mShowVelocity = false;
	bool Physics::mShowGrid = false;
	bool Physics::mExertingGravity = true;				// Flag indicating whether gravity is currently exerted
	std::vector<Vector2D> Physics::mContactPointsList = std::vector<Vector2D>();

	// Constructs a Physics instance
	Physics::Physics() 
	{
		//mNormal = Vector2D();
		//mDepth = std::numeric_limits<float>::max();
		mGravity = Vector2D(0, -981.f);			// Gravity of the world
		mMaxVelocity = 1000.f;					// Maximum velocity for game bodies
		mMinVelocity = -1000.f;					// Minimum velocity for game bodies
		mCurrentIterations = 0;					// Number of current iterations for physics step
		mTotalIterations = 20;						// Number of iterations for physics step
		mContactList = std::vector<Manifold>();
		
		mManifoldInfo;
		mImplicitGrid;
	}

	// Initializes the physics system
	void Physics::Initialize()
	{
		// Initialization logic here
		Subscribe(MessageType::Collide);
		//mGrid.ClearGrid();
		mImplicitGrid.ClearGrid();
	}

	// Updates the physics simulation for the given time step
	void Physics::Update(float dt)
	{
		if (InsightEngine::Instance().mRuntime == false) { return; }
		mContactPointsList.clear();

		// add new entity inside grid
		mImplicitGrid.AddIntoCell(mEntities);
		
		// for testing grid
		auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
		if (input->IsKeyPressed(GLFW_KEY_3)) {
			for (int i = 0; i < ImplicitGrid::mRows; i++) {
				std::cout << "[ " << i << " row ]" << mImplicitGrid.mRowsBitArray[i].count() << std::endl;
					
			}
			for (int j = 0; j < ImplicitGrid::mCols; j++) {
				std::cout << "[ " << j << " col ]" << mImplicitGrid.mColsBitArray[j].count() << std::endl;
			}
		}

		// physics update iteration
		for (; mCurrentIterations < mTotalIterations; mCurrentIterations++) {
			// empty contact list before going into collision step
			mContactList.clear();
			
			// Performs a physics step for the set of entities with dt, updates velocities and positions for game entities
			Step(dt, mEntities);

			BroadPhase();
			NarrowPhase();			
			
			// to do
			// add collision detect outside of the camera view

			// Detects collisions among a set of entities (normal way)
			//CollisionDetect(mEntities);
			//std::set<Entity> cell_list;
			//cell_list.clear();
			//for (auto const& entity : mEntities) {
			//	if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
			//		auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
			//		if (!(body.mIsInGrid)) {
			//			// emplace all the entities outside of the grid
			//			cell_list.emplace(entity);
			//		}
			//	}
			//}
			//if (!(cell_list.empty())) {
			//	CollisionDetect(cell_list);
			//}
		}

		// set it back to 0 for next iteration loop
		mCurrentIterations = 0;

	}
#if 0
	// useless, uniform grid
	void Physics::CellCollisionDetect() {
		// if the obj inside camera view (to be updated)
		for (int row = 0; row < Grid::mRows; row++) {
			for (int col = 0; col < Grid::mCols; col++) {
				if (mGrid.mCells[row][col].empty()) {
					continue;
				}
				// a temp list to store all entities with self cell and neighbor cell
				std::set<Entity> cell_list;
				cell_list.clear();

				// update the collision with the residing cell
				mGrid.EmplaceEntity(cell_list, mGrid.mCells[row][col]);

				// update the collision with neighbor cells
				if (col > 0) {
					// left cell
					//CollisionDetect(mGrid.mCells[row][col-1]);
					mGrid.EmplaceEntity(cell_list, mGrid.mCells[row][col - 1]);
					if (row > 0) {
						// top left cell
						//CollisionDetect(mGrid.mCells[row-1][col-1]);
						mGrid.EmplaceEntity(cell_list, mGrid.mCells[row - 1][col - 1]);
					}
					if (row < Grid::mRows - 1) {
						// bottom left cell
						//CollisionDetect(mGrid.mCells[row+1][col-1]);
						mGrid.EmplaceEntity(cell_list, mGrid.mCells[row + 1][col - 1]);
					}
				}
				if (row > 0) { // up cell
					//CollisionDetect(mGrid.mCells[row-1][col]);
					mGrid.EmplaceEntity(cell_list, mGrid.mCells[row - 1][col]);
				}
				if (cell_list.empty()) {
					continue;
				}
				CollisionDetect(cell_list);
				
			}
		}
	}
#endif
	// collision detect for implicit grid
	void Physics::ImplicitGridCollisionDetect() {
		// loop through each row with each col
		for (int row = 0; row < ImplicitGrid::mRows; row++) {
			for (int col = 0; col < ImplicitGrid::mCols; col++) {
				// check the existence of the objects in a cell, simply perform a bitwise AND operation
				std::bitset<MAX_ENTITIES> test_cell = mImplicitGrid.mRowsBitArray[row] & mImplicitGrid.mColsBitArray[col];
				// getting entity number in this cell
				size_t totalEntities = test_cell.count();
				mImplicitGrid.mInGridList.resize(totalEntities);
				if (totalEntities > 1) {// more than 1 entity to avoid self checking
					// emplace all the entities in current cell
					for (Entity e = 0; e < InsightEngine::Instance().EntitiesAlive(); ++e) {
						if (test_cell.test(e)) { // if the current bit entity is true
							// emplace into InGridList
							mImplicitGrid.mInGridList.emplace_back(e);
							totalEntities--;
							if (totalEntities <= 0) { 
								// if there is no more entities in this cell, break
								break;
							}
						}
					}

					// if the cell is at the edge of the grid, then need check with the entities overlap on the grid
					if (row == 0 || row == ImplicitGrid::mRows - 1 ||
						col == 0 || col == ImplicitGrid::mCols - 1) {
						for (auto const& entity : mImplicitGrid.mOverlapGridList) {
							mImplicitGrid.mInGridList.emplace_back(entity);
						}
					}

					/*for (auto const& entity : mImplicitGrid.mOverlapGridList) {
						mImplicitGrid.mInGridList.emplace_back(entity);
					}*/

					CollisionDetect(mImplicitGrid.mInGridList);
					// empty the list
					mImplicitGrid.mInGridList.clear();
					//std::vector<Entity>().swap(mImplicitGrid.mInGridList);
					
				}
			}
		}
	}


	void Physics::CollisionDetect(std::vector<Entity> const& entities) {

		// Loop through all Entities registered by the System
		for (size_t i = 0; i < entities.size() - 1; ++i) {
			const Entity& entityA = entities[i];

			// no need check HasComponent as AddIntoCell function did
			auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
			auto& transA = InsightEngine::Instance().GetComponent<Transform>(entityA);
			bodyA.BodyFollowTransform(transA);

			for (size_t j = i + 1; j < entities.size(); ++j) {
				const Entity& entityB = entities[j];

				if (entityA == entityB) {
					continue;
				}

				// no need check HasComponent as AddIntoCell function did
				auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
				auto& transB = InsightEngine::Instance().GetComponent<Transform>(entityB);

				if (bodyA.mBodyType != BodyType::Dynamic && bodyB.mBodyType != BodyType::Dynamic) {
					// continue if collision happens between two non dynamic entities
					continue;
				}

				bodyB.BodyFollowTransform(transB);

				// static AABB collision check, continue to the next loop if not colliding
				if (!StaticIntersectAABB(bodyA.GetAABB(), bodyB.GetAABB()))
				{
					continue;
				}

				bool isColliding = false;
				switch (bodyA.mBodyShape)
				{
				case BodyShape::Box:
					switch (bodyB.mBodyShape)
					{
						// box vs box
					case BodyShape::Box:
						isColliding = IntersectionPolygons(bodyA.GetTransformedVertices(), bodyA.mBodyTransform.getWorldPosition(), bodyB.GetTransformedVertices(), bodyB.mBodyTransform.getWorldPosition(), mManifoldInfo.mNormal, mManifoldInfo.mDepth);
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
				if (isColliding) {

					// vector of penetration depth to move entities apart
					SeparateBodies(&bodyA, &bodyB, &transA, &transB, mManifoldInfo.mNormal * mManifoldInfo.mDepth);

					mManifoldInfo.FindContactPoints(bodyA, bodyB);
					Manifold contact = Manifold(&bodyA, &bodyB, mManifoldInfo.mNormal, mManifoldInfo.mDepth, mManifoldInfo.mContact1, mManifoldInfo.mContact2, mManifoldInfo.mContactCount);
					mContactList.emplace_back(contact);
				}
				else { //not colliding

				}
			}
		}
	}

	void Physics::BroadPhase() {
		// detect collision through Implicit Grid
		ImplicitGridCollisionDetect();
	}

	void Physics::NarrowPhase() {
		for (int i = 0; i < mContactList.size(); i++) {
			Manifold contact = mContactList[i];
			//ResolveCollision(contact);
			ResolveCollisionWithRotation(contact);

			// add contact points only at the last iteration
			if (mCurrentIterations == mTotalIterations - 1) {
				// emplace the contact point
				if (std::find(mContactPointsList.begin(), mContactPointsList.end(), contact.mContact1) == mContactPointsList.end()) {
					// if first contact point not found in the list, emplace it
					mContactPointsList.emplace_back(contact.mContact1);
				}

				if (contact.mContactCount > 1) { // if more than 1 contact point
					if (std::find(mContactPointsList.begin(), mContactPointsList.end(), contact.mContact2) == mContactPointsList.end()) {
						// emplace second contact point
						mContactPointsList.emplace_back(contact.mContact2);
					}
				}
			}
		}
	}
#if 0
	// Detects collisions among a set of entities, running different collision detect function form collision.h based on the body shape (box, circle or line)
	void Physics::CollisionDetect(std::set<Entity> const& entities) {
		mContactList.clear();
		//loops through all Entities registered by the System this mEntities map
		for (std::set<Entity>::iterator entityA = entities.begin(); entityA != std::prev(entities.end(), 1); ++entityA) {
			if (InsightEngine::Instance().HasComponent<RigidBody>(*entityA)) { // if entityA has rigidbody component
				//getting rigidbody component for each entity
				auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(*entityA);
				auto& transA = InsightEngine::Instance().GetComponent<Transform>(*entityA);
				//auto& spriteA = InsightEngine::Instance().GetComponent<Sprite>(entityA);
				bodyA.BodyFollowTransform(transA);

				for (std::set<Entity>::iterator entityB = std::next(entityA, 1); entityB != entities.end(); ++entityB) {
					if (entityA == entityB) { // if self checking, continue
						continue;
					}
					if (InsightEngine::Instance().HasComponent<RigidBody>(*entityB)) { // if entityB has rigidbody component
						auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(*entityB);
						auto& transB = InsightEngine::Instance().GetComponent<Transform>(*entityB);
						//auto& spriteB = InsightEngine::Instance().GetComponent<Sprite>(entityB);
						bodyB.BodyFollowTransform(transB);
						// for calculating collision response
						Vector2D normal = Vector2D();
						float depth = std::numeric_limits<float>::max();
						bool isColliding = false;

						// static AABB collision check, continue to the next loop if not colliding
						if (!StaticIntersectAABB(bodyA.GetAABB(), bodyB.GetAABB()))
						{
							continue;
						}

						switch (bodyA.mBodyShape)
						{
						case BodyShape::Box:
							switch (bodyB.mBodyShape)
							{
								// box vs box
							case BodyShape::Box:
								isColliding = IntersectionPolygons(bodyA.GetTransformedVertices(), bodyA.mBodyTransform.getWorldPosition(), bodyB.GetTransformedVertices(), bodyB.mBodyTransform.getWorldPosition(), normal, depth);
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
						if (isColliding) {
							// vector of penetration
							Vector2D vec = normal * depth;
							// if body A is static 
							if (bodyA.mBodyType != BodyType::Dynamic)
							{
								transB.Move(vec);
								// to be optimize
								bodyB.mState = BodyState::GROUNDED;
							}
							// if body B is static 
							else if (bodyB.mBodyType != BodyType::Dynamic)
							{
								transA.Move(-vec);
								// to be optimize
								bodyA.mState = BodyState::GROUNDED;
							}
							else // both are dynamic
							{
								transA.Move(-vec / 2.f);
								transB.Move(vec / 2.f);
							}
							//spriteA.color = glm::vec3(1.f, 0.f, 1.f);
							//ResolveCollision(bodyA, bodyB, normal, depth);

							Manifold contact = Manifold(bodyA, bodyB, normal, depth, Vector2D(), Vector2D(), 0);
							mContactList.emplace_back(contact);
						}
						else { //not colliding
							//spriteB.color = glm::vec3(0.f, 0.f, 1.f);
						}
					}

				}
			}
		}

		for (int i = 0; i < mContactList.size(); i++) {
			Manifold contact = mContactList[i];
			ResolveCollision(contact);

			/*if (contact.ContactCount > 0) {
				ContactPointsList.push_back(contact.Contact1);

				if (contact.ContactCount > 1) {
					ContactPointsList.push_back(contact.Contact2);
				}
			}*/
		}
	}

	
#endif

	void Physics::SeparateBodies(RigidBody* bodyA, RigidBody* bodyB, Transform* transA, Transform* transB, Vector2D const& vec) {
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
		}
		else // both are dynamic
		{
			transA->Move(-vec / 2.f);
			transB->Move(vec / 2.f);
		}
	}

	// Resolves collisions between two rigid bodies by calculating and applying the impulse force to update the velocities of collding entities
	void Physics::ResolveCollision(Manifold & contact) {

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

	void Physics::ResolveCollisionWithRotation(Manifold& contact) {
		// init
		RigidBody* bodyA = contact.mBodyA;
		RigidBody* bodyB = contact.mBodyB;
		Vector2D normal = contact.mNormal;
		Vector2D contact1 = contact.mContact1;
		Vector2D contact2 = contact.mContact2;
		int contactCount = contact.mContactCount;

		// getting the lower restitution
		float e = std::min(bodyA->mRestitution, bodyB->mRestitution);

		std::vector<Vector2D> temp_contact_list;
		std::vector<Vector2D> temp_impulse_list;
		std::vector<Vector2D> temp_ra_list;
		std::vector<Vector2D> temp_rb_list;

		temp_contact_list.emplace_back(contact1);
		temp_contact_list.emplace_back(contact2);

		for (int i = 0; i < contactCount; i++) {
			temp_impulse_list.emplace_back(Vector2D());
			temp_ra_list.emplace_back(Vector2D());
			temp_rb_list.emplace_back(Vector2D());
		}

		for (int i = 0; i < contactCount; i++) {
			Vector2D ra = temp_contact_list[i] - bodyA->mBodyTransform.world_position;
			Vector2D rb = temp_contact_list[i] - bodyB->mBodyTransform.world_position;

			temp_ra_list[i] = ra;
			temp_rb_list[i] = rb;

			Vector2D raPerp = Vector2D(-ra.y, ra.x);
			Vector2D rbPerp = Vector2D(-rb.y, rb.x);

			Vector2D angularLinearVelocityA = raPerp * bodyA->mAngularVelocity;
			Vector2D angularLinearVelocityB = rbPerp * bodyB->mAngularVelocity;

			Vector2D relativeVelocity =
				(bodyB->mVelocity + angularLinearVelocityB) -
				(bodyA->mVelocity + angularLinearVelocityA);

			float contactVelocityMag = ISVector2DDotProduct(relativeVelocity, normal);

			if (contactVelocityMag > 0.f)
			{
				continue;
			}

			float raPerpDotN = ISVector2DDotProduct(raPerp, normal);
			float rbPerpDotN = ISVector2DDotProduct(rbPerp, normal);

			float denom = bodyA->mInvMass + bodyB->mInvMass +
				(raPerpDotN * raPerpDotN) * (1.f / bodyA->mInertia) +
				(rbPerpDotN * rbPerpDotN) * (1.f / bodyB->mInertia);

			float j = -(1.f + e) * contactVelocityMag;
			j /= denom;
			j /= (float)contactCount;

			Vector2D impulse = j * normal;
			temp_impulse_list[i] = impulse;
		}

		for (int i = 0; i < contactCount; i++)
		{
			Vector2D impulse = temp_impulse_list[i];
			Vector2D ra = temp_ra_list[i];
			Vector2D rb = temp_rb_list[i];

			bodyA->mVelocity += -impulse * bodyA->mInvMass;
			bodyA->mAngularVelocity += -ISVector2DCrossProductMag(ra, impulse) * (1.f / bodyA->mInertia);
			bodyB->mVelocity += impulse * bodyB->mInvMass;
			bodyB->mAngularVelocity += ISVector2DCrossProductMag(rb, impulse) * (1.f / bodyB->mInertia);
		}

	}

	void Physics::DrawOutLine(RigidBody& body, Sprite const& sprite) {
		// draw colliders in green
		if (mShowColliders) {
			for (size_t i = 0; i < body.mTransformedVertices.size(); i++) {
				Vector2D va = body.mTransformedVertices[i];
				Vector2D vb = body.mTransformedVertices[(i + 1) % body.mTransformedVertices.size()]; // modules by the size of the vector to avoid going out of the range
				sprite.drawLine(va, vb, { 0.f, 1.f, 0.f });
			}

			/*for (int i = 0; i < mContactPointsList.size(); i++) {
				Vector2D end = { 100.f, 100.f };
				sprite.drawLine(mContactPointsList[i], mContactPointsList[i] + end, { 1.f, 1.f, 1.f });
			}*/
		}

		// draw grid cell line in white
		if (mShowGrid)
			//Grid::DrawGrid(sprite);
			ImplicitGrid::DrawGrid(sprite);

		// draw the velocity line in blue
		if (mShowVelocity)
			sprite.drawLine(body.mBodyTransform.getWorldPosition(), body.mBodyTransform.getWorldPosition() + body.mVelocity, { 0.f, 0.f, 1.f });
	}

	// Updates the gravity vector based on user input
	//void Physics::UpdateGravity(auto const& key_input) {
	//	if (key_input->IsKeyPressed(GLFW_KEY_G)) {
	//		mExertingGravity = true;
	//		IS_CORE_DEBUG("mGravity Enabled!");
	//	}
	//	else if (key_input->IsKeyPressed(GLFW_KEY_F)) {
	//		mExertingGravity = false;
	//		IS_CORE_DEBUG("mGravity Disabled!");
	//	}
	//}

	// Performs a physics step for the specified time and set of entities, updates velocities and positions for game entities
	void Physics::Step(float time, std::set<Entity> const& entities) {
		// divide by iterations to increase precision
		time /= static_cast<float>(mTotalIterations);

		for (auto const& entity : entities) {
			auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
			// check input for applying gravity
			//UpdateGravity(input);

			// check if having rigidbody component
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
				auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				body.BodyFollowTransform(trans);
				
				// if the entity is static or kinematic, skip
				if (body.mBodyType != BodyType::Dynamic) {
					continue;
				}
				//freeze
				if (InsightEngine::Instance().mFreezeFrame) {
					if (!InsightEngine::Instance().mContinueFrame)
						return;
				}
				
				// update the velocity if gravity exists
				if (mExertingGravity) {
					body.mAcceleration = body.mForce * body.mInvMass + mGravity;
					body.mVelocity += body.mAcceleration * time;
					//body.mVelocity += mGravity * time;
				}

				// set range
				body.mVelocity.x = std::min(body.mVelocity.x, mMaxVelocity);
				body.mVelocity.y = std::min(body.mVelocity.y, mMaxVelocity);
				body.mVelocity.x = std::max(body.mVelocity.x, mMinVelocity);
				body.mVelocity.y = std::max(body.mVelocity.y, mMinVelocity);
				// update position
				mImplicitGrid.UpdateCell(entity, time);
				//trans.world_position += body.mVelocity * time;
				//mGrid.UpdateCell(entity, time);
			}
		}
	}
}

