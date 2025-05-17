cmake_minimum_required(VERSION 3.8)
project(MediaInfo_CLI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to control static/shared linking
option(MEDIAINFO_CLI_STATIC "Link MediaInfo CLI statically" ON)

if(MEDIAINFO_CLI_STATIC)
    # Prefer static libraries
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(BUILD_SHARED_LIBS OFF)
    target_compile_definitions(mediainfo_cli PRIVATE MEDIAINFO_DLL_STATIC)
else()
    # Prefer dynamic libraries
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(BUILD_SHARED_LIBS ON)
    target_compile_definitions(mediainfo_cli PRIVATE MEDIAINFO_DLL_RUNTIME)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../Source)

set(CLI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/CLI_Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/CommandLine_Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/Help.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Common/Core.cpp
)

add_executable(mediainfo_cli ${CLI_SOURCES})

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
find_package(zenlib REQUIRED)
find_package(mediainfolib REQUIRED)

set_target_properties(mediainfo_cli PROPERTIES OUTPUT_NAME mediainfo)

target_compile_definitions(mediainfo_cli PRIVATE UNICODE)

# Link libraries
target_link_libraries(mediainfo_cli
    PRIVATE
    mediainfo
    ZLIB::ZLIB
    Threads::Threads
)

install(TARGETS mediainfo_cli DESTINATION bin)
