// File_Mpegv - Info for MPEG Video files
// Copyright (C) 2004-2009 Jerome Martinez, Zen@MediaArea.net
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

//***************************************************************************
// Infos (Global)
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGV_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
extern const float32 Mpegv_frame_rate[]=
{
    (float32) 0,
    (float32)23.976,
    (float32)24,
    (float32)25,
    (float32)29.97,
    (float32)30,
    (float32)50,
    (float32)59.94,
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
const char* Mpegv_Colorimetry_format[]=
{
    "",
    "4:2:0",
    "4:2:2",
    "4:4:4",
};

//---------------------------------------------------------------------------
const char* Mpegv_profile_and_level_indication_profile[]=
{
    "",
    "High",
    "Spatial",
    "SNR",
    "Main",
    "Simple",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpegv_profile_and_level_indication_level[]=
{
    "",
    "",
    "",
    "",
    "High",
    "",
    "High-1440",
    "",
    "Main",
    "",
    "Low",
    "",
    "",
    "",
    "",
    "",
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
#if defined(MEDIAINFO_EIA608_YES)
    #include "MediaInfo/Text/File_Eia608.h"
#endif
#if defined(MEDIAINFO_EIA708_YES)
    #include "MediaInfo/Text/File_Eia708.h"
#endif
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
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpegv_user_data_DTG1_active_format[]=
{
    //1st value is for 4:3, 2nd is for 16:9
    "", //Undefined
    "Reserved",
    "Not recommended",
    "Not recommended",
    "Aspect ratio greater than 16:9", //Use GA94
    "Reserved",
    "Reserved",
    "Reserved",
    "4:3 full frame image / 16:9 full frame image",
    "4:3 full frame image / 4:3 pillarbox image",
    "16:9 letterbox image / 16:9 full frame image",
    "14:9 letterbox image / 14:9 pillarbox image",
    "Reserved",
    "4:3 full frame image, alternative 14:9 center / 4:3 pillarbox image, alternative 14:9 center",
    "16:9 letterbox image, alternative 14:9 center / 16:9 full frame image, alternative 14:9 center",
    "16:9 letterbox image, alternative 4:3 center / 16:9 full frame image, alternative 4:3 center",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpegv::File_Mpegv()
:File__Analyze()
{
    //Configuration
    Trusted_Multiplier=2;
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=64*1024;

    //In
    MPEG_Version=1;
    Frame_Count_Valid=32;
    FrameIsAlwaysComplete=false;
    TimeCodeIsNotTrustable=false;

    //Temp
    SizeToAnalyse_Begin=1*1024*1024;
    SizeToAnalyse_End=1*1024*1024;
    Searching_TimeStamp_Start_DoneOneTime=false;
    sequence_header_IsParsed=false;
    Parsing_End_ForDTS=false;
}

//---------------------------------------------------------------------------
File_Mpegv::~File_Mpegv()
{
    for (size_t Pos=0; Pos<DVD_CC_Parsers.size(); Pos++)
        delete DVD_CC_Parsers[Pos]; //GA94_03_CC_Parsers[Pos]=NULL;
    for (size_t Pos=0; Pos<GA94_03_CC_Parsers.size(); Pos++)
        delete GA94_03_CC_Parsers[Pos]; //GA94_03_CC_Parsers[Pos]=NULL;
}


//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpegv::Synched_Test()
{
    //Trailing 0xFF
    while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]==0xFF)
        Buffer_Offset++;

    //Trailing 0x00
    while(Buffer_Offset+3<=Buffer_Size && Buffer[Buffer_Offset]==0x00 && CC3(Buffer+Buffer_Offset)!=0x000001)
        Buffer_Offset++;

    //Must have enough buffer for having header
    if (Buffer_Offset+3>Buffer_Size)
        return false;

    //Quick test of synchro
    if (CC3(Buffer+Buffer_Offset)!=0x000001)
        Synched=false;

    //Quick search
    if (Synched && !Header_Parser_QuickSearch())
        return false;

    //We continue
    return true;
}

//---------------------------------------------------------------------------
void File_Mpegv::Synched_Init()
{
    //Temp
    Frame_Count=0;
    progressive_frame_Count=0;
    Interlaced_Top=0;
    Interlaced_Bottom=0;
    display_horizontal_size=0;
    display_vertical_size=0;
    vbv_delay=0;
    Time_Begin_Seconds=Error;
    Time_Begin_Frames=(int8u)-1;
    Time_End_Seconds=Error;
    Time_End_Frames=(int8u)-1;
    TemporalReference_Offset=0;
    TemporalReference_GA94_03_CC_Offset=0;
    picture_coding_type=(int8u)-1;
    bit_rate_value=0;
    FrameRate=0;
    horizontal_size_value=0;
    vertical_size_value=0;
    bit_rate_extension=0;
    aspect_ratio_information=0;
    frame_rate_code=0;
    profile_and_level_indication_profile=0;
    profile_and_level_indication_level=0;
    chroma_format=0;
    horizontal_size_extension=0;
    vertical_size_extension=0;
    frame_rate_extension_n=0;
    frame_rate_extension_d=0;
    video_format=5; //Unspecified video format
    DVD_CC_IsPresent=false;
    GA94_03_CC_IsPresent=false;
    Time_End_NeedComplete=false;
    load_intra_quantiser_matrix=false;
    load_non_intra_quantiser_matrix=false;
    progressive_sequence=true; //progressive by default
    top_field_first=false;
    repeat_first_field=false;
    FirstFieldFound=false;
    group_start_IsParsed=false;

    //Default stream values
    Streams.resize(0x100);
    Streams[0xB3].Searching_Payload=true;
    for (int8u Pos=0xBA; Pos!=0x00; Pos++)
        Streams[Pos].Searching_Payload=true; //Testing MPEG-PS
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Finalize()
{
    if (Trusted==0
     || !sequence_header_IsParsed
     || !IsSub && Frame_Count==0)
        return;
        
    //In case of partial data, and finalizing is forced (example: DecConfig in .mp4), but with at least one frame
    if (!IsFilled && sequence_header_IsParsed)
    {
        Time_End_Seconds=Error;
        slice_start_Fill();
    }

    //Duration
    if (Time_End_NeedComplete && MediaInfoLib::Config.ParseSpeed_Get()!=1)
        Time_End_Seconds=Error;
    if (Time_End_Seconds!=Error)
    {
        size_t Time_Begin=Time_Begin_Seconds*1000;
        size_t Time_End =Time_End_Seconds*1000;
        if (FrameRate)
        {
            Time_Begin+=(size_t)(Time_Begin_Frames*1000/FrameRate);
            Time_End  +=(size_t)(Time_End_Frames  *1000/FrameRate);
        }
        if (Time_End>Time_Begin)
            Fill(Stream_Video, 0, Video_Duration, Time_End-Time_Begin);
    }

    //Delay
    if (group_start_IsParsed)
    {
        size_t Time_Begin=Time_Begin_Seconds*1000;
        if (FrameRate)
            Time_Begin+=(size_t)(Time_Begin_Frames*1000/FrameRate);
        Fill(Stream_Video, 0, Video_Delay, Time_Begin);
        Fill(Stream_Video, 0, Video_Delay_Settings, Ztring(_T("drop_frame_flag="))+(group_start_drop_frame_flag?_T("1"):_T("0")));
        Fill(Stream_Video, 0, Video_Delay_Settings, Ztring(_T("closed_gop="))+(group_start_closed_gop?_T("1"):_T("0")));
        Fill(Stream_Video, 0, Video_Delay_Settings, Ztring(_T("broken_link="))+(group_start_broken_link?_T("1"):_T("0")));
    }

    //DVD captions
    for (size_t Pos=0; Pos<DVD_CC_Parsers.size(); Pos++)
        if (DVD_CC_Parsers[Pos] && DVD_CC_Parsers[Pos]->IsAccepted)
        {
            Open_Buffer_Finalize(DVD_CC_Parsers[Pos]);
            Merge(*DVD_CC_Parsers[Pos]);
            Fill(Stream_Text, StreamPos_Last, Text_ID, _T("DVD-")+Ztring::ToZtring(Pos));
            Fill(Stream_Text, StreamPos_Last, "MuxingMode", _T("DVD-Video"));
        }

    //GA94 captions
    for (size_t Pos=0; Pos<GA94_03_CC_Parsers.size(); Pos++)
        if (GA94_03_CC_Parsers[Pos] && GA94_03_CC_Parsers[Pos]->IsAccepted)
        {
            Open_Buffer_Finalize(GA94_03_CC_Parsers[Pos]);
            Merge(*GA94_03_CC_Parsers[Pos]);
            if (Pos<2)
                Fill(Stream_Text, StreamPos_Last, Text_ID, _T("608-")+Ztring::ToZtring(Pos));
            Fill(Stream_Text, StreamPos_Last, "MuxingMode", _T("EIA-708"));
        }

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Streams.clear();
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Header_Parse()
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
    while (           Buffer_Offset+4<=Buffer_Size
      &&   CC3(Buffer+Buffer_Offset)==0x000001)
    {
        //Getting start_code
        int8u start_code=Buffer[Buffer_Offset+3];

        //Searching start or timestamp
        if (Streams[start_code].Searching_Payload
         || Streams[start_code].Searching_TimeStamp_Start
         || Streams[start_code].Searching_TimeStamp_End)
            return true;

        //Getting size
        Buffer_Offset+=4;
        while (Buffer_Offset+4<=Buffer_Size)
        {
            while (Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset]!=0x00)
                Buffer_Offset++;
            if (Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset+1]==0x00)
                if (Buffer[Buffer_Offset+2]==0x01)
                    break;
            Buffer_Offset++;
        }
    }

    if(File_Offset+Buffer_Size==File_Size && !IsFilled && Frame_Count>=1)
        slice_start_Fill(); //End of file, and we have some frames
    else if (Buffer_Offset+4<=Buffer_Size)
        Trusted_IsNot("MPEG Video, Synchronisation lost");

    Synched=false;
    return Synchronize();
}

//---------------------------------------------------------------------------
void File_Mpegv::Data_Parse()
{
    //Parsing
    switch (Element_Code)
    {
        case 0x00: picture_start(); break;
        case 0xB2: user_data_start(); break;
        case 0xB3: sequence_header(); break;
        case 0xB4: sequence_error(); break;
        case 0xB5: extension_start(); break;
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
    if (IsFilled
     && (File_Size>SizeToAnalyse_Begin+SizeToAnalyse_End && File_Offset+Buffer_Offset+Element_Offset>SizeToAnalyse_Begin && File_Offset+Buffer_Offset+Element_Offset<File_Size-SizeToAnalyse_End && MediaInfoLib::Config.ParseSpeed_Get()<=0.01
      || IsSub))
    {
        if ((GA94_03_CC_IsPresent || DVD_CC_IsPresent) && Frame_Count<Frame_Count_Valid*10) //10 times the normal test
        {
            Streams[0x00].Searching_Payload=GA94_03_CC_IsPresent;
            Streams[0xB2].Searching_Payload=true;
            Streams[0xB3].Searching_Payload=GA94_03_CC_IsPresent;
            return;
        }

        //
        Time_End_Seconds=Error;
        Time_End_Frames=(int8u)-1;

        //Autorisation of other streams
        if (!IsSub)
            Streams[0x00].Searching_TimeStamp_End=false;

        //Jumping
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
    if (!NextCode_Test())
        return;

    #ifndef MEDIAINFO_MINIMIZESIZE
        if (Time_End_Seconds!=Error)
        {
            size_t Time_End  =Time_End_Seconds  *1000;
            if (FrameRate)
                Time_End  +=(size_t)(Time_End_Frames  *1000/FrameRate);
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
            Element_Info(Time);
        }
    #endif //MEDIAINFO_MINIMIZESIZE

    //Time
    if (Time_End_Seconds!=Error)
    {
        Time_End_Frames++; //One frame
        if (progressive_sequence && repeat_first_field)
        {
            Time_End_Frames++; //Frame repeated a second time
            if (top_field_first)
                Time_End_Frames++; //Frame repeated a third time
        }
    }

    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;
    Frame_Count_InThisBlock++;

    //Name
    Element_Name("picture_start");
    Element_Info(Ztring(_T("Field ")+Ztring::ToZtring(Frame_Count)));

    //Need to parse?
    if (!Streams[0x00].Searching_Payload)
    {
        Skip_XX(Element_Size,                                   "Data");
        return;
    }

    //Parsing
    BS_Begin();
    Get_S2 (10, temporal_reference,                             "temporal_reference");
    Get_S1 ( 3, picture_coding_type,                            "picture_coding_type"); Param_Info(Mpegv_picture_coding_type[picture_coding_type]);
    Element_Info(Mpegv_picture_coding_type[picture_coding_type]);
    Get_S2 (16, vbv_delay,                                      "vbv_delay");
    if (picture_coding_type==2 || picture_coding_type==3) //P or B
    {
        Skip_S1(1,                                              "full_pel_forward_vector");
        Skip_S1(3,                                              "forward_f_code");
    }
    if (picture_coding_type==3) //B
    {
        Skip_S1(1,                                              "full_pel_backward_vector");
        Skip_S1(3,                                              "backward_f_code");
    }
    bool extra_bit_picture;
    do
    {
        Peek_SB(extra_bit_picture);
        if (extra_bit_picture)
        {
            Skip_S1(1,                                          "extra_bit_picture");
            Skip_S1(8,                                          "extra_information_picture");
        }
    }
    while (extra_bit_picture);
    BS_End();
    
    FILLING_BEGIN();
        if (TemporalReference_Offset+temporal_reference>=TemporalReference.size())
            TemporalReference.resize(TemporalReference_Offset+temporal_reference+1);
        TemporalReference[TemporalReference_Offset+temporal_reference].IsValid=true;

        //if (GA94_03_IsPresent && Frame_Count>Frame_Count_Valid)
        //    return;

        //NextCode
        NextCode_Clear();
        for (int64u Element_Name_Next=0x01; Element_Name_Next<=0x1F; Element_Name_Next++)
            NextCode_Add(Element_Name_Next);
        NextCode_Add(0xB2);
        NextCode_Add(0xB5);
        NextCode_Add(0xB8);

        //Autorisation of other streams
        for (int8u Pos=0x01; Pos<=0x1F; Pos++)
            Streams[Pos].Searching_Payload=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "01" --> "AF"
void File_Mpegv::slice_start()
{
    if (!NextCode_Test())
        return;
    Element_Name("slice_start");

    //Parsing
    Skip_XX(Element_Size,                                       "data");

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0x00);
        NextCode_Add(0xB3);
        NextCode_Add(0xB8);

        //Autorisation of other streams
        for (int8u Pos=0x01; Pos<=0x1F; Pos++)
            Streams[Pos].Searching_Payload=false;

        //Filling only if not already done
        if (!IsFilled && (!DVD_CC_IsPresent && !GA94_03_CC_IsPresent && Frame_Count>=Frame_Count_Valid || Frame_Count>=Frame_Count_Valid*10))
            slice_start_Fill();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpegv::slice_start_Fill()
{
    //Filling
    Stream_Prepare(Stream_General);
    Stream_Prepare(Stream_Video);

    //Version
    if (MPEG_Version==2)
    {
        Fill(Stream_General, 0, General_Format, "MPEG Video");
        Fill(Stream_General, 0, General_Format_Version, "Version 2");
        Fill(Stream_Video, 0, Video_Format, "MPEG Video");
        Fill(Stream_Video, 0, Video_Format_Version, "Version 2");
        Fill(Stream_Video, 0, Video_Codec, "MPEG-2V");
        Fill(Stream_Video, 0, Video_Codec_String, "MPEG-2 Video");
    }
    else
    {
        Fill(Stream_General, 0, General_Format, "MPEG Video");
        Fill(Stream_General, 0, General_Format_Version, "Version 1");
        Fill(Stream_Video, 0, Video_Format, "MPEG Video");
        Fill(Stream_Video, 0, Video_Format_Version, "Version 1");
        Fill(Stream_Video, 0, Video_Codec, "MPEG-1V");
        Fill(Stream_Video, 0, Video_Codec_String, "MPEG-1 Video");
    }

    Fill(Stream_Video, 0, Video_Width, 0x1000*horizontal_size_extension+horizontal_size_value);
    Fill(Stream_Video, 0, Video_Height, 0x1000*vertical_size_extension+vertical_size_value);
    Fill(Stream_Video, 0, Video_Colorimetry, Mpegv_Colorimetry_format[chroma_format]);

    //AspectRatio
    if (MPEG_Version==2)
    {
        if (aspect_ratio_information==0)
            ;//Forbidden
        else if (aspect_ratio_information==1)
            Fill(Stream_Video, 0, Video_PixelAspectRatio, 1.000);
        else if (display_horizontal_size && display_vertical_size)
        {
            if (vertical_size_value && Mpegv_aspect_ratio2[aspect_ratio_information])
                Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, (float)(0x1000*horizontal_size_extension+horizontal_size_value)/(0x1000*vertical_size_extension+vertical_size_value)
                                                                             *Mpegv_aspect_ratio2[aspect_ratio_information]/((float)display_horizontal_size/display_vertical_size));
        }
        else if (Mpegv_aspect_ratio2[aspect_ratio_information])
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, Mpegv_aspect_ratio2[aspect_ratio_information]);
    }
    else //Version 1
    {
        if (vertical_size_value && Mpegv_aspect_ratio1[aspect_ratio_information])
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, (float)(0x1000*horizontal_size_extension+horizontal_size_value)/(0x1000*vertical_size_extension+vertical_size_value)/Mpegv_aspect_ratio1[aspect_ratio_information]);
    }

    //FrameRate
    if (frame_rate_extension_d!=0)
        Fill(Stream_Video, StreamPos_Last, Video_FrameRate, (float)frame_rate_extension_n/frame_rate_extension_d);
    else
        Fill(Stream_Video, StreamPos_Last, Video_FrameRate, Mpegv_frame_rate[frame_rate_code]);

    //BitRate
    if (vbv_delay==0xFFFF || (MPEG_Version==1 && bit_rate_value==0x3FFFF))
        Fill(Stream_Video, 0, Video_BitRate_Mode, "VBR");
    else if ((MPEG_Version==1 && bit_rate_value!=0x3FFFF) || MPEG_Version==2)
        Fill(Stream_Video, 0, Video_BitRate_Mode, "CBR");
    if (bit_rate_value!=0x3FFFF)
        Fill(Stream_Video, 0, Video_BitRate_Nominal, bit_rate_value*400);

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
        std::string TempRef;
        for (size_t Pos=0; Pos<TemporalReference.size(); Pos++)
            if (TemporalReference[Pos].IsValid)
            {
                TempRef+=TemporalReference[Pos].top_field_first?"T":"B";
                TempRef+=TemporalReference[Pos].repeat_first_field?"3":"2";
            }
        if (TempRef.find('3')!=std::string::npos)
        {
            if (TempRef.find("T2T3B2B3T2T3B2B3")!=std::string::npos
             || TempRef.find("B2B3T2T3B2B3T2T3")!=std::string::npos)
            {
                Fill(Stream_Video, 0, Video_ScanOrder, "2:3 Pulldown", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_FrameRate, Retrieve(Stream_Video, 0, Video_FrameRate).To_float32()*24/30, 3, true); //Real framerate
                Fill(Stream_Video, 0, Video_ScanType, "Progressive", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_Interlacement, "PPF", Unlimited, true, true);
            }
            if (TempRef.find("T2T2T2T2T2T2T2T2T2T2T2T3B2B2B2B2B2B2B2B2B2B2B2B3")!=std::string::npos
             || TempRef.find("B2B2B2B2B2B2B2B2B2B2B2B3T2T2T2T2T2T2T2T2T2T2T2T3")!=std::string::npos)
            {
                Fill(Stream_Video, 0, Video_ScanOrder, "2:2:2:2:2:2:2:2:2:2:2:3 Pulldown", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_FrameRate, Retrieve(Stream_Video, 0, Video_FrameRate).To_float32()*24/25, 3, true); //Real framerate
                Fill(Stream_Video, 0, Video_ScanType, "Progressive", Unlimited, true, true);
                Fill(Stream_Video, 0, Video_Interlacement, "PPF", Unlimited, true, true);
            }
        }
    }

    //Profile
    if (profile_and_level_indication_profile>0 && profile_and_level_indication_profile<8 && profile_and_level_indication_level>0 && profile_and_level_indication_level<16)
    {
        Fill(Stream_Video, 0, Video_Format_Profile, Ztring().From_Local(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile])+_T("@")+Ztring().From_Local(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]));
        Fill(Stream_Video, 0, Video_Codec_Profile, Ztring().From_Local(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile])+_T("@")+Ztring().From_Local(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]));
    }

    //Standard
    Fill(Stream_Video, 0, Video_Standard, Mpegv_video_format[video_format]);

    //Matrix
    if (load_intra_quantiser_matrix || load_intra_quantiser_matrix)
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

    //Autorisation of other streams
    NextCode_Clear();
    NextCode_Add(0x00);
    NextCode_Add(0xB8);
    for (int8u Pos=0x00; Pos<=0xB9; Pos++)
        Streams[Pos].Searching_Payload=false;
    Streams[0xB8].Searching_TimeStamp_End=true;
    if (IsSub)
        Streams[0x00].Searching_TimeStamp_End=true;

    //Detected
    Accept("MPEG Video");
    IsFilled=true;
    Detect_EOF();
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
        }
    }

    //Rejecting junk from the end
    size_t Library_End_Offset=(size_t)Element_Size;
    while (Library_End_Offset>0
        && (Buffer[Buffer_Offset+Library_End_Offset-1]<0x20
         || Buffer[Buffer_Offset+Library_End_Offset-1]>0x7D
         || (Buffer[Buffer_Offset+Library_End_Offset-1]>=0x3A
          && Buffer[Buffer_Offset+Library_End_Offset-1]<=0x40)))
        Library_End_Offset--;
    if (Library_End_Offset==0)
        return; //No good info

    //Accepting good data after junk
    size_t Library_Start_Offset=Library_End_Offset-1;
    while (Library_Start_Offset>0 && (Buffer[Buffer_Offset+Library_Start_Offset-1]>=0x20 && Buffer[Buffer_Offset+Library_Start_Offset-1]<=0x7D))
        Library_Start_Offset--;

    //But don't accept non-alpha caracters at the beginning (except for "3ivx")
    if (Library_End_Offset-Library_Start_Offset!=4 || CC4(Buffer+Buffer_Offset+Library_Start_Offset)!=0x33697678) //3ivx
        while (Library_Start_Offset<Element_Size && Buffer[Buffer_Offset+Library_Start_Offset]<=0x40)
            Library_Start_Offset++;

    //Parsing
    Ztring Temp;
    if (Library_Start_Offset>0)
        Skip_XX(Library_Start_Offset,                           "junk");
    if (Library_End_Offset-Library_Start_Offset)
        Get_Local(Library_End_Offset-Library_Start_Offset, Temp,"data");
    if (Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "junk");

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
    DVD_CC_IsPresent=true;

    Element_Info("DVD captioning");

    if (Count_Get(Stream_Text))
        return;

    //Parsing
    int8u cc_count;
    Skip_B4(                                                    "identifier");
    BS_Begin();
    Skip_SB(                                                    "field 1 then field 2");
    Get_S1 (7, cc_count,                                        "count");
    BS_End();
    for (int8u Pos=0; Pos<cc_count; Pos++)
    {
        Element_Begin("cc");
        int8u cc_type, cc_data_1, cc_data_2;
        BS_Begin();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Get_S1 (1, cc_type,                                     "cc_type");
        BS_End();

        while (cc_type>=DVD_CC_Parsers.size())
            DVD_CC_Parsers.push_back(NULL);
        if (DVD_CC_Parsers[cc_type]==NULL)
            DVD_CC_Parsers[cc_type]=new File_Eia608();
        Open_Buffer_Init(DVD_CC_Parsers[cc_type]);
        Open_Buffer_Continue(DVD_CC_Parsers[cc_type], Buffer+Buffer_Offset+(size_t)Element_Offset, 2);

        //Demux
        Demux(Buffer+Buffer_Offset+(size_t)Element_Offset, 2, Ztring::ToZtring(cc_type)+_T(".eia608"));

        Element_Offset+=2;
        Element_End();
    }
}

//---------------------------------------------------------------------------
// Packet "B2", DTG1
void File_Mpegv::user_data_start_DTG1()
{
    Element_Info("Active Format Description");

    //Parsing
    bool active_format_flag;
    Skip_B4(                                                    "afd_identifier");
    BS_Begin();
    Mark_0();
    Get_SB (active_format_flag,                                 "active_format_flag");
    Mark_0_NoTrustError();
    Mark_0_NoTrustError();
    Mark_0_NoTrustError();
    Mark_0_NoTrustError();
    Mark_0_NoTrustError();
    Mark_1_NoTrustError();
    if (active_format_flag)
    {
        Mark_1_NoTrustError();
        Mark_1_NoTrustError();
        Mark_1_NoTrustError();
        Mark_1_NoTrustError();
        Info_S1(4, active_format,                               "active_format"); Param_Info(Mpegv_user_data_DTG1_active_format[active_format]);
    }
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
    GA94_03_CC_IsPresent=true;

    Element_Info("Styled captioning");

    //Parsing
    int8u  cc_count;
    bool   process_em_data_flag, process_cc_data_flag, additional_data_flag;
    BS_Begin();
    Get_SB (process_em_data_flag,                               "process_em_data_flag");
    Get_SB (process_cc_data_flag,                               "process_cc_data_flag");
    Get_SB (additional_data_flag,                               "additional_data_flag");
    Get_S1 (5, cc_count,                                        "cc_count");
    BS_End();
    Skip_B1(                                                    process_em_data_flag?"em_data":"junk"); //Emergency message
    if (TemporalReference[TemporalReference_Offset+temporal_reference].GA94_03_CC.size()<cc_count)
        TemporalReference[TemporalReference_Offset+temporal_reference].GA94_03_CC.resize(cc_count);
    if (process_cc_data_flag)
    {
        for (int8u Pos=0; Pos<cc_count; Pos++)
        {
            Element_Begin("cc");
            int8u cc_type, cc_data_1, cc_data_2;
            bool   cc_valid;
            BS_Begin();
            Mark_1();
            Mark_1();
            Mark_1();
            Mark_1();
            Mark_1();
            Get_SB (   cc_valid,                                    "cc_valid");
            Get_S1 (2, cc_type,                                     "cc_type");
            BS_End();
            Get_B1 (cc_data_1,                                      "cc_data_1");
            Get_B1 (cc_data_2,                                      "cc_data_2");
            TemporalReference[TemporalReference_Offset+temporal_reference].GA94_03_CC[Pos].cc_valid=cc_valid;
            TemporalReference[TemporalReference_Offset+temporal_reference].GA94_03_CC[Pos].cc_type=cc_type;
            TemporalReference[TemporalReference_Offset+temporal_reference].GA94_03_CC[Pos].cc_data[0]=cc_data_1;
            TemporalReference[TemporalReference_Offset+temporal_reference].GA94_03_CC[Pos].cc_data[1]=cc_data_2;
            Element_End();
        }
    }
    else
        Skip_XX(cc_count*2,                                         "Junk");

    //Parsing Captions after reordering
    bool CanBeParsed=true;
    for (size_t GA94_03_CC_Pos=TemporalReference_GA94_03_CC_Offset; GA94_03_CC_Pos<TemporalReference.size(); GA94_03_CC_Pos++)
        if (!TemporalReference[GA94_03_CC_Pos].IsValid)
            CanBeParsed=false; //There is a missing field/frame
    if (CanBeParsed)
    {
       for (size_t GA94_03_CC_Pos=TemporalReference_GA94_03_CC_Offset; GA94_03_CC_Pos<TemporalReference.size(); GA94_03_CC_Pos++)
            for (int8u Pos=0; Pos<cc_count; Pos++)
            {
                if (Pos<TemporalReference[GA94_03_CC_Pos].GA94_03_CC.size() && TemporalReference[GA94_03_CC_Pos].GA94_03_CC[Pos].cc_valid)
                {
                    int8u cc_type=TemporalReference[GA94_03_CC_Pos].GA94_03_CC[Pos].cc_type;
                    size_t Parser_Pos=cc_type;
                    if (Parser_Pos==3)
                        Parser_Pos=2; //cc_type 2 and 3 are for the same text

                    while (Parser_Pos>=GA94_03_CC_Parsers.size())
                        GA94_03_CC_Parsers.push_back(NULL);
                    if (GA94_03_CC_Parsers[Parser_Pos]==NULL)
                        GA94_03_CC_Parsers[Parser_Pos]=cc_type<2?(File__Analyze*)new File_Eia608():(File__Analyze*)new File_Eia708();
                    if (cc_type>=2)
                        ((File_Eia708*)GA94_03_CC_Parsers[2])->cc_type=cc_type;
                    Open_Buffer_Init(GA94_03_CC_Parsers[Parser_Pos]);
                    Open_Buffer_Continue(GA94_03_CC_Parsers[Parser_Pos], TemporalReference[GA94_03_CC_Pos].GA94_03_CC[Pos].cc_data, 2);

                    //Demux
                    if (cc_type<2)
                        Demux(TemporalReference[GA94_03_CC_Pos].GA94_03_CC[Pos].cc_data, 2, Ztring::ToZtring(cc_type)+_T(".eia608"));
                    else
                        Demux(TemporalReference[GA94_03_CC_Pos].GA94_03_CC[Pos].cc_data, 2, _T("eia708"));
                }
            }

        TemporalReference_GA94_03_CC_Offset=TemporalReference.size();
    }

    BS_Begin();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    BS_End();

    if (additional_data_flag)
        Skip_XX(Element_Size-Element_Offset,                    "additional_user_data");
}

//---------------------------------------------------------------------------
// Packet "B2", GA94 0x06 (bar data)
void File_Mpegv::user_data_start_GA94_06()
{
    Element_Info("Bar data");

    //Parsing
    bool   top_bar_flag, bottom_bar_flag, left_bar_flag, right_bar_flag;
    BS_Begin();
    Get_SB (top_bar_flag,                                       "top_bar_flag");
    Get_SB (bottom_bar_flag,                                    "bottom_bar_flag");
    Get_SB (left_bar_flag,                                      "left_bar_flag");
    Get_SB (right_bar_flag,                                     "right_bar_flag");
    Mark_1_NoTrustError();
    Mark_1_NoTrustError();
    Mark_1_NoTrustError();
    Mark_1_NoTrustError();
    BS_End();
    if (top_bar_flag)
    {
        Mark_1();
        Mark_1();
        Skip_S2(14,                                             "line_number_end_of_top_bar");
    }
    if (bottom_bar_flag)
    {
        Mark_1();
        Mark_1();
        Skip_S2(14,                                             "line_number_start_of_bottom_bar");
    }
    if (left_bar_flag)
    {
        Mark_1();
        Mark_1();
        Skip_S2(14,                                             "pixel_number_end_of_left_bar");
    }
    if (right_bar_flag)
    {
        Mark_1();
        Mark_1();
        Skip_S2(14,                                             "pixel_number_start_of_right_bar");
    }
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    BS_End();

    if (Element_Size-Element_Offset)
        Skip_XX(Element_Size-Element_Offset,                    "additional_bar_data");
}

//---------------------------------------------------------------------------
// Packet "B3"
void File_Mpegv::sequence_header()
{
    Element_Name("sequence_header");

    //Reading
    bool  load_intra_quantiser_matrix, load_non_intra_quantiser_matrix;
    BS_Begin();
    Get_S2 (12, horizontal_size_value,                          "horizontal_size_value");
    Get_S2 (12, vertical_size_value,                            "vertical_size_value");
    Get_S1 ( 4, aspect_ratio_information,                       "aspect_ratio_information"); if (vertical_size_value && Mpegv_aspect_ratio1[aspect_ratio_information]) Param_Info((float)horizontal_size_value/vertical_size_value/Mpegv_aspect_ratio1[aspect_ratio_information]); Param_Info(Mpegv_aspect_ratio2[aspect_ratio_information]);
    Get_S1 ( 4, frame_rate_code,                                "frame_rate_code"); Param_Info(Mpegv_frame_rate[frame_rate_code]);
    Get_S3 (18, bit_rate_value,                                 "bit_rate_value"); Param_Info(bit_rate_value*400);
    Mark_1 ();
    Info_S2(10, vbv_buffer_size_value,                          "vbv_buffer_size_value"); Param_Info(16*1024*vbv_buffer_size_value);
    Skip_SB(                                                    "constrained_parameters_flag");
    TEST_SB_GET(load_intra_quantiser_matrix,                    "load_intra_quantiser_matrix");
        for (size_t Pos=0; Pos<64; Pos++)
        {
            int8u intra_quantiser;
            Get_S1 (8, intra_quantiser,                         "intra_quantiser");
            Ztring Value=Ztring::ToZtring(intra_quantiser, 16);
            if (Value.size()==1)
                Value.insert(0, _T("0"));
            Matrix_intra+=Value;
        }
    TEST_SB_END();
    TEST_SB_GET(load_non_intra_quantiser_matrix,                "load_non_intra_quantiser_matrix");
        for (size_t Pos=0; Pos<64; Pos++)
        {
            int8u non_intra_quantiser;
            Get_S1 (8, non_intra_quantiser,                     "non_intra_quantiser");
            Ztring Value=Ztring::ToZtring(non_intra_quantiser, 16);
            if (Value.size()==1)
                Value.insert(0, _T("0"));
            Matrix_nonintra+=Value;
        }
    TEST_SB_END();
    BS_End();

    FILLING_BEGIN();
        //Temporal reference
        TemporalReference_Offset=TemporalReference.size();
        if (TemporalReference_Offset>=0x800)
        {
            TemporalReference.erase(TemporalReference.begin(), TemporalReference.begin()+0x400);
            TemporalReference_Offset-=0x400;
            TemporalReference_GA94_03_CC_Offset-=0x400;
        }

        if (sequence_header_IsParsed)
            return;

        //NextCode
        NextCode_Clear();
        NextCode_Add(0x00);
        NextCode_Add(0xB2);
        NextCode_Add(0xB5);
        NextCode_Add(0xB8);

        //Autorisation of other streams
        Streams[0x00].Searching_Payload=true;
        Streams[0xB2].Searching_Payload=true;
        Streams[0xB5].Searching_Payload=true;
        Streams[0xB8].Searching_TimeStamp_Start=true;
        Streams[0xB8].Searching_TimeStamp_End=true;

        //Temp
        FrameRate=Mpegv_frame_rate[frame_rate_code];
        SizeToAnalyse_Begin=bit_rate_value*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure to have at least 2 timestamps (for integrity checking)
        SizeToAnalyse_End=bit_rate_value*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure

        //Setting as OK
        sequence_header_IsParsed=true;
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
        case 1 :{ //Sequence
                    //Parsing
                    Skip_SB(                                    "profile_and_level_indication_escape");
                    Get_S1 ( 3, profile_and_level_indication_profile, "profile_and_level_indication_profile"); Param_Info(Mpegv_profile_and_level_indication_profile[profile_and_level_indication_profile]);
                    Get_S1 ( 4, profile_and_level_indication_level, "profile_and_level_indication_level"); Param_Info(Mpegv_profile_and_level_indication_level[profile_and_level_indication_level]);
                    Get_SB (    progressive_sequence,           "progressive_sequence");
                    Get_S1 ( 2, chroma_format,                  "chroma_format"); Param_Info(Mpegv_Colorimetry_format[chroma_format]);
                    Get_S1 ( 2, horizontal_size_extension,      "horizontal_size_extension");
                    Get_S1 ( 2, vertical_size_extension,        "vertical_size_extension");
                    Get_S2 (12, bit_rate_extension,             "bit_rate_extension");
                    Mark_1 ();
                    Skip_S1( 8,                                 "vbv_buffer_size_extension");
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
        case 2 :{ //Sequence Display
                    //Parsing
                    Get_S1 ( 3, video_format,                   "video_format"); Param_Info(Mpegv_video_format[video_format]);
                    TEST_SB_SKIP(                               "load_intra_quantiser_matrix");
                        Skip_S1( 8,                             "colour_primaries");
                        Skip_S1( 8,                             "transfer_characteristics");
                        Skip_S1( 8,                             "matrix_coefficients");
                    TEST_SB_END();
                    Get_S2 (14, display_horizontal_size,        "display_horizontal_size");
                    Mark_1 ();
                    Get_S2 (14, display_vertical_size,          "display_vertical_size");
                    BS_End();
                }
                break;
        case 8 :{ //Picture Coding
                    //Parsing
                    bool progressive_frame;
                    Skip_S1( 4,                                 "f_code_forward_horizontal");
                    Skip_S1( 4,                                 "f_code_forward_vertical");
                    Skip_S1( 4,                                 "f_code_backward_horizontal");
                    Skip_S1( 4,                                 "f_code_backward_vertical");
                    Skip_S1( 2,                                 "intra_dc_precision");
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
                                TemporalReference[TemporalReference_Offset+temporal_reference].progressive_frame=progressive_frame;
                                TemporalReference[TemporalReference_Offset+temporal_reference].top_field_first=top_field_first;
                                TemporalReference[TemporalReference_Offset+temporal_reference].repeat_first_field=repeat_first_field;
                                TemporalReference[TemporalReference_Offset+temporal_reference].IsValid=true;
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
                                TemporalReference[TemporalReference_Offset+temporal_reference].progressive_frame=progressive_frame;
                                TemporalReference[TemporalReference_Offset+temporal_reference].top_field_first=top_field_first;
                                TemporalReference[TemporalReference_Offset+temporal_reference].repeat_first_field=repeat_first_field;
                                TemporalReference[TemporalReference_Offset+temporal_reference].IsValid=true;
                            }
                        }

                        if (picture_structure==2) //Bottom, and we want to add a frame only one time if 2 fields
                            Time_End_Frames--; //One frame
                    FILLING_END();
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

    if (!IsFilled && sequence_header_IsParsed)
        slice_start_Fill();
}

//---------------------------------------------------------------------------
// Packet "B8"
void File_Mpegv::group_start()
{
    if (!NextCode_Test())
        return;
    Element_Name("group_start");

    //Reading
    int8u Hours, Minutes, Seconds, Frames;
    bool drop_frame_flag, closed_gop, broken_link;
    BS_Begin();
    Get_SB (    drop_frame_flag,                                "time_code_drop_frame_flag");
    Get_S1 ( 5, Hours,                                          "time_code_time_code_hours");
    Get_S1 ( 6, Minutes,                                        "time_code_time_code_minutes");
    Mark_1();
    Get_S1 ( 6, Seconds,                                        "time_code_time_code_seconds");
    Get_S1 ( 6, Frames,                                         "time_code_time_code_pictures");
    Get_SB (    closed_gop,                                     "closed_gop");
    Get_SB (    broken_link,                                    "broken_link");
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

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0x00);
        NextCode_Add(0xB2);
        NextCode_Add(0xB5);
        NextCode_Add(0xB8);

        if (TimeCodeIsNotTrustable)
            return;

        //Calculating
        if (Time_Begin_Seconds==Error)
        {
            Time_Begin_Seconds=60*60*Hours+60*Minutes+Seconds;
            Time_Begin_Frames =Frames;
        }
        if (Time_Begin_Seconds==Error)
        {
            //Verifying if time_code is trustable
            if ((size_t)60*60*Hours+60*Minutes+Seconds==Time_Begin_Seconds && Frames==Time_Begin_Frames)
                Time_End_NeedComplete=true; //we can't trust time_code
        }
        if (!Time_End_NeedComplete)
        {
            Time_End_Seconds=60*60*Hours+60*Minutes+Seconds;
            Time_End_Frames =Frames;
        }
        if (!group_start_IsParsed)
        {
            group_start_IsParsed=true;
            group_start_drop_frame_flag=drop_frame_flag;
            group_start_closed_gop=closed_gop;
            group_start_broken_link=broken_link;
        }

        //Autorisation of other streams
        if (Searching_TimeStamp_Start_DoneOneTime)
        {
            Streams[0xB8].Searching_TimeStamp_Start=false; //group_start
            SizeToAnalyse_Begin=0;
        }
        else
            Searching_TimeStamp_Start_DoneOneTime=true;
        Streams[0x00].Searching_TimeStamp_End=true; //picture_start
    FILLING_END();
}

} //NameSpace

#endif //MEDIAINFO_MPEGV_YES

