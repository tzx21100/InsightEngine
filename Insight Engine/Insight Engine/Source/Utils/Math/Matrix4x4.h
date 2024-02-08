/*!
 * \file Matrix4x4.h
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * This header file declares the Matrix4x4 class for representing 4x4 matrices
 * and provides various matrix operations.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#ifndef GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MATRIX4X4_H
#define GAM200_INSIGHT_ENGINE_PHYSICS_SYSTEM_MATRIX4X4_H

#include "Vector3D.h"

namespace IS
{
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    /**
     * @union Matrix4x4
     * @brief Represents a 4x4 matrix and provides various matrix operations.
     */
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
        /**
         * @brief Default constructor. Initializes the matrix as an identity matrix.
         */
        Matrix4x4() : m00(1.0f), m01(0.0f), m02(0.0f), m03(0.0f),
            m10(0.0f), m11(1.0f), m12(0.0f), m13(0.0f),
            m20(0.0f), m21(0.0f), m22(1.0f), m23(0.0f),
            m30(0.0f), m31(0.0f), m32(0.0f), m33(1.0f) {}
#pragma warning(pop)
        /**
         * @brief Constructor that initializes the matrix with an array of floats.
         * @param pArr An array of floats representing the matrix.
         */
        Matrix4x4(const float* pArr);

        /**
         * @brief Constructor that initializes the matrix with individual elements.
         * @param _00 to _33 Elements of the matrix.
         */
        Matrix4x4(float _00, float _01, float _02, float _03,
            float _10, float _11, float _12, float _13,
            float _20, float _21, float _22, float _23,
            float _30, float _31, float _32, float _33);

        /**
         * @brief Copy assignment operator.
         * @param rhs The matrix to copy.
         * @return Reference to the assigned matrix.
         */
        Matrix4x4& operator=(const Matrix4x4& rhs);

        /**
         * @brief Copy constructor.
         * @param rhs The matrix to copy.
         */
        Matrix4x4(const Matrix4x4& rhs) = default;

        /**
         * @brief Compound multiplication assignment operator for matrices.
         * @param rhs The matrix to multiply with.
         * @return Reference to the resulting matrix.
         */
        Matrix4x4& operator *= (const Matrix4x4& rhs);
    } Mat4, Mtx44;

    /**
     * @brief Binary multiplication operator for matrices.
     * @param lhs The left-hand side matrix.
     * @param rhs The right-hand side matrix.
     * @return The resulting matrix.
     */
    Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs);

    /**
     * @brief Binary multiplication operator for a matrix and a vector.
     * @param pMtx The matrix.
     * @param rhs The vector.
     * @return The resulting vector.
     */
    Vector3D operator * (const Matrix4x4& pMtx, const Vector3D& rhs);

    /**
     * @brief Initializes the given matrix as an identity matrix.
     * @param pResult Pointer to the matrix to initialize.
     */
    void ISMtx44Identity(Matrix4x4& pResult);

    /**
     * @brief Translates the given matrix by the specified amounts.
     * @param pResult Pointer to the resulting matrix.
     * @param x Translation along the x-axis.
     * @param y Translation along the y-axis.
     * @param z Translation along the z-axis.
     */
    void ISMtx44Translate(Matrix4x4& pResult, float x, float y, float z);

    /**
     * @brief Scales the given matrix by the specified amounts.
     * @param pResult Pointer to the resulting matrix.
     * @param x Scaling along the x-axis.
     * @param y Scaling along the y-axis.
     * @param z Scaling along the z-axis.
     */
    void ISMtx44Scale(Matrix4x4& pResult, float x, float y, float z);

    /**
     * @brief Rotates the given matrix by the specified angle around the specified axis.
     * @param pResult Pointer to the resulting matrix.
     * @param angle Rotation angle in radians.
     * @param x X-component of the rotation axis.
     * @param y Y-component of the rotation axis.
     * @param z Z-component of the rotation axis.
     */
    void ISMtx44RotRad(Matrix4x4& pResult, float angle, float x, float y, float z);

    /**
     * @brief Rotates the given matrix by the specified angle around the specified axis.
     * @param pResult Pointer to the resulting matrix.
     * @param angle Rotation angle in degrees.
     * @param x X-component of the rotation axis.
     * @param y Y-component of the rotation axis.
     * @param z Z-component of the rotation axis.
     */
    void ISMtx44RotDeg(Matrix4x4& pResult, float angle, float x, float y, float z);

    /**
     * @brief Transposes the given matrix.
     * @param pResult Pointer to the resulting matrix.
     * @param pMtx Pointer to the input matrix.
     */
    void ISMtx44Transpose(Matrix4x4& pResult, const Matrix4x4& pMtx);

    /**
     * @brief Concatenates two matrices.
     * @param pResult Pointer to the resulting matrix.
     * @param Matrix1 The first matrix.
     * @param Matrix2 The second matrix.
     */
    void ISMtx44Concat(Matrix4x4& pResult, const Matrix4x4& Matrix1, const Matrix4x4& Matrix2);
}
#endif