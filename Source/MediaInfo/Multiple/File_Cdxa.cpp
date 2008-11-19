// File_Cdxa - Info for CDXA files
// Copyright (C) 2004-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_CDXA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Cdxa.h"
#include "ZenLib/Utils.h"
#include "MediaInfo/MediaInfo_Internal.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Format
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// CDXA = RIFF header + Raw sectors
// Riff header size = 44
// Raw sector size = 2352
//
// Raw sector :
// Sync             12 bytes (00 FF .. FF 00)
// Header           4 bytes
// SubHeader        8 bytes
// Datas            2324 bytes
// EDC (CRC)        4 bytes
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Cdxa::File_Cdxa()
:File__Analyze()
{
    //Basic configuration
    Synched=false;
    File_MaximumOffset=64*1024;
    Frame_Count_Valid=16;

    //Buffer
    Element_Size=0;

    //Temp - Global
    Frame_Count=0;

    //Pointers
    MI=NULL;
}

//---------------------------------------------------------------------------
File_Cdxa::~File_Cdxa()
{
    delete MI; //MI=NULL;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Cdxa::Read_Buffer_Finalize ()
{
    //If nothing
    if (MI->Info==NULL|| MI->Count_Get(Stream_General)==0)
    {
        Fill(Stream_General, 0, General_Format, "CDXA");
    }
    else
    {

        //General
        Open_Buffer_Finalize(MI->Info);
        Merge(*(MI->Info));
        Merge(*(MI->Info), Stream_General, 0, 0);
        const Ztring &Format=Retrieve(Stream_General, 0, General_Format);
        Fill(Stream_General, 0, General_Format, (Ztring(_T("CDXA/"))+Format).c_str(), Unlimited, true);
        Fill(Stream_General, 0, General_Duration, "", Unlimited, true);
        Fill(Stream_Video, 0, Video_Duration, "", Unlimited, true);
    }

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
    {
        delete MI; MI=NULL;
    }
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
// RIFF Header, 44 bytes
// RIFF header                      4 bytes, Pos=0
// RIFF data size                   4 bytes, Pos=4
// Format (CDXA)                    4 bytes, Pos=8
// Format Header                    4 bytes, Pos=12
// Format Size                      4 bytes, Pos=16
// Format user_id                   2 bytes, Pos=20
// Format group_id                  2 bytes, Pos=22
// Format attributes                2 bytes, Pos=24
// Format xa_signature              2 bytes, Pos=26 ("XA")
// Format xa_track_number           4 bytes, Pos=28
// Format Reserved                  4 bytes, Pos=32
// Data Header                      4 bytes, Pos=36
// Data Size                        4 bytes, Pos=40
//
// Attributes (big endian):
// 15 Directory
// 14 CDDA
// 13 Interleaved
// 12 Mode2Form2 --> 2324 bytes/block
// 11 Mode2Form1 --> 2048 bytes/block
// 10 Exec_Other
// 09 Reserved
// 08 Read_Other
// 07 Reserved
// 06 Exec_Group
// 05 Reserved
// 04 Read_Group
// 03 Reserved
// 02 Exec_User
// 01 Reserved
// 00 Read_User
//
void File_Cdxa::FileHeader_Parse()
{
    if (Element_Size<44)
    {
        Element_WaitForMoreData();
        return;
    }

    //Parsing
    if (                CC4(Buffer+Buffer_Offset+0x00)!=CC4("RIFF")
     || LittleEndian2int32u(Buffer+Buffer_Offset+0x04)!=LittleEndian2int32u(Buffer+Buffer_Offset+0x28)+0x24 //Sizes of chunks
     ||                 CC4(Buffer+Buffer_Offset+0x08)!=CC4("CDXA")
     ||                 CC4(Buffer+Buffer_Offset+0x0C)!=CC4("fmt ")
     || LittleEndian2int32u(Buffer+Buffer_Offset+0x10)!=0x10
     ||                 CC4(Buffer+Buffer_Offset+0x24)!=CC4("data")
       )
    {
        Finnished();
        return;
    }

    //Parsing
    Element_Begin("CDXA header");
    Skip_C4(                                                    "RIFF header");
    Skip_L4(                                                    "RIFF data size");
    Skip_C4(                                                    "CDXA");
    Skip_C4(                                                    "fmt header");
    Skip_L4(                                                    "fmt size");
    Skip_L2(                                                    "user_id");
    Skip_L2(                                                    "group_id");
    Skip_L2(                                                    "attributes");
    Skip_C2(                                                    "xa_signature");
    Skip_L4(                                                    "xa_track_number");
    Skip_L4(                                                    "reserved");
    Skip_C4(                                                    "data header");
    Skip_L4(                                                    "data size");
    Element_End();

    FILLING_BEGIN();
        if (MI==NULL)
        {
            MI=new MediaInfo_Internal;
            MI->Option(_T("File_IsSub"), _T("1"));
        }
        
        Stream_Prepare(Stream_General);
    FILLING_END();
}

//---------------------------------------------------------------------------
bool File_Cdxa::Header_Begin()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+12>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && !(CC8(Buffer+Buffer_Offset)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset+8)==0xFFFFFF00))
    {
        Trusted_IsNot("CDXA, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Cdxa::Header_Parse()
{
    //Parsing
    Skip_B4(                                                    "Sync1");
    Skip_B4(                                                    "Sync2");
    Skip_B4(                                                    "Sync3");
    Skip_B4(                                                    "Header");
    Skip_B8(                                                    "SubHeader");

    //Filling
    Header_Fill_Size(2352);
    Header_Fill_Code(0, "Chunk");
}

//---------------------------------------------------------------------------
void File_Cdxa::Data_Parse()
{
    if (MI==NULL)
    {
        //Where is the header? --> Problem
        Finnished();
        return;
    }

    //CRC or not?
    int64u CRC_Size=4;
    if (Element_Size!=2328)
        CRC_Size=0;

    //Parsing
    Skip_XX(Element_Size-CRC_Size,                              "Data");
    if (CRC_Size>0)
        Skip_B4(                                                "CRC");

    //Preparing to fill MediaInfo with a buffer
    MI->Open_Buffer_Init(File_Size, File_Offset+Buffer_Offset);

    //Sending the buffer to MediaInfo
    if (MI->Open_Buffer_Continue(Buffer+Buffer_Offset, (size_t)(Element_Size-CRC_Size))==0)
        Finnished(); //Get out of the loop, there was an error during the parsing

    //Testing if MediaInfo always need data
    File_GoTo=MI->Open_Buffer_Continue_GoTo_Get();
    if (File_GoTo!=(int64u)-1)
        Info("CDXA, Jumping to end of file");

    //Details
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (MediaInfoLib::Config.Details_Get())
    {
        if (!MI->Inform().empty())
            Element_Show_Add(MI->Inform());
    }
    #endif //MEDIAINFO_MINIMIZESIZE

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)(Element_Size-CRC_Size), _T("xxx"));
}

//***************************************************************************
// Elements
//***************************************************************************

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Cdxa::Synchronize()
{
    //Synchronizing
    while (           Buffer_Offset+2352*3+12<=Buffer_Size
      && !(CC8(Buffer+Buffer_Offset+2352*0)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset+2352*0+8)==0xFFFFFF00
        && CC8(Buffer+Buffer_Offset+2352*1)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset+2352*1+8)==0xFFFFFF00
        && CC8(Buffer+Buffer_Offset+2352*2)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset+2352*2+8)==0xFFFFFF00
        && CC8(Buffer+Buffer_Offset+2352*3)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset+2352*3+8)==0xFFFFFF00))
        Buffer_Offset++;
    if (Buffer_Offset+2352*3+12>Buffer_Size)
    {
        return false;
    }

    //Synched is OK
    Synched=true;
    return true;
}

    /*
//---------------------------------------------------------------------------
void File_Cdxa::Detect_EOF()
{
    //Jump to the end of the file
    if (File_Size!=(int64u)-1 && File_Size>File_Offset+Buffer_Size+1*1024*1024 && (
       (File_Offset>1*1024*1024)
    || (MI!=NULL && MI->Open_Buffer_Continue_GoTo_Get()!=(int64u)-1)
    ))
    {
        Info("CDXA, Jumping to end of file");
        File_GoTo=File_Size-1*1024*1024;
    }
}
    */

} //NameSpace

#endif //MEDIAINFO_CDXA_YES

