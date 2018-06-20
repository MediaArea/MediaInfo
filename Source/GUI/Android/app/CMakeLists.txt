# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
             mediainfo-interface

             # Sets the library as a static library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/mediainfo-interface.cpp )

target_compile_definitions(mediainfo-interface PRIVATE _FILE_OFFSET_BITS=64)

find_library(log-lib log)

option(BUILD_ZENLIB "" ON)
option(LARGE_FILES "Enable large files support" OFF)
add_subdirectory(../../../../../MediaInfoLib/Project/CMake ${CMAKE_CURRENT_BINARY_DIR}/MediaInfoLib)
set(MediaInfoLib_LIBRARY mediainfo)
include_directories(AFTER SYSTEM ../../../../../ZenLib/Source)
include_directories(AFTER SYSTEM ../../../../../MediaInfoLib/Source)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(mediainfo-interface ${log-lib})
target_link_libraries(mediainfo-interface ${MediaInfoLib_LIBRARY})
