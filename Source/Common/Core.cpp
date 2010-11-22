// Main - Main functions
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <sstream>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const MediaInfoNameSpace::Char* MEDIAINFO_TITLE=_T("MediaInfo - http://mediainfo.sourceforge.net");
ZenLib::Ztring Text_Temp;
using namespace ZenLib;
//---------------------------------------------------------------------------


//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Core::Core()
{
    MI=new MediaInfoNameSpace::MediaInfoList;

    Details=0;
}

Core::~Core()
{
    delete MI;
}

//---------------------------------------------------------------------------
void Core::Data_Prepare()
{
    //Inform
    Text=MI->Inform((size_t)-1).c_str();
}

//***************************************************************************
// Menu
//***************************************************************************

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_File (const String& FileName)
{
    Menu_File_Open_Files_Begin();
    return Menu_File_Open_Files_Continue(FileName);
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Files_Begin (bool Close, bool WithThread)
{
    if(WithThread)
        MI->Option(_T("Thread"), _T("1"));
    if(Close)
        MI->Close();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Continue (const String &FileName)
{
    //Initilaizing MediaInfo
    /*MediaInfo MI;

    //From: preparing an example file for reading
    ZenLib::File From; From.Open(FileName, ZenLib::File::Access_Read); //You can use something else than a file

    //From: preparing a memory buffer for reading
    ZenLib::int8u* From_Buffer=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    size_t From_Buffer_Size; //The size of the read file buffer

    //To (Output 1): preparing an example file for writing
    ZenLib::File To_1;
    To_1.Open(FileName+_T(".Extract1"), ZenLib::File::Access_Write_Append);

    //To (Output 1): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_1=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_1_Name_Temp;
    To_Buffer_1_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_1<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_1_Name=To_Buffer_1_Name_Temp.str();

    //Preparing the Program numbers we want
    MediaInfoLib::String ProgramNumber1=_T("1");

    //Registering for duplication
    size_t To_Buffer_1_Pos;

    //Preparing to fill MediaInfo with a buffer
    MI.Option(_T("File_ForceParser"), _T("MpegTs"));
    MI.Open_Buffer_Init();
    bool CanWrite_OnlyIfParsingIsOk=true;
    To_Buffer_1_Pos=Ztring(MI.Option(_T("File_Duplicate"), To_Buffer_1_Name+_T(";program_number=")+ProgramNumber1)).To_int32u(); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_IsSeekable"), _T("0"));

    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size=From.Read(From_Buffer, 7*188);

        //Sending the buffer to MediaInfo
        size_t Result=MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
        if (Result&2 && !CanWrite_OnlyIfParsingIsOk)
        {
            CanWrite_OnlyIfParsingIsOk=true;
            Text=MI.Inform(); //Inform is ready!
            To_Buffer_1_Pos=Ztring(MI.Option(_T("File_Duplicate"), To_Buffer_1_Name+_T(";program_number=")+ProgramNumber1)).To_int32u(); //"memory://pointer:size;program_number=..."
        }


        if (CanWrite_OnlyIfParsingIsOk)
        {
            //Retrieving data written in memory
            size_t To_Buffer_Size_1=MI.Output_Buffer_Get(To_Buffer_1_Pos);

            //Writing data to somewhere, do what you want for this.
            To_1.Write(To_Buffer_1, To_Buffer_Size_1);
        }
    }
    while (From_Buffer_Size>0);

    //Clean up
    delete[] From_Buffer;
    delete[] To_Buffer_1;

    return 0;*/
    //MI->Option(_T("Trace_Format"), _T("csv"));
    //MI->Option(_T("Trace_Level"), _T("Container1;1"));
    //MI->Option(_T("File_MpegTs_ForceMenu"), _T("1"));
    //MI->Option(_T("File_Demux_Unpacketize"), _T("1"));
    //MI->Option(_T("ParseSpeed"), _T("1.0"));
    //MI->Option(_T("Inform"), _T("Details;0.9"));
    //MI->Option(_T("Language"), _T("raw"));
    //MI->Option(_T("Complete"), _T("1"));
    //MI->Option(_T("File_MpegTs_ForceMenu"), _T("1"));
    //MI->Option(_T("Trace_Format"), _T("csv"));
    //MI->Option(_T("Trace_Level"), _T("Container1;1"));
    //MI->Option(_T("ParseSpeed"), _T("0.1"));
    //MI->Option(_T("Inform"), _T("CSV"));
    //MI->Option(_T("Inform"), _T("Details;0.9"));


     /*MediaInfo MII;
     MII.Option(_T("Demux"), _T("container"));
     MII.Option(_T("File_NextPacket"), _T("1"));

     MII.Open(FileName);
     while (MII.Open_NextPacket()&0x100)
     {
        MII.Option(_T("File_Seek"), _T("2000000"));
        MII.Option(_T("File_Seek"), _T("50%"));
     }*/

    //MI->Option(_T("MpegTs_MaximumScanDuration"), _T("60"));
    //MI->Option(_T("mpegts_maximumoffset"), _T("1880"));
     return MI->Open(FileName);
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Directory (const String &DirectoryName)
{
    MI->Open(DirectoryName);
}

//---------------------------------------------------------------------------
void Core::Menu_View_Easy ()
{
    Text=_T("Easy");
    Kind=Kind_Easy;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Sheet ()
{
    MI->Option(_T("Inform"), String());
    Text=_T("Sheet");
    Kind=Kind_Sheet;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Tree ()
{
    Text=_T("Root\n Item");
    Kind=Kind_Tree;
    if (Details>0)
        Menu_Debug_Details(Details);
}

//---------------------------------------------------------------------------
void Core::Menu_View_HTML ()
{
    MI->Option(_T("Inform"), _T("HTML"));
    Kind=Kind_HTML;
}

//---------------------------------------------------------------------------
void Core::Menu_View_XML ()
{
    MI->Option(_T("Inform"), _T("XML"));
    Kind=Kind_XML;
}

//---------------------------------------------------------------------------
void Core::Menu_View_PBCore ()
{
    MI->Option(_T("Inform"), _T("PBCore"));
    Kind=Kind_PBCore;
}

//---------------------------------------------------------------------------
void Core::Menu_View_MPEG7 ()
{
    MI->Option(_T("Inform"), _T("MPEG-7"));
    Kind=Kind_MPEG7;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Text ()
{
    MI->Option(_T("Inform"), String());
    Kind=Kind_Text;
    if (Details>0)
        Menu_Debug_Details(Details);
}

//---------------------------------------------------------------------------
void Core::Menu_Option_Preferences_Inform (const String& Inform)
{
    MI->Option(_T("Inform"), Inform);
}

//---------------------------------------------------------------------------
String Core::Menu_Option_Preferences_Option (const String& Param, const String& Value)
{
    return MI->Option(Param, Value);
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Complete (bool Value)
{
    if (Value)
        MI->Option(_T("Complete"), _T("1"));
    else
        MI->Option(_T("Complete"), _T("0"));
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Details (float Value)
{
    Details=Value;

    StringStream ToSend;
    ToSend<<_T("Details;");
    ToSend<<Value;

    if (Value==0)
        MI->Option(_T("Inform"), String(_T("Details;0")));
    else
        MI->Option(_T("Inform"), String(_T("Details;0.9")));
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Demux (const String &Value)
{
    if (Demux==Value)
        return;
    Demux=Value;

    MI->Option(_T("Demux"), Value);
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Version ()
{
    Text=MI->Option(_T("Info_Version"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Formats ()
{
    Text=MI->Option(_T("Info_Capacities"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Codecs ()
{
    Text=MI->Option(_T("Info_Codecs"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Parameters ()
{
    Text=MI->Option(_T("Info_Parameters"));
}

//---------------------------------------------------------------------------
void Core::Menu_Language (const String& Language)
{
    MI->Option(_T("Language"), Language);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
String& Core::Inform_Get ()
{
    if (Text_Temp.empty())
        Text=MI->Inform((size_t)-1).c_str();
    else
        Text=Text_Temp;
    return Text;
}

//---------------------------------------------------------------------------
Core::kind Core::Kind_Get ()
{
    return Kind;
}

//---------------------------------------------------------------------------
String Core::Inform_Get(size_t Pos, stream_t StreamKind, size_t StreamPos)
{
    MI->Option(_T("Inform"), String());
    return MI->Get(Pos, StreamKind, StreamPos, _T("Inform")).c_str();
}

// Accessors
size_t Core::Count_Get() {
    return MI->Count_Get();
}

//---------------------------------------------------------------------------
size_t Core::Count_Get(size_t File_Pos, stream_t StreamKind, size_t StreamNumber)
{
    return MI->Count_Get(File_Pos, StreamKind, StreamNumber);
}

//---------------------------------------------------------------------------
String Core::Summary_Get(int File_Pos, stream_t StreamKind, size_t StreamPos)
{
    MI->Option(_T("Inform"), _T("Summary"));
    return MI->Get(File_Pos, StreamKind, StreamPos, _T("Inform")).c_str();
}

//---------------------------------------------------------------------------
String Core::Get (size_t FilePos, stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t InfoKind) {
    return MI->Get(FilePos,StreamKind,StreamNumber,Parameter,InfoKind);
}

//---------------------------------------------------------------------------
String Core::Get (size_t FilePos, stream_t StreamKind, size_t StreamNumber, const String &Parameter, info_t InfoKind) {
    return MI->Get(FilePos,StreamKind,StreamNumber,Parameter,InfoKind);
}

//---------------------------------------------------------------------------
String Core::StreamName (stream_t StreamKind) {
    switch(StreamKind) {
    case Stream_General: return _T("General");
        break;
    case Stream_Audio: return _T("Audio");
        break;
    case Stream_Video: return _T("Video");
        break;
    case Stream_Chapters: return _T("Chapters");
        break;
    case Stream_Image: return _T("Image");
        break;
    case Stream_Menu: return _T("Menu");
        break;
    case Stream_Text: return _T("Text");
        break;
    default: return _T("Unknown stream");
        break;
    }
}

String Core::Parameters () {
    return MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"));
}

size_t Core::State_Get() {
    return MI->State_Get();
}
