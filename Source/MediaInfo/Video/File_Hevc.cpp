// File_Hevc - Info for HEVC files
// Copyright (C) 2013-2013 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
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

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_HEVC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Hevc.h"
#if MEDIAINFO_EVENTS
    #include "MediaInfo/MediaInfo_Config_MediaInfo.h"
    #include "MediaInfo/MediaInfo_Events.h"
    #include "MediaInfo/MediaInfo_Events_Internal.h"
#endif //MEDIAINFO_EVENTS
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Info
//***************************************************************************

//---------------------------------------------------------------------------
const char* Hevc_profile_idc(int32u profile_idc)
{
    switch (profile_idc)
    {
        case   0 : return "No profile";
        case   1 : return "Main";
        case   2 : return "Main 10";
        case   3 : return "Main Still";
        default  : return "Unknown";
    }
}

//---------------------------------------------------------------------------
const char* Hevc_chroma_format_idc(int8u chroma_format_idc)
{
    switch (chroma_format_idc)
    {
        case   0 : return "monochrome";
        case   1 : return "4:2:0";
        case   2 : return "4:2:2";
        case   3 : return "4:4:4";
        default  : return "Unknown";
    }
}

//---------------------------------------------------------------------------
const char* Hevc_pic_type[]=
{
    "I",
    "I, P",
    "I, P, B",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Hevc_slice_type(int32u slice_type)
{
    switch (slice_type)
    {
        case 0 : return "B";
        case 1 : return "P";
        case 2 : return "I";
        default: return "";
    }
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Hevc::File_Hevc()
{
    //Config
    #if MEDIAINFO_EVENTS
        ParserIDs[0]=MediaInfo_Parser_Hevc;
        StreamIDs_Width[0]=0;
    #endif //MEDIAINFO_EVENTS
    #if MEDIAINFO_TRACE
        Trace_Layers_Update(8); //Stream
    #endif //MEDIAINFO_TRACE
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=64*1024;
    PTS_DTS_Needed=true;
    IsRawStream=true;
    Frame_Count_NotParsedIncluded=0;

    //In
    Frame_Count_Valid=MediaInfoLib::Config.ParseSpeed_Get()>=0.3?512:2;
    FrameIsAlwaysComplete=false;
    MustParse_SPS_PPS=false;
    SizedBlocks=false;

    //File specific
    lengthSizeMinusOne=(int8u)-1;
}

//---------------------------------------------------------------------------
File_Hevc::~File_Hevc()
{
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Hevc::Streams_Fill()
{
    if (Count_Get(Stream_Video)==0)
        Stream_Prepare(Stream_Video);
    Fill(Stream_Video, 0, Video_Format, "HEVC");
    Fill(Stream_Video, 0, Video_Codec, "HEVC");

    for (std::vector<seq_parameter_set_struct*>::iterator seq_parameter_set_Item=seq_parameter_sets.begin(); seq_parameter_set_Item!=seq_parameter_sets.end(); ++seq_parameter_set_Item)
        if ((*seq_parameter_set_Item))
            Streams_Fill(seq_parameter_set_Item);
}

//---------------------------------------------------------------------------
void File_Hevc::Streams_Fill(std::vector<seq_parameter_set_struct*>::iterator seq_parameter_set_Item)
{
    Ztring Profile;
    if ((*seq_parameter_set_Item)->profile_idc)
        Profile=Ztring().From_Local(Hevc_profile_idc((*seq_parameter_set_Item)->profile_idc));
    if ((*seq_parameter_set_Item)->level_idc)
    {
        if ((*seq_parameter_set_Item)->profile_idc)
            Profile+=__T('@');
        Profile+=__T('L')+Ztring().From_Number(((float)(*seq_parameter_set_Item)->level_idc)/10, 1);
    }
    Fill(Stream_Video, 0, Video_Format_Profile, Profile);
    Fill(Stream_Video, 0, Video_Codec_Profile, Profile);
    Fill(Stream_Video, StreamPos_Last, Video_Width, (*seq_parameter_set_Item)->pic_width_in_luma_samples);
    Fill(Stream_Video, StreamPos_Last, Video_Height, (*seq_parameter_set_Item)->pic_height_in_luma_samples);
    //Fill(Stream_Video, 0, Video_PixelAspectRatio, PixelAspectRatio, 3, true);
    //Fill(Stream_Video, 0, Video_DisplayAspectRatio, Width*PixelAspectRatio/Height, 3, true); //More precise

    Fill(Stream_Video, 0, Video_ColorSpace, "YUV");
    Fill(Stream_Video, 0, Video_Colorimetry, Hevc_chroma_format_idc((*seq_parameter_set_Item)->chroma_format_idc));
    if ((*seq_parameter_set_Item)->bit_depth_luma_minus8==(*seq_parameter_set_Item)->bit_depth_chroma_minus8)
        Fill(Stream_Video, 0, Video_BitDepth, (*seq_parameter_set_Item)->bit_depth_luma_minus8+8);
}

//---------------------------------------------------------------------------
void File_Hevc::Streams_Finish()
{
}

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Hevc::FileHeader_Begin()
{
    if (!File__Analyze::FileHeader_Begin_0x000001())
        return false;

    if (!MustSynchronize)
    {
        Synched_Init();
        Buffer_TotalBytes_FirstSynched=0;
        File_Offset_FirstSynched=File_Offset;
    }

    //All should be OK
    return true;
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Hevc::Synchronize()
{
    //Synchronizing
    size_t Buffer_Offset_Min=Buffer_Offset;
    while(Buffer_Offset+4<=Buffer_Size && (Buffer[Buffer_Offset  ]!=0x00
                                        || Buffer[Buffer_Offset+1]!=0x00
                                        || Buffer[Buffer_Offset+2]!=0x01))
    {
        Buffer_Offset+=2;
        while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]!=0x00)
            Buffer_Offset+=2;
        if (Buffer_Offset>=Buffer_Size || Buffer[Buffer_Offset-1]==0x00)
            Buffer_Offset--;
    }
    if (Buffer_Offset>Buffer_Offset_Min && Buffer[Buffer_Offset-1]==0x00)
        Buffer_Offset--;

    //Parsing last bytes if needed
    if (Buffer_Offset+4==Buffer_Size && (Buffer[Buffer_Offset  ]!=0x00
                                      || Buffer[Buffer_Offset+1]!=0x00
                                      || Buffer[Buffer_Offset+2]!=0x00
                                      || Buffer[Buffer_Offset+3]!=0x01))
        Buffer_Offset++;
    if (Buffer_Offset+3==Buffer_Size && (Buffer[Buffer_Offset  ]!=0x00
                                      || Buffer[Buffer_Offset+1]!=0x00
                                      || Buffer[Buffer_Offset+2]!=0x01))
        Buffer_Offset++;
    if (Buffer_Offset+2==Buffer_Size && (Buffer[Buffer_Offset  ]!=0x00
                                      || Buffer[Buffer_Offset+1]!=0x00))
        Buffer_Offset++;
    if (Buffer_Offset+1==Buffer_Size &&  Buffer[Buffer_Offset  ]!=0x00)
        Buffer_Offset++;

    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Hevc::Synched_Test()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+6>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Buffer[Buffer_Offset  ]!=0x00
     || Buffer[Buffer_Offset+1]!=0x00
     || (Buffer[Buffer_Offset+2]!=0x01 && (Buffer[Buffer_Offset+2]!=0x00 || Buffer[Buffer_Offset+3]!=0x01)))
    {
        Synched=false;
        return true;
    }

    //Quick search
    if (!Header_Parser_QuickSearch())
        return false;

    //We continue
    return true;
}

//***************************************************************************
// Buffer - Demux
//***************************************************************************

//---------------------------------------------------------------------------
#if MEDIAINFO_DEMUX
bool File_Hevc::Demux_UnpacketizeContainer_Test()
{
    return true;
}
#endif //MEDIAINFO_DEMUX

//---------------------------------------------------------------------------
void File_Hevc::Synched_Init()
{
    //FrameInfo
    PTS_End=0;
    if (FrameInfo.DTS==(int64u)-1)
        FrameInfo.DTS=0; //No DTS in container
    DTS_Begin=FrameInfo.DTS;
    DTS_End=FrameInfo.DTS;

    //Status
    IFrame_Count=0;

    //Temp

    //Default values
    Streams.resize(0x100);
    Streams[33].Searching_Payload=true; //seq_parameter_set
    Streams[35].Searching_Payload=true; //access_unit_delimiter
    Streams[39].Searching_Payload=true; //sei
    for (int8u Pos=0xFF; Pos>=0xB9; Pos--)
        Streams[Pos].Searching_Payload=true; //Testing MPEG-PS
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Hevc::Read_Buffer_Unsynched()
{
    //Impossible to know TimeStamps now
    PTS_End=0;
    DTS_End=0;
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Hevc::Header_Parse()
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
    if (!SizedBlocks)
    {
        if (Buffer[Buffer_Offset+2]==0x00)
            Skip_B1(                                            "zero_byte");
        Skip_B3(                                                "start_code_prefix_one_3bytes");
        BS_Begin();
        Mark_0 ();
        Get_S1 (6, nal_unit_type,                               "nal_unit_type");
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Skip_S1(3,                                              "nuh_temporal_id_plus1");
        BS_End();
        if (!Header_Parser_Fill_Size())
        {
            Element_WaitForMoreData();
            return;
        }
    }
    else
    {
        int32u Size;
        switch (lengthSizeMinusOne)
        {
            case 0: {
                        int8u Size_;
                        Get_B1 (Size_,                          "size");
                        Size=Size_;
                    }
                    break;
            case 1: {
                        int16u Size_;
                        Get_B2 (Size_,                          "size");
                        Size=Size_;
                    }
                    break;
            case 2: {
                        int32u Size_;
                        Get_B3 (Size_,                          "size");
                        Size=Size_;
                    }
                    break;
            case 3:     Get_B4 (Size,                           "size");
                    break;
            default:    Trusted_IsNot("No size of NALU defined");
                        Size=(int32u)(Buffer_Size-Buffer_Offset);
        }
        BS_Begin();
        Mark_0 ();
        Get_S1 (6, nal_unit_type,                               "nal_unit_type");
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Mark_0_NoTrustError ();
        Skip_S1(3,                                              "nuh_temporal_id_plus1");
        BS_End();

        FILLING_BEGIN()
            Header_Fill_Size(Element_Offset+Size-2);
        FILLING_END()
    }

    //Filling
    #if MEDIAINFO_TRACE
        if (Trace_Activated)
            Header_Fill_Code(nal_unit_type, Ztring().From_CC1(nal_unit_type));
        else
    #endif //MEDIAINFO_TRACE
            Header_Fill_Code(nal_unit_type);
}

//---------------------------------------------------------------------------
bool File_Hevc::Header_Parser_Fill_Size()
{
    //Look for next Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if Header_Parse_Fill_Size() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset+4;
    while (Buffer_Offset_Temp+5<=Buffer_Size
        && CC3(Buffer+Buffer_Offset_Temp)!=0x000001)
    {
        Buffer_Offset_Temp+=2;
        while(Buffer_Offset_Temp<Buffer_Size && Buffer[Buffer_Offset_Temp]!=0x00)
            Buffer_Offset_Temp+=2;
        if (Buffer_Offset_Temp>=Buffer_Size || Buffer[Buffer_Offset_Temp-1]==0x00)
            Buffer_Offset_Temp--;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+5>Buffer_Size)
    {
        if (FrameIsAlwaysComplete || File_Offset+Buffer_Size>=File_Size)
            Buffer_Offset_Temp=Buffer_Size; //We are sure that the next bytes are a start
        else
            return false;
    }

    if (Buffer[Buffer_Offset_Temp-1]==0x00)
        Buffer_Offset_Temp--;

    //OK, we continue
    Header_Fill_Size(Buffer_Offset_Temp-Buffer_Offset);
    Buffer_Offset_Temp=0;
    return true;
}

//---------------------------------------------------------------------------
bool File_Hevc::Header_Parser_QuickSearch()
{
    while (       Buffer_Offset+6<=Buffer_Size
      &&   Buffer[Buffer_Offset  ]==0x00
      &&   Buffer[Buffer_Offset+1]==0x00
      &&  (Buffer[Buffer_Offset+2]==0x01
        || (Buffer[Buffer_Offset+2]==0x00 && Buffer[Buffer_Offset+3]==0x01)))
    {
        //Getting start_code
        int8u start_code;
        if (Buffer[Buffer_Offset+2]==0x00)
            start_code=CC1(Buffer+Buffer_Offset+4)&0x1F;
        else
            start_code=CC1(Buffer+Buffer_Offset+3)&0x1F;

        //Searching start33
        if (Streams[start_code].Searching_Payload)
            return true;

        //Synchronizing
        Buffer_Offset+=4;
        Synched=false;
        if (!Synchronize())
        {
            UnSynched_IsNotJunk=true;
            return false;
        }

        if (Buffer_Offset+6>Buffer_Size)
        {
            UnSynched_IsNotJunk=true;
            return false;
        }
    }

    Trusted_IsNot("HEVC, Synchronisation lost");
    return Synchronize();
}

//---------------------------------------------------------------------------
void File_Hevc::Data_Parse()
{
    //Specific case
    if (Element_Code==(int64u)-1)
    {
        SPS_PPS();
        return;
    }

    //Trailing zeroes
    int64u Element_Size_SaveBeforeZeroes=Element_Size;
    if (Element_Size)
    {
        while (Element_Size && Buffer[Buffer_Offset+(size_t)Element_Size-1]==0)
            Element_Size--;
    }

    //Searching emulation_prevention_three_byte
    int8u* Buffer_3Bytes=NULL;
    const int8u* Save_Buffer=Buffer;
    int64u Save_File_Offset=File_Offset;
    size_t Save_Buffer_Offset=Buffer_Offset;
    int64u Save_Element_Size=Element_Size;
    size_t Element_Offset_3Bytes=(size_t)Element_Offset;
    std::vector<size_t> ThreeByte_List;
    while (Element_Offset_3Bytes+3<=Element_Size)
    {
        if (CC3(Buffer+Buffer_Offset+(size_t)Element_Offset_3Bytes)==0x000003)
            ThreeByte_List.push_back(Element_Offset_3Bytes+2);
        Element_Offset_3Bytes+=2;
        while(Element_Offset_3Bytes<Element_Size && Buffer[Buffer_Offset+(size_t)Element_Offset_3Bytes]!=0x00)
            Element_Offset_3Bytes+=2;
        if (Element_Offset_3Bytes>=Element_Size || Buffer[Buffer_Offset+(size_t)Element_Offset_3Bytes-1]==0x00)
            Element_Offset_3Bytes--;
    }

    if (!ThreeByte_List.empty())
    {
        //We must change the buffer for keeping out
        Element_Size=Save_Element_Size-ThreeByte_List.size();
        File_Offset+=Buffer_Offset;
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
        case  0 :
        case  1 :
        case  2 :
        case  3 :
        case 16 :
        case 17 :
        case 18 :
        case 19 :
        case 20 :
        case 21 :
                  slice_segment_layer(); break;
        case  4 :
        case  5 :
        case  6 :
        case  7 :
        case  8 :
        case  9 :
                  slice_layer(); break;
        case 32 : video_parameter_set(); break;
        case 33 : seq_parameter_set(); break;
        case 34 : pic_parameter_set(); break;
        case 35 : access_unit_delimiter(); break;
        case 36 : end_of_seq(); break;
        case 37 : end_of_bitstream(); break;
        case 38 : filler_data(); break;
        case 39 :
        case 40 : 
                  sei(); break;
        default :
            Skip_XX(Element_Size-Element_Offset, "Data");
    }

    if (!ThreeByte_List.empty())
    {
        //We must change the buffer for keeping out
        Element_Size=Save_Element_Size;
        File_Offset=Save_File_Offset;
        Buffer_Offset=Save_Buffer_Offset;
        delete[] Buffer; Buffer=Save_Buffer;
        Buffer_3Bytes=NULL; //Same as Buffer...
        Element_Offset+=ThreeByte_List.size();
    }

    //Trailing zeroes
    Element_Size=Element_Size_SaveBeforeZeroes;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packets
void File_Hevc::slice_segment_layer()
{
    #if MEDIAINFO_TRACE
        Element_Name("slice_segment_layer");
        switch (Element_Code)
        {
            case 0 :
            case 1 : break;
            case 2 :
            case 3 : Element_Info("TSA"); break;
            case 16 :
            case 17 :
            case 18 : Element_Info("BLA"); break;
            case 19 :
            case 20 : Element_Info("IDR"); break;
            case 21 : Element_Info("CRA"); break;
            default: ;
        }
    #endif //MEDIAINFO_TRACE

    //Parsing
    RapPicFlag=Element_Code>=16 && Element_Code<=23;
    BS_Begin();
    slice_segment_header();
    BS_End();
    Skip_XX(Element_Size-Element_Offset,                        "(ToDo)");

    FILLING_BEGIN();
        //Count of I-Frames
        if (first_slice_segment_in_pic_flag && Element_Code==20)
            IFrame_Count++;

        //Frame_Count
        Frame_Count++;
        if (IFrame_Count && Frame_Count_NotParsedIncluded!=(int64u)-1)
            Frame_Count_NotParsedIncluded++;
        Frame_Count_InThisBlock++;

        //Filling only if not already done
        if (Frame_Count==1 && !Status[IsAccepted])
            Accept("HEVC");
        if (!Status[IsFilled])
        {
            if (IFrame_Count>=8)
                Frame_Count_Valid=Frame_Count; //We have enough frames
            if (Frame_Count>=Frame_Count_Valid)
            {
                Fill("HEVC");
                if (!IsSub && MediaInfoLib::Config.ParseSpeed_Get()<1.0)
                    Finish("HEVC");
            }
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packets
void File_Hevc::slice_layer()
{
    #if MEDIAINFO_TRACE
        Element_Name("slice_layer");
        switch (Element_Code)
        {
            case 4 :
            case 5 : Element_Info("STSA"); break;
            case 6 :
            case 7 : Element_Info("RADL"); break;
            case 8 :
            case 9 : Element_Info("RASL"); break;
            default: ;
        }
    #endif //MEDIAINFO_TRACE

    //Parsing
    Skip_XX(Element_Size-Element_Offset,                        "(ToDo)");
}

//---------------------------------------------------------------------------
// Packet "32"
void File_Hevc::video_parameter_set()
{
    Element_Name("video_parameter_set");

    //Warning: based on a draft of the specification, it is maybe not compliant to the final specification

    //Parsing
    int32u  vps_num_layer_sets_minus1;
    int8u   vps_video_parameter_set_id, vps_max_sub_layers_minus1, vps_max_layer_id;
    bool    vps_sub_layer_ordering_info_present_flag;
    BS_Begin();
    Get_S1 (4,  vps_video_parameter_set_id,                     "vps_video_parameter_set_id");
    Skip_S1(2,                                                  "vps_reserved_three_2bits");
    Skip_S1(6,                                                  "vps_reserved_zero_6bits");
    Get_S1 (3,  vps_max_sub_layers_minus1,                      "vps_max_sub_layers_minus1");
    Skip_SB(                                                    "vps_temporal_id_nesting_flag");
    Skip_S2(16,                                                 "vps_reserved_0xffff_16bits");
    profile_tier_level(vps_max_sub_layers_minus1);
    Get_SB (   vps_sub_layer_ordering_info_present_flag,        "vps_sub_layer_ordering_info_present_flag");
    for (int32u SubLayerPos=(vps_sub_layer_ordering_info_present_flag?0:vps_max_sub_layers_minus1); SubLayerPos<=vps_max_sub_layers_minus1; SubLayerPos++)
    {
        Element_Begin1("SubLayer");
        Skip_UE(                                                "vps_max_dec_pic_buffering_minus1");
        Skip_UE(                                                "vps_max_num_reorder_pics");
        Skip_UE(                                                "vps_max_latency_increase_plus1");
        Element_End0();
    }
    Get_S1 ( 6, vps_max_layer_id,                               "vps_max_layer_id");
    Get_UE (    vps_num_layer_sets_minus1,                      "vps_num_layer_sets_minus1");
    for (int32u LayerSetPos=1; LayerSetPos<=vps_num_layer_sets_minus1; LayerSetPos++)
        for (int8u LayerId=0; LayerId<=vps_max_layer_id; LayerId++)
            Skip_SB(                                            "layer_id_included_flag");
    TEST_SB_SKIP(                                               "vps_timing_info_present_flag");
        int32u vps_num_hrd_parameters;
        Skip_S4(32,                                             "vps_num_units_in_tick");
        Skip_S4(32,                                             "vps_time_scale");
        TEST_SB_SKIP(                                           "vps_poc_proportional_to_timing_flag");
            Skip_UE(                                            "vps_num_ticks_poc_diff_one_minus1");
        TEST_SB_END();
        Get_UE (    vps_num_hrd_parameters,                     "vps_num_hrd_parameters");
        for (int32u HrdPos=0; HrdPos<vps_num_hrd_parameters; HrdPos++)
        {
            Skip_UE(                                            "hrd_layer_set_idx");
            if (HrdPos)
                Skip_SB(                                        "cprms_present_flag");
            //hrd_parameters
        }
    TEST_SB_END();
    TEST_SB_SKIP(                                               "vps_extension_flag");
        Skip_BS(Data_BS_Remain(),                               "vps_extension_data");
    TEST_SB_END();
    BS_End();

    FILLING_BEGIN_PRECISE()
        //Creating Data
        if (vps_video_parameter_set_id>=video_parameter_sets.size())
            video_parameter_sets.resize(vps_video_parameter_set_id+1);
        std::vector<video_parameter_set_struct*>::iterator Data_Item=video_parameter_sets.begin()+vps_video_parameter_set_id;
        delete *Data_Item; *Data_Item=new video_parameter_set_struct();

        //Filling from stream
        (*Data_Item)->IsSynched                                     =true;
    FILLING_END()
}

//---------------------------------------------------------------------------
// Packet "33"
void File_Hevc::seq_parameter_set()
{
    Element_Name("seq_parameter_set");

    //Warning: based on a draft of the specification, it is maybe not compliant to the final specification

    //Parsing
    int32u  sps_seq_parameter_set_id, chroma_format_idc, pic_width_in_luma_samples, pic_height_in_luma_samples, bit_depth_luma_minus8, bit_depth_chroma_minus8;
    int8u   sps_video_parameter_set_id, sps_max_sub_layers_minus1;
    BS_Begin();
    Get_S1 (4, sps_video_parameter_set_id,                      "sps_video_parameter_set_id");
    std::vector<video_parameter_set_struct*>::iterator video_parameter_set_Item;
    if (sps_video_parameter_set_id>=video_parameter_sets.size() || (*(video_parameter_set_Item=video_parameter_sets.begin()+sps_video_parameter_set_id))==NULL || !(*video_parameter_set_Item)->IsSynched)
    {
        //Not yet present
        Skip_BS(Data_BS_Remain(),                               "Data (video_parameter_set is missing)");
        return;
    }
    Get_S1 (3, sps_max_sub_layers_minus1,                       "sps_max_sub_layers_minus1");
    Skip_SB(                                                    "sps_temporal_id_nesting_flag");
    profile_tier_level(sps_max_sub_layers_minus1);
    Get_UE (   sps_seq_parameter_set_id,                        "sps_seq_parameter_set_id");
    Get_UE (   chroma_format_idc,                               "chroma_format_idc"); Param_Info1(Hevc_chroma_format_idc((int8u)chroma_format_idc));
    if (chroma_format_idc==3)
        Skip_SB(                                                "separate_colour_plane_flag");
    Get_UE (    pic_width_in_luma_samples,                      "pic_width_in_luma_samples");
    Get_UE (    pic_height_in_luma_samples,                     "pic_height_in_luma_samples");
    TEST_SB_SKIP(                                               "conformance_window_flag ");
        Skip_UE(                                                "conf_win_left_offset");
        Skip_UE(                                                "conf_win_right_offset");
        Skip_UE(                                                "conf_win_top_offset");
        Skip_UE(                                                "conf_win_bottom_offset");
    TEST_SB_END();
    Get_UE (   bit_depth_luma_minus8,                           "bit_depth_luma_minus8");
    Get_UE (   bit_depth_chroma_minus8,                         "bit_depth_chroma_minus8");
    BS_End();
    Skip_XX(Element_Size-Element_Offset,                        "(ToDo)");

    FILLING_BEGIN_PRECISE()
        //Integrity
        if (sps_seq_parameter_set_id>=16)
        {
            Trusted_IsNot("sps_seq_parameter_set_id not valid");
            return; //Problem, not valid
        }

        //Creating Data
        if (sps_seq_parameter_set_id>=seq_parameter_sets.size())
            seq_parameter_sets.resize(sps_seq_parameter_set_id+1);
        std::vector<seq_parameter_set_struct*>::iterator Data_Item=seq_parameter_sets.begin()+sps_seq_parameter_set_id;
        delete *Data_Item; *Data_Item=new seq_parameter_set_struct();

        //Filling from stream
        (*Data_Item)->IsSynched                                     =true;
        (*Data_Item)->video_parameter_set_id                        =(int8u)sps_video_parameter_set_id;
        (*Data_Item)->profile_idc                                   =profile_idc;
        (*Data_Item)->level_idc                                     =level_idc;
        (*Data_Item)->pic_width_in_luma_samples                     =pic_width_in_luma_samples;
        (*Data_Item)->pic_height_in_luma_samples                    =pic_height_in_luma_samples;
        (*Data_Item)->chroma_format_idc                             =(int8u)chroma_format_idc;
        (*Data_Item)->chroma_format_idc                             =(int8u)chroma_format_idc;
        (*Data_Item)->bit_depth_luma_minus8                         =(int8u)bit_depth_luma_minus8;
        (*Data_Item)->bit_depth_chroma_minus8                       =(int8u)bit_depth_chroma_minus8;
    FILLING_END()
}

//---------------------------------------------------------------------------
// Packet "34"
void File_Hevc::pic_parameter_set()
{
    Element_Name("pic_parameter_set");

    //Warning: based on a draft of the specification, it is maybe not compliant to the final specification

    //Parsing
    int32u  pps_pic_parameter_set_id, pps_seq_parameter_set_id, num_ref_idx_l0_default_active_minus1, num_ref_idx_l1_default_active_minus1;
    int8u   num_extra_slice_header_bits;
    bool    tiles_enabled_flag, dependent_slice_segments_enabled_flag;
    BS_Begin();
    Get_UE (   	pps_pic_parameter_set_id,                       "pps_pic_parameter_set_id");
    Get_UE (   	pps_seq_parameter_set_id,                       "pps_seq_parameter_set_id");
    std::vector<seq_parameter_set_struct*>::iterator seq_parameter_set_Item;
    if (pps_seq_parameter_set_id>=seq_parameter_sets.size() || (*(seq_parameter_set_Item=seq_parameter_sets.begin()+pps_seq_parameter_set_id))==NULL || !(*seq_parameter_set_Item)->IsSynched)
    {
        //Not yet present
        Skip_BS(Data_BS_Remain(),                               "Data (seq_parameter_set is missing)");
        return;
    }
    Get_SB (    dependent_slice_segments_enabled_flag,          "dependent_slice_segments_enabled_flag");
    Skip_SB(                                                    "output_flag_present_flag");
    Get_S1 (3,  num_extra_slice_header_bits,                    "num_extra_slice_header_bits");
    Skip_SB(                                                    "sign_data_hiding_flag");
    Skip_SB(                                                    "cabac_init_present_flag");
    Get_UE (    num_ref_idx_l0_default_active_minus1,           "num_ref_idx_l0_default_active_minus1");
    Get_UE (    num_ref_idx_l1_default_active_minus1,           "num_ref_idx_l1_default_active_minus1");
    Skip_SE(                                                    "init_qp_minus26");
    Skip_SB(                                                    "constrained_intra_pred_flag");
    Skip_SB(                                                    "transform_skip_enabled_flag");
    TEST_SB_SKIP(                                               "cu_qp_delta_enabled_flag");
        Skip_UE(                                                "diff_cu_qp_delta_depth");
    TEST_SB_END();
    Skip_SE(                                                    "pps_cb_qp_offset");
    Skip_SE(                                                    "pps_cr_qp_offset");
    Skip_SB(                                                    "pps_slice_chroma_qp_offsets_present_flag");
    Skip_SB(                                                    "weighted_pred_flag");
    Skip_SB(                                                    "weighted_bipred_flag");
    Skip_SB(                                                    "transquant_bypass_enable_flag");
    Get_SB (    tiles_enabled_flag,                             "tiles_enabled_flag");
    Skip_SB(                                                    "entropy_coding_sync_enabled_flag");
    if (tiles_enabled_flag)
    {
        Element_Begin1("tiles");
        int32u  num_tile_columns_minus1, num_tile_rows_minus1;
        bool    uniform_spacing_flag;
        Get_UE (    num_tile_columns_minus1,                    "num_tile_columns_minus1");
        Get_UE (    num_tile_rows_minus1,                       "num_tile_rows_minus1");
        Get_SB (    uniform_spacing_flag,                       "uniform_spacing_flag");
        if (!uniform_spacing_flag)
        {
            for (int32u tile_pos=0; tile_pos<num_tile_columns_minus1; tile_pos++)
               Skip_UE(                                         "column_width_minus1");
            for (int32u tile_pos=0; tile_pos<num_tile_columns_minus1; tile_pos++)
               Skip_UE(                                         "row_height_minus1");
        }
        Skip_SB(                                                "loop_filter_across_tiles_enabled_flag");
        Element_End0();
    }
    Skip_SB(                                                    "pps_loop_filter_across_slices_enabled_flag");
    TEST_SB_SKIP(                                               "deblocking_filter_control_present_flag");
        bool pps_disable_deblocking_filter_flag;
        Skip_SB(                                                "deblocking_filter_override_enabled_flag");
        Get_SB (    pps_disable_deblocking_filter_flag,         "pps_disable_deblocking_filter_flag");
        if (!pps_disable_deblocking_filter_flag)
        {
           Skip_SE(                                             "pps_beta_offset_div2");
           Skip_SE(                                             "pps_tc_offset_div2");
        }
    TEST_SB_END();
    TEST_SB_SKIP(                                               "pps_scaling_list_data_present_flag ");
        BS_End();
        Skip_XX(Element_Size-Element_Offset,                    "(ToDo)");
        //scaling_list_data();
    TEST_SB_END();
    Skip_SB(                                                    "lists_modification_present_flag");
    Skip_UE(                                                    "log2_parallel_merge_level_minus2");
    Skip_SB(                                                    "slice_segment_header_extension_present_flag");
    TEST_SB_SKIP(                                               "pps_extension_flag");
        Skip_BS(Data_BS_Remain(),                               "pps_extension_data");
    TEST_SB_END();
    BS_End();

    FILLING_BEGIN_PRECISE();
        //Integrity
        if (pps_pic_parameter_set_id>=64)
        {
            Trusted_IsNot("pic_parameter_set_id not valid");
            return; //Problem, not valid
        }
        if (pps_seq_parameter_set_id>=16)
        {
            Trusted_IsNot("seq_parameter_set_id not valid");
            return; //Problem, not valid
        }

        //NextCode
        //NextCode_Clear();
        //NextCode_Add(0x05);
        //NextCode_Add(0x06);

        //Filling
        if (pps_pic_parameter_set_id>=pic_parameter_sets.size())
            pic_parameter_sets.resize(pps_pic_parameter_set_id+1);
        std::vector<pic_parameter_set_struct*>::iterator pic_parameter_sets_Item=pic_parameter_sets.begin()+pps_pic_parameter_set_id;
        delete *pic_parameter_sets_Item; *pic_parameter_sets_Item=new pic_parameter_set_struct();
        (*pic_parameter_sets_Item)->IsSynched                                       =true;
        (*pic_parameter_sets_Item)->seq_parameter_set_id                            =(int8u)pps_seq_parameter_set_id;
        (*pic_parameter_sets_Item)->num_ref_idx_l0_default_active_minus1            =(int8u)num_ref_idx_l0_default_active_minus1;
        (*pic_parameter_sets_Item)->num_ref_idx_l1_default_active_minus1            =(int8u)num_ref_idx_l1_default_active_minus1;
        (*pic_parameter_sets_Item)->dependent_slice_segments_enabled_flag           =dependent_slice_segments_enabled_flag;
        (*pic_parameter_sets_Item)->num_extra_slice_header_bits                     =num_extra_slice_header_bits;

        //Autorisation of other streams
        //if (!seq_parameter_sets.empty())
        //{
        //    for (int8u Pos=0x01; Pos<=0x06; Pos++)
        //    {
        //        Streams[Pos].Searching_Payload=true; //Coded slice...
        //        if (Streams[0x08].ShouldDuplicate)
        //            Streams[Pos].ShouldDuplicate=true;
        //    }
        //}

        //Setting as OK
        if (!Status[IsAccepted])
            Accept("HEVC");
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "35"
void File_Hevc::access_unit_delimiter()
{
    Element_Name("access_unit_delimiter");

    //Parsing
    BS_Begin();
    Info_S1( 3, pic_type,                                   "pic_type"); Param_Info1(Hevc_pic_type[pic_type]);
    BS_End();
}

//---------------------------------------------------------------------------
// Packet "36"
void File_Hevc::end_of_seq()
{
    Element_Name("end_of_seq");
}

//---------------------------------------------------------------------------
// Packet "37"
void File_Hevc::end_of_bitstream()
{
    Element_Name("end_of_bitstream");
}

//---------------------------------------------------------------------------
// Packet "38"
void File_Hevc::filler_data()
{
    Element_Name("filler_data");

    //Parsing
    Skip_XX(Element_Size,                                       "ff_bytes");
}

//---------------------------------------------------------------------------
// Packet "39" or "40
void File_Hevc::sei()
{
    Element_Name("sei");

    //Parsing
    while(Element_Offset+1<Element_Size)
    {
        Element_Begin1("sei message");
            sei_message();
        Element_End0();
    }
    BS_Begin();
    Mark_1(                                                     );
    BS_End();
}

//---------------------------------------------------------------------------
void File_Hevc::sei_message()
{
    //Parsing
    int32u  payloadType=0, payloadSize=0;
    int8u   payload_type_byte, payload_size_byte;
    Element_Begin1("sei message header");
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
    Element_End0();

    int64u Element_Offset_Save=Element_Offset+payloadSize;
    if (Element_Offset_Save>Element_Size)
    {
        Trusted_IsNot("Wrong size");
        Skip_XX(Element_Size-Element_Offset,                    "unknown");
        return;
    }
    int64u Element_Size_Save=Element_Size;
    Element_Size=Element_Offset_Save;
    switch (payloadType)
    {
        //case   0 :   sei_message_buffering_period(seq_parameter_set_id); break;
        //case   1 :   sei_message_pic_timing(payloadSize, seq_parameter_set_id); break;
        //case   4 :   sei_message_user_data_registered_itu_t_t35(); break;
        //case   5 :   sei_message_user_data_unregistered(payloadSize); break;
        //case   6 :   sei_message_recovery_point(); break;
        //case  32 :   sei_message_mainconcept(payloadSize); break;
        case 132 :   sei_message_decoded_picture_hash(payloadSize); break;
        default :
                    Element_Info1("unknown");
                    Skip_XX(payloadSize,                        "data");
    }
    Element_Offset=Element_Offset_Save; //Positionning in the right place.
    Element_Size=Element_Size_Save; //Positionning in the right place.
}

//---------------------------------------------------------------------------
void File_Hevc::sei_message_decoded_picture_hash(int32u payloadSize)
{
    //Parsing
    int8u hash_type;
    Get_B1 (hash_type,                                          "hash_type");
    for (int8u cIdx=0; cIdx<(chroma_format_idc?3:1); cIdx++)
        switch (hash_type)
        {
            case 0 :    // md5
                        Skip_XX(16,                             "md5");
                        break;
            case 1 :    // crc
                        Skip_XX( 2,                             "crc");
                        break;
            case 2 :    // checksum
                        Skip_XX( 4,                             "checksum");
                        break;
            default :   //
                        Skip_XX((Element_Size-1)/(chroma_format_idc?1:3), "unknown");
                        break;
        }
}

//***************************************************************************
// Sub-elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Hevc::slice_segment_header()
{
    Element_Begin1("slice_segment_header");

    //Parsing
    int32u  slice_pic_parameter_set_id, slice_type;
    bool    dependent_slice_segment_flag=false;
    Get_SB (   first_slice_segment_in_pic_flag,                 "first_slice_segment_in_pic_flag");
    if (RapPicFlag)
        Skip_SB(                                                "no_output_of_prior_pics_flag");
    Get_UE (   slice_pic_parameter_set_id,                      "slice_pic_parameter_set_id");
    std::vector<pic_parameter_set_struct*>::iterator pic_parameter_set_Item;
    if (slice_pic_parameter_set_id>=pic_parameter_sets.size() || (*(pic_parameter_set_Item=pic_parameter_sets.begin()+slice_pic_parameter_set_id))==NULL || !(*pic_parameter_set_Item)->IsSynched)
    {
        //Not yet present
        Skip_BS(Data_BS_Remain(),                               "Data (pic_parameter_set is missing)");
        return;
    }
    if (!first_slice_segment_in_pic_flag)
    {
        if ((*pic_parameter_set_Item)->dependent_slice_segments_enabled_flag)
            Get_SB (dependent_slice_segment_flag,               "dependent_slice_segment_flag");
        //Skip_BS(Ceil( Log2( PicSizeInCtbsY ) ),               "slice_segment_address");
        return;
    }
    if (!dependent_slice_segment_flag)
    {
        Skip_S1((*pic_parameter_set_Item)->num_extra_slice_header_bits, "slice_reserved_flags");
        Get_UE (slice_type,                                     "slice_type"); Param_Info1(Hevc_slice_type(slice_type));
    }
    //TODO...

    Element_End0();
}

//---------------------------------------------------------------------------
void File_Hevc::profile_tier_level(int8u maxNumSubLayersMinus1)
{
    Element_Begin1("profile_tier_level");

    //Parsing
    std::vector<bool>sub_layer_profile_present_flags, sub_layer_level_present_flags;
    Info_S1(2,  general_profile_space,                          "general_profile_space");
    Skip_SB(                                                    "general_tier_flag");
    Get_S1 (5,  profile_idc,                                    "general_profile_idc");
    Skip_S4(32,                                                 "general_profile_compatibility_flags");
    Skip_SB(                                                    "general_progressive_source_flag");
    Skip_SB(                                                    "general_interlaced_source_flag");
    Skip_SB(                                                    "general_non_packed_constraint_flag");
    Skip_SB(                                                    "general_frame_only_constraint_flag");
    Skip_S8(44,                                                 "general_reserved_zero_44bits");
    Get_S1 (8,  level_idc,                                      "general_level_idc");
    for (int32u SubLayerPos=0; SubLayerPos<maxNumSubLayersMinus1; SubLayerPos++)
    {
        Element_Begin1("SubLayer");
        bool sub_layer_profile_present_flag, sub_layer_level_present_flag;
        Get_SB (   sub_layer_profile_present_flag,              "sub_layer_profile_present_flag");
        Get_SB (   sub_layer_level_present_flag,                "sub_layer_level_present_flag");
        sub_layer_profile_present_flags.push_back(sub_layer_profile_present_flag);
        sub_layer_level_present_flags.push_back(sub_layer_level_present_flag);
        Element_End0();
    }
    if (maxNumSubLayersMinus1)
        for(int32u SubLayerPos=maxNumSubLayersMinus1; SubLayerPos<8; SubLayerPos++)
            Skip_S1(2,                                          "reserved_zero_2bits");
    for (int32u SubLayerPos=0; SubLayerPos<maxNumSubLayersMinus1; SubLayerPos++)
    {
        Element_Begin1("SubLayer");
        if (sub_layer_profile_present_flags[SubLayerPos])
        {
            Skip_S1(2,                                          "sub_layer_profile_space");
            Skip_SB(                                            "sub_layer_tier_flag");
            Skip_S1(5,                                          "sub_layer_profile_idc");
            Skip_S4(32,                                         "sub_layer_profile_compatibility_flags");
            Skip_SB(                                            "sub_layer_progressive_source_flag");
            Skip_SB(                                            "sub_layer_interlaced_source_flag");
            Skip_SB(                                            "sub_layer_non_packed_constraint_flag");
            Skip_SB(                                            "sub_layer_frame_only_constraint_flag");
            Skip_S8(44,                                         "sub_layer_reserved_zero_44bits");
        }
        if (sub_layer_level_present_flags[SubLayerPos])
        {
            Skip_S1(8,                                          "sub_layer_level_idc");
        }
        Element_End0();
    }

    Element_End0();
}

//***************************************************************************
// Specific
//***************************************************************************

//---------------------------------------------------------------------------
void File_Hevc::SPS_PPS()
{
    //Parsing
	int32u profile_compatibility_indications;
	int16u constraint_indicator_flags;
    int8u chromaFormat, bitDepthLumaMinus8, bitDepthChromaMinus8;
    int8u profile_space, profile_idc, level_idc, min_spatial_segmentation_idc;
	int8u numOfArrays, constantFrameRate, numTemporalLayers;
	bool temporalIdNested;

    //Warning: based on a draft of the specification, it is definitely not compliant to the final specification (unknown addtional bytes)
    BS_Begin();
	    Get_S1 (2, profile_space,                               "profile_space");
	    Skip_SB(                                                "tier_flag");
	    Get_S1 (5, profile_idc,                                 "profile_idc");
    BS_End();
    Get_B4 (profile_compatibility_indications,                  "profile_compatibility_indications");
    Get_B2 (constraint_indicator_flags,                         "constraint_indicator_flags");
    Get_B1 (level_idc,                                          "level_idc");
    Get_B1 (min_spatial_segmentation_idc,                       "min_spatial_segmentation_idc");
    Skip_B1(                                                    "Unknown");
    Skip_B1(                                                    "Unknown");
    Skip_B1(                                                    "Unknown");
    Skip_B1(                                                    "Unknown");
    Skip_B1(                                                    "Unknown");
    BS_Begin();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Skip_S1(2,                                              "parallelismType");
    BS_End();
    BS_Begin();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Get_S1 (2, chromaFormat,                                "chromaFormat");
    BS_End();
    BS_Begin();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Get_S1 (3, bitDepthLumaMinus8,                          "bitDepthLumaMinus8");
    BS_End();
    BS_Begin();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Mark_1_NoTrustError();
	    Get_S1 (3, bitDepthChromaMinus8,                        "bitDepthChromaMinus8");
    BS_End();
    Skip_B2(                                                    "avgFrameRate");
    BS_Begin();
	    Get_S1 (2, constantFrameRate,                           "constantFrameRate");
	    Get_S1 (3, numTemporalLayers,                           "numTemporalLayers");
	    Get_SB (   temporalIdNested,                            "temporalIdNested");
	    Get_S1 (2, lengthSizeMinusOne,                          "lengthSizeMinusOne");
    BS_End();
    Get_B1 (numOfArrays,                                        "numOfArrays");
    for (size_t ArrayPos=0; ArrayPos<numOfArrays; ArrayPos++)
    {
        Element_Begin1("Array");
        int16u numNalus;
        int8u NAL_unit_type;
        BS_Begin();
	        Skip_SB(                                            "array_completeness");
            Mark_0_NoTrustError();
	        Get_S1 (6, NAL_unit_type,                           "NAL_unit_type");
        BS_End();
        Get_B2 (numNalus,                                       "numNalus");
        for (size_t NaluPos=0; NaluPos<numNalus; NaluPos++)
        {
            Element_Begin1("nalUnit");
            int16u nalUnitLength;
            Get_B2 (nalUnitLength,                              "nalUnitLength");
            if (Element_Offset+nalUnitLength>Element_Size)
            {
                Trusted_IsNot("Size is wrong");
                break; //There is an error
            }
            
            //Header
            int8u nal_unit_type;
            BS_Begin();
            Mark_0 ();
            Get_S1 (6, nal_unit_type,                           "nal_unit_type");
            Mark_0_NoTrustError ();
            Mark_0_NoTrustError ();
            Mark_0_NoTrustError ();
            Mark_0_NoTrustError ();
            Mark_0_NoTrustError ();
            Mark_0_NoTrustError ();
            Skip_S1(3,                                          "nuh_temporal_id_plus1");
            BS_End();

            //Data
            int64u Element_Offset_Save=Element_Offset;
            int64u Element_Size_Save=Element_Size;
            Buffer_Offset+=(size_t)Element_Offset_Save;
            Element_Offset=0;
            Element_Size=nalUnitLength-2;
            Element_Code=nal_unit_type;
            Data_Parse();
            Buffer_Offset-=(size_t)Element_Offset_Save;
            Element_Offset=Element_Offset_Save+nalUnitLength-2;
            Element_Size=Element_Size_Save;

            Element_End0();
        }
        Element_End0();
    }

    //Filling
    FILLING_BEGIN_PRECISE();
        MustParse_SPS_PPS=false;
    FILLING_END();
}

} //NameSpace

#endif //MEDIAINFO_HEVC_YES
