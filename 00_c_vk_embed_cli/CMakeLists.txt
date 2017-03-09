cmake_minimum_required(VERSION 3.4.1)
project(c_vk_embed_cli C)

if (APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
endif ()

enable_language(C)
find_package(OpenSSL REQUIRED)

add_definitions(-DMG_ENABLE_SSL)

set(C_SOURCES
        c_vk_client.c
        ${CMAKE_SOURCE_DIR}/lib/3rdparty/frozen/frozen.c
        ${CMAKE_SOURCE_DIR}/lib/3rdparty/mongoose/mongoose.c
        )
include_directories(${CMAKE_SOURCE_DIR}/lib/3rdparty/frozen)
include_directories(${CMAKE_SOURCE_DIR}/lib/3rdparty/mongoose)
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(vk ${C_SOURCES})
target_link_libraries(vk ${OPENSSL_LIBRARIES} pthread)