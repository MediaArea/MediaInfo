// File__Duplicate_MpegTs - Duplication of some formats
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
// Duplication helper for some specific formats
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "MediaInfo/Duplicate/File__Duplicate_MpegTs.h"
#include "MediaInfo/MediaInfo_Config.h"
#include <ZenLib/ZtringList.h>
#include <ZenLib/File.h>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
// CRC_32_Table
// A CRC is computed like this:
// Init: int32u CRC_32 = 0xFFFFFFFF;
// for each data byte do
//     CRC_32=(CRC_32<<8) ^ CRC_32_Table[(CRC_32>>24)^(data_byte)];
static int32u CRC_32_Table[256] =
{
  0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9,
  0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
  0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
  0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
  0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
  0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
  0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
  0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
  0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
  0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
  0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,
  0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
  0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49,
  0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
  0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
  0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
  0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE,
  0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
  0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
  0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
  0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
  0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
  0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066,
  0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
  0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
  0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
  0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
  0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
  0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
  0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
  0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686,
  0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
  0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
  0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
  0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
  0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
  0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47,
  0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
  0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
  0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
  0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7,
  0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
  0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F,
  0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
  0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
  0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
  0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F,
  0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
  0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
  0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
  0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
  0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
  0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30,
  0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
  0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
  0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
  0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
  0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
  0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
  0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
  0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0,
  0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
  0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
  0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

File__Duplicate_MpegTs::File__Duplicate_MpegTs (const Ztring &Target)
: File__Duplicate__Base()
{
    Writer.Configure(Target);

    //Temp
    PAT_version_number=0xFF;
    PAT_version_number_ToBuffer=0xFF;
}

//***************************************************************************
// Set
//***************************************************************************

//---------------------------------------------------------------------------
void File__Duplicate_MpegTs::Configure (const Ztring &Value)
{
    //Form: "program_number"
    if (Value.find(_T("program_number="))==0)
    {
        Ztring program_number=Value.substr(15, string::npos);
        Wanted_program_numbers.insert(program_number.To_int16u());
    }
    //Form: "program_number"
    else if (Value.find(_T("program_map_PID="))==0)
    {
        Ztring program_map_PID=Value.substr(16, string::npos);
        Wanted_program_map_PIDs.insert(program_map_PID.To_int16u());
    }
    //Form: "elementary_PID"
    else if (Value.find(_T("elementary_PID="))==0)
    {
        Ztring elementary_PID=Value.substr(15, string::npos);
        Wanted_elementary_PIDs.insert(elementary_PID.To_int16u());
    }
    //Old
    else
        Wanted_program_numbers.insert(Value.To_int16u());
}

//***************************************************************************
// Write
//***************************************************************************

void File__Duplicate_MpegTs::Write (const int8u* ToAdd, size_t ToAdd_Size)
{
    int16u PID=CC2(ToAdd+1)&0x1FFF;
    if (PID==0x0000)
        Manage_PAT(ToAdd, ToAdd_Size);
    else if (program_map_PIDs.find(PID)!=program_map_PIDs.end())
        Manage_PMT(ToAdd, ToAdd_Size);
    else if (elementary_PIDs.find(PID)!=elementary_PIDs.end())
        Writer.Write(ToAdd, ToAdd_Size);
}

void File__Duplicate_MpegTs::Manage_PAT (const int8u* ToAdd, size_t ToAdd_Size)
{
    if (!Parsing_Begin(ToAdd, ToAdd_Size))
        return;

    //version_number
    int8u PAT_version_number_FromBuffer=(CC1(Buffer+Buffer_Pos+2)>>1)&0x1F;
    if (PAT_version_number==0xFF)
        PAT_version_number=PAT_version_number_FromBuffer;
    if (PAT_version_number_ToBuffer==0xFF)
        PAT_version_number_ToBuffer=PAT_version_number_FromBuffer;
    if (PAT_version_number_FromBuffer!=PAT_version_number)
    {
        PAT_version_number_ToBuffer++;
        if (PAT_version_number_ToBuffer>32)
            PAT_version_number_ToBuffer=0;
    }
    int8u ToReplace=Buffer[Buffer_Pos+2];
    ToReplace&=0xC1; //11000001, for removing old version_number
    ToReplace|=PAT_version_number_ToBuffer<<1; //merging, with 1 bit offset
    Buffer[Buffer_Pos+2]=ToReplace;

    //Positionning at first program
    Buffer_Pos+=5;

    //Programs
    while (Buffer_Pos<Buffer_End)
    {
        //For each program
        int16u program_number=CC2(Buffer+Buffer_Pos);
        int16u program_map_PID=CC2(Buffer+Buffer_Pos+2)&0x1FFF;
        if (Wanted_program_numbers.find(program_number)  !=Wanted_program_numbers.end()
         || Wanted_program_map_PIDs.find(program_map_PID)!=Wanted_program_map_PIDs.end())
        {
            //Integrating it
            program_map_PIDs.insert(program_map_PID);
            Buffer_Pos+=4;
        }
        else
        {
            //Removing it
            std::memmove(Buffer+Buffer_Pos, Buffer+Buffer_Pos+4, Buffer_End-(Buffer_Pos+4));
            Buffer_End-=4;
        }
    }

    Parsing_End();
}

void File__Duplicate_MpegTs::Manage_PMT (const int8u* ToAdd, size_t ToAdd_Size)
{
    if (!Parsing_Begin(ToAdd, ToAdd_Size))
        return;

    //version_number
    int16u program_number=CC2(Buffer+Buffer_Pos);
    int8u PMT_version_number_FromBuffer=(CC1(Buffer+Buffer_Pos+2)>>1)&0x1F;
    if (PMT_version_numbers.find(program_number)==PMT_version_numbers.end())
        PMT_version_numbers[program_number]=PMT_version_number_FromBuffer;
    if (PMT_version_numbers_ToBuffer.find(program_number)==PMT_version_numbers_ToBuffer.end())
        PMT_version_numbers_ToBuffer[program_number]=PMT_version_number_FromBuffer;
    if (PMT_version_numbers_ToBuffer[program_number]!=PMT_version_numbers[program_number])
    {
        PMT_version_numbers_ToBuffer[program_number]++;
        if (PMT_version_numbers_ToBuffer[program_number]>32)
            PMT_version_numbers_ToBuffer[program_number]=0;
    }

    //Positionning at program_info_length
    Buffer_Pos+=7;

    //program_info_length
    int16u program_info_length=CC2(Buffer+Buffer_Pos)&0x0FFF;
    Buffer_Pos+=2+program_info_length;

    //elementary_PIDs
    while (Buffer_Pos<Buffer_End)
    {
        //For each elementary_PID
        int16u elementary_PID=CC2(Buffer+Buffer_Pos+1)&0x1FFF;
        int16u ES_info_length=CC2(Buffer+Buffer_Pos+3)&0x0FFF;
        if (Wanted_elementary_PIDs.empty() || Wanted_elementary_PIDs.find(elementary_PID)!=Wanted_elementary_PIDs.end())
        {
            //Integrating it
            elementary_PIDs.insert(elementary_PID);
            Buffer_Pos+=5+ES_info_length;
        }
        else
        {
            //Removing it
            std::memmove(Buffer+Buffer_Pos, Buffer+Buffer_Pos+5+ES_info_length, Buffer_End-(Buffer_Pos+5+ES_info_length));
            Buffer_End-=5+ES_info_length;
        }
    }

    Parsing_End();
}

bool File__Duplicate_MpegTs::Parsing_Begin (const int8u* ToAdd, size_t ToAdd_Size)
{
    Buffer=new int8u[ToAdd_Size];
    std::memcpy(Buffer, ToAdd, ToAdd_Size);
    Buffer_Size=ToAdd_Size;
    Buffer_Pos=0;

    //adaptation_field_length
    adaptation_field_length=0;
    if (CC1(Buffer+3)&0x20) //adaptation_field_control (adaptation) == true
        adaptation_field_length=1+CC1(Buffer+4);

    //pointer_field
    Buffer_Pos+=4+adaptation_field_length;
    pointer_field=CC1(Buffer+Buffer_Pos);

    //section_length
    Buffer_Pos+=1+pointer_field+1;
    section_length=CC2(Buffer+Buffer_Pos)&0x0FFF;
    Buffer_End=4+adaptation_field_length+section_length;
    if (Buffer_End>ToAdd_Size-4)
    {
        delete[] Buffer; Buffer=NULL;
        return false;
    }

    //Verifying CRC
    int32u CRC_32=0xffffffff;
    const int8u* CRC_32_Buffer=Buffer+Buffer_Pos-1; //table_id position
    while(CRC_32_Buffer<Buffer+Buffer_End+4) //from table_id to the end, CRC_32 included
    {
        CRC_32=(CRC_32<<8) ^ CRC_32_Table[(CRC_32>>24)^(*CRC_32_Buffer)];
        CRC_32_Buffer++;
    }
    if (CRC_32)
    {
        delete[] Buffer; Buffer=NULL;
        return false; //Problem
    }

    //Positionning just after section_length
    Buffer_Pos+=2;
    return true;
}

void File__Duplicate_MpegTs::Parsing_End ()
{
    //section_length
    int8u ToReplace=CC1(Buffer+6)&0xF0; //before section_length
    section_length=Buffer_End-0x04;
    ToReplace|=section_length>>8;
    Buffer[0x06+0]=ToReplace;
    Buffer[0x06+1]=(int8u)(section_length&0xFF);

    //CRC32
    int32u CRC_32=0xFFFFFFFF;
    for (size_t Buffer_CRC_Pos=4+1+pointer_field; Buffer_CRC_Pos<Buffer_End; Buffer_CRC_Pos++)
        CRC_32=(CRC_32<<8) ^ CRC_32_Table[(CRC_32>>24)^(Buffer[Buffer_CRC_Pos])];

    Buffer[Buffer_Pos+0]=(CRC_32>>24)&0xFF;
    Buffer[Buffer_Pos+1]=(CRC_32>>16)&0xFF;
    Buffer[Buffer_Pos+2]=(CRC_32>> 8)&0xFF;
    Buffer[Buffer_Pos+3]= CRC_32     &0xFF;

    //Padding
    for (size_t Buffer_CRC_Pos=Buffer_Pos+4; Buffer_CRC_Pos<Buffer_Size; Buffer_CRC_Pos++)
        Buffer[Buffer_CRC_Pos]=0xFF;

    Writer.Write(Buffer, Buffer_Size);
    delete[] Buffer; Buffer=NULL;
}

//***************************************************************************
// Output_Buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Duplicate_MpegTs::Output_Buffer_Get (const Ztring &Code_, unsigned char** Output_Buffer)
{
    return Writer.Output_Buffer_Get(Output_Buffer);
}

} //NameSpace

