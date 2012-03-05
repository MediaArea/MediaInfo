// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <iostream>
#include <fstream>
#include "ZenLib/Ztring.h"

#ifdef __WINDOWS__
    #undef __TEXT
    #include <windows.h>
#endif

//---------------------------------------------------------------------------
//Get command line args in main()
#ifdef UNICODE
    #ifdef _WIN32
        #define GETCOMMANDLINE() \
            MediaInfoNameSpace::Char** argv=CommandLineToArgvW(GetCommandLineW(), &argc); \

    #else //WIN32
        #define GETCOMMANDLINE() \
            std::vector<MediaInfoNameSpace::String> argv; \
            for (int Pos=0; Pos<argc; Pos++) \
            { \
                    ZenLib::Ztring A; \
                    A.From_Local(argv_ansi[Pos]); \
                    argv.push_back(A); \
            } \

    #endif //WIN32
#else //UNICODE
    #define GETCOMMANDLINE() \
        MediaInfoNameSpace::Char** argv=argv_ansi; \

#endif //UNICODE

//---------------------------------------------------------------------------
//Write to terminal
inline void STRINGOUT(ZenLib::Ztring Text)
{
    #ifdef UNICODE
        #ifdef _MSC_VER
            std::wcout<<Text.c_str()<<std::endl;
        #else //_MSC_VER
            std::cout<<Text.To_Local().c_str()<<std::endl;
        #endif //_MSC_VER
    #else // UNICODE
            std::cout<<Text.c_str()<<std::endl;
    #endif // UNICODE
}
inline void STRINGERR(ZenLib::Ztring Text)
{
    #ifdef UNICODE
        #ifdef _MSC_VER
            std::wcerr<<Text.c_str()<<std::endl;
        #else //_MSC_VER
            std::cerr<<Text.To_Local().c_str()<<std::endl;
        #endif //_MSC_VER
    #else // UNICODE
            std::cerr<<Text.c_str()<<std::endl;
    #endif // UNICODE
}

inline void TEXTOUT(const char* Text)
{
    STRINGOUT(ZenLib::Ztring().From_ISO_8859_1(Text));
}
