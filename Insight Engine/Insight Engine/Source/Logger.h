#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H

#include <string>
#include <mutex>
#include <iostream>
#include <fstream>

namespace IS {

    // ANSI escape codes for console formatting for color/style
    const std::string RESET = "\033[m";
    const std::string BOLD = "\033[1m";
    const std::string WHITE = "\033[37m";
    const std::string CYAN = "\033[36m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW_BOLD = "\033[33m\033[1m";
    const std::string RED_BOLD = "\033[31m\033[1m";
    const std::string BOLD_ON_RED = "\033[1m\033[41m";

    // Severity level of log
    enum class aLogLevel : int{
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    // Simple thread-safe logger
    class Logger {
    public:
        // Constructor/Destructor
        Logger(std::string const& name = "");
        ~Logger();

        // Log functions
        template<typename... Args>
        void trace(Args const&... args);
        template<typename... Args>
        void debug(Args const&... args);
        template<typename... Args>
        void info(Args const&... args);
        template<typename... Args>
        void warn(Args const&... args);
        template<typename... Args>
        void error(Args const&... args);
        template<typename... Args>
        void critical(Args const&... args);

        // Getter/setter
        void setLoggerName(std::string const& new_logger_name);
        void setLogLevel(aLogLevel new_level);
        aLogLevel getLogLevel() const;
        void setTimestampFormat(std::string const& new_timestamp_format);
        std::string getTimestampFormat() const;
        void setLogDirectory(std::string const& new_log_directory);
        std::string getLogDirectory() const;

        // Additional options
        void enableColors(bool flag);
        void enableFileOutput();
    private:
        // Data members
        std::string logger_name;
        aLogLevel log_level = aLogLevel::Info;
        bool colors_enabled = true;
        std::string timestamp_format = "%H:%M:%S";
        std::string log_filename_timestamp_format = "%Y-%m-%d %H:%M:%S";
        std::string log_directory = "Logs";
        std::string log_filepath;
        std::ofstream log_file;

        static std::mutex log_mutex;

        template<typename... Args>
        void log(aLogLevel level, Args const&... args);

        std::string getTimestamp(std::string const& ts_format) const;
        void closeFile();

        // Static functions
        static void printLogLevel(aLogLevel level);
        static void setColor(aLogLevel level);
    };

    // Template implementation
    template<typename... Args>
    inline void Logger::trace(Args const&... args) {
        log(aLogLevel::Trace, args...);
    }

    template<typename... Args>
    inline void Logger::debug(Args const&... args) {
        log(aLogLevel::Debug, args...);
    }

    template<typename... Args>
    inline void Logger::info(Args const&... args) {
        log(aLogLevel::Info, args...);
    }

    template<typename... Args>
    inline void Logger::warn(Args const&... args) {
        log(aLogLevel::Warning, args...);
    }

    template<typename... Args>
    inline void Logger::error(Args const&... args) {
        log(aLogLevel::Error, args...);
    }

    template<typename... Args>
    inline void Logger::critical(Args const&... args) {
        log(aLogLevel::Critical, args...);
    }

    template<typename... Args>
    inline void Logger::log(aLogLevel level, Args const&... args) {
        if (level >= log_level) {
            std::scoped_lock lock(log_mutex);
            // Write to console
            if (colors_enabled)
                setColor(level);
            std::cout << getTimestamp(timestamp_format) << " ";
            if (!logger_name.empty())
                std::cout << "[" << logger_name << "] ";
            if (!colors_enabled)
                printLogLevel(level);
            (std::cout << ... << args) << RESET << std::endl;

            // Write to file
            if (log_file.is_open()) {
                log_file << getTimestamp(timestamp_format) << " ";
                (log_file << ... << args) << std::endl;
            }
        }
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
