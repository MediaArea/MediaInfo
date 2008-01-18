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
#include "MediaInfo/MediaInfo_Config.h"
#include <ZenLib/ZtringListList.h>
#include <ZenLib/Dir.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Gestion de la classe
//***************************************************************************

//---------------------------------------------------------------------------
//Constructeurs
MediaInfoList::MediaInfoList(size_t Count_Init)
{
    //Initialisation
    Info.reserve(Count_Init);
    for (size_t Pos=0; Pos<Info.size(); Pos++)
        Info[Pos]=NULL;
}

//---------------------------------------------------------------------------
//Destructeur
MediaInfoList::~MediaInfoList()
{
    Close();

    for (size_t Pos=0; Pos<Info.size(); Pos++)
        delete Info[Pos]; //Info[Pos]=NULL;
}

//***************************************************************************
// Fichiers
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfoList::Open(const String &File, const fileoptions_t Options)
{
    size_t CountValid=0; //Count of valid files

    //Option FileOption_Close
    if (Options & FileOption_CloseAll)
        Close(All);

    //Option Recursive
    //TODO

    //Get all filenames
    ZtringList List=Dir::GetAllFileNames(File);

    //Init each file
    size_t Size_Before=Info.size();
    Info.resize(Size_Before+List.size());
    for (size_t Pos=0; Pos<List.size(); Pos++)
    {
        Info[Size_Before+Pos]=new MediaInfo();
        Info[Size_Before+Pos]->Open(List[Pos]);
        CountValid++;
    }

    return CountValid;
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Open (const int8u* Begin, size_t Begin_Size, const int8u* End, size_t End_Size, int64u FileSize)
{
    Info.resize(Info.size()+1);
    Info[Info.size()-1]=new MediaInfo();
    Info[Info.size()-1]->Open(Begin, Begin_Size, End, End_Size, FileSize);
    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Save(size_t)
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
void MediaInfoList::Close(size_t FilePos)
{
    if (FilePos==Unlimited)
    {
        for (size_t Pos=0; Pos<Info.size(); Pos++)
        {
            delete Info[Pos]; Info[Pos]=NULL;
        }
        Info.clear();
    }
    else if (FilePos<Info.size())
    {
        delete Info[FilePos]; Info[FilePos]=NULL;
        Info.erase(Info.begin()+FilePos);
    }
}

//***************************************************************************
// Get File info
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfoList::Inform(size_t FilePos, size_t)
{
    if (FilePos==Error)
    {
        Ztring Retour;
        unsigned int FilePos=0;
        ZtringListList MediaInfo_Custom_View; MediaInfo_Custom_View.Write(Option(_T("Inform_Get")));
        Retour+=MediaInfo_Custom_View(Stream_Max+2, 1);//Page_Begin
        while (FilePos<Info.size())
        {
            Retour+=Inform(FilePos);
            if (FilePos<Info.size()-1)
                Retour+=MediaInfo_Custom_View(Stream_Max+3, 1);//Page_Middle
            FilePos++;
        }
        Retour+=MediaInfo_Custom_View(Stream_Max+4, 1);//Page_End
        //Retour.FindAndReplace(_T("\\n"),_T( "\n"), 0, Ztring_Recursive);
        return Retour.c_str();
    }

    if (FilePos>=Info.size() || Info[FilePos]==NULL || Info[FilePos]->Count_Get(Stream_General)==0)
        return Config.EmptyString_Get();

    return Info[FilePos]->Inform();
}

//---------------------------------------------------------------------------
String MediaInfoList::Get(size_t FilePos, stream_t KindOfStream, size_t StreamNumber, size_t Parameter, info_t KindOfInfo)
{
    if (FilePos==Error || FilePos>=Info.size() || Info[FilePos]==NULL || Info[FilePos]->Count_Get(Stream_General)==0)
        return Config.EmptyString_Get();

    return Info[FilePos]->Get(KindOfStream, StreamNumber, Parameter, KindOfInfo);
}

//---------------------------------------------------------------------------
String MediaInfoList::Get(size_t FilePos, stream_t KindOfStream, size_t StreamNumber, const String &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    //TRACE(Trace+=_T("Get(L), CompleteName=");Trace+=Info[FilePos].Get(Stream_General, 0, _T("CompleteName")).c_str();)
    //TRACE(Trace+=_T("Get(L), StreamKind=");Trace+=ZenLib::Ztring::ToZtring((int8u)KindOfStream);Trace+=_T(", StreamNumber=");Trace+=ZenLib::Ztring::ToZtring((int8u)StreamNumber);Trace+=_T(", Parameter=");Trace+=ZenLib::Ztring(Parameter);Trace+=_T(", KindOfInfo=");Trace+=ZenLib::Ztring::ToZtring((int8u)KindOfInfo);Trace+=_T(", KindOfSearch=");Trace+=ZenLib::Ztring::ToZtring((int8u)KindOfSearch);)
    //TRACE(Trace+=_T("Get(L), will return ");Trace+=Info[FilePos].Get(KindOfStream, StreamNumber, Parameter, KindOfInfo, KindOfSearch).c_str();)

    if (FilePos==Error || FilePos>=Info.size() || Info[FilePos]==NULL || Info[FilePos]->Count_Get(Stream_General)==0)
        return Config.EmptyString_Get();

    return Info[FilePos]->Get(KindOfStream, StreamNumber, Parameter, KindOfInfo, KindOfSearch);
}

//***************************************************************************
// Set File info
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfoList::Set(const String &ToSet, size_t FilePos, stream_t StreamKind, size_t StreamNumber, size_t Parameter, const String &OldValue)
{
    if (FilePos==(size_t)-1)
        FilePos=0; //TODO : average

    if (FilePos>=Info.size() || Info[FilePos]==NULL || Info[FilePos]->Count_Get(Stream_General)==0)
        return 0;

    return Info[FilePos]->Set(ToSet, StreamKind, StreamNumber, Parameter, OldValue);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Set(const String &ToSet, size_t FilePos, stream_t StreamKind, size_t StreamNumber, const String &Parameter, const String &OldValue)
{
    if (FilePos==(size_t)-1)
        FilePos=0; //TODO : average

    if (FilePos>=Info.size() || Info[FilePos]==NULL || Info[FilePos]->Count_Get(Stream_General)==0)
        return 0;

    return Info[FilePos]->Set(ToSet, StreamKind, StreamNumber, Parameter, OldValue);
}

//***************************************************************************
// Output buffer
//***************************************************************************

/*
//---------------------------------------------------------------------------
char* MediaInfoList::Output_Buffer_Get (size_t FilePos, size_t &Output_Buffer_Size)
{
    if (FilePos==(size_t)-1)
        FilePos=0; //TODO : average

    if (FilePos>=Info.size() || Info[FilePos]==NULL || Info[FilePos]->Count_Get(Stream_General)==0)
        return 0;

    return Info[FilePos]->Output_Buffer_Get(Output_Buffer_Size);
}
*/

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfoList::Option (const String &Option, const String &Value)
{
         if (Option==_T(""))
        return _T("");
    else if (Option==_T("Language_Update"))
    {
        //Special case : Language_Update must update all MediaInfo classes
        for (unsigned int Pos=0; Pos<Info.size(); Pos++)
            if (Info[Pos])
                Info[Pos]->Option(_T("Language_Update"), Value);

        return _T("");
    }
    else if (Option==_T("Create_Dummy"))
    {
        Info.resize(Info.size()+1);
        Info[Info.size()-1]=new MediaInfo();
        Info[Info.size()-1]->Option(Option, Value);
        return _T("");
    }
    else
        return MediaInfo::Option_Static(Option, Value);

}

//---------------------------------------------------------------------------
String MediaInfoList::Option_Static (const String &Option, const String &Value)
{
    return MediaInfo::Option_Static(Option, Value);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::State_Get()
{
    return 0; //Not yet implemented
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Count_Get (size_t FilePos, stream_t StreamKind, size_t StreamNumber)
{
    if (FilePos>=Info.size() || Info[FilePos]==NULL)
        return 0;

    return Info[FilePos]->Count_Get(StreamKind, StreamNumber);
}

//---------------------------------------------------------------------------
size_t MediaInfoList::Count_Get()
{
    return Info.size();
}

} //NameSpace

