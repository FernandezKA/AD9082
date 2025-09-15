cmake_minimum_required(VERSION 3.16)
project(AD9208)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Boost REQUIRED COMPONENTS system program_options)
find_package(Threads REQUIRED)

# SPI Library
add_library(spi src/spi.cpp)
target_include_directories(spi PUBLIC inc)

# TCP Server Library
add_library(tcp_server src/tcp_server.cpp)
target_include_directories(tcp_server PUBLIC inc)
target_link_libraries(tcp_server ${Boost_LIBRARIES} Threads::Threads)

add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/proj/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} tcp_server)
