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
#include "ZenLib/OS_Utils.h"

#ifdef __WINDOWS__
    #undef __TEXT
    #include <windows.h>
#endif
#if defined(_MSC_VER) && defined(UNICODE) && !defined(STREAM_MISSING)
    #include <cstdio>
    #include <cwchar>
    #include <io.h>
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
                    argv.push_back(ZenLib::FileName_From_Native(argv_ansi[Pos])); \
            } \

    #endif //WIN32
#else //UNICODE
    #define GETCOMMANDLINE() \
        MediaInfoNameSpace::Char** argv=argv_ansi; \

#endif //UNICODE

//---------------------------------------------------------------------------
//Write to terminal
inline void STRINGOUT(const ZenLib::tstring& Text)
{
    #ifdef UNICODE
        #if defined(STREAM_MISSING)
            wprintf(L"%ls\n", Text.c_str());
        #elif defined(_MSC_VER)
            //Written in one call instead of character by character with std::wcout, the C runtime converts it as stdout is in _O_U8TEXT mode
            std::wcout.flush();
            fflush(stdout);
            const char* Data=(const char*)Text.c_str();
            size_t Data_Size=wcslen(Text.c_str())*sizeof(wchar_t); //Up to the first NUL, as std::wcout
            while (Data_Size)
            {
                int Written=_write(_fileno(stdout), Data, (unsigned int)(Data_Size<0x1000000?Data_Size:0x1000000));
                if (Written<=0)
                    break;
                Data+=Written;
                Data_Size-=Written;
            }
            _write(_fileno(stdout), L"\n", sizeof(wchar_t));
        #else //_MSC_VER
            std::cout<<ZenLib::Ztring(Text).To_Local().c_str()<<std::endl;
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
