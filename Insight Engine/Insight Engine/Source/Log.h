#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H

#include "Logger.h"
#include <memory>   // std::shared_ptr

namespace IS {

    class Log {
    public:
        static void init();
        static void enableFileOutput();
        inline static std::shared_ptr<Logger>& getCoreLogger() { return core_logger; }
        inline static std::shared_ptr<Logger>& getClientLogger() { return client_logger; }
    private:
        static std::shared_ptr<Logger> core_logger;
        static std::shared_ptr<Logger> client_logger;
    };

} // end namespace IS

// Core log macros
#define IS_CORE_TRACE(...) ::IS::Log::getCoreLogger()->trace(__VA_ARGS__)
#define IS_CORE_DEBUG(...) ::IS::Log::getCoreLogger()->debug(__VA_ARGS__)
#define IS_CORE_INFO(...) ::IS::Log::getCoreLogger()->info(__VA_ARGS__)
#define IS_CORE_WARN(...) ::IS::Log::getCoreLogger()->warn(__VA_ARGS__)
#define IS_CORE_ERROR(...) ::IS::Log::getCoreLogger()->error(__VA_ARGS__)
#define IS_CORE_CRITICAL(...) ::IS::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define IS_TRACE(...) ::IS::Log::getClientLogger()->trace(__VA_ARGS__)
#define IS_DEBUG(...) ::IS::Log::getClientLogger()->debug(__VA_ARGS__)
#define IS_INFO(...) ::IS::Log::getClientLogger()->info(__VA_ARGS__)
#define IS_WARN(...) ::IS::Log::getClientLogger()->warn(__VA_ARGS__)
#define IS_ERROR(...) ::IS::Log::getClientLogger()->error(__VA_ARGS__)
#define IS_CRITICAL(...) ::IS::Log::getClientLogger()->critical(__VA_ARGS__)

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOG_H
