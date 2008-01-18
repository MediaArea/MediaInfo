// File_ApeTag - Info for ApeTag tagged files
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
#if defined(MEDIAINFO_APETAG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_ApeTag.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
bool File_ApeTag::Header_Begin()
{
    if (Buffer_Size<0x20)
        return false; //At least 32 bytes for footer, and we need 32 bytes for header

    return true;
}

//---------------------------------------------------------------------------
void File_ApeTag::Header_Parse()
{
    //Testing if begin or end of tags
    if (CC8(Buffer+Buffer_Offset)==CC8("APETAGEX"))
    {
        //Filling
        Header_Fill_Code(0xFFFFFFFF, Buffer_Offset+0x20>=Buffer_Size?"Footer":"Header");
        Header_Fill_Size(0x20);
        return;
    }

    //Parsing
    Ztring Value;
    int32u Flags, Length;
    Get_L4 (Length,                                         "Length");
    Get_L4 (Flags,                                          "Flags");
        Skip_Flags(Flags,  0,                               "Read Only");
        Skip_Flags(Flags,  1,                               "Binary");
        Skip_Flags(Flags,  2,                               "Locator of external stored information");
        Skip_Flags(Flags, 29,                               "Is the header");
        Skip_Flags(Flags, 30,                               "Contains a footer");
        Skip_Flags(Flags, 31,                               "Contains a header");
    Peek_String(Element_Size-Element_Offset, Key);
    Skip_String(Key.size(),                                 "Key");
    Skip_L1(                                                "0x00");

    //Filling
    Header_Fill_Code(0, Key.c_str());
    Header_Fill_Size(Element_Offset+Length);
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_ApeTag::Header()
{
    //Parsing
    int32u Flags;
    Skip_C8(                                                    "Preamble");
    Skip_L4(                                                    "Version");
    Skip_L4(                                                    "Size");
    Skip_L4(                                                    "Count");
    Get_L4 (Flags,                                              "Flags");
        Skip_Flags(Flags,  0,                                   "Read Only");
        Skip_Flags(Flags,  1,                                   "Binary");
        Skip_Flags(Flags,  2,                                   "Locator of external stored information");
        Skip_Flags(Flags, 29,                                   "Is the header");
        Skip_Flags(Flags, 30,                                   "Contains a footer");
        Skip_Flags(Flags, 31,                                   "Contains a header");
    Skip_L8(                                                    "Reserved");

    //Filling
    FILLING_BEGIN();
        if (Count_Get(Stream_General)==0)
        {
            Stream_Prepare(Stream_General);
            Stream_Prepare(Stream_Audio);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_ApeTag::Data_Parse()
{
    //If footer
    if (Element_Code==0xFFFFFFFF)
    {
        Header();
        return;
    }

    //Parsing
    Ztring Value;
    Get_UTF8(Element_Size, Value,                               "Value"); Element_Info(Value);

    //Filling
         if (Key=="ALBUM")          Fill(Stream_General, 0, "Album", Value);
    else if (Key=="ARTIST")         Fill(Stream_General, 0, "Performer", Value);
    else if (Key=="AUTHOR")         Fill(Stream_General, 0, "Performer", Value);
    else if (Key=="COMMENTS")       Fill(Stream_General, 0, "Comment", Value);
    else if (Key=="COMPOSER")       Fill(Stream_General, 0, "WrittentBy", Value);
    else if (Key=="YEAR")           Fill(Stream_General, 0, "Recorded_Date", Value);
    else if (Key=="TRACK")          Fill(Stream_General, 0, "Track/Position", Value);
    else if (Key=="TITLE")          Fill(Stream_General, 0, "Title", Value);
    else                            Fill(Stream_General, 0, Key.c_str(), Value);
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_ApeTag::HowTo(stream_t StreamKind)
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

