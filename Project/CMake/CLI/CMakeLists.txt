cmake_minimum_required(VERSION 3.8...3.31)

project(MediaInfo_CLI)

option(MEDIAINFO_CLI_STATIC "Link MediaInfo CLI statically" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake/Modules")

if(MSVC)
    add_compile_options(/GS /W3 /guard:cf /sdl)
    add_link_options(/guard:cf)
    if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL ARM64)
        add_compile_options(/guard:ehcont /guard:signret)
        add_link_options(/guard:ehcont /guard:delayloadsignret)
    endif()
    if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL x64)
        add_compile_options(/guard:ehcont)
        add_link_options(/guard:ehcont /CETCOMPAT)
    endif()
    if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL X86)
        add_compile_options()
        add_link_options(/CETCOMPAT)
    endif()
endif()

set(CLI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/CLI_Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/CommandLine_Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/CLI/Help.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Common/Core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../MSVC2022/CLI/MediaInfo_CLI.rc
)

add_executable(mediainfo_cli ${CLI_SOURCES})

set_target_properties(mediainfo_cli PROPERTIES OUTPUT_NAME mediainfo)

target_compile_definitions(mediainfo_cli PRIVATE _UNICODE UNICODE)

include(EnableLatestCXX)
enable_latest_cxx(mediainfo_cli)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../Source)

if(MEDIAINFO_CLI_STATIC)
    # Prefer static libraries
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})

    # Check and auto download dependencies 
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
        macro(ensure_dependency NAME REPO LOCAL_PATH CMAKELISTS_SUBDIR)
            if(NOT EXISTS "${LOCAL_PATH}/${CMAKELISTS_SUBDIR}/CMakeLists.txt")
                message(STATUS "${NAME} not found at ${LOCAL_PATH}. Downloading...")
                include(FetchContent)
                FetchContent_Declare(
                    ${NAME}
                    GIT_REPOSITORY ${REPO}
                    GIT_SHALLOW    TRUE
                    SOURCE_DIR     "${LOCAL_PATH}"
                    SOURCE_SUBDIR  does-not-exist-we-want-to-download-only-not-autobuild
                    BINARY_DIR     "${CMAKE_CURRENT_BINARY_DIR}/${NAME}"
                )
                FetchContent_MakeAvailable(${NAME})
            else()
                message(STATUS "Found local ${NAME} at ${LOCAL_PATH}")
            endif()
        endmacro()
        if(WIN32 OR BUILD_ZLIB)
            ensure_dependency(zlib "https://github.com/MediaArea/zlib.git" "${CMAKE_CURRENT_SOURCE_DIR}/../../../../zlib" "")
        endif()
        if(WIN32 OR BUILD_ZENLIB)
            ensure_dependency(ZenLib "https://github.com/MediaArea/ZenLib.git" "${CMAKE_CURRENT_SOURCE_DIR}/../../../../ZenLib" "Project/CMake")
        endif()
        ensure_dependency(MediaInfoLib "https://github.com/MediaArea/MediaInfoLib.git" "${CMAKE_CURRENT_SOURCE_DIR}/../../../../MediaInfoLib" "Project/CMake")
    endif()

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
    find_package(MediaInfoLib REQUIRED)
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
