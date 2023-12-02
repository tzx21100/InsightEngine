/*!
 * \file CollisionSystem.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains CollisionSystem class. This class is responsible for 
 * handling the physics of collisions between entities in a game.
 * It includes functions for detecting and resolving collisions.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_SYSTEM_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_SYSTEM_H

 /*                                                                   includes
 ----------------------------------------------------------------------------- */
#include "Engine/Core/Core.h"
#include "Engine/ECS/Entities.h"
#include "Physics/Collision/Collider.h"
#include "Physics/Collision/Manifold.h"
#include "Physics/Collision/Collision.h"
#include "Physics/Dynamics/Body.h"
#include "Physics/System/Physics.h"
#include <vector>

namespace IS
{
	/*!
	 * \enum CollidingStatus
	 * \brief Enumerates the possible collision statuses between different entity types.
	 */
	enum CollidingStatus : short{
		BOX_A_BOX_B = 1,
		BOX_A_CIRCLE_B,
		CIRCLE_A_BOX_B,
		CIRCLE_A_CIRCLE_B
	};

	/*!
	 * \class CollisionSystem
	 * \brief The CollisionSystem class handles detection and resolution of collisions.
	 *
	 * It includes methods for both broad and narrow phase collision detection,
	 * handling collision response, and managing collision data.
	 */
	class IS_API CollisionSystem : public ParentSystem
	{
	public:
		/*!
		 * \brief Constructor for CollisionSystem.
		 */
		CollisionSystem();

		/*!
		 * \brief Update function called each frame.
		 * \param dt Delta time since last frame.
		 */
		void Update(float dt) override;

		/*!
		 * \brief Gets the name of the collision system.
		 *
		 * \return The name of the collision system.
		 */
		std::string GetName() override { return "CollisionSystem"; }

		/*!
		 * \brief Initializes the collision system.
		 */
		void Initialize() override;

		/*!
		 * \brief Handles messages for the collision system.
		 *
		 * \param message The message to handle.
		 */
		void HandleMessage(const Message&) override {}

		/*!
		 * \brief Destructor for the collision system class.
		 */
		~CollisionSystem() {}

		/*!
		 * \brief Performs broad-phase collision detection.
		 *
		 * This method is responsible for quickly identifying potential collisions
		 * among a large number of entities, typically using methods like spatial partitioning.
		 */
		void BroadPhase();

		/*!
		 * \brief Performs narrow-phase collision detection.
		 *
		 * After potential collisions are identified in the broad phase, this method
		 * performs more accurate and computationally intensive checks to confirm
		 * and process these collisions.
		 */
		void NarrowPhase();

		/*!
		 * \brief Detects collisions using an implicit grid method.
		 *
		 * This method is an implementation of collision detection that utilizes an implicit grid
		 * to efficiently detect potential collisions in a large space with many entities.
		 */
		void ImplicitGridCollisionDetect();

		/*!
		 * \brief Detects collisions for a set of entities.
		 * \param entities A set of entities to check for collisions.
		 *
		 * This method checks for collisions among the provided entities. It uses different
		 * collision detection functions based on the shapes of the bodies involved (e.g., box, circle).
		 */
		void CollisionDetect(std::set<Entity> const& entities);

		/*!
		 * \brief Detects collisions for a vector of entities.
		 * \param entities A vector of entities to check for collisions.
		 *
		 * Similar to the set-based overload, this method checks for collisions among
		 * the entities provided in a vector.
		 */
		void CollisionDetect(std::vector<Entity> const& entities);

		/*!
		 * \brief Separates colliding bodies.
		 * \param typeA The type of the first body.
		 * \param typeB The type of the second body.
		 * \param transA The transform of the first body.
		 * \param transB The transform of the second body.
		 * \param vec The vector along which separation should occur.
		 *
		 * This method is used to separate two bodies that are colliding and penetrating each other.
		 */
		void SeparateColliders(BodyType typeA, BodyType typeB, Transform& transA, Transform& transB, Vector2D const& vec);
		
		/*!
		 * \brief Resolves collisions between two rigid bodies.
		 * \param contact The manifold containing collision information.
		 *
		 * This method calculates and applies the impulse force to update the velocities
		 * of the colliding entities, effectively resolving the collision.
		 */
		void ResolveCollision(Manifold& contact);

		/*!
		 * \brief Resolves collisions between two rigid bodies, considering their rotations.
		 * \param contact The manifold containing collision information.
		 * \param transA The transform of the first body involved in the collision.
		 * \param transB The transform of the second body involved in the collision.
		 *
		 * This method is similar to ResolveCollision but takes into account the rotation of the bodies.
		 * It calculates the necessary forces and updates the velocities and rotations of the colliding entities.
		 */
		void ResolveCollisionWithRotation(Manifold& contact, Transform const& transA, Transform const& transB);
		
		/*!
		 * \brief Resolves collisions with considerations for both rotation and friction.
		 * \param contact The manifold containing collision information.
		 * \param transA The transform of the first body involved in the collision.
		 * \param transB The transform of the second body involved in the collision.
		 *
		 * This method extends ResolveCollisionWithRotation by also considering the effects of friction
		 * in the collision resolution process. It provides a more realistic collision response by
		 * accounting for both rotational forces and frictional forces.
		 */
		void ResolveCollisionWithRotationAndFriction(Manifold& contact, Transform & transA, Transform & transB);

		/*!
		 * \brief Checks and processes the collision between two colliders. Used under NarrowPhase()
		 * \param collider_a The first collider involved in the collision.
		 * \param collider_b The second collider involved in the collision.
		 *
		 * This method checks if there is a collision between two colliders and if so,
		 * calculates and collects the collision data such as the normal and depth of the collision.
		 */
		void Colliding(Collider& collider_a, Collider& collider_b);

		/*!
		 * \brief Checks if an entity is colliding with any other entities. (Helper function for Scripting)
		 * \param entity The entity to check for collisions.
		 * \return True if the entity is colliding, false otherwise.
		 *
		 * This static helper function checks whether the given entity is currently colliding
		 * with any other entity in the system.
		 */
		static bool CheckColliding(Entity entity);

		/*!
		 * \brief Saves data about a collision.
		 * \param colliding_status The status of the collision.
		 * \param normal The normal vector of the collision.
		 * \param depth The depth of penetration in the collision.
		 *
		 * This method is used for saving detailed data about a collision, including the status
		 * (e.g., types of objects colliding), the normal vector at the point of collision, and
		 * the depth of penetration. This data can be used for further processing or analysis of the collision.
		 */
		void SavingCollidingData(short colliding_status,Vector2D const& normal, float depth);

		/*!
		 * \brief Advances the collision system by one simulation step, update the collider with transform
		 *
		 * This method progresses the state of the collision system by one step, updating
		 * the positions and states of all entities based on the current collision data.
		 */
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
