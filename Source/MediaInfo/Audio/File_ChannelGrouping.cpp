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
    SampleRate=0;
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
    if (!IsAes3)
        return; //Nothing to do
    
    Fill(Stream_General, 0, General_Format, "ChannelGrouping");

    if (Common->Channel_Master!=Channel_Pos)
        return;
    Fill(Common->Parser);
    Merge(*Common->Parser);
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Streams_Finish()
{
    if (!IsAes3)
        return; //Nothing to do
    
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
        ((File_Aes3*)Common->Parser)->SampleRate=SampleRate;
        ((File_Aes3*)Common->Parser)->ByteSize=ByteDepth*Channel_Total;
        Common->Channels.resize(Channel_Total);
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
            Common->Channels[Pos]=new common::channel;
        Open_Buffer_Init(Common->Parser);
    }

    IsAes3=false;
    #if MEDIAINFO_DEMUX
         Demux_UnpacketizeContainer=Config->Demux_Unpacketize_Get();
    #endif //MEDIAINFO_DEMUX
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Read_Buffer_Continue()
{
    //Handling of multiple frames in one block
    if (Buffer_Size==0 && Common->Parser && Common->Parser->Buffer_Size)
    {
        Open_Buffer_Continue(Common->Parser, Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, 0);
        return;
    }

    //Testing if it is AES3 instead of mono PCM
    if (!IsAes3 && ((Channel_Pos==0 && !Synchronize_AES3_0()) || (Channel_Pos==1 && !Synchronize_AES3_1())))
    {
        if (Buffer_TotalBytes+Buffer_Size<65536)
        {
            Buffer_Offset=0; //Reinit
            Element_WaitForMoreData();
            return;
        }

        #if MEDIAINFO_DEMUX
            Buffer_Offset=0;
            Demux_Offset=Buffer_Size;
            Demux_UnpacketizeContainer_Demux();
        #endif //MEDIAINFO_DEMUX

        Buffer_Offset=Buffer_Size;
        Frame_Count++;
        Accept();
        Finish();
        return;
    }
    IsAes3=true;
    Buffer_Offset=0;

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

    //Demux
    #if MEDIAINFO_DEMUX
        if (Demux_UnpacketizeContainer)
        {
            Common->Parser->Demux_UnpacketizeContainer=true;
            Demux_Level=4; //Intermediate

            if (Channel_Pos)
                StreamIDs[StreamIDs_Size-2]=StreamID;
            if (StreamIDs_Size>=2)
                Element_Code=StreamIDs[StreamIDs_Size-2];
            StreamIDs_Size--;
            Demux(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset, ContentType_MainStream);
            StreamIDs_Size++;
        }
    #endif //MEDIAINFO_EVENTS

    Common->Parser->FrameInfo=FrameInfo;
    Open_Buffer_Continue(Common->Parser, Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset);
    Common->MergedChannel.Buffer_Offset=Common->MergedChannel.Buffer_Size;

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

//---------------------------------------------------------------------------
void File_ChannelGrouping::Read_Buffer_Unsynched()
{
    if (Common->Parser)
        Common->Parser->Open_Buffer_Unsynch();
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_ChannelGrouping::Synchronize_AES3_0()
{
    //Synchronizing
    while (Buffer_Offset+8<=Buffer_Size)
    {
        if (CC2(Buffer+Buffer_Offset)==0xF872) //SMPTE 337M 16-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC2(Buffer+Buffer_Offset)==0x72F8) //SMPTE 337M 16-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x96F872) //SMPTE 337M 24-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x72F896) //SMPTE 337M 24-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x00F872) //16-bit in 24-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x0072F8) //16-bit in 24-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x6F8720) //20-bit in 24-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x20876F) //20-bit in 24-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x0000F872) //16-bit in 32-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x000072F8) //16-bit in 32-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x006F872) //20-bit in 32-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x0020876F) //20-bit in 32-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x0096F872) //24-bit in 32-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x0072F896) //24-bit in 32-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }

        if (ByteDepth)
            Buffer_Offset+=ByteDepth;
        else
            Buffer_Offset++;
    }

    //Parsing last bytes if needed
    if (Buffer_Offset+8>Buffer_Size)
        return false;

    //Synched
    return true;
}

//---------------------------------------------------------------------------
bool File_ChannelGrouping::Synchronize_AES3_1()
{
    //Synchronizing
    while (Buffer_Offset+8<=Buffer_Size)
    {
        if (CC2(Buffer+Buffer_Offset)==0x4E1F) //SMPTE 337M 16-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC2(Buffer+Buffer_Offset)==0x1F4E) //SMPTE 337M 16-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0xA54E1F) //SMPTE 337M 24-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x1F4E5A) //SMPTE 337M 24-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x004E1F) //16-bit in 24-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x001F4E) //16-bit in 24-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0x54E1F0) //20-bit in 24-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC3(Buffer+Buffer_Offset)==0xF0E154) //20-bit in 24-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x00004E1F) //16-bit in 32-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x00001F4E) //16-bit in 32-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x0054E1F0) //20-bit in 32-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x00F0E154) //20-bit in 32-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x0A54E1F) //24-bit in 32-bit, BE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }
        if (CC4(Buffer+Buffer_Offset)==0x001F4EA5) //24-bit in 32-bit, LE
        {
            if (Frame_Count)
                break; //while()
            Frame_Count++;
        }

        if (ByteDepth)
            Buffer_Offset+=ByteDepth;
        else
            Buffer_Offset++;
    }

    //Parsing last bytes if needed
    if (Buffer_Offset+8>Buffer_Size)
        return false;

    //Synched
    return true;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_AES3_YES
