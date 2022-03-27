AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = mediainfo
mediainfo_SOURCES = \
    ../../../Source/CLI/CLI_Main.cpp \
    ../../../Source/CLI/CommandLine_Parser.cpp \
    ../../../Source/CLI/Help.cpp \
    ../../../Source/Common/Core.cpp

#mediainfo_LDFLAGS     = -no-undefined -version-info 0:0:0
AM_CXXFLAGS = -std=c++11

INCLUDES = -I../../../Source -I../../../../MediaInfoLib/Source -I../../../../ZenLib/Source
