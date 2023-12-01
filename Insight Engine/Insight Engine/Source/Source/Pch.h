/*
This is a precompiled header, this is where we store header files that we most likely won't touch.
But we will use them alot so this helps with compile times.
We should put things like library headers here.
Don't put too many files that will make you confuse. Mostly files like STL (strings vectors etc).
*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_

//we can switch these to our own STL when we finish them

// Standard library
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <variant>
#include <cmath>
#include <limits>
#include <chrono>
#include <memory>
#include <ranges>
#include <set>
#include <bitset>
#include <array>
#include <vector>
#include <queue>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <functional>
#include <filesystem>

// Project files
#include "Debug/Logging/Log.h"
#include "Debug/Utils/Assertion.h"
#include "Debug/Profiling/Timer.h"
#include "Utils.h"

#endif //GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
