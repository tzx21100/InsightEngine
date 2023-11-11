#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MATRIX4X4_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MATRIX4X4_H

#include "Vector3D.h"

namespace IS
{
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    typedef union Matrix4x4
    {
        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };

        float m[16]{};
        float m2[4][4];
#pragma warning(push)
#pragma warning(disable : 26495)
        Matrix4x4() : m00(1.0f), m01(0.0f), m02(0.0f), m03(0.0f),
            m10(0.0f), m11(1.0f), m12(0.0f), m13(0.0f),
            m20(0.0f), m21(0.0f), m22(1.0f), m23(0.0f),
            m30(0.0f), m31(0.0f), m32(0.0f), m33(1.0f) {}
#pragma warning(pop)
        Matrix4x4(const float* pArr);
        Matrix4x4(float _00, float _01, float _02, float _03,
            float _10, float _11, float _12, float _13,
            float _20, float _21, float _22, float _23,
            float _30, float _31, float _32, float _33);
        Matrix4x4& operator=(const Matrix4x4& rhs);

        Matrix4x4(const Matrix4x4& rhs) = default;

        Matrix4x4& operator *= (const Matrix4x4& rhs);
    } Mat4, Mtx44;

    Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs);

    Vector3D operator * (const Matrix4x4& pMtx, const Vector3D& rhs);

    void ISMtx44Identity(Matrix4x4& pResult);

    void ISMtx44Translate(Matrix4x4& pResult, float x, float y, float z);

    void ISMtx44Scale(Matrix4x4& pResult, float x, float y, float z);

    void ISMtx44RotRad(Matrix4x4& pResult, float angle, float x, float y, float z);

    void ISMtx44RotDeg(Matrix4x4& pResult, float angle, float x, float y, float z);

    void ISMtx44Transpose(Matrix4x4& pResult, const Matrix4x4& pMtx);

    void ISMtx44Concat(Matrix4x4& pResult, const Matrix4x4& Matrix1, const Matrix4x4& Matrix2);
}
#endif