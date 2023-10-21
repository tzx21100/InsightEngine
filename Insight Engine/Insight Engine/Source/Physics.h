/*!
 * \file Physics.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
 * \brief
 * This header file contains the Physics class and related functions for real world physics simulation 
 * and collision handling.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   includes
  ----------------------------------------------------------------------------- */
#pragma once

#include "Pch.h"
#include "Sprite.h"

namespace IS 
{
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
         * \brief Static flag to enable or disable debug drawing of rigidbody shapes.
         */
        static bool isDebugDraw;

        static bool mShowColliders;
        static bool mShowVelocity;
        static bool mShowGrid;

        /*!
         * \brief Destructor for the Physics class.
         */
        ~Physics() {}

        void CellCollisionDetect();

        /*!
         * \brief Detects collisions among a set of entities, running different collision detect function form collision.h based on the body shape (box, circle or line).
        *
        * \param mEntities The set of entities to consider for collision detection.
        */
        void CollisionDetect(std::set<Entity> const& entities);

        /*!
         * \brief Resolves collisions between two rigid bodies by calculating and applying the impulse force to update the velocities of collding entities.
         *
         * \param bodyA The first rigid body involved in the collision.
         * \param bodyB The second rigid body involved in the collision.
         * \param normal The collision normal vector.
         * \param depth The depth of collision penetration.
         */
        //void ResolveCollision(RigidBody& bodyA, RigidBody& bodyB, Vector2D const& normal, float depth);
        void ResolveCollision(Manifold & contact);

        /*!
         * \brief Draws the velocity and an outline around the specified rigid body using the provided sprite based on vertices for polygons.
         *
         * \param body The rigid body to draw the outline for.
         * \param sprite The sprite used for drawing the outline.
         */
        static void DrawOutLine(RigidBody& body, Sprite const& sprite);

        /*!
         * \brief Updates the gravity vector based on user input.
         *
         * \param key_input The input related to gravity control.
         */
        void UpdateGravity(auto const& key_input);

        /*!
         * \brief Updates the debug drawing mode based on user input.
         *
         * \param key_input The input related to debug drawing control.
         */
        void UpdateDebugDraw(auto const& key_input);

        /*!
         * \brief Performs a physics step for the specified time and set of entities, updates velocities and positions for game entities.
         *
         * \param time The time for the physics step.
         * \param entities The set of entities to consider in the physics step.
         */
        void Step(float time, std::set<Entity> const& entities);

        
	private:

        Vector2D mGravity;                              // Gravity of the world
        bool mExertingGravity;                          // Flag indicating whether gravity is currently exerted
        float mMaxVelocity;                             // Maximum velocity for game bodies
        float mMinVelocity;                             // Minimum velocity for game bodies
        int mIterations;                                 // Number of iterations for physics step
        std::vector<Manifold> mContactList;
        std::vector<Vector2D> mContactPointsList;
        Grid mGrid;
	};

    // Pointer to the Physics instance
    extern Physics* PHYSICS;
}