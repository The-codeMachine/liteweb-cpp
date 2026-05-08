#include "../include/Logger.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Logger logger("test_logs/", "log_tests");

    logger.log("Test log");

    logger.log("very cool log message", LogLevel::WARNING);

    logger.log("big big debugging message", LogLevel::DEBUG);

    logger.log("security_warning", LogLevel::SECURITY);

    logger.log("idk what to call this", LogLevel::UNKNOWN);

    std::this_thread::sleep_for(std::chrono::minutes(6));

    logger.log("this should be in a new file", LogLevel::INFO);

    return 0;
}