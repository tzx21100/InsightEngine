/*!
 * \file Log.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class Log which encapsulates
 * the functionalities of an Engine-side log and a Client-side log.
 * 
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
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

    /*!
     * \brief The Log class provides logging functionality.
     *
     * This class provides static methods to initialize the logging system and obtain
     * core and client loggers.
     */
    class Log {
    public:
        /*!
         * \brief Deleted default constructor for the Log class.
         *
         * The default constructor is deleted, preventing the creation of instances of the Log class.
         */
        Log() = delete;

        /*!
         * \brief Deleted destructor for the Log class.
         *
         * The destructor is deleted, preventing the destruction of instances of the Log class.
         */
        ~Log() = delete;

        /*!
         * \brief Initialize the logging system.
         *
         * This method initializes the logging system.
         *
         * \param enable_output Whether to enable logging output (default is false).
         */
        static void Init(bool enable_output = false);

        /*!
         * \brief Get the core logger instance.
         *
         * \return A shared pointer to the core logger instance.
         */
        inline static std::shared_ptr<Logger>& GetCoreLogger() { return mCoreLogger; }

        /*!
         * \brief Get the client logger instance.
         *
         * \return A shared pointer to the client logger instance.
         */
        inline static std::shared_ptr<Logger>& GetClientLogger() { return mClientLogger; }

    private:

        /*!
         * \brief The core logger instance.
         */
        static std::shared_ptr<Logger> mCoreLogger;

        /*!
         * \brief The client logger instance.
         */
        static std::shared_ptr<Logger> mClientLogger;
    };

} // end namespace IS

// Core log macros
#define IS_CORE_TRACE(...) ::IS::Log::GetCoreLogger()->Trace(__VA_ARGS__)
#ifdef _DEBUG
#define IS_CORE_DEBUG(...) ::IS::Log::GetCoreLogger()->Debug(__VA_ARGS__)
#else
#define IS_CORE_DEBUG(...) (void)(__VA_ARGS__)
#endif
#define IS_CORE_INFO(...) ::IS::Log::GetCoreLogger()->Info(__VA_ARGS__)
#define IS_CORE_WARN(...) ::IS::Log::GetCoreLogger()->Warn(__VA_ARGS__)
#define IS_CORE_ERROR(...) ::IS::Log::GetCoreLogger()->Error(__VA_ARGS__)
#define IS_CORE_CRITICAL(...) ::IS::Log::GetCoreLogger()->Critical(__VA_ARGS__)

// Client log macros
#define IS_TRACE(...) ::IS::Log::GetClientLogger()->Trace(__VA_ARGS__)
#ifdef _DEBUG
#define IS_DEBUG(...) ::IS::Log::GetClientLogger()->Debug(__VA_ARGS__)
#else
#define IS_DEBUG(...) (void)(__VA_ARGS__)
#endif
#define IS_INFO(...) ::IS::Log::GetClientLogger()->Info(__VA_ARGS__)
#define IS_WARN(...) ::IS::Log::GetClientLogger()->Warn(__VA_ARGS__)
#define IS_ERROR(...) ::IS::Log::GetClientLogger()->Error(__VA_ARGS__)
#define IS_CRITICAL(...) ::IS::Log::GetClientLogger()->Critical(__VA_ARGS__)

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H
