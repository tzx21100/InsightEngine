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
#include <iostream>
#include <string>
#include <cstddef>          // for size_t
#include <initializer_list> // for std::initializer_list
#include <algorithm>
#include <cstdint>
#include <cassert>
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

// Project files
#include "Log.h"
#include "Assertion.h"
#include "Timer.h"
#include "ISContainers.h"
#include "Audio.h"
#include "Asset.h"
#include "ISMath.h"
#include "Collision.h"
#include "Body.h"
#include "Grid.h"
#include "Manifold.h"
#include "Collider.h"
#include "Physics.h"
#include "CollisionSystem.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Transform.h"
#include "Camera.h"
#include "Camera3D.h"
#include "Input.h"
#include "Text.h"
#include "Graphics.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "AIFSM.h"
#include "GameGui.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "Particle.h"
#endif //GAM200_INSIGHT_ENGINE_SOURCE_PCH_H_
