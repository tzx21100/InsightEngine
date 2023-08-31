
#include "Pch.h"
#include "Matrix3x3.h"
#include "math.h"

namespace IS
{
	// copy constructor
	Matrix3x3::Matrix3x3(const float* pArr) :m00(pArr[0]), m01(pArr[1]), m02(pArr[2]), m10(pArr[3]), m11(pArr[4]), m12(pArr[5]), m20(pArr[6]), m21(pArr[7]), m22(pArr[8]) {
		// empty by design
	}

	// copy constructor
	Matrix3x3::Matrix3x3(float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22) : m00(_00), m01(_01), m02(_02), m10(_10), m11(_11), m12(_12), m20(_20), m21(_21), m22(_22) {
		// empty by design
	}

	// copy assignment
	Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs) {
		m00 = rhs.m00;
		m01 = rhs.m01;
		m02 = rhs.m02;
		m10 = rhs.m10;
		m11 = rhs.m11;
		m12 = rhs.m12;
		m20 = rhs.m20;
		m21 = rhs.m21;
		m22 = rhs.m22;
		return *this;
	}

	// operator overloading for *=
	Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& rhs) {
		Matrix3x3 ret;

		ret.m00 = m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20;
		ret.m01 = m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21;
		ret.m02 = m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22;

		ret.m10 = m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20;
		ret.m11 = m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21;
		ret.m12 = m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22;

		ret.m20 = m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20;
		ret.m21 = m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21;
		ret.m22 = m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22;

		*this = ret;
		return *this;
	}

	// operator overloading for *
	Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs) {
		Matrix3x3 ret{ lhs };
		ret *= rhs;
		return ret;
	}

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	 */
	 /**************************************************************************/
	Vector2D  operator * (const Matrix3x3& pMtx, const Vector2D& rhs) {
		float x = (pMtx.m00 * rhs.x) + (pMtx.m01 * rhs.y) + pMtx.m02;
		float y = (pMtx.m10 * rhs.x) + (pMtx.m11 * rhs.y) + pMtx.m12;
		return Vector2D(x, y);
	}

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
	 */
	 /**************************************************************************/
	void ISMtx33Identity(Matrix3x3& pResult) {
		pResult.m00 = 1.f, pResult.m01 = 0.f, pResult.m02 = 0.f;
		pResult.m10 = 0.f, pResult.m11 = 1.f, pResult.m12 = 0.f;
		pResult.m20 = 0.f, pResult.m21 = 0.f, pResult.m22 = 1.f;
	}

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void ISMtx33Translate(Matrix3x3& pResult, float x, float y) {
		pResult.m00 = 1.f, pResult.m01 = 0.f, pResult.m02 = x;
		pResult.m10 = 0.f, pResult.m11 = 1.f, pResult.m12 = y;
		pResult.m20 = 0.f, pResult.m21 = 0.f, pResult.m22 = 1.f;
	}

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void ISMtx33Scale(Matrix3x3& pResult, float x, float y) {
		pResult.m00 = x, pResult.m01 = 0.f, pResult.m02 = 0.f;
		pResult.m10 = 0.f, pResult.m11 = y, pResult.m12 = 0.f;
		pResult.m20 = 0.f, pResult.m21 = 0.f, pResult.m22 = 1.f;
	}

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void ISMtx33RotRad(Matrix3x3& pResult, float angle) {
		pResult.m00 = cosf(angle), pResult.m01 = -sinf(angle), pResult.m02 = 0.f;
		pResult.m10 = sinf(angle), pResult.m11 = cosf(angle), pResult.m12 = 0.f;
		pResult.m20 = 0.f, pResult.m21 = 0.f, pResult.m22 = 1.f;
	}

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void ISMtx33RotDeg(Matrix3x3& pResult, float angle) {
		angle *= (PI / 180);

		float cosAngle = cosf(angle);
		float sinAngle = sinf(angle);

		pResult.m00 = cosAngle, pResult.m01 = -(sinAngle), pResult.m02 = 0.f;
		pResult.m10 = sinAngle, pResult.m11 = cosAngle, pResult.m12 = 0.f;
		pResult.m20 = 0.f, pResult.m21 = 0.f, pResult.m22 = 1.f;
	}

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
	 */
	 /**************************************************************************/
	void ISMtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx) {
		pResult.m00 = pMtx.m00, pResult.m01 = pMtx.m10, pResult.m02 = pMtx.m20;
		pResult.m10 = pMtx.m01, pResult.m11 = pMtx.m11, pResult.m12 = pMtx.m21;
		pResult.m20 = pMtx.m02, pResult.m21 = pMtx.m12, pResult.m22 = pMtx.m22;
	}

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void ISMtx33Inverse(Matrix3x3* pResult, float* determinant, const Matrix3x3& pMtx) {
		float det = pMtx.m00 * (pMtx.m11 * pMtx.m22 - pMtx.m21 * pMtx.m12) -
			pMtx.m01 * (pMtx.m10 * pMtx.m22 - pMtx.m20 * pMtx.m12) +
			pMtx.m02 * (pMtx.m10 * pMtx.m21 - pMtx.m20 * pMtx.m11);

		*determinant = det;

		if (fabs(det) <= 0.0001f)
		{
			*pResult = Matrix3x3();
			return;
		}

		Matrix3x3 inv;
		inv.m00 = (pMtx.m11 * pMtx.m22 - pMtx.m21 * pMtx.m12) / det;
		inv.m01 = (pMtx.m02 * pMtx.m21 - pMtx.m01 * pMtx.m22) / det;
		inv.m02 = (pMtx.m01 * pMtx.m12 - pMtx.m02 * pMtx.m11) / det;
		inv.m10 = (pMtx.m12 * pMtx.m20 - pMtx.m10 * pMtx.m22) / det;
		inv.m11 = (pMtx.m00 * pMtx.m22 - pMtx.m02 * pMtx.m20) / det;
		inv.m12 = (pMtx.m02 * pMtx.m10 - pMtx.m00 * pMtx.m12) / det;
		inv.m20 = (pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20) / det;
		inv.m21 = (pMtx.m01 * pMtx.m20 - pMtx.m00 * pMtx.m21) / det;
		inv.m22 = (pMtx.m00 * pMtx.m11 - pMtx.m01 * pMtx.m10) / det;

		*pResult = inv;
	}

	void ISMtx33Concat(Matrix3x3& pResult, Matrix3x3 const& Matrix1, Matrix3x3 const& Matrix2) {
		pResult = Matrix1 * Matrix2;
	}
}
