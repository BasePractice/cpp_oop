cmake_minimum_required(VERSION 3.4.1)
project(cpp_classes CXX)

set(CPP_SOURCES
        main.cpp)

add_executable(cpp_classes ${CPP_SOURCES})

target_link_libraries(cpp_classes c_basic)