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

// Remove this line to disable asserts
#define IS_ENABLE_ASSERTS

#ifdef IS_ENABLE_ASSERTS

// Engine side assert
// ---------------------------------------------------------------------------

    #define IS_CORE_ASSERT(x) {                    \
        if (!x) {                                  \
            IS_CORE_ERROR("IS_CORE_ASSERT: ", #x,  \
                          "\nLine: ", __LINE__,    \
                          "\nFunc: ", __FUNCSIG__, \
                          "\nFile: ", __FILE__);   \
            std::exit(EXIT_FAILURE);               \
        }                                          \
    }                                              \

// ---------------------------------------------------------------------------

    #define IS_CORE_ASSERT_MESG(x, y, ...) {          \
        if (!x) {                                  \
            IS_CORE_ERROR("IS_CORE_ASSERT: ", #x,  \
                          "\nLine: ", __LINE__,    \
                          "\nFunc: ", __FUNCSIG__, \
                          "\nFile: ", __FILE__);   \
            IS_CORE_ERROR(y, __VA_ARGS__);            \
            std::exit(EXIT_FAILURE);               \
        }                                          \
    }                                              \

// Client side assert
// ---------------------------------------------------------------------------

    #define IS_ASSERT(x) {                    \
        if (!x) {                             \
            IS_ERROR("IS_ASSERT: ", #x,       \
                     "\nLine: ", __LINE__,    \
                     "\nFunc: ", __FUNCSIG__, \
                     "\nFile: ", __FILE__);   \
            std::exit(EXIT_FAILURE);          \
        }                                     \
    }                                         \

// ---------------------------------------------------------------------------

#define IS_ASSERT_MESG(x, y, ...)                \
        if (!x) {                             \
            IS_ERROR("IS_ASSERT: ", #x,       \
                     "\nLine: ", __LINE__,    \
                     "\nFunc: ", __FUNCSIG__, \
                     "\nFile: ", __FILE__);   \
            IS_ERROR(y, __VA_ARGS__);            \
            std::exit(EXIT_FAILURE);          \
        }                                     \
    }                                         \

#else
    #define IS_CORE_ASSERT(x) ((void)0)
    #define IS_CORE_ASSERT_MESG(x, y, ...) ((void)0)
    #define IS_ASSERT(x) ((void)0)
    #define IS_ASSERT_MESG(x, y, ...) ((void)0)

#endif // IS_ENABLE_ASSERTS

#endif // !GAME200_INSIGHT_ENGINE_SOURCE_DEBUG_DEBUGHANDLER_H
