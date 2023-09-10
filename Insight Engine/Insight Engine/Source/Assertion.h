///////////////////////////////////////////////////////////////////////////////////////
//
//	Assertion.h
//	Error handling and assertions
//	
//	Authors:  Guo Yiming
//	Copyright 2023, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_ASSERTION_H
#define GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_ASSERTION_H

// Remove this line to disable asserts
#define IS_ENABLE_ASSERTS

#ifdef IS_ENABLE_ASSERTS

// Engine-side assert
// ---------------------------------------------------------------------------

    #define IS_CORE_ASSERT(x) {                                               \
        if (!x) {                                                             \
            IS_CORE_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                           #x, __LINE__, __FUNCSIG__, __FILE__);              \
            __debugbreak();                                                   \
        }                                                                     \
    }                                                                         \

// ---------------------------------------------------------------------------

    #define IS_CORE_ASSERT_MESG(x, ...) {                                     \
        if (!x) {                                                             \
            IS_CORE_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                           #x, __LINE__, __FUNCSIG__, __FILE__);              \
            IS_CORE_ERROR(__VA_ARGS__);                                       \
            __debugbreak();                                                   \
        }                                                                     \
    }                                                                         \

// Client-side assert
// ---------------------------------------------------------------------------

    #define IS_ASSERT(x) {                                               \
        if (!x) {                                                        \
            IS_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                      #x, __LINE__, __FUNCSIG__, __FILE__);              \
            __debugbreak();                                              \
        }                                                                \
    }                                                                    \

// ---------------------------------------------------------------------------

#define IS_ASSERT_MESG(x, ...) {                                         \
        if (!x) {                                                        \
            IS_ERROR("IS_CORE_ASSERT: {}\nLine: {}\nFunc: {}\nFile: {}", \
                      #x, __LINE__, __FUNCSIG__, __FILE__);              \
            IS_ERROR(__VA_ARGS__);                                       \
            __debugbreak();                                              \
        }                                                                \
    }                                                                    \

#else
    #define IS_CORE_ASSERT(x) ((void)0)
    #define IS_CORE_ASSERT_MESG(x, ...) ((void)0)
    #define IS_ASSERT(x) ((void)0)
    #define IS_ASSERT_MESG(x, ...) ((void)0)

#endif // IS_ENABLE_ASSERTS

#endif // !GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_ASSERTION_H
