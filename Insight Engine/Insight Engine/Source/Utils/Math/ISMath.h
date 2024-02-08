/*!
 * \file ISMath.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file defines the ISMath class for representing Min and Max operations.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/


#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MATH_ISMATH_H
#define GAM200_INSIGHT_ENGINE_SOURCE_MATH_ISMATH_H

#define PI 3.141592653589f

#include "Matrix3x3.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Random.h"

template <typename T>
T Min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template <typename T>
T Max(const T& a, const T& b) {
    return (a > b) ? a : b;
}



#endif // !GAM200_INSIGHT_ENGINE_SOURCE_MATH_ISMATH_H
