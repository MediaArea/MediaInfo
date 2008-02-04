// File_Vc1 - Info for MPEG Video files
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
//
// Information about MPEG Video files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Vc1H
#define MediaInfo_Vc1H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Vc1
//***************************************************************************

class File_Vc1 : public File__Analyze
{
public :
    //In
    size_t Frame_Count_Valid;
    bool   FrameIsAlwaysComplete;

protected :
    //Format
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Vc1();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    bool Header_Parse_Fill_Size();
    void Data_Parse();

    //Elements
    void EndOfSequence();
    void Slice();
    void Field();
    void FrameHeader();
    void FrameHeader_Fill();
    void EntryPointHeader();
    void SequenceHeader();
    void UserDefinedSlice();
    void UserDefinedField();
    void UserDefinedFrameHeader();
    void UserDefinedEntryPointHeader();
    void UserDefinedSequenceHeader();
    size_t Frame_Count;

    //From SequenceHeader
    int16u coded_width;
    int16u coded_height;
    int16u framerateexp;
    int8u  frameratecode_enr;
    int8u  frameratecode_dr;
    int8u  profile;
    int8u  level;
    int8u  chromaformat;
    int8u  AspectRatio;
    int8u  AspectRatioX;
    int8u  AspectRatioY;
    bool   interlace;
    bool   tfcntrflag;
    bool   framerate_present;
    bool   framerate_form;

    //Stream
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
    std::map<int8u, stream> Stream;

    //Helpers
    bool Synchronize();
    bool Header_Parser_QuickSearch();
    bool Detect_NonVC1();



    //Temp
    size_t Width;
    size_t Height;
    size_t RatioValue;
    float32 FrameRate;
    size_t BitRate;

    //Temp - Sequence Header
    //bool interlace;
    //bool tfcntrflag;

    //Error controls
    std::vector<int8u> Frame_ShouldBe;


};

} //NameSpace

#endif
