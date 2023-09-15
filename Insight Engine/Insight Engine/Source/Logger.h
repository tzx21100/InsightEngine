#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H

// STL
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <format>
#include <deque>

// dependencies
#include <imgui.h> // for LoggerGUI

namespace IS {

    // Maximum number of messages that can be stored in deque
    static constexpr int MAX_LOG_MESSAGES (100);

    // ANSI escape codes for console formatting for color/style
    /*const char RESET[] = "\033[m";
    const char BOLD[] = "\033[1m";
    const char WHITE[] = "\033[37m";
    const char CYAN[] = "\033[36m";
    const char GREEN[] = "\033[32m";
    const char YELLOW_BOLD[] = "\033[33m\033[1m";
    const char RED_BOLD[] = "\033[31m\033[1m";
    const char BOLD_ON_RED[] = "\033[1m\033[41m";*/

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

        class LoggerGUI {
        public:
            LoggerGUI();

            void clear();
            void addLog(const char* fmt, ...);
            void draw(const char* title);
            //ImU32 getLogLevelColor(aLogLevel log_level);

        private:
            ImGuiTextBuffer buffer;
            ImGuiTextFilter filter;
            ImVector<int>   line_offsets;   // index to line offset
            bool            auto_scroll;    // keep scrolling if already at the bottom
            std::vector<std::string> defaultFilters = { "Trace", "Debug", "Info", "Warning", "Error", "Critical" };
            int selectedFilterIndex = 2;  // Default filter index (e.g., "Info")
        };

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

        void clearLogMessages();

        // Additional options
        void enableFileOutput();

        static LoggerGUI& getLoggerGUI();

    private:

        // Data members
        std::string logger_name;
        aLogLevel log_level = aLogLevel::Info;
        std::string timestamp_format = "%H:%M:%S";
        std::string log_filename_timestamp_format = "%Y%m%d %H-%M-%S";
        std::ofstream log_file;
        //std::deque<std::pair<aLogLevel, std::string>> log_messages;
        static LoggerGUI logger_gui;

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
        std::scoped_lock lock(log_mutex);
        if (level >= log_level) {
            //setColor(level);
            
            // Construct log
            std::ostringstream log;
            std::string timestamp = getTimestamp(timestamp_format);
            std::string name = !logger_name.empty() ? '[' + logger_name + ']' : "";
            std::string loglevel = getLogLevelString(level);
            std::string all_args = std::vformat(fmt, std::make_format_args(args...));

            log << timestamp << name << loglevel << ' ' << all_args << '\n';

            // Print to console
            //std::clog << log.str() << RESET << std::endl;

            // Write to file
            if (log_file.is_open()) {
                log_file << timestamp << loglevel << ' ' << all_args << std::endl;
            }

            logger_gui.addLog(log.str().c_str());
        }
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
