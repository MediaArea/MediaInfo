// File_DvDif - Info for DVD objects (IFO) files
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
//
// Information about DV-DIF (Digital Video Digital Interface Format)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_DvDifH
#define MediaInfo_File_DvDifH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_DvDif
//***************************************************************************

class File_DvDif : public File__Analyze
{
public :
    //In
    size_t Frame_Count_Valid;
    int8u  AuxToAnalyze; //Only Aux must be parsed

protected :
    //Format
    void Read_Buffer_Finalize ();

public :
    File_DvDif();
    
private :
    //Buffer
    void Header_Parse();
    void Data_Parse();

    //Elements - Main
    void Header();
    void Subcode();
    void Subcode_Ssyb(int8u syb_num);
    void Aux();
    void Audio();
    void Video();

    //Elements - Sub
    void Element();
    void timecode();
    void audio_source();
    void audio_control();
    void audio_recdate();
    void audio_rectime();
    void video_source();
    void video_control();
    void video_recdate();
    void video_rectime();

    //Helpers
    Ztring recdate();
    Ztring rectime();

    //Temp
    Ztring Recorded_Date_Date;
    Ztring Recorded_Date_Time;
    size_t FrameCount;
    size_t FrameSize_Theory; //The size of a frame
    int64u Duration;
    int8u  DIFBlockNumber;
    int8u  Subcode_First;
    bool   dsf;
    bool   dsf_IsValid;
    int8u  apt;
    bool   tf1;
    bool   tf2;
    bool   tf3;
};

} //NameSpace

#endif
