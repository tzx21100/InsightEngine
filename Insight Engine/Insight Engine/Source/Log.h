#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H

#include "Logger.h"
#include <memory>   // std::shared_ptr

namespace IS {

    class Log {
    public:
        static void init(bool enable_output = false);
        inline static std::shared_ptr<Logger>& getCoreLogger() { return core_logger; }
        inline static std::shared_ptr<Logger>& getClientLogger() { return client_logger; }
    private:
        static std::shared_ptr<Logger> core_logger;
        static std::shared_ptr<Logger> client_logger;
    };

} // end namespace IS

// Core log macros
#define IS_CORE_TRACE(x, ...) ::IS::Log::getCoreLogger()->trace(x, __VA_ARGS__)
#define IS_CORE_DEBUG(x, ...) ::IS::Log::getCoreLogger()->debug(x, __VA_ARGS__)
#define IS_CORE_INFO(x, ...) ::IS::Log::getCoreLogger()->info(x, __VA_ARGS__)
#define IS_CORE_WARN(x, ...) ::IS::Log::getCoreLogger()->warn(x, __VA_ARGS__)
#define IS_CORE_ERROR(x, ...) ::IS::Log::getCoreLogger()->error(x, __VA_ARGS__)
#define IS_CORE_CRITICAL(x, ...) ::IS::Log::getCoreLogger()->critical(x, __VA_ARGS__)

// Client log macros
#define IS_TRACE(x, ...) ::IS::Log::getClientLogger()->trace(x, __VA_ARGS__)
#define IS_DEBUG(x, ...) ::IS::Log::getClientLogger()->debug(x, __VA_ARGS__)
#define IS_INFO(x, ...) ::IS::Log::getClientLogger()->info(x, __VA_ARGS__)
#define IS_WARN(x, ...) ::IS::Log::getClientLogger()->warn(x, __VA_ARGS__)
#define IS_ERROR(x, ...) ::IS::Log::getClientLogger()->error(x, __VA_ARGS__)
#define IS_CRITICAL(x, ...) ::IS::Log::getClientLogger()->critical(x, __VA_ARGS__)

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H
