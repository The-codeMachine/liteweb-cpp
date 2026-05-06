#include <Logger.hpp>

#include <chrono>

Logger::Logger(const std::string& file_location, const std::string& file_prefix, int max_files, uint32_t rotation_period) :
	_file_location(file_location), _file_prefix(file_prefix), _max_files(max_files), _rotation_period(rotation_period) {}


void Logger::changeFileLocation(const std::string& file_location) {
	_file_location = file_location;
}

void Logger::changeMaxFiles(int max_files) {
	_max_files = max_files;
}

void Logger::changeRotationPeriod(uint32_t rotation_period) {
	_rotation_period = rotation_period;
}

uint64_t Logger::_get_current_time() {
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();

	uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

	return seconds;
}

void Logger::_rotate() {
    uint64_t currentTime = _get_current_time();

    std::chrono::sys_time<std::chrono::seconds> tp{ std::chrono::seconds{currentTime} };
    auto day = floor<std::chrono::days>(tp);
    std::chrono::year_month_day ymd{ day };

    _last_log = currentTime;

    // Convert to integers safely
    int y = int(ymd.year());
    unsigned m = unsigned(ymd.month());
    unsigned d = unsigned(ymd.day());

    // Proper string construction
    _current_file_location = _file_location + "/" + _file_prefix + "_" +
        std::to_string(y) + "_" +
        std::to_string(m) + "_" +
        std::to_string(d);
}