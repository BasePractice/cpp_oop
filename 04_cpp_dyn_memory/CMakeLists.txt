cmake_minimum_required(VERSION 3.4.1)
project(cpp_dyn_memory CXX)

set(CPP_SOURCES
        main.cpp)

add_executable(cpp_dyn_memory ${CPP_SOURCES})

target_link_libraries(cpp_dyn_memory c_basic)