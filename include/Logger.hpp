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

class Logger {
public:
    Logger(
        const std::string& file_location,
        const std::string& file_prefix,
        int max_files = 7
    );

    void log(const std::string& message,
        LogLevel level = LogLevel::INFO);

private:
    std::shared_ptr<spdlog::logger> _logger;

private:
    static spdlog::level::level_enum
        _convert_level(LogLevel level);
};