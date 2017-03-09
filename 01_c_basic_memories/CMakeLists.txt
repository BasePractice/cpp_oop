cmake_minimum_required(VERSION 3.4.1)
project(c_basic_memories C)

set(C_SOURCES
        c_basic.c)

enable_language(C)

add_definitions(-DMAIN_CONFIGURATION=2)

add_library(c_basic STATIC ${C_SOURCES})
add_executable(c_basic_main c_basic_main.c)

target_link_libraries(c_basic_main c_basic)