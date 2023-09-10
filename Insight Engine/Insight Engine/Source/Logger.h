#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H

#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <format>

namespace IS {

    // ANSI escape codes for console formatting for color/style
    const char RESET[] = "\033[m";
    const char BOLD[] = "\033[1m";
    const char WHITE[] = "\033[37m";
    const char CYAN[] = "\033[36m";
    const char GREEN[] = "\033[32m";
    const char YELLOW_BOLD[] = "\033[33m\033[1m";
    const char RED_BOLD[] = "\033[31m\033[1m";
    const char BOLD_ON_RED[] = "\033[1m\033[41m";

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
        void trace(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void debug(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void info(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void warn(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void error(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void critical(std::string_view fmt, Args&&... args);

        // Getter/setter
        void setLoggerName(std::string const& new_logger_name);
        void setLogLevel(aLogLevel new_level);
        aLogLevel getLogLevel() const;
        void setTimestampFormat(std::string const& new_timestamp_format);
        std::string getTimestampFormat() const;

        // Additional options
        void enableColors(bool flag);
        void enableFileOutput();

    private:
        // Data members
        std::string logger_name;
        aLogLevel log_level = aLogLevel::Info;
        bool colors_enabled = true;
        std::string timestamp_format = "%H:%M:%S";
        std::string log_filename_timestamp_format = "%Y%m%d %H-%M-%S";
        std::ofstream log_file;

        static std::mutex log_mutex;

        template<typename... Args>
        void log(aLogLevel level, std::string_view fmt, Args&&... args);

        std::string getTimestamp(std::string const& ts_format) const;
        void closeFile();

        // Static functions
        static std::string getLogLevelString(aLogLevel level);
        static void setColor(aLogLevel level);
    };

    // Template implementation
    template<typename... Args>
    inline void Logger::trace(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Trace, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::debug(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Debug, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::info(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Info, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::warn(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Warning, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::error(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Error, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::critical(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Critical, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::log(aLogLevel level, std::string_view fmt, Args&&... args) {
        if (level >= log_level) {
            std::scoped_lock lock(log_mutex);
            if (colors_enabled)
                setColor(level);
            
            // Construct log
            std::ostringstream log;
            std::string timestamp = getTimestamp(timestamp_format);
            std::string name = !logger_name.empty() ? '[' + logger_name + ']' : "";
            std::string loglevel = !colors_enabled ? getLogLevelString(level) : "";
            std::string all_args = std::vformat(fmt, std::make_format_args(args...));

            log << timestamp << name << loglevel << ' ' << all_args;

            // Print to console
            std::clog << log.str() << RESET << std::endl;

            // Write to file
            if (log_file.is_open()) {
                log_file << timestamp << loglevel << ' ' << all_args << std::endl;
            }
        }
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
