#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR3D_H
#define GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR3D_H

namespace IS {
#ifdef _MSC_VER
#pragma warning(disable : 4201) // Supress warning: nonstandard extension used : nameless struct/union
#endif
    struct Vector3D {
        float x, y, z;

        // Constructors
#pragma warning(push)
#pragma warning(disable : 26495)
        Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3D(float _x, float _y, float _z);
#pragma warning(pop)
        Vector3D(const Vector3D& rhs) = default;
        Vector3D& operator=(const Vector3D& rhs) = default;

        // Assignment operators
        Vector3D& operator+=(const Vector3D& rhs);
        Vector3D& operator-=(const Vector3D& rhs);
        Vector3D& operator*=(float rhs);
        Vector3D& operator/=(float rhs);

        Vector3D operator-() const;
    };

    Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs);
    Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs);
    Vector3D operator*(const Vector3D& lhs, float rhs);
    Vector3D operator*(float lhs, const Vector3D& rhs);
    Vector3D operator/(const Vector3D& lhs, float rhs);
    bool operator==(const Vector3D& lhs, const Vector3D& rhs);

    // Additional functions related to 3D vectors
    void ISVector3DNormalize(Vector3D& pResult, const Vector3D& pVec0);
    float ISVector3DLength(const Vector3D& pVec0);
    float ISVector3DSquareLength(const Vector3D& pVec0);
    float ISVector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1);
    float ISVector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1);
    float ISVector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1);
    Vector3D ISVector3DCrossProduct(const Vector3D& pVec0, const Vector3D& pVec1);

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR3D_H