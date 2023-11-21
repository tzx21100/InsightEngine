
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_SYSTEM_H

#include "Entities.h"
#include "Body.h"
#include "Collider.h"
#include "Manifold.h"
#include "Physics.h"
#include "Collision.h"
#include <vector>

/*                                                                   includes
 ----------------------------------------------------------------------------- */

namespace IS
{
	// enum for saving all the possible colliding situations/cases
	enum CollidingStatus : short{
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

		// Detects collisions among all the possible entities
		void BroadPhase();

		// Resloves collisions among all the entities
		void NarrowPhase();

		void ImplicitGridCollisionDetect();

		// Detects collisions among a set of entities, running different collision detect function form collision.h based on the body shape (box, circle or line).
		void CollisionDetect(std::set<Entity> const& entities);

		void CollisionDetect(std::vector<Entity> const& entities);

		// separate two bodies if they colliding and penetrating
		void SeparateColliders(BodyType typeA, BodyType typeB, Transform& transA, Transform& transB, Vector2D const& vec);
		
		// Resolves collisions between two rigid bodies by calculating and applying the impulse force to update the velocities of collding entities.
		void ResolveCollision(Manifold& contact);
		void ResolveCollisionWithRotation(Manifold& contact, Transform const& transA, Transform const& transB);
		void ResolveCollisionWithRotationAndFriction(Manifold& contact, Transform & transA, Transform & transB);

		// check collding between two colliders, calculate and collect the colliding data like normal and depth
		void Colliding(Collider& collider_a, Collider& collider_b);

		// helper function to check if the current entity colliding with any other entities
		static bool CheckCollide(Entity& entity);

		static bool CheckColliding(Entity entity);

		void SavingCollidingData(short colliding_status,Vector2D const& normal, float depth);



		void Step();

	private:
		std::vector<std::pair<Entity, Entity>> mContactPair;	// vector list of each two contact entities
		Manifold mManifoldInfo;									// instance of Manifold
		bool mColliding;										// boolean status between two colliders in one collide
		std::bitset<MAX_COLLIDING_CASE> mCollidingCollection;	// collection of all possible collision happens between two colliders in one collide
		ImplicitGrid mImplicitGrid;								// instance of ImplicitGrid
	};
}
#endif
