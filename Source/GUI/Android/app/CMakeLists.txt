# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

project(MediaInfo)

# Build Zenlib
set(ZenLib_DIR ${CMAKE_CURRENT_BINARY_DIR}/ZenLib)
set(ZenLib_LIBRARY zen)

option(LARGE_FILES "Enable large files support" OFF)
add_subdirectory(../../../../../ZenLib/Project/CMake ${ZenLib_DIR})
include_directories(AFTER SYSTEM ../../../../../ZenLib/Source)
file(MAKE_DIRECTORY ${ZenLib_DIR}/../../../include) # fake system wide installation

# Build brotli
set(brotli_DIR ${CMAKE_CURRENT_BINARY_DIR}/brotli)
set(BROTLI_INCLUDE_DIR ../../../../../brotli/c/include)
set(BROTLICOMMON_LIBRARY brotlicommon)
set(BROTLIDEC_LIBRARY brotlidec)
add_subdirectory(../../../../../brotli ${brotli_DIR})

# Build MediaInfoLib
set(MediaInfoLib_LIBRARY mediainfo)
set(MediaInfoLib_DIR ${CMAKE_CURRENT_BINARY_DIR}/MediaInfoLib)

option(JNI "Build JNI interface" ON)
add_subdirectory(../../../../../MediaInfoLib/Project/CMake ${MediaInfoLib_DIR})
include_directories(AFTER SYSTEM ../../../../../MediaInfoLib/Source)