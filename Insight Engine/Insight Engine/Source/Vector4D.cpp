#include "Pch.h"
#include "Vector4D.h"

namespace IS {
    // Constructor definition
    Vector4D::Vector4D(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    // Operator overloading
    Vector4D& Vector4D::operator+=(const Vector4D& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4D& Vector4D::operator-=(const Vector4D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4D& Vector4D::operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    Vector4D& Vector4D::operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    Vector4D Vector4D::operator-() const {
        return Vector4D(-x, -y, -z, -w);
    }

    Vector4D operator+(const Vector4D& lhs, const Vector4D& rhs) {
        return Vector4D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    Vector4D operator-(const Vector4D& lhs, const Vector4D& rhs) {
        return Vector4D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    Vector4D operator*(const Vector4D& lhs, float rhs) {
        return Vector4D(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }

    Vector4D operator*(float lhs, const Vector4D& rhs) {
        return Vector4D(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
    }

    Vector4D operator/(const Vector4D& lhs, float rhs) {
        return Vector4D(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
    }

    bool operator==(const Vector4D& lhs, const Vector4D& rhs) {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w);
    }

    // Additional functionalities

    void ISVector4DNormalize(Vector4D& pResult, const Vector4D& pVec0) {
        pResult = pVec0;
        float magnitude = sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z + pVec0.w * pVec0.w);
        if (magnitude != 0.0f) {
            pResult /= magnitude;
        }
    }

    float ISVector4DLength(const Vector4D& pVec0) {
        return sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z + pVec0.w * pVec0.w);
    }

    float ISVector4DSquareLength(const Vector4D& pVec0) {
        return pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z + pVec0.w * pVec0.w;
    }

    float ISVector4DDotProduct(const Vector4D& pVec0, const Vector4D& pVec1) {
        return pVec0.x * pVec1.x + pVec0.y * pVec1.y + pVec0.z * pVec1.z + pVec0.w * pVec1.w;
    }

} // end namespace IS