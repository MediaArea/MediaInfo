// File_Lyrics3v2 - Info for Lyrics3v2 tagged files
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
#if defined(MEDIAINFO_LYRICS3V2_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Lyrics3v2.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3v2::Read_Buffer_Continue()
{
    if (Buffer_Size<26)
        return;

    Stream_Prepare(Stream_General);

    Header();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3v2::Header_Parse()
{
    if (Buffer_Offset+15>=Buffer_Size) //first 10 is minimum size of a tag, Second 10 is ID3v2 header size
    {
        //Place for footer
        Header_Fill_Code(0xFFFFFFFF, "Footer");
        Header_Fill_Size(Buffer_Size-Buffer_Offset);
        return;
    }

    //Parsing
    Ztring Size;
    int32u Field;
    Get_C3 (Field,                                           "Field");
    Get_Local(5, Size,                                       "Size");

    //Filling
    Header_Fill_Code(Field, Ztring().From_CC3(Field));
    Header_Fill_Size(8+Size.To_int64u());
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::Data_Parse()
{
    #define ELEMENT_CASE(_NAME) \
        case Lyrics3v2::_NAME : _NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(AUT);
        ELEMENT_CASE(CRC);
        ELEMENT_CASE(EAL);
        ELEMENT_CASE(EAR);
        ELEMENT_CASE(ETT);
        ELEMENT_CASE(IMG);
        ELEMENT_CASE(IND);
        ELEMENT_CASE(INF);
        ELEMENT_CASE(LYR);
        case 0xFFFFFFFF : Footer(); break;
        default : Skip_XX(Element_Size,                         "Data");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3v2::Header()
{
    //Parsing
    Element_Offset=0;
    Element_Size=11;
    Element_Begin("Header", 11);
    Skip_Local(11,                                              "Signature");
    Element_End();

    //Positioning
    Buffer_Offset+=11;
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::Footer()
{
    //Parsing
    Element_Info("Footer");
    Skip_Local(6,                                               "Size");
    Skip_Local(9,                                               "Signature");
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::AUT()
{
    Element_Info("Lyrics Author Name");

    //Parsing
    Skip_Local(Element_Size,                                    "Value");
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::CRC()
{
    Element_Info("CRC");

    //Parsing
    Skip_Local(Element_Size,                                    "Value");
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::EAL()
{
    Element_Info("Extended Album name");

    //Parsing
    Ztring Value;
    Get_Local(Element_Size, Value,                              "Value");

    //Filling
    Fill(Stream_General, 0, General_Album, Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::EAR()
{
    Element_Info("Extended Artist name");

    //Parsing
    Ztring Value;
    Get_Local(Element_Size, Value,                              "Value");

    //Filling
    Fill(Stream_General, 0, General_Performer, Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::ETT()
{
    Element_Info("Extended Track Title");

    //Parsing
    Ztring Value;
    Get_Local(Element_Size, Value,                              "Value");

    //Filling
    Fill(Stream_General, 0, General_Title, Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::IMG()
{
    Element_Info("Image location");

    //Parsing
    Skip_Local(Element_Size,                                    "Value");
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::IND()
{
    Element_Info("Indications field");

    //Parsing
    if (Element_Size>=1)
        Skip_Local(1,                                           "lyrics present");
    if (Element_Size>=2)
        Skip_Local(1,                                           "timestamp in lyrics");
    if (Element_Size>=3)
        Skip_Local(1,                                           "inhibits tracks for random selection");
    //Can be more
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::INF()
{
    Element_Info("Additional information");

    //Parsing
    Ztring Value;
    Get_Local(Element_Size, Value,                              "Value");

    //Filling
    Fill(Stream_General, 0, General_Comment, Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::LYR()
{
    Element_Info("Lyrics");

    //Parsing
    Skip_XX(Element_Size,                                       "Value");

    //Filling
    Stream_Prepare(Stream_Text);
    Fill(Stream_Text, 0, Text_Codec, "Lyrics3v2");
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

