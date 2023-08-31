/******************************************************************************/
/*!
\file		Vector2D.h
\author 	Wu Zekai, zekai.wu, 2201764
\par    	email: zekai.wu@digipen.edu
\date   	March 23, 2023
\brief		This file contains the declaration of functions of vector basic calculation

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once

namespace IE
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

/**************************************************************************/
/*!

 */
 /**************************************************************************/
	typedef union Vector2D
	{
		struct
		{
			float x, y;
		};

		float m[2];

		// Constructors
		Vector2D() : x(0.0f), y(0.0f) {}
		Vector2D(float _x, float _y);

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

	} Vector2D, Vec2, Point2D, Pt2;


#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

// Binary operators
	Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs);
	Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs);
	Vector2D operator * (const Vector2D& lhs, float rhs);
	Vector2D operator * (float lhs, const Vector2D& rhs);
	Vector2D operator / (const Vector2D& lhs, float rhs);

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void	Vector2DNormalize(Vector2D& pResult, const Vector2D& pVec0);

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vector2DLength(const Vector2D& pVec0);

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector2DSquareLength(const Vector2D& pVec0);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	 */
	 /**************************************************************************/
	float	Vector2DDistance(const Vector2D& pVec0, const Vector2D& pVec1);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector2DSquareDistance(const Vector2D& pVec0, const Vector2D& pVec1);

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector2DDotProduct(const Vector2D& pVec0, const Vector2D& pVec1);

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector2DCrossProductMag(const Vector2D& pVec0, const Vector2D& pVec1);
}