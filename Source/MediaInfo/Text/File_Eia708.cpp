// File_Eia708 - Info for EIA-708 files
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
#if defined(MEDIAINFO_EIA708_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_Eia708.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Eia708::File_Eia708()
:File__Analyze()
{
    //In
    cc_type=0x00;
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Eia708::Read_Buffer_Continue()
{
    if (!IsAccepted)
    {
        if (cc_type!=3)
        {
            Skip_B2(                                                "Waiting for header");
            return;
        }

        Accept("EIA-708");
        Stream_Prepare(Stream_General);
    }
}

//---------------------------------------------------------------------------
void File_Eia708::Read_Buffer_Finalize()
{
    for (size_t Pos=0; Pos<Captions.size(); Pos++)
    {
        if (!Captions[Pos].empty())
        {
            Stream_Prepare(Stream_Text);
            Fill(Stream_Text, StreamPos_Last, Text_Format, "EIA-708");
            Fill(Stream_Text, StreamPos_Last, Text_ID, Pos);
            //Fill(Stream_Text, StreamPos_Last, "xxx", Ztring::Ztring().From_Unicode(Captions[Pos]));
        }
    }
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Eia708::Header_Parse()
{
    //Parsing
    int8u packet_size, sequence_number;
    BS_Begin();
    Get_S1(2, sequence_number,                                  "sequence_number");
    Get_S1(6, packet_size,                                      "packet_size");
    BS_End();

    Header_Fill_Code(0, _T("DTVCC packet"));
    Header_Fill_Size(packet_size*2);
}

//---------------------------------------------------------------------------
void File_Eia708::Data_Parse()
{
    //Parsing
    int8u block_size, service_number;
    BS_Begin();
    Get_S1(3, service_number,                                   "service_number");
    Get_S1(5, block_size,                                       "block_size");
    if (service_number==7)
    {
        Mark_0_NoTrustError();
        Mark_0_NoTrustError();
        Get_S1(6, service_number,                               "extended_service_number");
    }
    BS_End();

    if (service_number>=Captions.size())
        Captions.resize(service_number+1);

    if (block_size)
    {
        Element_Begin("Service Block Packet", block_size);
        for (int8u Pos=0; Pos<block_size; Pos++)
        {
            int8u cc_data_1;
            Get_B1(cc_data_1,                                   "cc_data");
            switch (cc_data_1)
            {
                case 0x08 : if (!Captions[service_number].empty())
                                Captions[service_number].resize(Captions[service_number].size()-1);
                            break;
                case 0x10 : //EXT1
                            {
                                Get_B1 (cc_data_1,              "EXT1");
                                Pos++;
                                if (cc_data_1>=0x08 && cc_data_1<0x10)
                                {
                                    Skip_B1(                    "EXT1 1 byte");
                                    Pos++;
                                }
                                if (cc_data_1>=0x10 && cc_data_1<0x18)
                                {
                                    Skip_B2(                    "EXT1 2 byte");
                                    Pos+=2;
                                }
                                if (cc_data_1>=0x18 && cc_data_1<0x20)
                                {
                                    Skip_B3(                    "EXT1 3 byte");
                                    Pos+=3;
                                }
                                if (cc_data_1>=0x80 && cc_data_1<0x88)
                                {
                                    Skip_B4(                    "EXT1 4 byte");
                                    Pos+=4;
                                }
                                if (cc_data_1>=0x88 && cc_data_1<0x90)
                                {
                                    Skip_B5(                    "EXT1 5 byte");
                                    Pos+=5;
                                }
                            }
                            break;
                case 0x18 : //P16
                            {
                                Skip_B2(                        "P16");
                                Pos+=2;
                            }
                            break;
                case 0x20 : Captions[service_number]+=L' '     ; break;
                case 0x21 : Captions[service_number]+=L'!'     ; break;
                case 0x22 : Captions[service_number]+=L'"'     ; break;
                case 0x23 : Captions[service_number]+=L'#'     ; break;
                case 0x24 : Captions[service_number]+=L'$'     ; break;
                case 0x25 : Captions[service_number]+=L'%'     ; break;
                case 0x26 : Captions[service_number]+=L'&'     ; break;
                case 0x27 : Captions[service_number]+=L'\''     ; break;
                case 0x28 : Captions[service_number]+=L'('     ; break;
                case 0x29 : Captions[service_number]+=L')'     ; break;
                case 0x2A : Captions[service_number]+=L'*'     ; break;
                case 0x2B : Captions[service_number]+=L'+'     ; break;
                case 0x2C : Captions[service_number]+=L','     ; break;
                case 0x2D : Captions[service_number]+=L'-'     ; break;
                case 0x2E : Captions[service_number]+=L'.'     ; break;
                case 0x2F : Captions[service_number]+=L'/'     ; break;
                case 0x30 : Captions[service_number]+=L'0'     ; break;
                case 0x31 : Captions[service_number]+=L'1'     ; break;
                case 0x32 : Captions[service_number]+=L'2'     ; break;
                case 0x33 : Captions[service_number]+=L'3'     ; break;
                case 0x34 : Captions[service_number]+=L'4'     ; break;
                case 0x35 : Captions[service_number]+=L'5'     ; break;
                case 0x36 : Captions[service_number]+=L'6'     ; break;
                case 0x37 : Captions[service_number]+=L'7'     ; break;
                case 0x38 : Captions[service_number]+=L'8'     ; break;
                case 0x39 : Captions[service_number]+=L'9'     ; break;
                case 0x3A : Captions[service_number]+=L':'     ; break;
                case 0x3B : Captions[service_number]+=L';'     ; break;
                case 0x3C : Captions[service_number]+=L'<'     ; break;
                case 0x3E : Captions[service_number]+=L'>'     ; break;
                case 0x3F : Captions[service_number]+=L'?'     ; break;
                case 0x40 : Captions[service_number]+=L'@'     ; break;
                case 0x41 : Captions[service_number]+=L'A'     ; break;
                case 0x42 : Captions[service_number]+=L'B'     ; break;
                case 0x43 : Captions[service_number]+=L'C'     ; break;
                case 0x44 : Captions[service_number]+=L'D'     ; break;
                case 0x45 : Captions[service_number]+=L'E'     ; break;
                case 0x46 : Captions[service_number]+=L'F'     ; break;
                case 0x47 : Captions[service_number]+=L'G'     ; break;
                case 0x48 : Captions[service_number]+=L'H'     ; break;
                case 0x49 : Captions[service_number]+=L'I'     ; break;
                case 0x4A : Captions[service_number]+=L'J'     ; break;
                case 0x4B : Captions[service_number]+=L'K'     ; break;
                case 0x4C : Captions[service_number]+=L'L'     ; break;
                case 0x4D : Captions[service_number]+=L'M'     ; break;
                case 0x4E : Captions[service_number]+=L'N'     ; break;
                case 0x4F : Captions[service_number]+=L'O'     ; break;
                case 0x50 : Captions[service_number]+=L'P'     ; break;
                case 0x51 : Captions[service_number]+=L'Q'     ; break;
                case 0x52 : Captions[service_number]+=L'R'     ; break;
                case 0x53 : Captions[service_number]+=L'S'     ; break;
                case 0x54 : Captions[service_number]+=L'T'     ; break;
                case 0x55 : Captions[service_number]+=L'U'     ; break;
                case 0x56 : Captions[service_number]+=L'V'     ; break;
                case 0x57 : Captions[service_number]+=L'W'     ; break;
                case 0x58 : Captions[service_number]+=L'X'     ; break;
                case 0x59 : Captions[service_number]+=L'Y'     ; break;
                case 0x5A : Captions[service_number]+=L'Z'     ; break;
                case 0x5B : Captions[service_number]+=L'['     ; break;
                case 0x5C : Captions[service_number]+=L'\\'    ; break;
                case 0x5D : Captions[service_number]+=L']'     ; break;
                case 0x5E : Captions[service_number]+=L'^'     ; break;
                case 0x5F : Captions[service_number]+=L'_'     ; break;
                case 0x60 : Captions[service_number]+=L'`'     ; break;
                case 0x61 : Captions[service_number]+=L'a'     ; break;
                case 0x62 : Captions[service_number]+=L'b'     ; break;
                case 0x63 : Captions[service_number]+=L'c'     ; break;
                case 0x64 : Captions[service_number]+=L'd'     ; break;
                case 0x65 : Captions[service_number]+=L'e'     ; break;
                case 0x66 : Captions[service_number]+=L'f'     ; break;
                case 0x67 : Captions[service_number]+=L'g'     ; break;
                case 0x68 : Captions[service_number]+=L'h'     ; break;
                case 0x69 : Captions[service_number]+=L'i'     ; break;
                case 0x6A : Captions[service_number]+=L'j'     ; break;
                case 0x6B : Captions[service_number]+=L'k'     ; break;
                case 0x6C : Captions[service_number]+=L'l'     ; break;
                case 0x6D : Captions[service_number]+=L'm'     ; break;
                case 0x6E : Captions[service_number]+=L'n'     ; break;
                case 0x6F : Captions[service_number]+=L'o'     ; break;
                case 0x70 : Captions[service_number]+=L'p'     ; break;
                case 0x71 : Captions[service_number]+=L'q'     ; break;
                case 0x72 : Captions[service_number]+=L'r'     ; break;
                case 0x73 : Captions[service_number]+=L's'     ; break;
                case 0x74 : Captions[service_number]+=L't'     ; break;
                case 0x75 : Captions[service_number]+=L'u'     ; break;
                case 0x76 : Captions[service_number]+=L'v'     ; break;
                case 0x77 : Captions[service_number]+=L'w'     ; break;
                case 0x78 : Captions[service_number]+=L'x'     ; break;
                case 0x79 : Captions[service_number]+=L'y'     ; break;
                case 0x7A : Captions[service_number]+=L'z'     ; break;
                case 0x7B : Captions[service_number]+=L'{'     ; break;
                case 0x7C : Captions[service_number]+=L'|'     ; break;
                case 0x7D : Captions[service_number]+=L'}'     ; break;
                case 0x7E : Captions[service_number]+=L'~'     ; break;
                case 0x7F : Captions[service_number]+=L'\x266A'; break;
                case 0x88 : //CLW
                            {
                                Skip_B1(                        "CLW");
                                Pos+=1;
                            }
                            break;
                case 0x89 : //DSW
                            {
                                Skip_B1(                        "DSW");
                                Pos+=1;
                            }
                            break;
                case 0x8A : //HDW
                            {
                                Skip_B1(                        "HDW");
                                Pos+=1;
                            }
                            break;
                case 0x8B : //TGW
                            {
                                Skip_B1(                        "TGW");
                                Pos+=1;
                            }
                            break;
                case 0x8C : //DLW
                            {
                                Skip_B1(                        "DLW");
                                Pos+=1;
                            }
                            break;
                case 0x8D : //DLY
                            {
                                Skip_B1(                        "DLY");
                                Pos+=1;
                            }
                            break;
                case 0x90 : //SPA
                            {
                                Skip_B2(                        "SPA");
                                Pos+=2;
                            }
                            break;
                case 0x91 : //SPC
                            {
                                Skip_B3(                        "SPC");
                                Pos+=3;
                            }
                            break;
                case 0x92 : //SPL
                            {
                                Skip_B2(                        "SPL");
                                Pos+=2;
                            }
                            break;
                case 0x97 : //SWA
                            {
                                Skip_B4(                        "SWA");
                                Pos+=4;
                            }
                            break;
                case 0x98 : //
                            {
                                Skip_B6(                        "DF0");
                                Pos+=6;
                            }
                            break;
                case 0x99 : //
                            {
                                Skip_B6(                        "DF1");
                                Pos+=6;
                            }
                            break;
                case 0x9A : //
                            {
                                Skip_B6(                        "DF2");
                                Pos+=6;
                            }
                            break;
                case 0x9B : //
                            {
                                Skip_B6(                        "DF3");
                                Pos+=6;
                            }
                            break;
                case 0x9C : //
                            {
                                Skip_B6(                        "DF4");
                                Pos+=6;
                            }
                            break;
                case 0x9D : //
                            {
                                Skip_B6(                        "DF5");
                                Pos+=6;
                            }
                            break;
                case 0x9E : //
                            {
                                Skip_B6(                        "DF6");
                                Pos+=6;
                            }
                            break;
                case 0x9F : //
                            {
                                Skip_B6(                        "DF7");
                                Pos+=6;
                            }
                            break;
                default   : ;
            }
        }

        Element_End();
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_PGS_YES
