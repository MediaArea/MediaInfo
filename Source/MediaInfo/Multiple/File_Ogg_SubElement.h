// File_Ogg_SubElement - Info for OGG files
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
// Information about MPEG Transport Stream files, Program Map Section
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Ogg_Setup_VorbisH
#define MediaInfo_Ogg_Setup_VorbisH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Ogg_SubElement
//***************************************************************************

class File_Ogg_SubElement : public File__Analyze
{
public :
    //In
    stream_t StreamKind;
    bool     IsStandAlone; //This stream is the only one in the file
    int64u   absolute_granule_position_Resolution;

protected :
    //Formats
    void Read_Buffer_Init ();

public :
    File_Ogg_SubElement();
    ~File_Ogg_SubElement();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void Data_Parse();

    size_t OldSize;

    //Elements
    void Identification();
    void Identification_vorbis();
    void Identification_theora();
    void Identification_video();
    void Identification_audio();
    void Identification_text();
    void Identification_FLAC1();
    void Comment();
    void Comment_vorbis();
    void Comment_theora();
    void Comment_video();
    void Comment_audio();
    void Comment_text();
    void Setup();
    void Setup_vorbis();
    void FLAC1();
    void OutOfSpecs();

    //Temp
    File__Analyze* Setup_Vorbis;
    File__Analyze* Flac;
    int64u         ID_Identification;
    bool           IsOutOfSpecs_Flac;
};

#ifdef __BORLANDC__ //Borland converts int64u to int32u without error or warning
    #define OGG_ID(NAME, PART1, PART2) \
        const int32u NAME##1=0x##PART1; \
        const int32u NAME##2=0x##PART2; \

#else //__BORLANDC__
    #define OGG_ID(NAME, PART1, PART2) \
        const int64u NAME=0x##PART1##PART2##LL; \

#endif //__BORLANDC__

namespace Ogg
{
    const int32u OggS=0x4F676753;
    const int32u fLaC=0x664C6143;

    OGG_ID(vorbis, 766F, 72626973)
    OGG_ID(theora, 7468, 656F7261)
    OGG_ID(video,  7669, 64656F00)
    OGG_ID(audio,  6175, 64696F00)
    OGG_ID(text,   7465, 78740000)
    OGG_ID(FLAC1,  464C, 41430100)
}

} //NameSpace

#endif
