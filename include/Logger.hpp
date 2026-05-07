#pragma once

#include <memory>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

enum class LogLevel {
    UNKNOWN = 0,
    INFO,
    WARNING,
    DEBUG,
    SECURITY
};

/// <summary>
/// 
/// This logger rotates daily with a maximum of 7 files (by default, can be overridden). The logger
/// uses spdlog to log. 
/// 
/// Operations include:
///  - Generates a new Logger with a specified file location, file prefix, and max files (optional)
///  - Log into the file (uses spdlog)
/// 
/// </summary>
class Logger {
public:
    Logger(const std::string& file_location, const std::string& file_prefix, int max_files = 7);

    void log(const std::string& message, LogLevel level = LogLevel::INFO);

private:
    static spdlog::level::level_enum _convert_level(LogLevel level);

private:
    std::shared_ptr<spdlog::logger> _logger;

};