/*!
 * \file Logger.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class Logger, which encapsulates
 * the functionalities of a Logger, and class LoggerGUI, which encapsulates the
 * functionalities of Logger graphical user interface.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
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

    /**
     * @class Logger
     * @brief A flexible logging class that allows different log levels and output options.
     *
     * The `Logger` class provides a versatile logging system with support for different log levels,
     * timestamp formatting, and optional file output. It also includes a GUI interface for real-time log viewing.
     */
    class Logger {
    public:
        /**
         * @class LoggerGUI
         * @brief GUI interface for real-time log viewing.
         *
         * The `LoggerGUI` class provides an ImGui-based GUI for viewing and managing log messages in real-time.
         */
        class LoggerGUI {
        public:
            /**
             * @brief Constructor for the LoggerGUI class.
             */
            LoggerGUI();

            /**
             * @brief Clear the log messages in the GUI.
             */
            void Clear();

            /**
             * @brief Add a log message to the GUI.
             *
             * @param fmt The log message format.
             * @param ... Additional log message arguments.
             */
            void AddLog(const char* fmt, ...);

            /**
             * @brief Draw the GUI window for log viewing.
             *
             * @param title The title of the log window.
             */
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

        /**
         * @brief Constructor for the Logger class.
         *
         * @param name The name of the logger.
         */
        Logger(std::string const& name = "");

        /**
         * @brief Destructor for the Logger class.
         */
        ~Logger();

        /**
         * @brief Log a message with the Trace log level.
         *
         * This function is used to log a message with the Trace log level. It allows you to include
         * log message formatting and arguments.
         *
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void Trace(std::string_view fmt, Args&&... args);

        /**
         * @brief Log a message with the Debug log level.
         *
         * This function is used to log a message with the Debug log level. It allows you to include
         * log message formatting and arguments.
         *
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void Debug(std::string_view fmt, Args&&... args);

        /**
         * @brief Log a message with the Info log level.
         *
         * This function is used to log a message with the Info log level. It allows you to include
         * log message formatting and arguments.
         *
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void Info(std::string_view fmt, Args&&... args);

        /**
         * @brief Log a message with the Warning log level.
         *
         * This function is used to log a message with the Warning log level. It allows you to include
         * log message formatting and arguments.
         *
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void Warn(std::string_view fmt, Args&&... args);

        /**
         * @brief Log a message with the Error log level.
         *
         * This function is used to log a message with the Error log level. It allows you to include
         * log message formatting and arguments.
         *
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void Error(std::string_view fmt, Args&&... args);

        /**
         * @brief Log a message with the Critical log level.
         *
         * This function is used to log a message with the Critical log level. It allows you to include
         * log message formatting and arguments.
         *
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void Critical(std::string_view fmt, Args&&... args);

        /**
         * @brief Set the name of the logger.
         *
         * This function sets the name of the logger, which can be used for identification or labeling purposes.
         *
         * @param new_logger_name The new name for the logger.
         */
        void SetLoggerName(std::string const& new_logger_name);

        /**
         * @brief Set the log level for the logger.
         *
         * This function sets the log level for the logger, controlling which log messages are displayed.
         *
         * @param new_level The new log level.
         */
        void SetLogLevel(aLogLevel new_level);

        /**
         * @brief Get the current log level of the logger.
         *
         * This function returns the current log level of the logger, indicating the level of log messages
         * that are being logged and displayed.
         *
         * @return The current log level.
         */
        aLogLevel GetLogLevel() const;

        /**
         * @brief Set the timestamp format for log messages.
         *
         * This function sets the format for timestamping log messages. You can customize the timestamp format
         * to suit your needs.
         *
         * @param new_timestamp_format The new timestamp format.
         */
        void SetTimestampFormat(std::string const& new_timestamp_format);

        /**
         * @brief Get the current timestamp format for log messages.
         *
         * This function returns the current timestamp format used for log messages.
         *
         * @return The current timestamp format.
         */
        std::string GetTimestampFormat() const;

        // Additional options

        /**
         * @brief Enable file output for log messages.
         */
        void EnableFileOutput();

        /**
         * @brief Get the LoggerGUI instance for log viewing.
         *
         * @return A reference to the LoggerGUI instance.
         */
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

        /**
         * @brief Log a message with the specified log level.
         *
         * @param level The log level.
         * @param fmt The log message format.
         * @param args Additional log message arguments.
         */
        template<typename... Args>
        void log(aLogLevel level, std::string_view fmt, Args&&... args);

        /**
         * @brief Get a timestamp with the specified format.
         *
         * @param ts_format The timestamp format.
         * @return The formatted timestamp.
         */
        std::string getTimestamp(std::string const& ts_format) const;

        /**
         * @brief Close the log file (if open).
         */
        void closeFile();

        // Static functions

        /**
         * @brief Get a string representation of a log level.
         *
         * @param level The log level.
         * @return The log level as a string.
         */
        static std::string getLogLevelString(aLogLevel level);

        /**
         * @brief Set the color for displaying log messages of a specific level.
         *
         * @param level The log level.
         */
        static void setColor(aLogLevel level);
    };

    template<typename... Args>
    inline void Logger::Trace(std::string_view fmt, Args&&... args) { log(aLogLevel::Trace, fmt, args...); }

    template<typename... Args>
    inline void Logger::Debug(std::string_view fmt, Args&&... args) { log(aLogLevel::Debug, fmt, args...); }

    template<typename... Args>
    inline void Logger::Info(std::string_view fmt, Args&&... args) { log(aLogLevel::Info, fmt, args...); }

    template<typename... Args>
    inline void Logger::Warn(std::string_view fmt, Args&&... args) { log(aLogLevel::Warning, fmt, args...); }

    template<typename... Args>
    inline void Logger::Error(std::string_view fmt, Args&&... args) { log(aLogLevel::Error, fmt, args...); }

    template<typename... Args>
    inline void Logger::Critical(std::string_view fmt, Args&&... args) { log(aLogLevel::Critical, fmt, args...); }

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
            if (mLogFile.is_open())
                mLogFile << log.str();

            mLoggerGUI.AddLog(log.str().c_str());
        }
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_DEBUG_LOGGER_H
