// File_Avc - Info for AVC Video files
// Copyright (C) 2006-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_AVC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Avc.h"
#include "math.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const size_t Avc_Errors_MaxCount=32;

const int8u Avc_PixelAspectRatio_Size=14;
const float32 Avc_PixelAspectRatio[]=
{
    (float32)1, //Reserved
    (float32)1,
    (float32)12/(float32)11,
    (float32)10/(float32)11,
    (float32)16/(float32)11,
    (float32)40/(float32)33,
    (float32)24/(float32)11,
    (float32)20/(float32)11,
    (float32)32/(float32)11,
    (float32)80/(float32)33,
    (float32)18/(float32)11,
    (float32)15/(float32)11,
    (float32)64/(float32)33,
    (float32)160/(float32)99,
};

const char* Avc_video_format[]=
{
    "Component",
    "PAL",
    "NTSC",
    "SECAM",
    "MAC",
    "",
    "Reserved",
    "Reserved",
};

const char* Avc_primary_pic_type[]=
{
    "I",
    "I, P",
    "I, P, B",
    "SI",
    "SI, SP",
    "I, SI",
    "I, SI, P, SP",
    "I, SI, P, SP, B",
};

const char* Avc_slice_type[]=
{
    "P",
    "B",
    "I",
    "SP",
    "SI",
    "P",
    "B",
    "I",
    "SP",
    "SI",
};

const int8u Avc_pic_struct_Size=9;
const char* Avc_pic_struct[]=
{
    "frame",
    "top field",
    "bottom field",
    "top field, bottom field",
    "bottom field, top field",                                                     
    "top field, bottom field, top field",
    "bottom field, top field, bottom field",
    "frame doubling",
    "frame tripling",
};

const int8u Avc_NumClockTS[]=
{
    1,
    1,
    1,
    2,
    2,
    3,
    3,
    2,
    3,
};

const char* Avc_ct_type[]=
{
    "Progressive",
    "Interlaced",
    "Unknown",
    "Reserved",
};

const char* Avc_chroma_format_idc[]=
{
    "monochrome",
    "4:2:0",
    "4:2:2",
    "4:4:4",
};

const int8u Avc_SubWidthC[]=
{
    1,
    2,
    2,
    1,
};

const int8u Avc_SubHeightC[]=
{
    1,
    2,
    1,
    1,
};

//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Avc::File_Avc()
:File__Analyze()
{
    //In
    Frame_Count_Valid=2;
    FrameIsAlwaysComplete=false;
    MustParse_SPS_PPS=false;
    ShortHeader=false;

    //Count of a Packets
    Frame_Count=0;
    frame_num_LastOne=(int32u)-1;

    //From seq_parameter_set
    pic_width_in_mbs_minus1=0;
    pic_height_in_map_units_minus1=0;
    log2_max_frame_num_minus4=0;
    num_units_in_tick=0;
    time_scale=0;
    chroma_format_idc=1;
    frame_crop_left_offset=0;
    frame_crop_right_offset=0;
    frame_crop_top_offset=0;
    frame_crop_bottom_offset=0;
    sar_width=0;
    sar_height=0;
    profile_idc=0;
    level_idc=0;
    aspect_ratio_idc=0xFF;
    video_format=5;
    cpb_removal_delay_length_minus1=0;
    dpb_output_delay_length_minus1=0;
    time_offset_length=0;
    pic_struct=0;
    pic_struct_FirstDetected=(int8u)-1;
    frame_mbs_only_flag=false;
    timing_info_present_flag=false;
    pic_struct_present_flag=false;
    field_pic_flag=false;
    entropy_coding_mode_flag=false;
    CpbDpbDelaysPresentFlag=false;

    //Default values
    Stream[0x07].Searching_Payload=true; //seq_parameter_set
    Stream[0x09].Searching_Payload=true; //access_unit_delimiter
    for (int8u Pos=0xB9; Pos!=0x00; Pos++)
        Stream[Pos].Searching_Payload=true; //Testing MPEG-PS
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Avc::Read_Buffer_Finalize()
{
    //In case of partial data, and finalizing is forced (example: DecConfig in .mp4), but with at least one frame
    if (Count_Get(Stream_General)==0 && Frame_Count>0)
        slice_header_Fill();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Avc::Header_Begin()
{
    //Specific case
    if (MustParse_SPS_PPS)
        return true;
    if (ShortHeader)
    {
        Synched=true;
        return true;
    }

    //Trailing 0x00
    if (Synched)
    {
        while(Buffer_Offset+3<Buffer_Size && CC1(Buffer+Buffer_Offset)==0x00 && CC3(Buffer+Buffer_Offset)!=0x000001)
            Buffer_Offset++;
    }

    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC3(Buffer+Buffer_Offset)!=0x000001)
    {
        Trusted_IsNot("AVC, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //Quick search
    if (!Header_Parser_QuickSearch())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Avc::Header_Parse()
{
    //Specific case
    if (MustParse_SPS_PPS)
    {
        Header_Fill_Size(Element_Size);
        Header_Fill_Code((int64u)-1, "Specific");
        return;
    }

    //Parsing
    int8u nal_unit_type;
    if (!ShortHeader)
        Skip_B3(                                                "sync");
    else
    {
        int32u size;
        Get_B4 (size,                                           "size");
        Header_Fill_Size(Element_Offset+size);
    }
    BS_Begin();
    Mark_0 ();
    Skip_S1( 2,                                                 "nal_ref_idc");
    Get_S1 ( 5, nal_unit_type,                                  "nal_unit_type");
    BS_End();
    if (!ShortHeader && !Header_Parse_Fill_Size())
    {
        Element_WaitForMoreData();
        return;
    }

    //Filling
    Header_Fill_Code(nal_unit_type, Ztring().From_CC1(nal_unit_type));
}

//---------------------------------------------------------------------------
bool File_Avc::Header_Parse_Fill_Size()
{
    //Look for next Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if Header_Parse_Fill_Size() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset+4;
    while (Buffer_Offset_Temp+4<=Buffer_Size
        && CC3(Buffer+Buffer_Offset_Temp)!=0x000001)
        Buffer_Offset_Temp++;

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (FrameIsAlwaysComplete || File_Offset+Buffer_Size==File_Size)
            Buffer_Offset_Temp=Buffer_Size; //We are sure that the next bytes are a start
        else
            return false;
    }

    //Keeping out trailing zeroes
     while (CC3(Buffer+Buffer_Offset_Temp-1)==0x000000)
        Buffer_Offset_Temp--;

    //OK, we continue
    Header_Fill_Size(Buffer_Offset_Temp-Buffer_Offset);
    Buffer_Offset_Temp=0;
    return true;
}

//---------------------------------------------------------------------------
void File_Avc::Data_Parse()
{
    //Specific case
    if (Element_Code==(int64u)-1)
    {
        SPS_PPS();
        return;
    }

    //Searching emulation_prevention_three_byte
    int8u* Buffer_3Bytes=NULL;
    const int8u* Save_Buffer=Buffer;
    size_t Save_Buffer_Offset=Buffer_Offset;
    int64u Save_Element_Size=Element_Size;
    size_t Element_Offset_3Bytes=0;
    std::vector<size_t> ThreeByte_List;
    while (Element_Offset_3Bytes+3<=Element_Size)
    {
        if (CC3(Buffer+Buffer_Offset+Element_Offset_3Bytes)==0x000003)
            ThreeByte_List.push_back(Element_Offset_3Bytes+2);
        Element_Offset_3Bytes++;
    }
    if (!ThreeByte_List.empty())
    {
        //We must change the buffer for keeping out
        Element_Size=Save_Element_Size-ThreeByte_List.size();
        Buffer_Offset=0;
        Buffer_3Bytes=new int8u[(size_t)Element_Size];
        for (size_t Pos=0; Pos<=ThreeByte_List.size(); Pos++)
        {
            size_t Pos0=(Pos==ThreeByte_List.size())?(size_t)Save_Element_Size:(ThreeByte_List[Pos]);
            size_t Pos1=(Pos==0)?0:(ThreeByte_List[Pos-1]+1);
            size_t Buffer_3bytes_Begin=Pos1-Pos;
            size_t Save_Buffer_Begin  =Pos1;
            size_t Size=               Pos0-Pos1;
            std::memcpy(Buffer_3Bytes+Buffer_3bytes_Begin, Save_Buffer+Save_Buffer_Offset+Save_Buffer_Begin, Size);
        }
        Buffer=Buffer_3Bytes;
    }

    //Parsing
    switch (Element_Code)
    {
        case 0x00 : Element_Name("unspecified"); break;
        case 0x01 : slice_layer_without_partitioning_non_IDR(); break;
        case 0x02 : Element_Name("slice_data_partition_a_layer"); break;
        case 0x03 : Element_Name("slice_data_partition_b_layer"); break;
        case 0x04 : Element_Name("slice_data_partition_c_layer"); break;
        case 0x05 : slice_layer_without_partitioning_IDR(); break;
        case 0x06 : sei(); break;
        case 0x07 : seq_parameter_set(); break;
        case 0x08 : pic_parameter_set(); break;
        case 0x09 : access_unit_delimiter(); break;
        case 0x0A : Element_Name("end_of_seq"); break;
        case 0x0B : Element_Name("end_of_stream"); break;
        case 0x0C : Element_Name("filler_data"); break;
        case 0x0D : Element_Name("seq_parameter_set_extension"); break;
        case 0x13 : Element_Name("slice_layer_without_partitioning"); break;
        default :
            if (Element_Code<=0x17)
                Element_Name("reserved");
            else
                Element_Name("unspecified");
    }

    if (!ThreeByte_List.empty())
    {
        //We must change the buffer for keeping out
        Element_Size=Save_Element_Size;
        Buffer_Offset=Save_Buffer_Offset;
        delete[] Buffer; Buffer=Save_Buffer;
        Buffer_3Bytes=NULL; //Same as Buffer...
        Element_Offset+=ThreeByte_List.size();
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "01"
void File_Avc::slice_layer_without_partitioning_non_IDR()
{
    Element_Name("slice_layer_without_partitioning (non-IDR)");

    //Parsing
    slice_header();
}

//---------------------------------------------------------------------------
// Packet "05"
void File_Avc::slice_layer_without_partitioning_IDR()
{
    Element_Name("slice_layer_without_partitioning (IDR)");

    //Parsing
    slice_header();

    FILLING_BEGIN();
        //NextCode
        for (int8u Pos=0x01; Pos<=0x05; Pos++)
            NextCode_Add(Pos);
    FILLING_END();
}

//---------------------------------------------------------------------------
//
void File_Avc::slice_header()
{
    //Parsing
    int32u slice_type, frame_num;
    bool   bottom_field_flag=0;
    BS_Begin();
    Skip_UE(                                                    "first_mb_in_slice");
    Get_UE (slice_type,                                         "slice_type"); if (slice_type<9) Param_Info(Avc_slice_type[slice_type]);
    Skip_UE(                                                    "pic_parameter_set_id");
    Get_BS (log2_max_frame_num_minus4+4, frame_num,             "frame_num");
    if (!frame_mbs_only_flag)
    {
        TEST_SB_GET(field_pic_flag,                             "field_pic_flag");
            Get_SB (bottom_field_flag,                          "bottom_field_flag");
        TEST_SB_END();
    }
    //TODO...
    BS_End();
    Skip_XX(Element_Size-Element_Offset,                        "ToDo...");
    
    FILLING_BEGIN();
        //Counting
        if (File_Offset+Buffer_Offset+Element_Size==File_Size)
            Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
        if (frame_num!=frame_num_LastOne)
        {
            Frame_Count++;
            frame_num_LastOne=frame_num;
        }

        //pic_struct
        if (field_pic_flag && pic_struct_FirstDetected==(int8u)-1)
            pic_struct_FirstDetected=bottom_field_flag?2:1; //2=BFF, 1=TFF

        //Name
        Element_Info(Ztring::ToZtring(Frame_Count));

        //Filling only if not already done
        if (Frame_Count>=Frame_Count_Valid && Count_Get(Stream_Video)==0)
            slice_header_Fill();
    FILLING_END();
}

//---------------------------------------------------------------------------
//
void File_Avc::slice_header_Fill()
{
    //Calculating - Pixels
    int32u Width =(pic_width_in_mbs_minus1       +1)*16;
    int32u Height=(pic_height_in_map_units_minus1+1)*16*(2-frame_mbs_only_flag);
    int32u CropUnitX=Avc_SubWidthC [chroma_format_idc];
    int32u CropUnitY=Avc_SubHeightC[chroma_format_idc]*(2-frame_mbs_only_flag);
    Width -=(frame_crop_left_offset+frame_crop_right_offset )*CropUnitX;
    Height-=(frame_crop_top_offset +frame_crop_bottom_offset)*CropUnitY;

    //Calculating - Profile
    Ztring ProfileS;
    switch (profile_idc)
    {
        case  66 : ProfileS=_T("Baseline"); break;
        case  77 : ProfileS=_T("Main"); break;
        case  88 : ProfileS=_T("Extended"); break;
        case 100 : ProfileS=_T("High"); break;
        case 110 : ProfileS=_T("High 10"); break;
        case 122 : ProfileS=_T("High 4:2:2"); break;
        case 144 : ProfileS=_T("High 4:4:4"); break;
        default   : ProfileS.From_Number(profile_idc); break;
    }

    //Calculating - Level
    Ztring LevelS;
    LevelS.From_Number(((float)level_idc)/10, 1); //Level is Value*10, can have one digit example 5.1

    //Calculating - PixelAspectRatio
    float32 PixelAspectRatio;
    if (aspect_ratio_idc<Avc_PixelAspectRatio_Size)
        PixelAspectRatio=Avc_PixelAspectRatio[aspect_ratio_idc];
    else if (sar_height>0)
        PixelAspectRatio=((float)sar_width)/sar_height;
    else
        PixelAspectRatio=1; //Unknown

    Stream_Prepare(Stream_General);
    Fill("Format", "AVC");
    Stream_Prepare(Stream_Video);
    Fill("Codec", "AVC");

    Fill("Codec_Profile", ProfileS+_T("@L")+LevelS);
    Fill("Width", Width);
    Fill("Height", Height);
    Fill("PixelAspectRatio", PixelAspectRatio);
    if (Height!=0)
        Fill("DisplayAspectRatio", ((float)Width)/Height*PixelAspectRatio);
    Fill("Standard", Avc_video_format[video_format]);
    if (timing_info_present_flag)
    {
        if (!fixed_frame_rate_flag)
            Fill("FrameRate", "VFR");
        else if (time_scale && num_units_in_tick)
            Fill("FrameRate", (float)time_scale/num_units_in_tick/2);
    }
    Fill("Chroma", Avc_chroma_format_idc[chroma_format_idc]);
    if (frame_mbs_only_flag)
        Fill("Interlacement", "PPF");
    if (pic_struct_FirstDetected==1)
        Fill("Interlacement", "TFF");
    if (pic_struct_FirstDetected==2)
        Fill("Interlacement", "BFF");
    Fill(Stream_Video, 0, "Encoded_Library", Encoded_Library);
    Fill(Stream_Video, 0, "Encoded_Library_Settings", Encoded_Library_Settings);
    Fill(Stream_Video, 0, "BitRate_Nominal", BitRate_Nominal);
    if (entropy_coding_mode_flag)
    {
        Fill(Stream_Video, 0, "Codec_Settings", "CABAC");
        Fill(Stream_Video, 0, "Codec_Settings_CABAC", "Yes");
    }
    else
        Fill(Stream_Video, 0, "Codec_Settings_CABAC", "No");

    if (File_Offset+Buffer_Size<File_Size)
    {
        NextCode_Clear();

        Info("AVC, Jumping to end of file");
        Finnished();
    }
}

//---------------------------------------------------------------------------
// Packet "06"
void File_Avc::sei()
{
    Element_Name("sei");

    //Parsing
    while(Element_Offset+1<Element_Size)
    {
        Element_Begin("sei message");
            sei_message();
        Element_End();
    }
    BS_Begin();
    Mark_1(                                                     );
    BS_End();
}

//---------------------------------------------------------------------------
void File_Avc::sei_message()
{
    //Parsing
    int32u payloadType=0, payloadSize=0;
    int8u payload_type_byte, payload_size_byte;
    Element_Begin("sei message header");
        do
        {
            Get_B1 (payload_type_byte,                          "payload_type_byte");
            payloadType+=payload_type_byte;
        }
        while(payload_type_byte==0xFF);
        do
        {
            Get_B1 (payload_size_byte,                          "payload_size_byte");
            payloadSize+=payload_size_byte;
        }
        while(payload_size_byte==0xFF);
    Element_End();

    size_t Element_Offset_Save=Element_Offset+payloadSize;
    switch (payloadType)
    {
        case  0 :   sei_message_buffering_period(payloadSize); break;
        case  1 :   sei_message_pic_timing(payloadSize); break;
        case  5 :   sei_message_user_data_unregistered(payloadSize); break;
        case  6 :   sei_message_recovery_point(payloadSize); break;
        case 32 :   sei_message_mainconcept(payloadSize); break;
        default :
                    Element_Info("unknown");
                    Skip_XX(payloadSize,                        "data");
    }
    Element_Offset=Element_Offset_Save; //Positionning in the right place.
}

//---------------------------------------------------------------------------
void File_Avc::sei_message_buffering_period(int32u payloadSize)
{
    Element_Info("buffering_period");

    //Parsing
    //BS_Begin();
    //Skip_UE(                                                    "seq_parameter_set_id");
    //TODO...
    Skip_XX(payloadSize,                                        "data");
}

//---------------------------------------------------------------------------
void File_Avc::sei_message_pic_timing(int32u payloadSize)
{
    Element_Info("pic_timing");

    //Parsing
    BS_Begin();
    if (CpbDpbDelaysPresentFlag)
    {
        Skip_S1(cpb_removal_delay_length_minus1+1,              "cpb_removal_delay");
        Skip_S1(dpb_output_delay_length_minus1+1,               "dpb_output_delay");
    }
    if (pic_struct_present_flag)
    {
        Get_S1 (4, pic_struct,                                  "pic_struct");
        if (pic_struct>=Avc_pic_struct_Size)
        {
            Param_Info("Reserved");
            return; //NumClockTS is unknown
        }
        Param_Info(Avc_pic_struct[pic_struct]);
        int8u NumClockTS=Avc_NumClockTS[pic_struct];
        int8u seconds_value=0, minutes_value=0, hours_value=0; //Here because theses values can be reused in later ClockTSs.
        for (int8u i=0; i<NumClockTS; i++)
        {
            Element_Begin("ClockTS");
            TEST_SB_SKIP(                                       "clock_timestamp_flag");
                Ztring TimeStamp;
                int32u time_offset=0;
                int8u n_frames;
                bool full_timestamp_flag, nuit_field_based_flag;
                Info_S1(2, ct_type,                             "ct_type"); Param_Info(Avc_ct_type[ct_type]);
                Get_SB (   nuit_field_based_flag,               "nuit_field_based_flag");
                Skip_S1(5,                                      "counting_type");
                Get_SB (   full_timestamp_flag,                 "full_timestamp_flag");
                Skip_SB(                                        "discontinuity_flag");
                Skip_SB(                                        "cnt_dropped_flag");
                Get_S1 (8, n_frames,                            "n_frames");
                if (full_timestamp_flag)
                {
                    Get_S1 (6, seconds_value,                    "seconds_value");
                    Get_S1 (6, minutes_value,                    "minutes_value");
                    Get_S1 (5, hours_value,                      "hours_value");
                }
                else
                {
                    TEST_SB_SKIP(                               "seconds_flag");
                        Get_S1 (6, seconds_value,               "seconds_value");
                        TEST_SB_SKIP(                           "minutes_flag");
                            Get_S1 (6, minutes_value,           "minutes_value");
                            TEST_SB_SKIP(                       "hours_flag");
                                Get_S1 (5, hours_value,         "hours_value");
                            TEST_SB_END();
                        TEST_SB_END();
                    TEST_SB_END();
                }
                TimeStamp=Ztring::ToZtring(hours_value)+_T(':')+Ztring::ToZtring(minutes_value)+_T(':')+Ztring::ToZtring(seconds_value);
                if (time_offset_length>0)
                    Get_S4 (time_offset_length, time_offset,    "time_offset");
                if (time_scale)
                {
                    float32 Milliseconds=((float32)(n_frames*(num_units_in_tick*(1+nuit_field_based_flag))+time_offset))/time_scale;
                    TimeStamp+=_T('.');
                    TimeStamp+=Ztring::ToZtring(Milliseconds);
                }
                Param_Info(TimeStamp);
            TEST_SB_END();
            Element_End();
        }
    }

    FILLING_BEGIN();
        if (pic_struct_FirstDetected==(int8u)-1)
            pic_struct_FirstDetected=pic_struct;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Avc::sei_message_user_data_unregistered(int32u payloadSize)
{
    Element_Info("user_data_unregistered");

    //Parsing
    int128u uuid_iso_iec_11578;
    Get_UUID(uuid_iso_iec_11578,                               "uuid_iso_iec_11578");

    switch (uuid_iso_iec_11578.hi)
    {
        case  0xB748D9E6BDE945DCLL : Element_Info("x264");
                                     sei_message_user_data_unregistered_x264(payloadSize-16); break;
        case  0x684E92AC604A57FBLL : Element_Info("eavc");
                                     sei_message_user_data_unregistered_x264(payloadSize-16); break;
        default :
                    Element_Info("unknown");
                    Skip_XX(payloadSize-8,                      "data");
    }
}

//---------------------------------------------------------------------------
void File_Avc::sei_message_user_data_unregistered_x264(int32u payloadSize)
{
    //Parsing
    Ztring Data;
    Peek_Local(payloadSize, Data);
    size_t Data_Pos;
    size_t Data_Pos_Before=0;
    size_t Loop=0;
    do
    {
        Data_Pos=Data.find(_T(" - "), Data_Pos_Before);
        if (Data_Pos==std::string::npos)
            Data_Pos=Data.size();
        if (Data.find(_T("options: "), Data_Pos_Before)==Data_Pos_Before)
        {
            Element_Begin("options");
            size_t Options_Pos;
            size_t Options_Pos_Before=Data_Pos_Before;
            do
            {
                Options_Pos=Data.find(_T(" "), Options_Pos_Before);
                if (Options_Pos==std::string::npos)
                    Options_Pos=Data.size();
                Ztring option;
                Get_Local (Options_Pos-Options_Pos_Before, option, "option");
                Options_Pos_Before=Options_Pos;
                if (Options_Pos_Before+3<=Data.size())
                {
                    Skip_Local(1,                               "separator");
                    Options_Pos_Before+=1;
                }

                //Filling
                if (option!=_T("options:"))
                {
                    if (!Encoded_Library_Settings.empty())
                        Encoded_Library_Settings+=_T(" / ");
                    Encoded_Library_Settings+=option;
                    if (option.find("bitrate="))==0)
                        BitRate_Nominal=option.substr(8)+_T("000"); //After "bitrate="
                }
            }
            while (Options_Pos_Before!=Data.size());
            Element_End();
        }
        else
        {
            Ztring Value;
            Get_Local(Data_Pos-Data_Pos_Before, Value,          "data");

            //Saving
            if (Loop==0)
                Encoded_Library=Value;
            if (Loop==1 && Encoded_Library.find(_T("x264"))==0)
            {
                Encoded_Library+=_T(" - ");
                Encoded_Library+=Value;
            }
        }
        Data_Pos_Before=Data_Pos;
        if (Data_Pos_Before+3<=Data.size())
        {
            Skip_Local(3,                                       "separator");
            Data_Pos_Before+=3;
        }

        Loop++;
    }
    while (Data_Pos_Before!=Data.size());
}

//---------------------------------------------------------------------------
void File_Avc::sei_message_recovery_point(int32u payloadSize)
{
    Element_Info("recovery_point");

    //Parsing
    BS_Begin();
    Skip_UE(                                                    "recovery_frame_cnt");
    Skip_SB(                                                    "exact_match_flag");
    Skip_SB(                                                    "broken_link_flag");
    Skip_S1(2,                                                  "changing_slice_group_idc");
    BS_End();
}

//---------------------------------------------------------------------------
void File_Avc::sei_message_mainconcept(int32u payloadSize)
{
    Element_Info("MainConcept text");

    //Parsing
    Ztring Text;
    Get_Local(payloadSize, Text,                                "text");

    Encoded_Library=Text.SubString(_T("produced by "), _T(" MainConcept AG"));
}

//---------------------------------------------------------------------------
// Packet "07"
void File_Avc::seq_parameter_set()
{
    Element_Name("seq_parameter_set");

    int32u pic_order_cnt_type;
    Get_B1 (profile_idc,                                        "profile_idc");
    BS_Begin();
    Element_Begin("constraints");
        Skip_SB(                                                "constraint_set0_flag");
        Skip_SB(                                                "constraint_set1_flag");
        Skip_SB(                                                "constraint_set2_flag");
        Skip_SB(                                                "constraint_set3_flag");
        Skip_SB(                                                "reserved_zero_4bits");
        Skip_SB(                                                "reserved_zero_4bits");
        Skip_SB(                                                "reserved_zero_4bits");
        Skip_SB(                                                "reserved_zero_4bits");
    Element_End();
    Get_S1 ( 8, level_idc,                                      "level_idc");
    Skip_UE(                                                    "seq_parameter_set_id");
    if (profile_idc==0x64
     || profile_idc==0x6E
     || profile_idc==0x7A
     || profile_idc==0x90) //High profiles
    {
        Element_Begin("high profile specific");
        Get_UE (chroma_format_idc,                              "chroma_format_idc");
        if (chroma_format_idc>3)
        {
            Trusted_IsNot("chroma_format_idc is too high");
            chroma_format_idc=1;
        }
        Param_Info(Avc_chroma_format_idc[chroma_format_idc]);
        if (chroma_format_idc==3)
            Skip_SB(                                            "residual_colour_transform_flag");
        Skip_UE(                                                "bit_depth_luma_minus8");
        Skip_UE(                                                "bit_depth_chroma_minus8");
        Skip_SB(                                                "qpprime_y_zero_transform_bypass_flag");
        TEST_SB_SKIP(                                           "seq_scaling_matrix_present_flag");
            for (int32u Pos=0; Pos<8; Pos++)
            {
                TEST_SB_SKIP(                                   "seq_scaling_list_present_flag");
                    scaling_list(Pos<6?16:64);
                TEST_SB_END();
            }
        TEST_SB_END();
        Element_End();
    }
    Get_UE (log2_max_frame_num_minus4,                          "log2_max_frame_num_minus4");
    Get_UE (pic_order_cnt_type,                                 "pic_order_cnt_type");
    if (pic_order_cnt_type==0)
        Skip_UE(                                                "log2_max_pic_order_cnt_lsb_minus4");
    else if (pic_order_cnt_type==1)
    {
        int32u num_ref_frames_in_pic_order_cnt_cycle;
        Skip_SB(                                                "delta_pic_order_always_zero_flag");
        Skip_SE(                                                "offset_for_non_ref_pic");
        Skip_SE(                                                "offset_for_top_to_bottom_field");
        Get_UE (num_ref_frames_in_pic_order_cnt_cycle,          "num_ref_frames_in_pic_order_cnt_cycle");
        if (num_ref_frames_in_pic_order_cnt_cycle>=256)
        {
            Trusted_IsNot("num_ref_frames_in_pic_order_cnt_cycle too high");
            num_ref_frames_in_pic_order_cnt_cycle=0;
        }
        for(int32u Pos=0; Pos<num_ref_frames_in_pic_order_cnt_cycle; Pos++)
            Skip_SE(                                            "offset_for_ref_frame");
    }
    Skip_UE(                                                    "num_ref_frames");
    Skip_SB(                                                    "gaps_in_frame_num_value_allowed_flag");
    Get_UE (pic_width_in_mbs_minus1,                            "pic_width_in_mbs_minus1");
    Get_UE (pic_height_in_map_units_minus1,                     "pic_height_in_map_units_minus1");
    Get_SB (frame_mbs_only_flag,                                "frame_mbs_only_flag");
    if (!frame_mbs_only_flag)
        Skip_SB(                                                "mb_adaptive_frame_field_flag");
    Skip_SB(                                                    "direct_8x8_inference_flag");
    TEST_SB_SKIP(                                               "frame_cropping_flag");
        Get_UE (frame_crop_left_offset,                         "frame_crop_left_offset");
        Get_UE (frame_crop_right_offset,                        "frame_crop_right_offset");
        Get_UE (frame_crop_top_offset,                          "frame_crop_top_offset");
        Get_UE (frame_crop_bottom_offset,                       "frame_crop_bottom_offset");
    TEST_SB_END();
    TEST_SB_SKIP(                                               "vui_parameters_present_flag");
        vui_parameters();
    TEST_SB_END();
    Mark_1(                                                     );
    BS_End();

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0x08);

        //Autorisation of other streams
        Stream[0x06].Searching_Payload=true; //sei
        for (int8u Pos=0x08; Pos<=0x1F; Pos++)
            Stream[Pos].Searching_Payload=true; //pic_parameter_set, access_unit_delimiter, end_of_seq, end_of_stream, filler_data, reserved
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "08"
void File_Avc::pic_parameter_set()
{
    Element_Name("pic_parameter_set");

    //Parsing
    int32u num_slice_groups_minus1;
    BS_Begin();
    Skip_UE(                                                    "pic_parameter_set_id");
    Skip_UE(                                                    "seq_parameter_set_id");
    Get_SB (entropy_coding_mode_flag,                           "entropy_coding_mode_flag");
    Skip_SB(                                                    "pic_order_present_flag");
    Get_UE (num_slice_groups_minus1,                            "num_slice_groups_minus1");
    if (num_slice_groups_minus1>7)
    {
        Trusted_IsNot("num_slice_groups_minus1 too high");
        num_slice_groups_minus1=0;
    }
    if (num_slice_groups_minus1>0)
    {
        int32u slice_group_map_type;
        Get_UE (slice_group_map_type,                           "slice_group_map_type");
        if (slice_group_map_type==0)
        {
            for (int32u Pos=0; Pos<=num_slice_groups_minus1; Pos++)
                Skip_UE(                                        "run_length_minus1");
        }
        else if (slice_group_map_type==2)
        {
            for (int32u Pos=0; Pos<num_slice_groups_minus1; Pos++)
            {
                Skip_UE(                                        "top_left");
                Skip_UE(                                        "bottom_right");
            }
        }
        else if (slice_group_map_type==3
              || slice_group_map_type==4
              || slice_group_map_type==5)
        {
            Skip_SB(                                            "slice_group_change_direction_flag");
            Skip_UE(                                            "slice_group_change_rate_minus1");
        }
        else if (slice_group_map_type==6)
        {
            int32u pic_size_in_map_units_minus1;
            Get_UE (pic_size_in_map_units_minus1,               "pic_size_in_map_units_minus1");
            if(pic_size_in_map_units_minus1>(pic_width_in_mbs_minus1+1)*(pic_height_in_map_units_minus1+1))
            {
                Trusted_IsNot("pic_size_in_map_units_minus1 too high");
                pic_size_in_map_units_minus1=0;
            }
            int32u slice_group_id_Size=(int32u)(ceil(log((float32)(num_slice_groups_minus1+1))/log((float32)2))); //this is log2
            for (int32u Pos=0; Pos<=pic_size_in_map_units_minus1; Pos++)
                Skip_S4(slice_group_id_Size,                    "slice_group_id");
        }
    }
    Skip_UE(                                                    "num_ref_idx_l0_active_minus1");
    Skip_UE(                                                    "num_ref_idx_l1_active_minus1");
    Skip_SB(                                                    "weighted_pred_flag");
    Skip_S1(2,                                                  "weighted_bipred_idc");
    Skip_SE(                                                    "pic_init_qp_minus26");
    Skip_SE(                                                    "pic_init_qs_minus26");
    Skip_SE(                                                    "chroma_qp_index_offset");
    Skip_SB(                                                    "deblocking_filter_control_present_flag");
    Skip_SB(                                                    "constrained_intra_pred_flag");
    Skip_SB(                                                    "redundant_pic_cnt_present_flag");
    bool more_rbsp_data=false;
    if (Data_BS_Remain()>8)
        more_rbsp_data=true;
    else
    {
        bool  Test;
        Peek_SB(Test);
        if (!Test) //Is 0, this is not the Marker
            more_rbsp_data=true;
        {
            //Is 1, maybe Marker, maybe not
            int8u After;
            Peek_S1(Data_BS_Remain(), After);
            After&=(0xFF>>(9-Data_BS_Remain()));
            if (After)
                more_rbsp_data=true; //More bits after...

        }
    }
    if (more_rbsp_data)
    {
        bool transform_8x8_mode_flag;
        Get_SB (transform_8x8_mode_flag,                        "transform_8x8_mode_flag");
        TEST_SB_SKIP(                                           "pic_scaling_matrix_present_flag");
            for (int8u Pos=0; Pos<6+2*transform_8x8_mode_flag; Pos++ )
            {
                TEST_SB_SKIP(                                   "pic_scaling_list_present_flag");
                    scaling_list(Pos<6?16:64);
                TEST_SB_END();
            }
        TEST_SB_END();
        Skip_SB(                                                "second_chroma_qp_index_offset");
    }
    Mark_1(                                                     );
    BS_End();

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0x05);
        NextCode_Add(0x06);

        //Autorisation of other streams
        for (int8u Pos=0x01; Pos<=0x06; Pos++)
            Stream[Pos].Searching_Payload=true; //Coded slice...
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "09"
void File_Avc::access_unit_delimiter()
{
    Element_Name("access_unit_delimiter");

    int8u primary_pic_type;
    BS_Begin();
    Get_S1 ( 3, primary_pic_type,                               "primary_pic_type"); Param_Info(Avc_primary_pic_type[primary_pic_type]);
    Mark_1(                                                     );
    BS_End();

    FILLING_BEGIN();
        frame_num_LastOne=(int32u)-1; //Reseting
    FILLING_END();
}

//***************************************************************************
// SubElements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Avc::scaling_list(int32u ScalingList_Size)
{
    //From http://mpeg4ip.cvs.sourceforge.net/mpeg4ip/mpeg4ip/util/h264/main.cpp?revision=1.17&view=markup
    int32u lastScale=8, nextScale=8;
    for (int32u Pos=0; Pos<ScalingList_Size; Pos++)
    {
        if (nextScale!=0)
        {
            int32u delta_scale;
            Get_UE (delta_scale,                                "scale_delta");
            nextScale=(lastScale+delta_scale+256)%256;
        }
        lastScale=nextScale;
    }
}

//---------------------------------------------------------------------------
void File_Avc::vui_parameters()
{
    bool nal_hrd_parameters_present_flag, vcl_hrd_parameters_present_flag;
    TEST_SB_SKIP(                                               "aspect_ratio_info_present_flag");
        Get_S1 (8, aspect_ratio_idc,                            "aspect_ratio_idc"); if (aspect_ratio_idc<Avc_PixelAspectRatio_Size) Param_Info(Avc_PixelAspectRatio[aspect_ratio_idc]);
        if (aspect_ratio_idc==0xFF)
        {
            Get_S2 (16, sar_width,                              "sar_width");
            Get_S2 (16, sar_height,                             "sar_height");
        }
    TEST_SB_END();
    TEST_SB_SKIP(                                               "overscan_info_present_flag");
        Skip_SB(                                                "overscan_appropriate_flag");
    TEST_SB_END();
    TEST_SB_SKIP(                                               "video_signal_type_present_flag");
        Get_S1 (3, video_format,                                "video_format"); Param_Info(Avc_video_format[video_format]);
        Skip_SB(                                                "video_full_range_flag");
        TEST_SB_SKIP(                                           "colour_description_present_flag");
            Skip_S1(8,                                          "colour_primaries");
            Skip_S1(8,                                          "transfer_characteristics");
            Skip_S1(8,                                          "matrix_coefficients");
        TEST_SB_END();
    TEST_SB_END();
    TEST_SB_SKIP(                                               "chroma_loc_info_present_flag");
        Skip_UE(                                                "chroma_sample_loc_type_top_field");
        Skip_UE(                                                "chroma_sample_loc_type_bottom_field");
    TEST_SB_END();
    TEST_SB_GET (timing_info_present_flag,                      "timing_info_present_flag");
        Get_S4 (32, num_units_in_tick,                          "num_units_in_tick");
        Get_S4 (32, time_scale,                                 "time_scale");
        Get_SB (fixed_frame_rate_flag,                          "fixed_frame_rate_flag");
    TEST_SB_END();
    TEST_SB_GET (nal_hrd_parameters_present_flag,               "nal_hrd_parameters_present_flag");
        hrd_parameters();
    TEST_SB_END();
    TEST_SB_GET (vcl_hrd_parameters_present_flag,               "vcl_hrd_parameters_present_flag");
        hrd_parameters();
    TEST_SB_END();
    if(nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag)
    {
        CpbDpbDelaysPresentFlag=true;
        Skip_SB(                                                "low_delay_hrd_flag");
    }
    Get_SB (pic_struct_present_flag,                            "pic_struct_present_flag");
    TEST_SB_SKIP(                                               "bitstream_restriction_flag");
        Skip_SB(                                                "motion_vectors_over_pic_boundaries_flag");
        Skip_UE(                                                "max_bytes_per_pic_denom");
        Skip_UE(                                                "max_bits_per_mb_denom");
        Skip_UE(                                                "log2_max_mv_length_horizontal");
        Skip_UE(                                                "log2_max_mv_length_vertical");
        Skip_UE(                                                "num_reorder_frames");
        Skip_UE(                                                "max_dec_frame_buffering");
    TEST_SB_END();
}

//---------------------------------------------------------------------------
void File_Avc::hrd_parameters()
{
    int32u cpb_cnt_minus1;
    Get_UE (cpb_cnt_minus1,                                     "cpb_cnt_minus1");
    Skip_S1(4,                                                  "bit_rate_scale");
    Skip_S1(4,                                                  "cpb_size_scale");
    if (cpb_cnt_minus1>31)
    {
        Trusted_IsNot("cpb_cnt_minus1 too high");
        cpb_cnt_minus1=0;
    }
    for (int32u SchedSelIdx=0; SchedSelIdx<=cpb_cnt_minus1; SchedSelIdx++)
    {
        Element_Begin("ShedSel");
        Skip_UE(                                                "bit_rate_value_minus1");
        Skip_UE(                                                "cpb_size_value_minus1");
        Skip_SB(                                                "cbr_flag");
        Element_End();
    }
    Skip_S1(5,                                                  "initial_cpb_removal_delay_length_minus1");
    Get_S1 (5, cpb_removal_delay_length_minus1,                 "cpb_removal_delay_length_minus1");
    Get_S1 (5, dpb_output_delay_length_minus1,                  "dpb_output_delay_length_minus1");
    Get_S1 (5, time_offset_length,                              "time_offset_length");    
}

//***************************************************************************
// Specific
//***************************************************************************
//---------------------------------------------------------------------------
void File_Avc::SPS_PPS()
{
    //Parsing
    int8u Profile, Level, seq_parameter_set_count, pic_parameter_set_count;
    Skip_B1(                                                    "Reserved");
    Get_B1 (Profile,                                            "Profile");
    Skip_B1(                                                    "Reserved");
    Get_B1 (Level,                                              "Level");
    BS_Begin();
    Skip_S1(6,                                                  "Reserved");
    Skip_S1(2,                                                  "Size of NALU length minus 1");
    Skip_S1(3,                                                  "Reserved");
    Get_S1 (5, seq_parameter_set_count,                         "seq_parameter_set count");
    BS_End();
    for (int8u Pos=0; Pos<seq_parameter_set_count; Pos++)
    {
        Element_Begin("seq_parameter_set");
        int16u Size;
        Get_B2 (Size,                                           "Size");
        BS_Begin();
        Mark_0 ();
        Skip_S1( 2,                                             "nal_ref_idc");
        Skip_S1( 5,                                             "nal_unit_type");
        BS_End();
        int64u Element_Offset_Save=Element_Offset;
        int64u Element_Size_Save=Element_Size;
        Buffer_Offset+=(size_t)Element_Offset_Save;
        Element_Offset=0;
        Element_Size=Size-1;
        Element_Code=0x07; //seq_parameter_set
        Data_Parse();
        Buffer_Offset-=(size_t)Element_Offset_Save;
        Element_Offset=(size_t)(Element_Offset_Save+Size-1);
        Element_Size=Element_Size_Save;
        Element_End();
    }
    Get_B1 (pic_parameter_set_count,                            "pic_parameter_set count");
    for (int8u Pos=0; Pos<pic_parameter_set_count; Pos++)
    {
        Element_Begin("pic_parameter_set");
        int16u Size;
        Get_B2 (Size,                                           "Size");
        BS_Begin();
        Mark_0 ();
        Skip_S1( 2,                                             "nal_ref_idc");
        Skip_S1( 5,                                             "nal_unit_type");
        BS_End();
        int64u Element_Offset_Save=Element_Offset;
        int64u Element_Size_Save=Element_Size;
        Buffer_Offset+=(size_t)Element_Offset_Save;
        Element_Offset=0;
        Element_Size=Size-1;
        Element_Code=0x08; //pic_parameter_set
        Data_Parse();
        Buffer_Offset-=(size_t)Element_Offset_Save;
        Element_Offset=(size_t)(Element_Offset_Save+Size-1);
        Element_Size=Element_Size_Save;
        Element_End();
    }

    //Filling
    MustParse_SPS_PPS=false;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Avc::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+4<=Buffer_Size)
    {
         while (Buffer_Offset+4<=Buffer_Size
          && !(CC3(Buffer+Buffer_Offset  )==0x000001
            && CC1(Buffer+Buffer_Offset+3)< 0x80)) //bit 7 can not be 1
                 Buffer_Offset++;

        if (Buffer_Offset+4<=Buffer_Size)//Testing if header is coherant
        {
            int8u nal_unit_type=CC1(Buffer+Buffer_Offset+3);
            bool nal_ref_idc=(nal_unit_type&0x60)?true:false;
            nal_unit_type&=0x1F;
            if (Stream[nal_unit_type].Searching_Payload
             && ((nal_ref_idc  && (nal_unit_type==0x05 || nal_unit_type==0x07 || nal_unit_type==0x08))
              || (!nal_ref_idc && (nal_unit_type==0x06 || nal_unit_type==0x09 || nal_unit_type==0x10 || nal_unit_type==0x0A || nal_unit_type==0x0B))))
                 break;
            else
                Buffer_Offset++;
        }
    }
    if (Buffer_Offset+4>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+4==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset)!=0x000001)
            {
                Buffer_Offset++;
                if (CC3(Buffer+Buffer_Offset)!=0x000001)
                {
                    Buffer_Offset++;
                    if (CC2(Buffer+Buffer_Offset)!=0x0000)
                    {
                        Buffer_Offset++;
                        if (CC1(Buffer+Buffer_Offset)!=0x00)
                            Buffer_Offset++;
                    }
                }
            }
        }

        return false;
    }

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Avc::Header_Parser_QuickSearch()
{
    while (           Buffer_Offset+4<=Buffer_Size
      &&   CC3(Buffer+Buffer_Offset)==0x000001)
    {
        //Getting start_code
        int8u start_code=CC1(Buffer+Buffer_Offset+3)&0x1F;

        //Searching start
        if (Stream[start_code].Searching_Payload)
            return true;

        //Getting size
        Buffer_Offset+=4;
        while(Buffer_Offset+4<=Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
            Buffer_Offset++;
    }

    if (Buffer_Offset+4>Buffer_Size)
    {
        Synched=false;
        Synchronize();
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool File_Avc::Detect_NonAVC ()
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
        Finnished();
        return true;
    }

    //Detect TS files, and the parser is not enough precise to detect them later
    while (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset)!=0x47) //Look for first Sync word
        Buffer_Offset++;
    if (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset+188)==0x47 && CC1(Buffer+Buffer_Offset+188*2)==0x47 && CC1(Buffer+Buffer_Offset+188*3)==0x47)
    {
        Finnished();
        return true;
    }
    Buffer_Offset=0;

    //Seems OK
    return false;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Avc::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

} //NameSpace

#endif //MEDIAINFO_AVC_YES
