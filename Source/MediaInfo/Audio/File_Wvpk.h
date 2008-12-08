// File_Wvpk - Info for WavePack files
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
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
// Information about WavePack files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_WvpkH
#define MediaInfo_File_WvpkH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/Tag/File__Tags.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Wvpk
//***************************************************************************

class File_Wvpk : public File__Analyze, public File__Tags_Helper
{
public :
    //In
    size_t Frame_Count_Valid;
    bool   FromMKV;
    bool   FromMKV_CodecPrivateParsed;

protected :
    //Format
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

public :
    File_Wvpk();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void Data_Parse();
    void Data_Parse_Fill();
    void id_0D();
    void id_25();

    //Temp - Global
    size_t Frame_Count;

    //Temp - Technical info
    int32u total_samples_FirstFrame;
    int32u block_index_FirstFrame;
    int32u block_index_LastFrame;
    int32u block_samples_LastFrame;
    bool   resolution0;
    bool   resolution1;
    bool   mono;
    bool   hybrid;
    bool   joint_stereo;
    bool   cross_channel_decorrelation;
    int8u  SamplingRate;
    int8u  num_channels;
    int32u channel_mask;
    int32u Size;
    int16u version;
    Ztring Encoded_Library_Settings;

    //Helpers
    bool Synchronize();
};

} //NameSpace

#endif
