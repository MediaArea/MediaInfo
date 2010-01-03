// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <iostream>
#include "ZenLib/Ztring.h"

//---------------------------------------------------------------------------
//Get command line args in main()
#ifdef UNICODE
    #ifdef _WIN32
#undef __TEXT
    #include <windows.h>
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
inline void TEXTOUT(const char* Text)
{
    std::cout<<Text<<std::endl;
}

inline void STRINGOUT(ZenLib::Ztring Text)
{
    #ifdef __WINDOWS__
        Text.FindAndReplace(_T("\r\n"), _T("\n"), 0, ZenLib::Ztring_Recursive); //MediaInfoLib handle differences between platforms, but cout too!
    #endif //__WINDOWS__
    std::cout<<Text.To_Local().c_str()<<std::endl;
}


