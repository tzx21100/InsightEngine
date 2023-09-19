#pragma once

#include "Pch.h"
#include "Component.h"

namespace IS
{
    struct Box
    {
        Vector2D min;
        Vector2D max;
    };

    struct Circle
    {
        Vector2D center;
        float    radius;

    };

    struct Line
    {
        Vector2D start;
        Vector2D end;
        Vector2D normal;
    };

    enum class Shape {Box, Circle, Line};

    class Collider : public IComponent {
    public:
        //Shape colliderType;
        std::variant<Box, Circle, Line> myCollider;
    };

    // FOR BOX 
	// AABB Collision for static and dynamic, return true if they colliding
	bool CollisionIntersection_RectRect(const Box& aabb1, const Vector2D& vel1,
		const Box& aabb2, const Vector2D& vel2, const float& dt);

    // using AST to check the collision for two polygons, updating the normal and depth for response
    bool Intersection_Polygons(std::vector<Vector2D> const& trans_verticesA, Vector2D centerA, std::vector<Vector2D> trans_verticesB, Vector2D centerB, Vector2D& normal, float& depth);
    // helper function for projecting all the vertices onto the normal
    void ProjectVertices(std::vector<Vector2D> const& vertices, Vector2D const& normal, float& min, float& max);

    // FOR CIRCLE VS POLYGON

    // AST to check the collision between circle and polygon, update the normal and depth for calculating response
    bool Intersection_Cirlcec_Polygon(Vector2D circleCenter, float radius, Vector2D polygonCenter, std::vector<Vector2D> trans_vertices, Vector2D& normal, float& depth);
    // projecting the left and right edges of the circle onto the normal
    void ProjectCircle(Vector2D const& center, float const& radius, Vector2D const& normal, float& min, float& max);
    // getting the closest point on polygon to the circle
    int FindClosestPointOnPolygon(Vector2D circleCenter, std::vector<Vector2D> vertices);

    // FOR CIRCLE
    bool Intersection_Circles(Vector2D centerA, float radiusA, Vector2D centerB, float radiusB);

    //line segment reference, p0 - start, p1 - end
    void BuildLineSegment(Line& lineSegment, const Vector2D& p0, const Vector2D& p1);

    //check whether the ball is going to collide with the line segment and check whether the ball is inside or outside of the line segement
    int CollisionIntersection_CircleLineSegment(
        const Circle& circle,			                                        //Circle data - input
        const Vector2D& ptEnd,													//End circle position - input
        const Line& lineSeg,												    //Line segment - input
        Vector2D& interPt,														//Intersection point - output
        Vector2D& normalAtCollision,											//Normal vector at collision time - output
        float& interTime,														//Intersection time ti - output
        bool& checkLineEdges);													//The last parameter is for Extra Credits: when true => check collision with line segment edges
    
    //moving the ball when it is going to collide with the line edge and reflecting the ball accordingly
    int CheckMovingCircleToLineEdge(
        bool withinBothLines,	                            					//Flag stating that the circle is starting from between 2 imaginary line segments distant +/- Radius respectively - input
        const Circle& circle,													//Circle data - input
        const Vector2D& ptEnd,													//End circle position - input
        const Line& lineSeg,												    //Line segment - input
        Vector2D& interPt,														//Intersection point - output
        Vector2D& normalAtCollision,											//Normal vector at collision time - output
        float& interTime);														//Intersection time ti - output

    //reflecting the ball according to the hit direction with the line segment based on the normal of V
    void CollisionResponse_CircleLineSegment(
        const Vector2D & ptInter,		                                    		//Intersection position of the circle - input
        const Vector2D & normal,													//Normal vector of reflection on collision time - input
        Vector2D & ptEnd,															//Final position of the circle after reflection - output
        Vector2D & reflected);														//Normalized reflection vector direction - output


}