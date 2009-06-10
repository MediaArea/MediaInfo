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
#include "MediaInfo/File_Unknown.h"
#include "ZenLib/FileName.h"
#include "ZenLib/File.h"
#include "ZenLib/InfoMap.h"
#include <vector>
#include <cstring>
#if defined(MEDIAINFO_EIA708_YES)
    #include "MediaInfo/Text/File_Eia708.h"
#endif
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

const size_t Buffer_NormalSize=/*188*7;//*/64*1024;

//---------------------------------------------------------------------------
extern MediaInfo_Config         Config;
       ZtringListList           MediaInfo_Internal_Temp;
//       vector<ZtringListList>   MediaInfo_Internal_Capacities;
       String         MediaInfo_Internal_Capacities_Final;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/destructor
//***************************************************************************

//---------------------------------------------------------------------------
MediaInfo_Internal::MediaInfo_Internal()
{
    CriticalSectionLocker CSL(CS);
    Thread=NULL;
    BlockMethod=BlockMethod_Local;
    Info=NULL;
    Buffer=NULL;
    Buffer_Size=0;
    Buffer_Size_Max=0;
    BufferConst=NULL;
    File_Handle=NULL;
    File_Size=0;
    File_Offset=0;
    File_AlreadyBuffered=false;
    MultipleParsing_IsDetected=false;

    MediaInfoLib::Config.Init(); //Initialize Configuration
    Stream.resize(Stream_Max);
    Stream_More.resize(Stream_Max);
}

//---------------------------------------------------------------------------
MediaInfo_Internal::~MediaInfo_Internal()
{
    Close();

    CriticalSectionLocker CSL(CS);;
    delete Info; //Info=NULL;
    delete[] Buffer; //Buffer=NULL;
    delete (File*)File_Handle; //File_Handle=NULL;
}

//***************************************************************************
// Files
//***************************************************************************

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open(const String &File_Name_)
{
    //Test existence of the file
    File_Name=File_Name_;
    if (!File::Exists(File_Name))
        return 0;

    //Get the Extension
    Ztring Extension=FileName::Extension_Get(File_Name);
    Extension.MakeLowerCase();

    //Search the theorical format from extension
    InfoMap &FormatList=MediaInfoLib::Config.Format_Get();
    InfoMap::iterator Format=FormatList.begin();
    while (Format!=FormatList.end())
    {
        const Ztring &Extensions=FormatList.Get(Format->first, InfoFormat_Extensions);
        if (Extensions.find(Extension)!=Error)
        {
            if(Extension.size()==Extensions.size())
                break; //Only one extenion in the list
            if(Extensions.find(Extension+_T(" "))!=Error
            || Extensions.find(_T(" ")+Extension)!=Error)
                break;
        }
        Format++;
    }
    if (Format!=FormatList.end())
    {
        const Ztring &Parser=Format->second(InfoFormat_Parser);
        SelectFromExtension(Parser);
    }
    //Info=new File_Eia708;

    CriticalSectionLocker CSL(CS);
    //Test the theorical format
    if (Format_Test()>0)
         return 1;

    //Extension is not the good one, parse with all formats
    /*
    delete Info; Info=new File__MultipleParsing;
    if (Format_Test()>0)
         return 1;

    delete Info; Info=new File_Unknown;
    if (Format_Test()>0)
         return 1;
    return 0;
    */
    InternalMethod=1;
    size_t ToReturn=ListFormats();

    Format_Test_FillBuffer_Close();
    return ToReturn;
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::Format_Test()
{
    //Integrity
    if (Info==NULL)
        return 0;
    #ifndef MEDIAINFO_MINIMIZESIZE
        Info->Init(&Config, &Details, &Stream, &Stream_More);
    #else //MEDIAINFO_MINIMIZESIZE
        Info->Init(&Config, &Stream, &Stream_More);
    #endif //MEDIAINFO_MINIMIZESIZE
    Info->File_Name=File_Name;
    
    //Test the format with buffer
    //-Test is already test with failure
    if (File_AlreadyBuffered && File_Size==0)
        return 0; //Already tested, ad a big problem on it

    //-Initating the format
    if (Format_Test_FillBuffer_Init()<0)
        return 0;
    Info->Open_Buffer_Init(File_Size);

    //-Test the format with buffer
    do
    {
        if (Format_Test_FillBuffer_Continue()<0)
            break; //Error during reading
        else if (Info)
            Info->Open_Buffer_Continue(Buffer, Buffer_Size);
    }
    while (Info && !Info->IsFinished);

    //-Close
    Format_Test_FillBuffer_Close();

    //Is this file detected?
    if (!Info->IsAccepted)
    {
        delete Info; Info=NULL;
        return 0;
    }

    //Finalize
    Info->Open_Buffer_Finalize();
    Info->Finalize_Global();

    //Cleanup
    delete Info; Info=NULL;
    return 1;
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::Format_Test_Buffer()
{
    //Integrity
    if (Info==NULL)
        return 0;

    if (Info->Count_Get(Stream_General)==0)
        Info->Open_Buffer_Init(NULL, File_Size, File_Offset);

    //-Test the format with buffer
    Info->Open_Buffer_Continue(NULL, BufferConst?BufferConst:Buffer, Buffer_Size);
    //We must wait fo more data
    if (Info->Count_Get(Stream_General)>0)
        return 1;
    else
        return 0;
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::Format_Test_FillBuffer_Init()
{
    //Integrity
    if (Info==NULL)
        return -1;

    //Is there a file to open?
    if (File_Name.empty())
        return 1; //Buffer is handled elsewhere
    if (File_Handle)
    {
        File_AlreadyBuffered=true;
        return 1; //Already opened
    }

    //Init
    Buffer_Size_Max=Buffer_NormalSize;
    File_Offset=0;
    Buffer=NULL;

    //Opening the file
    File_Handle=new File;
    ((File*)File_Handle)->Open(File_Name);
    if (!((File*)File_Handle)->Opened_Get())
    {
        File_AlreadyBuffered=true; //We don't succeed to open it, so File_Size is 0
        return -1;
    }


    //FileSize
    if (File_Size==0) //If not provided by Open_Buffer_Init()
        File_Size=((File*)File_Handle)->Size_Get();

    //Buffer
    delete[] Buffer; Buffer=new int8u[Buffer_Size_Max];

    return 1;
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::Format_Test_FillBuffer_Continue()
{
    //Integrity
    if (Info==NULL)
        return -1;

    //Is there a file to open?
    if (File_Name.empty())
    {
        if (File_Offset==0)
            return 1; //Buffer is handled elsewhere
        else
            return -1; //Not possible to have more
    }

    //Seek (if needed)
    if (Info->File_GoTo!=(int64u)-1 && Info->File_GoTo<File_Size)
    {
        if (Info->File_GoTo>=((File*)File_Handle)->Size_Get())
            //Seek requested, but on a file bigger in theory than what is in the real file, we can't do this
            return -1;
        if (((File*)File_Handle)->GoTo(Info->File_GoTo))
        {
            File_Offset=Info->File_GoTo;
            Info->Open_Buffer_Init(File_Size, File_Offset);
        }
        else
            //File is not seekable
            return -1;
    }

    //Buffering
    if (!File_AlreadyBuffered)
    {
        Buffer_Size=((File*)File_Handle)->Read(Buffer, Buffer_Size_Max);
        if (Buffer_Size!=0)
        {
            //Read is OK
            if (Buffer_Size==0)
                return -1;
            File_Offset+=Buffer_Size;
        }
        else
            //Problem while reading
            return -1;
    }
    else
        File_AlreadyBuffered=false;

    return 1;
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::Format_Test_FillBuffer_Close()
{
    //Close
    delete (File*)File_Handle; File_Handle=NULL;
    Buffer_Clear();

    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open (const int8u* Begin_, size_t Begin_Size_, const int8u*, size_t, int64u FileSize_)
{
    CriticalSectionLocker CSL(CS);
    Buffer_Size_Max=Begin_Size_;
    delete[] Buffer; Buffer=new int8u[Buffer_Size_Max];
    std::memcpy(Buffer, Begin_, Begin_Size_);
    Buffer_Size=Begin_Size_;
    File_Name.clear();
    File_Size=FileSize_;

    InternalMethod=1;
    size_t ToReturn=ListFormats();

    Buffer_Clear();
    return ToReturn;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open_Buffer_Init (int64u File_Size_, int64u File_Offset_)
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
    {
        if (!Config.File_ForceParser_Get().empty())
        {
            SelectFromExtension(Config.File_ForceParser_Get());
            MultipleParsing_IsDetected=true;
        }
        else
            Info=new File__MultipleParsing;
    }
    #ifndef MEDIAINFO_MINIMIZESIZE
        Info->Init(&Config, &Details, &Stream, &Stream_More);
    #else //MEDIAINFO_MINIMIZESIZE
        Info->Init(&Config, &Stream, &Stream_More);
    #endif //MEDIAINFO_MINIMIZESIZE
    Info->Open_Buffer_Init(File_Size_, File_Offset_);

    //Saving the real file size, in case the user provide the theoritical file size, to be used instead of the real file size
    File_Size=File_Size_;

    return 1;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open_Buffer_Continue (const int8u* ToAdd, size_t ToAdd_Size)
{
    CriticalSectionLocker CSL(CS);
    if (Info==NULL)
        return 0;

    Info->Open_Buffer_Continue(ToAdd, ToAdd_Size);

    if (!MultipleParsing_IsDetected && Info->IsAccepted)
    {
        //Found
        File__Analyze* Info_ToDelete=Info;
        Info=((File__MultipleParsing*)Info)->Parser_Get();
        delete Info_ToDelete; //Info_ToDelete=NULL;
        MultipleParsing_IsDetected=true;
    }

    #if 0 //temp, for old users
    //The parser wanted seek but the buffer is not seekable
    if (Info->File_GoTo!=(int64u)-1 && Config.File_IsSeekable_Get()==0)
    {
        Info->Open_Buffer_Finalize(true);
        Info->Finalize_Global();
        Info->File_GoTo=(int64u)-1;
        return 0;
    }

    return 1;
    #else
    //The parser wanted seek but the buffer is not seekable
    if (Info->File_GoTo!=(int64u)-1 && Config.File_IsSeekable_Get()==0)
    {
        Info->Open_Buffer_Fill();
        Info->Open_Buffer_Update();
        Info->File_GoTo=(int64u)-1;
    }

    if (Info)
    {
        size_t ToReturn=Info->IsAccepted<< 0
                      | Info->IsFilled  << 1
                      | Info->IsUpdated << 2
                      | Info->IsFinished<< 3;
        return ToReturn;
    }
    return 0;
    #endif
}

//---------------------------------------------------------------------------
int64u MediaInfo_Internal::Open_Buffer_Continue_GoTo_Get ()
{
    CriticalSectionLocker CSL(CS);
    if (Info!=NULL)
        return Info->File_GoTo;
    else
        return 0;
}

//---------------------------------------------------------------------------
size_t MediaInfo_Internal::Open_Buffer_Finalize ()
{
    CriticalSectionLocker CSL(CS);
    if (Info!=NULL)
    {
        Info->Open_Buffer_Finalize();
        Info->Finalize_Global();
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
    Buffer_Clear();
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

    if (Info && Info->IsUpdated)
        Info->Open_Buffer_Update();

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

//---------------------------------------------------------------------------
void MediaInfo_Internal::Buffer_Clear()
{
    Buffer_Size_Max=0;
    delete[] Buffer; Buffer=NULL;
    Buffer_Size=0;
    File_Size=0;
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::ApplyMethod()
{
    switch (InternalMethod)
    {
        case 1 : //Open file
            return Format_Test();
        case 2 : //Open buffer
            return Format_Test_Buffer();
        case 3 : //Supported formats
            delete Info; Info=NULL;
            return 0; //We want to continue the format listing
    }
    return 0;
}

} //NameSpace


