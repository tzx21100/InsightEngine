/*
This is a precompiled header, this is where we store header files that we most likely won't touch.
But we will use them alot so this helps with compile times.
We should put things like library headers here.
Don't put too many files that will make you confuse. Mostly files like STL (strings vectors etc).
*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_

#if 1
#define USING_IMGUI // enable GUI
#endif

//we can switch these to our own STL when we finish them

// Standard library
#include <iostream>
#include <string>
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
#include <cmath>
#include <limits>
#include <chrono>
#include <memory>

// Project files
#include "Properties.h"
#include "Log.h"
#include "Assertion.h"
#include "ISContainers.h"
#include "Audio.h"
#include "Asset.h"
#include "ISMath.h"
#include "Collision.h"
#include "Body.h"
#include "Physics.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Transform.h"
#include "Input.h"
#include "Text.h"
#include "Graphics.h"
#include "ScriptEngine.h"


#endif //GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
