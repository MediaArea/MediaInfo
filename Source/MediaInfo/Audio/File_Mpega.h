// File_Mpega - Info for MPEG Audio files
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
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
#ifndef MediaInfo_File_MpegaH
#define MediaInfo_File_MpegaH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/Tag/File__Tags.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpega
//***************************************************************************

class File_Mpega : public File__Analyze, public File__Tags_Helper
{
public :
    //In
    size_t Frame_Count_Valid;
    bool   FrameIsAlwaysComplete;

    //Out
    size_t Delay;

protected :
    //Format
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

public :
    File_Mpega();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void Data_Parse();
    void Data_Parse_Fill();

    //Element
    bool Header_Xing();
    bool Header_VBRI();
    bool Header_Encoders();
    void Header_Encoders_Lame();
    void Encoded_Library_Guess();

    //Temp - Global
    size_t Frame_Count;
    int64u LastSync_Offset;

    //Temp - Technical info
    int8u  ID;
    int8u  layer;
    int8u  bitrate_index;
    int8u  sampling_frequency;
    int8u  mode;
    int8u  mode_extension;
    int8u  emphasis;
    bool   protection_bit;
    bool   padding_bit;
    bool   copyright;
    bool   original_home;

    //Temp - BitStream info
    std::map<int16u, size_t> BitRate_Count;
    size_t Block_Count[3]; //long, short, mixed
    size_t Channels_Count[4]; //Stereo, Join Stereo, Dual mono, mono
    size_t Extension_Count[4]; //No, IS, MS, IS+MS
    size_t Emphasis_Count[4]; //No, 50/15ms, Reserved, CCITT
    size_t Scfsi; //Total
    size_t Scalefac; //Total
    size_t Reservoir; //Total
    int32u Reservoir_Max;

    //Temp - VBR handling
    int32u VBR_Frames;
    int64u VBR_FileSize;
    Ztring BitRate_Mode;
    Ztring BitRate_Nominal;
    Ztring BitRate_Minimum;

    //Temp - Tags in a frame
    Ztring Encoded_Library;
    Ztring Encoded_Library_Settings;

    //Temp - Error detection
    std::map<int8u, size_t> sampling_frequency_Count;
    std::map<int8u, size_t> mode_Count;

    //Temp - Tags
    int32u Xing_Scale;

    //Helpers
    bool Synchronize();
    bool Detect_NonMPEGA();
    bool Element_Name_IsOK();
};

} //NameSpace

#endif
