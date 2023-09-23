/*!
 * \file Log.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This header file declares the interface for class Log which encapsulates
 *      the functionalities of an Engine-side log and a Client-side log.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_DEBUG_LOG_H
#define GAM200_INSIGHT_ENGINE_DEBUG_LOG_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Logger.h"
#include <memory>   // std::shared_ptr

namespace IS {

    class Log {
    public:
        static void Init(bool enable_output = false);
        inline static std::shared_ptr<Logger>& GetCoreLogger() { return mCoreLogger; }
        inline static std::shared_ptr<Logger>& GetClientLogger() { return mClientLogger; }
    private:
        static std::shared_ptr<Logger> mCoreLogger;
        static std::shared_ptr<Logger> mClientLogger;
    };

} // end namespace IS

// Core log macros
#define IS_CORE_TRACE(...) ::IS::Log::GetCoreLogger()->Trace(__VA_ARGS__)
#define IS_CORE_DEBUG(...) ::IS::Log::GetCoreLogger()->Debug(__VA_ARGS__)
#define IS_CORE_INFO(...) ::IS::Log::GetCoreLogger()->Info(__VA_ARGS__)
#define IS_CORE_WARN(...) ::IS::Log::GetCoreLogger()->Warn(__VA_ARGS__)
#define IS_CORE_ERROR(...) ::IS::Log::GetCoreLogger()->Error(__VA_ARGS__)
#define IS_CORE_CRITICAL(...) ::IS::Log::GetCoreLogger()->Critical(__VA_ARGS__)

// Client log macros
#define IS_TRACE(...) ::IS::Log::GetClientLogger()->Trace(__VA_ARGS__)
#define IS_DEBUG(...) ::IS::Log::GetClientLogger()->Debug(__VA_ARGS__)
#define IS_INFO(...) ::IS::Log::GetClientLogger()->Info(__VA_ARGS__)
#define IS_WARN(...) ::IS::Log::GetClientLogger()->Warn(__VA_ARGS__)
#define IS_ERROR(...) ::IS::Log::GetClientLogger()->Error(__VA_ARGS__)
#define IS_CRITICAL(...) ::IS::Log::GetClientLogger()->Critical(__VA_ARGS__)

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H
