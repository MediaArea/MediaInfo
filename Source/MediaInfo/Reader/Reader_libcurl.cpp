// Reader_libcurl - All info about media files
// Copyright (C) 2002-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// For user: you can disable or enable it
//#define MEDIAINFO_DEBUG
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_LIBCURL_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Reader/Reader_libcurl.h"
#include "MediaInfo/File__Analyze.h"
#if defined MEDIAINFO_LIBCURL_DLL_RUNTIME
    //Copy of cURL include files
    #include "MediaInfo/Reader/Reader_libcurl_Include.h"
#else
    #define CURL_STATICLIB
    #undef __TEXT
    #include "curl/curl.h"
#endif
#include <ctime>
using namespace ZenLib;
using namespace std;
#ifdef MEDIAINFO_DEBUG
    #include <iostream>
#endif // MEDIAINFO_DEBUG
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// libcurl stuff
//***************************************************************************

//---------------------------------------------------------------------------
struct Reader_libcurl::curl_data
{
    MediaInfo_Internal* MI;
    CURL*               Curl;
    #if MEDIAINFO_NEXTPACKET
        CURLM*          CurlM;
    #endif MEDIAINFO_NEXTPACKET
    struct curl_slist*  HttpHeader;
    std::bitset<32>     Status;
    String              File_Name;
    bool                Init_AlreadyDone;
    #if MEDIAINFO_NEXTPACKET
        bool            NextPacket;
    #endif MEDIAINFO_NEXTPACKET
    time_t              Time_Max;
    #ifdef MEDIAINFO_DEBUG
        int64u          Debug_BytesRead_Total;
        int64u          Debug_BytesRead;
        int64u          Debug_Count;
    #endif // MEDIAINFO_DEBUG

    curl_data()
    {
        MI=NULL;
        Curl=NULL;
        #if MEDIAINFO_NEXTPACKET
            CurlM=NULL;
        #endif MEDIAINFO_NEXTPACKET
        HttpHeader=NULL;
        Init_AlreadyDone=false;
        #if MEDIAINFO_NEXTPACKET
            NextPacket=false;
        #endif MEDIAINFO_NEXTPACKET
        Time_Max=0;
        #ifdef MEDIAINFO_DEBUG
            Debug_BytesRead_Total=0;
            Debug_BytesRead=0;
            Debug_Count=1;
        #endif // MEDIAINFO_DEBUG
    }
};

//---------------------------------------------------------------------------
size_t libcurl_WriteData_CallBack(void *ptr, size_t size, size_t nmemb, void *data)
{
    #ifdef MEDIAINFO_DEBUG
        ((Reader_libcurl::curl_data*)data)->Debug_BytesRead_Total+=size*nmemb;
        ((Reader_libcurl::curl_data*)data)->Debug_BytesRead+=size*nmemb;
    #endif //MEDIAINFO_DEBUG

    //Init
    if (!((Reader_libcurl::curl_data*)data)->Init_AlreadyDone)
    {
        double File_SizeD;
        CURLcode Result=curl_easy_getinfo(((Reader_libcurl::curl_data*)data)->Curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &File_SizeD);
        if (Result==CURLE_OK && File_SizeD!=-1)
        {
            ((Reader_libcurl::curl_data*)data)->MI->Open_Buffer_Init((int64u)File_SizeD, ((Reader_libcurl::curl_data*)data)->File_Name);
        }
        else
            ((Reader_libcurl::curl_data*)data)->MI->Open_Buffer_Init((int64u)-1, ((Reader_libcurl::curl_data*)data)->File_Name);
        ((Reader_libcurl::curl_data*)data)->Init_AlreadyDone=true;
    }

    //Continue
    ((Reader_libcurl::curl_data*)data)->Status=((Reader_libcurl::curl_data*)data)->MI->Open_Buffer_Continue((int8u*)ptr, size*nmemb);
    time_t CurrentTime = time(0);

    if (((Reader_libcurl::curl_data*)data)->Status[File__Analyze::IsFinished] || (((Reader_libcurl::curl_data*)data)->Time_Max && CurrentTime>=((Reader_libcurl::curl_data*)data)->Time_Max))
    {
        return 0;
    }

    //GoTo
    if (((Reader_libcurl::curl_data*)data)->MI->Open_Buffer_Continue_GoTo_Get()!=(int64u)-1)
    {
        return 0;
    }

    //Continue parsing
    return size*nmemb;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Reader_libcurl::Reader_libcurl ()
{
    Curl_Data=NULL;

    #if defined MEDIAINFO_LIBCURL_DLL_RUNTIME
        if (libcurl_Module_Count)
            return;

        size_t Errors=0;

        /* Load library */
        #ifdef MEDIAINFO_GLIBC
            libcurl_Module=g_module_open(MEDIAINFODLL_NAME, G_MODULE_BIND_LAZY);
        #elif defined (_WIN32) || defined (WIN32)
            libcurl_Module=LoadLibrary(_T(MEDIAINFODLL_NAME));
        #else
            libcurl_Module=dlopen(MEDIAINFODLL_NAME, RTLD_LAZY);
            if (!libcurl_Module)
                libcurl_Module=dlopen("./"MEDIAINFODLL_NAME, RTLD_LAZY);
            if (!libcurl_Module)
                libcurl_Module=dlopen("/usr/local/lib/"MEDIAINFODLL_NAME, RTLD_LAZY);
            if (!libcurl_Module)
                libcurl_Module=dlopen("/usr/local/lib64/"MEDIAINFODLL_NAME, RTLD_LAZY);
            if (!libcurl_Module)
                libcurl_Module=dlopen("/usr/lib/"MEDIAINFODLL_NAME, RTLD_LAZY);
            if (!libcurl_Module)
                libcurl_Module=dlopen("/usr/lib64/"MEDIAINFODLL_NAME, RTLD_LAZY);
        #endif
        if (!libcurl_Module)
            return ;

        /* Load methods */
        MEDIAINFO_ASSIGN    (curl_easy_init,            "curl_easy_init")
        MEDIAINFO_ASSIGN    (curl_easy_setopt,          "curl_easy_setopt")
        MEDIAINFO_ASSIGN    (curl_easy_perform,         "curl_easy_perform")
        MEDIAINFO_ASSIGN    (curl_easy_cleanup,         "curl_easy_cleanup")
        MEDIAINFO_ASSIGN    (curl_easy_getinfo,         "curl_easy_getinfo")
        MEDIAINFO_ASSIGN    (curl_slist_append,         "curl_slist_append")
        MEDIAINFO_ASSIGN    (curl_slist_free_all,       "curl_slist_free_all")
        MEDIAINFO_ASSIGN    (curl_easy_duphandle,       "curl_easy_duphandle")
        MEDIAINFO_ASSIGN    (curl_multi_init,           "curl_multi_init")
        MEDIAINFO_ASSIGN    (curl_multi_add_handle,     "curl_multi_add_handle")
        MEDIAINFO_ASSIGN    (curl_multi_remove_handle,  "curl_multi_remove_handle")
        MEDIAINFO_ASSIGN    (curl_multi_perform,        "curl_multi_perform")
        MEDIAINFO_ASSIGN    (curl_multi_cleanup,        "curl_multi_cleanup")
        if (Errors>0)
           return;

        libcurl_Module_Count++;
    #endif //defined MEDIAINFO_LIBCURL_DLL_RUNTIME
}

//---------------------------------------------------------------------------
Reader_libcurl::~Reader_libcurl ()
{
    //Cleanup
    if (Curl_Data->CurlM)
    {
         curl_multi_remove_handle(Curl_Data->CurlM, Curl_Data->Curl);
         curl_multi_cleanup(Curl_Data->CurlM);
    }
    if (Curl_Data->Curl)
        curl_easy_cleanup(Curl_Data->Curl);
    if (Curl_Data->HttpHeader)
        curl_slist_free_all(Curl_Data->HttpHeader);
    delete Curl_Data; //Curl_Data=NULL;
}

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
size_t Reader_libcurl::Format_Test(MediaInfo_Internal* MI, const String &File_Name)
{
    #if MEDIAINFO_EVENTS
        {
            struct MediaInfo_Event_General_Start_0 Event;
            Event.EventCode=MediaInfo_EventCode_Create(MediaInfo_Parser_None, MediaInfo_Event_General_Start, 0);
            Event.Stream_Size=File::Size_Get(File_Name);
            MI->Config.Event_Send((const int8u*)&Event, sizeof(MediaInfo_Event_General_Start_0));
        }
    #endif //MEDIAINFO_EVENTS

    //With Parser MultipleParsing
    return Format_Test_PerParser(MI, File_Name);
}

//---------------------------------------------------------------------------
size_t Reader_libcurl::Format_Test_PerParser(MediaInfo_Internal* MI, const String &File_Name)
{
    #if defined MEDIAINFO_LIBCURL_DLL_RUNTIME
        if (libcurl_Module_Count==0)
            return 0; //No libcurl library
    #endif //defined MEDIAINFO_LIBCURL_DLL_RUNTIME

    Curl_Data=new curl_data();
    Curl_Data->Curl=curl_easy_init();
    if (Curl_Data->Curl==NULL)
        return 0;
    #if MEDIAINFO_NEXTPACKET
        if (MI->Config.NextPacket_Get())
        {
            Curl_Data->CurlM=curl_multi_init( );
            if (Curl_Data->CurlM==NULL)
                return 0;
            CURLMcode CodeM=curl_multi_add_handle(Curl_Data->CurlM, Curl_Data->Curl);
            if (CodeM!=CURLM_OK)
                return 0;
            Curl_Data->NextPacket=true;
        }
    #endif //MEDIAINFO_NEXTPACKET
    Curl_Data->MI=MI;
    Curl_Data->File_Name=File_Name;
    string FileName_String=Ztring(Curl_Data->File_Name).To_Local();
    if (MI->Config.File_TimeToLive_Get())
        Curl_Data->Time_Max=time(0)+(time_t)MI->Config.File_TimeToLive_Get();
    if (!MI->Config.File_Curl_Get(_T("UserAgent")).empty())
        curl_easy_setopt(Curl_Data->Curl, CURLOPT_USERAGENT, MI->Config.File_Curl_Get(_T("UserAgent")).To_Local().c_str());
    if (!MI->Config.File_Curl_Get(_T("Proxy")).empty())
        curl_easy_setopt(Curl_Data->Curl, CURLOPT_PROXY, MI->Config.File_Curl_Get(_T("Proxy")).To_Local().c_str());
    if (!MI->Config.File_Curl_Get(_T("HttpHeader")).empty())
    {
        ZtringList HttpHeaderStrings; HttpHeaderStrings.Separator_Set(0, EOL); //End of line is set depending of the platform: \n on Linux, \r on Mac, or \r\n on Windows
        HttpHeaderStrings.Write(MI->Config.File_Curl_Get(_T("HttpHeader")));
        for (size_t Pos=0; Pos<HttpHeaderStrings.size(); Pos++)
            curl_slist_append(Curl_Data->HttpHeader, HttpHeaderStrings[Pos].To_Local().c_str());
        curl_easy_setopt(Curl_Data->Curl, CURLOPT_HTTPHEADER, Curl_Data->HttpHeader);
    }
    curl_easy_setopt(Curl_Data->Curl, CURLOPT_URL, FileName_String.c_str());
    curl_easy_setopt(Curl_Data->Curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(Curl_Data->Curl, CURLOPT_MAXREDIRS, 3);
    curl_easy_setopt(Curl_Data->Curl, CURLOPT_WRITEFUNCTION, &libcurl_WriteData_CallBack);
    curl_easy_setopt(Curl_Data->Curl, CURLOPT_WRITEDATA, Curl_Data);

    //Test the format with buffer
    return Format_Test_PerParser_Continue(MI);
}

//---------------------------------------------------------------------------
size_t Reader_libcurl::Format_Test_PerParser_Continue (MediaInfo_Internal* MI)
{
    bool StopAfterFilled=MI->Config.File_StopAfterFilled_Get();
    bool ShouldContinue=true;

    #if MEDIAINFO_DEMUX
    //PerPacket
    if (ShouldContinue && MI->Config.Demux_EventWasSent)
    {
        MI->Config.Demux_EventWasSent=false;

        Curl_Data->Status=MI->Open_Buffer_Continue(NULL, 0);

        //Demux
        if (MI->Config.Demux_EventWasSent)
            return 2; //Must return immediately

        //Threading
        if (MI->IsTerminating())
            return 1; //Termination is requested

        if (Curl_Data->Status[File__Analyze::IsFinished] || (StopAfterFilled && Curl_Data->Status[File__Analyze::IsFilled]))
            ShouldContinue=false;
    }
    #endif //MEDIAINFO_DEMUX

    if (ShouldContinue)
    {
        CURLcode Result=CURLE_WRITE_ERROR;
        while ((!(Curl_Data->Status[File__Analyze::IsFinished] || (StopAfterFilled && Curl_Data->Status[File__Analyze::IsFilled]))) && Result==CURLE_WRITE_ERROR)
        {
            //GoTo
            if (Curl_Data->MI->Open_Buffer_Continue_GoTo_Get()!=(int64u)-1)
            {
                #ifdef MEDIAINFO_DEBUG
                    std::cout<<std::hex<<Curl_Data->File_Offset-Curl_Data->Debug_BytesRead<<" - "<<Curl_Data->File_Offset<<" : "<<std::dec<<Curl_Data->Debug_BytesRead<<" bytes"<<std::endl;
                    Curl_Data->Debug_BytesRead=0;
                    Curl_Data->Debug_Count++;
                #endif //MEDIAINFO_DEBUG
                CURLcode Code;
                CURL* Temp=curl_easy_duphandle(Curl_Data->Curl);
                if (Temp==0)
                    return 0;
                if (Curl_Data->CurlM)
                        curl_multi_remove_handle(Curl_Data->CurlM, Curl_Data->Curl);
                curl_easy_cleanup(Curl_Data->Curl); Curl_Data->Curl=Temp;
                if (Curl_Data->CurlM)
                        curl_multi_add_handle(Curl_Data->CurlM, Curl_Data->Curl);
                if (Curl_Data->MI->Open_Buffer_Continue_GoTo_Get()<0x80000000)
                {
                    //We do NOT use large version if we can, because some version (tested: 7.15 linux) do NOT like large version (error code 18)
                    long File_GoTo_Long=(long)Curl_Data->MI->Open_Buffer_Continue_GoTo_Get();
                    Code=curl_easy_setopt(Curl_Data->Curl, CURLOPT_RESUME_FROM, File_GoTo_Long);
                }
                else
                {
                    curl_off_t File_GoTo_Off=(curl_off_t)Curl_Data->MI->Open_Buffer_Continue_GoTo_Get();
                    Code=curl_easy_setopt(Curl_Data->Curl, CURLOPT_RESUME_FROM_LARGE, File_GoTo_Off);
                }
                if (Code==CURLE_OK)
                    MI->Open_Buffer_Init((int64u)-1, Curl_Data->MI->Open_Buffer_Continue_GoTo_Get());
            }

            //Parsing
            #if MEDIAINFO_NEXTPACKET
                if (Curl_Data->NextPacket)
                {
                    int running_handles=0;
                    do
                    {
                        CURLMcode CodeM=curl_multi_perform(Curl_Data->CurlM, &running_handles);
                        if (CodeM!=CURLM_OK && CodeM!=CURLM_CALL_MULTI_PERFORM)
                            break; //There is a problem
                        #if MEDIAINFO_DEMUX
                            if (MI->Config.Demux_EventWasSent)
                                return 2; //Must return immediately
                        #endif //MEDIAINFO_DEMUX
                        if (running_handles==0)
                            break; //cUrl has finished
                    }
                    while (running_handles);
                    if (running_handles==0 && Curl_Data->MI->Open_Buffer_Continue_GoTo_Get()==(int64u)-1)
                        break; //cUrl has finished
                    Result=CURLE_WRITE_ERROR; //Configuring as if classic method is used
                }
                else
            #endif //MEDIAINFO_NEXTPACKET
                Result=curl_easy_perform(Curl_Data->Curl);

            #if MEDIAINFO_DEMUX
                if (MI->Config.Demux_EventWasSent)
                    return 2; //Must return immediately
            #endif //MEDIAINFO_DEMUX

            //Threading
            if (MI->IsTerminating())
                break; //Termination is requested
        }
    }

    #ifdef MEDIAINFO_DEBUG
        std::cout<<std::hex<<Curl_Data->File_Offset-Curl_Data->Debug_BytesRead<<" - "<<Curl_Data->File_Offset<<" : "<<std::dec<<Curl_Data->Debug_BytesRead<<" bytes"<<std::endl;
        std::cout<<"Total: "<<std::dec<<Curl_Data->Debug_BytesRead_Total<<" bytes in "<<Curl_Data->Debug_Count<<" blocks"<<std::endl;
    #endif //MEDIAINFO_DEBUG

    //Is this file detected?
    if (!Curl_Data->Status[File__Analyze::IsAccepted])
        return 0;

    MI->Open_Buffer_Finalize();

    #if MEDIAINFO_DEMUX
        if (MI->Config.Demux_EventWasSent)
            return 2; //Must return immediately
    #endif //MEDIAINFO_DEMUX

    return 1;
}

//---------------------------------------------------------------------------
#if MEDIAINFO_SEEK
size_t Reader_libcurl::Format_Test_PerParser_Seek (MediaInfo_Internal* MI, size_t Method, int64u Value, int64u ID)
{
    size_t ToReturn=MI->Open_Buffer_Seek(Method, Value, ID);

    if (ToReturn==0 || ToReturn==1)
    {
        //Reset
        Curl_Data->Status=0;
    }

    return ToReturn;
}
#endif //MEDIAINFO_SEEK

} //NameSpace

#endif //MEDIAINFO_LIBCURL_YES

