// File_Eia608 - Info for EIA-608 files
// Copyright (C) 2009-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_EIA608_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_Eia608.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Eia608::Read_Buffer_Continue()
{
    while (Element_Offset<Element_Size)
    {
        int8u cc_data_1;
        Get_B1 (cc_data_1,                                      "cc_data");

        if ((cc_data_1&0x7F)>0x00 && (cc_data_1&0x7F)<0x20)
        {
            if (Element_Offset>=Element_Size)
            {
                Element_Offset--; //Canceling, waiting for more data
                return;
            }
            else
            {
                int8u cc_data_2;
                Get_B1 (cc_data_2,                              "cc_data");
                switch (cc_data_1&0x7F)
                {
                    case 0x00 : break;
                    case 0x11 : //1st channel
                    //case 0x12 : //1st channel
                    //case 0x13 : //1st channel
                    //case 0x14 : //1st channel
                    //case 0x15 : //1st channel
                    //case 0x16 : //1st channel
                    //case 0x17 : //1st channel
                    case 0x19 : //2nd channel
                    //case 0x1A : //2nd channel
                    //case 0x1B : //2nd channel
                    //case 0x1C : //2nd channel
                    //case 0x1D : //2nd channel
                    //case 0x1E : //2nd channel
                    //case 0x1F : //2nd channel
                                switch (cc_data_2&0x7F)
                                {
                                    case 0x00 : break;
                                    case 0x20 : break; //White
                                    case 0x21 : break; //White Underline
                                    case 0x22 : break; //Green
                                    case 0x23 : break; //Green Underline
                                    case 0x24 : break; //Blue
                                    case 0x25 : break; //Blue Underline
                                    case 0x26 : break; //Cyan
                                    case 0x27 : break; //Cyan Underline
                                    case 0x28 : break; //Red
                                    case 0x29 : break; //Red Underline
                                    case 0x2A : break; //Yellow
                                    case 0x2B : break; //Yellow Underline
                                    case 0x2C : break; //Magenta
                                    case 0x2D : break; //Magenta Underline
                                    case 0x2E : break; //Italics
                                    case 0x2F : break; //Italics Underline
                                    case 0x30 : Captions+=L'\x2122'; break;  //Registered mark symbol
                                    case 0x31 : Captions+=L'\xB0'  ; break;  //Degree sign
                                    case 0x32 : Captions+=L'\xBD'  ; break;  //1/2
                                    case 0x33 : Captions+=L'\xBF'  ; break;  //interogation mark inverted
                                    case 0x34 : Captions+=L'\xA9'  ; break;  //Trademark symbol
                                    case 0x35 : Captions+=L'\xA2'  ; break;  //Cents sign
                                    case 0x36 : Captions+=L'\xA3'  ; break;  //Pounds Sterling sign
                                    case 0x37 : Captions+=L'\x266A'; break;  //Music note
                                    case 0x38 : Captions+=L'\xE0'  ; break;  //a grave
                                    case 0x39 : Captions+=L' '     ; break;  //Transparent space
                                    case 0x3A : Captions+=L'\xE8'  ; break;  //e grave
                                    case 0x3B : Captions+=L'\xE2'  ; break;  //a circumflex
                                    case 0x3C : Captions+=L'\xEA'  ; break;  //e circumflex
                                    case 0x3D : Captions+=L'\xEE'  ; break;  //i circumflex
                                    case 0x3E : Captions+=L'\xF4'  ; break;  //o circumflex
                                    case 0x3F : Captions+=L'\xFB'  ; break;  //u circumflex
                                    default   : ;
                                }
                                break;
                    case 0x14 : //1st channel
                    case 0x1C : //2nd channel
                                switch (cc_data_2&0x7F)
                                {
                                    case 0x20 : break; //Resume caption loading
                                    case 0x21 : if (!Captions.empty())
                                                    Captions.resize(Captions.size()-1); //Should manage the case if this is the first column
                                                break; //Backspace
                                    case 0x22 : break; //Alarm Off
                                    case 0x23 : break; //Alarm On
                                    case 0x24 : break; //Delete to End of Row
                                    case 0x25 : break; //Roll-Up Captions–2 Rows
                                    case 0x26 : break; //Roll-Up Captions–3 Rows
                                    case 0x27 : break; //Roll-Up Captions–4 Rows
                                    case 0x28 : break; //Flash On
                                    case 0x29 : break; //Resume Direct Captioning
                                    case 0x2A : break; //Text Restart
                                    case 0x2B : break; //Resume Text Display.
                                    case 0x2C : break; //Erase Displayed Memory
                                    case 0x2D : Captions+=_T(" / "); break; //Carriage Return
                                    case 0x2E : break; //Erase Non-Displayed Memory
                                    case 0x2F : break; //End of Caption
                                    default   : ;
                                }
                                break;
                    case 0x17 : //1st channel
                    case 0x1F : //2nd channel
                                switch (cc_data_2&0x7F)
                                {
                                    case 0x20 : break; //Tab Offset 1 Column
                                    case 0x21 : break; //Tab Offset 2 Columns
                                    case 0x22 : break; //Tab Offset 3 Columns
                                    default   : ;
                                }
                                break;
                    default    : ;
                }
            }
        }
        else switch (cc_data_1&0x7F)
        {
            case 0x20 : Captions+=L' '     ; break;
            case 0x21 : Captions+=L'!'     ; break;
            case 0x22 : Captions+=L'"'     ; break;
            case 0x23 : Captions+=L'#'     ; break;
            case 0x24 : Captions+=L'$'     ; break;
            case 0x25 : Captions+=L'%'     ; break;
            case 0x26 : Captions+=L'&'     ; break;
            case 0x27 : Captions+=L'\''    ; break;
            case 0x28 : Captions+=L'('     ; break;
            case 0x29 : Captions+=L')'     ; break;
            case 0x2A : Captions+=L'\xE1'  ; break; //a acute
            case 0x2B : Captions+=L'+'     ; break;
            case 0x2C : Captions+=L','     ; break;
            case 0x2D : Captions+=L'-'     ; break;
            case 0x2E : Captions+=L'.'     ; break;
            case 0x2F : Captions+=L'/'     ; break;
            case 0x30 : Captions+=L'0'     ; break;
            case 0x31 : Captions+=L'1'     ; break;
            case 0x32 : Captions+=L'2'     ; break;
            case 0x33 : Captions+=L'3'     ; break;
            case 0x34 : Captions+=L'4'     ; break;
            case 0x35 : Captions+=L'5'     ; break;
            case 0x36 : Captions+=L'6'     ; break;
            case 0x37 : Captions+=L'7'     ; break;
            case 0x38 : Captions+=L'8'     ; break;
            case 0x39 : Captions+=L'9'     ; break;
            case 0x3A : Captions+=L':'     ; break;
            case 0x3B : Captions+=L';'     ; break;
            case 0x3C : Captions+=L'<'     ; break;
            case 0x3E : Captions+=L'>'     ; break;
            case 0x3F : Captions+=L'?'     ; break;
            case 0x40 : Captions+=L'@'     ; break;
            case 0x41 : Captions+=L'A'     ; break;
            case 0x42 : Captions+=L'B'     ; break;
            case 0x43 : Captions+=L'C'     ; break;
            case 0x44 : Captions+=L'D'     ; break;
            case 0x45 : Captions+=L'E'     ; break;
            case 0x46 : Captions+=L'F'     ; break;
            case 0x47 : Captions+=L'G'     ; break;
            case 0x48 : Captions+=L'H'     ; break;
            case 0x49 : Captions+=L'I'     ; break;
            case 0x4A : Captions+=L'J'     ; break;
            case 0x4B : Captions+=L'K'     ; break;
            case 0x4C : Captions+=L'L'     ; break;
            case 0x4D : Captions+=L'M'     ; break;
            case 0x4E : Captions+=L'N'     ; break;
            case 0x4F : Captions+=L'O'     ; break;
            case 0x50 : Captions+=L'P'     ; break;
            case 0x51 : Captions+=L'Q'     ; break;
            case 0x52 : Captions+=L'R'     ; break;
            case 0x53 : Captions+=L'S'     ; break;
            case 0x54 : Captions+=L'T'     ; break;
            case 0x55 : Captions+=L'U'     ; break;
            case 0x56 : Captions+=L'V'     ; break;
            case 0x57 : Captions+=L'W'     ; break;
            case 0x58 : Captions+=L'X'     ; break;
            case 0x59 : Captions+=L'Y'     ; break;
            case 0x5A : Captions+=L'Z'     ; break;
            case 0x5B : Captions+=L'['     ; break;
            case 0x5C : Captions+=L'\xE9'  ; break; //e acute
            case 0x5D : Captions+=L']'     ; break;
            case 0x5E : Captions+=L'\xED'  ; break; //i acute
            case 0x5F : Captions+=L'\xF3'  ; break; //o acute
            case 0x60 : Captions+=L'\xFA'  ; break; //u acute
            case 0x61 : Captions+=L'a'     ; break;
            case 0x62 : Captions+=L'b'     ; break;
            case 0x63 : Captions+=L'c'     ; break;
            case 0x64 : Captions+=L'd'     ; break;
            case 0x65 : Captions+=L'e'     ; break;
            case 0x66 : Captions+=L'f'     ; break;
            case 0x67 : Captions+=L'g'     ; break;
            case 0x68 : Captions+=L'h'     ; break;
            case 0x69 : Captions+=L'i'     ; break;
            case 0x6A : Captions+=L'j'     ; break;
            case 0x6B : Captions+=L'k'     ; break;
            case 0x6C : Captions+=L'l'     ; break;
            case 0x6D : Captions+=L'm'     ; break;
            case 0x6E : Captions+=L'n'     ; break;
            case 0x6F : Captions+=L'o'     ; break;
            case 0x70 : Captions+=L'p'     ; break;
            case 0x71 : Captions+=L'q'     ; break;
            case 0x72 : Captions+=L'r'     ; break;
            case 0x73 : Captions+=L's'     ; break;
            case 0x74 : Captions+=L't'     ; break;
            case 0x75 : Captions+=L'u'     ; break;
            case 0x76 : Captions+=L'v'     ; break;
            case 0x77 : Captions+=L'w'     ; break;
            case 0x78 : Captions+=L'x'     ; break;
            case 0x79 : Captions+=L'y'     ; break;
            case 0x7A : Captions+=L'z'     ; break;
            case 0x7B : Captions+=L'\xE7'  ; break; //c with cedilla
            case 0x7C : Captions+=L'\xF7'  ; break; //division symbol
            case 0x7D : Captions+=L'\xD1'  ; break; //N tilde
            case 0x7E : Captions+=L'\xF1'  ; break; //n tilde
            case 0x7F : Captions+=L'\x25A0'; break; //Solid block
            default   : ;
        }
    }

    if (!IsAccepted && !Captions.empty())
    {
        Accept("EIA-608");
        Stream_Prepare(Stream_General);
        Stream_Prepare(Stream_Text);
        Fill(Stream_Text, 0, "Format", "EIA-608");
    }
}

//---------------------------------------------------------------------------
void File_Eia608::Read_Buffer_Finalize()
{
    //Fill(Stream_Text, 0, "xxx", Ztring::Ztring().From_Unicode(Captions));
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_PGS_YES
