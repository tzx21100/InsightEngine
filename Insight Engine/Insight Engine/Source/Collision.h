/*!
 * \file Collision.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
 * \brief
 * This header file contains various collision detection and resolution functions for different geometric shapes,
 * including boxes, circles, and lines. These functions are designed to facilitate precise and realistic physics simulations
 * within the game engine.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_COLLISION_H

 /*                                                                   includes
  ----------------------------------------------------------------------------- */

#include "Pch.h"
#include "Component.h"

namespace IS
{
    /*!
     * \struct Box
     * \brief A structure representing an axis-aligned bounding box (AABB) defined by its minimum and maximum corners.
     */
    struct Box
    {
        Vector2D min; // Minimum point of the bounding box
        Vector2D max; // Maximum point of the bounding box

        Box(const Vector2D& min, const Vector2D& max) : min(min), max(max) {}

        Box(float minX, float minY, float maxX, float maxY) : min(minX, minY), max(maxX, maxY) {}
    };

    /*!
     * \struct Circle
     * \brief A structure representing a circle defined by its center and radius.
     */
    struct Circle
    {
        Vector2D center; // Center of the circle
        float    radius; // Radius of the circle
    };

    /*!
     * \struct Line
     * \brief A structure representing a line segment defined by its start and end points, along with a normal vector.
     */
    struct Line
    {
        Vector2D start; // Starting point of the line segment
        Vector2D end; // Ending point of the line segment
        Vector2D normal; // Normal vector of the line segment
    };

    /*!
     * \enum BodyShape
     * \brief An enumeration representing the possible shapes of rigid bodies for collision detection.
     */
    enum class BodyShape {Box, Circle, Line};

    //class Collider : public IComponent {
    //public:
    //    //Shape colliderType;
    //    std::variant<Box, Circle, Line> myCollider;
    //};


     /*                                                                FOR BOX 
  ----------------------------------------------------------------------------- */

    /*!
     * \brief Detects collision between two AABB (Axis-Aligned Bounding Box) rectangles, considering their velocities and delta time.
     *
     * \param aabb1 The first AABB.
     * \param vel1 Velocity of the first AABB.
     * \param aabb2 The second AABB.
     * \param vel2 Velocity of the second AABB.
     * \param dt Delta time.
     * \return True if the two AABBs are colliding, false otherwise.
     */
	bool CollisionIntersectionAABB(const Box& aabb1, const Vector2D& vel1,
		const Box& aabb2, const Vector2D& vel2, const float& dt);

    // calculate the shortest distance from a point to the line segment
    void PointSegmentDistance(Vector2D const& point, Vector2D const& a, Vector2D const& b, float & distance_squared, Vector2D & closest_point);

    // static AABB
    bool StaticIntersectAABB(const Box& a, const Box& b);

     /*!
     * \brief Uses the Axis Separation Theorem (AST) to check collision between two polygons and calculates collision response data normal and depth.
     *
     * \param trans_verticesA Transformed vertices of polygon A.
     * \param centerA Center of polygon A.
     * \param trans_verticesB Transformed vertices of polygon B.
     * \param centerB Center of polygon B.
     * \param normal Normal vector of the collision.
     * \param depth Depth of collision penetration.
     * \return True if polygons A and B are colliding, false otherwise.
     */
    bool IntersectionPolygons(std::vector<Vector2D> const& trans_verticesA, Vector2D centerA, std::vector<Vector2D> trans_verticesB, Vector2D centerB, Vector2D& normal, float& depth);
    
    /*!
    * \brief Projects all vertices of a polygon onto a given normal vector and finds the minimum and maximum projections.
    *
    * \param vertices The vertices of the polygon.
    * \param normal The normal vector onto which the vertices are projected.
    * \param min The minimum projection value.
    * \param max The maximum projection value.
    */
    void ProjectVertices(std::vector<Vector2D> const& vertices, Vector2D const& normal, float& min, float& max);
 
     /*                                                   FOR CIRCLE VS POLYGON
  ----------------------------------------------------------------------------- */

    /*!
    * \brief Uses the Axis Separation Theorem (AST) to check collision between a circle and a polygon and calculates collision response data normal and depth.
    *
    * \param circle_center Center of the circle.
    * \param radius Radius of the circle.
    * \param polygon_center Center of the polygon.
    * \param trans_vertices Transformed vertices of the polygon.
    * \param normal Normal vector of the collision.
    * \param depth Depth of collision penetration.
    * \return True if the circle and polygon are colliding, false otherwise.
    */
    bool IntersectionCirlcecPolygon(Vector2D circle_center, float radius, Vector2D polygon_center, std::vector<Vector2D> trans_vertices, Vector2D& normal, float& depth);
    
    /*!
    * \brief Projects the left and right edges of a circle onto a given normal vector and finds the minimum and maximum projections.
    *
    * \param center The center of the circle.
    * \param radius The radius of the circle.
    * \param normal The normal vector onto which the circle edges are projected.
    * \param min The minimum projection value.
    * \param max The maximum projection value.
    */
    void ProjectCircle(Vector2D const& center, float const& radius, Vector2D const& normal, float& min, float& max);
    
    /*!
    * \brief Finds the closest point on a polygon to a given circle center.
    *
    * \param circle_center Center of the circle.
    * \param vertices Vertices of the polygon.
    * \return Index of the closest vertex on the polygon to the circle center.
    */
    int FindClosestPointOnPolygon(Vector2D circle_center, std::vector<Vector2D> vertices);

     /*                                                             FOR CIRCLE
  ----------------------------------------------------------------------------- */
    /*!
    * \brief Checks if two circles are intersecting.
    *
    * \param centerA Center of the first circle.
    * \param radiusA Radius of the first circle.
    * \param centerB Center of the second circle.
    * \param radiusB Radius of the second circle.
    * \return True if the two circles are intersecting, false otherwise.
    */
    bool Intersection_Circles(Vector2D centerA, float radiusA, Vector2D centerB, float radiusB);

    /*!
    * \brief Builds a line segment from two given points, p0 - start, p1 - end.
    *
    * \param lineSegment Reference to a Line structure to store the resulting line segment.
    * \param p0 The start point of the line segment.
    * \param p1 The end point of the line segment.
    */
    void BuildLineSegment(Line& lineSegment, const Vector2D& p0, const Vector2D& p1);

    /*!
    * \brief Checks whether a moving circle is going to collide with a line segment, calculates collision response data and check whether the ball is inside or outside of the line segement.
    *
    * \param circle The circle data.
    * \param ptEnd The end position of the circle.
    * \param lineSeg The line segment data.
    * \param interPt The intersection point (output).
    * \param normalAtCollision The normal vector at collision time (output).
    * \param interTime The intersection time (output).
    * \param checkLineEdges Flag for checking collision with line segment edges.
    * \return 0 if no collision, 1 if collision.
    */
    int CollisionIntersection_CircleLineSegment(
        const Circle& circle,			                                        //Circle data - input
        const Vector2D& ptEnd,													//End circle position - input
        const Line& lineSeg,												    //Line segment - input
        Vector2D& interPt,														//Intersection point - output
        Vector2D& normalAtCollision,											//Normal vector at collision time - output
        float& interTime,														//Intersection time ti - output
        bool& checkLineEdges);													//The last parameter is for Extra Credits: when true => check collision with line segment edges
    
    /*!
    * \brief Checks and handles the collision of a moving circle with a line edge, and reflect the ball accordingly.
    *
    * \param withinBothLines Flag indicating whether the circle starts between two imaginary line segments.
    * \param circle The circle data.
    * \param ptEnd The end position of the circle.
    * \param lineSeg The line segment data.
    * \param interPt The intersection point (output).
    * \param normalAtCollision The normal vector at collision time (output).
    * \param interTime The intersection time (output).
    * \return 0 if no collision, 1 if collision with line segment edges, 2 if collision with line segment body.
    */
    int CheckMovingCircleToLineEdge(
        bool withinBothLines,	                            					//Flag stating that the circle is starting from between 2 imaginary line segments distant +/- Radius respectively - input
        const Circle& circle,													//Circle data - input
        const Vector2D& ptEnd,													//End circle position - input
        const Line& lineSeg,												    //Line segment - input
        Vector2D& interPt,														//Intersection point - output
        Vector2D& normalAtCollision,											//Normal vector at collision time - output
        float& interTime);														//Intersection time ti - output

    /*!
    * \brief Calculates the reflection of a circle after collision with a line segment based on the collision normal.
    *
    * \param ptInter The intersection position of the circle.
    * \param normal The normal vector of reflection.
    * \param ptEnd The final position of the circle after reflection (output).
    * \param reflected The normalized reflection vector direction (output).
    */
    void CollisionResponse_CircleLineSegment(
        const Vector2D & ptInter,		                                    		//Intersection position of the circle - input
        const Vector2D & normal,													//Normal vector of reflection on collision time - input
        Vector2D & ptEnd,															//Final position of the circle after reflection - output
        Vector2D & reflected);														//Normalized reflection vector direction - output


}

#endif