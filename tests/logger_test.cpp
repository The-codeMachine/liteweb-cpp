#include "../include/Logger.hpp"

#include <spdlog/sinks/rotating_file_sink.h>

#include <iostream>
#include <thread>
#include <vector> 
#include <chrono>

int main() {
    // rotation test
    std::cout << "Logger rotation test\n";

    auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("test_logs/rotation_test.log", 128, 3);

    liteweb_cpp::Logger logger(sink, "rotation_test");

    for (int i = 0; i < 1000; ++i) {
        logger.logf(liteweb_cpp::LogLevel::Info, "Message {}", i);
    }

    // concurrency test
    std::cout << "Logger concurrency test\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < 16; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < 10000; ++j) {
                logger.log("thread test");
            }
        });
    }

    for (auto& t : threads)
        t.join();

    return 0;
}