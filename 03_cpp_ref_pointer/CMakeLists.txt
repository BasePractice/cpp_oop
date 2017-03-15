cmake_minimum_required(VERSION 3.4.1)
project(cpp_ref_pointer CXX)

set(CPP_SOURCES
        main.cpp)

add_executable(cpp_ref_pointer ${CPP_SOURCES})

target_link_libraries(cpp_ref_pointer c_basic)