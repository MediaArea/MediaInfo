// File_Id3 - Info for ID3 tagged files
// Copyright (C) 2005-2008 Jerome Martinez, Zen@MediaArea.net
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
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_ID3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Id3.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
void Mpega_ID3v1_KeepOutSpaces(Ztring &Tag)
{
    //Removing trailing spaces
    while (!Tag.empty() && Tag[Tag.size()-1]==_T(' '))
        Tag.resize(Tag.size()-1);
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Id3::FileHeader_Parse()
{
    //Parsing
    int32u ID;
    int8u Track=0, Genre;
    Get_C3    (    ID,                                          "ID");
    if (ID!=CC3("TAG"))
    {
        Element_WaitForMoreData();
        Finished();
        return;
    }
    Get_Local (30, Id3v1_Title,                                 "Title");
    Get_Local (30, Id3v1_Artist,                                "Artist");
    Get_Local (30, Id3v1_Album,                                 "Album");
    Get_Local ( 4, Id3v1_Year,                                  "Year");
    Get_Local (30, Id3v1_Comment,                               "Comment");
    if (Id3v1_Comment.size()<29) //Id3v1.1 specifications : Track number addition
    {
        Element_Offset-=2;
        int8u Zero;
        Peek_B1(Zero);
        if (Zero==0)
        {
            Skip_B1(                                            "Zero");
            Get_B1 (Track,                                      "Track");
        }
        else
            Element_Offset+=2;
    }
    Get_B1 (Genre,                                              "Genre");

    FILLING_BEGIN();
        Mpega_ID3v1_KeepOutSpaces(Id3v1_Title);
        Mpega_ID3v1_KeepOutSpaces(Id3v1_Artist);
        Mpega_ID3v1_KeepOutSpaces(Id3v1_Album);
        Mpega_ID3v1_KeepOutSpaces(Id3v1_Year);
        Mpega_ID3v1_KeepOutSpaces(Id3v1_Comment);
        if (Track!=0)
            Id3v1_Track.From_Number(Track);
        else
            Id3v1_Track.clear();
        if (Genre!=0 && Genre!=0xFF)
            Id3v1_Genre.From_Number(Genre);
        else
            Id3v1_Genre.clear();

        Stream_Prepare(Stream_General);
        Stream_Prepare(Stream_Audio);

        if (!Id3v1_Album.empty())
            Fill(Stream_General, 0, General_Album, Id3v1_Album);
        if (!Id3v1_Title.empty())
            Fill(Stream_General, 0, General_Track, Id3v1_Title);
        if (!Id3v1_Artist.empty())
            Fill(Stream_General, 0, General_Performer, Id3v1_Artist);
        if (!Id3v1_Comment.empty())
            Fill(Stream_General, 0, General_Comment, Id3v1_Comment);
        if (!Id3v1_Year.empty())
            Fill(Stream_General, 0, General_Recorded_Date, Id3v1_Year);
        if (!Id3v1_Genre.empty())
            Fill(Stream_General, 0, General_Genre, Id3v1_Genre);
        if (!Id3v1_Track.empty())
            Fill(Stream_General, 0, General_Track_Position, Id3v1_Track);
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

