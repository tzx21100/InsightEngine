/*!
 * \file Vector3D.cpp
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * Definition of the Vector3D class for representing 3D vectors and
 * provides various vector operations.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
 /*                                                                   includes
  ----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Vector3D.h"

namespace IS {
    // Constructor definition
    Vector3D::Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    // Operator overloading
    Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3D& Vector3D::operator-=(const Vector3D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3D& Vector3D::operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vector3D& Vector3D::operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    Vector3D Vector3D::operator-() const {
        return Vector3D(-x, -y, -z);
    }

    Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs) {
        return Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs) {
        return Vector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    Vector3D operator*(const Vector3D& lhs, float rhs) {
        return Vector3D(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }

    Vector3D operator*(float lhs, const Vector3D& rhs) {
        return Vector3D(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }

    Vector3D operator/(const Vector3D& lhs, float rhs) {
        return Vector3D(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
    }

    bool operator==(const Vector3D& lhs, const Vector3D& rhs) {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
    }

    // Additional functionalities
    void ISVector3DNormalize(Vector3D& pResult, const Vector3D& pVec0) {
        pResult = pVec0;
        float magnitude = sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z);
        if (magnitude != 0.0f) {
            pResult /= magnitude;
        }
    }

    float ISVector3DLength(const Vector3D& pVec0) {
        return sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z);
    }

    float ISVector3DSquareLength(const Vector3D& pVec0) {
        return pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z;
    }

    float ISVector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1) {
        return ISVector3DLength(pVec1 - pVec0);
    }

    float ISVector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1) {
        return ISVector3DSquareLength(pVec1 - pVec0);
    }

    float ISVector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1) {
        return pVec0.x * pVec1.x + pVec0.y * pVec1.y + pVec0.z * pVec1.z;
    }

    Vector3D ISVector3DCrossProduct(const Vector3D& pVec0, const Vector3D& pVec1) {
        return Vector3D((pVec0.y * pVec1.z) - (pVec0.z * pVec1.y),
            (pVec0.z * pVec1.x) - (pVec0.x * pVec1.z),
            (pVec0.x * pVec1.y) - (pVec0.y * pVec1.x));
    }

} // end namespace IS