/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#if defined(STREAM_MISSING)
#else
    #include <iostream>
#endif
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
        #if defined(STREAM_MISSING)
            wprintf(L"%ls\n", Text.c_str());
        #elif defined(_MSC_VER)
            std::wcout<<Text.c_str()<<std::endl;
        #else //_MSC_VER
            std::cout<<Text.To_Local().c_str()<<std::endl;
        #endif //_MSC_VER
    #else // UNICODE
        #if defined(STREAM_MISSING)
            fprintf(stdout, "%s\n", Text.c_str());
        #else
            std::cout<<Text.c_str()<<std::endl;
        #endif //_MSC_VER
    #endif // UNICODE
}
inline void STRINGERR(ZenLib::Ztring Text)
{
    #ifdef UNICODE
        #if defined(STREAM_MISSING)
            fwprintf(stderr, L"%ls\n", Text.c_str());
        #elif defined(_MSC_VER)
            std::wcerr<<Text.c_str()<<std::endl;
        #else //_MSC_VER
            std::cerr<<Text.To_Local().c_str()<<std::endl;
        #endif //_MSC_VER
    #else // UNICODE
        #if defined(STREAM_MISSING)
            fprintf(stderr, "%s\n", Text.c_str());
        #else
            std::cerr<<Text.c_str()<<std::endl;
        #endif //_MSC_VER
    #endif // UNICODE
}

inline void TEXTOUT(const char* Text)
{
    STRINGOUT(ZenLib::Ztring().From_ISO_8859_1(Text));
}

enum MI_Return
{
    MI_OK    = 0,
    MI_ERROR,
    MI_STOP,
    MI_ADD,
};
