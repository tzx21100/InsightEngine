
#include "Pch.h"
#include "CollisionSystem.h"

namespace IS
{

	CollisionSystem::CollisionSystem() {
		mContactPair = std::vector<std::pair<Entity, Entity>>();// vector list of each two contact entities
		mManifoldInfo;											// instance of Manifold
		mColliding = false;
		mCollidingCollection.reset();
		mImplicitGrid;											// instance of ImplicitGrid
	}

	void CollisionSystem::Initialize() {
		// Initialization logic here
		Subscribe(MessageType::Collide);

		mImplicitGrid.ClearGrid();
	}

	void CollisionSystem::Update([[maybe_unused]] float dt)
	{
		if (!InsightEngine::Instance().mRuntime)
		{
			// when the game is not running, update collider transform only
			Step();
			return;
		}

		for (int i = 0; i < 1; i++) {
			// empty contact pair before going into collision step
			mContactPair.clear();


			// add new entity inside grid
			mImplicitGrid.AddIntoCell(mEntities);

			// Step update, collider update transform
			Step();

			// Collision Detection
			BroadPhase();

			// Collision Resolution
			NarrowPhase();

			mImplicitGrid.ClearGrid();
		}
	}

	void CollisionSystem::BroadPhase() 
	{
		//CollisionDetect(mEntities);

		// if using implicit grid
		if (Physics::mEnableImplicitGrid)
		{
			// detect collision through Implicit Grid
			ImplicitGridCollisionDetect();
		}
		else
		{ // not using implict grid (Brute Force)
			/*std::vector<Entity> list;
			for (auto const& entity : mEntities)
			{
				list.emplace_back(entity);
			}*/
			CollisionDetect(mEntities);
		}
	}

	void CollisionSystem::NarrowPhase() 
	{

		for (int i = 0; i < mContactPair.size(); i++)
		{
			//if (i == 0) { // reset all the colliders
			//	for (auto entity: mEntities) {
			//		auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
			//		collider.mIsColliding = false;
			//		collider.mCollidedObjectAngle = 0.f;
			//	}
			//}

			std::pair<Entity, Entity> pair = mContactPair[i];
			Entity entityA = pair.first;
			Entity entityB = pair.second;

			//
			BodyType typeA = BodyType::Static;
			BodyType typeB = BodyType::Static;
			RigidBody* contact_bodyA = nullptr;
			RigidBody* contact_bodyB = nullptr;
			if (InsightEngine::Instance().HasComponent<RigidBody>(entityA))
			{
				auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
				typeA = bodyA.mBodyType;
				contact_bodyA = &bodyA;
			}

			if (InsightEngine::Instance().HasComponent<RigidBody>(entityB))
			{
				auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
				typeB = bodyB.mBodyType;
				contact_bodyB = &bodyB;
			}

			//if (contact_bodyA == nullptr && contact_bodyB == nullptr)
			//{
			//	continue; // skip if both entity have no body componet, default both static
			//}

			//if (contact_bodyA != nullptr && contact_bodyB != nullptr)
			//{
			//	auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
			//	auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
			//	if (bodyA.mBodyType == BodyType::Static && bodyB.mBodyType == BodyType::Static)
			//	{
			//		continue; // skip if collision happens between two non dynamic entities
			//	}
			//}

			//
			auto& transA = InsightEngine::Instance().GetComponent<Transform>(entityA);
			auto& transB = InsightEngine::Instance().GetComponent<Transform>(entityB);
			auto& colliderA = InsightEngine::Instance().GetComponent<Collider>(entityA);
			auto& colliderB = InsightEngine::Instance().GetComponent<Collider>(entityB);
			colliderA.UpdateCollider(transA);
			colliderB.UpdateCollider(transB);

			// reset mColliding and collection
			mColliding = false;
			mCollidingCollection.reset();

			// check collide between two colliders, if colliding, mColliding become true and calculate info like normal and depth
			Colliding(colliderA, colliderB);

			// if collider A and collider B colliding
			if (mColliding) {
				colliderA.mIsColliding = true;
				colliderA.mCollidingEntity = entityB;
				colliderB.mIsColliding = true;
				colliderB.mCollidingEntity = entityA;
				
				//for non-response enabled to check for angles of the collided object
				if (!colliderA.mResponseEnable) {
					colliderA.mCollidedObjectAngle = transB.getRotation();
				}

				if (colliderA.mResponseEnable && colliderB.mResponseEnable) {
					// vector of penetration depth to move entities apart
					SeparateColliders(typeA, typeB, transA, transB, mManifoldInfo.mNormal * mManifoldInfo.mDepth);

					// calculate the contact point information
					mManifoldInfo.FindContactPoints(colliderA, colliderB, mCollidingCollection);
					Manifold contact = Manifold(contact_bodyA, contact_bodyB, &colliderA, &colliderB, mManifoldInfo.mNormal, mManifoldInfo.mDepth, mManifoldInfo.mContact1, mManifoldInfo.mContact2, mManifoldInfo.mContactCount);
					//ResolveCollision(contact);
					//ResolveCollisionWithRotation(contact, transA, transB);
					ResolveCollisionWithRotationAndFriction(contact, transA, transB);
				}
			}
		}
	}

	void CollisionSystem::ImplicitGridCollisionDetect()
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

	void CollisionSystem::CollisionDetect(std::set<Entity> const& entities) {

		auto entities_set = std::vector<Entity>(entities.begin(), entities.end());

		for (size_t i = 0; i < entities_set.size(); ++i)
		{
			const Entity& entity_a = entities_set[i];
			if (InsightEngine::Instance().HasComponent<Collider>(entity_a))
			{

				for (size_t j = 0; j < entities_set.size(); ++j)
				{
					const Entity& entity_b = entities_set[j];
					if (entity_a == entity_b)
					{ // if self checking, continue
						continue;
					}

					if (InsightEngine::Instance().HasComponent<Collider>(entity_b))
					{

						if (!InsightEngine::Instance().HasComponent<RigidBody>(entity_a) && !InsightEngine::Instance().HasComponent<RigidBody>(entity_b))
						{
							continue; // continue if both entities have no rigidbody
						}

						if (InsightEngine::Instance().HasComponent<RigidBody>(entity_a) && InsightEngine::Instance().HasComponent<RigidBody>(entity_b))
						{
							auto& collider_a = InsightEngine::Instance().GetComponent<Collider>(entity_a);
							auto& collider_b = InsightEngine::Instance().GetComponent<Collider>(entity_b);
							auto& body_a = InsightEngine::Instance().GetComponent<RigidBody>(entity_a);
							auto& body_b = InsightEngine::Instance().GetComponent<RigidBody>(entity_b);
							if (body_a.mBodyType == BodyType::Static && body_b.mBodyType == BodyType::Static)
							{
								continue; // skip if collision happens between two static entities
							}

							if (!StaticIntersectAABB(collider_a.GetAABB(), collider_b.GetAABB()))
							{
								continue; // static AABB collision check, continue if not colliding
							}
						}
						// save into contact pair to solve the possible contacts
						mContactPair.emplace_back(std::make_pair(entity_a, entity_b));
					}
				}
			}
		}
	}

	void CollisionSystem::CollisionDetect(std::vector<Entity> const& entities)
	{
		for (size_t i = 0; i < entities.size(); ++i)
		{
			const Entity& entity_a = entities[i];
			if (InsightEngine::Instance().HasComponent<Collider>(entity_a))
			{

				for (size_t j = 0; j < entities.size(); ++j)
				{
					const Entity& entity_b = entities[j];
					if (entity_a == entity_b)
					{ // if self checking, continue
						continue;
					}

					if (InsightEngine::Instance().HasComponent<Collider>(entity_b))
					{

						if (!InsightEngine::Instance().HasComponent<RigidBody>(entity_a) && !InsightEngine::Instance().HasComponent<RigidBody>(entity_b))
						{
							continue; // continue if both entities have no rigidbody
						}

						if (InsightEngine::Instance().HasComponent<RigidBody>(entity_a) && InsightEngine::Instance().HasComponent<RigidBody>(entity_b))
						{
							auto& collider_a = InsightEngine::Instance().GetComponent<Collider>(entity_a);
							auto& collider_b = InsightEngine::Instance().GetComponent<Collider>(entity_b);
							auto& body_a = InsightEngine::Instance().GetComponent<RigidBody>(entity_a);
							auto& body_b = InsightEngine::Instance().GetComponent<RigidBody>(entity_b);
							if (body_a.mBodyType == BodyType::Static && body_b.mBodyType == BodyType::Static)
							{
								continue; // skip if collision happens between two static entities
							}

							if (!StaticIntersectAABB(collider_a.GetAABB(), collider_b.GetAABB()))
							{
								continue; // static AABB collision check, continue if not colliding
							}
						}
						// save into contact pair to solve the possible contacts
						mContactPair.emplace_back(std::make_pair(entity_a, entity_b));
					}
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
		RigidBody* bodyA = nullptr;
		RigidBody* bodyB = nullptr;
		Vector2D normal = contact.mNormal;

		if (contact.mBodyA) { // if entity A got rigidbody component
			bodyA = contact.mBodyA;
		}
		else { // entity A has no body, default static
			bodyA = new RigidBody();
			bodyA->mVelocity = Vector2D(0.f, 0.f);
			bodyA->mBodyType = BodyType::Static;
			bodyA->mRestitution = 0.5f;
			bodyA->mInvMass = 0.f;
		}

		if (contact.mBodyB) { // if entity B got rigidbody component
			bodyB = contact.mBodyB;
		}
		else { // entity B has no body component, default static
			bodyB = new RigidBody();
			bodyB->mVelocity = Vector2D(0.f, 0.f);
			bodyB->mBodyType = BodyType::Static;
			bodyB->mRestitution = 0.5f;
			bodyB->mInvMass = 0.f;
		}
		

		// calculate the relative velocity of two bodies
		Vector2D relativeVelocity = bodyB->mVelocity - bodyA->mVelocity;

		if (ISVector2DDotProduct(relativeVelocity, normal) > 0.f) // moving away
		{
			return;
		}
		// getting the lower restitution
		float e = std::min(bodyA->mRestitution, bodyB->mRestitution);

		// apply formula to calculate impulse
		float j = -(1.f + e) * ISVector2DDotProduct(relativeVelocity, normal);
		j /= bodyA->mInvMass + bodyB->mInvMass;

		Vector2D impulse = j * normal;

		// increment the velocity by impulse force
		bodyA->mVelocity += -impulse * bodyA->mInvMass;
		bodyB->mVelocity += impulse * bodyB->mInvMass;
	}

	void CollisionSystem::ResolveCollisionWithRotation(Manifold& contact, Transform const& transA, Transform const& transB)
	{
		// init
		RigidBody* bodyA = nullptr;
		RigidBody* bodyB = nullptr;
		Vector2D normal = contact.mNormal;
		Vector2D contact1 = contact.mContact1;
		Vector2D contact2 = contact.mContact2;
		int contactCount = contact.mContactCount;

		if (contact.mBodyA != nullptr) { // if entity A got rigidbody component
			bodyA = contact.mBodyA;
		}
		else { // entity A has no body, default static
			bodyA = new RigidBody();
			/*bodyA->mVelocity = Vector2D(0.f, 0.f);
			bodyA->mBodyType = BodyType::Static;
			bodyA->mRestitution = 0.5f;
			bodyA->mInvMass = 0.f;*/
			bodyA->CreateStaticBody(transA.world_position, 0.5f);
		}

		if (contact.mBodyB != nullptr) { // if entity B got rigidbody component
			bodyB = contact.mBodyB;
		}
		else { // entity B has no body component, default static
			bodyB = new RigidBody();
			bodyB->CreateStaticBody(transB.world_position, 0.5f);
		}

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
			Vector2D ra = temp_contact_list[i] - bodyA->mPosition;
			Vector2D rb = temp_contact_list[i] - bodyB->mPosition;

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
			// clamp if angular velocity nearly equal to 0
			//bodyA->mVelocity.y = (std::abs(bodyA->mVelocity.y) < 0.13f) ? 0.f : bodyA->mVelocity.y;
			//bodyB->mVelocity.y = (std::abs(bodyB->mVelocity.y) < 0.13f) ? 0.f : bodyB->mVelocity.y;
			//bodyA->mAngularVelocity = (std::abs(bodyA->mAngularVelocity) < 0.13f) ? 0.f : bodyA->mAngularVelocity;
			//bodyB->mAngularVelocity = (std::abs(bodyB->mAngularVelocity) < 0.13f) ? 0.f : bodyB->mAngularVelocity;
		}
	}

	void CollisionSystem::ResolveCollisionWithRotationAndFriction(Manifold& contact, Transform & transA, Transform & transB) 
	{
		
		// init
		RigidBody* bodyA = nullptr;
		RigidBody* bodyB = nullptr;
		Vector2D normal = contact.mNormal;
		Vector2D contact1 = contact.mContact1;
		Vector2D contact2 = contact.mContact2;
		int contactCount = contact.mContactCount;
		/*Vector2D test = { 100.f, 100.f };
		if (contactCount == 1) {
			Sprite::drawDebugLine(contact1, contact1 + test, { 1.f, 0.f, 0.f });
		}
		if (contactCount == 2) {
			Sprite::drawDebugLine(contact1, contact1 + test, { 1.f, 0.f, 0.f });
			Sprite::drawDebugLine(contact2, contact2 + test, { 1.f, 0.f, 0.f });
		}*/

		if (contact.mBodyA) { // if entity A got rigidbody component
			bodyA = contact.mBodyA;
		}
		else { // entity A has no body, default static
			bodyA = new RigidBody();
			bodyA->CreateStaticBody(transA.world_position, 0.5f);
		}

		if (contact.mBodyB) { // if entity B got rigidbody component
			bodyB = contact.mBodyB;
		}
		else { // entity B has no body component, default static
			bodyB = new RigidBody();
			bodyB->CreateStaticBody(transB.world_position, 0.5f);
		}

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
			Vector2D ra = temp_contact_list[i] - bodyA->mPosition;
			Vector2D rb = temp_contact_list[i] - bodyB->mPosition;

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

			if (contact_velocity_mag > 0.f || std::abs(contact_velocity_mag) < 1.f) // moving away
			{
				continue; // continue if moving away or value nearly 0 (1 or 5)
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
			transA.angle_speed = bodyA->mAngularVelocity;
			transB.angle_speed = bodyB->mAngularVelocity;

		}

		// calculation for static and dynamic friction
		for (int i = 0; i < contactCount; i++)
		{
			Vector2D ra_f = temp_contact_list[i] - bodyA->mPosition;
			Vector2D rb_f = temp_contact_list[i] - bodyB->mPosition;

			temp_ra_list[i] = ra_f;
			temp_rb_list[i] = rb_f;

			Vector2D ra_perp_f = { -ra_f.y, ra_f.x };
			Vector2D rb_perp_f = { -rb_f.y, rb_f.x };

			Vector2D angular_linear_velocity_a = ra_perp_f * bodyA->mAngularVelocity;
			Vector2D angular_linear_velocity_b = rb_perp_f * bodyB->mAngularVelocity;

			Vector2D relative_velocity =
				(bodyB->mVelocity + angular_linear_velocity_b) -
				(bodyA->mVelocity + angular_linear_velocity_a);
			
			Vector2D tangent = relative_velocity - ISVector2DDotProduct(relative_velocity, normal) * normal;
			
			if (mManifoldInfo.NearlyEqual(tangent, Vector2D()))
			{
				continue; // continue if the friction tangent is close to 0, no friction will be applied
			}
			else
			{
				ISVector2DNormalize(tangent, tangent);
			}

			float ra_perp_dotT = ISVector2DDotProduct(ra_perp_f, tangent);
			float rb_perp_dotT = ISVector2DDotProduct(rb_perp_f, tangent);

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
			}
			else
			{
				friction_impulse = -j * tangent * df;
			}

			temp_friction_impulse_list[i] = friction_impulse;
		}

		// apply on velocity and angular velocity
		for (int i = 0; i < contactCount; i++)
		{
			Vector2D friction_impulse = temp_friction_impulse_list[i];
			Vector2D ra = temp_ra_list[i];
			Vector2D rb = temp_rb_list[i];

			bodyA->mVelocity += -friction_impulse * bodyA->mInvMass;
			bodyA->mAngularVelocity += -ISVector2DCrossProductMag(ra, friction_impulse) * bodyA->mInvInertia;
			bodyB->mVelocity += friction_impulse * bodyB->mInvMass;
			bodyB->mAngularVelocity += ISVector2DCrossProductMag(rb, friction_impulse) * bodyB->mInvInertia;
			transA.angle_speed = bodyA->mAngularVelocity;
			transB.angle_speed = bodyB->mAngularVelocity;
		}
	}

	void CollisionSystem::Colliding(Collider& collider_a, Collider& collider_b) {
		bool box_box_colliding, box_circle_colliding, circle_box_colliding, circle_circle_colliding;
		box_box_colliding = box_circle_colliding = circle_box_colliding = circle_circle_colliding = false;
		Vector2D normal = Vector2D();
		float depth = 0.f;
		// box collider check
		if (collider_a.IsBoxColliderEnable()) {
			if (collider_b.IsBoxColliderEnable()) { // box vs box
				box_box_colliding = IntersectionPolygons(collider_a.mBoxCollider.transformedVertices, collider_a.mBoxCollider.center, collider_b.mBoxCollider.transformedVertices, collider_b.mBoxCollider.center, normal, depth);
				if (box_box_colliding) { SavingCollidingData(CollidingStatus::BOX_A_BOX_B, normal, depth); }
			}
			if (collider_b.IsCircleColliderEnable()) { // box vs circle
				box_circle_colliding = IntersectionCirlcecPolygon(collider_b.mCircleCollider.center, collider_b.mCircleCollider.radius, collider_a.mBoxCollider.center, collider_a.mBoxCollider.transformedVertices, normal, depth);
				mManifoldInfo.mNormal *= -1; // to be fixed
				if (box_circle_colliding) { SavingCollidingData(CollidingStatus::BOX_A_CIRCLE_B, normal, depth); }
			}
		}

		// circle collider check
		if (collider_a.IsCircleColliderEnable()) {
			if (collider_b.IsBoxColliderEnable()) { // circle vs box
				circle_box_colliding = IntersectionCirlcecPolygon(collider_a.mCircleCollider.center, collider_a.mCircleCollider.radius, collider_b.mBoxCollider.center, collider_b.mBoxCollider.transformedVertices, normal, depth);
				if (circle_box_colliding) { SavingCollidingData(CollidingStatus::CIRCLE_A_BOX_B, normal, depth); }
			}
			if (collider_b.IsCircleColliderEnable()) { // circle vs circle
				circle_circle_colliding = IntersectionCircles(collider_a.mCircleCollider.center, collider_a.mCircleCollider.radius, collider_b.mCircleCollider.center, collider_b.mCircleCollider.radius, normal, depth);
				if (circle_circle_colliding) { SavingCollidingData(CollidingStatus::CIRCLE_A_CIRCLE_B, normal, depth); }
			}
		}
		mColliding = (box_box_colliding || box_circle_colliding || circle_box_colliding || circle_circle_colliding);
	}
#if 0
	bool CollisionSystem::CheckCollide(Entity& entity) {
		for(){
			if (InsightEngine::Instance().HasComponent<Collider>(entityA)) {}

			bool result = false;

			Vector2D normal = Vector2D();
			float depth = 0.f;

			// box collider check
			if (collider_a.IsBoxColliderEnable()) {
				if (collider_b.IsBoxColliderEnable()) { // box vs box
					result = IntersectionPolygons(collider_a.mBoxCollider.transformedVertices, collider_a.mBoxCollider.center, collider_b.mBoxCollider.transformedVertices, collider_b.mBoxCollider.center, normal, depth);
					//mCollidingCollection.set(CollidingStatus::BOX_A_BOX_B);
				}
				if (collider_b.IsCircleColliderEnable()) { // box vs circle
					result = IntersectionCirlcecPolygon(collider_b.mCircleCollider.center, collider_b.mCircleCollider.radius, collider_a.mBoxCollider.center, collider_a.mBoxCollider.transformedVertices, normal, depth);
					//mManifoldInfo.mNormal *= -1; // to be fixed
					//mCollidingCollection.set(CollidingStatus::BOX_A_CIRCLE_B);
				}
			}

			// circle collider check
			if (collider_a.IsCircleColliderEnable()) {
				if (collider_b.IsBoxColliderEnable()) { // circle vs box
					result = IntersectionCirlcecPolygon(collider_a.mCircleCollider.center, collider_a.mCircleCollider.radius, collider_b.mBoxCollider.center, collider_b.mBoxCollider.transformedVertices, normal, depth);
					//mCollidingCollection.set(CollidingStatus::CIRCLE_A_BOX_B);
				}
				if (collider_b.IsCircleColliderEnable()) { // circle vs circle
					result = IntersectionCircles(collider_a.mCircleCollider.center, collider_a.mCircleCollider.radius, collider_b.mCircleCollider.center, collider_b.mCircleCollider.radius, normal, depth);
					//mCollidingCollection.set(CollidingStatus::CIRCLE_A_CIRCLE_B);
				}
			}
		}
		return result;
	}
#endif

	bool CollisionSystem::CheckColliding(Entity entity) {
		if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
			auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
			return collider.mIsColliding;
		}
		else {
			return false;
		}
	}

	void CollisionSystem::SavingCollidingData(short colliding_status, Vector2D const& normal, float depth)
	{
		mCollidingCollection.set(colliding_status);
		mManifoldInfo.mNormal = normal;
		mManifoldInfo.mDepth = depth;
	}

	void CollisionSystem::Step() {
		for (auto const& entity : mEntities) {
			if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
				collider.UpdateCollider(trans);
				// reset some attributes
				collider.mIsColliding = false;
				collider.mCollidedObjectAngle = 0.f;
				collider.mCollidingEntity = MAX_ENTITIES + 1; // no colliding entity
			}
		}
	}

}