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
using namespace IS;

namespace IS 
{
	Physics* PHYSICS = NULL;
	
	// Static flag to enable or disable debug drawing of rigidbody shapes
	bool Physics::isDebugDraw = false;

	// Constructs a Physics instance
	Physics::Physics() 
	{
		//ErrorIf(PHYSICS != NULL, "Physics already initialized");
		//PHYSICS = this;
		mGravity = Vector2D(0, -981.f);			// Gravity of the world
		mExertingGravity = false;				// Flag indicating whether gravity is currently exerted
		//isDebugDraw = false;
		mMaxVelocity = 1000.f;					// Maximum velocity for game bodies
		mMinVelocity = -1000.f;					// Minimum velocity for game bodies
		mIterations = 10;						// Number of iterations for physics step
		mContactList = std::vector<Manifold>();
		mContactPointsList = std::vector<Vector2D>();
		//mGrid = Grid();
	}

	// Initializes the physics system
	void Physics::Initialize()
	{
		// Initialization logic here
		Subscribe(MessageType::Collide);
		mGrid.ClearGrid();
	}

	// Updates the physics simulation for the given time step
	void Physics::Update(float dt)
	{
		/*mGrid.ClearGrid();
		for (auto const& entity : mEntities) {
			mGrid.AddIntoCell(entity);
		}*/
		auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
		for (int i = 0; i < Grid::mRows; i++) {
			for (int j = 0; j < Grid::mCols; j++) {
				if (input->IsKeyPressed(GLFW_KEY_3)) {
					std::cout << "[ " << i << "," << j << " ]" << mGrid.mCells[i][j].size() << std::endl;
				}
			}
		}
		
		for (size_t it = 0; it < mIterations; it++) {
			// Performs a physics step for the set of entities with dt, updates velocities and positions for game entities
			Step(dt, mEntities);

			// detect collision through grid cell
			//CellCollisionDetect();
			// 
			// Detects collisions among a set of entities (normal way)
			CollisionDetect(mEntities);
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

	}

	void Physics::CellCollisionDetect() {
		// if the obj inside camera view (to be updated)
		for (int rol = 0; rol < Grid::mRows; rol++) {
			for (int col = 0; col < Grid::mCols; col++) {
				if (mGrid.mCells[rol][col].empty()) {
					continue;
				}
				// a temp list to store all entities with self cell and neighbor cell
				std::set<Entity> cell_list;
				cell_list.clear();

				// update the collision with the residing cell
				mGrid.EmplaceEntity(cell_list, mGrid.mCells[rol][col]);

				// update the collision with neighbor cells
				if (col > 0) {
					// left cell
					//CollisionDetect(mGrid.mCells[rol][col-1]);
					mGrid.EmplaceEntity(cell_list, mGrid.mCells[rol][col - 1]);
					if (rol > 0) {
						// top left cell
						//CollisionDetect(mGrid.mCells[rol-1][col-1]);
						mGrid.EmplaceEntity(cell_list, mGrid.mCells[rol - 1][col - 1]);
					}
					if (rol < Grid::mRows - 1) {
						// bottom left cell
						//CollisionDetect(mGrid.mCells[rol+1][col-1]);
						mGrid.EmplaceEntity(cell_list, mGrid.mCells[rol + 1][col - 1]);
					}
				}
				if (rol > 0) { // up cell
					//CollisionDetect(mGrid.mCells[rol-1][col]);
					mGrid.EmplaceEntity(cell_list, mGrid.mCells[rol - 1][col]);
				}
				if (cell_list.empty()) {
					continue;
				}
				CollisionDetect(cell_list);
				
			}
		}
	}

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

	// Resolves collisions between two rigid bodies by calculating and applying the impulse force to update the velocities of collding entities
	void Physics::ResolveCollision(Manifold & contact) {

		RigidBody& bodyA = contact.mBodyA;
		RigidBody& bodyB = contact.mBodyB;
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
		bodyA.mVelocity -= impulse * bodyA.mInvMass;
		bodyB.mVelocity += impulse * bodyB.mInvMass;
	}

	void Physics::DrawOutLine(RigidBody & body, Sprite const& sprite) {
		for (size_t i = 0; i < body.mTransformedVertices.size(); i++) {
			Vector2D va = body.mTransformedVertices[i];
			Vector2D vb = body.mTransformedVertices[(i + 1) % body.mTransformedVertices.size()]; // modules by the size of the vector to avoid going out of the range
			sprite.drawLine(va, vb);
		}

		for (int i = 0; i < Grid::mRows; i++) {
			for (int j = 0; j < Grid::mCols; j++) {
				/*Vector2D test = { (j * Grid::mCellSize.x + Grid::mCellSize.x / 2) - (WIDTH / 2), (i * Grid::mCellSize.y + Grid::mCellSize.y / 2) - (HEIGHT / 2) };
				Vector2D testnew = { 0.f, 100.f };
				sprite.drawLine(test, test + testnew);*/
				auto [width, height] = InsightEngine::Instance().GetSystem<WindowSystem>("Window")->GetWindowSize();
				Vector2D vertical = { (j * Grid::mCellSize.x) - (width / 2), - (i * Grid::mCellSize.y) + (height / 2) };
				Vector2D verticalend = { 0.f,-(Grid::mCellSize.y)};
				sprite.drawLine(vertical, vertical + verticalend);
				Vector2D hori = { (j * Grid::mCellSize.x) - (width / 2), -(i * Grid::mCellSize.y) + (height / 2) };
				Vector2D horiend = { Grid::mCellSize.x, 0.f };
				sprite.drawLine(hori, hori + horiend);
			}
		}

		// draw the velocity line
		sprite.drawLine(body.mBodyTransform.getWorldPosition(), body.mBodyTransform.getWorldPosition() + body.mVelocity);
	}

	// Updates the gravity vector based on user input
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

	// Updates the debug drawing mode based on user input
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

	// Performs a physics step for the specified time and set of entities, updates velocities and positions for game entities
	void Physics::Step(float time, std::set<Entity> const& entities) {
		// divide by iterations to increase precision
		time /= static_cast<float>(mIterations);

		for (auto const& entity : entities) {
			auto input = InsightEngine::Instance().GetSystem<InputManager>("Input");
			// check input for applying gravity
			UpdateGravity(input);

			// check input for drawing lines
			UpdateDebugDraw(input);

			// check if having rigidbody component
			if (InsightEngine::Instance().HasComponent<RigidBody>(entity)) {
				auto& body = InsightEngine::Instance().GetComponent<RigidBody>(entity);
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				body.BodyFollowTransform(trans);

				/*if (!body.mFirstTransform) {
					std::cout << body.mBodyTransform.world_position.x << std::endl;
					body.mFirstTransform = true;
					mGrid.AddIntoCell(entity);
				}*/
				
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
				trans.world_position.x += body.mVelocity.x * time;
				trans.world_position.y += body.mVelocity.y * time;
				//mGrid.UpdateCell(entity, time);
			}
		}
	}
}

