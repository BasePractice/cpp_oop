cmake_minimum_required(VERSION 3.4.1)
project(common CXX)

set(SOURCES
        common_tests.cpp
        c_basic_tests.cpp)

add_executable(all_tests ${SOURCES})
target_link_libraries(all_tests c_basic gtest)