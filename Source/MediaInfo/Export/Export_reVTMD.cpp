// Export_reVTMD - reVTMD export
// Copyright (C) 2012-2012 U.S. National Archives
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the NARA OPEN SOURCE AGREEMENT VERSION 1.3
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
#include "MediaInfo/Export/Export_reVTMD.h"
#include "MediaInfo/File__Analyse_Automatic.h"
#include <ctime>
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Infos
//***************************************************************************


//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Export_reVTMD::Export_reVTMD ()
{
}

//---------------------------------------------------------------------------
Export_reVTMD::~Export_reVTMD ()
{
}

//***************************************************************************
// Input
//***************************************************************************

//---------------------------------------------------------------------------
void reVTMD_Transform__Codec (Ztring &ToReturn, MediaInfo_Internal &MI, stream_t StreamKind, size_t StreamPos, bool IsCodec)
{
    ToReturn+=IsCodec?_T("\t\t\t<codec>\n"):_T("\t\t<format>\n");

    //creatorLib
    if (!MI.Get(StreamKind, StreamPos, _T("Encoded_Library/Name")).empty())
    {
        if (IsCodec)
            ToReturn+=_T("\t");
        ToReturn+=_T("\t\t\t<creatorLib");
        if (!MI.Get(StreamKind, StreamPos, _T("Encoded_Library/Version")).empty())
            ToReturn+=_T(" version=\"")+MI.Xml_Content_Escape(MI.Get(StreamKind, StreamPos, _T("Encoded_Library/Version")))+_T('\"');
        ToReturn+=_T(">");
        ToReturn+=MI.Xml_Content_Escape(MI.Get(StreamKind, StreamPos, _T("Encoded_Library/Name")));
        ToReturn+=_T("</creatorLib>\n");
    }

    //creatorLibDate
    Ztring Encoded_Library_Date=MI.Get(StreamKind, StreamPos, _T("Encoded_Library/Date"));
    if (!Encoded_Library_Date.empty())
    {
        Encoded_Library_Date.FindAndReplace(_T("UTC "), _T(""));
        if (Encoded_Library_Date.find(_T(' '))!=string::npos)
        {
            Encoded_Library_Date.FindAndReplace(_T(" "), _T("T"));
            Encoded_Library_Date+=_T('Z');
        }
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<creatorLibDate>")+MI.Xml_Content_Escape(Encoded_Library_Date))+_T("</creatorLibDate>\n");
    }

    //name
    if (!MI.Get(StreamKind, StreamPos, _T("Format")).empty())
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<name>")+MI.Get(StreamKind, StreamPos, _T("Format")))+_T("</name>\n");

    //commercialName
    if (!MI.Get(StreamKind, StreamPos, _T("Format_Commercial_IfAny")).empty())
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<commercialName>")+MI.Get(StreamKind, StreamPos, _T("Format_Commercial_IfAny")))+_T("</commercialName>\n");

    //mimetype
    if (!MI.Get(StreamKind, StreamPos, _T("InternetMediaType")).empty())
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<mimetype>")+MI.Get(StreamKind, StreamPos, _T("InternetMediaType")))+_T("</mimetype>\n");
    else
    {
        //TODO: how to implement formats without Media Type?
        if (IsCodec)
            ToReturn+=_T("\t");
        ToReturn+=_T("\t\t\t<mimetype>");
        if (MI.Count_Get(Stream_Video))
            ToReturn+=_T("video/x-");
        else if (MI.Count_Get(Stream_Image))
            ToReturn+=_T("image/x-");
        else if (MI.Count_Get(Stream_Audio))
            ToReturn+=_T("audio/x-");
        else
            ToReturn+=_T("application/x-");
        ToReturn+=MI.Xml_Content_Escape(Ztring(MI.Get(StreamKind, StreamPos, _T("Format"))).MakeLowerCase());
        ToReturn+=_T("</mimetype>\n");
    }

    //profile
    if (!MI.Get(StreamKind, StreamPos, _T("Format_Profile")).empty())
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<profile>")+MI.Get(StreamKind, StreamPos, _T("Format_Profile")))+_T("</profile>\n");

    //settings
    if (!MI.Get(StreamKind, StreamPos, _T("Format_Settings")).empty())
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<settings>")+MI.Get(StreamKind, StreamPos, _T("Format_Settings")))+_T("</settings>\n");

    //version
    if (!MI.Get(StreamKind, StreamPos, _T("Format_Version")).empty())
        ToReturn+=(IsCodec?_T("\t"):_T(""))+(_T("\t\t\t<version>")+MI.Get(StreamKind, StreamPos, _T("Format_Version")))+_T("</version>\n");

    if (IsCodec)
    {
        //codecID
        if (!MI.Get(StreamKind, StreamPos, _T("CodecID")).empty())
            ToReturn+=_T("\t\t\t\t<codecID>")+MI.Get(StreamKind, StreamPos, _T("CodecID"))+_T("</codecID>\n");

        if (StreamKind==Stream_Video)
        {
            //scanType
            if (!MI.Get(Stream_Video, StreamPos, Video_ScanType_String).empty())
                ToReturn+=_T("\t\t\t\t<scanType>")+MI.Get(Stream_Video, StreamPos, Video_ScanType_String)+_T("</scanType>\n");

            //scanOrder
            if (!MI.Get(Stream_Video, StreamPos, Video_ScanOrder_String).empty())
                ToReturn+=_T("\t\t\t\t<scanOrder>")+MI.Get(Stream_Video, StreamPos, Video_ScanOrder_String)+_T("</scanOrder>\n");
        }

        if (StreamKind==Stream_Audio)
        {
            //channelCount
            if (!MI.Get(Stream_Audio, StreamPos, Audio_Channel_s_).empty())
                ToReturn+=_T("\t\t\t\t<channelCount>")+MI.Get(Stream_Audio, StreamPos, Audio_Channel_s_)+_T("</channelCount>\n");

            //endianness
            if (!MI.Get(Stream_Audio, StreamPos, Audio_Format_Settings_Endianness).empty())
                ToReturn+=_T("\t\t\t\t<endianness>")+MI.Get(Stream_Audio, StreamPos, Audio_Format_Settings_Endianness)+_T("</endianness>\n");

            //sign
            if (!MI.Get(Stream_Audio, StreamPos, Audio_Format_Settings_Sign).empty())
                ToReturn+=_T("\t\t\t\t<sign>")+MI.Get(Stream_Audio, StreamPos, Audio_Format_Settings_Sign)+_T("</sign>\n");
        }
    }

    ToReturn+=IsCodec?_T("\t\t\t</codec>\n"):_T("\t\t</format>\n");
}

//---------------------------------------------------------------------------
void reVTMD_Transform_Track(Ztring &ToReturn, MediaInfo_Internal &MI, stream_t StreamKind, size_t StreamPos, Ztring Type)
{
    //track, with id and type
    ToReturn+=_T("\t\t<track");
    if (!MI.Get(StreamKind, StreamPos, General_ID).empty())
        ToReturn+=_T(" id=\"")+MI.Get(StreamKind, StreamPos, General_ID)+_T('\"');
    ToReturn+=_T(" type=\"")+Type+_T("\">\n");

    //date
    Ztring Modified_Date=MI.Get(StreamKind, StreamPos, _T("Modified_Date"));
    if (!Modified_Date.empty())
    {
        Modified_Date.FindAndReplace(_T("UTC "), _T(""));
        Modified_Date.FindAndReplace(_T(" "), _T("T"));
        Modified_Date+=_T('Z');
        ToReturn+=_T("\t\t<date type=\"file modification\">")+MI.Xml_Content_Escape(Modified_Date)+_T("</date>\n");
    }

    //duration
    if (!MI.Get(StreamKind, StreamPos, _T("Duration/String3")).empty())
        ToReturn+=_T("\t\t\t<duration>")+MI.Get(StreamKind, StreamPos, _T("Duration/String3"))+_T("</duration>\n");

    //language
    if (!MI.Get(StreamKind, StreamPos, _T("Language/String3")).empty())
        ToReturn+=_T("\t\t\t<language>")+MI.Get(StreamKind, StreamPos, _T("Language/String3"))+_T("</language>\n");

    //size
    if (!MI.Get(StreamKind, StreamPos, _T("StreamSize")).empty())
        ToReturn+=_T("\t\t\t<size>")+MI.Get(StreamKind, StreamPos, _T("StreamSize"))+_T("</size>\n");

    //dataRate
    if (!MI.Get(StreamKind, StreamPos, _T("BitRate")).empty())
        ToReturn+=_T("\t\t\t<dataRate>")+MI.Get(StreamKind, StreamPos, _T("BitRate"))+_T("</dataRate>\n");

    //timecode
    if (!MI.Get(StreamKind, StreamPos, _T("Delay/String3")).empty() || !MI.Get(StreamKind, StreamPos, _T("Delay/String4")).empty())
    {
        ZtringList List_Value; List_Value.Separator_Set(0, _T(" / ")); List_Value.Write(MI.Get(StreamKind, StreamPos, MI.Get(StreamKind, StreamPos, _T("Delay/String4")).empty()?_T("Delay/String3"):_T("Delay/String4")));
        ZtringList List_Source; List_Source.Separator_Set(0, _T(" / ")); List_Source.Write(MI.Get(StreamKind, StreamPos, _T("Delay_Source")));
        
        for (size_t Pos=0; Pos<List_Value.size(); Pos++)
        {
            ToReturn+=_T("\t\t\t<timecode");
            Ztring Source=List_Source(Pos);
            if (!Source.empty())
                ToReturn+=_T(" recordMethod=\"")+MI.Xml_Content_Escape(Source)+_T('\"');
            ToReturn+=_T(">")+MI.Xml_Content_Escape(List_Value(Pos))+_T("</timecode>\n");
        }
    }

    if (StreamKind==Stream_Video)
    {
        //frame
        ToReturn+=_T("\t\t\t<frame>\n");
        if (!MI.Get(Stream_Video, StreamPos, Video_Width).empty())
            ToReturn+=_T("\t\t\t\t<pixelsHorizontal>")+MI.Get(Stream_Video, StreamPos, Video_Width)+_T("</pixelsHorizontal>\n");
        if (!MI.Get(Stream_Video, StreamPos, Video_Height).empty())
            ToReturn+=_T("\t\t\t\t<pixelsVertical>")+MI.Get(Stream_Video, StreamPos, Video_Height)+_T("</pixelsVertical>\n");
        if (!MI.Get(Stream_Video, StreamPos, Video_PixelAspectRatio).empty())
            ToReturn+=_T("\t\t\t\t<PAR>")+MI.Get(Stream_Video, StreamPos, Video_PixelAspectRatio)+_T("</PAR>\n");
        if (!MI.Get(Stream_Video, StreamPos, Video_DisplayAspectRatio).empty())
            ToReturn+=_T("\t\t\t\t<DAR>")+MI.Get(Stream_Video, StreamPos, Video_DisplayAspectRatio)+_T("</DAR>\n");
        if (!MI.Get(Stream_Video, StreamPos, Video_Rotation).empty())
            ToReturn+=_T("\t\t\t\t<rotation>")+MI.Get(Stream_Video, StreamPos, Video_Rotation)+_T("</rotation>\n");
        ToReturn+=_T("\t\t\t</frame>\n");

        //frameRate
        if (!MI.Get(Stream_Video, StreamPos, Video_FrameRate).empty())
            ToReturn+=_T("\t\t\t<frameRate>")+MI.Get(Stream_Video, StreamPos, Video_FrameRate)+_T("</frameRate>\n");
    }

    if (!MI.Get(StreamKind, StreamPos, _T("MD5")).empty())
        ToReturn+=_T("\t\t\t<checksum algorithm=\"md5\">")+MI.Get(StreamKind, StreamPos, _T("MD5"))+_T("</checksum>\n");

    //codec
    reVTMD_Transform__Codec(ToReturn, MI, StreamKind, StreamPos, true);

    //bitsPerSample
    if (!MI.Get(StreamKind, StreamPos, _T("BitDepth")).empty())
        ToReturn+=_T("\t\t\t<bitsPerSample>")+MI.Get(StreamKind, StreamPos, _T("BitDepth"))+_T("</bitsPerSample>\n");

    if (StreamKind==Stream_Video)
    {
        //bitsPerPixelStored
        if (!MI.Get(Stream_Video, StreamPos, Video_Bits__Pixel_Frame_).empty())
            ToReturn+=_T("\t\t\t<bitsPerPixelStored>")+MI.Get(Stream_Video, StreamPos, Video_Bits__Pixel_Frame_)+_T("</bitsPerPixelStored>\n");
    }

    //frameCount
    if (!MI.Get(StreamKind, StreamPos, _T("FrameCount")).empty())
        ToReturn+=_T("\t\t\t<frameCount>")+MI.Get(StreamKind, StreamPos, _T("FrameCount"))+_T("</frameCount>\n");

    if (StreamKind==Stream_Video)
    {
        //sampling
        if (!MI.Get(Stream_Video, StreamPos, Video_ChromaSubsampling).empty())
            ToReturn+=_T("\t\t\t<sampling>")+MI.Get(Stream_Video, StreamPos, Video_ChromaSubsampling)+_T("</sampling>\n");

        //signalFormat
        if (!MI.Get(Stream_Video, StreamPos, Video_Standard).empty())
            ToReturn+=_T("\t\t\t<signalFormat>")+MI.Get(Stream_Video, StreamPos, Video_Standard)+_T("</signalFormat>\n");
    }

    if (StreamKind==Stream_Audio)
    {
        //samplingRate
        if (!MI.Get(Stream_Audio, StreamPos, Audio_SamplingRate).empty())
            ToReturn+=_T("\t\t\t<samplingRate>")+MI.Get(Stream_Audio, StreamPos, Audio_SamplingRate)+_T("</samplingRate>\n");

        //sampleCount
        if (!MI.Get(Stream_Audio, StreamPos, Audio_SamplingCount).empty())
            ToReturn+=_T("\t\t\t<sampleCount>")+MI.Get(Stream_Audio, StreamPos, Audio_SamplingCount)+_T("</sampleCount>\n");
    }

    //annotation - all fields (except *_String*) separated by |
    /*
    Ztring Temp;
    for (size_t Pos=0; Pos<MI.Count_Get(StreamKind, StreamPos); Pos++)
        if (MI.Get(StreamKind, StreamPos, Pos, Info_Name).find(_T("String"))==std::string::npos && !MI.Get(StreamKind, StreamPos, Pos).empty())
            Temp+=MI.Get(StreamKind, StreamPos, Pos, Info_Name)+_T(": ")+MI.Get(StreamKind, StreamPos, Pos)+_T('|');
    if (!Temp.empty())
    {
        Temp.resize(Temp.size()-1);
        ToReturn+=_T("\t\t\t<annotation>"); ToReturn+=Temp; ToReturn+=_T("</annotation>\n");
    }
    */

    ToReturn+=_T("\t\t</track>\n");
}

//---------------------------------------------------------------------------
void reVTMD_Transform_Video(Ztring &ToReturn, MediaInfo_Internal &MI, size_t StreamPos)
{
    //Common
    reVTMD_Transform_Track(ToReturn, MI, Stream_Video, StreamPos, _T("video"));
}

//---------------------------------------------------------------------------
void reVTMD_Transform_Audio(Ztring &ToReturn, MediaInfo_Internal &MI, size_t StreamPos)
{
    //Common
    reVTMD_Transform_Track(ToReturn, MI, Stream_Audio, StreamPos, _T("audio"));
}

//---------------------------------------------------------------------------
void reVTMD_Transform_Text(Ztring &ToReturn, MediaInfo_Internal &MI, size_t StreamPos)
{
    //Common
    reVTMD_Transform_Track(ToReturn, MI, Stream_Text, StreamPos, _T("text"));
}

//---------------------------------------------------------------------------
void reVTMD_Transform_captureHistory(Ztring &ToReturn, MediaInfo_Internal &MI)
{
    Ztring Mastered_Date=MI.Get(Stream_General, 0, General_Mastered_Date);
    Ztring EncodedBy=MI.Get(Stream_General, 0, General_EncodedBy);
    Ztring OriginalSourceForm_DistributedBy=MI.Get(Stream_General, 0, General_OriginalSourceForm_DistributedBy);
    Ztring IMIT=MI.Get(Stream_General, 0, _T("IMIT"));
    if (Mastered_Date.empty() && EncodedBy.empty() && OriginalSourceForm_DistributedBy.empty() && IMIT.empty())
        return;

    Ztring Temp;

    //digitizationDate
    if (!Mastered_Date.empty())
    {
        Mastered_Date.FindAndReplace(_T(" "), _T("T"));
        Temp+=_T("\t\t\t<digitizationDate>")+Mastered_Date+_T("</digitizationDate>\n");
    }

    //digitizationEngineer
    if (!EncodedBy.empty())
        Temp+=_T("\t\t\t<digitizationEngineer>")+MI.Xml_Content_Escape(EncodedBy)+_T("</digitizationEngineer>\n");

    //source
    if (!OriginalSourceForm_DistributedBy.empty())
        Temp+=_T("\t\t\t<source>")+MI.Xml_Content_Escape(OriginalSourceForm_DistributedBy)+_T("</source>\n");

    //IMIT, specific to NARA
    ZtringListList List;
    List.Separator_Set(0, _T(" / ")); //Note: MediaInfo automaticly replace carriage return by " / "
    List.Separator_Set(1, _T(","));
    List.Write(IMIT);
    for (size_t Line=0; Line<List.size(); Line++)
    {
        Ztring Temp2;
        for (size_t Column=0; Column<List[Line].size(); Column++)
        {
            //role
            if (List[Line][Column].size()>=2 && List[Line][Column][0]==_T('R') && List[Line][Column][1]==_T('='))
                Temp2+=_T("\t\t\t\t<role>")+MI.Xml_Content_Escape(List[Line][Column].substr(2, string::npos))+_T("</role>\n");

            //description
            if (List[Line][Column].size()>=2 && List[Line][Column][0]==_T('T') && List[Line][Column][1]==_T('='))
                Temp2+=_T("\t\t\t\t<description>")+MI.Xml_Content_Escape(List[Line][Column].substr(2, string::npos))+_T("</description>\n");

            //manufacturer
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('M') && List[Line][Column][1]==_T('N') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<manufacturer>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</manufacturer>\n");

            //modelName
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('M') && List[Line][Column][1]==_T('D') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<modelName>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</modelName>\n");

            //serialNumber
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('S') && List[Line][Column][1]==_T('N') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<serialNumber>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</serialNumber>\n");

            //signal
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('S') && List[Line][Column][1]==_T('G') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<signal>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</signal>\n");

            //version
            if (List[Line][Column].size()>=2 && List[Line][Column][0]==_T('V') && List[Line][Column][1]==_T('='))
                Temp2+=_T("\t\t\t\t<version>")+MI.Xml_Content_Escape(List[Line][Column].substr(2, string::npos))+_T("</version>\n");

            //videoEncoding
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('V') && List[Line][Column][1]==_T('E') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<videoEncoding>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</videoEncoding>\n");

            //audioEncoding
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('A') && List[Line][Column][1]==_T('E') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<audioEncoding>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</audioEncoding>\n");

            //videoRate
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('V') && List[Line][Column][1]==_T('R') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<videoRate>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</videoRate>\n");

            //audioRate
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('A') && List[Line][Column][1]==_T('R') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<audioRate>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</audioRate>\n");

            //videoPixelFormat
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('P') && List[Line][Column][1]==_T('X') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<videoPixelFormat>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</videoPixelFormat>\n");

            //videoBitRate
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('V') && List[Line][Column][1]==_T('B') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<videoBitRate>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</videoBitRate>\n");

            //audioBitRate
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('A') && List[Line][Column][1]==_T('B') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<audioBitRate>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</audioBitRate>\n");

            //videoBitDepth
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('V') && List[Line][Column][1]==_T('D') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<videoBitDepth>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</videoBitDepth>\n");

            //audioBitDepth
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('A') && List[Line][Column][1]==_T('D') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<audioBitDepth>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</audioBitDepth>\n");

            //settings
            if (List[Line][Column].size()>=3 && List[Line][Column][0]==_T('S') && List[Line][Column][1]==_T('T') && List[Line][Column][2]==_T('='))
                Temp2+=_T("\t\t\t\t<settings>")+MI.Xml_Content_Escape(List[Line][Column].substr(3, string::npos))+_T("</settings>\n");
        }

        if (!Temp2.empty())
        {
            Temp+=_T("\t\t\t<codingProcessHistory>\n");
            Temp+=Temp2;
            Temp+=_T("\t\t\t</codingProcessHistory>\n");
        }
    }

    if (!Temp.empty())
    {
        ToReturn+=_T("\t\t<captureHistory>\n");
        ToReturn+=Temp;
        ToReturn+=_T("\t\t</captureHistory>\n");
    }
}

//---------------------------------------------------------------------------
Ztring Export_reVTMD::Transform(MediaInfo_Internal &MI)
{
    //Current date/time is ISO format
    time_t Time=time(NULL);
    Ztring TimeS; TimeS.Date_From_Seconds_1970((int32u)Time);
    TimeS.FindAndReplace(_T("UTC "), _T(""));
    TimeS.FindAndReplace(_T(" "), _T("T"));
    TimeS+=_T('Z');

    Ztring ToReturn;
    
    //Begin
    if (MI.IsFirst)
    {
        ToReturn+=_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        ToReturn+=_T("<reVTMD xsi:schemaLocation=\"http://www.archives.gov/reVTMD/ http://www.archives.gov/reVTMD/reVTMD.xsd\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.archives.gov/reVTMD/\" recordCreation=\"")+TimeS+_T("\" profile=\"")+MediaInfoLib::Config.Info_Version_Get()+_T("\">\n");
    }

    //Object
    ToReturn+=_T("\t<object>\n");

    //filename
    ToReturn+=_T("\t\t<filename>");
    ToReturn+=MI.Xml_Content_Escape(MI.Get(Stream_General, 0, General_FileName));
    if (!MI.Get(Stream_General, 0, General_FileExtension).empty())
        ToReturn+=_T('.')+MI.Xml_Content_Escape(MI.Get(Stream_General, 0, General_FileExtension));
    ToReturn+=_T("</filename>\n");

    //identifier
    if (!MI.Get(Stream_General, 0, General_Comment).empty())
    {
        ZtringListList List;
        List.Separator_Set(0, _T("; "));
        List.Separator_Set(1, _T(", "));
        List.Write(MI.Get(Stream_General, 0, General_Comment));
        if (!List.empty() && List[0].size()>=3)
            for (size_t Pos=0; Pos<List.size(); Pos++)
            {
                ToReturn+=_T("\t\t<identifier");
                if (!List[Pos](2).empty())
                    ToReturn+=_T(" annotation=\"")+MI.Xml_Content_Escape(List[Pos](2))+_T('\"');
                if (!List[Pos](1).empty())
                    ToReturn+=_T(" type=\"")+MI.Xml_Content_Escape(List[Pos](1))+_T('\"');
                ToReturn+=_T(">");
                ToReturn+=MI.Xml_Content_Escape(List[Pos](0));
                ToReturn+=_T("</identifier>\n");
            }
    }
    
    //organization
    if (!MI.Get(Stream_General, 0, General_Archival_Location).empty())
    {
        ToReturn+=_T("\t\t<organization>\n");
        ToReturn+=_T("\t\t\t<organization_main>\n");
        ToReturn+=_T("\t\t\t\t<name>")+MI.Xml_Content_Escape(MI.Get(Stream_General, 0, General_Archival_Location))+_T("</name>\n");
        ToReturn+=_T("\t\t\t</organization_main>\n");
        ToReturn+=_T("\t\t</organization>\n");
    }

    //date
    Ztring Modified_Date=MI.Get(Stream_General, 0, General_File_Modified_Date);
    if (!Modified_Date.empty())
    {
        Modified_Date.FindAndReplace(_T("UTC "), _T(""));
        Modified_Date.FindAndReplace(_T(" "), _T("T"));
        Modified_Date+=_T('Z');
        ToReturn+=_T("\t\t<date type=\"file modification\">")+MI.Xml_Content_Escape(Modified_Date)+_T("</date>\n");
    }

    //duration
    if (!MI.Get(Stream_General, 0, General_Duration_String3).empty())
        ToReturn+=_T("\t\t<duration>")+MI.Get(Stream_General, 0, General_Duration_String3)+_T("</duration>\n");

    //size
    if (!MI.Get(Stream_General, 0, General_FileSize).empty())
        ToReturn+=_T("\t\t<size>")+MI.Get(Stream_General, 0, General_FileSize)+_T("</size>\n");

    //dataRate
    if (!MI.Get(Stream_General, 0, General_OverallBitRate).empty())
    {
        ToReturn+=_T("\t\t<dataRate");
        if (!MI.Get(Stream_General, 0, General_OverallBitRate_Mode_String).empty())
            ToReturn+=_T(" mode=\"")+MI.Get(Stream_General, 0, General_OverallBitRate_Mode_String)+_T('\"');
        ToReturn+=_T(">");
        ToReturn+=MI.Get(Stream_General, 0, General_OverallBitRate);
        ToReturn+=_T("</dataRate>\n");
    }

    //format
    reVTMD_Transform__Codec(ToReturn, MI, Stream_General, 0, false);

    //location
    if (!MI.Get(Stream_General, 0, General_CompleteName).empty())
        ToReturn+=_T("\t\t<location type=\"URN\">")+MI.Xml_Content_Escape(MI.Get(Stream_General, 0, General_CompleteName))+_T("</location>\n");

    //checksum
    //TODO, not yet available in MI

    //Video streams
    for (size_t StreamPos=0; StreamPos<MI.Count_Get(Stream_Video); StreamPos++)
        reVTMD_Transform_Video(ToReturn, MI, StreamPos);

    //Audio streams
    for (size_t StreamPos=0; StreamPos<MI.Count_Get(Stream_Audio); StreamPos++)
        reVTMD_Transform_Audio(ToReturn, MI, StreamPos);

    //Text streams
    for (size_t StreamPos=0; StreamPos<MI.Count_Get(Stream_Text); StreamPos++)
        reVTMD_Transform_Text(ToReturn, MI, StreamPos);

    //captureHistory
    reVTMD_Transform_captureHistory(ToReturn, MI);

    //Object
    ToReturn+=_T("\t</object>\n");

    //End
    if (MI.IsLast)
        ToReturn+=_T("</reVTMD>\n");

    //Carriage return
    ToReturn.FindAndReplace(_T("\n"), EOL, 0, Ztring_Recursive);

    return ToReturn;
}

//***************************************************************************
//
//***************************************************************************

} //NameSpace
