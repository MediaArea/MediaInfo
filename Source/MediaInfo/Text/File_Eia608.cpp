// File_Eia608 - Info for EIA-608 files
// Copyright (C) 2009-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
using namespace std;
//---------------------------------------------------------------------------


//***************************************************************************
// Constants
//***************************************************************************

/* screen size -- specified by the standard */
const int8u Eia608_Rows=15;
const int8u Eia608_Columns=32;

//---------------------------------------------------------------------------
// CAE-608-E section F.1.1.5
static const int8u Eia608_PAC_Row[]=
{
    10,
    0,  //or 1
    2,  //or 3
    11, //or 12
    13, //or 14
    4,  //or 5
    6,  //or 7
    8   //or 9
};

//***************************************************************************
//
//***************************************************************************

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Eia608::File_Eia608()
:File__Analyze()
{
    //Configuration
    PTS_DTS_Needed=true;

    //In
    cc_type=(int8u)-1;

    //Temp
    TextMode=false;
    DataChannelMode=false;
    InBack=false;
    HasChanged=false;
    Characters_Front.resize(Eia608_Rows);
    for (size_t Pos=0; Pos<Characters_Front.size(); Pos++)
        Characters_Front[Pos].resize(Eia608_Columns);
    Characters_Back.resize(Eia608_Rows);
    for (size_t Pos=0; Pos<Characters_Back.size(); Pos++)
        Characters_Back[Pos].resize(Eia608_Columns);
    x=0;
    y=0;
    Attribute_Current=0;
    RollUpLines=1;
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Eia608::Streams_Fill()
{
    Stream_Prepare(Stream_Text);
    Fill(Stream_Text, 0, Text_Format, "EIA-608");
    Fill(Stream_Text, 0, Text_StreamSize, 0);
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Eia608::Read_Buffer_Continue()
{
    if (!Status[IsAccepted])
        Accept("EIA-608");

    size_t Captions_Size=Captions.size();

    int8u cc_data_1, cc_data_2;
    Get_B1 (cc_data_1,                                          "cc_data");
    Get_B1 (cc_data_2,                                          "cc_data");

    //Removing checksume
    cc_data_1&=0x7F;
    cc_data_2&=0x7F;

    if ((cc_data_1 && cc_data_1<0x10) || !XDS_Data.empty()) //XDS
    {
        XDS_Data.push_back(cc_data_1);
        XDS_Data.push_back(cc_data_2);
        if (cc_data_1==0x0F)
            XDS();
        if (XDS_Data.size()>64)
            XDS_Data.clear(); //This is a security
    }
    else if (cc_data_1>=0x20) //Basic characters
    {
        Characters_Eia608(cc_data_1);
        if ((cc_data_2&0x7F)>=0x20)
            Characters_Eia608(cc_data_2);
    }
    else //Special
        Special(cc_data_1, cc_data_2);

    if (Captions_Size<Captions.size())
        Param_Info(Captions.substr(Captions_Size, std::string::npos));
}

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
void File_Eia608::XDS()
{
    if (XDS_Data.size()<4)
    {
        XDS_Data.clear();
        return; //There is a problem
    }

    switch (XDS_Data[0])
    {
        case 0x01 : XDS_Current(); break;
        case 0x09 : XDS_PublicService(); break;
        default   : ;
    }

    XDS_Data.clear();
}

//---------------------------------------------------------------------------
void File_Eia608::XDS_Current()
{
    switch (XDS_Data[1])
    {
        case 0x05 : XDS_Current_ContentAdvisory(); break;
        case 0x08 : XDS_Current_CopyAndRedistributionControlPacket(); break;
        default   : ;
    }
}

//---------------------------------------------------------------------------
void File_Eia608::XDS_Current_ContentAdvisory()
{
    if (XDS_Data.size()!=6)
    {
        return; //There is a problem
    }
}

//---------------------------------------------------------------------------
void File_Eia608::XDS_Current_CopyAndRedistributionControlPacket()
{
    if (XDS_Data.size()!=6)
    {
        return; //There is a problem
    }
}

//---------------------------------------------------------------------------
void File_Eia608::XDS_PublicService()
{
    switch (XDS_Data[1])
    {
        case 0x01 : XDS_PublicService_NationalWeatherService(); break;
        default   : ;
    }
}

//---------------------------------------------------------------------------
void File_Eia608::XDS_PublicService_NationalWeatherService()
{
    if (XDS_Data.size()!=20)
    {
        return; //There is a problem
    }
}

//---------------------------------------------------------------------------
void File_Eia608::Special(int8u cc_data_1, int8u cc_data_2)
{
    //Data channel check
    DataChannelMode=(cc_data_1&0x08)!=0; //bit3 is the Data Channel number
    cc_data_1&=~0x08;

    if (cc_data_1>=0x10 && cc_data_1<=0x17 && cc_data_2>=0x40)
    {
        PAC(cc_data_1, cc_data_2);
    }
    else if (cc_data_1==0x12 && cc_data_2>=0x20 && cc_data_2<=0x3F)
    {
        //Optional replacement chars
        //if (x)
        //    x--;
        //Add here 6.4.2, table 5-6-7
    }
    else if (cc_data_1==0x13 && cc_data_2>=0x20 && cc_data_2<=0x3F)
    {
        //Optional replacement chars
        //if (x)
        //    x--;
        //Add here 6.4.2, table 8-9-10
    }
    else if (cc_data_1==0x11 && cc_data_2>=0x20 && cc_data_2<=0x3F)
    {
        //
        if ((cc_data_2&0xFE)==0x2E) //Italic
            Attribute_Current|=Attribute_Italic;
        else //Other attributes
            Attribute_Current=(cc_data_2&0x0F)>>1;

        //Underline
        if (cc_data_2&0x01)
            Attribute_Current|=Attribute_Underline;
    }
    else
    {
        switch (cc_data_1)
        {
            case 0x11 : Special_11(cc_data_2); break;
            case 0x14 : Special_14(cc_data_2); break;
            case 0x17 : Special_17(cc_data_2); break;
            default   : ;
        }
    }
}

//---------------------------------------------------------------------------
void File_Eia608::PAC(int8u cc_data_1, int8u cc_data_2)
{
    //Horizontal position
    y=Eia608_PAC_Row[cc_data_1&0x0F]+((cc_data_2&0x20)?0:1);
    if (y>=Eia608_Rows)
    {
        y=Eia608_Rows-1;
    }

    //Attributes (except Underline)
    if (cc_data_2&0x10) //0x5x and 0x7x
    {
        x=(cc_data_2&0x0E)<<1;
        Attribute_Current=Attribute_Color_White;
    }
    else if ((cc_data_2&0x0E)==0x0E) //0x4E, 0x4F, 0x6E, 0x6F
    {
        Attribute_Current=Attribute_Color_White|Attribute_Italic;
    }
    else //0x40-0x4D, 0x60-0x6D
        Attribute_Current=(cc_data_2&0x0E)>>1;

    //Underline
    if (cc_data_2&0x01)
        Attribute_Current|=Attribute_Underline;
}

//---------------------------------------------------------------------------
void File_Eia608::Special_11(int8u cc_data_2)
{
    switch (cc_data_2)
    {
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
}

//---------------------------------------------------------------------------
void File_Eia608::Special_14(int8u cc_data_2)
{
    switch (cc_data_2)
    {
        case 0x20 : //RCL - Resume Caption Loading
        case 0x25 : //RU2 - Roll-Up Captions–2 Rows
        case 0x26 : //RU3 - Roll-Up Captions–3 Rows
        case 0x27 : //RU4 - Roll-Up Captions–4 Rows
        case 0x29 : //RDC - Resume Direct Captioning
                    TextMode=false; break; //This is CC
        case 0x2A : //TR  - Text Restart
        case 0x2B : //RTD - Resume Text Display
                    TextMode=false; break; //This is Text
        default: ;
    }

    switch (cc_data_2)
    {
        case 0x20 : InBack=true;
                    break; //RCL - Resume Caption Loading
        case 0x21 : if (x)
                        x--;
                    (InBack?Characters_Back:Characters_Front)[y][x].Value=L' '; //Clear the character
                    (InBack?Characters_Back:Characters_Front)[y][x].Attribute=L' '; //Clear the character
                    if (!InBack)
                        HasChanged=true; //TODO: set the right data mode
                    break; //BS  - Backspace
        case 0x22 : break; //AOF - Alarm Off
        case 0x23 : break; //AON - Alarm On
        case 0x24 : for (size_t Pos=x; Pos<Eia608_Columns; Pos++)
                    {
                        (InBack?Characters_Back:Characters_Front)[y][Pos].Value=L' '; //Clear up to the end of line
                        (InBack?Characters_Back:Characters_Front)[y][Pos].Attribute=0; //Clear up to the end of line
                    }
                    if (!InBack)
                        HasChanged=true; //TODO: set the right data mode
                    break; //DER - Delete to End of Row
        case 0x25 : //RU2 - Roll-Up Captions–2 Rows
        case 0x26 : //RU3 - Roll-Up Captions–3 Rows
        case 0x27 : //RU4 - Roll-Up Captions–4 Rows
                    RollUpLines=cc_data_2-0x25+2;
                    InBack=false;
                    if (RollUpLines>y)
                        y=Eia608_Rows-1; //TODO: Why?
                    break;
        case 0x28 : break; //FON - Flash On
        case 0x29 : InBack=false;
                    break; //RDC - Resume Direct Captioning
        case 0x2A : InBack=false;
                    RollUpLines=Eia608_Rows; //TODO: Why?
                    y=Eia608_Rows-1; //TODO: Why?
                    Attribute_Current=0; //TODO: Why?
                    break; //TR  - Text Restart
        case 0x2B : break; //RTD - Resume Text Display
        case 0x2C :
                    for (size_t Pos_Y=0; Pos_Y<Eia608_Rows; Pos_Y++)
                        for (size_t Pos_X=0; Pos_X<Eia608_Columns; Pos_X++)
                        {
                            (InBack?Characters_Back:Characters_Front)[Pos_Y][Pos_X].Value=L' ';
                            (InBack?Characters_Back:Characters_Front)[Pos_Y][Pos_X].Attribute=0;
                        }
                    if (!InBack)
                        HasChanged=true; //TODO: set the right data mode
                    break; //EDM - Erase Displayed Memory
        case 0x2D : for (size_t Pos=0; Pos<(RollUpLines-1); Pos++)
                    {
                        if (y>4-Pos)
                            Characters_Front[y-4+Pos]=Characters_Front[y-4+Pos];
                    }
                    for (size_t Pos_X=0; Pos_X<Eia608_Columns; Pos_X++)
                    {
                        Characters_Front[y][Pos_X].Value=L' ';
                        Characters_Front[y][Pos_X].Attribute=0;
                    }
                    if (!InBack)
                        HasChanged=true; //TODO: set the right data mode
                    break; //CR  - Carriage Return
        case 0x2E : for (size_t Pos_Y=0; Pos_Y<Eia608_Rows; Pos_Y++)
                        for (size_t Pos_X=0; Pos_X<Eia608_Columns; Pos_X++)
                        {
                            Characters_Back[Pos_Y][Pos_X].Value=L' ';
                            Characters_Back[Pos_Y][Pos_X].Attribute=0;
                        }
                    break; //ENM - Erase Non-Displayed Memory
        case 0x2F : Characters_Front.swap(Characters_Back);
                    HasChanged=true; //TODO: set the right data mode
                    break; //EOC - End of Caption
        default   : ;
    }
}

//---------------------------------------------------------------------------
void File_Eia608::Special_17(int8u cc_data_2)
{
    switch (cc_data_2)
    {
        case 0x20 : //TO1 - Tab Offset 1 Column
        case 0x21 : //TO2 - Tab Offset 2 Columns
        case 0x22 : //TO3 - Tab Offset 3 Columns
                    y+=cc_data_2&0x03;
                    if (y>=Eia608_Rows)
                    {
                        y=Eia608_Rows-1;
                    }
                    break;
        default   : ;
    }
}

//---------------------------------------------------------------------------
void File_Eia608::Characters_Eia608(int8u Character)
{
    if (x+1==Eia608_Columns)
    {
        x--; //There is a problem

        //TODO: Put it at the end, for the conversion
        //TODO: Handle special chars
    }

    switch (Character)
    {
        case 0x20 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L' '     ; break;
        case 0x21 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'!'     ; break;
        case 0x22 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'"'     ; break;
        case 0x23 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'#'     ; break;
        case 0x24 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'$'     ; break;
        case 0x25 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'%'     ; break;
        case 0x26 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'&'     ; break;
        case 0x27 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\''    ; break;
        case 0x28 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'('     ; break;
        case 0x29 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L')'     ; break;
        case 0x2A : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xE1'  ; break; //a acute
        case 0x2B : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'+'     ; break;
        case 0x2C : (InBack?Characters_Back:Characters_Front)[y][x].Value=L','     ; break;
        case 0x2D : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'-'     ; break;
        case 0x2E : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'.'     ; break;
        case 0x2F : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'/'     ; break;
        case 0x30 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'0'     ; break;
        case 0x31 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'1'     ; break;
        case 0x32 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'2'     ; break;
        case 0x33 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'3'     ; break;
        case 0x34 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'4'     ; break;
        case 0x35 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'5'     ; break;
        case 0x36 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'6'     ; break;
        case 0x37 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'7'     ; break;
        case 0x38 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'8'     ; break;
        case 0x39 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'9'     ; break;
        case 0x3A : (InBack?Characters_Back:Characters_Front)[y][x].Value=L':'     ; break;
        case 0x3B : (InBack?Characters_Back:Characters_Front)[y][x].Value=L';'     ; break;
        case 0x3C : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'<'     ; break;
        case 0x3E : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'>'     ; break;
        case 0x3F : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'?'     ; break;
        case 0x40 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'@'     ; break;
        case 0x41 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'A'     ; break;
        case 0x42 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'B'     ; break;
        case 0x43 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'C'     ; break;
        case 0x44 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'D'     ; break;
        case 0x45 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'E'     ; break;
        case 0x46 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'F'     ; break;
        case 0x47 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'G'     ; break;
        case 0x48 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'H'     ; break;
        case 0x49 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'I'     ; break;
        case 0x4A : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'J'     ; break;
        case 0x4B : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'K'     ; break;
        case 0x4C : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'L'     ; break;
        case 0x4D : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'M'     ; break;
        case 0x4E : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'N'     ; break;
        case 0x4F : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'O'     ; break;
        case 0x50 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'P'     ; break;
        case 0x51 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'Q'     ; break;
        case 0x52 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'R'     ; break;
        case 0x53 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'S'     ; break;
        case 0x54 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'T'     ; break;
        case 0x55 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'U'     ; break;
        case 0x56 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'V'     ; break;
        case 0x57 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'W'     ; break;
        case 0x58 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'X'     ; break;
        case 0x59 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'Y'     ; break;
        case 0x5A : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'Z'     ; break;
        case 0x5B : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'['     ; break;
        case 0x5C : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xE9'  ; break; //e acute
        case 0x5D : (InBack?Characters_Back:Characters_Front)[y][x].Value=L']'     ; break;
        case 0x5E : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xED'  ; break; //i acute
        case 0x5F : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xF3'  ; break; //o acute
        case 0x60 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xFA'  ; break; //u acute
        case 0x61 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'a'     ; break;
        case 0x62 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'b'     ; break;
        case 0x63 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'c'     ; break;
        case 0x64 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'd'     ; break;
        case 0x65 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'e'     ; break;
        case 0x66 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'f'     ; break;
        case 0x67 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'g'     ; break;
        case 0x68 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'h'     ; break;
        case 0x69 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'i'     ; break;
        case 0x6A : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'j'     ; break;
        case 0x6B : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'k'     ; break;
        case 0x6C : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'l'     ; break;
        case 0x6D : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'm'     ; break;
        case 0x6E : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'n'     ; break;
        case 0x6F : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'o'     ; break;
        case 0x70 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'p'     ; break;
        case 0x71 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'q'     ; break;
        case 0x72 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'r'     ; break;
        case 0x73 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L's'     ; break;
        case 0x74 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L't'     ; break;
        case 0x75 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'u'     ; break;
        case 0x76 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'v'     ; break;
        case 0x77 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'w'     ; break;
        case 0x78 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'x'     ; break;
        case 0x79 : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'y'     ; break;
        case 0x7A : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'z'     ; break;
        case 0x7B : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xE7'  ; break; //c with cedilla
        case 0x7C : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xF7'  ; break; //division symbol
        case 0x7D : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xD1'  ; break; //N tilde
        case 0x7E : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\xF1'  ; break; //n tilde
        case 0x7F : (InBack?Characters_Back:Characters_Front)[y][x].Value=L'\x25A0'; break; //Solid block
        default   : ;
    }

    x++;
    HasChanged=true;
    
    if (!Status[IsFilled])
    {
        Fill("EIA-608");
        if (MediaInfoLib::Config.ParseSpeed_Get()<1)
            Finish("EIA-608");
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_EIA608_YES
