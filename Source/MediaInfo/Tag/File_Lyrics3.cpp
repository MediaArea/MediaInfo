// File_Lyrics3 - Info for Lyrics3 tagged files
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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_LYRICS3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Lyrics3.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3::Read_Buffer_Continue()
{
    if (Buffer_Size<20)
        return;

    Stream_Prepare(Stream_General);

    Header();
    Lyrics();
    Footer();

    Finnished();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3::Header()
{

    //Parsing
    Element_Offset=0;
    Element_Size=11;
    Element_Begin("Header", 11);
    Skip_Local(11,                                              "Signature");
    Element_End();

    //Positionning
    Buffer_Offset+=11;
}

//---------------------------------------------------------------------------
void File_Lyrics3::Footer()
{
    //Parsing
    Element_Offset=0;
    Element_Size=9;
    Element_Begin("Footer", 9);
    Skip_Local(9,                                               "Signature");
    Element_End();

    //Positionning
    Buffer_Offset+=9;
}

//---------------------------------------------------------------------------
void File_Lyrics3::Lyrics()
{
    //Parsing
    Element_Offset=0;
    Element_Size=Buffer_Size-20;
    Element_Begin("Lyrics", Element_Size);
    Skip_XX(Element_Size,                                       "Value");
    Element_End();

    //Filling
    Stream_Prepare(Stream_Text);
    Fill("Name", "Lyrics");
    Fill("Codec", "Lyrics3");

    //Positionning
    Buffer_Offset+=(size_t)Element_Size;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("Album", "R");
            Fill_HowTo("Part/Position", "R");
            Fill_HowTo("Part/Position_Total", "R");
            Fill_HowTo("Track", "R");
            Fill_HowTo("Track/Position", "R");
            Fill_HowTo("Track/Position_Total", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

