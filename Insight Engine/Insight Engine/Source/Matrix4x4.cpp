#include "Pch.h"
#include "Matrix4x4.h"

#pragma warning(disable : 26495)

namespace IS {

    // Constructor using a float array
    Matrix4x4::Matrix4x4(const float* pArr) {
        for (int i = 0; i < 16; ++i) {
            m[i] = pArr[i];
        }
    }

    // Constructor with individual elements
    Matrix4x4::Matrix4x4(float _00, float _01, float _02, float _03,
        float _10, float _11, float _12, float _13,
        float _20, float _21, float _22, float _23,
        float _30, float _31, float _32, float _33) :
        m00(_00), m01(_01), m02(_02), m03(_03),
        m10(_10), m11(_11), m12(_12), m13(_13),
        m20(_20), m21(_21), m22(_22), m23(_23),
        m30(_30), m31(_31), m32(_32), m33(_33) {}

    // Assignment operator
    Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs) {
        if (this != &rhs) {
            for (int i = 0; i < 16; ++i) {
                m[i] = rhs.m[i];
            }
        }
        return *this;
    }

    // Matrix multiplication
    Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs) {
        Matrix4x4 result;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    sum += m2[i][k] * rhs.m2[k][j];
                }
                result.m2[i][j] = sum;
            }
        }

        *this = result;
        return *this;
    }

    // Matrix multiplication operator
    Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs) {
        Matrix4x4 result = lhs;
        result *= rhs;
        return result;
    }

    // Vector multiplication
    Vector3D operator*(const Matrix4x4& pMtx, const Vector3D& rhs) {
        Vector3D result;
        result.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02 * rhs.z + pMtx.m03;
        result.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12 * rhs.z + pMtx.m13;
        result.z = pMtx.m20 * rhs.x + pMtx.m21 * rhs.y + pMtx.m22 * rhs.z + pMtx.m23;
        return result;
    }

    // Identity matrix generation
    void ISMtx44Identity(Matrix4x4& pResult) {
        pResult = Matrix4x4(); // Using the default constructor with identity matrix values
    }

    // Translation matrix creation
    void ISMtx44Translate(Matrix4x4& pResult, float x, float y, float z) {
        pResult = Matrix4x4(
            1.0f, 0.0f, 0.0f, x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f, z,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    // Scaling matrix creation
    void ISMtx44Scale(Matrix4x4& pResult, float x, float y, float z) {
        pResult = Matrix4x4(
            x, 0.0f, 0.0f, 0.0f,
            0.0f, y, 0.0f, 0.0f,
            0.0f, 0.0f, z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    // Rotation matrix creation in radians
    void ISMtx44RotRad(Matrix4x4& pResult, float angle, float x, float y, float z) {
        // Normalize the axis vector
        float length = sqrt(x * x + y * y + z * z);
        if (length != 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }

        float c = cos(angle);
        float s = sin(angle);
        float t = 1.0f - c;

        pResult = Matrix4x4(
            t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0.0f,
            t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0.0f,
            t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    // Rotation matrix creation in degrees
    void ISMtx44RotDeg(Matrix4x4& pResult, float angle, float x, float y, float z) {
        // Convert degrees to radians and call ISMtx44RotRad
        ISMtx44RotRad(pResult, angle * 0.01745329251994329576923690768489f, x, y, z);
    }

    // Matrix transpose operation
    void ISMtx44Transpose(Matrix4x4& pResult, const Matrix4x4& pMtx) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                pResult.m2[i][j] = pMtx.m2[j][i];
            }
        }
    }


    // Matrix concatenation
    void ISMtx44Concat(Matrix4x4& pResult, const Matrix4x4& Matrix1, const Matrix4x4& Matrix2) {
        pResult = Matrix1 * Matrix2;
    }
} // end namespace IS