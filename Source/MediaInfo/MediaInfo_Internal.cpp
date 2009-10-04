// MediaInfo_Internal - All info about media files
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
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Internal.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/File__MultipleParsing.h"
#include "ZenLib/Dir.h"
#include "ZenLib/File.h"
#if defined(MEDIAINFO_DIRECTORY_YES)
    #include "MediaInfo/Reader/Reader_Directory.h"
#endif
#if defined(MEDIAINFO_FILE_YES)
    #include "MediaInfo/Reader/Reader_File.h"
#endif
#if defined(MEDIAINFO_LIBCURL_YES)
    #include "MediaInfo/Reader/Reader_libcurl.h"
#endif
#if defined(MEDIAINFO_LIBMMS_YES)
    #include "MediaInfo/Reader/Reader_libmms.h"
#endif
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/destructor
//***************************************************************************

//---------------------------------------------------------------------------
MediaInfo_Internal::MediaInfo_Internal()
{
    MediaInfoLib::Config.Init(); //Initialize Configuration

    CriticalSectionLocker CSL(CS);
    Thread=NULL;
    BlockMethod=BlockMethod_Local;
    Info=NULL;
    Info_IsMultipleParsing=false;

    Stream.resize(Stream_Max);
    Stream_More.resize(Stream_Max);
}

//---------------------------------------------------------------------------
MediaInfo_Internal::~MediaInfo_Internal()
{
    Close();

    CriticalSectionLocker CSL(CS);;
    delete Info; //Info=NULL;
}

//***************************************************************************
// Files
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open(const String &File_Name)
{
    #if defined(MEDIAINFO_LIBCURL_YES)
        if ((File_Name.size()>=7
          && File_Name[0]==_T('h')
          && File_Name[1]==_T('t')
          && File_Name[2]==_T('t')
          && File_Name[3]==_T('p')
          && File_Name[4]==_T(':')
          && File_Name[5]==_T('/')
          && File_Name[6]==_T('/'))
         || (File_Name.size()>=6
          && File_Name[0]==_T('f')
          && File_Name[1]==_T('t')
          && File_Name[2]==_T('p')
          && File_Name.find(_T("://"))!=std::string::npos)))
        {
            //Load libcurl
            if (Reader_libcurl::Format_Test(this, File_Name)>0)
                 return 1;
            return 0;
        }
    #endif //MEDIAINFO_LIBCURL_YES

    #if defined(MEDIAINFO_LIBCURL_YES)
        if ((File_Name.size()>=7
          && File_Name[0]==_T('m')
          && File_Name[1]==_T('m')
          && File_Name[2]==_T('s')
          && File_Name[3]==_T('h')
          && File_Name[4]==_T(':')
          && File_Name[5]==_T('/')
          && File_Name[6]==_T('/'))
         || (File_Name.size()>=6
          && File_Name[0]==_T('m')
          && File_Name[1]==_T('m')
          && File_Name[2]==_T('s')
          && File_Name[4]==_T(':')
          && File_Name[5]==_T('/')
          && File_Name[6]==_T('/')))
        {
            //Load libmms
            if (Reader_libmms::Format_Test(this, File_Name)>0)
                 return 1;
            return 0;
        }
    #endif //MEDIAINFO_LIBCURL_YES

    #if defined(MEDIAINFO_DIRECTORY_YES)
        if (Dir::Exists(File_Name))
        {
            if (Reader_Directory::Format_Test(this, File_Name)>0)
                 return 1;
            return 0;
        }
    #endif //MEDIAINFO_DIRECTORY_YES

    #if defined(MEDIAINFO_FILE_YES)
        if (File::Exists(File_Name))
        {
            if (Reader_Directory::Format_Test(this, File_Name)>0)
                 return 1;
            return 0;
        }
    #endif //MEDIAINFO_FILE_YES

    if (SelectFromExtension(_T("Unknown")))
    {
        Open_Buffer_Init((int64u)-1, File_Name);
        Open_Buffer_Finalize();
        return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open (const int8u* Begin, size_t Begin_Size, const int8u*, size_t, int64u File_Size)
{
    Open_Buffer_Init(File_Size);
    Open_Buffer_Continue(Begin, Begin_Size);
    Open_Buffer_Finalize();

    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open_Buffer_Init (int64u File_Size_, const String &File_Name)
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
    {
        if (!Config.File_ForceParser_Get().empty())
            SelectFromExtension(Config.File_ForceParser_Get());
        else
        {
            Info=new File__MultipleParsing;
            Info_IsMultipleParsing=true;
        }
    }
    #ifndef MEDIAINFO_MINIMIZESIZE
        Info->Init(&Config, &Details, &Stream, &Stream_More);
    #else //MEDIAINFO_MINIMIZESIZE
        Info->Init(&Config, &Stream, &Stream_More);
    #endif //MEDIAINFO_MINIMIZESIZE
    if (!File_Name.empty())
        Info->File_Name=File_Name;
    Info->Open_Buffer_Init(File_Size_);

    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open_Buffer_Init (int64u File_Size_, int64u File_Offset_)
{
    Open_Buffer_Init(File_Size_);

    CriticalSectionLocker CSL(CS);

    Info->Open_Buffer_Position_Set(File_Offset_);

    return 1;
}

//---------------------------------------------------------------------------
std::bitset<32> MediaInfo_Internal::Open_Buffer_Continue (const int8u* ToAdd, size_t ToAdd_Size)
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
        return 0;

    Info->Open_Buffer_Continue(ToAdd, ToAdd_Size);

    if (Info_IsMultipleParsing && Info->Status[File__Analyze::IsAccepted])
    {
        //Found
        File__Analyze* Info_ToDelete=Info;
        Info=((File__MultipleParsing*)Info)->Parser_Get();
        delete Info_ToDelete; //Info_ToDelete=NULL;
        Info_IsMultipleParsing=false;
    }

    #if 0 //temp, for old users
    //The parser wanted seek but the buffer is not seekable
    if (Info->File_GoTo!=(int64u)-1 && Config.File_IsSeekable_Get()==0)
    {
        Info->Open_Buffer_Finalize(true);
        Info->File_GoTo=(int64u)-1;
        return 0;
    }

    return 1;
    #else
    //The parser wanted seek but the buffer is not seekable
    if (Info->File_GoTo!=(int64u)-1 && Config.File_IsSeekable_Get()==0)
    {
        Info->Fill();
        Info->File_GoTo=(int64u)-1;
    }

    if (!Info->Status[File__Analyze::IsFilled] && Info->Status[File__Analyze::IsUpdated])
        Info->Status[File__Analyze::IsUpdated]=false; //No updated info until IsFilled is set

    return Info->Status;
    #endif
}

//---------------------------------------------------------------------------
int64u MediaInfo_Internal::Open_Buffer_Continue_GoTo_Get ()
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
        return 0;

    return Info->File_GoTo;
}

bool MediaInfo_Internal::Open_Buffer_Position_Set(int64u File_Offset)
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
        return false;

    Info->Open_Buffer_Position_Set(File_Offset);
    return true;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open_Buffer_Finalize ()
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
        return 0;

    Info->Open_Buffer_Finalize();

    //Cleanup
    if (!Config.File_IsSub_Get() && !Config.File_KeepInfo_Get()) //We need info for the calling parser
    {
        delete Info; Info=NULL;
    }

    return 1;
}

//---------------------------------------------------------------------------
void MediaInfo_Internal::Close()
{
    CriticalSectionLocker CSL(CS);
    Stream.clear();
    Stream.resize(Stream_Max);
    Stream_More.clear();
    Stream_More.resize(Stream_Max);
    delete Info; Info=NULL;
}

//***************************************************************************
// Get File info
//***************************************************************************

/*//---------------------------------------------------------------------------
String MediaInfo_Internal::Inform(size_t)
{
    //Info case
    if (Info)
        return Info->Inform();

    if (!Info)
        return MediaInfoLib::Config.EmptyString_Get();

    return Info->Inform();
} */

//---------------------------------------------------------------------------
String MediaInfo_Internal::Get(stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t KindOfInfo)
{
    CriticalSectionLocker CSL(CS);

    if (Info)
    {
        Info->Status[File__Analyze::IsUpdated]=false;
        for (size_t Pos=File__Analyze::User_16; Pos<File__Analyze::User_16+16; Pos++)
            Info->Status[Pos]=false;
    }

    //Check integrity
    if (StreamKind>=Stream_Max || StreamNumber>=Stream[StreamKind].size() || Parameter>=MediaInfoLib::Config.Info_Get(StreamKind).size()+Stream_More[StreamKind][StreamNumber].size() || KindOfInfo>=Info_Max)
        return MediaInfoLib::Config.EmptyString_Get(); //Parameter is unknown

    else if (Parameter<MediaInfoLib::Config.Info_Get(StreamKind).size())
    {
        //Optimization : KindOfInfo>Info_Text is in static lists
        if (KindOfInfo!=Info_Text)
            return MediaInfoLib::Config.Info_Get(StreamKind, Parameter, KindOfInfo); //look for static information only
        else if (Parameter<Stream[StreamKind][StreamNumber].size())
            return Stream[StreamKind][StreamNumber][Parameter];
        else
            return MediaInfoLib::Config.EmptyString_Get(); //This parameter is known, but not filled
    }
    else
        return Stream_More[StreamKind][StreamNumber][Parameter-MediaInfoLib::Config.Info_Get(StreamKind).size()](KindOfInfo);
}

//---------------------------------------------------------------------------
String MediaInfo_Internal::Get(stream_t StreamKind, size_t StreamPos, const String &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    //Legacy
    if (Parameter.find(_T("_String"))!=Error)
    {
        Ztring S1=Parameter;
        S1.FindAndReplace(_T("_String"), _T("/String"));
        return Get(StreamKind, StreamPos, S1, KindOfInfo, KindOfSearch);
    }
    if (Parameter==_T("Channels"))
        return Get(StreamKind, StreamPos, _T("Channel(s)"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("Artist"))
        return Get(StreamKind, StreamPos, _T("Performer"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("AspectRatio"))
        return Get(StreamKind, StreamPos, _T("DisplayAspectRatio"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("AspectRatio/String"))
        return Get(StreamKind, StreamPos, _T("DisplayAspectRatio/String"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("Chroma"))
        return Get(StreamKind, StreamPos, _T("Colorimetry"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("PlayTime"))
        return Get(StreamKind, StreamPos, _T("Duration"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("PlayTime/String"))
        return Get(StreamKind, StreamPos, _T("Duration/String"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("PlayTime/String1"))
        return Get(StreamKind, StreamPos, _T("Duration/String1"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("PlayTime/String2"))
        return Get(StreamKind, StreamPos, _T("Duration/String2"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("PlayTime/String3"))
        return Get(StreamKind, StreamPos, _T("Duration/String3"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate/String"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate/String"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate_Minimum"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate_Minimum"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate_Minimum/String"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate_Minimum/String"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate_Nominal"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate_Nominal"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate_Nominal/String"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate_Nominal/String"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate_Maximum"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate_Maximum"), KindOfInfo, KindOfSearch);
    if (StreamKind==Stream_General && Parameter==_T("BitRate_Maximum/String"))
        return Get(Stream_General, StreamPos, _T("OverallBitRate_Maximum/String"), KindOfInfo, KindOfSearch);

    CS.Enter();

    if (Info)
    {
        Info->Status[File__Analyze::IsUpdated]=false;
        for (size_t Pos=File__Analyze::User_16; Pos<File__Analyze::User_16+16; Pos++)
            Info->Status[Pos]=false;
    }

    //Check integrity
    if (StreamKind>=Stream_Max || StreamPos>=Stream[StreamKind].size() || KindOfInfo>=Info_Max)
    {
        CS.Leave();
        return MediaInfoLib::Config.EmptyString_Get(); //Parameter is unknown
    }

    //Special cases
    //-Inform for a stream
    if (Parameter==_T("Inform"))
    {
        CS.Leave();
        Ztring InformZtring=Inform(StreamKind, StreamPos);
        CS.Enter();
        size_t Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(_T("Inform"));
        if (Pos!=Error)
            Stream[StreamKind][StreamPos](Pos)=InformZtring;
    }

    //Case of specific info
    size_t ParameterI=MediaInfoLib::Config.Info_Get(StreamKind).Find(Parameter, KindOfSearch);
    if (ParameterI==Error)
    {
        ParameterI=Stream_More[StreamKind][StreamPos].Find(Parameter, KindOfSearch);
        if (ParameterI==Error)
        {
            CS.Leave();
            return MediaInfoLib::Config.EmptyString_Get(); //Parameter is unknown
        }
        CS.Leave();
        CriticalSectionLocker CSL(CS);
        return Stream_More[StreamKind][StreamPos][ParameterI](KindOfInfo);
    }

    CS.Leave();

    return Get(StreamKind, StreamPos, ParameterI, KindOfInfo);
}

//***************************************************************************
// Set File info
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Set(const String &ToSet, stream_t StreamKind, size_t StreamNumber, size_t Parameter, const String &OldValue)
{
    CriticalSectionLocker CSL(CS);
    if (!Info)
        return 0;

    return Info->Set(StreamKind, StreamNumber, Parameter, ToSet, OldValue);
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Set(const String &ToSet, stream_t StreamKind, size_t StreamNumber, const String &Parameter, const String &OldValue)
{
    CriticalSectionLocker CSL(CS);
    if (!Info)
        return 0;

    return Info->Set(StreamKind, StreamNumber, Parameter, ToSet, OldValue);
}

//***************************************************************************
// Output buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Output_Buffer_Get (const String &Value)
{
    CriticalSectionLocker CSL(CS);
    if (!Info)
        return 0;

    return Info->Output_Buffer_Get(Value);
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Output_Buffer_Get (size_t Pos)
{
    CriticalSectionLocker CSL(CS);
    if (!Info)
        return 0;

    return Info->Output_Buffer_Get(Pos);
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
String MediaInfo_Internal::Option (const String &Option, const String &Value)
{
    CriticalSectionLocker CSL(CS);
    Ztring OptionLower=Option; OptionLower.MakeLowerCase();
         if (Option.empty())
        return _T("");
    else if (OptionLower==_T("language_update"))
    {
        if (!Info || Info->Get(Stream_General, 0, _T("CompleteName"))==_T(""))
            return _T("");

        ZtringListList Language=Value.c_str();
        MediaInfoLib::Config.Language_Set(Language);

        return _T("");
    }
    else if (OptionLower==_T("create_dummy"))
    {
        CreateDummy (Value);
        delete Info; Info=NULL;
        return _T("");
    }
    else if (Option==_T("info_capacities"))
    {
        return _T("Option removed");
    }
    else if (OptionLower.find(_T("file_"))==0)
    {
        Ztring ToReturn=Config.Option(Option, Value);
        if (Info)
            Info->Option_Manage();
        return ToReturn;
    }
    else
        return MediaInfoLib::Config.Option(Option, Value);
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Count_Get (stream_t StreamKind, size_t StreamPos)
{
    CriticalSectionLocker CSL(CS);
    //Integrity
    if (StreamKind>=Stream_Max)
        return 0;

    //Count of streams
    if (StreamPos==Error)
        return Stream[StreamKind].size();

    //Integrity
    if (StreamPos>=Stream[StreamKind].size())
        return 0;

    //Count of piece of information in a stream
    return MediaInfoLib::Config.Info_Get(StreamKind).size()+Stream_More[StreamKind][StreamPos].size();
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::State_Get ()
{
    CriticalSectionLocker CSL(CS);
    return 0; //Not yet implemented
}

} //NameSpace


