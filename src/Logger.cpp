#include "Logger.hpp"

#include <filesystem>

// Generates the correct sinks (for spdlog)
Logger::Logger(const std::string& file_location, const std::string& file_prefix, int max_files) {
    std::filesystem::create_directories(file_location);

    std::string file = file_location + "/" + file_prefix + ".log";

    /*
        Rotates every day at midnight.

        Keeps max_files old logs.
    */
    auto sink =
        std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            file,
            0,
            0,
            false,
            max_files
        );

    _logger =
        std::make_shared<spdlog::logger>(
            file_prefix,
            sink
        );

    _logger->set_pattern(
        "[%Y-%m-%d %H:%M:%S] [%^%l%$] %v"
    );

    _logger->flush_on(spdlog::level::info);

    spdlog::register_logger(_logger);
}

// converts the LogLevel into a spdlog::level::level_enum
spdlog::level::level_enum
Logger::_convert_level(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:
        return spdlog::level::info;

    case LogLevel::WARNING:
        return spdlog::level::warn;

    case LogLevel::DEBUG:
        return spdlog::level::debug;

    case LogLevel::SECURITY:
        return spdlog::level::critical;

    default:
        return spdlog::level::info;
    }
}

// logs the message with a level flag
void Logger::log(const std::string& message, LogLevel level) {
    _logger->log(
        _convert_level(level),
        message
    );
}