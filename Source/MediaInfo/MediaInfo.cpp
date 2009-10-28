// MediaInfo - All info about media files
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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
//#define MEDIAINFO_DEBUG_BUFFER_SAVE
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfo_Internal.h"
#ifdef MEDIAINFO_DEBUG
    #include <ZenLib/File.h>
#endif //MEDIAINFO_DEBUG
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
//To clarify the code
namespace MediaInfo_Debug_MediaInfo
{

#ifdef MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG_WANTED
    File F;
    Ztring Debug;
    #ifdef WINDOWS
        const Char* MediaInfo_Debug_Name=_T("MediaInfo_Debug");
    #else
        const Char* MediaInfo_Debug_Name=_T("/tmp/MediaInfo_Debug");
    #endif

    #undef MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG(_TOAPPEND) \
        F.Open(Ztring(MediaInfo_Debug_Name)+_T(".")+Ztring::ToZtring((size_t)this, 16)+_T(".txt"), File::Access_Write_Append); \
        Debug.clear(); \
        Debug+=Ztring::ToZtring((size_t)this, 16); \
        Debug.resize(17, ' '); \
        _TOAPPEND; \
        Debug+=_T("\r\n"); \
        F.Write(Debug); \
        F.Close();
#else // MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG(_TOAPPEND)
#endif // MEDIAINFO_DEBUG

#ifdef MEDIAINFO_DEBUG_WANTED
    #define MEDIAINFO_DEBUG_STATIC(_TOAPPEND) \
        F.Open(Ztring(MediaInfo_Debug_Name)+_T(".static.txt"), File::Access_Write_Append); \
        Debug.clear(); \
        _TOAPPEND; \
        Debug+=_T("\r\n"); \
        F.Write(Debug); \
        F.Close();
#else // MEDIAINFO_DEBUG_WANTED
    #define MEDIAINFO_DEBUG_STATIC(_TOAPPEND)
#endif // MEDIAINFO_DEBUG_WANTED

#ifdef MEDIAINFO_DEBUG_WANTED
#define EXECUTE_VOID(_METHOD,_DEBUGB) \
        ((MediaInfo_Internal*)Internal)->_METHOD; \
        MEDIAINFO_DEBUG(_DEBUGB)
#else //MEDIAINFO_DEBUG_WANTED
#define EXECUTE_VOID(_METHOD,_DEBUGB) \
        ((MediaInfo_Internal*)Internal)->_METHOD;
#endif //MEDIAINFO_DEBUG_WANTED

#ifdef MEDIAINFO_DEBUG_WANTED
#define EXECUTE_INT(_METHOD,_DEBUGB) \
        int64u ToReturn=((MediaInfo_Internal*)Internal)->_METHOD; \
        MEDIAINFO_DEBUG(_DEBUGB) \
        return ToReturn;
#else //MEDIAINFO_DEBUG_WANTED
#define EXECUTE_INT(_METHOD, _DEBUGB) \
        return ((MediaInfo_Internal*)Internal)->_METHOD;
#endif //MEDIAINFO_DEBUG_WANTED

#ifdef MEDIAINFO_DEBUG_WANTED
#define EXECUTE_STRING(_METHOD,_DEBUGB) \
        Ztring ToReturn=((MediaInfo_Internal*)Internal)->_METHOD; \
        MEDIAINFO_DEBUG(_DEBUGB) \
        return ToReturn;
#else //MEDIAINFO_DEBUG_WANTED
#define EXECUTE_STRING(_METHOD,_DEBUGB) \
        return ((MediaInfo_Internal*)Internal)->_METHOD;
#endif //MEDIAINFO_DEBUG_WANTED

#ifdef MEDIAINFO_DEBUG_BUFFER_SAVE
    #include <stdio.h>
    #ifdef WINDOWS
        FILE* Buffer_Stream=fopen("MediaInfo_Debug_Stream.raw", "a+b");
        FILE* Buffer_Sizes=fopen("MediaInfo_Debug_Stream.sizes", "a+b");
    #else
        FILE* Buffer_Stream=fopen("/tmp/MediaInfo_Debug_Stream.raw", "a+b");
        FILE* Buffer_Sizes=fopen("/tmp/MediaInfo_Debug_Stream.sizes", "a+b");
    #endif

    #undef MEDIAINFO_DEBUG_BUFFER_SAVE
    #define MEDIAINFO_DEBUG_BUFFER_SAVE(_BUFFER, _SIZE) \
        fwrite(_BUFFER, _SIZE, 1, Buffer_Stream); \
        fwrite((char*)&_SIZE, sizeof(size_t), 1, Buffer_Sizes);
#else // MEDIAINFO_DEBUG_BUFFER_SAVE
    #define MEDIAINFO_DEBUG_BUFFER_SAVE(_BUFFER, _SIZE)
#endif // MEDIAINFO_DEBUG_BUFFER_SAVE

}
using namespace MediaInfo_Debug_MediaInfo;

//***************************************************************************
// Constructor/destructor
//***************************************************************************

//---------------------------------------------------------------------------
MediaInfo::MediaInfo()
{
    MEDIAINFO_DEBUG(Debug+=_T("Construction");)
    Internal=new MediaInfo_Internal();
}

//---------------------------------------------------------------------------
MediaInfo::~MediaInfo()
{
    MEDIAINFO_DEBUG(Debug+=_T("Destruction");)
    delete (MediaInfo_Internal*)Internal; //Internal=NULL;
}

//***************************************************************************
// Files
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Open(const String &File_Name_)
{
    MEDIAINFO_DEBUG(Debug+=_T("Open, File=");Debug+=Ztring(File_Name_).c_str();)
    return ((MediaInfo_Internal*)Internal)->Open(File_Name_);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open (const int8u* Begin_, size_t Begin_Size_, const int8u*, size_t, int64u)
{
    return ((MediaInfo_Internal*)Internal)->Open(Begin_, Begin_Size_);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Init (int64u File_Size, int64u File_Offset)
{
    MEDIAINFO_DEBUG(Debug+=_T("Open_Buffer_Init, File_Size=");Debug+=Ztring::ToZtring(File_Size);Debug+=_T(", File_Offset=");Debug+=Ztring::ToZtring(File_Offset);)
    EXECUTE_INT(Open_Buffer_Init(File_Size, File_Offset), Debug+=_T("Open_Buffer, will return ");Debug+=Ztring::ToZtring(ToReturn);)
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Continue (const int8u* ToAdd, size_t ToAdd_Size)
{
    MEDIAINFO_DEBUG_BUFFER_SAVE(ToAdd, ToAdd_Size);

    size_t ToReturn=((MediaInfo_Internal*)Internal)->Open_Buffer_Continue(ToAdd, ToAdd_Size).to_ulong();
    if (ToReturn==0)
    {
        MEDIAINFO_DEBUG(Debug+=_T("Open_Buffer_Continue, will return ");Debug+=Ztring::ToZtring(ToReturn);Debug+=_T(", forcing a Get() :");)
        Get(Stream_General, 0, _T("Format"));        
    }
    return ToReturn;
}

//---------------------------------------------------------------------------
int64u MediaInfo::Open_Buffer_Continue_GoTo_Get ()
{
    return ((MediaInfo_Internal*)Internal)->Open_Buffer_Continue_GoTo_Get();
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Finalize ()
{
    MEDIAINFO_DEBUG(Debug+=_T("Open_Buffer_Finalize");)
    EXECUTE_INT(Open_Buffer_Finalize(), Debug+=_T("Open_Buffer_Finalize, will return ");Debug+=Ztring::ToZtring(ToReturn);)
}

//---------------------------------------------------------------------------
size_t MediaInfo::Save()
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
void MediaInfo::Close()
{
    MEDIAINFO_DEBUG(Debug+=_T("Close");)
    return ((MediaInfo_Internal*)Internal)->Close();
}

//***************************************************************************
// Get File info
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfo::Inform(size_t)
{
    MEDIAINFO_DEBUG(Debug+=_T("Inform");)
    EXECUTE_STRING(Inform(), Debug+=_T("Inform, will return ");Debug+=ToReturn;)
}

//---------------------------------------------------------------------------
String MediaInfo::Get(stream_t StreamKind, size_t StreamPos, size_t Parameter, info_t KindOfInfo)
{
    MEDIAINFO_DEBUG(Debug+=_T("Get, StreamKind=");Debug+=Ztring::ToZtring((size_t)StreamKind);Debug+=_T(", StreamKind=");Debug+=Ztring::ToZtring(StreamPos);Debug+=_T(", Parameter=");Debug+=Ztring::ToZtring(Parameter);)
    EXECUTE_STRING(Get(StreamKind, StreamPos, Parameter, KindOfInfo), Debug+=_T("Get, will return ");Debug+=ToReturn;)
}

//---------------------------------------------------------------------------
String MediaInfo::Get(stream_t StreamKind, size_t StreamPos, const String &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    MEDIAINFO_DEBUG(Debug+=_T("Get, StreamKind=");Debug+=Ztring::ToZtring((size_t)StreamKind);Debug+=_T(", StreamKind=");Debug+=Ztring::ToZtring(StreamPos);Debug+=_T(", Parameter=");Debug+=Ztring(Parameter);)

    EXECUTE_STRING(Get(StreamKind, StreamPos, Parameter, KindOfInfo, KindOfSearch), Debug+=_T("Get, will return ");Debug+=ToReturn;)
}

//***************************************************************************
// Set File info
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Set(const String &, stream_t, size_t, size_t, const String &)
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
size_t MediaInfo::Set(const String &, stream_t, size_t, const String &, const String &)
{
    return 0; //Not yet implemented
}

//***************************************************************************
// Output buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Output_Buffer_Get (const String &Value)
{
    return ((MediaInfo_Internal*)Internal)->Output_Buffer_Get(Value);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Output_Buffer_Get (size_t Pos)
{
    return ((MediaInfo_Internal*)Internal)->Output_Buffer_Get(Pos);
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfo::Option (const String &Option, const String &Value)
{
    MEDIAINFO_DEBUG(Debug+=_T("Option, Option=");Debug+=Ztring(Option);Debug+=_T(", Value=");Debug+=Ztring(Value);)
    EXECUTE_STRING(Option(Option, Value), Debug+=_T("Option, will return ");Debug+=ToReturn;)
}

//---------------------------------------------------------------------------
String MediaInfo::Option_Static (const String &Option, const String &Value)
{
    MEDIAINFO_DEBUG_STATIC(Debug+=_T("Option_Static, Option=");Debug+=Ztring(Option);Debug+=_T(", Value=");Debug+=Ztring(Value);)
    MediaInfoLib::Config.Init(); //Initialize Configuration

         if (Option==_T("Info_Capacities"))
    {
        return _T("Option disactivated for this version, will come back soon!");
        //MediaInfo_Internal MI;
        //return MI.Option(Option);
    }
    else if (Option==_T("Info_Version"))
    {
        Ztring ToReturn=MediaInfoLib::Config.Info_Version_Get();
        if (MediaInfo_Internal::LibraryIsModified())
            ToReturn+=_T(" modified");
        return ToReturn;
    }
    else
        return MediaInfoLib::Config.Option(Option, Value);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Count_Get (stream_t StreamKind, size_t StreamPos)
{
    return ((MediaInfo_Internal*)Internal)->Count_Get(StreamKind, StreamPos);

}

//---------------------------------------------------------------------------
size_t MediaInfo::State_Get ()
{
    return ((MediaInfo_Internal*)Internal)->State_Get();
}

} //NameSpace


