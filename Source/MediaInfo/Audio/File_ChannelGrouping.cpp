// File_ChannelGrouping - Regrouping PCM streams
// Copyright (C) 2011-2011 MediaArea.net SARL, Info@MediaArea.net
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
#if defined(MEDIAINFO_AES3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_ChannelGrouping.h"
#if defined(MEDIAINFO_AES3_YES)
    #include "MediaInfo/Audio/File_Aes3.h"
#endif
#if MEDIAINFO_EVENTS
    #include "MediaInfo/MediaInfo_Events.h"
#endif //MEDIAINFO_EVENTS
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

File_ChannelGrouping::File_ChannelGrouping()
{
    //Configuration
    #if MEDIAINFO_EVENTS
        ParserIDs[0]=MediaInfo_Parser_ChannelGrouping;
    #endif //MEDIAINFO_EVENTS
    #if MEDIAINFO_DEMUX
        Demux_Level=2; //Container
    #endif //MEDIAINFO_DEMUX
    #if MEDIAINFO_TRACE
        Trace_Layers_Update(0); //Container1
    #endif //MEDIAINFO_TRACE

    //In
    ByteDepth=0;
    Common=NULL;
    Channel_Pos=0;
    Channel_Total=1;
}

File_ChannelGrouping::~File_ChannelGrouping()
{
    if (Channel_Pos==0)
        delete Common; //Common=NULL;
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_ChannelGrouping::Streams_Fill()
{
    Fill(Stream_General, 0, General_Format, "ChannelGrouping");

    if (Common->Channel_Master!=Channel_Pos)
        return;
    Fill(Common->Parser);
    Merge(*Common->Parser);
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Streams_Finish()
{
    if (Common->Channel_Master!=Channel_Pos)
        return;
    Finish(Common->Parser);
    //Merge(*Common->Parser);
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_ChannelGrouping::Read_Buffer_Init()
{
    if (Common==NULL)
    {
        Common=new common;
        Common->Parser=new File_Aes3;
        ((File_Aes3*)Common->Parser)->ByteSize=ByteDepth*2;
        Common->Channels.resize(Channel_Total);
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
            Common->Channels[Pos]=new common::channel;
        Open_Buffer_Init(Common->Parser);
    }
    if (Channel_Pos)
        StreamIDs[StreamIDs_Size-2]=StreamID;
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Read_Buffer_Continue()
{
    if (Buffer_Size<ByteDepth)
        return; //Waiting for enough data

    //Copying to Channel buffer
    if (Common->Channels[Channel_Pos]->Buffer_Size+Buffer_Size>Common->Channels[Channel_Pos]->Buffer_Size_Max)
        Common->Channels[Channel_Pos]->resize(Common->Channels[Channel_Pos]->Buffer_Size+Buffer_Size);
    memcpy(Common->Channels[Channel_Pos]->Buffer+Common->Channels[Channel_Pos]->Buffer_Size, Buffer, Buffer_Size);
    Common->Channels[Channel_Pos]->Buffer_Size+=Buffer_Size;
    Buffer_Offset=Buffer_Size;
    Common->Channel_Current++;
    if (Common->Channel_Current>=Channel_Total)
        Common->Channel_Current=0;

    //Copying to merged channel
    size_t Minimum=(size_t)-1;
    for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
        if (Minimum>Common->Channels[Pos]->Buffer_Size-Common->Channels[Pos]->Buffer_Offset)
            Minimum=Common->Channels[Pos]->Buffer_Size-Common->Channels[Pos]->Buffer_Offset;
    while (Minimum>=ByteDepth)
    {
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
        {
            if (Common->MergedChannel.Buffer_Size+Minimum>Common->MergedChannel.Buffer_Size_Max)
                Common->MergedChannel.resize(Common->MergedChannel.Buffer_Size+Minimum);
            memcpy(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Size, Common->Channels[Pos]->Buffer+Common->Channels[Pos]->Buffer_Offset, ByteDepth);
            Common->Channels[Pos]->Buffer_Offset+=ByteDepth;
            Common->MergedChannel.Buffer_Size+=ByteDepth;
        }
        Minimum-=ByteDepth;
    }

    if (Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset==0)
        return;

    //Buffer handling
    Ztring Format;
    /*
    if (StreamIDs_Size)
        Format=Config->ID_Format_Get(Ztring::ToZtring(StreamIDs[0]));
    if (Config->Demux_Unpacketize_Get() && (Format==_T("AES3") || Format==_T("Dolby E")))
    {
        while (Common->MergedChannel.Buffer_Offset+16<=Common->MergedChannel.Buffer_Size)
        {
            size_t Buffer_Offset_Temp=Common->MergedChannel.Buffer_Offset;
            if (Synchronize_AES3())
            {
                if (Buffer_Offset_Temp!=Common->MergedChannel.Buffer_Offset)
                    Open_Buffer_Continue(Common->Parser, Common->MergedChannel.Buffer+Buffer_Offset_Temp, Common->MergedChannel.Buffer_Offset-Buffer_Offset_Temp);

                size_t Buffer_Offset_Begin=Common->MergedChannel.Buffer_Offset;
                Common->MergedChannel.Buffer_Offset+=ByteDepth*2;
                if (Synchronize_AES3())
                {
                    size_t Buffer_Offset_End=Common->MergedChannel.Buffer_Offset;
                    Common->MergedChannel.Buffer_Offset=Buffer_Offset_Temp;

                    Demux(Common->MergedChannel.Buffer+Buffer_Offset_Begin, Buffer_Offset_End-Buffer_Offset_Begin, ContentType_MainStream);

                    Open_Buffer_Continue(Common->Parser, Common->MergedChannel.Buffer+Buffer_Offset_Begin, Buffer_Offset_End-Buffer_Offset_Begin);
                    Common->MergedChannel.Buffer_Offset=Buffer_Offset_End;
                }
                else
                {
                    Common->MergedChannel.Buffer_Offset=Buffer_Offset_Temp;
                    break;
                }
            }
            else
            {
                Common->MergedChannel.Buffer_Offset=Buffer_Offset_Temp;
                break;
            }
        }
    }
    else
    */
    {
        Demux(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset, ContentType_MainStream);

        Open_Buffer_Continue(Common->Parser, Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset);
        Common->MergedChannel.Buffer_Offset=Common->MergedChannel.Buffer_Size;
    }

    if (!Status[IsAccepted] && Common->Parser->Status[IsAccepted])
    {
        if (Common->Channel_Master==(size_t)-1)
            Common->Channel_Master=Channel_Pos;
        Accept();
    }
    if (!Status[IsAccepted] && Buffer_TotalBytes>=0x8000)
        Reject();
    if (Common->Parser->Status[IsFinished])
        Finish();

    //Optimize buffer
    for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
        Common->Channels[Pos]->optimize();
    Common->MergedChannel.optimize();
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_ChannelGrouping::Synchronize_AES3()
{
    //Synchronizing
    while (Common->MergedChannel.Buffer_Offset+16<=Common->MergedChannel.Buffer_Size)
    {
        if (CC4(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0xF8724E1F) //SMPTE 337M 16-bit, BE
        {
            break; //while()
        }
        if (CC4(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x72F81F4E) //SMPTE 337M 16-bit, LE
        {
            break; //while()
        }
        if (CC5(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x6F87254E1FLL) //SMPTE 337M 20-bit, BE
        {
            break; //while()
        }
        if (CC6(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x96F872A54E1FLL) //SMPTE 337M 24-bit, BE
        {
            break; //while()
        }
        if (CC6(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x72F8961F4E5ALL) //SMPTE 337M 24-bit, LE
        {
            break; //while()
        }
        if (CC6(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x00F872004E1FLL) //16-bit in 24-bit, BE
        {
            break; //while()
        }
        if (CC6(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x0072F8001F4ELL) //16-bit in 24-bit, LE
        {
            break; //while()
        }
        if (CC6(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x6F872054E1F0LL) //20-bit in 24-bit, BE
        {
            break; //while()
        }
        if (CC6(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x20876FF0E154LL) //20-bit in 24-bit, LE
        {
            break; //while()
        }
        if (CC8(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x0000F87200004E1FLL) //16-bit in 32-bit, BE
        {
            break; //while()
        }
        if (CC8(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x000072F800001F4ELL) //16-bit in 32-bit, LE
        {
            break; //while()
        }
        if (CC8(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x006F87200054E1F0LL) //20-bit in 32-bit, BE
        {
            break; //while()
        }
        if (CC8(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x0020876F00F0E154LL) //20-bit in 32-bit, LE
        {
            break; //while()
        }
        if (CC8(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x0096F8720A54E1FLL) //24-bit in 32-bit, BE
        {
            break; //while()
        }
        if (CC8(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset)==0x0072F896001F4EA5LL) //24-bit in 32-bit, LE
        {
            break; //while()
        }

        if (ByteDepth!=(size_t)-1)
            Common->MergedChannel.Buffer_Offset+=ByteDepth*2;
        else
            Common->MergedChannel.Buffer_Offset++;
    }

    //Parsing last bytes if needed
    if (Common->MergedChannel.Buffer_Offset+16>Common->MergedChannel.Buffer_Size)
    {
        return false;
    }

    //Synched
    return true;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_AES3_YES
