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
//---------------------------------------------------------------------------
//Strings: Unicode, else UTF-8 text with the C++ API of the library (as in
//ZenLib), from the same arguments and with the same output as with Unicode,
//else (C API of the library) the bytes of the arguments and of the output
#if !defined(UNICODE) && !defined(MEDIAINFO_DLL_RUNTIME) && !defined(MEDIAINFO_DLL_STATIC)
    #define CLI_STRINGS_UTF8
#endif
//Wide console of the Microsoft C library, UTF-8 when the output is redirected
#if defined(_MSC_VER) && (defined(UNICODE) || defined(CLI_STRINGS_UTF8))
    #define CLI_CONSOLE_WIDE
#endif
#if defined(CLI_CONSOLE_WIDE) && !defined(STREAM_MISSING)
    #include <cstdio>
    #include <cwchar>
    #include <io.h>
    #include <vector>
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
#elif defined(CLI_STRINGS_UTF8)
    #ifdef _WIN32
        //UTF-16 command line, also for the characters which are not in the code page
        #define GETCOMMANDLINE() \
            wchar_t** argv_wide=CommandLineToArgvW(GetCommandLineW(), &argc); \
            std::vector<MediaInfoNameSpace::String> argv; \
            for (int Pos=0; Pos<argc; Pos++) \
                argv.push_back(argv_wide?ZenLib::Ztring().From_Unicode(argv_wide[Pos]):ZenLib::FileName_From_Native(argv_ansi[Pos])); \
            LocalFree(argv_wide); \

    #else //WIN32
        #define GETCOMMANDLINE() \
            std::vector<MediaInfoNameSpace::String> argv; \
            for (int Pos=0; Pos<argc; Pos++) \
                argv.push_back(ZenLib::FileName_From_Native(argv_ansi[Pos])); \

    #endif //WIN32
#else //UNICODE
    #define GETCOMMANDLINE() \
        MediaInfoNameSpace::Char** argv=argv_ansi; \

#endif //UNICODE

//---------------------------------------------------------------------------
//Write to a file or a pipe the text and a line end, in UTF-8 with CRLF line endings as the C runtime does in _O_U8TEXT mode,
//but by blocks of 1 MiB instead of about 1 KiB per system call
#if defined(CLI_CONSOLE_WIDE) && !defined(STREAM_MISSING)
inline void STRINGOUT_UTF8(HANDLE Handle, const wchar_t* Data, size_t Data_Size)
{
    const size_t Buffer_Size=0x100000;
    std::vector<char> Buffer(Buffer_Size+8); //Room for the last character
    size_t Buffer_Pos=0;
    for (size_t i=0; i<=Data_Size; i++)
    {
        unsigned int C=i<Data_Size?Data[i]:L'\n';
        if (C<0x80)
        {
            if (C==L'\n')
                Buffer[Buffer_Pos++]='\r';
            Buffer[Buffer_Pos++]=(char)C;
        }
        else if (C<0x800)
        {
            Buffer[Buffer_Pos++]=(char)(0xC0|(C>>6));
            Buffer[Buffer_Pos++]=(char)(0x80|(C&0x3F));
        }
        else if (C>=0xD800 && C<0xE000)
        {
            if (C<0xDC00 && i+1<Data_Size && Data[i+1]>=0xDC00 && Data[i+1]<0xE000)
            {
                C=0x10000+((C-0xD800)<<10)+(Data[++i]-0xDC00);
                Buffer[Buffer_Pos++]=(char)(0xF0|(C>>18));
                Buffer[Buffer_Pos++]=(char)(0x80|((C>>12)&0x3F));
                Buffer[Buffer_Pos++]=(char)(0x80|((C>>6)&0x3F));
                Buffer[Buffer_Pos++]=(char)(0x80|(C&0x3F));
            }
            else
            {
                //Lone surrogate, replaced by U+FFFD as with WideCharToMultiByte()
                Buffer[Buffer_Pos++]=(char)0xEF;
                Buffer[Buffer_Pos++]=(char)0xBF;
                Buffer[Buffer_Pos++]=(char)0xBD;
            }
        }
        else
        {
            Buffer[Buffer_Pos++]=(char)(0xE0|(C>>12));
            Buffer[Buffer_Pos++]=(char)(0x80|((C>>6)&0x3F));
            Buffer[Buffer_Pos++]=(char)(0x80|(C&0x3F));
        }

        if (Buffer_Pos>=Buffer_Size || i==Data_Size)
        {
            const char* ToWrite=&Buffer[0];
            while (Buffer_Pos)
            {
                DWORD Written;
                if (!WriteFile(Handle, ToWrite, (DWORD)Buffer_Pos, &Written, NULL) || !Written)
                    return;
                ToWrite+=Written;
                Buffer_Pos-=Written;
            }
        }
    }
}
//---------------------------------------------------------------------------
//Write to the terminal the text and a line end: by blocks to a file or a pipe, in one call to the console
inline void STRINGOUT_Wide(const wchar_t* Text)
{
    std::wcout.flush();
    fflush(stdout);
    size_t Data_Size=wcslen(Text); //Up to the first NUL, as std::wcout
    HANDLE Handle=(HANDLE)_get_osfhandle(_fileno(stdout));
    DWORD Console_Mode;
    if (Handle!=INVALID_HANDLE_VALUE && !GetConsoleMode(Handle, &Console_Mode))
    {
        STRINGOUT_UTF8(Handle, Text, Data_Size);
        return;
    }

    //Console: written in one call instead of character by character with std::wcout, the C runtime converts it as stdout is in _O_U8TEXT mode
    const char* Data=(const char*)Text;
    Data_Size*=sizeof(wchar_t);
    while (Data_Size)
    {
        int Written=_write(_fileno(stdout), Data, (unsigned int)(Data_Size<0x1000000?Data_Size:0x1000000));
        if (Written<=0)
            break;
        Data+=Written;
        Data_Size-=Written;
    }
    _write(_fileno(stdout), L"\n", sizeof(wchar_t));
}
#endif

//---------------------------------------------------------------------------
//Write to terminal
inline void STRINGOUT(const ZenLib::tstring& Text)
{
    #ifdef UNICODE
        #if defined(STREAM_MISSING)
            wprintf(L"%ls\n", Text.c_str());
        #elif defined(_MSC_VER)
            STRINGOUT_Wide(Text.c_str());
        #else //_MSC_VER
            //Converted by blocks, without a copy of the whole text
            const ZenLib::Char* Data=Text.c_str();
            size_t Data_Size=ZenLib::tstring::traits_type::length(Data); //Up to the first NUL, as the converted text
            const size_t Block_Size=0x10000;
            for (size_t Pos=0; Pos<Data_Size;)
            {
                size_t Size=Data_Size-Pos;
                if (Size>Block_Size)
                {
                    Size=Block_Size;
                    if (Data[Pos+Size-1]>=0xD800 && Data[Pos+Size-1]<0xDC00)
                        Size++; //No split of a UTF-16 surrogate pair
                }
                std::cout<<ZenLib::Ztring(Data+Pos, Size).To_Local().c_str();
                Pos+=Size;
            }
            std::cout<<std::endl;
        #endif //_MSC_VER
    #elif defined(CLI_STRINGS_UTF8)
        #if defined(STREAM_MISSING)
            wprintf(L"%ls\n", ZenLib::Ztring(Text).To_Unicode().c_str());
        #elif defined(_MSC_VER)
            STRINGOUT_Wide(ZenLib::Ztring(Text).To_Unicode().c_str());
        #else //_MSC_VER
            //Converted by blocks, without a copy of the whole text
            const char* Data=Text.c_str();
            size_t Data_Size=ZenLib::tstring::traits_type::length(Data); //Up to the first NUL, as the converted text
            const size_t Block_Size=0x10000;
            for (size_t Pos=0; Pos<Data_Size;)
            {
                size_t Size=Data_Size-Pos;
                if (Size>Block_Size)
                {
                    Size=Block_Size;
                    while (Size && ((unsigned char)Data[Pos+Size]&0xC0)==0x80)
                        Size--; //No split of a UTF-8 character (also the 3 bytes of a byte kept by ZenLib)
                    if (!Size)
                        Size=Block_Size; //Not UTF-8
                }
                std::cout<<ZenLib::Ztring(Data+Pos, Size).To_Local().c_str();
                Pos+=Size;
            }
            std::cout<<std::endl;
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
    #elif defined(CLI_STRINGS_UTF8)
        #if defined(STREAM_MISSING)
            fwprintf(stderr, L"%ls\n", Text.To_Unicode().c_str());
        #elif defined(_MSC_VER)
            std::wcerr<<Text.To_Unicode().c_str()<<std::endl;
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
