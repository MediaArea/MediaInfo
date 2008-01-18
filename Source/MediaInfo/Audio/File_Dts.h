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

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Dts();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void Data_Parse();
    void Data_Parse_Fill();

    //Count
    size_t Frame_Count;

    //Temp - Technical info
    int8u  channel_arrangement;
    int8u  sample_frequency;
    int8u  bit_rate;
    int8u  lfe_effects;
    int8u  bits_per_sample;
    int8u  ExtensionAudioDescriptor;
    bool   ExtendedCoding;
    bool   Word;
    bool   BigEndian;

    //Temp - Unknown
    size_t DTS_HD_Unknown_Size;
    int32u SyncCode;

    //Helpers
    bool Synchronize();
    bool Detect_NonDTS();
};

} //NameSpace

#endif
