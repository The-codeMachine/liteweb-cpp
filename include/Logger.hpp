#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include <spdlog/logger.h>
#include <spdlog/sinks/sink.h>

namespace liteweb_cpp{

    enum class LogLevel {
        Info,
        Warning,
        Debug,
        Error,
        Critical
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
        // Primary constructor
        explicit Logger(std::shared_ptr<spdlog::sinks::sink> sink, std::string_view logger_name);

        // Convenience factory for daily rotating logger
        static Logger dailyFileLogger(const std::filesystem::path& directory, std::string_view file_prefix,
            int rotation_hour = 0, int rotation_minute = 0, std::size_t max_files = 7);

        Logger(const Logger&) = default;
        Logger(Logger&&) noexcept = default;

        Logger& operator=(const Logger&) = default;
        Logger& operator=(Logger&&) noexcept = default;

        ~Logger() noexcept;

    public:
        void log(std::string_view message, LogLevel level = LogLevel::Info);

        template<typename... Args>
        void logf(LogLevel level, fmt::format_string<Args...> fmt, Args&&... args) {
            _logger->log(_convert_level(level), fmt, std::forward<Args>(args)...);
        }

        void flush();

    private:
        static spdlog::level::level_enum _convert_level(LogLevel level);

    private:
        std::shared_ptr<spdlog::logger> _logger;

    };

} // namespace liteweb_cpp