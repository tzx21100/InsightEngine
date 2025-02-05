/*!
 * \file Physics.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 26-11-2023
 * \brief
 * This header file contains the Physics class and related functions for real world physics simulation 
 * and collision handling.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_PHYSICS_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_PHYSICS_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/ECS/System.h"
#include "Physics/Dynamics/Body.h"
#include "Physics/Collision/Manifold.h"

namespace IS {
    /*!
     * \class Physics
     * \brief The Physics class handles physics simulation, collision detection and response for game entities.
     */
	class Physics : public ParentSystem {
	public:
        /*!
         * \brief Constructs a Physics instance.
         */
        Physics();

        /*!
         * \brief Updates the physics simulation for the given time step.
         *
         * \param dt The time step for the physics update.
         */
        void Update(float dt) override;

        /*!
         * \brief Gets the name of the physics system.
         *
         * \return The name of the physics system.
         */
        std::string GetName() override { return "Physics"; }

        /*!
         * \brief Initializes the physics system.
         */
        void Initialize() override;

        /*!
         * \brief Handles messages for the physics system.
         *
         * \param message The message to handle.
         */
        void HandleMessage(const Message&) override {}

        /*!
         * \brief Boolean flag to show colliders.
         */
        static bool mShowColliders;

        /*!
         * \brief Boolean flag to show velocity.
         */
        static bool mShowVelocity;

        /*!
         * \brief Boolean flag to show grid.
         */
        static bool mShowGrid;

        /*!
         * \brief Boolean flag to enable implicit grid for collision detect.
         */
        static bool mEnableImplicitGrid;

        /*!
         * \brief Boolean flag to exert gravity
         */
        static bool mExertingGravity;

        /*!
         * \brief Gravity of the world
         */
        static Vector2D mGravity;  

        /*!
         * \brief physics enable list to update in each frame
         */
        static std::set<Entity> PhysicsEnableList;

        /*!
         * \brief Destructor for the Physics class.
         */
        ~Physics() {}

        /*!
         * \brief Spatial partitioning by implicit grid
        *
        */
        void ImplicitGridCollisionDetect();

        /*!
         * \brief Detects collisions among a set of entities, running different collision detect function form collision.h based on the body shape (box, circle or line).
        *
        * \param mEntities The vector of entities to consider for collision detection.
        */
        void CollisionDetect(std::vector<Entity> const& entities);

        /*!
         * \brief Detects collisions among all the possible entities
        *
        */
        void BroadPhase();

        /*!
         * \brief Resloves collisions among all the entities
        *
        */
        void NarrowPhase();

        /*!
         * \brief separate two bodies if they colliding and penetrating
        *
        * \param bodyA pointer to the first body.
        * \param bodyB pointer to the second body.
        * \param transA pointer to the first transform.
        * \param transB pointer to the second transform.
        * \param vec vector of penetration depth to move entities apart (normal * penetration depth)
        */
        void SeparateBodies(RigidBody* bodyA, RigidBody* bodyB, Transform* transA, Transform* transB, Vector2D const& vec);

        /*!
         * \brief Draws the velocity and an outline around the specified rigid body using the provided sprite based on vertices for polygons.
         *
         * \param body The rigid body to draw the outline for.
         * \param sprite The sprite used for drawing the outline.
         * \param color The color of the outline.
         * \param thickness The width of the outline.
         */
        static void DrawOutLine(Collider& collider, std::tuple<float, float, float> const& color = { 0.f, 1.f, 0.f });

        // for shadow lights
        /*!
         * \brief Add into line segements for shadow lights
         *
         * \param entity The set of entities to consider in the physics step.
         */
        void AddLineSegementsForLights(Entity const& entity);

        /*!
         * \brief Performs a physics step for the specified time and set of entities, updates velocities and positions for game entities.
         *
         * \param time The time for the physics step.
         * \param entities The set of entities to consider in the physics step.
         */
        void Step(float time, std::set<Entity> const& entities);


	private:
        float mMaxVelocity;                                 // Maximum velocity for game bodies
        float mMinVelocity;                                 // Minimum velocity for game bodies
        int mTotalIterations;                               // Number of iterations for physics step
        int mCurrentIterations;                             // Number of current iterations for physics step
	};

}

#endif // !GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_PHYSICS_H
  