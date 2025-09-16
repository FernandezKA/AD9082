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

# Add AD9081 API subdirectory
add_subdirectory(api)

# TCP Server Library
add_library(tcp_server src/tcp_server.cpp)
target_include_directories(tcp_server PUBLIC inc)
target_link_libraries(tcp_server ${Boost_LIBRARIES} Threads::Threads)

# Main executable
add_executable(${CMAKE_PROJECT_NAME} proj/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ad9081_api tcp_server)
# Install target
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION share/${CMAKE_PROJECT_NAME})

# CPack configuration
include(CPack)
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "Home")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AD9208 Application")
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "FernandezKA")
set(CPACK_DEBIAN_PACKAGE_SECTION "electronics")
set(CPACK_RPM_PACKAGE_LICENSE "Proprietary")