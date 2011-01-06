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
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

File_ChannelGrouping::File_ChannelGrouping()
{
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
        Common->Channels.resize(Channel_Total);
        for (size_t Pos=0; Pos<Common->Channels.size(); Pos++)
            Common->Channels[Pos]=new common::channel;
        Open_Buffer_Init(Common->Parser);
    }
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

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_AES3_YES
