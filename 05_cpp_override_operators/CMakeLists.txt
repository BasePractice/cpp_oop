cmake_minimum_required(VERSION 3.4.1)
project(cpp_override_operators CXX)

set(CPP_SOURCES
        main.cpp)

add_executable(cpp_override_operators ${CPP_SOURCES})

target_link_libraries(cpp_override_operators c_basic)