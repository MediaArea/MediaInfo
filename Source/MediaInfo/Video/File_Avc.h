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
#ifndef MediaInfo_AvcH
#define MediaInfo_AvcH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Avc
//***************************************************************************

class File_Avc : public File__Analyze
{
public :
    //In
    size_t Frame_Count_Valid;
    bool   FrameIsAlwaysComplete;
    bool   MustParse_SPS_PPS;
    bool   MustParse_SPS_PPS_Only;
    bool   MustParse_SPS_PPS_Done;
    bool   FromMKV;

protected :
    //Format
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Avc();

private :
    //Replacement of File__Base
    const int8u* Buffer_ToSave;
    size_t Buffer_Size_ToSave;

    //Buffer
    bool Header_Begin();
    void Header_Parse();
    bool Header_Parse_Fill_Size();
    void Data_Parse();

    //Packets
    void slice_layer_without_partitioning_IDR();
    void slice_layer_without_partitioning_non_IDR();
    void slice_header();
    void slice_header_Fill();
    void seq_parameter_set();
    void pic_parameter_set();
    void sei();
    void sei_message();
    void sei_message_user_data_unregistered(int32u payloadSize);
    void sei_message_buffering_period(int32u payloadSize);
    void sei_message_pic_timing(int32u payloadSize);
    void sei_message_user_data_unregistered_x264(int32u payloadSize);
    void sei_message_recovery_point(int32u payloadSize);
    void sei_message_mainconcept(int32u payloadSize);
    void access_unit_delimiter();

    //Packets - SubElements
    void scaling_list(int32u ScalingList_Size);
    void vui_parameters();
    void hrd_parameters();

    //Packets - Specific
    void SPS_PPS();

    //Count of a Packets
    size_t Frame_Count;
    int32u frame_num_LastOne;

    //From seq_parameter_set
    Ztring Encoded_Library;
    Ztring Encoded_Library_Settings;
    Ztring BitRate_Nominal;
    int32u pic_width_in_mbs_minus1;
    int32u pic_height_in_map_units_minus1;
    int32u log2_max_frame_num_minus4;
    int32u num_units_in_tick;
    int32u time_scale;
    int32u chroma_format_idc;
    int32u frame_crop_left_offset;
    int32u frame_crop_right_offset;
    int32u frame_crop_top_offset;
    int32u frame_crop_bottom_offset;
    int16u sar_width;
    int16u sar_height;
    int8u  profile_idc;
    int8u  level_idc;
    int8u  aspect_ratio_idc;
    int8u  video_format;
    int8u  cpb_removal_delay_length_minus1;
    int8u  dpb_output_delay_length_minus1;
    int8u  time_offset_length;
    int8u  pic_struct;
    int8u  pic_struct_FirstDetected;
    bool   frame_mbs_only_flag;
    bool   timing_info_present_flag;
    bool   fixed_frame_rate_flag;
    bool   pic_struct_present_flag;
    bool   field_pic_flag;
    bool   entropy_coding_mode_flag;
    bool   CpbDpbDelaysPresentFlag;

    //PS
    struct avc_stream
    {
        bool   Searching_Payload;

        avc_stream()
        {
            Searching_Payload=false;
        }
    };
    std::map<int8u, avc_stream> Stream;

    //Helpers
    bool Synchronize();
    bool Header_Parser_QuickSearch();
    bool Detect_NonAVC();
};

} //NameSpace

#endif
