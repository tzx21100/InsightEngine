/******************************************************************************/
/*!
\file		Vector2D.cpp
\author 	Wu Zekai, zekai.wu, 2201764
\par    	email: zekai.wu@digipen.edu
\date   	March 23, 2023
\brief		This file contains the declaration of functions of vector basic calculation

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Pch.h"
#include "Vector2D.h"
#include "math.h"

namespace IE 
{
	// copy constructor
	Vector2D::Vector2D(float _x, float _y) :x{ _x }, y{ _y } {
		// empty by design
	}

	// operator overloading for +=
	Vector2D& Vector2D::operator +=(const Vector2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	// operator overloading for -=
	Vector2D& Vector2D::operator -=(const Vector2D& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	// operator overloading for *=
	Vector2D& Vector2D::operator *=(float rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	// operator overloading for /=
	Vector2D& Vector2D::operator /=(float rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}

	// operator overloading for - (unary)
	Vector2D Vector2D::operator -()const {
		return Vector2D(-x, -y);
	}

	// operator overloading for +
	Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs) {
		Vector2D ret{ lhs };
		ret.x += rhs.x;
		ret.y += rhs.y;
		return ret;
	}

	// operator overloading for -
	Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs) {
		Vector2D ret{ lhs };
		ret.x -= rhs.x;
		ret.y -= rhs.y;
		return ret;
	}

	// operator overloading for *
	Vector2D operator * (const Vector2D& lhs, float rhs) {
		Vector2D ret{ lhs };
		ret.x *= rhs;
		ret.y *= rhs;
		return ret;
	}

	// operator overloading for *
	Vector2D operator * (float lhs, const Vector2D& rhs) {
		Vector2D ret{ rhs };
		ret.x *= lhs;
		ret.y *= lhs;
		return ret;
	}

	// operator overloading for /
	Vector2D operator / (const Vector2D& lhs, float rhs) {
		Vector2D ret{ lhs };
		ret.x /= rhs;
		ret.y /= rhs;
		return ret;
	}

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void Vector2DNormalize(Vector2D& pResult, const Vector2D& pVec0) {
		pResult = pVec0;
		float magnitude = sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
		if (magnitude != 0.f) {
			pResult /= magnitude;
		}
	}

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float Vector2DLength(const Vector2D& pVec0) {
		return sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
	}

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	 */
	 /**************************************************************************/
	float Vector2DSquareLength(const Vector2D& pVec0) {
		return pVec0.x * pVec0.x + pVec0.y * pVec0.y;
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	 */
	 /**************************************************************************/
	float Vector2DDistance(const Vector2D& pVec0, const Vector2D& pVec1) {
		return sqrtf(((pVec1.x - pVec0.x) * (pVec1.x - pVec0.x)) + ((pVec1.y - pVec0.y) * (pVec1.y - pVec0.y)));
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float Vector2DSquareDistance(const Vector2D& pVec0, const Vector2D& pVec1) {
		return ((pVec1.x - pVec0.x) * (pVec1.x - pVec0.x)) + ((pVec1.y - pVec0.y) * (pVec1.y - pVec0.y));
	}

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float Vector2DDotProduct(const Vector2D& pVec0, const Vector2D& pVec1) {
		return (pVec0.x * pVec1.x) + (pVec0.y * pVec1.y);
	}

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float Vector2DCrossProductMag(const Vector2D& pVec0, const Vector2D& pVec1) {
		return (pVec0.x * pVec1.y) - (pVec0.y * pVec1.x);
	}
}