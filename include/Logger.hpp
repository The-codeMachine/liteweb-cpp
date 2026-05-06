#include <string>
#include <cstdint>

enum class LogLevel {
	UNKNOWN = 0,
	INFO,
	WARNING,
	DEBUG,
	SECURITY
};

class Logger {
public:
	//																				keeps 7 days							every 24 hours
	Logger(const std::string& file_location, const std::string& file_prefix, int max_files = 7, uint32_t rotation_period = 86400);

	// throws an exception for an error
	void log(const std::string& message, LogLevel level = LogLevel::INFO) const;

	// returns a result (0 for success, everything else is for a specific error)
	int log_r(const std::string& message, LogLevel level = LogLevel::INFO) const noexcept;

	// file_name includes prefix
	std::string readLogFile(const std::string& file_name) const;

	void changeFileLocation(const std::string& file_location);
	void changeMaxFiles(int max_files);
	void changeRotationPeriod(uint32_t rotation_period);

private:
	static uint64_t _get_current_time();
	void _rotate();

	static inline std::string _parse_log_level(LogLevel level);

	inline std::string _read_line(int index, const std::string& file_name) const;

private:
	std::string _file_location;
	std::string _file_prefix;
	std::string _current_file_location;

	int _max_files;
	uint32_t _rotation_period;
	uint64_t _last_log;

};