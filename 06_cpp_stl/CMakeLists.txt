cmake_minimum_required(VERSION 3.4.1)
project(cpp_stl CXX)

set(CPP_SOURCES
        main.cpp)

add_executable(cpp_stl ${CPP_SOURCES})
add_executable(nmh nmh.cpp)

target_link_libraries(cpp_stl c_basic)
target_link_libraries(nmh c_basic)