// File_Hevc - Info for HEVC files
// Copyright (C) 2013-2012 MediaArea.net SARL, Info@MediaArea.net
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
#ifndef MediaInfo_HevcH
#define MediaInfo_HevcH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Hevc
//***************************************************************************

class File_Hevc : public File__Analyze
{
public :
    //In
    int64u Frame_Count_Valid;
    bool   FrameIsAlwaysComplete;
    bool   MustParse_SPS_PPS;
    bool   SizedBlocks;

    //Config
    int8u                               lengthSizeMinusOne;

    //Constructor/Destructor
    File_Hevc();
    ~File_Hevc();

private :
    File_Hevc(const File_Hevc &File_Hevc); //No copy

    //Structures - video_parameter_set
    struct video_parameter_set_struct
    {
        bool    IsSynched; //Computed value
    };
    typedef vector<video_parameter_set_struct*> video_parameter_set_structs;

    //Structures - seq_parameter_set
    struct seq_parameter_set_struct
    {
        int32u  profile_idc;
        int32u  level_idc;
        int32u  pic_width_in_luma_samples;
        int32u  pic_height_in_luma_samples;
        int8u   video_parameter_set_id;
        int8u   chroma_format_idc;
        int8u   bit_depth_luma_minus8;
        int8u   bit_depth_chroma_minus8;
        bool    IsSynched; //Computed value
    };
    typedef vector<seq_parameter_set_struct*> seq_parameter_set_structs;

    //Structures - pic_parameter_set
    struct pic_parameter_set_struct
    {
        int8u   seq_parameter_set_id;
        int8u   num_ref_idx_l0_default_active_minus1;
        int8u   num_ref_idx_l1_default_active_minus1;
        int8u   num_extra_slice_header_bits;
        bool    dependent_slice_segments_enabled_flag;
        bool    IsSynched; //Computed value
    };
    typedef vector<pic_parameter_set_struct*> pic_parameter_set_structs;

    //Streams management
    void Streams_Fill();
    void Streams_Fill(vector<seq_parameter_set_struct*>::iterator seq_parameter_set_Item);
    void Streams_Finish();

    //Buffer - File header
    bool FileHeader_Begin();

    //Buffer - Synchro
    bool Synchronize();
    bool Synched_Test();
    void Synched_Init();

    //Buffer - Demux
    #if MEDIAINFO_DEMUX
    bool Demux_UnpacketizeContainer_Test();
    #endif //MEDIAINFO_DEMUX

    //Buffer - Global
    void Read_Buffer_Unsynched();

    //Buffer - Per element
    void Header_Parse();
    bool Header_Parser_QuickSearch();
    bool Header_Parser_Fill_Size();
    void Data_Parse();

    //Elements
    void slice_segment_layer();
    void slice_layer();
    void video_parameter_set();
    void seq_parameter_set();
    void pic_parameter_set();
    void access_unit_delimiter();
    void end_of_seq();
    void end_of_bitstream();
    void filler_data();
    void sei();
    void sei_message();
    void sei_message_decoded_picture_hash(int32u payloadSize);

    //Packets - SubElements
    void slice_segment_header();
    void profile_tier_level(int8u maxNumSubLayersMinus1);

    //Packets - Specific
    void SPS_PPS();

    //Streams
    struct stream
    {
        bool   Searching_Payload;

        stream()
        {
            Searching_Payload=false;
        }
    };
    vector<stream> Streams;

    //Replacement of File__Analyze buffer
    const int8u*                        Buffer_ToSave;
    size_t                              Buffer_Size_ToSave;

    //parameter_sets
    video_parameter_set_structs         video_parameter_sets;
    seq_parameter_set_structs           seq_parameter_sets;
    pic_parameter_set_structs           pic_parameter_sets;

    //File specific
    size_t                              IFrame_Count;

    //Temp
    int32u  chroma_format_idc;
    int8u   profile_idc;
    int8u   level_idc;
    bool    RapPicFlag;
    bool    first_slice_segment_in_pic_flag;
};

} //NameSpace

#endif
