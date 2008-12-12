// File_Mpegv - Info for MPEG Video files
// Copyright (C) 2004-2008 Jerome Martinez, Zen@MediaArea.net
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
// Constants
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
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
namespace MediaInfoLib
{
//---------------------------------------------------------------------------

//***************************************************************************
// Constants
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

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpegv::File_Mpegv()
:File__Analyze()
{
    //In
    MPEG_Version=1;
    Frame_Count_Valid=32;
    FrameIsAlwaysComplete=false;

    //temp
    SizeToAnalyse_Begin=1*1024*1024;
    SizeToAnalyse_End=1*1024*1024;
    Time_Begin_Seconds_IsFrozen=false;
    Searching_TimeStamp_Start_DoneOneTime=false;
    sequence_header_IsParsed=false;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonMPEGV())
        return;
}

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Finalize()
{
    if (Streams.empty())
        return; //Not initialized

    //In case of partial data, and finalizing is forced (example: DecConfig in .mp4), but with at least one frame
    if (Count_Get(Stream_General)==0 && sequence_header_IsParsed)
        slice_start_Fill();

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
        if (Count_Get(Stream_Video) && Time_End>Time_Begin)
            Fill(Stream_Video, 0, Video_Duration, Time_End-Time_Begin);
    }

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Streams.clear();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpegv::Header_Begin()
{
    //Trailing 0xFF
    if (Synched)
    {
        while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]==0xFF)
            Buffer_Offset++;
    }

    //Trailing 0x00
    if (Synched)
    {
        while(Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset]==0x00 && CC3(Buffer+Buffer_Offset)!=0x000001)
            Buffer_Offset++;
    }

    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC3(Buffer+Buffer_Offset)!=0x000001)
    {
        Trusted_IsNot("MPEG-V, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //Quick search
    if (!Header_Parser_QuickSearch())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Mpegv::Header_Parse()
{
    //Parsing
    int8u start_code;
    Skip_B3(                                                    "synchro");
    Get_B1 (start_code,                                         "start_code");
    if (!Header_Parse_Fill_Size())
    {
        Element_WaitForMoreData();
        return;
    }

    //Filling
    Header_Fill_Code(start_code, Ztring().From_CC1(start_code));
}

//---------------------------------------------------------------------------
bool File_Mpegv::Header_Parse_Fill_Size()
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
            else if (Element_Code==0xE0 && Element_Size>=2 && CC2(Buffer+Buffer_Offset)==0x0000)
            {
                Trusted=0; //This is surely an extract from MPEG-TS
                Trusted_IsNot("Unattended element");
            }
            else
                Trusted_IsNot("Unattended element");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "00"
void File_Mpegv::picture_start()
{
    //DETAILLEVEL_SET(0);

    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;
    if (Frame_Count==4)
        File_MaximumOffset*=Frame_Count_Valid; //We are nearly sure this is a Mpegv stream, augmenting parsing limit
    Frame_Count_InThisBlock++;
    
    //Name
    Element_Name("picture_start");
    Element_Info(Ztring(_T("Field ")+Ztring::ToZtring(Frame_Count)));

    //Parsing
    int8u picture_coding_type;
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

    //Details
    if (MediaInfoLib::Config.Details_Get())
    {
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
    }

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

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        for (int64u Element_Name_Next=0x01; Element_Name_Next<=0x1F; Element_Name_Next++)
            NextCode_Add(Element_Name_Next);
        NextCode_Add(0xB2);
        NextCode_Add(0xB5);

        //Autorisation of other streams
        if (Streams[0x00].Searching_Payload)
            for (int8u Pos=0x01; Pos<=0xAF; Pos++)
                Streams[Pos].Searching_Payload=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "01" --> "AF"
void File_Mpegv::slice_start()
{
    //DETAILLEVEL_SET(1);
    Element_Name("slice_start");

    //Parsing
    Skip_XX(Element_Size,                                       "data");

    FILLING_BEGIN();
        //NextCode
        NextCode_Test();
        NextCode_Clear();
        NextCode_Add(0x00);
        for (int8u Element_Name_Next=(int8u)Element_Code; Element_Name_Next<=0x1F; Element_Name_Next++)
            NextCode_Add(Element_Name_Next);
        NextCode_Add(0xB3);
        NextCode_Add(0xB8);

        //Filling only if not already done
        if (Frame_Count>=Frame_Count_Valid && Count_Get(Stream_Video)==0)
            slice_start_Fill();

        //Autorisation of other streams
        for (int8u Pos=0x01; Pos<=0xAF; Pos++)
            Streams[Pos].Searching_Payload=false;
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
        for (std::map<int16u, temporalreference>::iterator Temp=TemporalReference.begin(); Temp!=TemporalReference.end(); Temp++)
        {
            TempRef+=Temp->second.top_field_first?"T":"B";
            TempRef+=Temp->second.repeat_first_field?"3":"2";
        }
        if (TempRef.find('3')!=std::string::npos)
        {
            if (TempRef.find('3')!=std::string::npos) //A pulldown maybe is detected
            {
                if (TempRef.find("T2T3B2B3T2T3B2B3")!=std::string::npos)
                    Fill(Stream_Video, 0, Video_ScanOrder, "2:3 Pulldown", Unlimited, true, true);
                if (TempRef.find("B2B3T2T3B2B3T2T3")!=std::string::npos)
                    Fill(Stream_Video, 0, Video_ScanOrder, "2:3 Pulldown", Unlimited, true, true);
                if (TempRef.find("T2T2T2T2T2T2T2T2T2T2T2T3B2B2B2B2B2B2B2B2B2B2B2B3")!=std::string::npos)
                    Fill(Stream_Video, 0, Video_ScanOrder, "2:2:2:2:2:2:2:2:2:2:2:3 Pulldown", Unlimited, true, true);
                if (TempRef.find("B2B2B2B2B2B2B2B2B2B2B2B3T2T2T2T2T2T2T2T2T2T2T2T3")!=std::string::npos)
                    Fill(Stream_Video, 0, Video_ScanOrder, "2:2:2:2:2:2:2:2:2:2:2:3 Pulldown", Unlimited, true, true);
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
    for (int8u Pos=0x00; Pos<0xB9; Pos++)
    {
        Streams[Pos].Searching_Payload=false;
        Streams[Pos].Searching_TimeStamp_End=false;
    }
    Streams[0x00].Searching_Payload=false; //picture_start
    Streams[0x00].Searching_TimeStamp_End=true; //picture_start
    Streams[0xB5].Searching_Payload=false; //extension_start
    Streams[0xB5].Searching_TimeStamp_End=true; //extension_start
    Streams[0xB8].Searching_Payload=true; //group_start
    Streams[0xB8].Searching_TimeStamp_End=true; //group_start
    Streams[0xB9].Searching_Payload=true; //sequence_end

    //Jumping
    Detect_EOF();
}

//---------------------------------------------------------------------------
// Packet "B2"
void File_Mpegv::user_data_start()
{
    Element_Name("user_data_start");

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
        while (Buffer[Buffer_Offset+Library_Start_Offset]<=0x40)
            Library_Start_Offset++;

    //Parsing
    Ztring Temp;
    if (Library_Start_Offset>0)
        Skip_XX(Library_Start_Offset,                           "junk");
    Get_Local(Library_End_Offset-Library_Start_Offset, Temp,    "data");
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
// Packet "B3"
void File_Mpegv::sequence_header()
{
    //DETAILLEVEL_SET(0);
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
        TemporalReference_Offset+=0x800; //Twice the value for cycle

        //NextCode
        NextCode_Clear();
        NextCode_Add(0xB2);
        NextCode_Add(0xB5);
        NextCode_Add(0xB8);

        //Autorisation of other streams
        Streams[0x00].Searching_Payload=true;
        for (int8u Pos=0xB0; Pos<0xB9; Pos++)
            Streams[Pos].Searching_Payload=true;
        Streams[0xB8].Searching_TimeStamp_Start=true;
        Streams[0xB8].Searching_TimeStamp_End=true;

        //Temp
        FrameRate=Mpegv_frame_rate[frame_rate_code];
        SizeToAnalyse_Begin=bit_rate_value*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure to have at least 2 timestamps (for integrity checking)
        SizeToAnalyse_End=bit_rate_value*50; //standard delay between TimeStamps is 0.7s, we try 1s to be sure

        //Setting as OK
        sequence_header_IsParsed=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B4"
void File_Mpegv::sequence_error()
{
    //DETAILLEVEL_SET(0);
    Element_Name("sequence_error");
}

//---------------------------------------------------------------------------
// Packet "B5"
void File_Mpegv::extension_start()
{
    //DETAILLEVEL_SET(0);
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
                                if (TemporalReference.size()<30)
                                {
                                    if (temporal_reference<=30)
                                        temporal_reference+=0x400; //10 bits cyclic, avoiding cases of the limit
                                    temporal_reference+=TemporalReference_Offset;
                                    TemporalReference[temporal_reference].top_field_first=top_field_first;
                                    TemporalReference[temporal_reference].repeat_first_field=repeat_first_field;
                                }
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
                            progressive_frame_Count++;

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
    //DETAILLEVEL_SET(0);
    Element_Name("sequence_end");

    if (Frame_Count>0)
        slice_start_Fill();
}

//---------------------------------------------------------------------------
// Packet "B8"
void File_Mpegv::group_start()
{
    //DETAILLEVEL_SET(0);
    Element_Name("group_start");

    //Reading
    int8u Hours, Minutes, Seconds, Frames;
    BS_Begin();
    Skip_SB(                                                    "time_code_drop_frame_flag");
    Get_S1 ( 5, Hours,                                          "time_code_time_code_hours");
    Get_S1 ( 6, Minutes,                                        "time_code_time_code_minutes");
    Skip_SB(                                                    "time_code_marker_bit");
    Get_S1 ( 6, Seconds,                                        "time_code_time_code_seconds");
    Get_S1 ( 6, Frames,                                         "time_code_time_code_pictures");
    Skip_SB(                                                    "closed_gop");
    Skip_SB(                                                    "broken_link");
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

        //Calculating
        if (Time_Begin_Seconds!=Error || Time_Begin_Seconds_IsFrozen)
        {
            if (Time_Begin_Seconds==(size_t)(60*60*Hours+60*Minutes+Seconds)
             && Time_Begin_Frames ==Frames)
            {
                //Same as before, there is a problem at starting of the time
                Time_Begin_Seconds_IsFrozen=true;
                SizeToAnalyse_Begin=SizeToAnalyse_End*10; //10s
                Time_Begin_Seconds=Error;
                Time_Begin_Frames =(int8u)-1;
                Searching_TimeStamp_Start_DoneOneTime=false;
            }
            else if (Time_Begin_Seconds_IsFrozen)
            {
                //We consider this is the first valid time_code
                Time_Begin_Seconds_IsFrozen=false;
                SizeToAnalyse_Begin=SizeToAnalyse_End*2; //2s
                Time_Begin_Seconds=Error;
                Time_Begin_Frames =(int8u)-1;
                Searching_TimeStamp_Start_DoneOneTime=true;
            }
        }
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

        //Autorisation of other streams
        if (Searching_TimeStamp_Start_DoneOneTime)
            Streams[0xB8].Searching_TimeStamp_Start=false; //group_start
        else
            Searching_TimeStamp_Start_DoneOneTime=true;
        Streams[0x00].Searching_TimeStamp_End=true; //picture_start
        Streams[0xB5].Searching_TimeStamp_End=true; //extension_start
    FILLING_END();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpegv::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+4<=Buffer_Size)
    {
        while (Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset]!=0x00)
            Buffer_Offset++;
        if (Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset+1]==0x00)
            if (Buffer[Buffer_Offset+2]==0x01)
                break;
        Buffer_Offset++;
    }
    if (Buffer_Offset+4>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+3==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset)!=0x000001)
            {
                Buffer_Offset++;
                if (CC2(Buffer+Buffer_Offset)!=0x0000)
                {
                    Buffer_Offset++;
                    if (Buffer[Buffer_Offset]!=0x00)
                        Buffer_Offset++;
                }
            }
        }

        return false;
    }

    //Synched is OK
    Synched=true;
    if (Streams.empty())
    {
        //Count of a Packets
        Frame_Count=0;
        progressive_frame_Count=0;
        Interlaced_Top=0;
        Interlaced_Bottom=0;

        //From picture_start
        display_horizontal_size=0;
        display_vertical_size=0;
        Time_Begin_Seconds=Error;
        Time_Begin_Frames=(int8u)-1;
        Time_End_Seconds=Error;
        Time_End_Frames=(int8u)-1;
        bit_rate_value=0;
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
        Time_End_NeedComplete=false;
        load_intra_quantiser_matrix=false;
        load_non_intra_quantiser_matrix=false;
        progressive_sequence=true; //progressive by default
        top_field_first=false;
        repeat_first_field=false;
        FirstFieldFound=false;
        TemporalReference_Offset=0;

        //Default stream values
        Streams.resize(0x100);
        Streams[0xB3].Searching_Payload=true;
        for (int8u Pos=0xB9; Pos!=0x00; Pos++)
            Streams[Pos].Searching_Payload=true; //Testing MPEG-PS

        //Temp
        FrameRate=0;
    }
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

        //Searching start
        if (Streams[start_code].Searching_Payload)
            return true;

        //Searching TimeStamp_End
        if (Streams[start_code].Searching_TimeStamp_End)
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

    if (Buffer_Offset+4<=Buffer_Size)
    {
        if(File_Offset+Buffer_Size==File_Size && Count_Get(Stream_General)==0 && Frame_Count>=2)
            slice_start_Fill(); //End of file, and we have some frames
        else
            Trusted_IsNot("Mpegv, Synchronisation lost");
    }
    Synched=false;
    return Synchronize();
}

//---------------------------------------------------------------------------
bool File_Mpegv::Detect_NonMPEGV ()
{
    //File_Size
    if (File_Size<188*4)
        return false; //We can't do detection

    //Element_Size
    if (Buffer_Size<188*4)
        return true; //Must wait for more data

    //Detect mainly DAT files, and the parser is not enough precise to detect them later
    if (CC4(Buffer)==CC4("RIFF"))
    {
        Finished();
        return true;
    }

    //Detect TS files, and the parser is not enough precise to detect them later
    while (Buffer_Offset<188 && Buffer[Buffer_Offset]!=0x47) //Look for first Sync word
        Buffer_Offset++;
    if (Buffer_Offset<188 && Buffer[Buffer_Offset+188]==0x47 && Buffer[Buffer_Offset+188*2]==0x47 && Buffer[Buffer_Offset+188*3]==0x47)
    {
        Finished();
        return true;
    }
    Buffer_Offset=0;

    //Seems OK
    return false;
}

//---------------------------------------------------------------------------
void File_Mpegv::Detect_EOF()
{
    if (Count_Get(Stream_Video)
     && (File_Size>SizeToAnalyse_Begin+SizeToAnalyse_End && File_Offset+Buffer_Offset+Element_Offset>SizeToAnalyse_Begin && File_Offset+Buffer_Offset+Element_Offset<File_Size-SizeToAnalyse_End && MediaInfoLib::Config.ParseSpeed_Get()<=0.01
      || File_Name.empty()))
    {
        //
        NextCode_Clear();
        Time_End_Seconds=Error;
        Time_End_Frames=(int8u)-1;

        //Waiting for a group_start
        if (!IsSub)
            Streams[0x00].Searching_TimeStamp_End=false; //picture_start
        Streams[0xB5].Searching_TimeStamp_End=false; //extension_start

        //Jumping
        Data_GoTo(File_Size-(IsSub?0:SizeToAnalyse_End), "MPEG-V");
    }
}

} //NameSpace

#endif //MEDIAINFO_MPEGV_YES

