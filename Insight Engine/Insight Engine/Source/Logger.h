#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H

#include <string>
#include <sstream>
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
        void trace(Args&&... args);
        template<typename... Args>
        void debug(Args&&... args);
        template<typename... Args>
        void info(Args&&... args);
        template<typename... Args>
        void warn(Args&&... args);
        template<typename... Args>
        void error(Args&&... args);
        template<typename... Args>
        void critical(Args&&... args);

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
        void log(aLogLevel level, Args&&... args);

        std::string getTimestamp(std::string const& ts_format) const;
        void closeFile();

        // Static functions
        static std::string getLogLevelString(aLogLevel level);
        static void setColor(aLogLevel level);
    };

    // Template implementation
    template<typename... Args>
    inline void Logger::trace(Args&&... args) {
        log(aLogLevel::Trace, args...);
    }

    template<typename... Args>
    inline void Logger::debug(Args&&... args) {
        log(aLogLevel::Debug, args...);
    }

    template<typename... Args>
    inline void Logger::info(Args&&... args) {
        log(aLogLevel::Info, args...);
    }

    template<typename... Args>
    inline void Logger::warn(Args&&... args) {
        log(aLogLevel::Warning, args...);
    }

    template<typename... Args>
    inline void Logger::error(Args&&... args) {
        log(aLogLevel::Error, args...);
    }

    template<typename... Args>
    inline void Logger::critical(Args&&... args) {
        log(aLogLevel::Critical, args...);
    }

    template<typename... Args>
    inline void Logger::log(aLogLevel level, Args&&... args) {
        if (level >= log_level) {
            std::scoped_lock lock(log_mutex);
            if (colors_enabled)
                setColor(level);
            
            // Construct log
            std::ostringstream oss;
            std::string timestamp = getTimestamp(timestamp_format);
            std::string name = !logger_name.empty() ? '[' + logger_name + ']' : "";
            std::string loglevel = !colors_enabled ? getLogLevelString(level) : "";
            std::ostringstream all_args;
            (all_args << ... << args);
            oss << timestamp << name << loglevel << ' ' << all_args.str();

            // Print to console
            std::cout << oss.str() << RESET << std::endl;

            // Write to file
            if (log_file.is_open()) {
                log_file << timestamp << all_args.str() << std::endl;
            }
        }
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
