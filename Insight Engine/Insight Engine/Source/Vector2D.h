#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR2D_H
#define GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR2D_H

namespace IS {

#ifdef _MSC_VER
    #pragma warning(disable : 4201) // Supress warning: nonstandard extension used : nameless struct/union
#endif

    /**************************************************************************/
    /*!
        This typedef union represents a mathematical vector in 2D space.
     */
     /**************************************************************************/
    typedef union Vector2D {
        struct {
            float x, y;
        };

        float m[2];

        // Constructors
    #pragma warning(push)
    #pragma warning(disable : 26495)
        Vector2D() : x(0.0f), y(0.0f) {}
        Vector2D(float _x, float _y);
    #pragma warning(pop)

        //Do not change the following
        Vector2D& operator=(const Vector2D& rhs) = default;
        Vector2D(const Vector2D& rhs) = default;

        // Assignment operators
        Vector2D& operator += (const Vector2D& rhs);
        Vector2D& operator -= (const Vector2D& rhs);
        Vector2D& operator *= (float rhs);
        Vector2D& operator /= (float rhs);

        // Unary operators
        Vector2D operator -() const;

    } Vec2;


#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

    // Binary operators
    Vector2D operator +  (const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator -  (const Vector2D& lhs, const Vector2D& rhs);
    Vector2D operator *  (const Vector2D& lhs, float rhs);
    Vector2D operator *  (float lhs, const Vector2D& rhs);
    Vector2D operator /  (const Vector2D& lhs, float rhs);
    bool     operator == (const Vector2D& lhs, const Vector2D& rhs);

    /**************************************************************************/
    /*!
        In this function, pResult will be the unit vector of pVec0
     */
     /**************************************************************************/
    void ISVector2DNormalize(Vector2D& pResult, const Vector2D& pVec0);

    /**************************************************************************/
    /*!
        This function returns the length of the vector pVec0
     */
     /**************************************************************************/
    float ISVector2DLength(const Vector2D& pVec0);

    /**************************************************************************/
    /*!
        This function returns the square of pVec0's length. Avoid the square root
     */
     /**************************************************************************/
    float ISVector2DSquareLength(const Vector2D& pVec0);

    /**************************************************************************/
    /*!
        In this function, pVec0 and pVec1 are considered as 2D points.
        The distance between these 2 2D points is returned
     */
     /**************************************************************************/
    float ISVector2DDistance(const Vector2D& pVec0, const Vector2D& pVec1);

    /**************************************************************************/
    /*!
        In this function, pVec0 and pVec1 are considered as 2D points.
        The squared distance between these 2 2D points is returned.
        Avoid the square root
     */
     /**************************************************************************/
    float ISVector2DSquareDistance(const Vector2D& pVec0, const Vector2D& pVec1);

    /**************************************************************************/
    /*!
        This function returns the dot product between pVec0 and pVec1
     */
     /**************************************************************************/
    float ISVector2DDotProduct(const Vector2D& pVec0, const Vector2D& pVec1);

    /**************************************************************************/
    /*!
        This function returns the cross product magnitude
        between pVec0 and pVec1
     */
     /**************************************************************************/
    float ISVector2DCrossProductMag(const Vector2D& pVec0, const Vector2D& pVec1);

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR2D_H
