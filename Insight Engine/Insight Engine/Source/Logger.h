/*!
 * \file Logger.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This header file declares the interface for class Logger, which encapsulates
 *      the functionalities of a Logger, and class LoggerGUI, which encapsulates the
 *      functionalities of Logger graphical user interface.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_DEBUG_LOGGER_H
#define GAM200_INSIGHT_ENGINE_DEBUG_LOGGER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <format>

// dependencies
#include <imgui.h> // for LoggerGUI

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

        class LoggerGUI {
        public:
            LoggerGUI();

            void Clear();
            void AddLog(const char* fmt, ...);
            void Draw(const char* title);
            //ImU32 getLogLevelColor(aLogLevel log_level);

        private:
            ImGuiTextBuffer mBuffer;
            ImGuiTextFilter mFilter;
            ImVector<int>   mLineOffsets;   // index to line offset
            bool            mAutoScroll;    // keep scrolling if already at the bottom
            std::vector<std::string> mDefaultFilters = { "Trace", "Debug", "Info", "Warning", "Error", "Critical" };
            int mSelectedFilterIndex = 2;  // Default filter index (e.g., "Info")
        };

        // Constructor/Destructor
        Logger(std::string const& name = "");
        ~Logger();

        // Log functions
        template<typename... Args>
        void Trace(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void Debug(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void Info(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void Warn(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void Error(std::string_view fmt, Args&&... args);
        template<typename... Args>
        void Critical(std::string_view fmt, Args&&... args);

        // Getter/setter
        void SetLoggerName(std::string const& new_logger_name);
        void SetLogLevel(aLogLevel new_level);
        aLogLevel GetLogLevel() const;
        void SetTimestampFormat(std::string const& new_timestamp_format);
        std::string GetTimestampFormat() const;

        // Additional options
        void EnableFileOutput();

        static LoggerGUI& GetLoggerGUI();

    private:

        // Data members
        std::string mLoggerName;
        aLogLevel mLogLevel = aLogLevel::Info;
        std::string mTimestampFormat = "%H:%M:%S";
        std::string mLogFilenameTimestampFormat = "%Y%m%d %H-%M-%S";
        std::ofstream mLogFile;
        //std::deque<std::pair<aLogLevel, std::string>> log_messages;
        static LoggerGUI mLoggerGUI;

        static std::mutex mLogMutex;

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
    inline void Logger::Trace(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Trace, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::Debug(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Debug, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::Info(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Info, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::Warn(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Warning, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::Error(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Error, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::Critical(std::string_view fmt, Args&&... args) {
        log(aLogLevel::Critical, fmt, args...);
    }

    template<typename... Args>
    inline void Logger::log(aLogLevel level, std::string_view fmt, Args&&... args) {
        std::scoped_lock lock(mLogMutex);
        if (level >= mLogLevel) {
            setColor(level);
            
            // Construct log
            std::ostringstream log;
            std::string timestamp = getTimestamp(mTimestampFormat);
            std::string name = !mLoggerName.empty() ? '[' + mLoggerName + ']' : "";
            std::string loglevel = getLogLevelString(level);
            std::string all_args = std::vformat(fmt, std::make_format_args(args...));

            log << timestamp << name << loglevel << ' ' << all_args << '\n';

            // Print to console
            std::clog << log.str() << RESET;

            // Write to file
            if (mLogFile.is_open()) {
                mLogFile << log.str();
            }

        #ifdef USING_IMGUI
            mLoggerGUI.AddLog(log.str().c_str());
        #endif // USING_IMGUI
        }
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_LOGGER_H
