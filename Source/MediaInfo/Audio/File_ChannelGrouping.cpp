// File_ChannelGrouping - Regrouping PCM streams
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_SMPTEST0337_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_ChannelGrouping.h"
#if defined(MEDIAINFO_SMPTEST0337_YES)
    #include "MediaInfo/Audio/File_SmpteSt0337.h"
#endif
#if defined(MEDIAINFO_PCM_YES)
    #include "MediaInfo/Audio/File_Pcm.h"
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
        StreamIDs_Width[0]=0;
    #endif //MEDIAINFO_EVENTS
    #if MEDIAINFO_DEMUX
        Demux_Level=2; //Container
    #endif //MEDIAINFO_DEMUX
    #if MEDIAINFO_TRACE
        Trace_Layers_Update(0); //Container1
    #endif //MEDIAINFO_TRACE
    IsRawStream=true;

    //In
    ByteDepth=0;
    Common=NULL;
    Channel_Pos=0;
    Channel_Total=1;
    SampleRate=0;
    Endianness=0;
    CanBePcm=false;
}

File_ChannelGrouping::~File_ChannelGrouping()
{
    Common->Instances--;

    if (Common->Instances==0)
    {
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
            delete Common->Channels[Pos]; //Common->Channels[Pos]=NULL;
        delete Common; //Common=NULL;
    }
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_ChannelGrouping::Streams_Fill()
{
    Fill(Stream_General, 0, General_Format, "ChannelGrouping");

    if (Channel_Pos!=Common->Channels.size()-1)
        return;

    if (Common->Parsers.size()!=1 && CanBePcm) // Last parser is PCM, impossible to detect with another method if htere is only one block
    {
        for (size_t Pos=0; Pos<Common->Parsers.size()-1; Pos++)
            delete Common->Parsers[Pos];
        Common->Parsers.erase(Common->Parsers.begin(), Common->Parsers.begin()+Common->Parsers.size()-1);
        Common->Parsers[0]->Accept();
        Common->Parsers[0]->Fill();
    }

    if (Common->Parsers.size()!=1)
        return;

    Fill(Common->Parsers[0]);
    Merge(*Common->Parsers[0]);
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Streams_Finish()
{
    if (Channel_Pos!=Common->Channels.size()-1 || Common->Parsers.size()!=1)
        return;

    Finish(Common->Parsers[0]);
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
        Common->Channels.resize(Channel_Total);
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
            Common->Channels[Pos]=new common::channel;
        Element_Code=(int64u)-1;

        File_SmpteSt0337* Parser=new File_SmpteSt0337;
        Parser->Endianness=Endianness;
        Parser->Container_Bits=ByteDepth*8;
        Common->Parsers.push_back(Parser);

        if (CanBePcm)
        {
            File_Pcm* Parser=new File_Pcm;
            Parser->Endianness=Endianness;
            Common->Parsers.push_back(Parser);
        }

        for (size_t Pos=0; Pos<Common->Parsers.size(); Pos++)
            Open_Buffer_Init(Common->Parsers[Pos]);
    }
    Common->Instances++;

    #if MEDIAINFO_DEMUX
         Demux_UnpacketizeContainer=Config->Demux_Unpacketize_Get();
    #endif //MEDIAINFO_DEMUX
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Read_Buffer_Continue()
{
    //Handling of multiple frames in one block
    if (Buffer_Size==0)
    {
        Offsets_Stream.clear();
        Offsets_Buffer.clear();
        for (size_t Pos=0; Pos<Common->Parsers.size(); Pos++)
            Open_Buffer_Continue(Common->Parsers[Pos], Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, 0, false);
        return;
    }

    //Demux
    #if MEDIAINFO_DEMUX
        if (Demux_UnpacketizeContainer)
        {
            for (size_t Pos=0; Pos<Common->Parsers.size(); Pos++)
            {
                Common->Parsers[Pos]->Demux_UnpacketizeContainer=true;
                Common->Parsers[Pos]->Demux_Level=2; //Container
            }
            Demux_Level=4; //Intermediate
        }
        Demux(Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset, ContentType_MainStream);
    #endif //MEDIAINFO_EVENTS

    //Copying to Channel buffer
    if (Common->Channels[Channel_Pos]->Buffer_Size+Buffer_Size>Common->Channels[Channel_Pos]->Buffer_Size_Max)
        Common->Channels[Channel_Pos]->resize(Common->Channels[Channel_Pos]->Buffer_Size+Buffer_Size);
    memcpy(Common->Channels[Channel_Pos]->Buffer+Common->Channels[Channel_Pos]->Buffer_Size, Buffer, Buffer_Size);
    Common->Channels[Channel_Pos]->Buffer_Size+=Buffer_Size;
    Common->Channels[Channel_Pos]->Offsets_Stream.insert(Common->Channels[Channel_Pos]->Offsets_Stream.begin(), Offsets_Stream.begin(), Offsets_Stream.end());
    Offsets_Stream.clear();
    Common->Channels[Channel_Pos]->Offsets_Buffer.insert(Common->Channels[Channel_Pos]->Offsets_Buffer.begin(), Offsets_Buffer.begin(), Offsets_Buffer.end());
    Offsets_Buffer.clear();
    Skip_XX(Buffer_Size,                                        "Channel grouping data");
    Common->Channel_Current++;
    if (Common->Channel_Current>=Channel_Total)
        Common->Channel_Current=0;

    //Copying to merged channel
    size_t Minimum=(size_t)-1;
    for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
        if (Minimum>Common->Channels[Pos]->Buffer_Size-Common->Channels[Pos]->Buffer_Offset)
            Minimum=Common->Channels[Pos]->Buffer_Size-Common->Channels[Pos]->Buffer_Offset;
    if (Minimum>=ByteDepth)
    {
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
        {
            Common->MergedChannel.Offsets_Stream.insert(Common->MergedChannel.Offsets_Stream.end(), Common->Channels[Pos]->Offsets_Stream.begin(), Common->Channels[Pos]->Offsets_Stream.end());
            Common->Channels[Pos]->Offsets_Stream.clear();
            Common->MergedChannel.Offsets_Buffer.insert(Common->MergedChannel.Offsets_Buffer.end(), Common->Channels[Pos]->Offsets_Buffer.begin(), Common->Channels[Pos]->Offsets_Buffer.end());
            Common->Channels[Pos]->Offsets_Buffer.clear();
        }

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
    }

    if (Common->MergedChannel.Buffer_Size>Common->MergedChannel.Buffer_Offset)
    {
        for (size_t Pos=0; Pos<Common->Parsers.size(); Pos++)
        {
            if (FrameInfo_Next.DTS!=(int64u)-1)
                Common->Parsers[Pos]->FrameInfo=FrameInfo_Next; //AES3 parse has its own buffer management
            else
                Common->Parsers[Pos]->FrameInfo=FrameInfo;
            Common->Parsers[Pos]->Offsets_Stream.insert(Common->Parsers[Pos]->Offsets_Stream.end(), Common->MergedChannel.Offsets_Stream.begin(), Common->MergedChannel.Offsets_Stream.end());
            Common->Parsers[Pos]->Offsets_Buffer.insert(Common->Parsers[Pos]->Offsets_Buffer.end(), Common->MergedChannel.Offsets_Buffer.begin(), Common->MergedChannel.Offsets_Buffer.end());
            for (size_t Offsets_Pos_Temp=Common->Parsers[Pos]->Offsets_Buffer.size()-Common->MergedChannel.Offsets_Buffer.size(); Offsets_Pos_Temp<Common->Parsers[Pos]->Offsets_Buffer.size(); Offsets_Pos_Temp++)
                Common->Parsers[Pos]->Offsets_Buffer[Offsets_Pos_Temp]+=Common->Parsers[Pos]->Buffer_Size/Common->Channels.size();
            Open_Buffer_Continue(Common->Parsers[Pos], Common->MergedChannel.Buffer+Common->MergedChannel.Buffer_Offset, Common->MergedChannel.Buffer_Size-Common->MergedChannel.Buffer_Offset, false);

            //Multiple parsers
            if (Common->Parsers.size()>1)
            {
                if (!Common->Parsers[Pos]->Status[IsAccepted] && Common->Parsers[Pos]->Status[IsFinished])
                {
                    delete *(Common->Parsers.begin()+Pos);
                    Common->Parsers.erase(Common->Parsers.begin()+Pos);
                    Pos--;
                }
                else if (Common->Parsers.size()>1 && Common->Parsers[Pos]->Status[IsAccepted])
                {
                    File__Analyze* Parser=Common->Parsers[Pos];
                    for (size_t Pos2=0; Pos2<Common->Parsers.size(); Pos2++)
                    {
                        if (Pos2!=Pos)
                            delete *(Common->Parsers.begin()+Pos2);
                    }
                    Common->Parsers.clear();
                    Common->Parsers.push_back(Parser);
                }
            }
        }
        Common->MergedChannel.Buffer_Offset=Common->MergedChannel.Buffer_Size;
        Common->MergedChannel.Offsets_Stream.clear();
        Common->MergedChannel.Offsets_Buffer.clear();
    }
    if (!Status[IsAccepted] && Common->Parsers.size()==1 && Common->Parsers[0]->Status[IsAccepted])
        Accept();
    if (!Status[IsFilled] && Common->Parsers.size()==1 && Common->Parsers[0]->Status[IsFilled])
        Fill();
    if (!Status[IsFinished] && Common->Parsers.size()==1 && Common->Parsers[0]->Status[IsFinished])
        Finish();

    //Optimize buffer
    for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
        Common->Channels[Pos]->optimize();
    Common->MergedChannel.optimize();
}

//---------------------------------------------------------------------------
void File_ChannelGrouping::Read_Buffer_Unsynched()
{
    for (size_t Pos=0; Pos<Common->Parsers.size(); Pos++)
        if (Common->Parsers[Pos])
            Common->Parsers[Pos]->Open_Buffer_Unsynch();

    Common->MergedChannel.Buffer_Offset=0;
    Common->MergedChannel.Buffer_Size=0;
    for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
    {
        Common->Channels[Pos]->Buffer_Offset=0;
        Common->Channels[Pos]->Buffer_Size=0;
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_SMPTEST0337_YES

