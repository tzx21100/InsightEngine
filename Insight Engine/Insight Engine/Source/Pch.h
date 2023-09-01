/*
This is a precompiled header, this is where we store header files that we most likely won't touch.
But we will use them alot so this helps with compile times.
We should put things like library headers here.
Don't put too many files that will make you confuse. Mostly files like STL (strings vectors etc).
*/
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_

//we can switch these to our own STL when we finish them
#include "Properties.h"
#include "Log.h"
#include <vector>
#include <iostream>
#include <string>
#include "ISVector.h"
#include "ISString.h"
#include "ISArray.h"
#include <cstddef>          // for size_t
#include <initializer_list> // for std::initializer_list
#include <algorithm>

#endif //GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_