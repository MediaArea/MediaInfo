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
//
// Information about MPEG Video files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_MpegvH
#define MediaInfo_MpegvH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpegv
//***************************************************************************

class File_Mpegv : public File__Analyze
{
public :
    //In
    int8u  MPEG_Version;
    size_t Frame_Count_Valid;
    bool   FrameIsAlwaysComplete;

    //Constructor/Destructor
    File_Mpegv();

private :
    //Buffer - File header
    bool FileHeader_Begin() {return FileHeader_Begin_0x000001();}

    //Buffer - Synchro
    bool Synchronize() {return Synchronize_0x000001();}
    bool Synched_Test();
    void Synched_Init();
    
    //Buffer - Global
    void Read_Buffer_Finalize ();

    //Buffer - Per element
    void Header_Parse();
    bool Header_Parser_QuickSearch();
    bool Header_Parser_Fill_Size();
    void Data_Parse();

    //EOF
    void Detect_EOF();

    //Elements
    void picture_start();
    void slice_start();
    void slice_start_Fill();
    void user_data_start();
    void sequence_header();
    void sequence_error();
    void extension_start();
    void sequence_end();
    void group_start();

    //Streams
    struct stream
    {
        bool   Searching_Payload;
        bool   Searching_TimeStamp_Start;
        bool   Searching_TimeStamp_End;

        stream()
        {
            Searching_Payload=false;
            Searching_TimeStamp_Start=false;
            Searching_TimeStamp_End=false;
        }
    };
    std::vector<stream> Streams;

    //Temporal reference
    struct temporalreference
    {
        bool   top_field_first;
        bool   repeat_first_field;
    };
    std::map<int16u, temporalreference> TemporalReference; //int32u is the reference
    int16u                              TemporalReference_Offset;

    //Temp
    Ztring Library;
    Ztring Library_Name;
    Ztring Library_Version;
    Ztring Matrix_intra;
    Ztring Matrix_nonintra;
    size_t Frame_Count;
    size_t progressive_frame_Count;
    size_t Interlaced_Top;
    size_t Interlaced_Bottom;
    size_t Time_Begin_Seconds;
    size_t Time_End_Seconds;
    int64u SizeToAnalyse_Begin; //Total size of a chunk to analyse, it may be changed by the parser
    int64u SizeToAnalyse_End; //Total size of a chunk to analyse, it may be changed by the parser
    int32u bit_rate_value;
    float32 FrameRate;
    int16u horizontal_size_value;
    int16u vertical_size_value;
    int16u bit_rate_extension;
    int16u temporal_reference;
    int16u display_horizontal_size;
    int16u display_vertical_size;
    int16u vbv_delay;
    int8u  Time_Begin_Frames;
    int8u  Time_End_Frames;
    int8u  aspect_ratio_information;
    int8u  frame_rate_code;
    int8u  profile_and_level_indication_profile;
    int8u  profile_and_level_indication_level;
    int8u  chroma_format;
    int8u  horizontal_size_extension;
    int8u  vertical_size_extension;
    int8u  frame_rate_extension_n;
    int8u  frame_rate_extension_d;
    int8u  video_format;
    int8u  picture_structure;
    bool   Time_End_NeedComplete;
    bool   load_intra_quantiser_matrix;
    bool   load_non_intra_quantiser_matrix;
    bool   progressive_sequence;
    bool   top_field_first;
    bool   repeat_first_field;
    bool   FirstFieldFound;
    bool   sequence_header_IsParsed;
    bool   group_start_IsParsed;
    bool   group_start_drop_frame_flag;
    bool   group_start_closed_gop;
    bool   group_start_broken_link;
    bool   Time_Begin_Seconds_IsFrozen;
    bool   Searching_TimeStamp_Start_DoneOneTime;
    bool   Parsing_End_ForDTS;
};

} //NameSpace

#endif
