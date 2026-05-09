cmake_minimum_required(VERSION 4.1)

project(liteweb-cpp-test)

add_library(liteweb-cpp STATIC IMPORTED)

set(CMAKE_CXX_STANDARD 20)

set(LITEWEB_ROOT ${CMAKE_SOURCE_DIR}/..)

set_target_properties(liteweb-cpp PROPERTIES
    IMPORTED_LOCATION "${LITEWEB_ROOT}/x64/Debug/liteweb-cpp.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${LITEWEB_ROOT}/include"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")

add_executable(logger_test logger_test.cpp)
add_executable(server_test server_test.cpp ../src/server.cpp)

target_link_libraries(logger_test PRIVATE liteweb-cpp)
target_link_libraries(server_test PRIVATE liteweb-cpp)