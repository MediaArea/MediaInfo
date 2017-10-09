/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const MediaInfoNameSpace::Char* MEDIAINFO_TITLE=__T("MediaArea.net/MediaInfo");
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

    GUI_Main_Handler=NULL;
    Kind=Kind_Easy;
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
    Text=MI->Inform((size_t)-1);
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
        MI->Option(__T("Thread"), __T("1"));
    if(Close)
        MI->Close();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Continue (const String &FileName)
{
    /*
    //Initilaizing MediaInfo
    MediaInfo MI;

    //From: preparing an example file for reading
    ZenLib::File From; From.Open(FileName, ZenLib::File::Access_Read); //You can use something else than a file

    //From: preparing a memory buffer for reading
    ZenLib::int8u* From_Buffer=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    size_t From_Buffer_Size; //The size of the read file buffer

    //Preparing to fill MediaInfo with a buffer
    MI.Option(__T("ReadByHuman"), __T("0"));
    MI.Option(__T("File_ForceParser"), __T("MpegTs"));
    MI.Option(__T("File_MpegTs_ForceMenu"), __T("1"));
    MI.Open_Buffer_Init();
    MI.Option(__T("File_IsSeekable"), __T("0"));
    int A=0;

    //The parsing loop
    bool CanWrite_OnlyIfParsingIsOk=false;
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size=From.Read(From_Buffer, 7*188);

        //Sending the buffer to MediaInfo
        size_t Result=MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
        if (Result&0xA && !CanWrite_OnlyIfParsingIsOk)
        {
            CanWrite_OnlyIfParsingIsOk=true;
        }


        if (CanWrite_OnlyIfParsingIsOk)
        {
            MI.Open_Buffer_Finalize();
            //MI.Option(__T("ReadByHuman"), __T("0"));
            //MI.Option(__T("File_ForceParser"), __T("MpegTs"));
            //MI.Option(__T("File_MpegTs_ForceMenu"), __T("1"));
            MI.Open_Buffer_Init();
            //MI.Option(__T("File_IsSeekable"), __T("0"));
            return 0;
            CanWrite_OnlyIfParsingIsOk=false;
            int64u File_Pos=From.Position_Get();
            A++;
            std::cout<<A<<std::endl;
            //if (A>2) {
            //    break;
            //}
        }
    }
    while (From_Buffer_Size>0);

    //Clean up
    delete[] From_Buffer;

    return 0;
    //MI->Option(__T("Trace_Format"), __T("csv"));
    //MI->Option(__T("Trace_Level"), __T("Container1;1"));
    //MI->Option(__T("File_MpegTs_ForceMenu"), __T("1"));
    //MI->Option(__T("File_Demux_Unpacketize"), __T("1"));
    //MI->Option(__T("ParseSpeed"), __T("1.0"));
    //MI->Option(__T("Inform"), __T("Details;0.9"));
    //MI->Option(__T("Language"), __T("raw"));
    //MI->Option(__T("Complete"), __T("1"));
    //MI->Option(__T("File_MpegTs_ForceMenu"), __T("1"));
    //MI->Option(__T("Trace_Format"), __T("csv"));
    //MI->Option(__T("Trace_Level"), __T("Container1;1"));
    //MI->Option(__T("ParseSpeed"), __T("0.1"));
    //MI->Option(__T("Inform"), __T("CSV"));
    //MI->Option(__T("Inform"), __T("Details;0.9"));


     MediaInfo MII;
     MII.Option(__T("Demux"), __T("container"));
     MII.Option(__T("File_NextPacket"), __T("1"));

     MII.Open(FileName);
     while (MII.Open_NextPacket()&0x100)
     {
        MII.Option(__T("File_Seek"), __T("2000000"));
        MII.Option(__T("File_Seek"), __T("50%"));
     }*/

    //MI->Option(__T("MpegTs_MaximumScanDuration"), __T("60"));
    //MI->Option(__T("mpegts_maximumoffset"), __T("1880"));
    //MI->Option(__T("Trace_Level"), __T("Container1;1"));
    //MI->Option(__T("ParseSpeed"), __T("1.0"));
    //MI->Option(__T("LegacyStreamDisplay"), __T("0"));
    //MI->Option(__T("File_MpegTs_Atsc_transport_stream_id_Trust"), __T("0"));
    //MI->Option(__T("Trace_Level"), __T("Container1;1"));
    //MI->Option(__T("File_Mxf_TimeCodeFromMaterialPackage"), __T("1"));
    //MI->Option(__T("demux"), __T("container"));
    //MI->Option(__T("file_demux_unpacketize"), __T("1"));
    //MI->Option(__T("Language"), __T("raw"));
    //MI->Option(__T("Inform"), __T("PBCore2"));
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
    Text=__T("Easy");
    Kind=Kind_Easy;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Sheet ()
{
    MI->Option(__T("Inform"), String());
    Text=__T("Sheet");
    Kind=Kind_Sheet;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Tree ()
{
    Text=__T("Root\n Item");
    Kind=Kind_Tree;
    if (Details>0)
        Menu_Debug_Details(Details);
}

//---------------------------------------------------------------------------
void Core::Menu_View_Text()
{
    MI->Option(__T("Inform"), String());
    Kind = Kind_Text;
    if (Details>0)
        Menu_Debug_Details(Details);
}

//---------------------------------------------------------------------------
void Core::Menu_View_HTML ()
{
    MI->Option(__T("Inform"), __T("HTML"));
    Kind=Kind_HTML;
}

//---------------------------------------------------------------------------
void Core::Menu_View_XML ()
{
    MI->Option(__T("Inform"), __T("MIXML"));
    Kind=Kind_XML;
}

//---------------------------------------------------------------------------
void Core::Menu_View_PBCore ()
{
    MI->Option(__T("Inform"), __T("PBCore"));
    Kind=Kind_PBCore;
}

//---------------------------------------------------------------------------
void Core::Menu_View_PBCore2 ()
{
    MI->Option(__T("Inform"), __T("PBCore2"));
    Kind=Kind_PBCore2;
}

//---------------------------------------------------------------------------
void Core::Menu_View_MPEG7 ()
{
    MI->Option(__T("Inform"), __T("MPEG-7"));
    Kind=Kind_MPEG7;
}

//---------------------------------------------------------------------------
void Core::Menu_View_EBUCore_1_5 ()
{
    MI->Option(__T("Inform"), __T("EBUCore_1.5"));
    Kind=Kind_EBUCore_1_5;
}

//---------------------------------------------------------------------------
void Core::Menu_View_EBUCore_1_6()
{
    MI->Option(__T("Inform"), __T("EBUCore_1.6"));
    Kind = Kind_EBUCore_1_6;
}

//---------------------------------------------------------------------------
void Core::Menu_View_EBUCore_1_8_ps()
{
    MI->Option(__T("Inform"), __T("EBUCore_1.8_parameterSegment"));
    Kind=Kind_EBUCore_1_8_ps;
}

//---------------------------------------------------------------------------
void Core::Menu_View_EBUCore_1_8_sp()
{
    MI->Option(__T("Inform"), __T("EBUCore_1.8_segmentParameter"));
    Kind=Kind_EBUCore_1_8_sp;
}

//---------------------------------------------------------------------------
void Core::Menu_View_EBUCore_1_8_ps_json()
{
    MI->Option(__T("Inform"), __T("EBUCore_1.8_parameterSegment_JSON"));
    Kind=Kind_EBUCore_1_8_ps_json;
}

//---------------------------------------------------------------------------
void Core::Menu_View_EBUCore_1_8_sp_json()
{
    MI->Option(__T("Inform"), __T("EBUCore_1.8_segmentParameter_JSON"));
    Kind=Kind_EBUCore_1_8_sp_json;
}

//---------------------------------------------------------------------------
void Core::Menu_View_FIMS_1_1 ()
{
    MI->Option(__T("Inform"), __T("FIMS_1.1"));
    Kind=Kind_FIMS_1_1;
}

//---------------------------------------------------------------------------
void Core::Menu_View_FIMS_1_2 ()
{
    MI->Option(__T("Inform"), __T("FIMS_1.2"));
    Kind=Kind_FIMS_1_2;
}

//---------------------------------------------------------------------------
void Core::Menu_View_FIMS_1_3 ()
{
    MI->Option(__T("Inform"), __T("FIMS_1.3"));
    Kind=Kind_FIMS_1_3;
}

//---------------------------------------------------------------------------
void Core::Menu_View_reVTMD()
{
    MI->Option(__T("Inform"), __T("reVTMD"));
    Kind = Kind_reVTMD;
}

//---------------------------------------------------------------------------
void Core::Menu_Option_Preferences_Inform (const String& Inform)
{
    MI->Option(__T("Inform"), Inform);
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
        MI->Option(__T("Complete"), __T("1"));
    else
        MI->Option(__T("Complete"), __T("0"));
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Details (float Value)
{
    Details=Value;

    MI->Option(__T("Inform"), __T("Details;")+ZenLib::Ztring::ToZtring(Value));
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Demux (const String &Value)
{
    if (Demux==Value)
        return;
    Demux=Value;

    MI->Option(__T("Demux"), Value);
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Version ()
{
    Text=MI->Option(__T("Info_Version"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Formats ()
{
    Text=MI->Option(__T("Info_Capacities"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Codecs ()
{
    Text=MI->Option(__T("Info_Codecs"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Parameters ()
{
    Text=MI->Option(__T("Info_Parameters"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_CanHandleUrls()
{
    Text=MI->Option(__T("Info_CanHandleUrls"));
}

//---------------------------------------------------------------------------
void Core::Menu_Language (const String& Language)
{
    MI->Option(__T("Language"), Language);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
String& Core::Inform_Get ()
{
    if (Text_Temp.empty())
        Text=MI->Inform((size_t)-1);
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
    MI->Option(__T("Inform"), String());
    return MI->Get(Pos, StreamKind, StreamPos, __T("Inform"));
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
    MI->Option(__T("Inform"), __T("Summary"));
    return MI->Get(File_Pos, StreamKind, StreamPos, __T("Inform"));
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
    case Stream_General: return __T("General");
        break;
    case Stream_Audio: return __T("Audio");
        break;
    case Stream_Video: return __T("Video");
        break;
    case Stream_Text: return __T("Text");
        break;
    case Stream_Other: return __T("Other");
        break;
    case Stream_Image: return __T("Image");
        break;
    case Stream_Menu: return __T("Menu");
        break;
    default: return __T("Unknown stream");
        break;
    }
}

String Core::Parameters () {
    return MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"));
}

size_t Core::State_Get() {
    return MI->State_Get();
}
