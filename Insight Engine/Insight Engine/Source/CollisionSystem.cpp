
#include "Pch.h"
#include "CollisionSystem.h"

namespace IS
{

	CollisionSystem::CollisionSystem() {
		mContactPair = std::vector<std::pair<Entity, Entity>>();// vector list of each two contact entities
		mManifoldInfo;
		mColliding = false;
		mCollidingCollection.reset();
	}

	void CollisionSystem::Initialize() {

	}

	void CollisionSystem::Update([[maybe_unused]] float dt)
	{
		for (int i = 0; i < 1; i++) {
			// empty contact pair before going into collision step
			mContactPair.clear();

			// Step update, collider update transform
			Step();

			// Collision Detection
			BroadPhase();

			// Collision Resolution
			NarrowPhase();
		}
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
			colliderA.UpdateCollider(transA);
			colliderB.UpdateCollider(transB);

			mColliding = false;
			// collection of all possible collision happens between two colliders
			mCollidingCollection.reset();
			// check collide between two colliders, if colliding, calculate info like normal and depth
			Colliding(colliderA, colliderB);

			// if collider A and collider B colliding
			if (mColliding) {
				colliderA.mIsColliding = true;
				colliderB.mIsColliding = true;
				BodyType typeA = BodyType::Static;
				BodyType typeB = BodyType::Static;
				RigidBody* contact_bodyA = nullptr;

				RigidBody* contact_bodyB = nullptr;
				if (InsightEngine::Instance().HasComponent<RigidBody>(entityA)) {
					auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
					typeA = bodyA.mBodyType;
					contact_bodyA = &bodyA;
				}

				if (InsightEngine::Instance().HasComponent<RigidBody>(entityB)) {
					auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);
					typeB = bodyB.mBodyType;
					contact_bodyB = &bodyB;
				}

				// vector of penetration depth to move entities apart
				SeparateColliders(typeA, typeB, transA, transB, mManifoldInfo.mNormal * mManifoldInfo.mDepth);

				// calculate the contact point information
				mManifoldInfo.FindContactPoints(colliderA, colliderB, mCollidingCollection);
				Manifold contact = Manifold(contact_bodyA, contact_bodyB, &colliderA, &colliderB, mManifoldInfo.mNormal, mManifoldInfo.mDepth, mManifoldInfo.mContact1, mManifoldInfo.mContact2, mManifoldInfo.mContactCount);
				ResolveCollision(contact);
				//ResolveCollisionWithRotation(contact, transA, transB);

			}
			else {
				colliderA.mIsColliding = false;
				colliderB.mIsColliding = false;
			}
		}
	}

	void CollisionSystem::CollisionDetect(std::set<Entity> const& entities) {
		for (auto const& entityA : entities)
		{
			if (InsightEngine::Instance().HasComponent<Collider>(entityA)) {

				for (auto const& entityB : entities) {

					if (entityA == entityB) { // if self checking, continue
						continue;
					}

					if (InsightEngine::Instance().HasComponent<Collider>(entityB)) {
						//auto& transA = InsightEngine::Instance().GetComponent<Transform>(entityA);
						//auto& transB = InsightEngine::Instance().GetComponent<Transform>(entityB);
						//auto& colliderA = InsightEngine::Instance().GetComponent<Collider>(entityA);
						//auto& colliderB = InsightEngine::Instance().GetComponent<Collider>(entityB);
						
						//auto& bodyA = InsightEngine::Instance().GetComponent<RigidBody>(entityA);
						//auto& bodyB = InsightEngine::Instance().GetComponent<RigidBody>(entityB);

						//if (bodyA.mBodyType != BodyType::Dynamic && bodyB.mBodyType != BodyType::Dynamic)
						//{
						//	// continue if collision happens between two non dynamic entities
						//	continue;
						//}

						if (!InsightEngine::Instance().HasComponent<RigidBody>(entityA) && !InsightEngine::Instance().HasComponent<RigidBody>(entityB)) {
							continue; // continue if both entities have no rigidbody
						}

						mContactPair.emplace_back(std::make_pair(entityA, entityB));

						
					}
				}

				//for (bits that are set in collider)
				//{
				//	// update respective collider
				//	//somecollider.update(trans.world_position);
				//}

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

	void CollisionSystem::Colliding(Collider& collider_a, Collider& collider_b) {
		// box collider check
		if (collider_a.IsBoxColliderEnable()) {
			if (collider_b.IsBoxColliderEnable()) { // box vs box
				mColliding = IntersectionPolygons(collider_a.mBoxCollider.transformedVertices, collider_a.mBoxCollider.center, collider_b.mBoxCollider.transformedVertices, collider_b.mBoxCollider.center, mManifoldInfo.mNormal, mManifoldInfo.mDepth);
				mCollidingCollection.set(CollidingStatus::BOX_A_BOX_B);
			}
			if (collider_b.IsCircleColliderEnable()) { // box vs circle
				mColliding = IntersectionCirlcecPolygon(collider_b.mCircleCollider.center, collider_b.mCircleCollider.radius, collider_a.mBoxCollider.center, collider_a.mBoxCollider.transformedVertices, mManifoldInfo.mNormal, mManifoldInfo.mDepth);
				mManifoldInfo.mNormal *= -1; // to be fixed
				mCollidingCollection.set(CollidingStatus::BOX_A_CIRCLE_B);
			}
		}

		// circle collider check
		if (collider_a.IsCircleColliderEnable()) {
			if (collider_b.IsBoxColliderEnable()) { // circle vs box
				mColliding = IntersectionCirlcecPolygon(collider_a.mCircleCollider.center, collider_a.mCircleCollider.radius, collider_b.mBoxCollider.center, collider_b.mBoxCollider.transformedVertices, mManifoldInfo.mNormal, mManifoldInfo.mDepth);
				mCollidingCollection.set(CollidingStatus::CIRCLE_A_BOX_B);
			}
			if (collider_b.IsCircleColliderEnable()) { // circle vs circle
				mColliding = IntersectionCircles(collider_a.mCircleCollider.center, collider_a.mCircleCollider.radius, collider_b.mCircleCollider.center, collider_b.mCircleCollider.radius, mManifoldInfo.mNormal, mManifoldInfo.mDepth);
				mCollidingCollection.set(CollidingStatus::CIRCLE_A_CIRCLE_B);
			}
		}
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

	bool CheckColliding(Entity entity) {
		if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
			auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
			return collider.mIsColliding;
		}
		else {
			return false;
		}
	}

	void CollisionSystem::Step() {
		for (auto const& entity : mEntities) {
			if (InsightEngine::Instance().HasComponent<Collider>(entity)) {
				auto& trans = InsightEngine::Instance().GetComponent<Transform>(entity);
				auto& collider = InsightEngine::Instance().GetComponent<Collider>(entity);
				collider.UpdateCollider(trans);
				/*if (InsightEngine::Instance().HasComponent<Sprite>(entity))
				{
					InsightEngine::Instance().GetComponent<Sprite>(entity).drawDebugCircle(trans.world_position,
						{ collider.mCircleCollider.radius, collider.mCircleCollider.radius }, { 1.f, 0.f, 0.f });
				}*/
			}
		}
	}

}