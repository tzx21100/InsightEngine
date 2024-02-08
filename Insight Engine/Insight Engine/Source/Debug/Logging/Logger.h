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
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
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
#include "Math/ISMath.h"

#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <format>
#include <deque>

#ifdef USING_IMGUI

#include <imgui.h>

#endif // USING_IMGUI

namespace IS {

    // ANSI escape codes for console formatting for color/style
    constexpr const char RESET[] = "\033[m";
    constexpr const char BOLD[] = "\033[1m";
    constexpr const char WHITE[] = "\033[37m";
    constexpr const char CYAN[] = "\033[36m";
    constexpr const char GREEN[] = "\033[32m";
    constexpr const char YELLOW_BOLD[] = "\033[33m\033[1m";
    constexpr const char RED_BOLD[] = "\033[31m\033[1m";
    constexpr const char BOLD_ON_RED[] = "\033[1m\033[41m";

    // Severity level of log
    enum class aLogLevel : int {
        LOGLEVEL_ALL, // Only meant for filter
        LOGLEVEL_TRACE,
        LOGLEVEL_DEBUG,
        LOGLEVEL_INFO,
        LOGLEVEL_WARNING,
        LOGLEVEL_ERROR,
        LOGLEVEL_CRITICAL
    };

    /*!
     * \brief Get an int representation of a log level.
     *
     * \param level The log level.
     * \return The log level as an int.
     */
    int LogLevelToInt(aLogLevel level);

    /*!
     * \brief Get a log level of an int representation.
     *
     * \param level The log level as an int.
     * \return The log level.
     */
    aLogLevel IntToLogLevel(int level);

    /*!
     * \brief Get a string representation of a log level.
     *
     * \param level The log level.
     * \return The log level as a string.
     */
    std::string LogLevelToString(aLogLevel level);

    /*!
     * \brief Get a log level of a string representation.
     *
     * \param level The log level as a string.
     * \return The log level.
     */
    aLogLevel StringToLogLevel(std::string const& level);
#ifdef USING_IMGUI
    /*!
     * \class LoggerGUI
     * \brief GUI interface for real-time log viewing.
     *
     * The `LoggerGUI` class provides an ImGui-based GUI for viewing and managing log messages in real-time.
     */
    class LoggerGUI
    {
    public:

        /*!
         * \brief Structure to encapsulate a log entry.
         */
        struct LogEntry
        {
            aLogLevel mLogLevel;
            std::string mLogMessage;
        };

        /*!
         * \brief Add a log message to the GUI.
         *
         * \param level The severity of the log message.
         * \param message The contents of the log message.
         */
        void AddLog(aLogLevel severity, const char* message);

        /*!
         * \brief Draw the GUI window for log viewing.
         *
         * \param title The title of the log window.
         */
        void Draw(const char* title);

        /*!
         * \brief Clear the log entries.
         */
        void Clear();

        /*!
         * \brief Gets the color corresponding to the log level.
         *
         * \param level The log severity level.
         * \return The color of corresponding to the log level.
         */
        static ImVec4 GetLogLevelColor(aLogLevel level);

        /*!
         * \brief Gets the instance of LoggerGUI.
         *
         * \return The instance of LoggerGUI.
         */
        static LoggerGUI& Instance();

    private:
        ImGuiTextFilter mFilter; ///< Text filter
        aLogLevel mSelectedFilter = aLogLevel::LOGLEVEL_ALL; ///< Selected filter
        std::deque<LogEntry> mLogEntries; ///< The log entries displayed
        bool mAutoScroll = true; ///< Boolean flag indicating auto scroll
        Vec2 mPanelSize; ///< Size of the panel.

        static const int MAX_LOG_LINES = 1000; // Limit the number of log lines to prevent excessive memory usage

        /*!
         * \brief Constructor for the LoggerGUI class.
         */
        LoggerGUI() = default;

        friend class ConsolePanel;
    };

#endif // USING_IMGUI


    /*!
     * \class Logger
     * \brief A flexible logging class that allows different log levels and output options.
     *
     * The `Logger` class provides a versatile logging system with support for different log levels,
     * timestamp formatting, and optional file output. It also includes a GUI interface for real-time log viewing.
     */
    class Logger {
    public:

        /*!
         * \brief Constructor for the Logger class.
         *
         * \param name The name of the logger.
         */
        Logger(std::string const& name = "");

        /*!
         * \brief Destructor for the Logger class.
         */
        ~Logger();

        /*!
         * \brief Log a message with the Trace log level.
         *
         * This function is used to log a message with the Trace log level. It allows you to include
         * log message formatting and arguments.
         *
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Trace(const char* fmt, Args&&... args);

        /*!
         * \brief Log a message with the Debug log level.
         *
         * This function is used to log a message with the Debug log level. It allows you to include
         * log message formatting and arguments.
         *
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Debug(const char* fmt, Args&&... args);

        /*!
         * \brief Log a message with the Info log level.
         *
         * This function is used to log a message with the Info log level. It allows you to include
         * log message formatting and arguments.
         *
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Info(const char* fmt, Args&&... args);

        /*!
         * \brief Log a message with the Warning log level.
         *
         * This function is used to log a message with the Warning log level. It allows you to include
         * log message formatting and arguments.
         *
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Warn(const char* fmt, Args&&... args);

        /*!
         * \brief Log a message with the Error log level.
         *
         * This function is used to log a message with the Error log level. It allows you to include
         * log message formatting and arguments.
         *
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Error(const char* fmt, Args&&... args);

        /*!
         * \brief Log a message with the Critical log level.
         *
         * This function is used to log a message with the Critical log level. It allows you to include
         * log message formatting and arguments.
         *
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Critical(const char* fmt, Args&&... args);

        /*!
         * \brief Set the name of the logger.
         *
         * This function sets the name of the logger, which can be used for identification or labeling purposes.
         *
         * \param new_logger_name The new name for the logger.
         */
        void SetLoggerName(std::string const& new_logger_name);

        /*!
         * \brief Set the log level for the logger.
         *
         * This function sets the log level for the logger, controlling which log messages are displayed.
         *
         * \param new_level The new log level.
         */
        void SetLogLevel(aLogLevel new_level);

        /*!
         * \brief Get the current log level of the logger.
         *
         * This function returns the current log level of the logger, indicating the level of log messages
         * that are being logged and displayed.
         *
         * \return The current log level.
         */
        aLogLevel GetLogLevel() const;

        /*!
         * \brief Set the timestamp format for log messages.
         *
         * This function sets the format for timestamping log messages. You can customize the timestamp format
         * to suit your needs.
         *
         * \param new_timestamp_format The new timestamp format.
         */
        void SetTimestampFormat(std::string const& new_timestamp_format);

        /*!
         * \brief Get the current timestamp format for log messages.
         *
         * This function returns the current timestamp format used for log messages.
         *
         * \return The current timestamp format.
         */
        std::string GetTimestampFormat() const;

        /*!
         * \brief Enable file output for log messages.
         */
        void EnableFileOutput();

    private:
        std::string mLoggerName; ///< Name of the Logger
        aLogLevel mMinLogLevel = aLogLevel::LOGLEVEL_INFO; ///< Minimum log severity level.
        std::string mTimestampFormat = "%H:%M:%S"; ///< Timestamp format to display log with.
        std::string mLogFilenameTimestampFormat = "%Y%m%d %H-%M-%S"; ///< Timestamp format for the log file name.
        std::ofstream mLogFile; ///< The output log file.
        static std::mutex mLogMutex; ///< Mutex of the logger.

        /*!
         * \brief Log a message with the specified log level.
         *
         * \param level The log level.
         * \param fmt The log message format.
         * \param args Additional log message arguments.
         */
        template<typename... Args>
        void Log(aLogLevel level, const char* fmt, Args&&... args);

        /*!
         * \brief Get a timestamp with the specified format.
         *
         * \param ts_format The timestamp format.
         * \return The formatted timestamp.
         */
        std::string GetTimestamp(std::string const& ts_format) const;

        /*!
         * \brief Close the log file (if open).
         */
        void CloseFile();

        /*!
         * \brief Set the color for displaying log messages of a specific level.
         *
         * \param level The log level.
         */
        static void SetColor(aLogLevel level);
    };

    template<typename... Args>
    inline void Logger::Trace(const char* fmt, Args&&... args) { Log(aLogLevel::LOGLEVEL_TRACE, fmt, args...); }
    template<typename... Args>
    inline void Logger::Debug(const char* fmt, Args&&... args) { Log(aLogLevel::LOGLEVEL_DEBUG, fmt, args...); }
    template<typename... Args>
    inline void Logger::Info(const char* fmt, Args&&... args) { Log(aLogLevel::LOGLEVEL_INFO, fmt, args...); }
    template<typename... Args>
    inline void Logger::Warn(const char* fmt, Args&&... args) { Log(aLogLevel::LOGLEVEL_WARNING, fmt, args...); }
    template<typename... Args>
    inline void Logger::Error(const char* fmt, Args&&... args) { Log(aLogLevel::LOGLEVEL_ERROR, fmt, args...); }
    template<typename... Args>
    inline void Logger::Critical(const char* fmt, Args&&... args) { Log(aLogLevel::LOGLEVEL_CRITICAL, fmt, args...); }

    template<typename... Args>
    inline void Logger::Log(aLogLevel level, const char* fmt, Args&&... args) {
        if (level < mMinLogLevel)
            return;

        std::scoped_lock lock(mLogMutex);

    #ifdef _DEBUG
        SetColor(level);
    #endif

        // Construct log
        std::ostringstream log;
        std::string timestamp = GetTimestamp(mTimestampFormat);
        std::string name = !mLoggerName.empty() ? '[' + mLoggerName + ']' : "";
        std::string all_args = std::vformat(fmt, std::make_format_args(args...));

        log << timestamp << name << ' ' << all_args << '\n';

        // Print to console
    #ifdef _DEBUG
            std::clog << log.str() << RESET;
    #endif

    #ifdef USING_IMGUI
            LoggerGUI::Instance().AddLog(level, log.str().c_str());
    #endif // USING_IMGUI

        // Write to file
        if (!mLogFile.is_open())
            return;

        std::string loglevel = '[' + LogLevelToString(level) + ']';
        log.str("");
        log.clear();
        log << timestamp << name;
        log << std::setw(9) << std::left << loglevel << ' ';
        log << all_args << '\n';
        mLogFile << log.str();
    }

} // end namespace IS

#endif // GAM200_INSIGHT_ENGINE_DEBUG_LOGGER_H
