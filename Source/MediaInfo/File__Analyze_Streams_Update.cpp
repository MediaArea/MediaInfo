// File__Analyze_Inform - Base for other files
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
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
// Init and Finalize part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/Utils.h"
#include "ZenLib/File.h"
#include "ZenLib/FileName.h"
#include "MediaInfo/File__Analyze.h"
#ifdef SS
   #undef SS //Solaris defines this somewhere
#endif
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Global()
{
    if (IsSub)
        return;

    Streams_Update_StreamOnly();
    Streams_Update_InterStreams();
    Streams_Update_StreamOnly();
    Streams_Update_Cosmetic();
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly()
{
    //Generic
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<Count_Get((stream_t)StreamKind); StreamPos++)
            Streams_Update_StreamOnly((stream_t)StreamKind, StreamPos);

    //For each kind of (*Stream)
    for (size_t Pos=0; Pos<Count_Get(Stream_General);  Pos++) Streams_Update_StreamOnly_General(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Video);    Pos++) Streams_Update_StreamOnly_Video(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Audio);    Pos++) Streams_Update_StreamOnly_Audio(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Text);     Pos++) Streams_Update_StreamOnly_Text(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Chapters); Pos++) Streams_Update_StreamOnly_Chapters(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Image);    Pos++) Streams_Update_StreamOnly_Image(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Menu);     Pos++) Streams_Update_StreamOnly_Menu(Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly(stream_t StreamKind, size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_General(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_Video(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_Audio(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_Text(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_Chapters(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_Image(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_StreamOnly_Menu(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_InterStreams()
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic()
{
    //Generic
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<Count_Get((stream_t)StreamKind); StreamPos++)
            Streams_Update_Cosmetic((stream_t)StreamKind, StreamPos);

    //For each kind of (*Stream)
    for (size_t Pos=0; Pos<Count_Get(Stream_General);  Pos++) Streams_Update_Cosmetic_General(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Video);    Pos++) Streams_Update_Cosmetic_Video(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Audio);    Pos++) Streams_Update_Cosmetic_Audio(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Text);     Pos++) Streams_Update_Cosmetic_Text(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Chapters); Pos++) Streams_Update_Cosmetic_Chapters(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Image);    Pos++) Streams_Update_Cosmetic_Image(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Menu);     Pos++) Streams_Update_Cosmetic_Menu(Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic(stream_t StreamKind, size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_General(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_Video(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_Audio(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_Text(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_Chapters(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_Image(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Streams_Update_Cosmetic_Menu(size_t StreamPos)
{
}

} //NameSpace

