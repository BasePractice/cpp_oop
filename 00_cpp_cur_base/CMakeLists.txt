cmake_minimum_required(VERSION 3.4.1)
project(cpp_cur_base CXX)

set(CPP_SOURCES
        graphics.cpp)

if (APPLE)
    find_package(Curses REQUIRED)
    list(APPEND CPP_SOURCES ncurses_graphics.cpp)
else(WIN32)
    list(APPEND CPP_SOURCES win32_console_graphics.cpp)
endif ()


include_directories(${CURSES_INCLUDE_DIR})

add_library(cpp_cur_base SHARED ${CPP_SOURCES})
add_executable(graphics_main graphics_main.cpp)

target_link_libraries(cpp_cur_base ${CURSES_LIBRARIES})
target_link_libraries(graphics_main cpp_cur_base ${CURSES_LIBRARIES})
