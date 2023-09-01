///////////////////////////////////////////////////////////////////////////////////////
//
//	DebugHandler.h
//	Error handling and assertions
//	
//	Authors:  Guo Yiming
//	Copyright 2023, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_DEBUGHANDLER_H
#define GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_DEBUGHANDLER_H

// #define IS_ENABLE_ASSERTS

#ifdef IS_ENABLE_ASSERTS
    #define IS_CORE_ASSERT(x, ...) { if(!x) { IS_CORE_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }
    #define IS_ASSERT(x, ...) { if(!x) { IS_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }
#else
    #define IS_CORE_ASSERT(x, ...) ((void)0)
    #define IS_CORE_ASSERT(x, ...) ((void)0)
#endif // INSIGHT_ENGINE_ENABLE_ASSERTS

#endif // !GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_DEBUGHANDLER_H
