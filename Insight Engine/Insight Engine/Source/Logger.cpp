#include "Pch.h"
#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <filesystem>

namespace IS {

    // Initialize static data member
    std::mutex Logger::log_mutex;

    Logger::Logger(std::string const& name) : logger_name(name) {}

    Logger::~Logger() {
        closeFile();
    }

    void Logger::setLoggerName(std::string const& new_logger_name) {
        logger_name = new_logger_name;
    }

    void Logger::setLogLevel(aLogLevel new_level) {
        log_level = new_level;
    }

    aLogLevel Logger::getLogLevel() const {
        return log_level;
    }

    void Logger::enableColors(bool flag) {
        colors_enabled = flag;
    }

    void Logger::enableFileOutput() {
        // Thread safety
        std::scoped_lock lock(log_mutex);
        
        // Close file if already opened
        if (log_file.is_open())
            log_file.close();

        // Construct log filename with timestamp
        std::ostringstream filepath;
        filepath << "Logs" << "/" << logger_name;
        if (!std::filesystem::exists(filepath.str())) {
            std::filesystem::create_directories(filepath.str());
        }
        filepath << "/" << getTimestamp(log_filename_timestamp_format) << ".log";

        // Append logs to log file
        log_file.open(filepath.str(), std::ios_base::app);
        if (!log_file.is_open())
            std::cerr << "Error: Failed to open log file at " << filepath.str() << std::endl;
    }
    
    void Logger::setTimestampFormat(std::string const& new_timestamp_format) {
        timestamp_format = new_timestamp_format;
    }

    std::string Logger::getTimestampFormat() const {
        return timestamp_format;
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
            oss << BOLD << "[WARNING]";
            break;
        case aLogLevel::Error:
            oss << BOLD << "[ERROR]";
            break;
        case aLogLevel::Critical:
            oss << BOLD << "[CRITICAL]";
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
            if (log_file) {
                log_file.close();
            }
        } catch (std::ios_base::failure& e) {
            std::cerr << e.what() << std::endl;
        }
    }

} // end namespace IS
