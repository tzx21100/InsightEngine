/*!
 * \file Manifold.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the declaration of the Manifold class, which represents contact information
 * between two objects in a physics simulation.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MANIFOLD_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MANIFOLD_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/Core/Core.h"
#include "Physics/Dynamics/Body.h"
#include "Collider.h"
#include "Math/ISMath.h"

#define MAX_COLLIDING_CASE 5 // 4 cases

namespace IS 
{
     /*!
     * \class Manifold
     * \brief Represents contact information between two objects in a physics simulation.
     *
     * The Manifold class is a mathematical construct used to store contact information between two rigid bodies.
     * It holds data such as the normal vector, penetration depth, and contact points.
     */
	class IS_API Manifold {
	public:
        RigidBody* mBodyA;          //!< Pointer to the first rigid body involved in the collision.
        RigidBody* mBodyB;          //!< Pointer to the second rigid body involved in the collision.
        Collider* mColliderA;       //!< Pointer to the first collider involved in the collision.
        Collider* mColliderB;       //!< Pointer to the second collider involved in the collision.
        Vector2D mNormal;           //!< Normal vector of the contact.
        float mDepth;               //!< Penetration depth of the collision.
        Vector2D mContact1;         //!< First contact point in world space.
        Vector2D mContact2;         //!< Second contact point in world space.
        int mContactCount;          //!< Number of contact points.

        /*!
         * \brief Default constructor for the Manifold class.
         */
        Manifold();

        /*!
         * \brief Constructor for the Manifold class that initializes its members.
         *
         * \param bodyA Pointer to the first rigid body involved in the collision.
         * \param bodyB Pointer to the second rigid body involved in the collision.
         * \param colliderA Pointer to the first rigid body involved in the collision.
         * \param colliderB Pointer to the second rigid body involved in the collision.
         * \param normal The normal vector of the contact.
         * \param depth The penetration depth of the collision.
         * \param contact1 The first contact point in world space.
         * \param contact2 The second contact point in world space.
         * \param contactCount The number of contact points.
         */
        Manifold(RigidBody* bodyA, RigidBody* bodyB,
            Collider* colliderA, Collider* colliderB,
            Vector2D const& normal, float const& depth,
            Vector2D const& contact1, Vector2D const& contact2, int const& contactCount);

        /*!
         * \brief Calculates the contact points for the collision between two rigid bodies.
         *
         * This method calculates the contact points based on the shapes and positions of the two rigid bodies.
         * The contact points are stored in the `mContact1` and `mContact2` members.
         *
         * \param colliderA The first collider involved in the collision.
         * \param colliderB The second collider involved in the collision.
         */
        void FindContactPoints(Collider & colliderA, Collider& colliderB, std::bitset<MAX_COLLIDING_CASE> colliding_collection);

        /*!
         * \brief Calculates contact points for a collision between two polygons.
         *
         * This method calculates contact points for a collision between two polygonal shapes
         * represented by their vertices. The contact points are stored in the `contact1` and `contact2` parameters.
         *
         * \param verticesA Vertices of the first polygon.
         * \param verticesB Vertices of the second polygon.
         * \param contact1 Output parameter to store the first contact point.
         * \param contact2 Output parameter to store the second contact point.
         * \param contactCount Output parameter to store the number of contact points.
         */
        void FindPolygonsContactPoints(std::vector<Vector2D> const& verticesA, std::vector<Vector2D> const& verticesB, Vector2D & contact1, Vector2D & contact2, int & contactCount);
       
        /*!
         * \brief Calculates contact points for a collision between a circle and a polygon.
         *
         * \details
         * This method handles the calculation of the contact point(s) when a circular body collides with a polygonal body.
         * It takes into account the radius of the circle, the vertices of the polygon, and their respective positions
         * to accurately determine the point of collision.
         *
         * \param circle_center Center of the circle.
         * \param circle_radius Radius of the circle.
         * \param polygon_center Center of the polygon.
         * \param polygon_vertices Vertices of the polygon.
         * \param contact_point Output parameter to store the contact point.
         */
        void FindCirclePolygonContactPoints(Vector2D const& circle_center, float const& circle_radius, Vector2D const& polygon_center, std::vector<Vector2D> const& polygon_vertices, Vector2D& contact_point);
        
        /*!
         * \brief Calculates the contact point for a collision between two circles.
         *
         * \details
         * This method is used to determine the point of contact when two circular bodies collide.
         * It is a simpler case of collision detection but crucial for accurately handling collisions
         * between spherical bodies in the simulation.
         *
         * \param center_a Center of the first circle.
         * \param radius_a Radius of the first circle.
         * \param center_b Center of the second circle.
         * \param contact_point Output parameter to store the contact point.
         */
        void FindCirlcesContactPoints(Vector2D const& center_a, float const& radius_a, Vector2D const& center_b, Vector2D & contact_point);

        /*!
         * \brief Compares two floating-point values for near equality.
         *
         * \param a The first floating-point value to compare.
         * \param b The second floating-point value to compare.
         * \return `true` if the two values are nearly equal, `false` otherwise.
         */
        bool NearlyEqual(float const& a, float const& b);

        /*!
         * \brief Compares two 2D vectors for near equality.
         *
         * \param a The first vector to compare.
         * \param b The second vector to compare.
         * \return `true` if the two vectors are nearly equal, `false` otherwise.
         */
        bool NearlyEqual(Vector2D const& a, Vector2D const& b);
	};

}

#endif