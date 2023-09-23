/*!
 * \file Assertion.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file defines assertions for Insight Engine.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#ifndef GAME200_INSIGHT_ENGINE_DEBUG_ASSERTION_H
#define GAME200_INSIGHT_ENGINE_DEBUG_ASSERTION_H

#ifdef _MSC_VER
    #define IS_DEBUG_BREAK __debugbreak()
#else
    #define IS_DEBUG_BREAK do { __asm int 3 } while(0)
#endif

#if !defined(IS_ENABLE_ASSERTS) 
    #if defined(_DEBUG)
        #define IS_ENABLE_ASSERTS 1
    #else
        #define IS_ENABLE_ASSERTS 0
    #endif
#endif


#if IS_ENABLE_ASSERTS

// Engine-side assert
// ---------------------------------------------------------------------------

    #define IS_CORE_ASSERT(x) {                                               \
        if (!x) {                                                             \
            IS_CORE_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                           #x, __LINE__, __FUNCSIG__, __FILE__);              \
            IS_DEBUG_BREAK;                                                   \
        }                                                                     \
    }                                                                         \

// ---------------------------------------------------------------------------

    #define IS_CORE_ASSERT_MESG(x, ...) {                                     \
        if (!x) {                                                             \
            IS_CORE_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                           #x, __LINE__, __FUNCSIG__, __FILE__);              \
            IS_CORE_ERROR(__VA_ARGS__);                                       \
            IS_DEBUG_BREAK;                                                   \
        }                                                                     \
    }                                                                         \

// Client-side assert
// ---------------------------------------------------------------------------

    #define IS_ASSERT(x) {                                               \
        if (!x) {                                                        \
            IS_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                      #x, __LINE__, __FUNCSIG__, __FILE__);              \
            IS_DEBUG_BREAK;                                              \
        }                                                                \
    }                                                                    \

// ---------------------------------------------------------------------------

#define IS_ASSERT_MESG(x, ...) {                                         \
        if (!x) {                                                        \
            IS_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                      #x, __LINE__, __FUNCSIG__, __FILE__);              \
            IS_ERROR(__VA_ARGS__);                                       \
            IS_DEBUG_BREAK;                                              \
        }                                                                \
    }                                                                    \

#else
    #define IS_CORE_ASSERT(x) __noop
    #define IS_CORE_ASSERT_MESG(x, ...) __noop
    #define IS_ASSERT(x) __noop
    #define IS_ASSERT_MESG(x, ...) __noop

#endif // IS_ENABLE_ASSERTS

#endif // !GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_ASSERTION_H
