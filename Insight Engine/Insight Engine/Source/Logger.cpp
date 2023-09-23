/*!
 * \file Logger.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This source file defines the implementation for class Logger, which
 *      encapsulates the functionalities of a Logger, and class LoggerGUI, which
 *      encapsulates the functionalities of Logger graphical user interface.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Logger.h"

// STL
#include <chrono>
#include <iomanip>
#include <filesystem>

namespace IS {

    /*
     * Logger
     * ---------------------------------------------------------------------------------------------------------------------------------
     */

    // Initialize static data member
    Logger::LoggerGUI Logger::mLoggerGUI;
    std::mutex Logger::mLogMutex;

    Logger::Logger(std::string const& name) : mLoggerName(name) {}

    Logger::~Logger() {
        closeFile();
    }

    void Logger::SetLoggerName(std::string const& new_logger_name) {
        mLoggerName = new_logger_name;
    }

    void Logger::SetLogLevel(aLogLevel new_level) {
        mLogLevel = new_level;
    }

    aLogLevel Logger::GetLogLevel() const {
        return mLogLevel;
    }

    void Logger::EnableFileOutput() {
        // Thread safety
        std::scoped_lock lock(mLogMutex);
        
        // Close file if already opened
        if (mLogFile.is_open())
            mLogFile.close();

        // Construct log filename with timestamp
        std::ostringstream filepath;
        filepath << "Logs" << "/" << mLoggerName;
        if (!std::filesystem::exists(filepath.str())) {
            std::filesystem::create_directories(filepath.str());
        }
        filepath << "/" << getTimestamp(mLogFilenameTimestampFormat) << ".log";

        // Append logs to log file
        mLogFile.open(filepath.str(), std::ios_base::app);
        if (!mLogFile.is_open())
            std::cerr << "Error: Failed to open log file at " << filepath.str() << std::endl;
    }

    Logger::LoggerGUI& Logger::GetLoggerGUI() {
        return mLoggerGUI;
    }
    
    void Logger::SetTimestampFormat(std::string const& new_timestamp_format) {
        mTimestampFormat = new_timestamp_format;
    }

    std::string Logger::GetTimestampFormat() const {
        return mTimestampFormat;
    }

    std::string Logger::getLogLevelString(aLogLevel level) {
        std::ostringstream oss;
        switch (level) {
        case aLogLevel::Trace:
            oss << "[TRACE]";
            break;
        case aLogLevel::Debug:
            oss << "[DEBUG]";
            break;
        case aLogLevel::Info:
            oss << "[INFO]";
            break;
        case aLogLevel::Warning:
            oss << "[WARNING]";
            break;
        case aLogLevel::Error:
            oss << "[ERROR]";
            break;
        case aLogLevel::Critical:
            oss << "[CRITICAL]";
            break;
        }
        return oss.str();
    }

    std::string Logger::getTimestamp(std::string const& ts_format) const {
        auto now = std::chrono::system_clock::now();
        long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream timestamp;
        timestamp << '[' << std::put_time(std::localtime(&current_time), ts_format.c_str())
                  << '.' << std::setfill('0') << std::setw(3) << milliseconds % 1000 << ']';

        return timestamp.str();
    }

    void Logger::setColor(aLogLevel level) {
        using enum aLogLevel;
        switch(level) {
        case Trace:
            std::clog << WHITE;
            break;
        case Debug:
            std::clog << CYAN;
            break;
        case Info:
            std::clog << GREEN;
            break;
        case Warning:
            std::clog << YELLOW_BOLD;
            break;
        case Error:
            std::clog << RED_BOLD;
            break;
        case Critical:
            std::clog << BOLD_ON_RED;
            break;
        }
    }

    void Logger::closeFile() {
        try {
            if (mLogFile) {
                mLogFile.close();
            }
        } catch (std::ios_base::failure& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    /*
     * LoggerGUI
     * ---------------------------------------------------------------------------------------------------------------------------------
     */
    Logger::LoggerGUI::LoggerGUI() : mAutoScroll(true) { Clear(); }

    void Logger::LoggerGUI::Clear() {
        mBuffer.clear();
        mLineOffsets.clear();
        mLineOffsets.push_back(0);
    }

    void Logger::LoggerGUI::AddLog(const char* fmt, ...) {
        int old_size = mBuffer.size();
        va_list args;
        va_start(args, fmt);
        mBuffer.appendfv(fmt, args);
        va_end(args);
        for (int new_size{ mBuffer.size() }; old_size < new_size; ++old_size) {
            if (mBuffer[old_size] == '\n')
                mLineOffsets.push_back(old_size + 1);
        }
    }

    void Logger::LoggerGUI::Draw(const char* title) {
        ImGui::Begin(title);

        // Options menu
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &mAutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear_flag = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy_flag = ImGui::Button("Copy");
        ImGui::SameLine();
        mFilter.Draw("Filter", -100.f);
        ImGui::SameLine();

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar)) {
            if (clear_flag)
                Clear();
            if (copy_flag)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            const char* buf = mBuffer.begin();
            const char* buf_end = mBuffer.end();
            if (mFilter.IsActive()) {
                for (int line{}; line < mLineOffsets.size(); ++line) {
                    const char* line_start = buf + mLineOffsets[line];
                    const char* line_end = (line + 1 < mLineOffsets.size()) ? (buf + mLineOffsets[line + 1] - 1) : buf_end;
                    if (mFilter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            } else {
                ImGuiListClipper clipper;
                clipper.Begin(mLineOffsets.size());
                while (clipper.Step()) {
                    for (int line{ clipper.DisplayStart }; line < clipper.DisplayEnd; ++line) {
                        const char* line_start = buf + mLineOffsets[line];
                        const char* line_end = (line + 1 < mLineOffsets.size()) ? (buf + mLineOffsets[line + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.f);
        }
        ImGui::EndChild();
        ImGui::End();
    }

    //ImU32 Logger::LoggerGUI::getLogLevelColor(aLogLevel log_level) {
    //    switch (log_level) {
    //    case aLogLevel::Trace:
    //        return IM_COL32(127, 127, 127, 255); // Gray for Trace
    //    case aLogLevel::Debug:
    //        return IM_COL32(0, 0, 255, 255);     // Blue for Debug
    //    case aLogLevel::Info:
    //        return IM_COL32(0, 255, 0, 255);     // Green for Info
    //    case aLogLevel::Warning:
    //        return IM_COL32(255, 255, 0, 255);   // Yellow for Warning
    //    case aLogLevel::Error:
    //        return IM_COL32(255, 0, 0, 255);     // Red for Error
    //    case aLogLevel::Critical:
    //        return IM_COL32(255, 0, 0, 255);     // Red for Critical (same as Error)
    //    default:
    //        return IM_COL32_WHITE; // Default to white for unknown log levels
    //    }
    //}

} // end namespace IS
