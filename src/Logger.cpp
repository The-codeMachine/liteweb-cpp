#include "Logger.hpp"

#include <filesystem>

// Sets the logger to the generated one (default of midnight for rotation)
Logger::Logger(const std::string& file_location, const std::string& file_prefix, 
    int rotation_hour, int rotation_minute, int max_files) {
    _logger = _generate_logger(file_location, file_prefix, rotation_hour, rotation_minute, max_files);
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

// Generates a shared_ptr for the logger based off some preferences
std::shared_ptr<spdlog::logger> Logger::_generate_logger(const std::string& file_location, const std::string& file_prefix,
    int rotation_hour, int rotation_minute, int max_files) {
    std::filesystem::create_directories(file_location);

    std::string file = file_location + "/" + file_prefix + ".log";

    /*
        Rotates every day at midnight.

        Keeps max_files old logs.
    */
    auto sink =
        std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            file,
            rotation_hour,
            rotation_minute,
            false,
            max_files
        );

    std::shared_ptr<spdlog::logger> logger =
        std::make_shared<spdlog::logger>(
            file_prefix,
            sink
        );

    logger->set_pattern(
        "[%Y-%m-%d %H:%M:%S] [%^%l%$] %v"
    );

    logger->flush_on(spdlog::level::info);

    spdlog::register_logger(logger);

    return logger;
}

// logs the message with a level flag
void Logger::log(const std::string& message, LogLevel level) {
    _logger->log(
        _convert_level(level),
        message
    );
}