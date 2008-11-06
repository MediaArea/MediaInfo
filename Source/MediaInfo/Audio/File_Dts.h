// File_Dts - Info for DTS files
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
#ifndef MediaInfo_DtsH
#define MediaInfo_DtsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Dts
//***************************************************************************

class File_Dts : public File__Analyze
{
public :
    //In
    size_t Frame_Count_Valid;

    //Out
    size_t Delay;

protected :
    //Format
    void Read_Buffer_Continue ();

public :
    File_Dts();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void Data_Parse();
    void Data_Parse_Fill();

    //Elements
    void Core();
    void HD();
    void XCh(int64u Size);
    void XXCh(int64u Size);
    void X96k(int64u Size);
    void XLL(int64u Size);
    void XBR(int64u Size);
    void XSA(int64u Size);

    //Count
    size_t Frame_Count;

    //Temp - Technical info
    std::vector<ZenLib::int32u> Asset_Sizes;
    Ztring Profile;
    int32u SyncCode;
    int16u Primary_Frame_Byte_Size_minus_1;
    int32u HD_size;
    int8u  channel_arrangement;
    int8u  channel_arrangement_XCh;
    int8u  sample_frequency;
    int8u  bit_rate;
    int8u  lfe_effects;
    int8u  bits_per_sample;
    int8u  ExtensionAudioDescriptor;
    bool   ExtendedCoding;
    bool   Word;
    bool   BigEndian;
    bool   ES;
    std::map<int32u, int64u> HD_Sizes;

    //Temp - HD
    int16u HD_SpeakerActivityMask;
    int8u  HD_BitResolution;
    int8u  HD_MaximumSampleRate;
    int8u  HD_TotalNumberChannels;

    //Helpers
    bool Synchronize();
    bool Detect_NonDTS();
};

} //NameSpace

#endif
