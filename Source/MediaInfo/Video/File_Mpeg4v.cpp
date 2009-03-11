// File_Mpeg4v - Info for MPEG-4 Visual files
// Copyright (C) 2006-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_MPEG4V_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Mpeg4v.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const char* Mpeg4v_Colorimetry[]=
{
    "",
    "4:2:0",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpeg4v_visual_object_type[]=
{
    "",
    "video",
    "still texture",
    "mesh",
    "FBA",
    "3D mesh",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpeg4v_visual_object_verid[]=
{
    "",
    "ISO/IEC 14496-2",
    "ISO/IEC 14496-2 AMD 1",
    "ISO/IEC 14496-2 AMD 2",
    "ISO/IEC 14496-2 AMD 3 (Studio)",
    "ISO/IEC 14496-2 AMD 4 (SVP)",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpeg4v_video_object_layer_verid[]=
{
    "",
    "ISO/IEC 14496-2",
    "ISO/IEC 14496-2 AMD 1",
    "ISO/IEC 14496-2 AMD 2",
    "ISO/IEC 14496-2 AMD 3 (Studio)",
    "ISO/IEC 14496-2 AMD 4 (SVP)",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpeg4v_vop_coding_type[]=
{
    "I",
    "P",
    "B",
    "S",
};

//---------------------------------------------------------------------------
const char* Mpeg4v_Profile_Level(int32u Profile_Level)
{
    switch (Profile_Level)
    {
        case B8(00000001) : return "Simple@L1";
        case B8(00000010) : return "Simple@L2";
        case B8(00000011) : return "Simple@L3";
        case B8(00010001) : return "Simple Scalable@L1";
        case B8(00010010) : return "Simple Scalable@L2";
        case B8(00100001) : return "Core@L1";
        case B8(00100010) : return "Core@L2";
        case B8(00110010) : return "Main@L2";
        case B8(00110011) : return "Main@L3";
        case B8(00110100) : return "Main@L4";
        case B8(01000010) : return "N-bit@L2";
        case B8(01010001) : return "Scalable Texture@L1";
        case B8(01100001) : return "Simple Face Animation@L1";
        case B8(01100010) : return "Simple Face Animation@L2";
        case B8(01100011) : return "Simple FBA@L1";
        case B8(01100100) : return "Simple FBA@L2";
        case B8(01110001) : return "Basic Animated Texture@L1";
        case B8(01110010) : return "Basic Animated Texture@L2";
        case B8(10000001) : return "Hybrid@L1";
        case B8(10000010) : return "Hybrid@L2";
        case B8(10010001) : return "Advanced Real Time Simple@L1";
        case B8(10010010) : return "Advanced Real Time Simple@L2";
        case B8(10010011) : return "Advanced Real Time Simple@L3";
        case B8(10010100) : return "Advanced Real Time Simple@L4";
        case B8(10100001) : return "Core Scalable@L1";
        case B8(10100010) : return "Core Scalable@L2";
        case B8(10100011) : return "Core Scalable@L3";
        case B8(10110001) : return "Advanced Coding Efficiency@L1";
        case B8(10110010) : return "Advanced Coding Efficiency@L2";
        case B8(10110011) : return "Advanced Coding Efficiency@L3";
        case B8(10110100) : return "Advanced Coding Efficiency@L4";
        case B8(11000001) : return "Advanced Core@L1";
        case B8(11000010) : return "Advanced Core@L2";
        case B8(11010001) : return "Advanced Scalable Texture@L1";
        case B8(11010010) : return "Advanced Scalable Texture@L2";
        case B8(11010011) : return "Advanced Scalable Texture@L3";
        case B8(11100001) : return "Simple Studio@L1";
        case B8(11100010) : return "Simple Studio@L2";
        case B8(11100011) : return "Simple Studio@L3";
        case B8(11100100) : return "Simple Studio@L4";
        case B8(11100101) : return "Core Studio@L1";
        case B8(11100110) : return "Core Studio@L2";
        case B8(11100111) : return "Core Studio@L3";
        case B8(11101000) : return "Core Studio@L4";
        case B8(11110001) : return "Simple Streaming Video@L1";
        case B8(11110010) : return "Simple Streaming Video@L2";
        case B8(11110011) : return "Simple Streaming Video@L3";
        case B8(11110100) : return "Simple Streaming Video@L4";
        case B8(11110101) : return "Streaming Video@L1";
        case B8(11110110) : return "Streaming Video@L2";
        case B8(11110111) : return "Streaming Video@L3";
        case B8(11111000) : return "Streaming Video@L4";
        case B8(11111111) : return "";
        default :           return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg4v::File_Mpeg4v()
:File__Analyze()
{
    //Config
    Trusted_Multiplier=2;
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=64*1024;

    //In
    Frame_Count_Valid=30;
    FrameIsAlwaysComplete=false;

    //Temp
    video_object_layer_start_IsParsed=false;
}

//---------------------------------------------------------------------------
void File_Mpeg4v::OnlyVOP()
{
    //Default stream values
    Synched_Init();
    Streams[0xB6].Searching_Payload=true; //vop_start
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg4v::Synched_Test()
{
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
void File_Mpeg4v::Synched_Init()
{
    //Count of a Packets
    Frame_Count=0;
    IVOP_Count=0;
    PVOP_Count=0;
    BVOP_Count=0;
    SVOP_Count=0;
    NVOP_Count=0;
    Interlaced_Top=0;
    Interlaced_Bottom=0;

    //From VOL, needed in VOP
    fixed_vop_time_increment=0;
    object_layer_width=0;
    object_layer_height=0;
    vop_time_increment_resolution=0;
    visual_object_verid=1;
    profile_and_level_indication=0;
    no_of_sprite_warping_points=0;
    aspect_ratio_info=0;
    par_width=0;
    par_height=0;
    bits_per_pixel=8;
    shape=0;
    sprite_enable=0;
    estimation_method=0;
    chroma_format=(int8u)-1;
    quarter_sample=false;
    low_delay=false;
    load_intra_quant_mat=false;
    load_nonintra_quant_mat=false;
    load_intra_quant_mat_grayscale=false;
    load_nonintra_quant_mat_grayscale=false;
    interlaced=false;
    newpred_enable=0;
    time_size=0;
    reduced_resolution_vop_enable=0;
    shape=(int8u)-1;
    sprite_enable=0;
    scalability=0;
    enhancement_type=0;
    complexity_estimation_disable=0;
    vop_time_increment_resolution=0;
    opaque=false;
    transparent=false;
    intra_cae=false;
    inter_cae=false;
    no_update=false;
    upsampling=false;
    intra_blocks=false;
    inter_blocks=false;
    inter4v_blocks=false;
    not_coded_blocks=false;
    dct_coefs=false;
    dct_lines=false;
    vlc_symbols=false;
    vlc_bits=false;
    apm=false;
    npm=false;
    interpolate_mc_q=false;
    forw_back_mc_q=false;
    halfpel2=false;
    halfpel4=false;
    sadct=false;
    quarterpel=false;
    quant_type=false;

    //Default stream values
    Streams.resize(0x100);
    Streams[0x00].Searching_Payload=true; //video_object_start
    Streams[0x20].Searching_Payload=true; //video_object_layer_start
    Streams[0xB0].Searching_Payload=true; //visual_object_sequence_start
    Streams[0xB5].Searching_Payload=true; //visual_object_start
    NextCode_Add(0x20); //video_object_layer_start
    for (int8u Pos=0xB7; Pos!=0x00; Pos++)
        Streams[Pos].Searching_Payload=true; //Testing other mpeg4v elements and MPEG-PS
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4v::Read_Buffer_Finalize()
{
    //In case of partial data, and finalizing is forced (example: DecConfig in .mp4), but with at least one frame
    if (!IsFilled && video_object_layer_start_IsParsed)
        vop_start_Fill();

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Streams.clear();
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4v::Header_Parse()
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
bool File_Mpeg4v::Header_Parser_Fill_Size()
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
bool File_Mpeg4v::Header_Parser_QuickSearch()
{
    while (           Buffer_Offset+4<=Buffer_Size
      &&   CC3(Buffer+Buffer_Offset)==0x000001)
    {
        //Getting start_code
        int8u start_code=CC1(Buffer+Buffer_Offset+3);

        //Searching start
        if (Streams[start_code].Searching_Payload)
            return true;

        //Getting size
        Buffer_Offset+=4;
        while(Buffer_Offset+4<=Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
        {
            Buffer_Offset+=2;
            while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]!=0x00)
                Buffer_Offset+=2;
            if (Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset-1]==0x00 || Buffer_Offset>=Buffer_Size)
                Buffer_Offset--;
        }
    }

    if (Buffer_Offset+4<=Buffer_Size)
        Trusted_IsNot("Mpeg4v, Synchronisation lost");
    Synched=false;
    return Synchronize();
}

//---------------------------------------------------------------------------
void File_Mpeg4v::Data_Parse()
{
    //Parsing
    switch (Element_Code)
    {
        case 0xB0: visual_object_sequence_start(); break;
        case 0xB1: visual_object_sequence_end(); break;
        case 0xB2: user_data_start(); break;
        case 0xB3: group_of_vop_start(); break;
        case 0xB4: video_session_error(); break;
        case 0xB5: visual_object_start(); break;
        case 0xB6: vop_start(); break;
        case 0xB7: slice_start(); break;
        case 0xB8: extension_start(); break;
        case 0xB9: fgs_vop_start(); break;
        case 0xBA: fba_object_start(); break;
        case 0xBB: fba_object_plane_start(); break;
        case 0xBC: mesh_object_start(); break;
        case 0xBD: mesh_object_plane_start(); break;
        case 0xBE: still_texture_object_start(); break;
        case 0xBF: texture_spatial_layer_start(); break;
        case 0xC0: texture_snr_layer_start(); break;
        case 0xC1: texture_tile_start(); break;
        case 0xC2: texture_shape_layer_start(); break;
        case 0xC3: stuffing_start(); break;
        default:
                 if (Element_Code<=0x1F) video_object_start();
            else if (Element_Code>=0x20
                  && Element_Code<=0x2F) video_object_layer_start();
            else if (Element_Code>=0x40
                  && Element_Code<=0x4F) fgs_bp_start();
            else if (Element_Code<=0xC5) reserved();
            else
                Trusted_IsNot("Unattended element!");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "00"
void File_Mpeg4v::video_object_start()
{
    Element_Name("video_object_start");

    //Integrity
    if (Element_Size>0)
    {
        Trusted_IsNot("size is wrong");
        return;
    }
}

//---------------------------------------------------------------------------
// Packet "20"
void File_Mpeg4v::video_object_layer_start()
{
    Element_Name("video_object_layer_start");

    //Parsing
    int8u  video_object_layer_verid=visual_object_verid;
    int8u  shape_extension=0;
    int32u aux_comp_count=0;
    BS_Begin();
    Skip_SB(                                                    "random_accessible_vol");
    Skip_S1(8,                                                  "video_object_type_indication");
    TEST_SB_SKIP(                                               "is_object_layer_identifier");
        Get_S1 (4, video_object_layer_verid,                    "video_object_layer_verid"); Param_Info(Mpeg4v_video_object_layer_verid[video_object_layer_verid]);
        Skip_S1(3,                                              "video_object_layer_priority");
    TEST_SB_END();
    Get_S1 (4, aspect_ratio_info,                               "aspect_ratio_info");
    if (aspect_ratio_info==0x0F)
    {
        Get_S1 (8, par_width,                                   "par_width");
        Get_S1 (8, par_height,                                  "par_height");
    }
    TEST_SB_SKIP(                                               "vol_control_parameters");
        Get_S1 (2, chroma_format,                               "chroma_format");
        Get_SB (   low_delay,                                   "low_delay");
        TEST_SB_SKIP(                                           "vbv_parameters");
            Skip_S2(15,                                         "first_half_bit_rate");
            Mark_1 ();
            Skip_S2(15,                                         "latter_half_bit_rate");
            Mark_1 ();
            Skip_S2(15,                                         "first_half_vbv_Element_Size");
            Mark_1 ();
            Skip_S1( 3,                                         "latter_half_vbv_Element_Size");
            Skip_S2(11,                                         "first_half_vbv_occupancy");
            Mark_1 ();
            Skip_S2(15,                                         "latter_half_vbv_occupancy");
            Mark_1 ();
        TEST_SB_END();
    TEST_SB_END();
    Get_S1 (2, shape,                                           "video_object_layer_shape");
    if (shape==3 && video_object_layer_verid!=1) //Shape=GrayScale
        Get_S1 (4, shape_extension,                             "video_object_layer_shape_extension");
    if (shape_extension==0 && shape_extension==1 && shape_extension==5 && shape_extension==7 && shape_extension==8) aux_comp_count=1;
    if (shape_extension==2 && shape_extension==3 && shape_extension==6 && shape_extension==9 && shape_extension==11) aux_comp_count=2;
    if (shape_extension==4 && shape_extension==10 && shape_extension==12) aux_comp_count=3;
    Mark_1 ();
    Get_S2 (16, vop_time_increment_resolution,                  "vop_time_increment_resolution");
    int32u PowerOf2=1;
    for (time_size=0; time_size<=16; time_size++)
    {
        if ((int32u)vop_time_increment_resolution<PowerOf2)
            break;
        PowerOf2<<=1;
    }
    Mark_1 ();
    TEST_SB_SKIP(                                               "fixed_vop_rate");
        Get_BS (time_size, fixed_vop_time_increment,            "fixed_vop_time_increment"); if (vop_time_increment_resolution>0) Param_Info(fixed_vop_time_increment*1000/vop_time_increment_resolution, " ms");
    TEST_SB_END();
    if (shape!=2) //Shape!=BinaryOnly
    {
        if (shape==0) //Shape=Rectangular
        {
            Mark_1 ();
            Get_S2 (13, object_layer_width,                     "object_layer_width");
            Mark_1 ();
            Get_S2 (13, object_layer_height,                    "object_layer_height");
            Mark_1 ();
        }
        Get_SB (interlaced,                                     "interlaced");
        Skip_SB(                                                "obmc_disable");
        if (video_object_layer_verid==1)
            Get_S1 (1, sprite_enable,                           "sprite_enable");
        else
            Get_S1 (2, sprite_enable,                           "sprite_enable");
        if (sprite_enable==1 || sprite_enable==2 )  //static or GMC
        {
            if (sprite_enable!=2) //No GMC
            {
                Skip_S2(13,                                     "sprite_width");
                Mark_1 ();
                Skip_S2(13,                                     "sprite_height");
                Mark_1 ();
                Skip_S2(13,                                     "sprite_top_coordinate");
                Mark_1 ();
                Skip_S2(13,                                     "sprite_left_coordinate");
                Mark_1 ();
            }
            Get_S1 (6, no_of_sprite_warping_points,             "no_of_sprite_warping_points");
            Skip_S1(2,                                          "sprite_warping_accuracy");
            Skip_SB(                                            "sprite_brightness_change");
            if (sprite_enable!=2) //No GMC
                Skip_SB(                                        "low_latency_sprite_enable");
        }
        if (video_object_layer_verid==1 && shape!=0) //Shape!=Rectangular
            Skip_SB(                                            "sadct_disable");
        TEST_SB_SKIP(                                           "bits_per_pixel_not_8_bit");
            Skip_S1(4,                                          "quant_precision");
            Get_S1 (4, bits_per_pixel,                          "bits_per_pixel");
        TEST_SB_END();
        if (shape==3) //Shape=GrayScale
        {
            Skip_SB(                                            "no_gray_quant_update");
            Skip_SB(                                            "composition_method");
            Skip_SB(                                            "linear_composition");
        }
        TEST_SB_GET (quant_type,                                "quant_type");
            Get_SB (load_intra_quant_mat,                       "load_intra_quant_mat");
            if(load_intra_quant_mat)
                for (int16u Pos=0; Pos<64; Pos++)
                {
                    int8u intra_quant_mat;
                    Get_S1 (8, intra_quant_mat,                 "intra_quant_mat");
                    if (!intra_quant_mat)
                    {
                        if (Matrix_intra.size()<2)
                            break; //There is a problem
                        Ztring Value=Matrix_intra.substr(Matrix_intra.size()-2, 2);
                        for (;Pos<64; Pos++)
                            Matrix_intra+=Value;
                    }
                    else
                    {
                        Ztring Value=Ztring::ToZtring(intra_quant_mat, 16);
                        if (Value.size()==1)
                            Value.insert(0, _T("0"));
                        Matrix_intra+=Value;
                    }
                }
            else
                Matrix_intra="Default";
            Get_SB (load_nonintra_quant_mat,                    "load_nonintra_quant_mat");
            if(load_nonintra_quant_mat)
                for (int16u Pos=0; Pos<64; Pos++)
                {
                    int8u nonintra_quant_mat;
                    Get_S1 (8, nonintra_quant_mat,              "nonintra_quant_mat");
                    if (!nonintra_quant_mat)
                    {
                        if (Matrix_nonintra.size()<2)
                            break; //There is a problem
                        Ztring Value=Matrix_nonintra.substr(Matrix_nonintra.size()-2, 2);
                        for (;Pos<64; Pos++)
                            Matrix_nonintra+=Value;
                    }
                    else
                    {
                        Ztring Value=Ztring::ToZtring(nonintra_quant_mat, 16);
                        if (Value.size()==1)
                            Value.insert(0, _T("0"));
                        Matrix_nonintra+=Value;
                    }
                }
            else
                Matrix_nonintra="Default";
            if(shape==3) //Shape=GrayScale
            {
                for(size_t Pos=0; Pos<aux_comp_count; Pos++)
                {
                    Get_SB (load_intra_quant_mat_grayscale,     "load_intra_quant_mat_grayscale");
                    if(load_intra_quant_mat_grayscale)
                        for (int16u Pos=0; Pos<64; Pos++)
                        {
                            int8u intra_quant_mat_grayscale;
                            Get_S1 (8, intra_quant_mat_grayscale, "intra_quant_mat_grayscale");
                            if (!intra_quant_mat_grayscale)
                                Pos=64;
                        }
                    Get_SB (load_nonintra_quant_mat_grayscale,  "load_nonintra_quant_mat_grayscale");
                    if(load_nonintra_quant_mat_grayscale)
                        for (int16u Pos=0; Pos<64; Pos++)
                        {
                            int8u nonintra_quant_mat_grayscale;
                            Get_S1 (8, nonintra_quant_mat_grayscale, "nonintra_quant_mat_grayscale");
                            if (!nonintra_quant_mat_grayscale)
                                Pos=64;
                        }
                }
            }
        TEST_SB_END();
        if (video_object_layer_verid!=1)
        {
            Get_SB (quarter_sample,                             "quarter_sample");
        }
        Get_SB (complexity_estimation_disable,                  "complexity_estimation_disable");
        if (!complexity_estimation_disable)
        {
            Get_S1 (2, estimation_method,                       "estimation_method");
            if (estimation_method==0 || estimation_method==1)
            {
                TEST_SB_SKIP(                                   "shape_complexity_estimation_disable");
                    Get_SB (opaque,                             "opaque");
                    Get_SB (transparent,                        "transparent");
                    Get_SB (intra_cae,                          "intra_cae");
                    Get_SB (inter_cae,                          "inter_cae");
                    Get_SB (no_update,                          "no_update");
                    Get_SB (upsampling,                         "upsampling");
                TEST_SB_END();
                TEST_SB_SKIP(                                   "texture_complexity_estimation_set_1_disable");
                    Get_SB (intra_blocks,                       "intra_blocks");
                    Get_SB (inter_blocks,                       "inter_blocks");
                    Get_SB (inter4v_blocks,                     "inter4v_blocks");
                    Get_SB (not_coded_blocks,                   "not_coded_blocks");
                TEST_SB_END();
                Mark_1 ();
                TEST_SB_SKIP(                                   "texture_complexity_estimation_set_2_disable");
                    Get_SB (dct_coefs,                          "dct_coefs");
                    Get_SB (dct_lines,                          "dct_lines");
                    Get_SB (vlc_symbols,                        "vlc_symbols");
                    Get_SB (vlc_bits,                           "vlc_bits");
                TEST_SB_END();
                TEST_SB_SKIP(                                   "motion_compensation_complexity_disable");
                    Get_SB (apm,                                "apm");
                    Get_SB (npm,                                "npm");
                    Get_SB (interpolate_mc_q,                   "interpolate_mc_q");
                    Get_SB (forw_back_mc_q,                     "forw_back_mc_q");
                    Get_SB (halfpel2,                           "halfpel2");
                    Get_SB (halfpel4,                           "halfpel4");
                TEST_SB_END();
                Mark_1 ();
                if (estimation_method==1)
                {
                    TEST_SB_SKIP(                               "version2_complexity_estimation_disable");
                        Get_SB (sadct,                          "sadct");
                        Get_SB (quarterpel,                     "quarterpel");
                    TEST_SB_END();
                }
            }
        }
        Skip_SB(                                                "resync_marker_disable");
        TEST_SB_SKIP(                                           "data_partitioned");
            Skip_SB(                                            "reversible_vlc");
        TEST_SB_END();
        if (video_object_layer_verid!=1)
        {
            Get_SB (newpred_enable,                             "newpred_enable");
            if (newpred_enable)
            {
                Skip_S1(2,                                      "requested_upstream_message_type");
                Skip_SB(                                        "newpred_segment_type");
            }
            Get_SB (reduced_resolution_vop_enable,              "reduced_resolution_vop_enable");
        }
        Get_SB (scalability,                                    "scalability");
        if (scalability==1)
        {
            bool Hierarchy;
            Get_SB (   Hierarchy,                               "hierarchy_type");
            Skip_S1(4,                                          "ref_layer_id");
            Skip_SB(                                            "ref_layer_sampling_direc");
            Skip_S1(5,                                          "hor_sampling_factor_n");
            Skip_S1(5,                                          "hor_sampling_factor_m");
            Skip_S1(5,                                          "vert_sampling_factor_n");
            Skip_S1(5,                                          "vert_sampling_factor_m");
            Get_SB (   enhancement_type,                        "enhancement_type");
            if (shape==1 && Hierarchy==0) //Shape=Binary
            {
                Skip_SB(                                        "use_ref_shape");
                Skip_SB(                                        "use_ref_texture");
                Skip_S1(5,                                      "shape_hor_sampling_factor_n");
                Skip_S1(5,                                      "shape_hor_sampling_factor_m");
                Skip_S1(5,                                      "shape_vert_sampling_factor_n");
                Skip_S1(5,                                      "shape_vert_sampling_factor_m");
            }
        }
    }
    else
    {
        if (video_object_layer_verid!=1)
        {
            TEST_SB_SKIP(                                       "scalability");
                Skip_S1(4,                                      "ref_layer_id");
                Skip_S1(5,                                      "shape_hor_sampling_factor_n");
                Skip_S1(5,                                      "shape_hor_sampling_factor_m");
                Skip_S1(5,                                      "shape_vert_sampling_factor_n");
                Skip_S1(5,                                      "shape_vert_sampling_factor_m");
            TEST_SB_END();
        }
        Skip_SB(                                                "resync_marker_disable");
    }
    BS_End();

    //Coherancy
    if (object_layer_width==0 || object_layer_height==0 || object_layer_width/object_layer_height<0.1 || object_layer_width/object_layer_height>10)
        Trusted_IsNot("Problem with width and height!");

    FILLING_BEGIN();
        //NextCode
        NextCode_Test();
        NextCode_Clear();
        NextCode_Add(0xB2); //user_data
        NextCode_Add(0xB3); //group_of_vop_start
        NextCode_Add(0xB6); //vop_start
        for (int8u Pos=0x00; Pos<0x1F; Pos++)
            NextCode_Add(Pos); //video_object_start

        //Autorisation of other streams
        Streams[0xB2].Searching_Payload=true; //user_data
        Streams[0xB3].Searching_Payload=true; //group_of_vop_start
        Streams[0xB6].Searching_Payload=true; //vop_start

        //Setting as OK
        video_object_layer_start_IsParsed=true;
    FILLING_END()
}

//---------------------------------------------------------------------------
// Packet "40"
//
void File_Mpeg4v::fgs_bp_start()
{
    Element_Name("fgs_bp_start");
}

//---------------------------------------------------------------------------
// Packet "B0"
void File_Mpeg4v::visual_object_sequence_start()
{
    Element_Name("visual_object_sequence_start");

    //Parsing
    Get_B1 (profile_and_level_indication,                       "profile_and_level_indication"); Param_Info(Mpeg4v_Profile_Level(profile_and_level_indication));

    //Integrity
    if (Element_Size>1)
    {
        Trusted_IsNot("Size is wrong");
        return;
    }

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0xB1); //visual_object_sequence_end
        NextCode_Add(0xB2); //user_data
        NextCode_Add(0xB5); //visual_object_start

        //Autorisation of other streams
        Streams[0xB1].Searching_Payload=true, //visual_object_sequence_end
        Streams[0xB2].Searching_Payload=true; //user_data
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B1"
void File_Mpeg4v::visual_object_sequence_end()
{
    Element_Name("visual_object_sequence_end");

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0xB0); //visual_object_sequence_start
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B2", User defined size, this is often used of library name
void File_Mpeg4v::user_data_start()
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
        if (Temp.size()>=4)
        {
            if (Temp.find(_T("build"))==0)
                Library+=Ztring(_T(" "))+Temp;
            else
                Library=Temp;

            //Library
            if (Library.find(_T("DivX50"))==0)
            {
                Library_Name=_T("DivX");
                Ztring Version=Library.SubString(_T("Build"), _T(""));
                if (Version.empty())
                    Version=Library.SubString(_T("b"), _T("p"));
                if (Version.empty())
                    Version=Library.SubString(_T("b"), _T(""));
                Library_Version=MediaInfoLib::Config.Library_Get(InfoLibrary_Format_DivX, Version, InfoLibrary_Version);
                if (Library_Version.empty())
                    Library_Version=Version;
                Library_Date=MediaInfoLib::Config.Library_Get(InfoLibrary_Format_DivX, Version, InfoLibrary_Date);
            }
            if (Library.find(_T("XviD"))==0)
            {
                Library_Name=_T("XviD");
                if (Library.find(_T("build="))==std::string::npos)
                {
                    Ztring Version=Library.SubString(_T("XviD"), _T(""));
                    Version.FindAndReplace(_T("C"), _T(""));
                    Version.TrimLeft(_T('0'));
                    Library_Version=MediaInfoLib::Config.Library_Get(InfoLibrary_Format_XviD, Version, InfoLibrary_Version);
                    if (Library_Version.empty())
                        Library_Version=Version;
                    Library_Date=MediaInfoLib::Config.Library_Get(InfoLibrary_Format_XviD, Version, InfoLibrary_Date);
                }
                else
                {
                    Library_Version=Library.SubString(_T("XviD"), _T(""));
                    Ztring Date=Library.SubString(_T(" build="), _T(""));
                    if (Date.size()==10)
                    {
                        Date[4]=_T('-');
                        Date[7]=_T('-');
                        Library_Date=_T("UTC ")+Date;
                    }
                }
            }
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B3"
void File_Mpeg4v::group_of_vop_start()
{
    Element_Name("group_of_vop_start");

    //Parsing
    BS_Begin();
    Skip_BS(18,                                                 "time_code");
    Skip_BS( 1,                                                 "closed_gov");
    Skip_BS( 1,                                                 "broken_link");
    BS_End();

    FILLING_BEGIN();
        //NextCode
        NextCode_Test();
        NextCode_Clear();
        for (int8u Pos=0x00; Pos<0x1F; Pos++)
            NextCode_Add(Pos); //video_object_start
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B4"
void File_Mpeg4v::video_session_error()
{
    Element_Name("video_session_error");
}

//---------------------------------------------------------------------------
// Packet "B5"
void File_Mpeg4v::visual_object_start()
{
    Element_Name("visual_object_start");

    //Parsing
    int8u visual_object_type;
    BS_Begin();
    TEST_SB_SKIP(                                               "is_visual_object_identifier");
        Get_S1 ( 4, visual_object_verid,                        "visual_object_verid");  Param_Info(Mpeg4v_visual_object_verid[visual_object_verid]);
        Skip_BS( 3,                                             "visual_object_priority");
    TEST_SB_END();
    Get_S1 ( 4, visual_object_type,                             "visual_object_type"); Param_Info(Mpeg4v_visual_object_type[visual_object_type]);
    if (visual_object_type==1 || visual_object_type==2)
    {
        TEST_SB_SKIP(                                           "video_signal_type");
            Skip_S1(3,                                          "video_format");
            Skip_SB(                                            "video_range");
            TEST_SB_SKIP(                                       "colour_description");
                Skip_S1(8,                                      "colour_primaries");
                Skip_S1(8,                                      "transfer_characteristics");
                Skip_S1(8,                                      "matrix_coefficients");
            TEST_SB_END();
        TEST_SB_END();
        BS_End();

        //Integrity
        if (Element_Offset<Element_Size)
            Trusted_IsNot("Size is wrong");
    }

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0xB2); //user_data
        for (int8u Pos=0x00; Pos<0x2F; Pos++)
            NextCode_Add(Pos); //video_object_start and video_object_layer_start

        //Autorisation of other streams
        Streams[0xB2].Searching_Payload=true;
        for (int8u Pos=0x00; Pos<0x25; Pos++)
            Streams[Pos].Searching_Payload=true; //video_object_start and video_object_layer_start
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "B6"
void File_Mpeg4v::vop_start()
{
    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;
    Frame_Count_InThisBlock++;

    //Name
    Element_Name("vop_start");
    Element_Info(Ztring(_T("Frame ")+Ztring::ToZtring(Frame_Count)));

    //Parsing
    int8u vop_coding_type;
    bool  vop_coded;
    BS_Begin();
    Get_S1 (2, vop_coding_type,                                 "vop_coding_type"); Param_Info(Mpeg4v_vop_coding_type[vop_coding_type]);
    Element_Info(Mpeg4v_vop_coding_type[vop_coding_type]);
    bool modulo_time_base_Continue;
    int8u modulo_time_base=0;
    do
    {
        Get_SB (modulo_time_base_Continue,                      "modulo_time_base");
        modulo_time_base++;
    }
    while (modulo_time_base_Continue);
    Mark_1 ();
    Info_S4(time_size, vop_time_increment,                      "vop_time_increment"); if (vop_time_increment_resolution) Param_Info(vop_time_increment*1000/vop_time_increment_resolution, " ms");
    Mark_1 ();
    Get_SB (vop_coded,                                          "vop_coded");
    if (vop_coded)
    {
        if (newpred_enable)
        {
            Skip_S4(time_size+3<15?time_size+3:15,              "vop_id");
            TEST_SB_SKIP(                                       "vop_id_for_prediction_indication");
                Skip_BS(time_size+3<15?time_size+3:15,          "vop_id_for_prediction");
            TEST_SB_END();
            Mark_1 ();
        }

        if (shape!=2              //Shape!=BinaryOnly
         && (vop_coding_type==1   //Type=P
          || (vop_coding_type==3  //Type=S
           && sprite_enable==2))) //Sprite_Enable=GMC
            Skip_SB(                                            "vop_rounding_type");
        if (reduced_resolution_vop_enable==1
         && shape==0                         //Shape=Rectangular
         && (vop_coding_type==1              //Type=P
          || vop_coding_type==0))            //Type=I
            Skip_SB(                                            "vop_reduced_resolution");
        if (shape!=0) //Shape!=Rectangular
        {
            if (sprite_enable==1    //Sprite_Enable=Static
             && vop_coding_type==0) //Type=I
            {
                Skip_S2(13,                                     "vop_width");
                Mark_1 ();
                Skip_S2(13,                                     "vop_height");
                Mark_1 ();
                Skip_S2(13,                                     "vop_horizontal_mc_spatial_ref");
                Mark_1 ();
                Skip_S2(13,                                     "vop_vertical_mc_spatial_ref");
                Mark_1 ();
            }
            if (shape!=2             //Shape=BinaryOnly
             && scalability==1
             && enhancement_type==1)
                Skip_SB(                                        "background_composition");
            Skip_SB(                                            "change_conv_ratio_disable");
            TEST_SB_SKIP(                                       "vop_constant_alpha");
                Skip_S1(8,                                      "vop_constant_alpha_value");
            TEST_SB_END();
        }

        if (shape!=2) //Shape=BinaryOnly
            if (complexity_estimation_disable==0)
            {
                if (estimation_method==0)
                {
                    if (vop_coding_type==0) //I
                    {
                        if (opaque)             Skip_S1(8,      "dcecs_opaque");
                        if (transparent)        Skip_S1(8,      "dcecs_transparent");
                        if (intra_cae)          Skip_S1(8,      "dcecs_intra_cae");
                        if (inter_cae)          Skip_S1(8,      "dcecs_inter_cae");
                        if (no_update)          Skip_S1(8,      "dcecs_no_update");
                        if (upsampling)         Skip_S1(8,      "dcecs_upsampling");
                        if (intra_blocks)       Skip_S1(8,      "dcecs_intra_blocks");
                        if (not_coded_blocks)   Skip_S1(8,      "dcecs_not_coded_blocks");
                        if (dct_coefs)          Skip_S1(8,      "dcecs_dct_coefs");
                        if (dct_lines)          Skip_S1(8,      "dcecs_dct_lines");
                        if (vlc_symbols)        Skip_S1(8,      "dcecs_vlc_symbols");
                        if (vlc_bits)           Skip_S1(4,      "dcecs_vlc_bits");
                        if (sadct)              Skip_S1(8,      "dcecs_sadct");
                    }
                    if (vop_coding_type==1) //P
                    {
                        if (opaque)             Skip_S1(8,      "dcecs_opaque");
                        if (transparent)        Skip_S1(8,      "dcecs_transparent");
                        if (intra_cae)          Skip_S1(8,      "dcecs_intra_cae");
                        if (inter_cae)          Skip_S1(8,      "dcecs_inter_cae");
                        if (no_update)          Skip_S1(8,      "dcecs_no_update");
                        if (upsampling)         Skip_S1(8,      "dcecs_upsampling");
                        if (intra_blocks)       Skip_S1(8,      "dcecs_intra_blocks");
                        if (not_coded_blocks)   Skip_S1(8,      "dcecs_not_coded_blocks");
                        if (dct_coefs)          Skip_S1(8,      "dcecs_dct_coefs");
                        if (dct_lines)          Skip_S1(8,      "dcecs_dct_lines");
                        if (vlc_symbols)        Skip_S1(8,      "dcecs_vlc_symbols");
                        if (vlc_bits)           Skip_S1(4,      "dcecs_vlc_bits");
                        if (inter_blocks)       Skip_S1(8,      "dcecs_inter_blocks");
                        if (inter4v_blocks)     Skip_S1(8,      "dcecs_inter4v_blocks");
                        if (apm)                Skip_S1(8,      "dcecs_apm");
                        if (npm)                Skip_S1(8,      "dcecs_npm");
                        if (forw_back_mc_q)     Skip_S1(8,      "dcecs_forw_back_mc_q");
                        if (halfpel2)           Skip_S1(8,      "dcecs_halfpel2");
                        if (halfpel4)           Skip_S1(8,      "dcecs_halfpel4");
                        if (sadct)              Skip_S1(8,      "dcecs_sadct");
                        if (quarterpel)         Skip_S1(8,      "dcecs_quarterpel");
                    }
                    if (vop_coding_type==2) //B
                    {
                        if (opaque)             Skip_S1(8,      "dcecs_opaque");
                        if (transparent)        Skip_S1(8,      "dcecs_transparent");
                        if (intra_cae)          Skip_S1(8,      "dcecs_intra_cae");
                        if (inter_cae)          Skip_S1(8,      "dcecs_inter_cae");
                        if (no_update)          Skip_S1(8,      "dcecs_no_update");
                        if (upsampling)         Skip_S1(8,      "dcecs_upsampling");
                        if (intra_blocks)       Skip_S1(8,      "dcecs_intra_blocks");
                        if (not_coded_blocks)   Skip_S1(8,      "dcecs_not_coded_blocks");
                        if (dct_coefs)          Skip_S1(8,      "dcecs_dct_coefs");
                        if (dct_lines)          Skip_S1(8,      "dcecs_dct_lines");
                        if (vlc_symbols)        Skip_S1(8,      "dcecs_vlc_symbols");
                        if (vlc_bits)           Skip_S1(4,      "dcecs_vlc_bits");
                        if (inter_blocks)       Skip_S1(8,      "dcecs_inter_blocks");
                        if (inter4v_blocks)     Skip_S1(8,      "dcecs_inter4v_blocks");
                        if (apm)                Skip_S1(8,      "dcecs_apm");
                        if (npm)                Skip_S1(8,      "dcecs_npm");
                        if (forw_back_mc_q)     Skip_S1(8,      "dcecs_forw_back_mc_q");
                        if (halfpel2)           Skip_S1(8,      "dcecs_halfpel2");
                        if (halfpel4)           Skip_S1(8,      "dcecs_halfpel4");
                        if (interpolate_mc_q)   Skip_S1(8,      "dcecs_interpolate_mc_q");
                        if (sadct)              Skip_S1(8,      "dcecs_sadct");
                        if (quarterpel)         Skip_S1(8,      "dcecs_sadct");
                    }
                    if (vop_coding_type==3 && sprite_enable==1) //B and static
                    {
                        if (intra_blocks)       Skip_S1(8,      "dcecs_intra_blocks");
                        if (not_coded_blocks)   Skip_S1(8,      "dcecs_not_coded_blocks");
                        if (dct_coefs)          Skip_S1(8,      "dcecs_dct_coefs");
                        if (dct_lines)          Skip_S1(8,      "dcecs_dct_lines");
                        if (vlc_symbols)        Skip_S1(8,      "dcecs_vlc_symbols");
                        if (vlc_bits)           Skip_S1(4,      "dcecs_vlc_bits");
                        if (inter_blocks)       Skip_S1(8,      "dcecs_inter_blocks");
                        if (inter4v_blocks)     Skip_S1(8,      "dcecs_inter4v_blocks");
                        if (apm)                Skip_S1(8,      "dcecs_apm");
                        if (npm)                Skip_S1(8,      "dcecs_npm");
                        if (forw_back_mc_q)     Skip_S1(8,      "dcecs_forw_back_mc_q");
                        if (halfpel2)           Skip_S1(8,      "dcecs_halfpel2");
                        if (halfpel4)           Skip_S1(8,      "dcecs_halfpel4");
                        if (interpolate_mc_q)   Skip_S1(8,      "dcecs_interpolate_mc_q");
                    }
                }
            }
        if (shape!=2) //Shape!=BinaryOnly
        {
            Skip_S1(3,                                          "intra_dc_vlc_thr");
            if (interlaced)
            {
                bool top_field_first;
                Get_SB (top_field_first,                        "top_field_first");
                Skip_SB(                                        "alternate_vertical_scan_flag");

                //Filling
                if (top_field_first)
                    Interlaced_Top++;
                else
                    Interlaced_Bottom++;
            }
        }
        //...
    }

    if (!vop_coded)              NVOP_Count++; //VOP with no data
    else if (vop_coding_type==0) IVOP_Count++; //Type I
    else if (vop_coding_type==1) PVOP_Count++; //Type P
    else if (vop_coding_type==2) BVOP_Count++; //Type B
    else if (vop_coding_type==3) SVOP_Count++; //Type S


    FILLING_BEGIN();
        //NextCode
        NextCode_Test();
        NextCode_Clear();
        for (int8u Pos=0x00; Pos<0x2F; Pos++)
            NextCode_Add(Pos); //video_object_start and video_object_layer_start
        NextCode_Add(0xB1); //visual_object_sequence_end
        NextCode_Add(0xB3); //group_of_vop_start_code
        NextCode_Add(0xB5); //visual_object_start
        NextCode_Add(0xB6); //vop_start

        //Filling only if not already done
        if (Frame_Count>=Frame_Count_Valid && Count_Get(Stream_Video)==0)
            vop_start_Fill();

    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4v::vop_start_Fill()
{
    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "MPEG-4 Visual");
    Stream_Prepare(Stream_Video);
    Fill(Stream_Video, 0, Video_Format, "MPEG-4 Visual");
    Fill(Stream_Video, 0, Video_Codec, "MPEG-4V");

    if (profile_and_level_indication>0)
    {
        Fill(Stream_Video, 0, Video_Format_Profile, Mpeg4v_Profile_Level(profile_and_level_indication));
        Fill(Stream_Video, 0, Video_Codec_Profile, Mpeg4v_Profile_Level(profile_and_level_indication));
    }

    if (fixed_vop_time_increment && vop_time_increment_resolution)
        Fill(Stream_Video, StreamPos_Last, Video_FrameRate, ((float)vop_time_increment_resolution)/fixed_vop_time_increment);
    if (object_layer_height)
    {
        Fill(Stream_Video, StreamPos_Last, Video_Width, object_layer_width);
        Fill(Stream_Video, StreamPos_Last, Video_Height, object_layer_height);
        float32 PixelAspectRatio_Value=1.0;
             if (aspect_ratio_info==0x01) PixelAspectRatio_Value=(float32)1;
             if (aspect_ratio_info==0x02) PixelAspectRatio_Value=(float32)12/(float32)11;
        else if (aspect_ratio_info==0x03) PixelAspectRatio_Value=(float32)10/(float32)11;
        else if (aspect_ratio_info==0x04) PixelAspectRatio_Value=(float32)16/(float32)11;
        else if (aspect_ratio_info==0x05) PixelAspectRatio_Value=(float32)40/(float32)13;
        else if (aspect_ratio_info==0x0F && par_height) PixelAspectRatio_Value=((float32)par_width)/par_height;
        Fill(Stream_Video, 0, Video_PixelAspectRatio, PixelAspectRatio_Value);
        Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, ((float)object_layer_width)/object_layer_height*PixelAspectRatio_Value);
    }
    Fill(Stream_Video, 0, Video_Resolution, bits_per_pixel*3);
    if (chroma_format<4)
        Fill(Stream_Video, 0, Video_Colorimetry, Mpeg4v_Colorimetry[chroma_format]);
    if (low_delay)
    {
        Fill(Stream_Video, 0, Video_Format_Settings_BVOP, "No");
        Fill(Stream_Video, 0, Video_Codec_Settings_BVOP, "No");
    }
    else
    {
        Fill(Stream_Video, 0, Video_Format_Settings, "BVOP");
        Fill(Stream_Video, 0, Video_Format_Settings_BVOP, "Yes");
        Fill(Stream_Video, 0, Video_Codec_Settings, "BVOP");
        Fill(Stream_Video, 0, Video_Codec_Settings_BVOP, "Yes");
    }
    if (no_of_sprite_warping_points)
    {
        Fill(Stream_Video, 0, Video_Format_Settings, Ztring(_T("GMC"))+Ztring::ToZtring(no_of_sprite_warping_points));
        Fill(Stream_Video, 0, Video_Format_Settings_GMC, no_of_sprite_warping_points);
        Fill(Stream_Video, 0, Video_Codec_Settings, Ztring(_T("GMC"))+Ztring::ToZtring(no_of_sprite_warping_points));
        Fill(Stream_Video, 0, Video_Codec_Settings_GMC, no_of_sprite_warping_points);
    }
    else
    {
        Fill(Stream_Video, 0, Video_Format_Settings_GMC, 0);
        Fill(Stream_Video, 0, Video_Codec_Settings_GMC, 0);
    }
    if (quarter_sample)
    {
        Fill(Stream_Video, 0, Video_Format_Settings, "QPel");
        Fill(Stream_Video, 0, Video_Format_Settings_QPel, "Yes");
        Fill(Stream_Video, 0, Video_Codec_Settings, "QPel");
        Fill(Stream_Video, 0, Video_Codec_Settings_QPel, "Yes");
    }
    else
    {
        Fill(Stream_Video, 0, Video_Format_Settings_QPel, "No");
        Fill(Stream_Video, 0, Video_Codec_Settings_QPel, "No");
    }
    if (!quant_type)
    {
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix, "Default (H.263)");
        Fill(Stream_Video, 0, Video_Codec_Settings_Matrix, "Default (H.263)");
    }
    else if (load_intra_quant_mat_grayscale || load_nonintra_quant_mat_grayscale)
    {
        Fill(Stream_Video, 0, Video_Format_Settings, "Custom Matrix (Gray)");
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix, "Custom (Gray)");
        Fill(Stream_Video, 0, Video_Codec_Settings, "Custom Matrix (Gray)");
        Fill(Stream_Video, 0, Video_Codec_Settings_Matrix, "Custom (Gray)");
    }
    else if (load_intra_quant_mat || load_nonintra_quant_mat)
    {
        Fill(Stream_Video, 0, Video_Format_Settings, "Custom Matrix");
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix, "Custom");
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix_Data, Matrix_intra);
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix_Data, Matrix_nonintra);
        Fill(Stream_Video, 0, Video_Codec_Settings, "Custom Matrix");
        Fill(Stream_Video, 0, Video_Codec_Settings_Matrix, "Custom");
    }
    else
    {
        Fill(Stream_Video, 0, Video_Format_Settings_Matrix, "Default (MPEG)");
        Fill(Stream_Video, 0, Video_Codec_Settings_Matrix, "Default (MPEG)");
    }
    if (interlaced)
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
    else
    {
        Fill(Stream_Video, 0, Video_ScanType, "Progressive");
        Fill(Stream_Video, 0, Video_Interlacement, "PPF");
    }
    if (!Library.empty())
    {
        Fill(Stream_Video, 0, Video_Encoded_Library, Library);
        Fill(Stream_Video, 0, Video_Encoded_Library_Name, Library_Name);
        Fill(Stream_Video, 0, Video_Encoded_Library_Version, Library_Version);
        Fill(Stream_Video, 0, Video_Encoded_Library_Date, Library_Date);
    }

    //Jumping
    Accept("MPEG-4 Visual");
    IsFilled=true;
    Finish("MPEG-4 Visual");
}

//---------------------------------------------------------------------------
// Packet "B7"
void File_Mpeg4v::slice_start()
{
    Element_Name("slice_start");
}

//---------------------------------------------------------------------------
// Packet "B8"
void File_Mpeg4v::extension_start()
{
    Element_Name("extension_start");
}

//---------------------------------------------------------------------------
// Packet "B9"
void File_Mpeg4v::fgs_vop_start()
{
    Element_Name("fgs_vop_start");
}

//---------------------------------------------------------------------------
// Packet "BA"
void File_Mpeg4v::fba_object_start()
{
    Element_Name("fba_object_start");
}

//---------------------------------------------------------------------------
// Packet "BB"
void File_Mpeg4v::fba_object_plane_start()
{
    Element_Name("fba_object_plane_start");
}

//---------------------------------------------------------------------------
// Packet "BC"
void File_Mpeg4v::mesh_object_start()
{
    Element_Name("mesh_object_start");
}

//---------------------------------------------------------------------------
// Packet "BD"
void File_Mpeg4v::mesh_object_plane_start()
{
    Element_Name("mesh_object_plane_start");
}

//---------------------------------------------------------------------------
// Packet "BE"
void File_Mpeg4v::still_texture_object_start()
{
    Element_Name("still_texture_object_start");
}

//---------------------------------------------------------------------------
// Packet "BF"
void File_Mpeg4v::texture_spatial_layer_start()
{
    Element_Name("texture_spatial_layer_start");
}

//---------------------------------------------------------------------------
// Packet "C0"
void File_Mpeg4v::texture_snr_layer_start()
{
    Element_Name("texture_snr_layer_start");
}

//---------------------------------------------------------------------------
// Packet "C1"
void File_Mpeg4v::texture_tile_start()
{
    Element_Name("texture_tile_start");
}

//---------------------------------------------------------------------------
// Packet "C2"
void File_Mpeg4v::texture_shape_layer_start()
{
    Element_Name("texture_shape_layer_start");
}

//---------------------------------------------------------------------------
// Packet "C3"
void File_Mpeg4v::stuffing_start()
{
    Element_Name("stuffing_start");
}

//---------------------------------------------------------------------------
// Packet "C4" and C5"
void File_Mpeg4v::reserved()
{
    Element_Name("reserved");
}


//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4V_YES
