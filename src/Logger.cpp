#include <Logger.hpp>

#include <stdexcept>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace liteweb_cpp {

    Logger::Logger(std::shared_ptr<spdlog::sinks::sink> sink, std::string_view logger_name) {
        if (!sink)
            throw std::invalid_argument("Sink cannot be null");

        if (logger_name.empty())
            throw std::invalid_argument("Logger name cannot be empty");

        _logger = std::make_shared<spdlog::logger>(std::string(logger_name), std::move(sink));

        _logger->set_pattern(
            "[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] %v"
        );

        _logger->set_level(spdlog::level::debug);

        _logger->flush_on(spdlog::level::warn);
    }


    Logger Logger::dailyFileLogger(const std::filesystem::path& directory, std::string_view file_prefix,
        int rotation_hour, int rotation_minute, std::size_t max_files) {
        if (rotation_hour < 0 || rotation_hour > 23)
            throw std::invalid_argument("rotation_hour must be between 0 and 23");


        if (rotation_minute < 0 || rotation_minute > 59)
            throw std::invalid_argument("rotation_minute must be between 0 and 59");


        if (max_files == 0)
            throw std::invalid_argument("max_files must be > 0");


        if (file_prefix.empty())
            throw std::invalid_argument("file_prefix cannot be empty");


        std::filesystem::create_directories(directory);

        std::filesystem::path log_file = directory / (std::string(file_prefix) + ".log");

        auto sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_file.string(),
            rotation_hour, rotation_minute, false, max_files);

        return Logger(sink, file_prefix);
    }

    // flushes the logger, then destroys the object
    Logger::~Logger() noexcept {
        try {
            flush();
        }
        catch (...) {
            // never throw from destructor
        }
    }

    void Logger::log(std::string_view message, LogLevel level) {
        _logger->log(_convert_level(level), message);
    }

    // converts the LogLevel into a spdlog::level::level_enum
    spdlog::level::level_enum Logger::_convert_level(LogLevel level) {
        switch (level) {
        case LogLevel::Info:
            return spdlog::level::info;

        case LogLevel::Warning:
            return spdlog::level::warn;

        case LogLevel::Debug:
            return spdlog::level::debug;

        case LogLevel::Error:
            return spdlog::level::err;

        case LogLevel::Critical:
            return spdlog::level::critical;

        default:
            throw std::invalid_argument("Invalid LogLevel");
        }
    }

    // flushes the logger
    void Logger::flush() {
        _logger->flush();
    }

} // namesapce liteweb_cpp