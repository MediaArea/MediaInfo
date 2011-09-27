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
    #include <io.h>
    #include <fcntl.h>
#endif

//---------------------------------------------------------------------------
//Initialize STDOUT/STDERR for the terminal 
inline void INIT_TERMINAL(void)
{
    #if defined(__WINDOWS__) && defined(UNICODE)
        FILE *fStdOut = NULL, *fStdErr = NULL;
        int hStdOut = _open_osfhandle((intptr_t) GetStdHandle(STD_OUTPUT_HANDLE), _O_WRONLY|_O_U8TEXT);
        int hStdErr = _open_osfhandle((intptr_t) GetStdHandle(STD_ERROR_HANDLE), _O_WRONLY|_O_U8TEXT);
        if(hStdOut >= 0) fStdOut = _fdopen(hStdOut, "wb");
        if(hStdErr >= 0) fStdErr = _fdopen(hStdErr, "wb");
        if(fStdOut) { *stdout = *fStdOut; std::cout.rdbuf(new std::filebuf(fStdOut)); }
        if(fStdErr) { *stderr = *fStdErr; std::cerr.rdbuf(new std::filebuf(fStdErr)); }
    #endif
}

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
inline void TEXTOUT(const char* Text)
{
    std::cout<<Text<<std::endl;
}

inline void STRINGOUT(ZenLib::Ztring Text)
{
    #ifdef UNICODE
        #ifdef __WINDOWS__
            UINT Cp_Old = GetConsoleOutputCP();
            SetConsoleOutputCP(CP_UTF8);
            std::cout << Text.To_UTF8().c_str() << std::endl;
            SetConsoleOutputCP(Cp_Old);
        #else //__WINDOWS__
            std::cout<<Text.To_Local().c_str()<<std::endl;
        #endif //__WINDOWS__
    #else // UNICODE
            std::cout<<Text.c_str()<<std::endl;
    #endif // UNICODE
}


