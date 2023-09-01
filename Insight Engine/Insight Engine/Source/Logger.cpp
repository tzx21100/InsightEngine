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

        // Create directory if does not exist
        if (!std::filesystem::exists(log_directory)) {
            std::filesystem::create_directory(log_directory);
        }

        // Construct log filename with timestamp
        std::ostringstream filepath;
        filepath << log_directory << "/" << logger_name;
        if (!std::filesystem::exists(filepath.str())) {
            std::filesystem::create_directory(filepath.str());
        }
        filepath << "/" << getTimestamp(log_filename_timestamp_format) << ".txt";
        log_filepath = filepath.str();

        // Append logs to log file
        log_file.open(log_filepath, std::ofstream::app);
        if (!log_file.is_open())
            std::cerr << "Error: Failed to open log file at" << log_filepath << std::endl;
    }
    
    void Logger::setTimestampFormat(std::string const& new_timestamp_format) {
        timestamp_format = new_timestamp_format;
    }

    std::string Logger::getTimestampFormat() const {
        return timestamp_format;
    }

    void Logger::setLogDirectory(std::string const& new_log_directory) {
        log_directory = new_log_directory;
    }

    std::string Logger::getLogDirectory() const {
        return log_directory;
    }

    void Logger::printLogLevel(aLogLevel level) {
        switch (level) {
        case aLogLevel::Trace:
            std::cout << "[TRACE] ";
            break;
        case aLogLevel::Debug:
            std::cout << "[DEBUG] ";
            break;
        case aLogLevel::Info:
            std::cout << "[INFO] ";
            break;
        case aLogLevel::Warning:
            std::cout << BOLD << "[WARNING] ";
            break;
        case aLogLevel::Error:
            std::cout << BOLD << "[ERROR] ";
            break;
        case aLogLevel::Critical:
            std::cout << BOLD << "[CRITICAL] ";
            break;
        default:
            break;
        }
    }

    std::string Logger::getTimestamp(std::string const& ts_format) const {
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream timestamp;
        timestamp << "[" << std::put_time(std::localtime(&current_time), ts_format.c_str()) << "]";
        return timestamp.str();
    }

    void Logger::setColor(aLogLevel level) {
        using enum aLogLevel;
        switch(level) {
        case Trace:
            std::cout << WHITE;
            break;
        case Debug:
            std::cout << CYAN;
            break;
        case Info:
            std::cout << GREEN;
            break;
        case Warning:
            std::cout << YELLOW_BOLD;
            break;
        case Error:
            std::cout << RED_BOLD;
            break;
        case Critical:
            std::cout << BOLD_ON_RED;
            break;
        default:
            std::cout << RESET;
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
