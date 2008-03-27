// MediaInfoList - A list of MediaInfo
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
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
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(__BORLANDC__) && defined (_DEBUG)
    //Why? in Debug mode with release Wx Libs, wxAssert is not defined?
    void wxAssert (int, const wchar_t*, int, const wchar_t*, const wchar_t*){return;}
    void wxAssert (int, const char*, int, const char*, const char*){return;}
#endif
#include "MediaInfoList.h"
#include "MediaInfoList_Internal.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Gestion de la classe
//***************************************************************************

//---------------------------------------------------------------------------
//Constructeurs
MediaInfoList::MediaInfoList(size_t Count_Init)
{
    Internal=new MediaInfoList_Internal(Count_Init);
}

//---------------------------------------------------------------------------
//Destructeur
MediaInfoList::~MediaInfoList()
{
    delete (MediaInfoList_Internal*)Internal; //MI=NULL;
}

//***************************************************************************
// Files
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfoList::Open(const String &File, const fileoptions_t Options)
{
    return ((MediaInfoList_Internal*)Internal)->Open(File, Options);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Open_Buffer_Init (int64u File_Size_, int64u File_Offset_)
{
    return ((MediaInfoList_Internal*)Internal)->Open_Buffer_Init(File_Size_, File_Offset_);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Open_Buffer_Continue (size_t FilePos, const int8u* ToAdd, size_t ToAdd_Size)
{
    return ((MediaInfoList_Internal*)Internal)->Open_Buffer_Continue(FilePos, ToAdd, ToAdd_Size);
}

//---------------------------------------------------------------------------
int64u MediaInfoList::Open_Buffer_Continue_GoTo_Get (size_t FilePos)
{
    return ((MediaInfoList_Internal*)Internal)->Open_Buffer_Continue_GoTo_Get(FilePos);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Open_Buffer_Finalize (size_t FilePos)
{
    return ((MediaInfoList_Internal*)Internal)->Open_Buffer_Finalize(FilePos);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Save(size_t)
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
void MediaInfoList::Close(size_t FilePos)
{
    ((MediaInfoList_Internal*)Internal)->Close(FilePos);
}

//***************************************************************************
// Get File info
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfoList::Inform(size_t FilePos, size_t)
{
    return ((MediaInfoList_Internal*)Internal)->Inform(FilePos);
}

//---------------------------------------------------------------------------
String MediaInfoList::Get(size_t FilePos, stream_t KindOfStream, size_t StreamNumber, size_t Parameter, info_t KindOfInfo)
{
    return ((MediaInfoList_Internal*)Internal)->Get(FilePos, KindOfStream, StreamNumber, Parameter, KindOfInfo);
}

//---------------------------------------------------------------------------
String MediaInfoList::Get(size_t FilePos, stream_t KindOfStream, size_t StreamNumber, const String &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    //TRACE(Trace+=_T("Get(L), CompleteName=");Trace+=Info[FilePos].Get(Stream_General, 0, _T("CompleteName")).c_str();)
    //TRACE(Trace+=_T("Get(L), StreamKind=");Trace+=ZenLib::Ztring::ToZtring((int8u)KindOfStream);Trace+=_T(", StreamNumber=");Trace+=ZenLib::Ztring::ToZtring((int8u)StreamNumber);Trace+=_T(", Parameter=");Trace+=ZenLib::Ztring(Parameter);Trace+=_T(", KindOfInfo=");Trace+=ZenLib::Ztring::ToZtring((int8u)KindOfInfo);Trace+=_T(", KindOfSearch=");Trace+=ZenLib::Ztring::ToZtring((int8u)KindOfSearch);)
    //TRACE(Trace+=_T("Get(L), will return ");Trace+=Info[FilePos].Get(KindOfStream, StreamNumber, Parameter, KindOfInfo, KindOfSearch).c_str();)

    return ((MediaInfoList_Internal*)Internal)->Get(FilePos, KindOfStream, StreamNumber, Parameter, KindOfInfo, KindOfSearch);
}

//***************************************************************************
// Set File info
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfoList::Set(const String &ToSet, size_t FilePos, stream_t StreamKind, size_t StreamNumber, size_t Parameter, const String &OldValue)
{
    return ((MediaInfoList_Internal*)Internal)->Set(ToSet, FilePos, StreamKind, StreamNumber, Parameter, OldValue);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Set(const String &ToSet, size_t FilePos, stream_t StreamKind, size_t StreamNumber, const String &Parameter, const String &OldValue)
{
    return ((MediaInfoList_Internal*)Internal)->Set(ToSet, FilePos, StreamKind, StreamNumber, Parameter, OldValue);
}

//***************************************************************************
// Output buffer
//***************************************************************************

/*
//---------------------------------------------------------------------------
char* MediaInfoList::Output_Buffer_Get (size_t FilePos, size_t &Output_Buffer_Size)
{
    return ((MediaInfoList_Internal*)Internal)->Output_Buffer_Get(FilePos, Output_Buffer_Size);
}
*/

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfoList::Option (const String &Option, const String &Value)
{
    return ((MediaInfoList_Internal*)Internal)->Option(Option, Value);

}

//---------------------------------------------------------------------------
String MediaInfoList::Option_Static (const String &Option, const String &Value)
{
    return MediaInfo::Option_Static(Option, Value);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::State_Get()
{
    return ((MediaInfoList_Internal*)Internal)->State_Get();
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Count_Get (size_t FilePos, stream_t StreamKind, size_t StreamNumber)
{
    return ((MediaInfoList_Internal*)Internal)->Count_Get(FilePos, StreamKind, StreamNumber);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Count_Get()
{
    return ((MediaInfoList_Internal*)Internal)->Count_Get();
}

} //NameSpace

