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
    #include <stdio.h>
    FILE* F;
    std::string Debug;
    #ifdef WINDOWS
        const char* MediaInfo_Debug_Name="MediaInfo_Debug.txt";
    #else
        const char* MediaInfo_Debug_Name="/tmp/MediaInfo_Debug.txt";
    #endif

    #undef MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG(_TOAPPEND) \
        F=fopen(MediaInfo_Debug_Name, "a+t"); \
        Debug.clear(); \
        Debug+=ToString((size_t)this); \
        Debug.resize(11, ' '); \
        _TOAPPEND; \
        Debug+="\r\n"; \
        fwrite(Debug.c_str(), Debug.size(), 1, F); \
        fclose(F);
#else // MEDIAINFO_DEBUG
    #define MEDIAINFO_DEBUG(_TOAPPEND)
#endif // MEDIAINFO_DEBUG

#ifdef MEDIAINFO_DEBUG_WANTED
    #define MEDIAINFO_DEBUG_STATIC(_TOAPPEND) \
        F=fopen(MediaInfo_Debug_Name, "a+t"); \
        Debug.clear(); \
        Debug.resize(11, ' '); \
        _TOAPPEND; \
        Debug+="\r\n"; \
        fwrite(Debug.c_str(), Debug.size(), 1, F); \
        fclose(F);
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

inline std::string ToString(int64u Integer)
{
    return Ztring::ToZtring(Integer).To_Local();
}

//***************************************************************************
// Constructor/destructor
//***************************************************************************

//---------------------------------------------------------------------------
MediaInfo::MediaInfo()
{
    MEDIAINFO_DEBUG(Debug+="Construction";)
    Internal=new MediaInfo_Internal();
}

//---------------------------------------------------------------------------
MediaInfo::~MediaInfo()
{
    MEDIAINFO_DEBUG(Debug+="Destruction";)
    delete (MediaInfo_Internal*)Internal; //Internal=NULL;
}

//***************************************************************************
// Files
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Open(const String &File_Name_)
{
    MEDIAINFO_DEBUG(Debug+="Open, File=";Debug+=Ztring(File_Name_).To_Local().c_str();)
    return ((MediaInfo_Internal*)Internal)->Open(File_Name_);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open (const int8u* Begin_, size_t Begin_Size_, const int8u*, size_t, int64u FileSize_)
{
    return ((MediaInfo_Internal*)Internal)->Open(Begin_, Begin_Size_);
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Init (int64u File_Size, int64u File_Offset)
{
    MEDIAINFO_DEBUG(Debug+="Open_Buffer_Init, File_Size=";Debug+=ToString(File_Size);Debug+=", File_Offset=";Debug+=ToString(File_Offset);)
    EXECUTE_INT(Open_Buffer_Init(File_Size, File_Offset), Debug+="Open_Buffer, will return ";Debug+=ToString(ToReturn);)
}

//---------------------------------------------------------------------------
size_t MediaInfo::Open_Buffer_Continue (const int8u* ToAdd, size_t ToAdd_Size)
{
    MEDIAINFO_DEBUG_BUFFER_SAVE(ToAdd, ToAdd_Size);

    size_t ToReturn=((MediaInfo_Internal*)Internal)->Open_Buffer_Continue(ToAdd, ToAdd_Size);
    if (ToReturn==0)
    {
        MEDIAINFO_DEBUG(Debug+="Open_Buffer_Continue, will return ";Debug+=ToString(ToReturn);Debug+=", forcing a Get() :";)
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
    MEDIAINFO_DEBUG(Debug+="Open_Buffer_Finalize";)
    EXECUTE_INT(Open_Buffer_Finalize(), Debug+="Open_Buffer_Finalize, will return ";Debug+=ToString(ToReturn);)
}

//---------------------------------------------------------------------------
size_t MediaInfo::Save()
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
void MediaInfo::Close()
{
    MEDIAINFO_DEBUG(Debug+="Close";)
    return ((MediaInfo_Internal*)Internal)->Close();
}

//***************************************************************************
// Get File info
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfo::Inform(size_t)
{
    MEDIAINFO_DEBUG(Debug+="Inform";)
    EXECUTE_STRING(Inform(), Debug+="Inform, will return ";Debug+=ToReturn.To_Local();)
}

//---------------------------------------------------------------------------
String MediaInfo::Get(stream_t StreamKind, size_t StreamPos, size_t Parameter, info_t KindOfInfo)
{
    MEDIAINFO_DEBUG(Debug+="Get, StreamKind=";Debug+=ToString((size_t)StreamKind);Debug+=", StreamKind=";Debug+=ToString(StreamPos);Debug+=", Parameter=";Debug+=ToString(Parameter);)
    EXECUTE_STRING(Get(StreamKind, StreamPos, Parameter, KindOfInfo), Debug+="Get, will return ";Debug+=ToReturn.To_Local();)
}

//---------------------------------------------------------------------------
String MediaInfo::Get(stream_t StreamKind, size_t StreamPos, const String &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    MEDIAINFO_DEBUG(Debug+="Get, StreamKind=";Debug+=ToString((size_t)StreamKind);Debug+=", StreamKind=";Debug+=ToString(StreamPos);Debug+=", Parameter=";Debug+=Ztring(Parameter).To_Local();)

    EXECUTE_STRING(Get(StreamKind, StreamPos, Parameter, KindOfInfo), Debug+="Get, will return ";Debug+=ToReturn.To_Local();)
}

//***************************************************************************
// Set File info
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo::Set(const String &ToSet, stream_t StreamKind, size_t StreamNumber, size_t Parameter, const String &OldValue)
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
size_t MediaInfo::Set(const String &ToSet, stream_t StreamKind, size_t StreamNumber, const String &Parameter, const String &OldValue)
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
    MEDIAINFO_DEBUG(Debug+="Option, Option=";Debug+=Ztring(Option).To_Local();Debug+=", Value=";Debug+=Ztring(Value).To_Local();)
    EXECUTE_STRING(Option(Option, Value), Debug+="Option, will return ";Debug+=ToReturn.To_Local();)
}

//---------------------------------------------------------------------------
String MediaInfo::Option_Static (const String &Option, const String &Value)
{
    MEDIAINFO_DEBUG_STATIC(Debug+="Option_Static, Option=";Debug+=Ztring(Option).To_Local();Debug+=", Value=";Debug+=Ztring(Value).To_Local();)
    MediaInfoLib::Config.Init(); //Initialize Configuration

         if (Option==_T("Info_Capacities"))
    {
        return _T("Option disactivated for this version, will come back soon!");
        MediaInfo_Internal MI;
        return MI.Option(Option);
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
    return 0; //Not yet implemented
}

} //NameSpace


