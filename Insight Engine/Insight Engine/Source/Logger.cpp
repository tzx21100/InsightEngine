/*!
 * \file Logger.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class Logger, which
 * encapsulates the functionalities of a Logger, and class LoggerGUI, which
 * encapsulates the functionalities of Logger graphical user interface.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Logger.h"
#include "EditorUtils.h"

// STL
#include <chrono>
#include <iomanip>
#include <filesystem>

namespace IS {

    int LogLevelToInt(aLogLevel level) { return static_cast<int>(level); }

    aLogLevel IntToLogLevel(int level) { return static_cast<aLogLevel>(level); }

    std::string LogLevelToString(aLogLevel level)
    {
        switch (level)
        {
        case aLogLevel::LOGLEVEL_ALL:      return "All";
        case aLogLevel::LOGLEVEL_TRACE:    return "Trace";
        case aLogLevel::LOGLEVEL_DEBUG:    return "Debug";
        case aLogLevel::LOGLEVEL_INFO:     return "Info";
        case aLogLevel::LOGLEVEL_WARNING:  return "Warning";
        case aLogLevel::LOGLEVEL_ERROR:    return "Error";
        case aLogLevel::LOGLEVEL_CRITICAL: return "Critical";
        }

        return "Unknown";
    }

    aLogLevel StringToLogLevel(std::string const& level)
    {
        if (level == "All")      return aLogLevel::LOGLEVEL_ALL;
        if (level == "Trace")    return aLogLevel::LOGLEVEL_TRACE;
        if (level == "Debug")    return aLogLevel::LOGLEVEL_DEBUG;
        if (level == "Info")     return aLogLevel::LOGLEVEL_INFO;
        if (level == "Warning")  return aLogLevel::LOGLEVEL_WARNING;
        if (level == "Error")    return aLogLevel::LOGLEVEL_ERROR;
        if (level == "Critical") return aLogLevel::LOGLEVEL_CRITICAL;

        return aLogLevel::LOGLEVEL_TRACE; // default to TRACE if unknown
    }

    /*
     * Logger
     * ---------------------------------------------------------------------------------------------------------------------------------
     */

    // Initialize static data member
    std::mutex Logger::mLogMutex;

    Logger::Logger(std::string const& name) : mLoggerName(name) {}

    Logger::~Logger() { CloseFile(); }

    void Logger::SetLoggerName(std::string const& new_logger_name) { mLoggerName = new_logger_name; }

    void Logger::SetLogLevel(aLogLevel new_level) { mMinLogLevel = new_level; }

    aLogLevel Logger::GetLogLevel() const { return mMinLogLevel; }

    void Logger::EnableFileOutput()
    {
        // Thread safety
        std::scoped_lock lock(mLogMutex);
        
        // Close file if already opened
        if (mLogFile.is_open())
            mLogFile.close();

        // Construct log filename with timestamp
        std::ostringstream filepath;
        filepath << "Logs" << "/" << mLoggerName;
        if (!std::filesystem::exists(filepath.str()))
            std::filesystem::create_directories(filepath.str());

        filepath << "/" << GetTimestamp(mLogFilenameTimestampFormat) << ".log";

        // Append logs to log file
        mLogFile.open(filepath.str(), std::ios_base::app);
        if (!mLogFile.is_open())
            std::cerr << "Error: Failed to open log file at " << filepath.str() << std::endl;
    }
    
    void Logger::SetTimestampFormat(std::string const& new_timestamp_format) { mTimestampFormat = new_timestamp_format; }

    std::string Logger::GetTimestampFormat() const { return mTimestampFormat; }

    std::string Logger::GetTimestamp(std::string const& ts_format) const
    {
        auto now = std::chrono::system_clock::now();
        long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream timestamp;
        timestamp << '[' << std::put_time(std::localtime(&current_time), ts_format.c_str())
                  << '.' << std::setfill('0') << std::setw(3) << milliseconds % 1'000 << ']';

        return timestamp.str();
    }

    void Logger::SetColor(aLogLevel level)
    {
        switch(level)
        {
        case aLogLevel::LOGLEVEL_TRACE:    std::clog << WHITE;       break;
        case aLogLevel::LOGLEVEL_DEBUG:    std::clog << CYAN;        break;
        case aLogLevel::LOGLEVEL_INFO:     std::clog << GREEN;       break;
        case aLogLevel::LOGLEVEL_WARNING:  std::clog << YELLOW_BOLD; break;
        case aLogLevel::LOGLEVEL_ERROR:    std::clog << RED_BOLD;    break;
        case aLogLevel::LOGLEVEL_CRITICAL: std::clog << BOLD_ON_RED; break;
        }
    }

    void Logger::CloseFile()
    {
        try {
            if (mLogFile) {
                mLogFile.close();
            }
        } catch (std::ios_base::failure& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void Logger::LoggerGUI::AddLog(aLogLevel level, const char* message)
    {
        // Store log messages with their log levels
        mLogEntries.emplace_back(level, message);

        // Keep the log size within a maximum limit (e.g., 1000 lines)
        while (mLogEntries.size() > MAX_LOG_LINES)
            mLogEntries.pop_front();
    }

    void Logger::LoggerGUI::Draw(const char* title)
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        ImGui::Begin(title, nullptr, window_flags);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
        
        // Options
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &mAutoScroll);
            ImGui::EndPopup();
        }

        // Display Options
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear_flag = ImGui::Button("Clear");

        // Filter based on log level
        ImGuiStyle& style = ImGui::GetStyle();
        const float PADDING = style.FramePadding.x;
        const float LONGEST_TEXT_WIDTH = ImGui::CalcTextSize("Warning").x + 2 * PADDING;
        const float ARROW_WIDTH = style.ItemSpacing.x + 2 * PADDING;
        const float COMBO_WIDTH = LONGEST_TEXT_WIDTH + ARROW_WIDTH + PADDING;

        ImGui::SetNextItemWidth(COMBO_WIDTH);
        ImGui::SameLine();
        
        if (ImGui::BeginCombo("##LogLevels", LogLevelToString(mSelectedFilter).c_str()))
        {
            for (int i{ LogLevelToInt(aLogLevel::LOGLEVEL_ALL) }; i <= LogLevelToInt(aLogLevel::LOGLEVEL_CRITICAL); ++i)
            {
                aLogLevel log_level = IntToLogLevel(i);
                if (ImGui::Selectable(LogLevelToString(log_level).c_str(), log_level == mSelectedFilter))
                    mSelectedFilter = log_level;
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        EditorUtils::RenderFilterWithHint(mFilter, "Search...");

        ImGui::Spacing();

        // Display Logs
        ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding;
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::BeginChild("Log Entries", { 0, 0 }, false, child_window_flags);
        if (clear_flag)
            Clear();
        for (auto const& [level, message] : mLogEntries)
        {
            const bool filtered = !(mSelectedFilter == aLogLevel::LOGLEVEL_ALL || level == mSelectedFilter);
            if (message.empty() || !mFilter.PassFilter(message.c_str()) || filtered)
                continue;

            ImGuiIO& io = ImGui::GetIO();
            ImFont* const font = io.Fonts->Fonts[EditorUtils::FontTypeToInt(
                                 level < aLogLevel::LOGLEVEL_WARNING
                                       ? aFontType::FONT_TYPE_LOG_DEFAULT
                                       : aFontType::FONT_TYPE_LOG_BOLD)];

            ImGui::PushFont(font);
            ImGui::TextColored(GetLogLevelColor(level), message.c_str());
            ImGui::PopFont();
        }

        if (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild(); // end child window Log Entries

        ImGui::PopStyleVar(); // end rounding
        ImGui::End(); // end main window
    }

    void Logger::LoggerGUI::Clear() { mLogEntries.clear(); }

    Logger::LoggerGUI& Logger::LoggerGUI::Instance() { static LoggerGUI gui; return gui; }

    ImVec4 Logger::LoggerGUI::GetLogLevelColor(aLogLevel level) {
        ImU32 color = IM_COL32_WHITE;

        switch (level)
        {
        case aLogLevel::LOGLEVEL_TRACE:    color = IM_COL32_WHITE;               break; // White Color
        case aLogLevel::LOGLEVEL_DEBUG:    color = IM_COL32( 57, 148, 218, 255); break; // Cyan Color
        case aLogLevel::LOGLEVEL_INFO:     color = IM_COL32( 18, 150,  14, 255); break; // Green Color
        case aLogLevel::LOGLEVEL_WARNING:  color = IM_COL32(248, 240, 164, 255); break; // Yellow Color
        case aLogLevel::LOGLEVEL_ERROR:    color = IM_COL32(210,  66,  79, 255); break; // Red Color
        case aLogLevel::LOGLEVEL_CRITICAL: color = IM_COL32(210,  66,  79, 255); break; // Red Color
        }

        return ImVec4(((color >> IM_COL32_R_SHIFT) & 0xFF) / 255.f,
                      ((color >> IM_COL32_G_SHIFT) & 0xFF) / 255.f,
                      ((color >> IM_COL32_B_SHIFT) & 0xFF) / 255.f,
                      ((color >> IM_COL32_A_SHIFT) & 0xFF) / 255.f);
    }

} // end namespace IS
