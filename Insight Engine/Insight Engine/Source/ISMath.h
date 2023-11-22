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
