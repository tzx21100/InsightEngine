/*
This is a precompiled header, this is where we store header files that we most likely won't touch.
But we will use them alot so this helps with compile times.
We should put things like library headers here.
Don't put too many files that will make you confuse. Mostly files like STL (strings vectors etc).
*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_

//we can switch these to our own STL when we finish them
//#include "MemoryLeakCheck.h"
#include "Properties.h"
#include "Log.h"
#include "Assertion.h"
#include <iostream>
#include <string>
#include "ISVector.h"
#include "ISString.h"
#include "ISArray.h"
#include "Audio.h"
#include "Asset.h"
#include <cstddef>          // for size_t
#include <initializer_list> // for std::initializer_list
#include <algorithm>
#include <cstdint>
#include <bitset>
#include <queue>
#include <deque>
#include <cassert>
#include <array>
#include <set>
#include <unordered_map>
#include <vector>
#include <variant>
#include "Math.h"
#include "Matrix3x3.h"
#include "Vector2D.h"
#include "Body.h"
#include "Collision.h"
#include "Physics.h"

#endif //GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_