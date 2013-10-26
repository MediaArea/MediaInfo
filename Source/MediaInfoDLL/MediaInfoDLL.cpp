/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Public DLL interface implementation
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// For user: you can disable or enable it
//#define MEDIAINFO_DEBUG
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/Conf.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifdef UNICODE //DLL C Interface is currently done only in UNICODE mode
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//#if defined(MUST_INCLUDE_STDAFX) //Windows ATL for Shell Extension request stdafx.h
//    #include "stdafx.h"
//#endif //MUST_INCLUDE_STDAFX
#include "MediaInfo/MediaInfoList.h"
#include "MediaInfo/MediaInfo.h"
#define MEDIAINFO_DLL_EXPORT
#include "MediaInfoDLL_Static.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/CriticalSection.h"
#include <map>
#include <vector>
#include <clocale>
using namespace MediaInfoLib;
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
struct MI_List
{
    std::string  Ansi;    //One-Byte-sized characters
    std::wstring Unicode; //Unicode characters
};

struct MI_List_FromAnsi
{
    int8u Pos;                          //Where are we?
    std::vector<ZenLib::Ztring> Unicode;  //Unicode characters multiple times
};

std::map<void*, MI_List*> MI_Handle;
std::map<void*, MI_List_FromAnsi> MI_FromAnsi;
std::string  MediaInfo_Info_Ansi;    //For general information
std::wstring MediaInfo_Info_Unicode; //For general information

static bool v07Mode=false; //Legacy : Test if the user try to use <0.7 Open method
static CriticalSection Critical;
static bool utf8=false;

//---------------------------------------------------------------------------
const char* WC2MB(void* Handle, const wchar_t* Text)
{
    CriticalSectionLocker Locker(Critical);

    //Coherancy
    if (MI_Handle.find(Handle)==MI_Handle.end())
    {
        if (Handle==NULL && v07Mode)
            MediaInfo_Info_Ansi="Note to developer : you must create an object before using this method";
        else
            MediaInfo_Info_Ansi="Your software uses an outdated interface, you must use MediaInfo.DLL 0.6 instead"; //Compatibility <0.7 : return a message
        return MediaInfo_Info_Ansi.c_str();
    }

    //Adaptation
    if (utf8)
       MI_Handle[Handle]->Ansi=Ztring(Text).To_UTF8();
    else
       MI_Handle[Handle]->Ansi=Ztring(Text).To_Local();
    return MI_Handle[Handle]->Ansi.c_str();
}

//---------------------------------------------------------------------------
const wchar_t* MB2WC(void* Handle, const char* Text)
{
    //2 strings at same time
    if (MI_FromAnsi[Handle].Unicode.size()<2)
    {
        MI_FromAnsi[Handle].Pos=1;
        MI_FromAnsi[Handle].Unicode.resize(2);
    }
    MI_FromAnsi[Handle].Pos=(MI_FromAnsi[Handle].Pos==0?1:0); //Changing the position

    //Adaptation
    if (utf8)
        return MI_FromAnsi[Handle].Unicode[MI_FromAnsi[Handle].Pos].From_UTF8(Text).c_str();
    else
        return MI_FromAnsi[Handle].Unicode[MI_FromAnsi[Handle].Pos].From_Local(Text).c_str();
}

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
// For Widgets with DEBUG in BCB
// Here only because this is useful in all MediaInfo
#ifdef _DEBUG
    void wxOnAssert(const wchar_t*, int, const char*, const wchar_t*, const wchar_t*) {}
#endif //_DEBUG

//---------------------------------------------------------------------------
//To clarify the code
#ifdef MEDIAINFO_DEBUG
    #include <stdio.h>
    FILE* F;
    std::string Debug;

#undef MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG(_TOAPPEND) \
        F=fopen("MediaInfo_Debug.txt", "a+t"); \
        Debug.clear(); \
        _TOAPPEND; \
        fwrite(Debug.c_str(), Debug.size(), 1, F); \
        fclose(F);
#else // MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG(_TOAPPEND)
#endif // MEDIAINFO_DEBUG

//---------------------------------------------------------------------------
#define INTEGRITY_VOID(_DEBUGA) \
    MEDIAINFO_DEBUG(_DEBUGA) \
    CriticalSectionLocker Locker(Critical); \
    if (Handle==NULL || MI_Handle.find(Handle)==MI_Handle.end()) \
        return; \

#define INTEGRITY_SIZE_T(_DEBUGA) \
    MEDIAINFO_DEBUG(_DEBUGA) \
    CriticalSectionLocker Locker(Critical); \
    if (Handle==NULL || MI_Handle.find(Handle)==MI_Handle.end()) \
        return 0; \

#define INTEGRITY_INT64U(_DEBUGA) \
    MEDIAINFO_DEBUG(_DEBUGA) \
    CriticalSectionLocker Locker(Critical); \
    if (Handle==NULL || MI_Handle.find(Handle)==MI_Handle.end()) \
        return 0; \

#define INTEGRITY_STRING(_DEBUGA) \
    MEDIAINFO_DEBUG(_DEBUGA) \
    CriticalSectionLocker Locker(Critical); \
    if (Handle==NULL || MI_Handle.find(Handle)==MI_Handle.end()) \
    { \
        if (v07Mode==false) \
        { \
            MediaInfo_Info_Unicode=L"Your software uses an outdated interface, You must use MediaInfoList.DLL 0.6 instead"; \
            return MediaInfo_Info_Unicode.c_str(); \
        } \
        MI_Handle[NULL]->Unicode==L"Note to developer : you must create an object before"; \
        return MI_Handle[NULL]->Unicode.c_str(); \
    } \

#ifndef MEDIAINFO_DEBUG
#define EXECUTE_VOID(_CLASS,_METHOD,_DEBUGB) \
    try \
    { \
        ((_CLASS*)Handle)->_METHOD; \
    } catch (...) {return;}
#else //MEDIAINFO_DEBUG
#define EXECUTE_VOID(_CLASS,_METHOD,_DEBUGB) \
    try \
    { \
        ((_CLASS*)Handle)->_METHOD; \
        MEDIAINFO_DEBUG(_DEBUGB) \
    } catch (...) {return;}
#endif //MEDIAINFO_DEBUG

#ifndef MEDIAINFO_DEBUG
#define EXECUTE_SIZE_T(_CLASS,_METHOD,_DEBUGB) \
    try \
    { \
        return ((_CLASS*)Handle)->_METHOD; \
    } catch (...) {return -1;}
#else //MEDIAINFO_DEBUG
#define EXECUTE_SIZE_T(_CLASS,_METHOD, _DEBUGB) \
    try \
    { \
        size_t ToReturn=((_CLASS*)Handle)->_METHOD; \
        MEDIAINFO_DEBUG(_DEBUGB) \
        return ToReturn; \
    } catch (...) {MEDIAINFO_DEBUG(Debug+="!!!Exception thrown!!!\r\n";) return (size_t)-1;}

#define EXECUTE_INT64U(_CLASS,_METHOD, _DEBUGB) \
    try \
    { \
        int64u ToReturn=((_CLASS*)Handle)->_METHOD; \
        MEDIAINFO_DEBUG(_DEBUGB) \
        return ToReturn; \
    } catch (...) {MEDIAINFO_DEBUG(Debug+="!!!Exception thrown!!!\r\n";) return (size_t)-1;}
#endif //MEDIAINFO_DEBUG

#ifndef MEDIAINFO_DEBUG
#define EXECUTE_STRING(_CLASS,_METHOD,_DEBUGB) \
    try \
    { \
        MI_Handle[Handle]->Unicode=((_CLASS*)Handle)->_METHOD; \
    } catch (...) {MI_Handle[Handle]->Unicode.clear();} \
    return MI_Handle[Handle]->Unicode.c_str();
#else //MEDIAINFO_DEBUG
#define EXECUTE_STRING(_CLASS,_METHOD,_DEBUGB) \
    try \
    { \
        MI_Handle[Handle]->Unicode=((_CLASS*)Handle)->_METHOD; \
    } catch (...) {MEDIAINFO_DEBUG(Debug+="!!!Exception thrown!!!\r\n";) MI_Handle[Handle]->Unicode.clear();} \
    ZenLib::Ztring ToReturn=MI_Handle[Handle]->Unicode; \
    MEDIAINFO_DEBUG(_DEBUGB) \
    return MI_Handle[Handle]->Unicode.c_str();
#endif //MEDIAINFO_DEBUG

#define MANAGE_VOID(_CLASS,_METHOD,_DEBUGA,_DEBUGB) \
    INTEGRITY_VOID(_DEBUGA) EXECUTE_VOID(_CLASS,_METHOD,_DEBUGB)

#define MANAGE_INT(_CLASS,_METHOD,_DEBUGA,_DEBUGB) \
    INTEGRITY_SIZE_T(_DEBUGA) EXECUTE_SIZE_T(_CLASS,_METHOD,_DEBUGB)

#define MANAGE_STRING(_CLASS,_METHOD,_DEBUGA,_DEBUGB) \
    INTEGRITY_STRING(_DEBUGA) EXECUTE_STRING(_CLASS,_METHOD,_DEBUGB)

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------

void*           __stdcall MediaInfoA_New ()
{
    MEDIAINFO_DEBUG(Debug+="MediaInfoA_New(), no arguments\r\n";)
    return MediaInfo_New();
}

void*           __stdcall MediaInfoA_New_Quick (const char* File, const char* Options)
{
    return MediaInfo_New_Quick(MB2WC(NULL, File), MB2WC(NULL, Options));
}

void            __stdcall MediaInfoA_Delete (void* Handle)
{
    MediaInfo_Delete(Handle);
}

size_t          __stdcall MediaInfoA_Open (void* Handle, const char* File)
{
    MEDIAINFO_DEBUG(Debug+="OpenA, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", File=";Debug+=File;Debug+="\r\n";)
    return MediaInfo_Open(Handle, MB2WC(Handle, File));
}

size_t          __stdcall MediaInfoA_Open_Buffer (void* Handle, const unsigned char* Begin, size_t  Begin_Size, const unsigned char* End, size_t  End_Size)
{
    return MediaInfo_Open_Buffer(Handle, Begin, Begin_Size, End, End_Size);
}

size_t          __stdcall MediaInfoA_Open_Buffer_Init (void* Handle, MediaInfo_int64u File_Size, MediaInfo_int64u File_Offset)
{
    return MediaInfo_Open_Buffer_Init(Handle, File_Size, File_Offset);
}

size_t          __stdcall MediaInfoA_Open_Buffer_Continue (void* Handle, MediaInfo_int8u* Buffer, size_t Buffer_Size)
{
    return MediaInfo_Open_Buffer_Continue(Handle, Buffer, Buffer_Size);
}

MediaInfo_int64u __stdcall MediaInfoA_Open_Buffer_Continue_GoTo_Get (void* Handle)
{
    return MediaInfo_Open_Buffer_Continue_GoTo_Get(Handle);
}

size_t          __stdcall MediaInfoA_Open_Buffer_Finalize (void* Handle)
{
    return MediaInfo_Open_Buffer_Finalize(Handle);
}

size_t          __stdcall MediaInfoA_Open_NextPacket (void* Handle)
{
    return MediaInfo_Open_NextPacket(Handle);
}

size_t          __stdcall MediaInfoA_Save (void* Handle)
{
    return MediaInfo_Save(Handle);
}

void            __stdcall MediaInfoA_Close (void* Handle)
{
    MediaInfo_Close(Handle);
}

const char*     __stdcall MediaInfoA_Inform (void* Handle, size_t Reserved)
{
        return WC2MB(Handle, MediaInfo_Inform(Handle, 0));
}

const char*     __stdcall MediaInfoA_GetI (void* Handle, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, MediaInfo_info_C KindOfInfo)
{
    return WC2MB(Handle, MediaInfo_GetI(Handle, StreamKind, StreamNumber, Parameter, KindOfInfo));
}

const char*     __stdcall MediaInfoA_Get (void* Handle, MediaInfo_stream_t StreamKind, size_t StreamNumber, const char* Parameter, MediaInfo_info_C KindOfInfo, MediaInfo_info_C KindOfSearch)
{
    MEDIAINFO_DEBUG(Debug+="Get, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", Parameter=";Parameter;Debug+="\r\n";)
    return WC2MB(Handle, MediaInfo_Get(Handle, StreamKind, StreamNumber, MB2WC(Handle, Parameter), KindOfInfo, KindOfSearch));
}

size_t          __stdcall MediaInfoA_SetI (void* Handle, const char* ToSet, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, const char* OldParameter)
{
    return MediaInfo_SetI(Handle, MB2WC(Handle, ToSet), StreamKind, StreamNumber, Parameter, MB2WC(Handle, OldParameter));
}

size_t          __stdcall MediaInfoA_Set (void* Handle, const char* ToSet, MediaInfo_stream_t StreamKind, size_t StreamNumber, const char* Parameter, const char* OldParameter)
{
    return MediaInfo_Set(Handle, MB2WC(Handle, ToSet), StreamKind, StreamNumber, MB2WC(Handle, Parameter), MB2WC(Handle, OldParameter));
}

size_t          __stdcall MediaInfoA_Output_Buffer_Get (void* Handle, const char* Value)
{
    return MediaInfo_Output_Buffer_Get(Handle, MB2WC(Handle, Value));
}

size_t          __stdcall MediaInfoA_Output_Buffer_GetI (void* Handle, size_t Pos)
{
    return MediaInfo_Output_Buffer_GetI(Handle, Pos);
}

const char*     __stdcall MediaInfoA_Option (void* Handle, const char* Option, const char* Value)
{
    return WC2MB(Handle, MediaInfo_Option(Handle, MB2WC(Handle, Option), MB2WC(Handle, Value)));
}

size_t          __stdcall MediaInfoA_State_Get(void* Handle)
{
    return MediaInfo_State_Get(Handle);
}

size_t          __stdcall MediaInfoA_Count_Get(void* Handle, MediaInfo_stream_t StreamKind, size_t StreamNumber)
{
    return MediaInfo_Count_Get(Handle, StreamKind, StreamNumber);
}

//---------------------------------------------------------------------------

void*           __stdcall MediaInfoListA_New ()
{
    return MediaInfoList_New();
}

void*           __stdcall MediaInfoListA_New_Quick (const char* File, const char* Options)
{
    return MediaInfoList_New_Quick(MB2WC(NULL, File), MB2WC(NULL, Options));
}

void            __stdcall MediaInfoListA_Delete (void* Handle)
{
    MediaInfoList_Delete(Handle);
}

size_t          __stdcall MediaInfoListA_Open (void* Handle, const char* File, const MediaInfo_fileoptions_C Options)
{
    return MediaInfoList_Open(Handle, MB2WC(Handle, File), Options);
}

size_t          __stdcall MediaInfoListA_Open_Buffer (void* Handle, const unsigned char* Begin, size_t  Begin_Size, const unsigned char* End, size_t  End_Size)
{
    return MediaInfoList_Open_Buffer(Handle, Begin, Begin_Size, End, End_Size);
}

size_t          __stdcall MediaInfoListA_Save (void* Handle, size_t  FilePos)
{
    return MediaInfoList_Save(Handle, FilePos);
}

void            __stdcall MediaInfoListA_Close (void* Handle, size_t  FilePos)
{
    MediaInfoList_Close(Handle, FilePos);
}

const char*     __stdcall MediaInfoListA_Inform (void* Handle, size_t  FilePos, size_t Reserved)
{
    return WC2MB(Handle, MediaInfoList_Inform(Handle, FilePos, 0));
}

const char*     __stdcall MediaInfoListA_GetI (void* Handle, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, MediaInfo_info_C KindOfInfo)
{
    return WC2MB(Handle, MediaInfoList_GetI(Handle, FilePos, StreamKind, StreamNumber, Parameter, KindOfInfo));
}

const char*     __stdcall MediaInfoListA_Get (void* Handle, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, const char* Parameter, MediaInfo_info_C KindOfInfo, MediaInfo_info_C KindOfSearch)
{
    return WC2MB(Handle, MediaInfoList_Get(Handle, FilePos, StreamKind, StreamNumber, MB2WC(Handle, Parameter), KindOfInfo, KindOfSearch));
}

size_t          __stdcall MediaInfoListA_SetI (void* Handle, const char* ToSet, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, const char* OldParameter)
{
    return MediaInfoList_SetI(Handle, MB2WC(Handle, ToSet), FilePos, StreamKind, StreamNumber, Parameter, MB2WC(Handle, OldParameter));
}

size_t          __stdcall MediaInfoListA_Set (void* Handle, const char* ToSet, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, const char* Parameter, const char* OldParameter)
{
    return MediaInfoList_Set(Handle, MB2WC(Handle, ToSet), FilePos, StreamKind, StreamNumber, MB2WC(Handle, Parameter), MB2WC(Handle, OldParameter));
}

const char*     __stdcall MediaInfoListA_Option (void* Handle, const char* Option, const char* Value)
{
    return WC2MB(Handle, MediaInfoList_Option(Handle, MB2WC(Handle, Option), MB2WC(Handle, Value)));
}

size_t          __stdcall MediaInfoListA_State_Get(void* Handle)
{
    return MediaInfoList_State_Get(Handle);
}

size_t          __stdcall MediaInfoListA_Count_Get(void* Handle, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber)
{
    return MediaInfoList_Count_Get(Handle, FilePos, StreamKind, StreamNumber);
}

size_t          __stdcall MediaInfoListA_Count_Get_Files(void* Handle)
{
    return MediaInfoList_Count_Get_Files(Handle);
}

//---------------------------------------------------------------------------

void*           __stdcall MediaInfo_New ()
{
    MEDIAINFO_DEBUG(Debug+="New, no arguments\r\n";)
    CriticalSectionLocker Locker(Critical);

    //First init
    if (MI_Handle.find(NULL)==MI_Handle.end())
    {
        MI_Handle[NULL]=new MI_List; //Generic Handle
        MI_Handle[NULL]->Ansi.clear();
    }
    v07Mode=true; //Application is compatible with v0.7+ interface

    //New
    MediaInfo* M=NULL;
    try
    {
        M=new MediaInfo;
        MI_Handle[M]=new MI_List;
        MI_Handle[M]->Ansi.clear();
        MEDIAINFO_DEBUG(Debug+="New, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)M).To_Local().c_str();Debug+="\r\n";)
        return M;
    }
    catch(...)
    {
        delete M;
        return NULL;
    }
}

void*           __stdcall MediaInfo_New_Quick (const wchar_t* File, const wchar_t* Options)
{
    MediaInfo_Option(NULL, L"QuickInit", Options);
    void* Handle=MediaInfo_New();
    if (MediaInfo_Open(Handle, File)==0)
    {
        //No valid files, return NULL
        MediaInfo* M=(MediaInfo*)Handle;
        delete M;
        return NULL;
    }
    return Handle;
}

void            __stdcall MediaInfo_Delete (void* Handle)
{
    CriticalSectionLocker Locker(Critical);

    //Integrity test
    if (MI_Handle.find(Handle)==MI_Handle.end())
        return; //Handle is not a MediaInfo Handle

    //Delete the object
    MediaInfo* M=(MediaInfo*)Handle;
    delete MI_Handle[M];
    MI_Handle.erase(M);
    delete M;

    //In case of the last object : delete the NULL object, no more need
    if (MI_Handle.size()==1 && MI_Handle.find(NULL)!=MI_Handle.end())
    {
        delete MI_Handle[NULL];
        MI_Handle.erase(NULL);
    }
}

size_t          __stdcall MediaInfo_Open (void* Handle, const wchar_t* File)
{
    INTEGRITY_SIZE_T(Debug+="Open, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", File=";Debug+=ZenLib::Ztring(File).To_Local();Debug+="\r\n";)

    if (MI_Handle.find(Handle)==MI_Handle.end())
    {
        //Compatibility <0.7 : return a MediaInfo handle, but empty
        MediaInfo* M=new MediaInfo;
        return (size_t)M;
    }

    EXECUTE_SIZE_T(MediaInfo, Open(File), Debug+="Open, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Open_Buffer (void* Handle, const unsigned char* Begin, size_t  Begin_Size, const unsigned char* End, size_t  End_Size)
{
    INTEGRITY_SIZE_T(Debug+="Open_Buffer, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    if (MI_Handle.find(Handle)==MI_Handle.end())
    {
        //Compatibility <0.7 : return a MediaInfo handle, but empty
        MediaInfo* M=new MediaInfo;
        return (size_t)M;
    }

    EXECUTE_SIZE_T(MediaInfo, Open(Begin, Begin_Size, End, End_Size), Debug+="Open_Buffer, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Open_Buffer_Init (void* Handle, MediaInfo_int64u File_Size, MediaInfo_int64u File_Offset)
{
    INTEGRITY_SIZE_T(Debug+="Open_Buffer_Init, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    EXECUTE_SIZE_T(MediaInfo, Open_Buffer_Init(File_Size, File_Offset), Debug+="Open_Buffer_Init, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Open_Buffer_Continue (void* Handle, MediaInfo_int8u* Buffer, size_t Buffer_Size)
{
    INTEGRITY_SIZE_T(Debug+="MediaInfo_Open_Buffer_Continue, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    EXECUTE_SIZE_T(MediaInfo, Open_Buffer_Continue(Buffer, Buffer_Size), Debug+="MediaInfo_Open_Buffer_Continue, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

MediaInfo_int64u __stdcall MediaInfo_Open_Buffer_Continue_GoTo_Get (void* Handle)
{
    INTEGRITY_INT64U(Debug+="MediaInfo_Open_Buffer_Continue_GoTo_Get, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    EXECUTE_INT64U(MediaInfo, Open_Buffer_Continue_GoTo_Get(), Debug+="MediaInfo_Open_Buffer_Continue_GoTo_Get, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Open_Buffer_Finalize (void* Handle)
{
    INTEGRITY_SIZE_T(Debug+="MediaInfo_Open_Buffer_Finalize, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    EXECUTE_SIZE_T(MediaInfo, Open_Buffer_Finalize(), Debug+="MediaInfo_Open_Buffer_Finalize, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Open_NextPacket (void* Handle)
{
    INTEGRITY_SIZE_T(Debug+="MediaInfo_Open_NextPacket, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    EXECUTE_SIZE_T(MediaInfo, Open_NextPacket(), Debug+="MediaInfo_Open_NextPacket, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Save (void* Handle)
{
    MANAGE_INT(MediaInfo, Save(), Debug+="Save, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Save, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

void            __stdcall MediaInfo_Close (void* Handle)
{
    MANAGE_VOID(MediaInfo, Close(), Debug+="Close, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";,  Debug+="Close, will return nothing\r\n";)
}

const wchar_t*  __stdcall MediaInfo_Inform (void* Handle, size_t Reserved)
{
    MANAGE_STRING(MediaInfo, Inform(), Debug+="Inform, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Inform, will return ";Debug+=ToReturn.To_Local().c_str();Debug+="\r\n";)
}

const wchar_t*  __stdcall MediaInfo_GetI (void* Handle, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, MediaInfo_info_C KindOfInfo)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    KindOfInfo=(MediaInfo_info_C)  (((size_t)KindOfInfo)&0xFF);
    MANAGE_STRING(MediaInfo, Get((stream_t)StreamKind, StreamNumber, Parameter, (info_t)KindOfInfo), Debug+="GetI, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="GetI, will return ";Debug+=ToReturn.To_Local().c_str();Debug+="\r\n";)
}

const wchar_t*  __stdcall MediaInfo_Get (void* Handle, MediaInfo_stream_t StreamKind, size_t StreamNumber, const wchar_t* Parameter, MediaInfo_info_C KindOfInfo, MediaInfo_info_C KindOfSearch)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    KindOfInfo=(MediaInfo_info_C)  (((size_t)KindOfInfo)&0xFF);
    KindOfSearch=(MediaInfo_info_C)(((size_t)KindOfSearch)&0xFF);
    MANAGE_STRING(MediaInfo, Get((stream_t)StreamKind, StreamNumber, Parameter, (info_t)KindOfInfo, (info_t)KindOfSearch), Debug+="Get, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", Parameter=";Debug+=ZenLib::Ztring(Parameter).To_Local().c_str();Debug+=", KindOfInfo=";Debug+=ZenLib::Ztring::ToZtring(KindOfInfo).To_Local().c_str();Debug+=", KindOfSearch=";Debug+=ZenLib::Ztring::ToZtring(KindOfSearch).To_Local().c_str();Debug+="\r\n";, Debug+="Get, will return ";Debug+=ToReturn.To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_SetI (void* Handle, const wchar_t* ToSet, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, const wchar_t* OldParameter)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    MANAGE_INT(MediaInfo, Set(ToSet, (stream_t)StreamKind, StreamNumber, Parameter, OldParameter), Debug+="SetI, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="SetI, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Set (void* Handle, const wchar_t* ToSet, MediaInfo_stream_t StreamKind, size_t StreamNumber, const wchar_t* Parameter, const wchar_t* OldParameter)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    MANAGE_INT(MediaInfo, Set(ToSet, (stream_t)StreamKind, StreamNumber, Parameter, OldParameter), Debug+="Set, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Set, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Output_Buffer_Get (void* Handle, const wchar_t* Value)
{
    MANAGE_INT(MediaInfo, Output_Buffer_Get(Value), ;, ;)
}

size_t          __stdcall MediaInfo_Output_Buffer_GetI (void* Handle, size_t Pos)
{
    MANAGE_INT(MediaInfo, Output_Buffer_Get(Pos), ;, ;)
}

const wchar_t*     __stdcall MediaInfo_Option (void* Handle, const wchar_t* Option, const wchar_t* Value)
{
    MEDIAINFO_DEBUG(Debug+="Option, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", Option=";Debug+=ZenLib::Ztring(Option).To_Local().c_str();Debug+="\r\n";)
    CriticalSectionLocker Locker(Critical);

    //First init - Option could be called without using MediaInfo_New()
    if (MI_Handle.find(NULL)==MI_Handle.end())
    {
        MI_Handle[NULL]=new MI_List; //Generic Handle
        MI_Handle[NULL]->Ansi.clear();
    }

    //DLL only option
    if (ZenLib::Ztring(Option).Compare(L"CharSet", L"=="))
    {
        if (ZenLib::Ztring(Value).Compare(L"UTF-8", L"=="))
            utf8=true;
        else
            utf8=false;
        MI_Handle[NULL]->Unicode=L"OK";
        return MI_Handle[NULL]->Unicode.c_str();
    }

    if (Handle)
    {
        if (MI_Handle.find(Handle)==MI_Handle.end())
        {
            if (v07Mode==false)
            {
                MediaInfo_Info_Unicode=L"Your software uses an outdated interface, You must use MediaInfoList.DLL 0.6 instead";
                return MediaInfo_Info_Unicode.c_str();
            }
              MI_Handle[NULL]->Unicode==L"Note to developer : you must create an object before";
            return MI_Handle[NULL]->Unicode.c_str();
        }
        EXECUTE_STRING(MediaInfo, Option(Option, Value), Debug+="Option, will return ";Debug+=ToReturn.To_Local().c_str();Debug+="\r\n";)
    }
    else
    {
        MI_Handle[Handle]->Unicode=MediaInfo::Option_Static(Option, Value);
        return MI_Handle[Handle]->Unicode.c_str();
    }
}

size_t          __stdcall MediaInfo_State_Get(void* Handle)
{
    MANAGE_INT(MediaInfo, State_Get(), Debug+="State_Get, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="State_Get, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfo_Count_Get(void* Handle, MediaInfo_stream_t StreamKind, size_t StreamNumber)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    MANAGE_INT(MediaInfo, Count_Get((stream_t)StreamKind, StreamNumber), Debug+="Count_Get, Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", StreamKind=";Debug+=ZenLib::Ztring::ToZtring((size_t)StreamKind).To_Local().c_str();Debug+=", StreamNumber=";Debug+=ZenLib::Ztring::ToZtring((size_t)StreamNumber).To_Local().c_str();Debug+="\r\n", Debug+="Count_Get, will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

//---------------------------------------------------------------------------

void*           __stdcall MediaInfoList_New ()
{
    MEDIAINFO_DEBUG(Debug+="New(L)";Debug+="\r\n";)
    CriticalSectionLocker Locker(Critical);

    //First init
    if (MI_Handle.find(NULL)==MI_Handle.end())
    {
        MI_Handle[NULL]=new MI_List; //Generic Handle
        MI_Handle[NULL]->Ansi.clear();
    }
    v07Mode=true; //Application is compatible with v0.7+ interface

    //New
    MediaInfoList* M=NULL;
    try
    {
        M=new MediaInfoList;
        MI_Handle[M]=new MI_List;
        MI_Handle[M]->Ansi.clear();
        MEDIAINFO_DEBUG(Debug+="New(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)M).To_Local().c_str();Debug+="\r\n";)
        return M;
    }
    catch(...)
    {
        delete M;
        return NULL;
    }
}

void*           __stdcall MediaInfoList_New_Quick (const wchar_t* File, const wchar_t* Options)
{
    MediaInfoList_Option(NULL, L"QuickInit", Options);
    void* Handle=MediaInfoList_New();
    if (MediaInfoList_Open(Handle, File, MediaInfo_FileOption_Nothing)==0)
    {
        //No valid files, return NULL
        MediaInfoList* M=(MediaInfoList*)Handle;
        delete M;
        return NULL;
    }
    return Handle;
}

void            __stdcall MediaInfoList_Delete (void* Handle)
{
    CriticalSectionLocker Locker(Critical);

    //Integrity test
    if (MI_Handle.find(Handle)==MI_Handle.end())
        return; //Handle is not a MediaInfoList Handle

    //Delete the object
    MediaInfoList* M=(MediaInfoList*)Handle;
    delete MI_Handle[M];
    MI_Handle.erase(M);
    delete M;

    //In case of the last object : delete the NULL object, no more need
    if (MI_Handle.size()==1 && MI_Handle.find(NULL)!=MI_Handle.end())
    {
        delete MI_Handle[NULL];
        MI_Handle.erase(NULL);
    }
}

size_t          __stdcall MediaInfoList_Open (void* Handle, const wchar_t* File, const MediaInfo_fileoptions_C Options)
{
    INTEGRITY_SIZE_T(Debug+="Open(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", ";Debug+="\r\n";)

    if (MI_Handle.find(Handle)==MI_Handle.end())
    {
        //Compatibility <0.7 : return a MediaInfo handle, but empty
        MediaInfo* M=new MediaInfo;
        return (size_t)M;
    }

    EXECUTE_SIZE_T(MediaInfoList, Open(File, (fileoptions_t)Options), Debug+="Open(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfoList_Open_Buffer (void* Handle, const unsigned char* Begin, size_t  Begin_Size, const unsigned char* End, size_t  End_Size)
{
    INTEGRITY_SIZE_T(Debug+="Open_Buffer(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";)

    if (MI_Handle.find(Handle)==MI_Handle.end())
    {
        //Compatibility <0.7 : return a MediaInfo handle, but empty
        MediaInfo* M=new MediaInfo;
        return (size_t)M;
    }

    return 0; //No more implemented
}

size_t          __stdcall MediaInfoList_Save (void* Handle, size_t  FilePos)
{
    MANAGE_INT(MediaInfoList, Save(FilePos), Debug+="Save(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Save(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

void            __stdcall MediaInfoList_Close (void* Handle, size_t  FilePos)
{
    MANAGE_VOID(MediaInfoList, Close(FilePos), Debug+="MediaInfoList_Close\r\n";, Debug+="MediaInfoList_Close 2\r\n";)
}

const wchar_t*  __stdcall MediaInfoList_Inform (void* Handle, size_t  FilePos, size_t Reserved)
{
    MANAGE_STRING(MediaInfoList, Inform(FilePos, 0), Debug+="Inform(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Inform(L), will return ";Debug+=ToReturn.To_Local();Debug+="\r\n";)
}

const wchar_t*  __stdcall MediaInfoList_GetI (void* Handle, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, MediaInfo_info_C KindOfInfo)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    KindOfInfo=(MediaInfo_info_C)  (((size_t)KindOfInfo)&0xFF);
    MANAGE_STRING(MediaInfoList, Get(FilePos, (stream_t)StreamKind, StreamNumber, Parameter, (info_t)KindOfInfo), Debug+="Get(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((int64u)Handle).To_Local().c_str();Debug+=", FilePos=";Debug+=ZenLib::Ztring::ToZtring((size_t)FilePos).To_Local().c_str();Debug+=", StreamKind=";Debug+=ZenLib::Ztring::ToZtring((int8u)StreamKind).To_Local().c_str();Debug+=", StreamNumber=";Debug+=ZenLib::Ztring::ToZtring((int8u)StreamNumber).To_Local().c_str();Debug+=", Parameter=";Debug+=ZenLib::Ztring::ToZtring(Parameter).To_Local().c_str();Debug+=", KindOfInfo=";Debug+=ZenLib::Ztring::ToZtring((int8u)KindOfInfo).To_Local().c_str();Debug+="\r\n";, Debug+="Get(L), will return ";Debug+=ToReturn.To_Local();Debug+="\r\n";)
}

const wchar_t*  __stdcall MediaInfoList_Get (void* Handle, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, const wchar_t* Parameter, MediaInfo_info_C KindOfInfo, MediaInfo_info_C KindOfSearch)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    KindOfInfo=(MediaInfo_info_C)  (((size_t)KindOfInfo)&0xFF);
    MANAGE_STRING(MediaInfoList, Get(FilePos, (stream_t)((int8u)StreamKind), StreamNumber, Parameter, (info_t)((int8u)KindOfInfo), (info_t)((int8u)KindOfSearch)), Debug+="Get(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", FilePos=";Debug+=ZenLib::Ztring::ToZtring((size_t)FilePos).To_Local().c_str();Debug+=", StreamKind=";Debug+=ZenLib::Ztring::ToZtring((int8u)StreamKind).To_Local().c_str();Debug+=", StreamNumber=";Debug+=ZenLib::Ztring::ToZtring((int8u)StreamNumber).To_Local().c_str();Debug+=", Parameter=";Debug+=ZenLib::Ztring(Parameter).To_Local().c_str();Debug+=", KindOfInfo=";Debug+=ZenLib::Ztring::ToZtring((int8u)KindOfInfo).To_Local().c_str();Debug+=", KindOfSearch=";Debug+=ZenLib::Ztring::ToZtring((int8u)KindOfSearch).To_Local().c_str();Debug+="\r\n";, Debug+="Get(L), will return ";Debug+=ToReturn.To_Local();Debug+="\r\n";)
}

size_t          __stdcall MediaInfoList_SetI (void* Handle, const wchar_t* ToSet, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, size_t  Parameter, const wchar_t* OldParameter)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    MANAGE_INT(MediaInfoList, Set(ToSet, FilePos, (stream_t)StreamKind, StreamNumber, Parameter, OldParameter), Debug+="SetI(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="SetI(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfoList_Set (void* Handle, const wchar_t* ToSet, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber, const wchar_t* Parameter, const wchar_t* OldParameter)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    MANAGE_INT(MediaInfoList, Set(ToSet, FilePos, (stream_t)StreamKind, StreamNumber, Parameter, OldParameter), Debug+="Set(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Set(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

const wchar_t*     __stdcall MediaInfoList_Option (void* Handle, const wchar_t* Option, const wchar_t* Value)
{
    MEDIAINFO_DEBUG(Debug+="Option(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", Option=";Debug+=ZenLib::Ztring(Option).To_Local().c_str();Debug+=", Value=";Debug+=ZenLib::Ztring(Value).To_Local().c_str();Debug+="\r\n";)
    CriticalSectionLocker Locker(Critical);

    //First init - Option could be called without using MediaInfo_New()
    if (MI_Handle.find(NULL)==MI_Handle.end())
    {
        MI_Handle[NULL]=new MI_List; //Generic Handle
        MI_Handle[NULL]->Ansi.clear();
    }

    //DLL only option
    if (std::wstring(Option).compare(L"CharSet")==0)
    {
        if (std::wstring(Value).compare(L"UTF-8")==0)
            utf8=true;
        else
            utf8=false;
        MI_Handle[NULL]->Unicode=L"OK";
        return MI_Handle[NULL]->Unicode.c_str();
    }

    if (Handle)
    {
        if (MI_Handle.find(Handle)==MI_Handle.end())
        {
            if (v07Mode==false)
            {
                MediaInfo_Info_Unicode=L"Your software uses an outdated interface, You must use MediaInfoList.DLL 0.6 instead";
                return MediaInfo_Info_Unicode.c_str();
            }
            MI_Handle[NULL]->Unicode==L"Note to developer : you must create an object before";
            return MI_Handle[NULL]->Unicode.c_str();
        }
        EXECUTE_STRING(MediaInfoList, Option(Option, Value), Debug+="Option(L), will return ";Debug+=ToReturn.To_Local().c_str();Debug+="\r\n";)
    }
    else
    {
        MI_Handle[Handle]->Unicode=MediaInfoList::Option_Static(Option, Value);
        return MI_Handle[Handle]->Unicode.c_str();
    }
}

size_t          __stdcall MediaInfoList_State_Get(void* Handle)
{
    MANAGE_INT(MediaInfoList, State_Get(), Debug+="State_Get(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="State_Get(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfoList_Count_Get(void* Handle, size_t  FilePos, MediaInfo_stream_t StreamKind, size_t StreamNumber)
{
    StreamKind=(MediaInfo_stream_t)(((size_t)StreamKind)&0xFF);
    MANAGE_INT(MediaInfoList, Count_Get(FilePos, (stream_t)StreamKind, StreamNumber), Debug+="Count_Get(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+=", FilePos=";Debug+=ZenLib::Ztring::ToZtring((size_t)FilePos).To_Local().c_str();Debug+=", StreamKind=";Debug+=ZenLib::Ztring::ToZtring((size_t)StreamKind).To_Local().c_str();Debug+=", StreamNumber=";Debug+=ZenLib::Ztring::ToZtring((size_t)StreamNumber).To_Local().c_str();Debug+="\r\n";, Debug+="Count_Get(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

size_t          __stdcall MediaInfoList_Count_Get_Files(void* Handle)
{
    MANAGE_INT(MediaInfoList, Count_Get(), Debug+="Count_Get_Files(L), Handle=";Debug+=ZenLib::Ztring::ToZtring((size_t)Handle).To_Local().c_str();Debug+="\r\n";, Debug+="Count_Get_Files(L), will return ";Debug+=ZenLib::Ztring::ToZtring((size_t)ToReturn).To_Local().c_str();Debug+="\r\n";)
}

//---------------------------------------------------------------------------

const char*     __stdcall MediaInfo_Info_Version()
{
    MEDIAINFO_DEBUG("Info_Version") \
    //Compatibility <0.7 : return a message
    MediaInfo_Info_Ansi="Your software uses an outdated interface, You must use MediaInfo.DLL 0.4.1.1 instead";
    return MediaInfo_Info_Ansi.c_str();
    //wchar_t* MediaInfo_wChar=new wchar_t[1000];
    //GetModuleFileNameW (NULL, MediaInfo_wChar, 1000);
    //return WC2MB(MediaInfo_wChar);
}

//***************************************************************************
//
//***************************************************************************

#endif //UNICODE
