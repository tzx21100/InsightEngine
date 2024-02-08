/* Start Header **************************************************************/
/*!
 * \file Vector3D.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2401
 * \date 25-11-2023
 * \brief
 * Declaration of the Vector3D class for representing 3D vectors and
 * provides various vector operations.
 *
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
/* End Header ****************************************************************/
 /*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR3D_H
#define GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR3D_H

/*                                                                   includes
----------------------------------------------------------------------------- */

namespace IS {
#ifdef _MSC_VER
#pragma warning(disable : 4201) // Supress warning: nonstandard extension used : nameless struct/union
#endif
    /*!
     * \struct Vector3D
     * \brief Represents a 3-dimensional vector.
     *
     * This struct provides functionality for basic vector operations in a 3D space,
     * including arithmetic operations, normalization, dot product, and cross product.
     */
    struct Vector3D {
        float x, y, z;

        // Constructors
#pragma warning(push)
#pragma warning(disable : 26495)
        /*!
         * \brief Default constructor that initializes to 0 for x,y,z.
         */
        Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}

        /*!
         * \brief Constructor that initializes the vector to given coordinates.
         * \param _x X-coordinate.
         * \param _y Y-coordinate.
         * \param _z Z-coordinate.
         */
        Vector3D(float _x, float _y, float _z);
#pragma warning(pop)
        // Use default copy constructor
        Vector3D(const Vector3D& rhs) = default;
        // Use default copy assignment constructor
        Vector3D& operator=(const Vector3D& rhs) = default;

        /*!
         * \brief Adds another vector to this vector.
         * \param rhs Right-hand side vector to add.
         * \return Reference to this vector after addition.
         */
        Vector3D& operator+=(const Vector3D& rhs);

        /*!
         * \brief Subtracts another vector by this vector.
         * \param rhs Right-hand side vector to subtract from.
         * \return Reference to this vector after subtraction.
         */
        Vector3D& operator-=(const Vector3D& rhs);

        /*!
         * \brief Multiples vector by rhs value.
         * \param rhs Right-hand side value to multiply by.
         * \return Reference to this vector after multiplication.
         */
        Vector3D& operator*=(float rhs);

        /*!
         * \brief Divides vector by rhs value.
         * \param rhs Right-hand side value to divide by.
         * \return Reference to this vector after division.
         */
        Vector3D& operator/=(float rhs);

        /*!
         * \brief Negates the vector.
         * \return negated vector.
         */
        Vector3D operator-() const;
    };

    /*!
     * \brief Overloads the + operator for vector addition.
     * \param lhs Left-hand side vector.
     * \param rhs Right-hand side vector.
     * \return A new vector which is the sum of lhs and rhs.
     */
    Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs);

    /*!
     * \brief Overloads the - operator for vector subtraction.
     * \param lhs Left-hand side vector.
     * \param rhs Right-hand side vector.
     * \return A new vector which is the difference of lhs and rhs.
     */
    Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs);

    /*!
     * \brief Overloads the * operator for scalar multiplication.
     * \param lhs Vector to multiply.
     * \param rhs Scalar value to multiply with.
     * \return A new vector which is the product of lhs and rhs.
     */
    Vector3D operator*(const Vector3D& lhs, float rhs);

    /*!
     * \brief Overloads the * operator for scalar multiplication.
     * \param lhs Scalar value to multiply with.
     * \param rhs Vector to multiply.
     * \return A new vector which is the product of lhs and rhs.
     */
    Vector3D operator*(float lhs, const Vector3D& rhs);

    /*!
     * \brief Overloads the / operator for scalar division.
     * \param lhs Vector to divide.
     * \param rhs Scalar value to divide by.
     * \return A new vector which is the quotient of lhs divided by rhs.
     */
    Vector3D operator/(const Vector3D& lhs, float rhs);

    /*!
     * \brief Overloads the == operator for vector comparison.
     * \param lhs Left-hand side vector.
     * \param rhs Right-hand side vector.
     * \return True if lhs and rhs are equal, false otherwise.
     */
    bool operator==(const Vector3D& lhs, const Vector3D& rhs);

    /*!
     * \brief Normalizes the given vector.
     * \param pResult Vector to store the result.
     * \param pVec0 Vector to normalize.
     */
    void ISVector3DNormalize(Vector3D& pResult, const Vector3D& pVec0);

    /*!
     * \brief Calculates the length of the vector.
     * \param pVec0 Vector to calculate the length of.
     * \return The length of pVec0.
     */
    float ISVector3DLength(const Vector3D& pVec0);

    /*!
     * \brief Calculates the square of the length of the vector.
     * \param pVec0 Vector to calculate the square length of.
     * \return The square length of pVec0.
     */
    float ISVector3DSquareLength(const Vector3D& pVec0);

    /*!
     * \brief Calculates the distance between two vectors.
     * \param pVec0 First vector.
     * \param pVec1 Second vector.
     * \return The distance between pVec0 and pVec1.
     */
    float ISVector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1);

    /*!
     * \brief Calculates the square of the distance between two vectors.
     * \param pVec0 First vector.
     * \param pVec1 Second vector.
     * \return The square distance between pVec0 and pVec1.
     */
    float ISVector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1);

    /*!
     * \brief Calculates the dot product of two vectors.
     * \param pVec0 First vector.
     * \param pVec1 Second vector.
     * \return The dot product of pVec0 and pVec1.
     */
    float ISVector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1);

    /*!
     * \brief Calculates the cross product of two vectors.
     * \param pVec0 First vector.
     * \param pVec1 Second vector.
     * \return The cross product of pVec0 and pVec1.
     */
    Vector3D ISVector3DCrossProduct(const Vector3D& pVec0, const Vector3D& pVec1);

}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_MATH_VECTOR3D_H