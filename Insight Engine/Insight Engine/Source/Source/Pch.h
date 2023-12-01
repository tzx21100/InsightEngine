/*
This is a precompiled header, this is where we store header files that we most likely won't touch.
But we will use them alot so this helps with compile times.
We should put things like library headers here.
Don't put too many files that will make you confuse. Mostly files like STL (strings vectors etc).
*/
#ifndef GAM200_INSIGHT_ENGINE_PCH_H_
#define GAM200_INSIGHT_ENGINE_PCH_H_

// Standard library
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <variant>
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

/*
 * DO NOT INCLUDE FILES HERE FOR YOUR OWN CONVENIENCE!
 * INCLUDE THE HEADERS YOU NEED IN YOUR SOURCE FILES AND DO NOT RELY ON PCH.H!
 * THIS IS TO ENSURE THAT YOU KNOW WHAT YOU NEED IN YOUR OWN SOURCE FILES AND NOT RELY ON BLIND LUCK!
 * THIS IS A FRIENDLY REMINDER, THOSE WHO DO NOT ADHERE TO THE ABOVE INSTRUCTIONS WILL RECEIVE REPERCUSSIONS!
 */
// Project files
#include "Debug/Logging/Log.h"
#include "Debug/Utils/Assertion.h"
#include "Debug/Profiling/Timer.h"
#include "Utils.h"

#endif //!GAM200_INSIGHT_ENGINE_PCH_H_
