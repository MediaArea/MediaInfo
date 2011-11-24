// File_Mpegv - Info for MPEG Video files
// Copyright (C) 2004-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
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

//***************************************************************************
// Infos (Global)
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGV_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES) || defined(MEDIAINFO_MXF_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
extern const float32 Mpegv_frame_rate[16]=
{
    (float32) 0,
    (float32)24000/(float32)1001,
    (float32)24,
    (float32)25,
    (float32)30000/(float32)1001,
    (float32)30,
    (float32)50,
    (float32)60000/(float32)1001,
    (float32)60,
    (float32) 0,
    (float32) 0,
    (float32) 0,
    (float32) 0,
    (float32) 0,
    (float32) 0,
    (float32) 0,
};

//---------------------------------------------------------------------------
const char* Mpegv_Colorimetry_format[4]=
{
    "",
    "4:2:0",
    "4:2:2",
    "4:4:4",
};

//---------------------------------------------------------------------------
const char* Mpegv_profile_and_level_indication (int8u profile_and_level_indication)
{
    switch (profile_and_level_indication)
    {
        case 0x82 : return "4:2:2@High";
        case 0x85 : return "4:2:2@Main";
        case 0x8A : return "Multi-view@High";
        case 0x8B : return "Multi-view@High-1440";
        case 0x8D : return "Multi-view@Main";
        case 0x8E : return "Multi-view@Low";
        default : return "";
    }
};

//---------------------------------------------------------------------------
const char* Mpegv_profile_and_level_indication_profile[]=
{
    "0",
    "High",
    "Spatial Scalable",
    "SNR Scalable",
    "Main",
    "Simple",
    "6",
    "7",
}; //4:2:2 Profile?

//---------------------------------------------------------------------------
const char* Mpegv_profile_and_level_indication_level[]=
{
    "0",
    "1",
    "2",
    "3",
    "High",
    "4",
    "High 1440",
    "5",
    "Main",
    "6",
    "Low",
    "7",
    "8",
    "9",
    "10",
    "11",
};

} //NameSpace

//---------------------------------------------------------------------------
#endif //...
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Mpegv.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/Utils.h"
#if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
    #include "MediaInfo/Text/File_DtvccTransport.h"
#endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
#if defined(MEDIAINFO_SCTE20_YES)
    #include "MediaInfo/Text/File_Scte20.h"
#endif //defined(MEDIAINFO_SCTE20_YES)
#if defined(MEDIAINFO_CDP_YES)
    #include "MediaInfo/Text/File_Cdp.h"
    #include <cstring>
#endif //defined(MEDIAINFO_CDP_YES)
#if defined(MEDIAINFO_AFDBARDATA_YES)
    #include "MediaInfo/Video/File_AfdBarData.h"
    #include <cstring>
#endif //defined(MEDIAINFO_AFDBARDATA_YES)
#if MEDIAINFO_EVENTS
    #include "MediaInfo/MediaInfo_Config_MediaInfo.h"
    #include "MediaInfo/MediaInfo_Events.h"
#endif //MEDIAINFO_EVENTS
using namespace ZenLib;

#undef FILLING_BEGIN
#define FILLING_BEGIN() \
    while (Element_Offset<Element_Size && Buffer[Buffer_Offset+(size_t)Element_Offset]==0x00) \
        Element_Offset++; \
    if (Element_Offset!=Element_Size) \
        Trusted_IsNot("Size error"); \
    else if (Element_IsOK()) \
    { \

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
namespace MediaInfoLib
{
//---------------------------------------------------------------------------

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const float32 Mpegv_aspect_ratio1[]=
{
    (float32)0,
    (float32)1,
    (float32)0.6735,
    (float32)0.7031, //16/9 PAL
    (float32)0.7615,
    (float32)0.8055,
    (float32)0.8437, //16/9 NTSC
    (float32)0.8935,
    (float32)0.9375, //4/3 PAL
    (float32)0.9815,
    (float32)1.0255,
    (float32)1.0695,
    (float32)1.1250, //4/3 NTSC
    (float32)1.1575,
    (float32)1.2015,
    (float32)0,
};

//---------------------------------------------------------------------------
const float32 Mpegv_aspect_ratio2[]=
{
    (float32)0,
    (float32)1,
    (float32)4/(float32)3,
    (float32)16/(float32)9,
    (float32)2.21,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
    (float32)0,
};

//---------------------------------------------------------------------------
const char* Mpegv_video_format[]=
{
    "Component",
    "PAL",
    "NTSC",
    "SECAM",
    "MAC",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpegv_picture_structure[]=
{
    "",
    "T", //Top Field
    "B", //Bottom Field
    "F", //Frame
};

const char* Mpegv_picture_coding_type[]=
{
    "",
    "I",
    "P",
    "B",
    "D",
    "",
    "",
    "",
};

const char* Mpegv_extension_start_code_identifier[]=
{
    "",
    "Sequence",
    "Sequence Display",
    "Quant Matrix",
    "Copyright",
    "Sequence Scalable",
    "",
    "Picture Display",
    "Picture Coding",
    "Picture Spatial Scalable",
    "Picture Temporal Scalable",
    "Camera Parameters",
    "ITU-T",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpegv_colour_primaries(int8u colour_primaries)
{
    switch (colour_primaries)
    {
        case  1 : return "BT.709";
        case  4 : return "BT.470-2 system M";
        case  5 : return "BT.470-2 System B, BT.470-2 System G";
        case  6 : return "SMPTE 170M";
        case  7 : return "SMPTE 240M";
        default : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpegv_transfer_characteristics(int8u transfer_characteristics)
{
    switch (transfer_characteristics)
    {
        case  1 : return "BT.709";
        case  4 : return "BT.470-2 System M";
        case  5 : return "BT.470-2 System B, BT.470-2 System G";
        case  6 : return "SMPTE 170M";
        case  7 : return "SMPTE 240M";
        case  8 : return "Linear";
        default : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpegv_matrix_coefficients(int8u matrix_coefficients)
{
    switch (matrix_coefficients)
    {
        case  1 : return "BT.709";
        case  4 : return "FCC";
        case  5 : return "BT.470-2 System B, BT.470-2 System G";
        case  6 : return "SMPTE 170M";
        case  7 : return "SMPTE 240M";
        default : return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpegv::File_Mpegv()
:File__Analyze()
{
    //Configuration
    ParserName=_T("MPEG Video");
    #if MEDIAINFO_EVENTS
        ParserIDs[0]=MediaInfo_Parser_Mpegv;
        StreamIDs_Width[0]=16;
    #endif //MEDIAINFO_EVENTS
    #if MEDIAINFO_TRACE
        Trace_Layers_Update(8); //Stream
    #endif //MEDIAINFO_TRACE
    Trusted_Multiplier=2;
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=64*1024;
    PTS_DTS_Needed=true;
    IsRawStream=true;
    Frame_Count_NotParsedIncluded=0;

    //In
    MPEG_Version=1;
    Frame_Count_Valid=Config_ParseSpeed>=0.3?40:2;
    FrameIsAlwaysComplete=false;
    TimeCodeIsNotTrustable=false;
    #if defined(MEDIAINFO_ANCILLARY_YES)
        Ancillary=NULL;
    #endif //defined(MEDIAINFO_ANCILLARY_YES)

    //temporal_reference
    TemporalReference_Offset=0;
    #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        GA94_03_Parser=NULL;
        GA94_03_TemporalReference_Offset=0;
        GA94_03_IsPresent=false;
        Text_Positions.push_back(text_position(GA94_03_Parser));
        CC___Parser=NULL;
        CC___IsPresent=false;
        Text_Positions.push_back(text_position(CC___Parser));
    #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
    #if defined(MEDIAINFO_SCTE20_YES)
        Scte_Parser=NULL;
        Scte_TemporalReference_Offset=0;
        Scte_IsPresent=false;
        Text_Positions.push_back(text_position(Scte_Parser));
    #endif //defined(MEDIAINFO_SCTE20_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        DTG1_Parser=NULL;
        GA94_06_Parser=NULL;
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
    #if defined(MEDIAINFO_CDP_YES)
        Cdp_Parser=NULL;
        Cdp_IsPresent=false;
        Text_Positions.push_back(text_position(Cdp_Parser));
    #endif //defined(MEDIAINFO_CDP_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        AfdBarData_Parser=NULL;
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)

    //Temp
    SizeToAnalyse_Begin=1*1024*1024;
    SizeToAnalyse_End=1*1024*1024;
    Searching_TimeStamp_Start_DoneOneTime=false;
    sequence_header_IsParsed=false;
    FrameInfo.DTS=0;
    Parsing_End_ForDTS=false;
}

//---------------------------------------------------------------------------
File_Mpegv::~File_Mpegv()
{
    for (size_t Pos=0; Pos<TemporalReference.size(); Pos++)
        delete TemporalReference[Pos]; //TemporalReference[Pos]=NULL;
    #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        delete GA94_03_Parser; //GA94_03_Parser=NULL;
        delete CC___Parser; //CC___Parser=NULL;
    #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
    #if defined(MEDIAINFO_SCTE20_YES)
        delete Scte_Parser; //Scte_Parser=NULL;
    #endif //defined(MEDIAINFO_SCTE20_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        delete DTG1_Parser; //DTG1_Parser=NULL;
        delete GA94_06_Parser; //GA94_06_Parser=NULL;
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Streams_Accept()
{
    Stream_Prepare(Stream_Video);
}

//---------------------------------------------------------------------------
void File_Mpegv::Streams_Update()
{
    for (size_t Text_Positions_Pos=0; Text_Positions_Pos<Text_Positions.size(); Text_Positions_Pos++)
    {
        if (*Text_Positions[Text_Positions_Pos].Parser && (*Text_Positions[Text_Positions_Pos].Parser)->Status[IsUpdated])
        {
            Update(*Text_Positions[Text_Positions_Pos].Parser);
            for (size_t Pos=0; Pos<(*Text_Positions[Text_Positions_Pos].Parser)->Count_Get(Stream_Text); Pos++)
            {
                bool IsNewStream=false;
                if (Retrieve(Stream_Text, Text_Positions[Text_Positions_Pos].StreamPos+Pos, Text_ID)!=(*Text_Positions[Text_Positions_Pos].Parser)->Get(Stream_Text, Pos, Text_ID))
                {
                    Stream_Prepare(Stream_Text, Text_Positions[Text_Positions_Pos].StreamPos+Pos);
                    for (size_t Text_Positions_Pos2=Text_Positions_Pos+1; Text_Positions_Pos2<Text_Positions.size(); Text_Positions_Pos2++)
                        Text_Positions[Text_Positions_Pos2].StreamPos++;
                    IsNewStream=true;
                }
                Merge(*(*Text_Positions[Text_Positions_Pos].Parser), Stream_Text, Pos, Text_Positions[Text_Positions_Pos].StreamPos+Pos);
                
                if (IsNewStream)
                {
                    if ((*Text_Positions[Text_Positions_Pos].Parser)==GA94_03_Parser)
                    {
                        Ztring MuxingMode=Retrieve(Stream_Text, Text_Positions[Text_Positions_Pos].StreamPos+Pos, "MuxingMode");
                        Fill(Stream_Text, Text_Positions[Text_Positions_Pos].StreamPos+Pos, "MuxingMode", _T("A/53 / ")+MuxingMode, true);
                    }
                    if ((*Text_Positions[Text_Positions_Pos].Parser)==Cdp_Parser)
                    {
                        Ztring MuxingMode=Retrieve(Stream_Text, Text_Positions[Text_Positions_Pos].StreamPos+Pos, "MuxingMode");
                        Fill(Stream_Text, Text_Positions[Text_Positions_Pos].StreamPos+Pos, "MuxingMode", _T("Ancillary data / ")+MuxingMode, true);
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
void File_Mpegv::Streams_Fill()
{
    //Version
    if (MPEG_Version==2)
    {
        Fill(Stream_General, 0, General_Format_Version, "Version 2");
        Fill(Stream_Video, 0, Video_Format, "MPEG Video");
        Fill(Stream_Video, 0, Video_Format_Version, "Version 2");
        Fill(Stream_Video, 0, Video_Format_Commercial, "MPEG-2 Video");
        Fill(Stream_Video, 0, Video_Codec, "MPEG-2V");
        Fill(Stream_Video, 0, Video_Codec_String, "MPEG-2 Video", Unlimited, true, true);
    }
    else
    {
        Fill(Stream_General, 0, General_Format_Version, "Version 1");
        Fill(Stream_Video, 0, Video_Format, "MPEG Video");
        Fill(Stream_Video, 0, Video_Format_Version, "Version 1");
        Fill(Stream_Video, 0, Video_Format_Commercial, "MPEG-1 Video");
        Fill(Stream_Video, 0, Video_Codec, "MPEG-1V");
        Fill(Stream_Video, 0, Video_Codec_String, "MPEG-1 Video", Unlimited, true, true);
    }

    Fill(Stream_Video, 0, Video_Width, 0x1000*horizontal_size_extension+horizontal_size_value);
    Fill(Stream_Video, 0, Video_Height, 0x1000*vertical_size_extension+vertical_size_value);
    Fill(Stream_Video, 0, Video_Colorimetry, Mpegv_Colorimetry_format[chroma_format]);
    Fill(Stream_Video, 0, Video_ColorSpace, "YUV");
    Fill(Stream_Video, 0, Video_BitDepth, 8);

    //AspectRatio
    if (MPEG_Version==2)
    {
        if (aspect_ratio_information==0)
            ;//Forbidden
        else if (aspect_ratio_information==1)
            Fill(Stream_Video, 0, Video_PixelAspectRatio, 1.000, 3, true);
        else if (display_horizontal_size && display_vertical_size)
        {
            if (vertical_size_value && Mpegv_aspect_ratio2[aspect_ratio_information])
                Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, (float)(0x1000*horizontal_size_extension+horizontal_size_value)/(0x1000*vertical_size_extension+vertical_size_value)
                                                                             *Mpegv_aspect_ratio2[aspect_ratio_information]/((float)display_horizontal_size/display_vertical_size), 3, true);
        }
        else if (Mpegv_aspect_ratio2[aspect_ratio_information])
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, Mpegv_aspect_ratio2[aspect_ratio_information], 3, true);
    }
    else //Version 1
    {
        if (vertical_size_value && Mpegv_aspect_ratio1[aspect_ratio_information])
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, (float)(0x1000*horizontal_size_extension+horizontal_size_value)/(0x1000*vertical_size_extension+vertical_size_value)/Mpegv_aspect_ratio1[aspect_ratio_information], 3, true);
    }

    //FrameRate
    Fill(Stream_Video, StreamPos_Last, Video_FrameRate, (float)(Mpegv_frame_rate[frame_rate_code] * (frame_rate_extension_n + 1)) / (float)(frame_rate_extension_d + 1));

    //BitRate
    if (vbv_delay==0xFFFF || (MPEG_Version==1 && bit_rate_value==0x3FFFF))
        Fill(Stream_Video, 0, Video_BitRate_Mode, "VBR");
    if (bit_rate_value_IsValid && (bit_rate_extension>0 || bit_rate_value!=0x3FFFF))
        Fill(Stream_Video, 0, Video_BitRate_Maximum, ((((int32u)bit_rate_extension<<12))+bit_rate_value)*400);

    //Interlacement
    if (MPEG_Version==1)
    {
        Fill(Stream_Video, 0, Video_ScanType, "Progressive");
        Fill(Stream_Video, 0, Video_Interlacement, "PPF");
    }
    else if (progressive_frame_Count && progressive_frame_Count!=Frame_Count)
    {
        //This is mixed
    }
    else if (Frame_Count>0) //Only if we have at least one progressive_frame definition
    {
        if (progressive_sequence || progressive_frame_Count==Frame_Count)
        {
            Fill(Stream_Video, 0, Video_ScanType, "Progressive");
            Fill(Stream_Video, 0, Video_Interlacement, "PPF");
            if (!progressive_sequence && !(Interlaced_Top && Interlaced_Bottom) && !(!Interlaced_Top && !Interlaced_Bottom))
                Fill(Stream_Video, 0, Video_ScanOrder, Interlaced_Top?"TFF":"BFF");
        }
        else
        {
            Fill(Stream_Video, 0, Video_ScanType, "Interlaced");
            if ((Interlaced_Top && Interlaced_Bottom) || (!Interlaced_Top && !Interlaced_Bottom))
                Fill(Stream_Video, 0, Video_Interlacement, "Interlaced");
            else
            {
                Fill(Stream_Video, 0, Video_ScanOrder, Interlaced_Top?"TFF":"BFF");
                Fill(Stream_Video, 0, Video_Interlacement, Interlaced_Top?"TFF":"BFF");
            }
        }
        std::string TempRef, CodingType;
        for (size_t Pos=0; Pos<TemporalReference.size(); Pos++)
            if (TemporalReference[Pos] && TemporalReference[Pos]->HasPictureCoding)
            {
                TempRef+=TemporalReference[Pos]->top_field_first?"T":"B";
                TempRef+=TemporalReference[Pos]->repeat_first_field?"3":"2";
                CodingType+=Mpegv_picture_coding_type[TemporalReference[Pos]->picture_coding_type];
            }
        if (TempRef.find('3')!=std::string::npos)
        {
            if (TempRef.find("T2T3B2B3T2T3B2B3")!=std::string::npos
             || TempRef.find("B2B3T2T3B2B3T2T3")!=std::string::npos)
            {
                Fill(Stream_Video, 0, Video_ScanOrder, "2:3 Pulldown", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_FrameRate, FrameRate*24/30, 3, true); //Real framerate
                Fill(Stream_Video, 0, Video_ScanType, "Progressive", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_Interlacement, "PPF", Unlimited, true, true);
            }
            if (TempRef.find("T2T2T2T2T2T2T2T2T2T2T2T3B2B2B2B2B2B2B2B2B2B2B2B3")!=std::string::npos
             || TempRef.find("B2B2B2B2B2B2B2B2B2B2B2B3T2T2T2T2T2T2T2T2T2T2T2T3")!=std::string::npos)
            {
                Fill(Stream_Video, 0, Video_ScanOrder, "2:2:2:2:2:2:2:2:2:2:2:3 Pulldown", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_FrameRate, FrameRate*24/25, 3, true); //Real framerate
                Fill(Stream_Video, 0, Video_ScanType, "Progressive", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_Interlacement, "PPF", Unlimited, true, true);
            }
        }

        //GOP
        std::vector<Ztring> GOPs;
        size_t GOP_Frame_Count=0;
        size_t GOP_BFrames_Max=0;
        size_t I_Pos1=CodingType.find(_T('I'));
        while (I_Pos1!=std::string::npos)
        {
            size_t I_Pos2=CodingType.find(_T('I'), I_Pos1+1);
            if (I_Pos2!=std::string::npos)
            {
                std::vector<size_t> P_Positions;
                size_t P_Position=I_Pos1;
                do
                {
                    P_Position=CodingType.find(_T('P'), P_Position+1);
                    if (P_Position<I_Pos2)
                        P_Positions.push_back(P_Position);
                }
                while (P_Position<I_Pos2);
                Ztring GOP;
                if (!P_Positions.empty())
                {
                    GOP+=_T("M=")+Ztring::ToZtring(P_Positions[0]-I_Pos1)+_T(", ");
                    if (P_Positions[0]-I_Pos1>GOP_BFrames_Max)
                        GOP_BFrames_Max=P_Positions[0]-I_Pos1;
                }
                GOP+=_T("N=")+Ztring::ToZtring(I_Pos2-I_Pos1);
                GOPs.push_back(GOP);
                GOP_Frame_Count+=I_Pos2-I_Pos1;
            }
            I_Pos1=I_Pos2;
        }

        if (GOP_Frame_Count+GOP_BFrames_Max>=Frame_Count && !GOPs.empty())
            GOPs.resize(GOPs.size()-1); //Removing the last one, there may have uncomplete B-frame filling

        if (!GOPs.empty())
        {
            size_t Unique=0;
            for (size_t Pos=1; Pos<GOPs.size(); Pos++)
                if (GOPs[Pos]!=GOPs[0])
                    Unique++;
            if ((Frame_Count<Frame_Count_Valid*10 && Unique) || Unique>2) //In order to accept some unsynch //TODO: change the method, synching with next I-Frame
                GOPs.clear(); //Not a fixed GOP
        }
        else if (TemporalReference.size()==1 && CodingType=="I" && Frame_Count>1)
            GOPs.push_back(_T("N=1"));
        if (!GOPs.empty())
            Fill(Stream_Video, 0, Video_Format_Settings_GOP, GOPs[0]);
    }

    //Profile
    if (!profile_and_level_indication_escape && profile_and_level_indication_profile!=(int8u)-1 && profile_and_level_indication_level!=(int8u)-1)
    {
        Fill(Stream_Video, 0, Video_Format_Profile, Ztring().From_Local(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile])+_T("@")+Ztring().From_Local(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]));
        Fill(Stream_Video, 0, Video_Codec_Profile, Ztring().From_Local(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile])+_T("@")+Ztring().From_Local(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]));
    }
    else if (profile_and_level_indication_escape)
    {
        Fill(Stream_Video, 0, Video_Format_Profile, Ztring().From_Local(Mpegv_profile_and_level_indication(profile_and_level_indication)));
        Fill(Stream_Video, 0, Video_Codec_Profile, Ztring().From_Local(Mpegv_profile_and_level_indication(profile_and_level_indication)));
    }

    //Standard
    Fill(Stream_Video, 0, Video_Standard, Mpegv_video_format[video_format]);
    Fill(Stream_Video, 0, "colour_primaries", Mpegv_colour_primaries(colour_primaries));
    Fill(Stream_Video, 0, "transfer_characteristics", Mpegv_transfer_characteristics(transfer_characteristics));
    Fill(Stream_Video, 0, "matrix_coefficients", Mpegv_matrix_coefficients(matrix_coefficients));

    //Matrix
    if (load_intra_quantiser_matrix || load_non_intra_quantiser_matrix)
    {
        Fill(Stream_Video, 0, Video_Format_Settings, "CustomMatrix");
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix, "Custom");
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix_Data, Matrix_intra);
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix_Data, Matrix_nonintra);
        Fill(Stream_Video, 0, Video_Codec_Settings, "CustomMatrix");
        Fill(Stream_Video, 0, Video_Codec_Settings_Matrix, "Custom");
    }
    else
    {
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix, "Default");
        Fill(Stream_Video, 0, Video_Codec_Settings_Matrix, "Default");
    }

    //library
    if (Library.size()>=8)
    {
        Fill(Stream_Video, 0, Video_Encoded_Library, Library);
        Fill(Stream_Video, 0, Video_Encoded_Library_Name, Library_Name);
        Fill(Stream_Video, 0, Video_Encoded_Library_Version, Library_Version);
        Fill(Stream_Video, 0, General_Encoded_Library, Library);
        Fill(Stream_Video, 0, General_Encoded_Library_Name, Library_Name);
        Fill(Stream_Video, 0, General_Encoded_Library_Version, Library_Version);
    }

    //Delay
    if (group_start_FirstPass)
    {
        float64 Time_Begin=Time_Begin_Seconds*1000;
        if (FrameRate)
            Time_Begin+=((float64)Time_Begin_Frames)*1000/FrameRate;
        Fill(Stream_Video, 0, Video_Delay, Time_Begin, 0);
        Fill(Stream_Video, 0, Video_Delay_Settings, Ztring(_T("drop_frame_flag="))+(group_start_drop_frame_flag?_T("1"):_T("0")));
        Fill(Stream_Video, 0, Video_Delay_Settings, Ztring(_T("closed_gop="))+(group_start_closed_gop?_T("1"):_T("0")));
        Fill(Stream_Video, 0, Video_Delay_Settings, Ztring(_T("broken_link="))+(group_start_broken_link?_T("1"):_T("0")));
        Fill(Stream_Video, 0, Video_Delay_Source, "Stream");
    }

    //BVOP
    if (BVOP_Count>0)
    {
        Fill(Stream_Video, 0, Video_Format_Settings, "BVOP");
        Fill(Stream_Video, 0, Video_Format_Settings_BVOP, "Yes");
    }
    else
        Fill(Stream_Video, 0, Video_Format_Settings_BVOP, "No");

    //Buffer
    Fill(Stream_Video, 0, Video_BufferSize, 2*1024*((((int32u)vbv_buffer_size_extension)<<10)+vbv_buffer_size_value));

    //Autorisation of other streams
    if (!Status[IsAccepted])
    {
        NextCode_Clear();
        NextCode_Add(0x00);
        NextCode_Add(0xB8);
    }
    for (int8u Pos=0x00; Pos<=0xB9; Pos++)
        Streams[Pos].Searching_Payload=false;
    Streams[0xB8].Searching_TimeStamp_End=true;
    if (IsSub)
        Streams[0x00].Searching_TimeStamp_End=true;

    //Caption may be in user_data, must be activated if full parsing is requested
    Streams[0x00].Searching_Payload=GA94_03_IsPresent || Cdp_IsPresent;
    Streams[0xB2].Searching_Payload=GA94_03_IsPresent || CC___IsPresent || Scte_IsPresent;
    Streams[0xB3].Searching_Payload=GA94_03_IsPresent || Cdp_IsPresent;
    if (Config_ParseSpeed>=1)
    {
        Streams[0x00].Searching_Payload=true;
        Streams[0xB2].Searching_Payload=true;
        Streams[0xB3].Searching_Payload=true;
        Streams[0xB5].Searching_Payload=true;
    }

    #if defined(MEDIAINFO_AFDBARDATA_YES)
        if (DTG1_Parser)
            Merge(*DTG1_Parser, Stream_Video, 0, 0);
        if (GA94_06_Parser)
            Merge(*GA94_06_Parser, Stream_Video, 0, 0);
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        if (AfdBarData_Parser)
            Merge(*AfdBarData_Parser, Stream_Video, 0, 0);
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)

    //extra
    if (intra_dc_precision!=(int8u)-1)
    {
        Fill(Stream_Video, 0, "intra_dc_precision", 8+intra_dc_precision);
        (*Stream_More)[Stream_Video][0](Ztring().From_Local("intra_dc_precision"), Info_Options)=_T("N NT");
    }

    //Commercial name
    if (Retrieve(Stream_Video, 0, Video_Format_Version)==_T("Version 2")
     && Retrieve(Stream_Video, 0, Video_DisplayAspectRatio)==_T("1.778")
     && Retrieve(Stream_Video, 0, Video_BitDepth)==_T("8")
     && Retrieve(Stream_Video, 0, Video_ChromaSubsampling)==_T("4:2:0"))
    {
        //HDV1
        if (Retrieve(Stream_Video, 0, Video_Width)==_T("1280")
         && Retrieve(Stream_Video, 0, Video_Height)==_T("720")
         && Retrieve(Stream_Video, 0, Video_ScanType)==_T("Progressive")
         && (Retrieve(Stream_Video, 0, Video_FrameRate)==_T("60.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("59.940") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("30.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("29.970") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("24.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("23.976") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("50.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("25.000"))
         && (Retrieve(Stream_Video, 0, Video_Format_Profile)==_T("Main@High") || Retrieve(Stream_Video, 0, Video_Format_Profile)==_T("Main@High 1440"))
         && Retrieve(Stream_Video, 0, Video_BitRate).To_int64u()<20000000 && Retrieve(Stream_Video, 0, Video_BitRate_Maximum).To_int64u()<20000000)
            Fill(Stream_Video, 0, Video_Format_Commercial_IfAny, "HDV 720p");

        //HDV2
        if (Retrieve(Stream_Video, 0, Video_Width)==_T("1440")
         && Retrieve(Stream_Video, 0, Video_Height)==_T("1080")
         && Retrieve(Stream_Video, 0, Video_Format_Profile)==_T("Main@High 1440")
         && Retrieve(Stream_Video, 0, Video_BitRate).To_int64u()<27000000 && Retrieve(Stream_Video, 0, Video_BitRate_Maximum).To_int64u()<27000000)
        {
            //Interlaced
            if (Retrieve(Stream_Video, 0, Video_ScanType)==_T("Interlaced")
             && (Retrieve(Stream_Video, 0, Video_FrameRate)==_T("30.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("29.970") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("50.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("25.000")))
            Fill(Stream_Video, 0, Video_Format_Commercial_IfAny, "HDV 1080i");

            //Progressive
            if (Retrieve(Stream_Video, 0, Video_ScanType)==_T("Progressive")
             && (Retrieve(Stream_Video, 0, Video_FrameRate)==_T("30.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("29.970") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("24.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("23.976") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("50.000") || Retrieve(Stream_Video, 0, Video_FrameRate)==_T("25.000")))
            Fill(Stream_Video, 0, Video_Format_Commercial_IfAny, "HDV 1080p");
        }
    }
}

//---------------------------------------------------------------------------
void File_Mpegv::Streams_Finish()
{
    //Duration
    if (PTS_End>PTS_Begin)
        Fill(Stream_Video, 0, Video_Duration, float64_int64s(((float64)(PTS_End-PTS_Begin))/1000000));
    else if (Time_End_Seconds!=Error)
    {
        float32 Time_Begin=Time_Begin_Seconds*1000;
        float32 Time_End =Time_End_Seconds*1000;
        if (FrameRate)
        {
            float32 FrameRate_Corrected=FrameRate;
            if (FrameRate>=29.970 && FrameRate<=29.971 && !group_start_drop_frame_flag)
                FrameRate_Corrected=30;
            Time_Begin+=Time_Begin_Frames*1000/FrameRate_Corrected;
            Time_End  +=Time_End_Frames  *1000/FrameRate_Corrected;
        }
        float32 Duration=Time_End-Time_Begin;
        if (FrameRate>=29.970 && FrameRate<=29.971 && !group_start_drop_frame_flag)
        {
            Duration*=1001;
            Duration/=1000;
        }
        Fill(Stream_Video, 0, Video_Duration, Duration, 0);
    }
    else if (Frame_Count_NotParsedIncluded!=(int64u)-1)
    {
        Fill(Stream_Video, 0, Video_FrameCount, Frame_Count_NotParsedIncluded);
    }

    //Other parsers
    #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        if (GA94_03_Parser && !GA94_03_Parser->Status[IsFinished] && GA94_03_Parser->Status[IsAccepted])
            Finish(GA94_03_Parser);
        if (CC___Parser && !CC___Parser->Status[IsFinished] && CC___Parser->Status[IsAccepted])
            Finish(CC___Parser);
    #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
    #if defined(MEDIAINFO_SCTE20_YES)
        if (Scte_Parser && !Scte_Parser->Status[IsFinished] && Scte_Parser->Status[IsAccepted])
            Finish(Scte_Parser);
    #endif //defined(MEDIAINFO_SCTE20_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        if (DTG1_Parser && !DTG1_Parser->Status[IsFinished] && DTG1_Parser->Status[IsAccepted])
        {
            Finish(DTG1_Parser);
            Merge(*DTG1_Parser, Stream_Video, 0, 0);
        }
        if (GA94_06_Parser && !GA94_06_Parser->Status[IsFinished] && GA94_06_Parser->Status[IsAccepted])
        {
            Finish(GA94_06_Parser);
            Merge(*GA94_06_Parser, Stream_Video, 0, 0);
        }
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
    #if defined(MEDIAINFO_CDP_YES)
        if (Cdp_Parser && !Cdp_Parser->Status[IsFinished] && Cdp_Parser->Status[IsAccepted])
            Finish(Cdp_Parser);
    #endif //defined(MEDIAINFO_CDP_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        if (AfdBarData_Parser && !AfdBarData_Parser->Status[IsFinished] && AfdBarData_Parser->Status[IsAccepted])
        {
            Finish(AfdBarData_Parser);
            Merge(*AfdBarData_Parser, Stream_Video, 0, 0);
        }
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)

    #if MEDIAINFO_IBI
        int64u Numerator=0, Denominator=0;
        switch (frame_rate_code)
        {
            case 1 : Numerator=24000; Denominator=1001; break;
            case 2 : Numerator=24;    Denominator=1;    break;
            case 3 : Numerator=25;    Denominator=1;    break;
            case 4 : Numerator=30000; Denominator=1001; break;
            case 5 : Numerator=30;    Denominator=1;    break;
            case 6 : Numerator=50;    Denominator=1;    break;
            case 7 : Numerator=60000; Denominator=1001; break;
            case 8 : Numerator=60;    Denominator=1;    break;
            default: ;
        }
        if (Numerator)
        {
            Numerator*=frame_rate_extension_n+1;
            Denominator*=frame_rate_extension_d+1;
            Ibi_Stream_Finish(Numerator, Denominator);
        }
    #endif //MEDIAINFO_IBI
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpegv::Synched_Test()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Buffer[Buffer_Offset  ]!=0x00
     || Buffer[Buffer_Offset+1]!=0x00
     || Buffer[Buffer_Offset+2]!=0x01)
    {
        Synched=false;
        return true;
    }

    //Quick search
    if (Synched && !Header_Parser_QuickSearch())
        return false;

    #if MEDIAINFO_IBI
        if (Ibi_SliceParsed)
        {
            if (Buffer[Buffer_Offset+3]==0x00 && Buffer_Offset+5>Buffer_Size)
                return false;
            bool RandomAccess=(Buffer[Buffer_Offset+3]==0x00 && (Buffer[Buffer_Offset+5]&0x38)==0x08) || Buffer[Buffer_Offset+3]==0xB3; //picture_start with I-Frame || sequence_header
            if (RandomAccess)
                Ibi_Add();
            Ibi_SliceParsed=false;
        }
    #endif MEDIAINFO_IBI

    //We continue
    return true;
}

//---------------------------------------------------------------------------
void File_Mpegv::Synched_Init()
{
    //Temp
    BVOP_Count=0;
    progressive_frame_Count=0;
    Interlaced_Top=0;
    Interlaced_Bottom=0;
    display_horizontal_size=0;
    display_vertical_size=0;
    vbv_delay=0;
    vbv_buffer_size_value=0;
    Time_Current_Seconds=Error;
    Time_Current_Frames=(int8u)-1;
    Time_Begin_Seconds=Error;
    Time_Begin_Frames=(int8u)-1;
    Time_End_Seconds=Error;
    Time_End_Frames=(int8u)-1;
    picture_coding_type=(int8u)-1;
    bit_rate_value=0;
    FrameRate=0;
    horizontal_size_value=0;
    vertical_size_value=0;
    bit_rate_extension=0;
    temporal_reference_Old=(int16u)-1;
    aspect_ratio_information=0;
    frame_rate_code=0;
    profile_and_level_indication_profile=(int8u)-1;
    profile_and_level_indication_level=(int8u)-1;
    chroma_format=0;
    horizontal_size_extension=0;
    vertical_size_extension=0;
    frame_rate_extension_n=0;
    frame_rate_extension_d=0;
    video_format=5; //Unspecified video format
    colour_primaries=(int8u)-1;
    transfer_characteristics=(int8u)-1;
    matrix_coefficients=(int8u)-1;
    vbv_buffer_size_extension=0;
    intra_dc_precision=(int8u)-1;
    load_intra_quantiser_matrix=false;
    load_non_intra_quantiser_matrix=false;
    progressive_sequence=true; //progressive by default
    top_field_first=false;
    repeat_first_field=false;
    FirstFieldFound=false;
    group_start_IsParsed=false;
    group_start_FirstPass=false;
    PTS_LastIFrame=(int64u)-1;
    bit_rate_value_IsValid=false;
    profile_and_level_indication_escape=false;
    RefFramesCount=0;
    BVOPsSinceLastRefFrames=0;
    temporal_reference_LastIFrame=0;
    tc=0;
    IFrame_IsParsed=false;
    #if MEDIAINFO_IBI
        Ibi_SliceParsed=true;
    #endif //MEDIAINFO_IBI

    //Default stream values
    Streams.resize(0x100);
    Streams[0xB3].Searching_Payload=true;
    for (int8u Pos=0xFF; Pos>=0xB9; Pos--)
        Streams[Pos].Searching_Payload=true; //Testing MPEG-PS
}

//***************************************************************************
// Buffer - Demux
//***************************************************************************

//---------------------------------------------------------------------------
#if MEDIAINFO_DEMUX
bool File_Mpegv::Demux_UnpacketizeContainer_Test()
{
    if ((Demux_IntermediateItemFound && Buffer[Buffer_Offset+3]==0x00) || Buffer[Buffer_Offset+3]==0xB3)
    {
        if (Demux_Offset==0)
        {
            Demux_Offset=Buffer_Offset;
            Demux_IntermediateItemFound=false;
        }
        while (Demux_Offset+4<=Buffer_Size)
        {
            //Synchronizing
            while(Demux_Offset+3<=Buffer_Size && (Buffer[Demux_Offset  ]!=0x00
                                                || Buffer[Demux_Offset+1]!=0x00
                                                || Buffer[Demux_Offset+2]!=0x01))
            {
                Demux_Offset+=2;
                while(Demux_Offset<Buffer_Size && Buffer[Buffer_Offset]!=0x00)
                    Demux_Offset+=2;
                if (Demux_Offset<Buffer_Size && Buffer[Demux_Offset-1]==0x00 || Demux_Offset>=Buffer_Size)
                    Demux_Offset--;
            }

            if (Demux_Offset+4<=Buffer_Size)
            {
                if (Demux_IntermediateItemFound)
                {
                    bool MustBreak;
                    switch (Buffer[Demux_Offset+3])
                    {
                        case 0x00 :
                        case 0xB3 :
                                    MustBreak=true; break;
                        default   : MustBreak=false;
                    }
                    if (MustBreak)
                        break; //while() loop
                }
                else
                {
                    if (!Buffer[Demux_Offset+3])
                        Demux_IntermediateItemFound=true;
                }
            }
            Demux_Offset++;
        }

        if (Demux_Offset+4>Buffer_Size && File_Offset+Buffer_Size!=File_Size)
            return false; //No complete frame

        bool RandomAccess=Buffer[Buffer_Offset+3]==0xB3;
        if (!Status[IsAccepted])
        {
            Accept("AVC");
            if (Config->Demux_EventWasSent)
                return false;
        }
        if (IFrame_IsParsed || RandomAccess)
            Demux_UnpacketizeContainer_Demux(RandomAccess);
        else
            Demux_UnpacketizeContainer_Demux_Clear();
    }

    return true;
}
#endif //MEDIAINFO_DEMUX

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Unsynched()
{
    for (int8u Pos=0x00; Pos<0xB9; Pos++)
    {
        Streams[Pos].Searching_Payload=false;
        Streams[Pos].Searching_TimeStamp_Start=false;
        Streams[Pos].Searching_TimeStamp_End=false;
    }
    Streams[0xB3].Searching_TimeStamp_End=true; //sequence_header
    Streams[0xB8].Searching_TimeStamp_End=true; //group_start
    Time_End_Seconds=Error;
    Time_End_Frames=(int8u)-1;
    RefFramesCount=0;
    sequence_header_IsParsed=false;
    group_start_IsParsed=false;
    PTS_LastIFrame=(int64u)-1;
    IFrame_IsParsed=false;
    #if MEDIAINFO_IBI
        Ibi_SliceParsed=true;
    #endif //MEDIAINFO_IBI

    temporal_reference_Old=(int16u)-1;
    for (size_t Pos=0; Pos<TemporalReference.size(); Pos++)
        delete TemporalReference[Pos]; //TemporalReference[Pos]=NULL;
    TemporalReference.clear();
    TemporalReference_Offset=0;
    #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        GA94_03_TemporalReference_Offset=0;
        if (GA94_03_Parser)
            GA94_03_Parser->Open_Buffer_Unsynch();
        if (CC___Parser)
            CC___Parser->Open_Buffer_Unsynch();
    #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
    #if defined(MEDIAINFO_SCTE20_YES)
        Scte_TemporalReference_Offset=0;
        if (Scte_Parser)
            Scte_Parser->Open_Buffer_Unsynch();
    #endif //defined(MEDIAINFO_SCTE20_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        if (DTG1_Parser)
            DTG1_Parser->Open_Buffer_Unsynch();
        if (GA94_06_Parser)
            GA94_06_Parser->Open_Buffer_Unsynch();
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
    #if defined(MEDIAINFO_CDP_YES)
        if (Cdp_Parser)
            Cdp_Parser->Open_Buffer_Unsynch();
    #endif //defined(MEDIAINFO_CDP_YES)
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        if (AfdBarData_Parser)
            AfdBarData_Parser->Open_Buffer_Unsynch();
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)

    #if defined(MEDIAINFO_ANCILLARY_YES)
        if (Ancillary && *Ancillary && (*Ancillary)->Cdp_Data.empty())
            (*Ancillary)->AspectRatio=0;
    #endif //defined(MEDIAINFO_ANCILLARY_YES)

    //NextCode
    if (!Status[IsAccepted])
    {
        NextCode_Clear();
        NextCode_Add(0xB3);
        NextCode_Add(0xB8);
    }
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Header_Parse()
{
    #if MEDIAINFO_TRACE
    if (Trace_Activated)
    {
        //Parsing
        int8u start_code;
        Skip_B3(                                                    "synchro");
        Get_B1 (start_code,                                         "start_code");
        if (!Header_Parser_Fill_Size())
        {
            Element_WaitForMoreData();
            return;
        }

        //Filling
        Header_Fill_Code(start_code, Ztring().From_CC1(start_code));
    }
    else
    {
    #endif //MEDIAINFO_TRACE
        //Parsing
        int8u start_code=Buffer[Buffer_Offset+3];
        Element_Offset+=4;
        if (!Header_Parser_Fill_Size())
        {
            Element_WaitForMoreData();
            return;
        }

        //Filling
        Header_Fill_Code(start_code, Ztring().From_CC1(start_code));
    #if MEDIAINFO_TRACE
    }
    #endif //MEDIAINFO_TRACE
}

//---------------------------------------------------------------------------
bool File_Mpegv::Header_Parser_Fill_Size()
{
    //Look for next Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if Header_Parse_Fill_Size() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset+4;
    while (Buffer_Offset_Temp+4<=Buffer_Size
        && CC3(Buffer+Buffer_Offset_Temp)!=0x000001)
    {
        Buffer_Offset_Temp+=2;
        while(Buffer_Offset_Temp<Buffer_Size && Buffer[Buffer_Offset_Temp]!=0x00)
            Buffer_Offset_Temp+=2;
        if (Buffer_Offset_Temp<Buffer_Size && Buffer[Buffer_Offset_Temp-1]==0x00 || Buffer_Offset_Temp>=Buffer_Size)
            Buffer_Offset_Temp--;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (FrameIsAlwaysComplete || File_Offset+Buffer_Size==File_Size)
            Buffer_Offset_Temp=Buffer_Size; //We are sure that the next bytes are a start
        else
            return false;
    }

    //OK, we continue
    Header_Fill_Size(Buffer_Offset_Temp-Buffer_Offset);
    Buffer_Offset_Temp=0;
    return true;
}

//---------------------------------------------------------------------------
bool File_Mpegv::Header_Parser_QuickSearch()
{
    while (       Buffer_Offset+4<=Buffer_Size
      &&   Buffer[Buffer_Offset  ]==0x00
      &&   Buffer[Buffer_Offset+1]==0x00
      &&   Buffer[Buffer_Offset+2]==0x01)
    {
        //Getting start_code
        int8u start_code=Buffer[Buffer_Offset+3];

        //Searching start or timestamp
        if (Streams[start_code].Searching_Payload
         || Streams[start_code].Searching_TimeStamp_Start
         || Streams[start_code].Searching_TimeStamp_End)
            return true;

        //Synchronizing
        Buffer_Offset+=4;
        Synched=false;
        if (!Synchronize())
        {
            if (File_Offset+Buffer_Size==File_Size)
            {
                Synched=true;
                return true;
            }
            return false;
        }

        if (Buffer_Offset+4>Buffer_Size)
            return false;
    }

    Trusted_IsNot("MPEG Video, Synchronisation lost");
    return Synchronize();
}

//---------------------------------------------------------------------------
void File_Mpegv::Data_Parse()
{
    //Parsing
    switch (Element_Code)
    {
        case 0x00: picture_start(); break;
        case 0xB0: Skip_XX(Element_Size,                        "Unknown"); break;
        case 0xB1: Skip_XX(Element_Size,                        "Unknown"); break;
        case 0xB2: user_data_start(); break;
        case 0xB3: sequence_header(); break;
        case 0xB4: sequence_error(); break;
        case 0xB5: extension_start(); break;
        case 0xB6: Skip_XX(Element_Size,                        "Unknown"); break;
        case 0xB7: sequence_end(); break;
        case 0xB8: group_start(); break;
        default:
            if (Element_Code>=0x01
             && Element_Code<=0xAF) slice_start();
            else
                Trusted_IsNot("Unattended element");
    }
}

//***************************************************************************
// EOF
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Detect_EOF()
{
    if (IsSub && Status[IsFilled]
     || (!IsSub && File_Size>SizeToAnalyse_Begin+SizeToAnalyse_End && File_Offset+Buffer_Offset+Element_Offset>SizeToAnalyse_Begin && File_Offset+Buffer_Offset+Element_Offset<File_Size-SizeToAnalyse_End && Config_ParseSpeed<=0.5))
    {
        if (MustExtendParsingDuration && Frame_Count<Frame_Count_Valid*10 //10 times the normal test
         && !(!IsSub && File_Size>SizeToAnalyse_Begin*10+SizeToAnalyse_End*10 && File_Offset+Buffer_Offset+Element_Offset>SizeToAnalyse_Begin*10 && File_Offset+Buffer_Offset+Element_Offset<File_Size-SizeToAnalyse_End*10))
        {
            Streams[0x00].Searching_Payload=GA94_03_IsPresent || Cdp_IsPresent;
            Streams[0xB2].Searching_Payload=GA94_03_IsPresent || CC___IsPresent || Scte_IsPresent;
            Streams[0xB3].Searching_Payload=GA94_03_IsPresent || Cdp_IsPresent;
            return;
        }

        //
        Time_End_Seconds=Error;
        Time_End_Frames=(int8u)-1;

        //Autorisation of other streams
        if (!IsSub)
            Streams[0x00].Searching_TimeStamp_End=false;

        //Jumping
        if (!Status[IsFilled])
            Fill("MPEG Video");

        if (!IsSub)
            Open_Buffer_Unsynch();
        GoToFromEnd(SizeToAnalyse_End, "MPEG Video");
        EOF_AlreadyDetected=true; //Sometimes called from Filling
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "00"
void File_Mpegv::picture_start()
{
    Element_Name("picture_start");

    //Coherency
    if (!Status[IsAccepted])
    {
        if (!NextCode_Test())
            return;
    }

    //Parsing
    int8u picture_coding_type_Old=picture_coding_type;
    #if MEDIAINFO_TRACE
    if (Trace_Activated)
    {
        //Parsing
        BS_Begin();
        Get_S2 (10, temporal_reference,                         "temporal_reference");
        Get_S1 ( 3, picture_coding_type,                        "picture_coding_type"); Param_Info(Mpegv_picture_coding_type[picture_coding_type]);
        Get_S2 (16, vbv_delay,                                  "vbv_delay");
        if (picture_coding_type==2 || picture_coding_type==3) //P or B
        {
            Skip_S1(1,                                          "full_pel_forward_vector");
            Skip_S1(3,                                          "forward_f_code");
        }
        if (picture_coding_type==3) //B
        {
            Skip_S1(1,                                          "full_pel_backward_vector");
            Skip_S1(3,                                          "backward_f_code");
        }
        bool extra_bit_picture;
        do
        {
            Peek_SB(extra_bit_picture);
            if (extra_bit_picture)
            {
                Skip_S1(1,                                      "extra_bit_picture");
                Skip_S1(8,                                      "extra_information_picture");
            }
        }
        while (extra_bit_picture);
        BS_End();
    }
    else
    {
    #endif //MEDIAINFO_TRACE
        //Parsing
        size_t Buffer_Pos=Buffer_Offset+(size_t)Element_Offset;
        temporal_reference      =(Buffer[Buffer_Pos]<<2) | (Buffer[Buffer_Pos+1]>>6);
        picture_coding_type     =(Buffer[Buffer_Pos+1]>>3)&0x07;
        vbv_delay               =(Buffer[Buffer_Pos+1]<<13) | (Buffer[Buffer_Pos+2]<<5) | (Buffer[Buffer_Pos+3]>>3);
        Element_Offset=Element_Size;
    #if MEDIAINFO_TRACE
    }
    #endif //MEDIAINFO_TRACE

    FILLING_BEGIN();
        #if MEDIAINFO_EVENTS
            {
                struct MediaInfo_Event_Video_SliceInfo_0 Event;
                Event.EventCode=MediaInfo_EventCode_Create(MediaInfo_Parser_None, MediaInfo_Event_Video_SliceInfo, 0);
                Event.Stream_Offset=File_Offset+Buffer_Offset;
                Event.PCR=FrameInfo.PCR;
                Event.PTS=FrameInfo.PTS;
                Event.DTS=FrameInfo.DTS;
                Event.DUR=FrameInfo.DUR;
                Event.StreamIDs_Size=StreamIDs_Size;
                Event.StreamIDs=(MediaInfo_int64u*)StreamIDs;
                Event.StreamIDs_Width=(MediaInfo_int8u*)StreamIDs_Width;
                Event.ParserIDs=(MediaInfo_int8u* )ParserIDs;
                Event.FramePosition=Frame_Count;
                Event.FieldPosition=Field_Count;
                Event.SlicePosition=0;
                switch (picture_coding_type)
                {
                    case 1 :
                                Event.SliceType=0; break;
                    case 2 :
                                Event.SliceType=1; break;
                    case 3 :
                                Event.SliceType=2; break;
                    default:
                                Event.SliceType=(int8u)-1;
                }
                Event.Flags=0;
                Config->Event_Send((const int8u*)&Event, sizeof(MediaInfo_Event_Video_SliceInfo_0));
            }
        #endif MEDIAINFO_EVENTS

        //Detection of first I-Frame
        if (!IFrame_IsParsed)
        {
            if (picture_coding_type==1 || picture_coding_type==4) //I-Frame or D-Frame
                IFrame_IsParsed=true;
        }

        //Config
        progressive_frame=true;
        picture_structure=3; //Frame is default

        //Temporal reference
        if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
            TemporalReference.resize(TemporalReference_Offset+temporal_reference+1);
        if (TemporalReference[TemporalReference_Offset+temporal_reference]==NULL)
            TemporalReference[TemporalReference_Offset+temporal_reference]=new temporalreference;
        TemporalReference[TemporalReference_Offset+temporal_reference]->IsValid=true;

        //Detecting streams with only I-Frames
        if (picture_coding_type==1 && picture_coding_type_Old==1)
            temporal_reference_Old=(int16u)-1; //Resetting temporal_reference_Old

        //NextCode
        if (!Status[IsAccepted])
        {
            NextCode_Clear();
            for (int64u Element_Name_Next=0x01; Element_Name_Next<=0x1F; Element_Name_Next++)
                NextCode_Add(Element_Name_Next);
            NextCode_Add(0xB2);
            NextCode_Add(0xB5);
            NextCode_Add(0xB8);
        }

        //Autorisation of other streams
        for (int8u Pos=0x01; Pos<=0x1F; Pos++)
            Streams[Pos].Searching_Payload=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "01" --> "AF"
void File_Mpegv::slice_start()
{
    if (!Status[IsAccepted])
    {
        if (!NextCode_Test())
            return;
    }
    Element_Name("slice_start");

    //Parsing
    Skip_XX(Element_Size,                                       "data");

    FILLING_BEGIN();

        //Timestamp
        if (group_start_FirstPass && (Time_Begin_Seconds==Error || Time_Current_Seconds*FrameRate+Time_Current_Frames+temporal_reference<Time_Begin_Seconds*FrameRate+Time_Begin_Frames))
        {
            Time_Begin_Seconds=Time_Current_Seconds;
            Time_Begin_Frames =Time_Current_Frames+(int8u)temporal_reference;
        }
        if (!TimeCodeIsNotTrustable && (picture_coding_type==1 || picture_coding_type==2)) //IFrame or PFrame
        {
            Time_End_Seconds=Time_Current_Seconds;
            Time_End_Frames =Time_Current_Frames+(int8u)temporal_reference;
        }

        if (temporal_reference==temporal_reference_Old)
        {
            Frame_Count--;
            if (IFrame_IsParsed && Frame_Count_NotParsedIncluded!=(int64u)-1)
                Frame_Count_NotParsedIncluded--;
            Frame_Count_InThisBlock--;
        }
        else
        {
            temporal_reference_Old=temporal_reference;
        }

        if (picture_coding_type==1) //IFrame
        {
            temporal_reference_LastIFrame=temporal_reference;
            PTS_LastIFrame=FrameInfo.PTS;
        }
        if (PTS_LastIFrame!=(int64u)-1)
        {
            FrameInfo.PTS=PTS_LastIFrame+(temporal_reference-temporal_reference_LastIFrame)*tc;
            if (PTS_Begin==(int64u)-1 && picture_coding_type==1) //IFrame
                PTS_Begin=FrameInfo.PTS;
        }

        //Info
        #if MEDIAINFO_TRACE
            if (Trace_Activated)
            {
                Element_Info(_T("Frame ")+Ztring::ToZtring(Frame_Count));
                Element_Info(_T("picture_coding_type ")+Ztring().From_Local(Mpegv_picture_coding_type[picture_coding_type]));
                Element_Info(_T("temporal_reference ")+Ztring::ToZtring(temporal_reference));
                if (FrameInfo.PTS!=(int64u)-1)
                    Element_Info(_T("PTS ")+Ztring().Duration_From_Milliseconds(float64_int64s(((float64)FrameInfo.PTS)/1000000)));
                if (FrameInfo.DTS!=(int64u)-1)
                    Element_Info(_T("DTS ")+Ztring().Duration_From_Milliseconds(float64_int64s(((float64)FrameInfo.DTS)/1000000)));
                if (Time_End_Seconds!=Error)
                {
                    int32u Time_End  =Time_End_Seconds  *1000;
                    if (FrameRate)
                        Time_End  +=(int32u)float32_int32s((Time_Current_Frames+temporal_reference)*1000/FrameRate);
                    size_t Hours  = Time_End/60/60/1000;
                    size_t Minutes=(Time_End-(Hours*60*60*1000))/60/1000;
                    size_t Seconds=(Time_End-(Hours*60*60*1000)-(Minutes*60*1000))/1000;
                    size_t Milli  =(Time_End-(Hours*60*60*1000)-(Minutes*60*1000)-(Seconds*1000));

                    Ztring Time;
                    Time+=Ztring::ToZtring(Hours);
                    Time+=_T(':');
                    Time+=Ztring::ToZtring(Minutes);
                    Time+=_T(':');
                    Time+=Ztring::ToZtring(Seconds);
                    if (FrameRate!=0)
                    {
                        Time+=_T('.');
                        Time+=Ztring::ToZtring(Milli);
                    }
                    Element_Info(_T("time_code ")+Time);
                }
            }
        #endif //MEDIAINFO_TRACE

        //CDP
        #if defined(MEDIAINFO_CDP_YES)
            if (Ancillary)
             {
                Cdp_IsPresent=true;
                MustExtendParsingDuration=true;
                Buffer_TotalBytes_Fill_Max=(int64u)-1; //Disabling this feature for this format, this is done in the parser

                Element_Begin("CDP");
                if ((*Ancillary)==NULL)
                    (*Ancillary)=new File_Ancillary();
                (*Ancillary)->AspectRatio=MPEG_Version==1?Mpegv_aspect_ratio1[aspect_ratio_information]:Mpegv_aspect_ratio2[aspect_ratio_information];
                (*Ancillary)->FrameRate=((float)(Mpegv_frame_rate[frame_rate_code] * (frame_rate_extension_n + 1)) / (float)(frame_rate_extension_d + 1));
                if ((*Ancillary)->Status[IsAccepted]) //In order to test if there is a parser using ancillary data
                    Open_Buffer_Continue((*Ancillary), Buffer+Buffer_Offset, 0);
                Element_End();
            }
        #endif //defined(MEDIAINFO_CDP_YES)

        //Active Format Description & Bar Data
        #if defined(MEDIAINFO_AFDBARDATA_YES)
            if (Ancillary && *Ancillary && !(*Ancillary)->AfdBarData_Data.empty())
            {
                Element_Begin("Active Format Description & Bar Data");

                //Parsing
                if (AfdBarData_Parser==NULL)
                {
                    AfdBarData_Parser=new File_AfdBarData;
                    Open_Buffer_Init(AfdBarData_Parser);
                    ((File_AfdBarData*)AfdBarData_Parser)->Format=File_AfdBarData::Format_S2016_3;
                }
                if (AfdBarData_Parser->PTS_DTS_Needed)
                    AfdBarData_Parser->FrameInfo.DTS=FrameInfo.DTS;
                if (!AfdBarData_Parser->Status[IsFinished])
                    Open_Buffer_Continue(AfdBarData_Parser, (*Ancillary)->AfdBarData_Data[0]->Data, (*Ancillary)->AfdBarData_Data[0]->Size);

                //Removing data from stack
                delete (*Ancillary)->AfdBarData_Data[0]; //(*Ancillary)->AfdBarData_Data[0]=NULL;
                (*Ancillary)->AfdBarData_Data.erase((*Ancillary)->AfdBarData_Data.begin());

                Element_End();
            }
        #endif //defined(MEDIAINFO_AFDBARDATA_YES)

        //Counting
        if (File_Offset+Buffer_Offset+Element_Size==File_Size)
            Frame_Count_Valid=Frame_Count; //Finish frames in case of there are less than Frame_Count_Valid frames
        if (!TimeCodeIsNotTrustable && (picture_coding_type==1 || picture_coding_type==2)) //IFrame or PFrame
        {
            Time_End_Frames++; //One frame
            if (progressive_sequence && repeat_first_field)
            {
                Time_End_Frames++; //Frame repeated a second time
                if (top_field_first)
                    Time_End_Frames++; //Frame repeated a third time
            }
        }
        Frame_Count++;
        Frame_Count_InThisBlock++;
        if (IFrame_IsParsed && Frame_Count_NotParsedIncluded!=(int64u)-1)
            Frame_Count_NotParsedIncluded++;
        if (!(progressive_sequence || picture_structure==3))
        {
            Field_Count++;
            Field_Count_InThisBlock++;
        }
        if (picture_coding_type==3)
            BVOP_Count++;
        else
            BVOPsSinceLastRefFrames=0;
        if (RefFramesCount<2 && (picture_coding_type==1 || picture_coding_type==2))
            RefFramesCount++;
        int64u tc_ToAdd=tc/((progressive_sequence || picture_structure==3)?1:2); //Progressive of Frame
        if (repeat_first_field)
        {
            if (progressive_sequence)
            {
                tc_ToAdd+=tc;
                if (top_field_first)
                    tc_ToAdd+=tc;
            }
            else
                tc_ToAdd+=tc/2;
        }
        if (FrameInfo.DTS!=(int64u)-1)
        {
            FrameInfo.DTS+=tc_ToAdd;
            if (DTS_End<FrameInfo.DTS)
                DTS_End=FrameInfo.DTS;
        }
        if (FrameInfo.PTS!=(int64u)-1)
        {
            FrameInfo.PTS+=tc_ToAdd;
            if (PTS_End<FrameInfo.PTS)
                PTS_End=FrameInfo.PTS;
        }

        //NextCode
        if (!Status[IsAccepted])
        {
            NextCode_Clear();
            NextCode_Add(0x00);
            NextCode_Add(0xB3);
            NextCode_Add(0xB8);
        }

        //Autorisation of other streams
        for (int8u Pos=0x01; Pos<=0x1F; Pos++)
            Streams[Pos].Searching_Payload=false;

        //Filling only if not already done
        if (!Status[IsAccepted])
            Accept("MPEG Video");
        if (!Status[IsFilled] && (!MustExtendParsingDuration && Frame_Count>=Frame_Count_Valid || Frame_Count>=Frame_Count_Valid*10))
        {
            Fill("MPEG Video");
            if (File_Size==(int64u)-1)
                Finish("MPEG Video");
            else if (!IsSub && 2*(File_Offset+Buffer_Size+SizeToAnalyse_End)<File_Size && Config_ParseSpeed<1.0)
            {
                Open_Buffer_Unsynch();
                GoToFromEnd(SizeToAnalyse_End);
            }
        }

        //Skipping slices (if already unpacketized)
        #if MEDIAINFO_DEMUX
            if (Demux_UnpacketizeContainer && Buffer_TotalBytes+Buffer_Offset<Demux_TotalBytes)
            {
                Element_Offset=Demux_TotalBytes-(Buffer_TotalBytes+Buffer_Offset);
            }
        #endif //MEDIAINFO_DEMUX

        #if MEDIAINFO_IBI
            Ibi_SliceParsed=true;
        #endif MEDIAINFO_IBI
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B2"
void File_Mpegv::user_data_start()
{
    Element_Name("user_data_start");

    //GA94 stuff
    if (Element_Size>=4)
    {
        int32u GA94_Identifier;
        Peek_B4(GA94_Identifier);
        switch (GA94_Identifier)
        {
            case 0x434301F8 :   user_data_start_CC(); return;
            case 0x44544731 :   user_data_start_DTG1(); return;
            case 0x47413934 :   user_data_start_GA94(); return;
            default         :   {
                                int8u SCTE20_Identifier;
                                Peek_B1(SCTE20_Identifier);
                                if (SCTE20_Identifier==0x03)
                                {
                                    user_data_start_3();
                                    return;
                                }
                                }
        }
    }

    //Rejecting junk at the begin
    size_t Library_Start_Offset=0;
    while (Library_Start_Offset+4<=Element_Size)
    {
        bool OK=true;
        for (size_t Pos=0; Pos<4; Pos++)
        {
            if (!((Buffer[Buffer_Offset+Library_Start_Offset+Pos]==0x20 && Pos)
               ||  Buffer[Buffer_Offset+Library_Start_Offset+Pos]==0x22
               ||  Buffer[Buffer_Offset+Library_Start_Offset+Pos]==0x27
               ||  Buffer[Buffer_Offset+Library_Start_Offset+Pos]==0x28
               || (Buffer[Buffer_Offset+Library_Start_Offset+Pos]==0x29 && Pos)
               || (Buffer[Buffer_Offset+Library_Start_Offset+Pos]>=0x30
               &&  Buffer[Buffer_Offset+Library_Start_Offset+Pos]<=0x3F)
               || (Buffer[Buffer_Offset+Library_Start_Offset+Pos]>=0x41
               && Buffer[Buffer_Offset+Library_Start_Offset+Pos]<=0x7D)))
            {
                OK=false;
                break;
            }
        }
        if (OK)
            break;
        Library_Start_Offset++;
    }
    if (Library_Start_Offset+4>Element_Size)
    {
        Skip_XX(Element_Size,                                   "junk");
        return; //No good info
    }

    //Accepting good data after junk
    size_t Library_End_Offset=Library_Start_Offset+4;
    while (Library_End_Offset<Element_Size
        && (Buffer[Buffer_Offset+Library_End_Offset]==0x0D
         || Buffer[Buffer_Offset+Library_End_Offset]==0x0A
         || Buffer[Buffer_Offset+Library_End_Offset]>=0x20
         && Buffer[Buffer_Offset+Library_End_Offset]<=0x3F
         || Buffer[Buffer_Offset+Library_End_Offset]>=0x41
         && Buffer[Buffer_Offset+Library_End_Offset]<=0x7D))
        Library_End_Offset++;

    //Parsing
    Ztring Temp;
    if (Library_Start_Offset>0)
        Skip_XX(Library_Start_Offset,                           "junk");
    if (Library_End_Offset-Library_Start_Offset)
        Get_Local(Library_End_Offset-Library_Start_Offset, Temp,"data");
    if (Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "junk");

    //Cleanup
    while(Temp.size()>3 && Temp[1]==_T('e') && Temp[2]==_T('n') && Temp[3]==_T('c'))
        Temp.erase(0, 1);
    while(Temp.size()>5 && Temp[3]==_T('M') && Temp[4]==_T('P') && Temp[5]==_T('E'))
        Temp.erase(0, 1);

    //Cleanup
    while(!Temp.empty() && Temp[0]==_T('0'))
        Temp.erase(0, 1);

    FILLING_BEGIN();
        if (!Temp.empty())
        {
            if (Temp.find(_T("build"))==0)
                Library+=Ztring(_T(" "))+Temp;
            else
                Library=Temp;

            //Library
            if (Temp.find(_T("Created with Nero"))==0)
            {
                Library_Name=_T("Ahead Nero");
            }
            else if (Library.find(_T("encoded by avi2mpg1 ver "))==0)
            {
                Library_Name=_T("avi2mpg1");
                Library_Version=Library.SubString(_T("encoded by avi2mpg1 ver "), _T(""));
            }
            else if (Library.find(_T("encoded by TMPGEnc (ver. "))==0)
            {
                Library_Name=_T("TMPGEnc");
                Library_Version=Library.SubString(_T("encoded by TMPGEnc (ver. "), _T(")"));
            }
            else if (Library.find(_T("encoded by TMPGEnc 4.0 XPress Version. "))==0)
            {
                Library_Name=_T("TMPGEnc XPress");
                Library_Version=Library.SubString(_T("encoded by TMPGEnc 4.0 XPress Version. "), _T(""));
            }
            else if (Library.find(_T("encoded by TMPGEnc MPEG Editor "))==0)
            {
                Library_Name=_T("TMPGEnc MPEG Editor");
                Library_Version=Library.SubString(_T("Version. "), _T(""));
            }
            else if (Library.find(_T("encoded by TMPGEnc "))==0)
            {
                Library_Name=_T("TMPGEnc");
                Library_Version=Library.SubString(_T("encoded by TMPGEnc "), _T(""));
            }
            else if (Library.find(_T("MPEG Encoder v"))==0)
            {
                Library_Name=_T("MPEG Encoder by Tristan Savatier");
                Library_Version=Library.SubString(_T("MPEG Encoder v"), _T(" by"));
            }
            else
                Library_Name=Library;

        }
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B2", CC (From DVD)
void File_Mpegv::user_data_start_CC()
{
    Skip_B4(                                                    "identifier");

    #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        Element_Info("DVD Captions");

        //Parsing
        #if MEDIAINFO_DEMUX
            Element_Code=0x434301F800000000LL;
        #endif //MEDIAINFO_DEMUX
        if (CC___Parser==NULL)
        {
            CC___IsPresent=true;
            MustExtendParsingDuration=true;
            Buffer_TotalBytes_Fill_Max=(int64u)-1; //Disabling this feature for this format, this is done in the parser
            CC___Parser=new File_DtvccTransport;
            Open_Buffer_Init(CC___Parser);
            ((File_DtvccTransport*)CC___Parser)->Format=File_DtvccTransport::Format_DVD;
        }
        if (CC___Parser->PTS_DTS_Needed)
        {
            CC___Parser->FrameInfo.PCR=FrameInfo.PCR;
            CC___Parser->FrameInfo.PTS=FrameInfo.PTS;
            CC___Parser->FrameInfo.DTS=FrameInfo.DTS;
        }
        Demux(Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset), ContentType_MainStream);
        Open_Buffer_Continue(CC___Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Element_Offset=Element_Size;
    #else //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        Skip_XX(Element_Size-Element_Offset,                    "DVD Captions");
    #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
}

//---------------------------------------------------------------------------
// Packet "B2", 0x03 (SCTE20)
void File_Mpegv::user_data_start_3()
{
    Skip_B1(                                                    "identifier");

    #if defined(MEDIAINFO_SCTE20_YES)
        Scte_IsPresent=true;
        MustExtendParsingDuration=true;
        Buffer_TotalBytes_Fill_Max=(int64u)-1; //Disabling this feature for this format, this is done in the parser

        Element_Info("SCTE 20");

        //Coherency
        if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
            return;

        //Purging too old orphelins
        if (Scte_TemporalReference_Offset+8<TemporalReference_Offset+temporal_reference)
        {
            size_t Pos=TemporalReference_Offset+temporal_reference;
            do
            {
                if (TemporalReference[Pos]==NULL || !TemporalReference[Pos]->IsValid)
                    break;
                Pos--;
            }
            while (Pos>0);
            Scte_TemporalReference_Offset=Pos+1;
        }

        if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
            TemporalReference.resize(TemporalReference_Offset+temporal_reference+1);
        if (TemporalReference[TemporalReference_Offset+temporal_reference]==NULL)
            TemporalReference[TemporalReference_Offset+temporal_reference]=new temporalreference;
        temporalreference::buffer_data* BufferData=new temporalreference::buffer_data;
        BufferData->Size=(size_t)(Element_Size-Element_Offset);
        BufferData->Data=new int8u[(size_t)(Element_Size-Element_Offset)];
        std::memcpy(BufferData->Data, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        TemporalReference[TemporalReference_Offset+temporal_reference]->Scte.push_back(BufferData);
        TemporalReference[TemporalReference_Offset+temporal_reference]->Scte_Parsed.push_back(false);
        if (TemporalReference[TemporalReference_Offset+temporal_reference]->Scte_Parsed.size()>=2 && TemporalReference[TemporalReference_Offset+temporal_reference]->Scte_Parsed[TemporalReference[TemporalReference_Offset+temporal_reference]->Scte_Parsed.size()-2] && Scte_TemporalReference_Offset==TemporalReference_Offset+temporal_reference+1)
            Scte_TemporalReference_Offset--;

        //Parsing
        Skip_XX(Element_Size-Element_Offset,                    "SCTE 20 data");

        //Parsing Captions after reordering
        bool CanBeParsed=true;
        size_t FirstSize=(size_t)-1;
        for (size_t Scte20_Pos=Scte_TemporalReference_Offset; Scte20_Pos<TemporalReference.size(); Scte20_Pos++)
        {
            if (TemporalReference[Scte20_Pos]==NULL || !TemporalReference[Scte20_Pos]->IsValid || TemporalReference[Scte20_Pos]->Scte.empty())
                CanBeParsed=false; //There is a missing field/frame
            if (CanBeParsed)
            {
                if (FirstSize==(size_t)-1)
                    FirstSize=TemporalReference[Scte20_Pos]->Scte.size();
                else if (!progressive_sequence && !TemporalReference[Scte20_Pos]->progressive_frame && TemporalReference[Scte20_Pos]->picture_structure==3 && TemporalReference[Scte20_Pos]->Scte.size()!=FirstSize)
                    CanBeParsed=false; //There is a missing field in a frame duo
            }
        }
        if (CanBeParsed)
        {
            for (size_t Scte20_Pos=Scte_TemporalReference_Offset; Scte20_Pos<TemporalReference.size(); Scte20_Pos++)
            {
                Element_Begin("Reordered SCTE 20");

                //Parsing
                #if MEDIAINFO_DEMUX
                    Element_Code=0x0000000300000000LL;
                #endif //MEDIAINFO_DEMUX
                if (Scte_Parser==NULL)
                {
                    Scte_Parser=new File_Scte20;
                    Open_Buffer_Init(Scte_Parser);
                }
                //((File_Scte20*)Scte_Parser)->AspectRatio=MPEG_Version==1?Mpegv_aspect_ratio1[aspect_ratio_information]:Mpegv_aspect_ratio2[aspect_ratio_information];
                ((File_Scte20*)Scte_Parser)->picture_structure=TemporalReference[Scte20_Pos]->picture_structure;
                ((File_Scte20*)Scte_Parser)->progressive_sequence=progressive_sequence;
                ((File_Scte20*)Scte_Parser)->progressive_frame=TemporalReference[Scte20_Pos]->progressive_frame;
                ((File_Scte20*)Scte_Parser)->top_field_first=TemporalReference[Scte20_Pos]->top_field_first;
                ((File_Scte20*)Scte_Parser)->repeat_first_field=TemporalReference[Scte20_Pos]->repeat_first_field;
                for (size_t Pos=0; Pos<TemporalReference[Scte20_Pos]->Scte.size(); Pos++)
                    if (!TemporalReference[Scte20_Pos]->Scte_Parsed[Pos])
                    {
                        if (Scte_Parser->PTS_DTS_Needed)
                        {
                            Scte_Parser->FrameInfo.PCR=FrameInfo.PCR;
                            Scte_Parser->FrameInfo.PTS=FrameInfo.PTS-(TemporalReference.size()-1-Scte20_Pos)*tc;
                            Scte_Parser->FrameInfo.DTS=FrameInfo.DTS-(TemporalReference.size()-1-Scte20_Pos)*tc;
                        }
                        Demux(TemporalReference[Scte20_Pos]->Scte[Pos]->Data, TemporalReference[Scte20_Pos]->Scte[Pos]->Size, ContentType_MainStream);
                        Open_Buffer_Continue(Scte_Parser, TemporalReference[Scte20_Pos]->Scte[Pos]->Data, TemporalReference[Scte20_Pos]->Scte[Pos]->Size);
                        TemporalReference[Scte20_Pos]->Scte_Parsed[Pos]=true;
                    }

                Element_End();
            }
            Scte_TemporalReference_Offset=TemporalReference.size();
        }
    #else //defined(MEDIAINFO_SCTE20_YES)
        Skip_XX(Element_Size-Element_Offset,                    "SCTE 20 data");
    #endif //defined(MEDIAINFO_SCTE20_YES)
}

//---------------------------------------------------------------------------
// Packet "B2", DTG1
void File_Mpegv::user_data_start_DTG1()
{
    Skip_B4(                                                    "identifier");

    #if defined(MEDIAINFO_AFDBARDATA_YES)
        Element_Info("Active Format Description");

        //Parsing
        if (DTG1_Parser==NULL)
        {
            DTG1_Parser=new File_AfdBarData;
            Open_Buffer_Init(DTG1_Parser);
            ((File_AfdBarData*)DTG1_Parser)->Format=File_AfdBarData::Format_A53_4_DTG1;

            //Aspect ratio for AFD
            float32 DAR=0;
            if (MPEG_Version==2)
            {
                if (aspect_ratio_information==0)
                    ; //Forbidden
                else if (aspect_ratio_information==1)
                    DAR=((float32)(0x1000*horizontal_size_extension+horizontal_size_value))/(0x1000*vertical_size_extension+vertical_size_value);
                else if (display_horizontal_size && display_vertical_size)
                {
                    if (vertical_size_value && Mpegv_aspect_ratio2[aspect_ratio_information])
                        DAR=((float32)(0x1000*horizontal_size_extension+horizontal_size_value))/(0x1000*vertical_size_extension+vertical_size_value)
                                                                                     *Mpegv_aspect_ratio2[aspect_ratio_information]/((float32)display_horizontal_size/display_vertical_size);
                }
                else if (Mpegv_aspect_ratio2[aspect_ratio_information])
                    DAR=Mpegv_aspect_ratio2[aspect_ratio_information];
            }
            else //Version 1
            {
                if (vertical_size_value && Mpegv_aspect_ratio1[aspect_ratio_information])
                    DAR=((float32)(0x1000*horizontal_size_extension+horizontal_size_value))/(0x1000*vertical_size_extension+vertical_size_value)/Mpegv_aspect_ratio1[aspect_ratio_information];
            }
            if (DAR>=1.330 && DAR<1.336) ((File_AfdBarData*)DTG1_Parser)->aspect_ratio_FromContainer=0; //4/3
            if (DAR>=1.774 && DAR<1.780) ((File_AfdBarData*)DTG1_Parser)->aspect_ratio_FromContainer=1; //16/9
        }
        if (DTG1_Parser->PTS_DTS_Needed)
        {
            DTG1_Parser->FrameInfo.PCR=FrameInfo.PCR;
            DTG1_Parser->FrameInfo.PTS=FrameInfo.PTS;
            DTG1_Parser->FrameInfo.DTS=FrameInfo.DTS;
        }
        Open_Buffer_Continue(DTG1_Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Element_Offset=Element_Size;
    #else //defined(MEDIAINFO_AFDBARDATA_YES)
        Skip_XX(Element_Size-Element_Offset,                    "Active Format Description");
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
}

//---------------------------------------------------------------------------
// Packet "B2", GA94
void File_Mpegv::user_data_start_GA94()
{
    //Parsing
    int8u user_data_type_code;
    Skip_B4(                                                    "GA94_identifier");
    Get_B1 (user_data_type_code,                                "user_data_type_code");
    switch (user_data_type_code)
    {
        case 0x03 : user_data_start_GA94_03(); break;
        case 0x06 : user_data_start_GA94_06(); break;
        default   : Skip_XX(Element_Size-Element_Offset,        "GA94_reserved_user_data");
    }
}

//---------------------------------------------------------------------------
// Packet "B2", GA94 0x03 (styled captioning)
void File_Mpegv::user_data_start_GA94_03()
{
    #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        GA94_03_IsPresent=true;
        MustExtendParsingDuration=true;
        Buffer_TotalBytes_Fill_Max=(int64u)-1; //Disabling this feature for this format, this is done in the parser

        Element_Info("DTVCC Transport");

        //Coherency
        if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
            return;

        //Purging too old orphelins
        if (GA94_03_TemporalReference_Offset+8<TemporalReference_Offset+temporal_reference)
        {
            size_t Pos=TemporalReference_Offset+temporal_reference;
            do
            {
                if (TemporalReference[Pos]==NULL || !TemporalReference[Pos]->IsValid)
                    break;
                Pos--;
            }
            while (Pos>0);
            GA94_03_TemporalReference_Offset=Pos+1;
        }

        if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
            TemporalReference.resize(TemporalReference_Offset+temporal_reference+1);
        if (TemporalReference[TemporalReference_Offset+temporal_reference]==NULL)
            TemporalReference[TemporalReference_Offset+temporal_reference]=new temporalreference;
        if (TemporalReference[TemporalReference_Offset+temporal_reference]->GA94_03==NULL)
            TemporalReference[TemporalReference_Offset+temporal_reference]->GA94_03=new temporalreference::buffer_data;
        TemporalReference[TemporalReference_Offset+temporal_reference]->GA94_03->Size=(size_t)(Element_Size-Element_Offset);
        delete[] TemporalReference[TemporalReference_Offset+temporal_reference]->GA94_03->Data;
        TemporalReference[TemporalReference_Offset+temporal_reference]->GA94_03->Data=new int8u[(size_t)(Element_Size-Element_Offset)];
        std::memcpy(TemporalReference[TemporalReference_Offset+temporal_reference]->GA94_03->Data, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));

        //Parsing
        Skip_XX(Element_Size-Element_Offset,                    "CC data");

        //Parsing Captions after reordering
        bool CanBeParsed=true;
        for (size_t GA94_03_Pos=GA94_03_TemporalReference_Offset; GA94_03_Pos<TemporalReference.size(); GA94_03_Pos++)
            if (TemporalReference[GA94_03_Pos]==NULL || !TemporalReference[GA94_03_Pos]->IsValid || TemporalReference[GA94_03_Pos]->GA94_03==NULL)
                CanBeParsed=false; //There is a missing field/frame
        if (CanBeParsed)
        {
            for (size_t GA94_03_Pos=GA94_03_TemporalReference_Offset; GA94_03_Pos<TemporalReference.size(); GA94_03_Pos++)
            {
                Element_Begin("Reordered DTVCC Transport");

                //Parsing
                #if MEDIAINFO_DEMUX
                    Element_Code=0x4741393400000003LL;
                #endif //MEDIAINFO_DEMUX
                if (GA94_03_Parser==NULL)
                {
                    GA94_03_Parser=new File_DtvccTransport;
                    Open_Buffer_Init(GA94_03_Parser);
                    ((File_DtvccTransport*)GA94_03_Parser)->Format=File_DtvccTransport::Format_A53_4_GA94_03;
                }
                if (GA94_03_Parser->PTS_DTS_Needed)
                {
                    GA94_03_Parser->FrameInfo.PCR=FrameInfo.PCR;
                    GA94_03_Parser->FrameInfo.PTS=FrameInfo.PTS-(TemporalReference.size()-1-GA94_03_Pos)*tc;
                    GA94_03_Parser->FrameInfo.DTS=FrameInfo.DTS-(TemporalReference.size()-1-GA94_03_Pos)*tc;
                }
                Demux(TemporalReference[GA94_03_Pos]->GA94_03->Data, TemporalReference[GA94_03_Pos]->GA94_03->Size, ContentType_MainStream);
                ((File_DtvccTransport*)GA94_03_Parser)->AspectRatio=MPEG_Version==1?Mpegv_aspect_ratio1[aspect_ratio_information]:Mpegv_aspect_ratio2[aspect_ratio_information];
                Open_Buffer_Continue(GA94_03_Parser, TemporalReference[GA94_03_Pos]->GA94_03->Data, TemporalReference[GA94_03_Pos]->GA94_03->Size);

                Element_End();
            }
            GA94_03_TemporalReference_Offset=TemporalReference.size();
        }
    #else //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
        Skip_XX(Element_Size-Element_Offset,                    "DTVCC Transport data");
    #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
}

//---------------------------------------------------------------------------
// Packet "B2", GA94 0x06 (bar data)
void File_Mpegv::user_data_start_GA94_06()
{
    #if defined(MEDIAINFO_AFDBARDATA_YES)
        Element_Info("Bar Data");

        //Parsing
        if (GA94_06_Parser==NULL)
        {
            GA94_06_Parser=new File_AfdBarData;
            Open_Buffer_Init(GA94_06_Parser);
            ((File_AfdBarData*)GA94_06_Parser)->Format=File_AfdBarData::Format_A53_4_GA94_06;
        }
        if (GA94_06_Parser->PTS_DTS_Needed)
        {
            GA94_06_Parser->FrameInfo.PCR=FrameInfo.PCR;
            GA94_06_Parser->FrameInfo.PTS=FrameInfo.PTS;
            GA94_06_Parser->FrameInfo.DTS=FrameInfo.DTS;
        }
        Open_Buffer_Init(GA94_06_Parser);
        Open_Buffer_Continue(GA94_06_Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Element_Offset=Element_Size;
    #else //defined(MEDIAINFO_AFDBARDATA_YES)
        Skip_XX(Element_Size-Element_Offset,                    "Bar Data");
    #endif //defined(MEDIAINFO_AFDBARDATA_YES)
}

//---------------------------------------------------------------------------
// Packet "B3"
void File_Mpegv::sequence_header()
{
    Element_Name("sequence_header");

    //Reading
    int32u bit_rate_value_temp;
    BS_Begin();
    Get_S2 (12, horizontal_size_value,                          "horizontal_size_value");
    Get_S2 (12, vertical_size_value,                            "vertical_size_value");
    Get_S1 ( 4, aspect_ratio_information,                       "aspect_ratio_information"); if (vertical_size_value && Mpegv_aspect_ratio1[aspect_ratio_information]) Param_Info((float)horizontal_size_value/vertical_size_value/Mpegv_aspect_ratio1[aspect_ratio_information]); Param_Info(Mpegv_aspect_ratio2[aspect_ratio_information]);
    Get_S1 ( 4, frame_rate_code,                                "frame_rate_code"); Param_Info(Mpegv_frame_rate[frame_rate_code]);
    Get_S3 (18, bit_rate_value_temp,                            "bit_rate_value"); Param_Info(bit_rate_value_temp*400);
    Mark_1 ();
    Get_S2 (10, vbv_buffer_size_value,                          "vbv_buffer_size_value"); Param_Info(2*1024*((int32u)vbv_buffer_size_value), " bytes");
    Skip_SB(                                                    "constrained_parameters_flag");
    TEST_SB_GET(load_intra_quantiser_matrix,                    "load_intra_quantiser_matrix");
        bool FillMatrix=Matrix_intra.empty();
        for (size_t Pos=0; Pos<64; Pos++)
        {
            int8u intra_quantiser;
            Get_S1 (8, intra_quantiser,                         "intra_quantiser");
            if (FillMatrix)
            {
                Ztring Value=Ztring::ToZtring(intra_quantiser, 16);
                if (Value.size()==1)
                    Value.insert(0, _T("0"));
                Matrix_intra+=Value;
            }
        }
    TEST_SB_END();
    TEST_SB_GET(load_non_intra_quantiser_matrix,                "load_non_intra_quantiser_matrix");
        bool FillMatrix=Matrix_nonintra.empty();
        for (size_t Pos=0; Pos<64; Pos++)
        {
            int8u non_intra_quantiser;
            Get_S1 (8, non_intra_quantiser,                     "non_intra_quantiser");
            if (FillMatrix)
            {
                Ztring Value=Ztring::ToZtring(non_intra_quantiser, 16);
                if (Value.size()==1)
                    Value.insert(0, _T("0"));
                Matrix_nonintra+=Value;
            }
        }
    TEST_SB_END();
    BS_End();

    //0x00 at the end
    if (Element_Offset<Element_Size)
    {
        int64u NullBytes_Begin=Element_Size-1;
        while (NullBytes_Begin>Element_Offset && Buffer[Buffer_Offset+(size_t)NullBytes_Begin]==0x00)
            NullBytes_Begin--;

        if (NullBytes_Begin==Element_Offset)
            Skip_XX(Element_Size-Element_Offset,                "Padding");
    }

    FILLING_BEGIN_PRECISE();
        //Temporal reference
        temporal_reference_Old=(int16u)-1;
        TemporalReference_Offset=TemporalReference.size();
        if (TemporalReference_Offset>=0x800)
        {
            for (size_t Pos=0; Pos<0x400; Pos++)
                delete TemporalReference[Pos]; //TemporalReference[Pos]=NULL;
            TemporalReference.erase(TemporalReference.begin(), TemporalReference.begin()+0x400);
            if (0x400<TemporalReference_Offset)
                TemporalReference_Offset-=0x400;
            else
                TemporalReference_Offset=0;
            #if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
                if (0x400<GA94_03_TemporalReference_Offset)
                    GA94_03_TemporalReference_Offset-=0x400;
                else
                    GA94_03_TemporalReference_Offset=0;
            #endif //defined(MEDIAINFO_DTVCCTRANSPORT_YES)
            #if defined(MEDIAINFO_SCTE20_YES)
                if (0x400<Scte_TemporalReference_Offset)
                    Scte_TemporalReference_Offset-=0x400;
                else
                    Scte_TemporalReference_Offset=0;
            #endif //defined(MEDIAINFO_SCTE20_YES)
        }

        //Bit_rate
        if (bit_rate_value_IsValid && bit_rate_value_temp!=bit_rate_value)
            bit_rate_value_IsValid=false; //two bit_rate_values, not handled.
        else if (bit_rate_value==0)
        {
            bit_rate_value=bit_rate_value_temp;
            bit_rate_value_IsValid=true;
        }

        //NextCode
        if (!Status[IsAccepted])
        {
            NextCode_Clear();
            NextCode_Add(0x00);
            NextCode_Add(0xB2);
            NextCode_Add(0xB5);
            NextCode_Add(0xB8);
        }

        //Autorisation of other streams
        Streams[0x00].Searching_Payload=true;
        Streams[0xB2].Searching_Payload=true;
        Streams[0xB5].Searching_Payload=true;
        if (Frame_Count==0)
            Streams[0xB8].Searching_TimeStamp_Start=true;
        Streams[0xB8].Searching_TimeStamp_End=true;

        //Temp
        if (Mpegv_frame_rate[frame_rate_code])
            tc=float64_int64s(((float64)1000000000)/Mpegv_frame_rate[frame_rate_code]);
        if (Frame_Count<Frame_Count_Valid)
        {
            FrameRate=Mpegv_frame_rate[frame_rate_code];
            SizeToAnalyse_Begin=bit_rate_value*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure to have at least 2 timestamps (for integrity checking)
            SizeToAnalyse_End=bit_rate_value*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure
        }

        //Setting as OK
        sequence_header_IsParsed=true;
        if (Frame_Count==0 && FrameInfo.DTS==(int64u)-1)
            FrameInfo.DTS=0; //No DTS in container
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B4"
void File_Mpegv::sequence_error()
{
    Element_Name("sequence_error");
}

//---------------------------------------------------------------------------
// Packet "B5"
void File_Mpegv::extension_start()
{
    Element_Name("extension_start");
    MPEG_Version=2; //extension_start only exists in MPEG-2 specs

    //Parsing
    int8u extension_start_code_identifier;
    BS_Begin();
    Get_S1 ( 4, extension_start_code_identifier,                "extension_start_code_identifier"); Param_Info(Mpegv_extension_start_code_identifier[extension_start_code_identifier]);
    Element_Info(Mpegv_extension_start_code_identifier[extension_start_code_identifier]);

    switch (extension_start_code_identifier)
    {
        case  1 :{ //Sequence
                    //Parsing
                    Peek_SB(profile_and_level_indication_escape);
                    if (profile_and_level_indication_escape)
                    {
                        Get_S1 ( 8, profile_and_level_indication, "profile_and_level_indication"); Param_Info(Mpegv_profile_and_level_indication(profile_and_level_indication));
                    }
                    else
                    {
                        Skip_SB(                               "profile_and_level_indication_escape");
                        Get_S1 ( 3, profile_and_level_indication_profile, "profile_and_level_indication_profile"); Param_Info(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile]);
                        Get_S1 ( 4, profile_and_level_indication_level, "profile_and_level_indication_level"); Param_Info(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]);
                    }
                    Get_SB (    progressive_sequence,           "progressive_sequence");
                    Get_S1 ( 2, chroma_format,                  "chroma_format"); Param_Info(Mpegv_Colorimetry_format[chroma_format]);
                    Get_S1 ( 2, horizontal_size_extension,      "horizontal_size_extension");
                    Get_S1 ( 2, vertical_size_extension,        "vertical_size_extension");
                    Get_S2 (12, bit_rate_extension,             "bit_rate_extension");
                    Mark_1 ();
                    Get_S1 ( 8, vbv_buffer_size_extension,      "vbv_buffer_size_extension"); Param_Info(2*1024*((((int32u)vbv_buffer_size_extension)<<10)+vbv_buffer_size_value), " bytes");
                    Skip_SB(                                    "low_delay");
                    Get_S1 ( 2, frame_rate_extension_n,         "frame_rate_extension_n");
                    Get_S1 ( 5, frame_rate_extension_d,         "frame_rate_extension_d");
                    BS_End();

                    FILLING_BEGIN();
                        if (frame_rate_extension_d)
                            FrameRate=(float)frame_rate_extension_n/frame_rate_extension_d;
                    FILLING_END();
                }
                break;
        case  2 :{ //Sequence Display
                    //Parsing
                    Get_S1 ( 3, video_format,                   "video_format"); Param_Info(Mpegv_video_format[video_format]);
                    TEST_SB_SKIP(                               "colour_description");
                        Get_S1 (8, colour_primaries,            "colour_primaries"); Param_Info(Mpegv_colour_primaries(colour_primaries));
                        Get_S1 (8, transfer_characteristics,    "transfer_characteristics"); Param_Info(Mpegv_transfer_characteristics(transfer_characteristics));
                        Get_S1 (8, matrix_coefficients,         "matrix_coefficients"); Param_Info(Mpegv_matrix_coefficients(matrix_coefficients));
                    TEST_SB_END();
                    Get_S2 (14, display_horizontal_size,        "display_horizontal_size");
                    Mark_1 ();
                    Get_S2 (14, display_vertical_size,          "display_vertical_size");
                    BS_End();
                }
                break;
        case  5 :{ //Sequence Scalable Extension
                    //Parsing
                    Skip_S1(4,                                  "data");
                    BS_End();
                    Skip_XX(Element_Size-Element_Offset,        "data");
                }
                break;
        case  8 :{ //Picture Coding
                    //Parsing
                    Skip_S1( 4,                                 "f_code_forward_horizontal");
                    Skip_S1( 4,                                 "f_code_forward_vertical");
                    Skip_S1( 4,                                 "f_code_backward_horizontal");
                    Skip_S1( 4,                                 "f_code_backward_vertical");
                    Get_S1 ( 2, intra_dc_precision,             "intra_dc_precision");
                    Get_S1 ( 2, picture_structure,              "picture_structure"); Param_Info(Mpegv_picture_structure[picture_structure]);
                    Get_SB (    top_field_first,                "top_field_first");
                    Skip_SB(                                    "frame_pred_frame_dct");
                    Skip_SB(                                    "concealment_motion_vectors");
                    Skip_SB(                                    "q_scale_type");
                    Skip_SB(                                    "intra_vlc_format");
                    Skip_SB(                                    "alternate_scan");
                    Get_SB (    repeat_first_field,             "repeat_first_field");
                    Skip_SB(                                    "chroma_420_type");
                    Get_SB (    progressive_frame,              "progressive_frame");
                    TEST_SB_SKIP(                               "composite_display_flag");
                        Skip_SB(                                "v_axis");
                        Skip_S1( 3,                             "field_sequence");
                        Skip_SB(                                "sub_carrier");
                        Skip_S1( 7,                             "burst_amplitude");
                        Skip_S1( 8,                             "sub_carrier_phase");
                    TEST_SB_END();
                    BS_End();
                }

                FILLING_BEGIN();
                    if (progressive_frame==false)
                    {
                        if (picture_structure==3)           //Frame
                        {
                            if (top_field_first)
                                Interlaced_Top++;
                            else
                                Interlaced_Bottom++;
                            FirstFieldFound=false;
                            if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
                                TemporalReference.resize(TemporalReference_Offset+temporal_reference+1);
                            if (TemporalReference[TemporalReference_Offset+temporal_reference]==NULL)
                                TemporalReference[TemporalReference_Offset+temporal_reference]=new temporalreference;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->picture_coding_type=picture_coding_type;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->progressive_frame=progressive_frame;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->picture_structure=picture_structure;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->top_field_first=top_field_first;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->repeat_first_field=repeat_first_field;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->HasPictureCoding=true;
                        }
                        else                                //Field
                        {
                            if (!FirstFieldFound)
                            {
                                if (picture_structure==1)   //-Top
                                    Interlaced_Top++;
                                else                        //-Bottom
                                    Interlaced_Bottom++;
                            }
                            FirstFieldFound=!FirstFieldFound;
                        }
                    }
                    else
                    {
                        progressive_frame_Count++;
                        if (top_field_first)
                            Interlaced_Top++;
                        else
                            Interlaced_Bottom++;
                        if (picture_structure==3)           //Frame
                        {
                            if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
                                TemporalReference.resize(TemporalReference_Offset+temporal_reference+1);
                            if (TemporalReference[TemporalReference_Offset+temporal_reference]==NULL)
                                TemporalReference[TemporalReference_Offset+temporal_reference]=new temporalreference;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->picture_coding_type=picture_coding_type;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->progressive_frame=progressive_frame;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->picture_structure=picture_structure;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->top_field_first=top_field_first;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->repeat_first_field=repeat_first_field;
                            TemporalReference[TemporalReference_Offset+temporal_reference]->HasPictureCoding=true;
                        }
                    FILLING_END();
                }
                break;
        case  9 :{ //Picture Spatial Scalable Extension
                    //Parsing
                    Skip_S1(4,                                  "data");
                    BS_End();
                    Skip_XX(Element_Size-Element_Offset,        "data");
                }
                break;
        case 10 :{ //Picture Temporal Scalable Extension
                    //Parsing
                    Skip_S1(4,                                  "data");
                    BS_End();
                    Skip_XX(Element_Size-Element_Offset,        "data");
                }
                break;
        default:{
                    //Parsing
                    Skip_S1(4,                                  "data");
                    BS_End();
                    Skip_XX(Element_Size-Element_Offset,        "data");
                }
    }
}

//---------------------------------------------------------------------------
// Packet "B7"
void File_Mpegv::sequence_end()
{
    Element_Name("sequence_end");

    if (!Status[IsFilled] && sequence_header_IsParsed)
    {
        //End of file, and we have some frames
        Accept("MPEG Video");
        Finish("MPEG Video");
    }
}

//---------------------------------------------------------------------------
// Packet "B8"
void File_Mpegv::group_start()
{
    if (!Status[IsAccepted])
    {
        if (!NextCode_Test())
            return;
    }
    Element_Name("group_start");

    //Reading
    int8u Hours, Minutes, Seconds, Frames;
    bool drop_frame_flag, closed_gop, broken_link;
    #if MEDIAINFO_TRACE
    if (Trace_Activated)
    {
        //Parsing
        BS_Begin();
        Get_SB (    drop_frame_flag,                            "time_code_drop_frame_flag");
        Get_S1 ( 5, Hours,                                      "time_code_time_code_hours");
        Get_S1 ( 6, Minutes,                                    "time_code_time_code_minutes");
        Mark_1();
        Get_S1 ( 6, Seconds,                                    "time_code_time_code_seconds");
        Get_S1 ( 6, Frames,                                     "time_code_time_code_pictures");
        Get_SB (    closed_gop,                                 "closed_gop");
        Get_SB (    broken_link,                                "broken_link");
        BS_End();
        Ztring Time;
        Time+=Ztring::ToZtring(Hours);
        Time+=_T(':');
        Time+=Ztring::ToZtring(Minutes);
        Time+=_T(':');
        Time+=Ztring::ToZtring(Seconds);
        if (FrameRate!=0)
        {
            Time+=_T('.');
            Time+=Ztring::ToZtring(Frames*1000/FrameRate, 0);
        }
        Element_Info(Time);
    }
    else
    {
    #endif //MEDIAINFO_TRACE
        //Parsing
        size_t Buffer_Pos=Buffer_Offset+(size_t)Element_Offset;
        drop_frame_flag                     = (Buffer[Buffer_Pos  ]&0x80)?true:false;
        Hours                               =((Buffer[Buffer_Pos  ]&0x7C)>> 2);
        Minutes                             =((Buffer[Buffer_Pos  ]&0x03)<< 4)
                                           | ((Buffer[Buffer_Pos+1]     )>> 4);
        Seconds                             =((Buffer[Buffer_Pos+1]&0x07)<< 3)
                                           | ((Buffer[Buffer_Pos+2]     )>> 5);
        Frames                              =((Buffer[Buffer_Pos+2]&0x1F)<< 1)
                                           | ((Buffer[Buffer_Pos+3]     )>> 7);
        closed_gop                          = (Buffer[Buffer_Pos+3]&0x40)?true:false;
        broken_link                         = (Buffer[Buffer_Pos+3]&0x20)?true:false;
        Element_Offset+=4;
    #if MEDIAINFO_TRACE
    }
    #endif //MEDIAINFO_TRACE

    FILLING_BEGIN();
        //NextCode
        if (!Status[IsAccepted])
        {
            NextCode_Clear();
            NextCode_Add(0x00);
            NextCode_Add(0xB2);
            NextCode_Add(0xB5);
            NextCode_Add(0xB8);
        }

        if (TimeCodeIsNotTrustable)
            return;

        if (Time_Current_Seconds==0 && Time_Current_Frames==0 && Hours==0 && Minutes==0 && Seconds==0 && Frames==0)
        {
            //Time code is always 0
            TimeCodeIsNotTrustable=true;
            Time_Begin_Seconds=(size_t)-1;
            Time_End_Seconds=(size_t)-1;
            return;
        }

        //Calculating
        Time_Current_Seconds=60*60*Hours+60*Minutes+Seconds;
        Time_Current_Frames =Frames;

        if (!group_start_IsParsed)
            group_start_IsParsed=true;
        if (!group_start_FirstPass)
        {
            group_start_FirstPass=true;
            group_start_drop_frame_flag=drop_frame_flag;
            group_start_closed_gop=closed_gop;
            group_start_broken_link=broken_link;
        }

        RefFramesCount=0;

        //Autorisation of other streams
        if (Searching_TimeStamp_Start_DoneOneTime)
            Streams[0xB8].Searching_TimeStamp_Start=false; //group_start
        else
            Searching_TimeStamp_Start_DoneOneTime=true;
        Streams[0x00].Searching_TimeStamp_End=true; //picture_start
    FILLING_END();
}

} //NameSpace

#endif //MEDIAINFO_MPEGV_YES
