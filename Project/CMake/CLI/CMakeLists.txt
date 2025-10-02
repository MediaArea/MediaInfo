cmake_minimum_required(VERSION 3.8...3.31)

project(MediaInfo_CLI)

option(MEDIAINFO_CLI_STATIC "Link MediaInfo CLI statically" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/CLI_Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/CommandLine_Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/Help.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Common/Core.cpp
)

add_executable(mediainfo_cli ${CLI_SOURCES})

set_target_properties(mediainfo_cli PROPERTIES OUTPUT_NAME mediainfo)

target_compile_definitions(mediainfo_cli PRIVATE UNICODE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../Source)

if (MSVC)
  string(APPEND CMAKE_CXX_FLAGS " /EHsc")
endif()

if(MEDIAINFO_CLI_STATIC)
    # Prefer static libraries
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../MediaInfoLib/Project/CMake ${CMAKE_CURRENT_BINARY_DIR}/MediaInfoLib)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../MediaInfoLib/Source)

    # Link libraries
    target_link_libraries(mediainfo_cli
        PRIVATE
        mediainfo
    )
else()
    target_compile_definitions(mediainfo_cli PRIVATE MEDIAINFO_DLL_RUNTIME)
    find_package(ZLIB REQUIRED)
    find_package(Threads REQUIRED)
    find_package(mediainfolib REQUIRED)
    find_package(ZenLib REQUIRED)


    # Link libraries
    target_link_libraries(mediainfo_cli
        PRIVATE
        mediainfo
        "${ZenLib_LIBRARY}"
        ZLIB::ZLIB
        Threads::Threads
    )
endif()

install(TARGETS mediainfo_cli DESTINATION bin)
