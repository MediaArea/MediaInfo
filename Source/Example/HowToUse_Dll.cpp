/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Example for MediaInfoLib
// Command line version
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef MEDIAINFO_LIBRARY
    #include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
    #define MediaInfoNameSpace MediaInfoLib;
#else //MEDIAINFO_LIBRARY
    #include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
    #define MediaInfoNameSpace MediaInfoDLL;
#endif //MEDIAINFO_LIBRARY
#include <iostream>
#include <iomanip>
using namespace MediaInfoNameSpace;

#ifdef __MINGW32__
    #ifdef _UNICODE
        #define _itot _itow
    #else //_UNICODE
        #define _itot itoa
    #endif //_UNICODE
#endif //__MINGW32

int main (int /*argc*/, Char * /*argv[]*/)
{
    //Information about MediaInfo
    MediaInfo MI;
    String To_Display=MI.Option(__T("Info_Version"), __T("0.7.13;MediaInfoDLL_Example_MSVC;0.7.13")).c_str();

    To_Display += __T("\r\n\r\nInfo_Parameters\r\n");
    To_Display += MI.Option(__T("Info_Parameters")).c_str();

    To_Display += __T("\r\n\r\nInfo_Codecs\r\n");
    To_Display += MI.Option(__T("Info_Codecs")).c_str();

    //An example of how to use the library
    To_Display += __T("\r\n\r\nOpen\r\n");
    MI.Open(__T("Example.ogg"));

    To_Display += __T("\r\n\r\nInform with Complete=false\r\n");
    MI.Option(__T("Complete"));
    To_Display += MI.Inform().c_str();

    To_Display += __T("\r\n\r\nInform with Complete=true\r\n");
    MI.Option(__T("Complete"), __T("1"));
    To_Display += MI.Inform().c_str();

    To_Display += __T("\r\n\r\nCustom Inform\r\n");
    MI.Option(__T("Inform"), __T("General;Example : FileSize=%FileSize%"));
    To_Display += MI.Inform().c_str();

    To_Display += __T("\r\n\r\nGet with Stream=General and Parameter=\"FileSize\"\r\n");
    To_Display += MI.Get(Stream_General, 0, __T("FileSize"), Info_Text, Info_Name).c_str();

    To_Display += __T("\r\n\r\nGetI with Stream=General and Parameter=46\r\n");
    To_Display += MI.Get(Stream_General, 0, 46, Info_Text).c_str();

    To_Display += __T("\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n");
    #ifdef __MINGW32__
        Char* C1=new Char[33];
        _itot (MI.Count_Get(Stream_Audio), C1, 10);
        To_Display +=C1;
        delete[] C1;
    #else
        toStringStream SS;
        SS << std::setbase(10) << MI.Count_Get(Stream_Audio);
        To_Display += SS.str();
    #endif

    To_Display += __T("\r\n\r\nGet with Stream=General and Parameter=\"AudioCount\"\r\n");
    To_Display += MI.Get(Stream_General, 0, __T("AudioCount"), Info_Text, Info_Name).c_str();

    To_Display += __T("\r\n\r\nGet with Stream=Audio and Parameter=\"StreamCount\"\r\n");
    To_Display += MI.Get(Stream_Audio, 0, __T("StreamCount"), Info_Text, Info_Name).c_str();

    To_Display += __T("\r\n\r\nClose\r\n");
    MI.Close();

    #ifdef _UNICODE
        std::wcout << To_Display;
    #else
        std::cout  << To_Display;
    #endif

    return 0;
}

//***************************************************************************
// By buffer example
//***************************************************************************
/*
//---------------------------------------------------------------------------
//Note: you can replace file operations by your own buffer management class
#include <stdio.h>
int main (int argc, Char *argv[])
{

    //From: preparing an example file for reading
    FILE* F=fopen("Example.ogg", "rb"); //You can use something else than a file
    if (F==0)
        return 1;

    //From: preparing a memory buffer for reading
    unsigned char* From_Buffer=new unsigned char[7*188]; //Note: you can do your own buffer
    size_t From_Buffer_Size; //The size of the read file buffer

    //From: retrieving file size
    fseek(F, 0, SEEK_END);
    long F_Size=ftell(F);
    fseek(F, 0, SEEK_SET);

    //Initializing MediaInfo
    MediaInfo MI;

    //Preparing to fill MediaInfo with a buffer
    MI.Open_Buffer_Init(F_Size, 0);

    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size=fread(From_Buffer, 1, 7*188, F);

        //Sending the buffer to MediaInfo
        size_t Status=MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
        if (Status&0x08) //Bit3=Finished
            break;

        //Testing if there is a MediaInfo request to go elsewhere
        if (MI.Open_Buffer_Continue_GoTo_Get()!=(MediaInfo_int64u)-1)
        {
            fseek(F, (long)MI.Open_Buffer_Continue_GoTo_Get(), SEEK_SET);   //Position the file
            MI.Open_Buffer_Init(F_Size, ftell(F));                          //Informing MediaInfo we have seek
        }
    }
    while (From_Buffer_Size>0);

    //Finalizing
    MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work

    //Get() example
    String To_Display=MI.Get(Stream_General, 0, __T("Format"));

    #ifdef _UNICODE
        std::wcout << To_Display;
    #else
        std::cout  << To_Display;
    #endif
}
*/
