
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_SYSTEM_H

/*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Pch.h"

namespace IS
{
	enum CollidingStatus {
		BOX_A_BOX_B = 1,
		BOX_A_CIRCLE_B,
		CIRCLE_A_BOX_B,
		CIRCLE_A_CIRCLE_B
	};

	class CollisionSystem : public ParentSystem
	{
	public:
		CollisionSystem();

		void Update(float dt) override;

		std::string GetName() override { return "CollisionSystem"; }

		void Initialize() override;

		void HandleMessage(const Message&) override {}

		~CollisionSystem() {}

		void Step();

		void BroadPhase();

		void NarrowPhase();

		void CollisionDetect(std::vector<Entity> const& entities);
		void CollisionDetect(std::set<Entity> const& entities);

		void SeparateColliders(BodyType typeA, BodyType typeB, Transform& transA, Transform& transB, Vector2D const& vec);
		
		void ResolveCollision(Manifold& contact);
		void ResolveCollisionWithRotation(Manifold& contact, Transform const& transA, Transform const& transB);
		void ResolveCollisionWithRotationAndFriction(Manifold& contact);
	private:
		std::vector<std::pair<Entity, Entity>> mContactPair;// vector list of each two contact entities
		Manifold mManifoldInfo;
	};
}
#endif
