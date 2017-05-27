cmake_minimum_required(VERSION 3.4.1)
project(common CXX)

set(SOURCES
        common_tests.cpp
        c_basic_tests.cpp
        c_math_parser_tests.cpp)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/environment.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/environment.h
        @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(all_tests ${SOURCES})
target_link_libraries(all_tests c_basic gtest)