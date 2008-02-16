// File__Tags - Info for all kind of framed tags tagged files
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
// Information about all kind of framed tags tagged files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File__TagsH
#define MediaInfo_File__TagsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#if defined(MEDIAINFO_APETAG_YES)
    #include "MediaInfo/Tag/File_ApeTag.h"
#endif //MEDIAINFO_APETAG_YES
#if defined(MEDIAINFO_ID3_YES)
    #include "MediaInfo/Tag/File_Id3.h"
#endif //MEDIAINFO_ID3_YES
#if defined(MEDIAINFO_ID3V2_YES)
    #include "MediaInfo/Tag/File_Id3v2.h"
#endif //MEDIAINFO_ID3V2_YES
#if defined(MEDIAINFO_LYRICS3_YES)
    #include "MediaInfo/Tag/File_Lyrics3.h"
#endif //MEDIAINFO_LYRICS3_YES
#if defined(MEDIAINFO_LYRICS3V2_YES)
    #include "MediaInfo/Tag/File_Lyrics3v2.h"
#endif //MEDIAINFO_LYRICS3V2_YES
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__Tags_Helper
//***************************************************************************

class File__Tags_Helper
{
public :
    //Out
    int64u File_BeginTagSize;   //Begin
    int64u File_EndTagSize;     //End

public :
    File__Tags_Helper();
    ~File__Tags_Helper();

protected :
    //From elsewhere
    bool Read_Buffer_Continue ();       //return true if we can continue, false if want return
    void Read_Buffer_Finalize ();

    //Elements
    bool Header_Begin ();               //return true if we can continue, false if want return
    void Data_GoTo    (int64u GoTo, const char* Message);

    //Handlers
    File__Analyze* Base;

private :
    //Offset
    int64u Id3v1_Offset;
    int64u Lyrics3_Offset;
    int64u Lyrics3v2_Offset;
    int64u ApeTag_Offset;
    int64u JumpTo_WantedByParser;

    //Size
    int64u Id3v1_Size;
    int64u Lyrics3_Size;
    int64u Lyrics3v2_Size;
    int64u ApeTag_Size;
    bool TagSizeIsFinal;
    bool SearchingForEndTags;

    //Handlers
    File__Analyze* Id3;
    File__Analyze* Id3v2;
    File__Analyze* Lyrics3;
    File__Analyze* Lyrics3v2;
    File__Analyze* ApeTag;

    //Helpers
    bool DetectBeginOfEndTags();        //return true if we can continue, false if want return
    bool DetectBeginOfEndTags_Test();

public :
    inline static int32u SynchSafeInt (const int8u* B) {return ((B[0]<<21)
                                                              | (B[1]<<14)
                                                              | (B[2]<< 7)
                                                              | (B[3]<< 0));}
};
} //NameSpace

#endif
