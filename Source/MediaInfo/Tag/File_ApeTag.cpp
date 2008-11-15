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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_APETAG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_ApeTag.h"
#include <algorithm>
#include <ctime>
using namespace std;
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
    if (Key.find('\0')!=std::string::npos)
        Key.resize(Key.find('\0'));
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
    transform(Key.begin(), Key.end(), Key.begin(), (int(*)(int))toupper); //(int(*)(int)) is a patch for unix
         if (Key=="ALBUM")          Fill(Stream_General, 0, General_Album, Value);
    else if (Key=="ARTIST")         Fill(Stream_General, 0, General_Performer, Value);
    else if (Key=="AUTHOR")         Fill(Stream_General, 0, General_WrittenBy, Value);
    else if (Key=="BAND")           Fill(Stream_General, 0, General_Performer, Value);
    else if (Key=="COMMENT")        Fill(Stream_General, 0, General_Comment, Value);
    else if (Key=="COMMENTS")       Fill(Stream_General, 0, General_Comment, Value);
    else if (Key=="COMPOSER")       Fill(Stream_General, 0, General_Composer, Value);
    else if (Key=="CONTENTGROUP")   Fill(Stream_General, 0, General_Genre, Value);
    else if (Key=="COPYRIGHT")      Fill(Stream_General, 0, General_Copyright, Value);
    else if (Key=="DISK")
    {
                                    if (Value.find(_T("/"))!=Error)
                                    {
                                        Fill(Stream_General, 0, General_Part_Position_Total, Value.SubString(_T("/"), _T("")));
                                        Fill(Stream_General, 0, General_Part_Position, Value.SubString(_T(""), _T("/")));
                                    }
                                    else
                                        Fill(Stream_General, 0, General_Track_Position, Value);
    }
    else if (Key=="ENCODEDBY")      Fill(Stream_General, 0, General_EncodedBy, Value);
    else if (Key=="GENRE")          Fill(Stream_General, 0, General_Genre, Value);
    else if (Key=="ORIGARTIST")     Fill(Stream_General, 0, General_Original_Performer, Value);
    else if (Key=="TITLE")          Fill(Stream_General, 0, General_Title, Value);
    else if (Key=="TRACK")
    {
                                    if (Value.find(_T("/"))!=Error)
                                    {
                                        Fill(Stream_General, 0, General_Track_Position_Total, Value.SubString(_T("/"), _T("")));
                                        Fill(Stream_General, 0, General_Track_Position, Value.SubString(_T(""), _T("/")));
                                    }
                                    else
                                        Fill(Stream_General, 0, General_Track_Position, Value);
    }
    else if (Key=="UNSYNCEDLYRICS") Fill(Stream_General, 0, General_Lyrics, Value);
    else if (Key=="WWW")            Fill(Stream_General, 0, General_Title_Url, Value);
    else if (Key=="YEAR")           Fill(Stream_General, 0, General_Recorded_Date, Value);
    else if (Key=="CONTENT GROUP DESCRIPTION") Fill(Stream_General, 0, General_Title, Value);
    else if (Key=="ORIGINAL ALBUM/MOVIE/SHOW TITLE") Fill(Stream_General, 0, General_Original_Album, Value);
    else if (Key=="ORIGINAL ARTIST(S)/PERFORMER(S)") Fill(Stream_General, 0, General_Original_Performer, Value);
    else                            Fill(Stream_General, 0, Key.c_str(), Value);
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

