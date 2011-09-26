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
    #ifdef UNICODE
        #ifdef __WINDOWS__
            DWORD CharsWritten;
            std::string Buffer = Text.To_UTF8();
            UINT Cp_Old = GetConsoleOutputCP();
            SetConsoleOutputCP(CP_UTF8);
            WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), Buffer.c_str(), Buffer.length(), &CharsWritten, NULL);
            WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\r\n", 2, &CharsWritten, NULL);
            SetConsoleOutputCP(Cp_Old);
        #else //__WINDOWS__
            std::cout<<Text.To_Local().c_str()<<std::endl;
        #endif //__WINDOWS__
    #else // UNICODE
            std::cout<<Text.c_str()<<std::endl;
    #endif // UNICODE
}


