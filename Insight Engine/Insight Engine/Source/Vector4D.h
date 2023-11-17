#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR4D_H
#define GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR4D_H

namespace IS {
#ifdef _MSC_VER
#pragma warning(disable : 4201) // Suppress warning: nonstandard extension used : nameless struct/union
#endif
    struct Vector4D {
        float x, y, z, w;

        // Constructors
#pragma warning(push)
#pragma warning(disable : 26495)
        Vector4D() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        Vector4D(float _x, float _y, float _z, float _w);
#pragma warning(pop)
        Vector4D(const Vector4D& rhs) = default;
        Vector4D& operator=(const Vector4D& rhs) = default;

        // Assignment operators
        Vector4D& operator+=(const Vector4D& rhs);
        Vector4D& operator-=(const Vector4D& rhs);
        Vector4D& operator*=(float rhs);
        Vector4D& operator/=(float rhs);

        Vector4D operator-() const;
    };

    Vector4D operator+(const Vector4D& lhs, const Vector4D& rhs);
    Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs);
    Vector4D operator*(const Vector4D& lhs, float rhs);
    Vector4D operator*(float lhs, const Vector4D& rhs);
    Vector4D operator/(const Vector4D& lhs, float rhs);
    bool operator==(const Vector4D& lhs, const Vector4D& rhs);

    // Additional functions related to 4D vectors
    void ISVector4DNormalize(Vector4D& pResult, const Vector4D& pVec0);
    float ISVector4DLength(const Vector4D& pVec0);
    float ISVector4DSquareLength(const Vector4D& pVec0);
    float ISVector4DDotProduct(const Vector4D& pVec0, const Vector4D& pVec1);

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR4D_H
