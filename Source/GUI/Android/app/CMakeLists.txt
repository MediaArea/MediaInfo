# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.5.0)

project(MediaInfo)

# Build MediaInfoLib
set(BUILD_SHARED_LIBS ON CACHE BOOL "Force shared library build" FORCE)
set(LARGE_FILES OFF CACHE BOOL "Enable large files support" FORCE)
set(BUILD_ZENLIB ON CACHE BOOL "Build bundled ZenLib" FORCE)

add_subdirectory(../../../../../MediaInfoLib/Project/CMake ${CMAKE_CURRENT_BINARY_DIR}/MediaInfoLib)
