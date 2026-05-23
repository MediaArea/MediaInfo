# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.5.0...4.3.0)

project(MediaInfo)

# Build MediaInfoLib
set(BUILD_SHARED_LIBS ON CACHE BOOL "Force shared library build" FORCE)
set(LARGE_FILES OFF CACHE BOOL "Enable large files support" FORCE)
set(BUILD_ZENLIB ON CACHE BOOL "Build bundled ZenLib" FORCE)

add_subdirectory(../../../../../MediaInfoLib/Project/CMake ${CMAKE_CURRENT_BINARY_DIR}/MediaInfoLib)

target_link_options(mediainfo
        PRIVATE
        -Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/libmediainfo.map.txt
        # This causes the linker to emit an error when a version script names a
        # symbol that is not found, rather than silently ignoring that line.
        -Wl,--no-undefined-version
)

# Without this, changes to the version script will not cause the library to relink.
set_target_properties(mediainfo
        PROPERTIES
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libmediainfo.map.txt
)

# disable warning in aes_via_ace.h for x86 arch
if(NOT "${CMAKE_VERSION}" VERSION_LESS "3.18")
    set_property(
        SOURCE
            "../../../../../MediaInfoLib/Source/ThirdParty/aes-gladman/aes_modes.c"
            "../../../../../MediaInfoLib/Source/ThirdParty/aes-gladman/aeskey.c"
        DIRECTORY "../../../../../MediaInfoLib/Project/CMake"
        APPEND
        PROPERTY COMPILE_OPTIONS "-Wno-qualified-void-return-type"
    )
endif()
