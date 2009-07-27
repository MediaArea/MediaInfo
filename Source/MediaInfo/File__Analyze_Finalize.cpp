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
#include "MediaInfo/File__Analyze.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Global()
{
    if (IsSub)
        return;

    Finalize_StreamOnly();
    Finalize_InterStreams();
    Finalize_StreamOnly();
    Finalize_Cosmetic();
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly()
{
    //Generic
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<Count_Get((stream_t)StreamKind); StreamPos++)
            Finalize_StreamOnly((stream_t)StreamKind, StreamPos);

    //For each kind of (*Stream)
    for (size_t Pos=0; Pos<Count_Get(Stream_General);  Pos++) Finalize_StreamOnly_General(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Video);    Pos++) Finalize_StreamOnly_Video(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Audio);    Pos++) Finalize_StreamOnly_Audio(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Text);     Pos++) Finalize_StreamOnly_Text(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Chapters); Pos++) Finalize_StreamOnly_Chapters(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Image);    Pos++) Finalize_StreamOnly_Image(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Menu);     Pos++) Finalize_StreamOnly_Menu(Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly(stream_t StreamKind, size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_General(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_Video(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_Audio(size_t Pos)
{
    //Delay/Video
    if (Count_Get(Stream_Video)>0 && Retrieve(Stream_Audio, Pos, Audio_Video_Delay).empty() && !Retrieve(Stream_Audio, Pos, Audio_Delay).empty() && !Retrieve(Stream_Video, 0, Video_Delay).empty())
        Fill(Stream_Audio, Pos, Audio_Video_Delay, Retrieve(Stream_Audio, Pos, Audio_Delay).To_int64s()-Retrieve(Stream_Video, 0, Video_Delay).To_int64s());
    //Delay/Video0
    if (Count_Get(Stream_Video)>0 && Retrieve(Stream_Audio, Pos, Audio_Video0_Delay).empty() && !Retrieve(Stream_Audio, Pos, Audio_Delay).empty() && !Retrieve(Stream_Video, 0, Video_Delay).empty())
        Fill(Stream_Audio, Pos, Audio_Video0_Delay, Retrieve(Stream_Audio, Pos, Audio_Delay).To_int64s()-Retrieve(Stream_Video, 0, Video_Delay).To_int64s());
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_Text(size_t Pos)
{
    //Delay/Video
    if (Count_Get(Stream_Video)>0 && Retrieve(Stream_Text, Pos, Text_Video_Delay).empty() && !Retrieve(Stream_Text, Pos, Text_Delay).empty())
        Fill(Stream_Text, Pos, Text_Video_Delay, Retrieve(Stream_Text, Pos, Text_Delay).To_int32s()-Retrieve(Stream_Video, 0, "Delay").To_int32s());
    //Delay/Video0
    if (Count_Get(Stream_Video)>0 && Retrieve(Stream_Text, Pos, Text_Video0_Delay).empty() && !Retrieve(Stream_Text, Pos, Text_Delay).empty())
        Fill(Stream_Text, Pos, Text_Video0_Delay, Retrieve(Stream_Text, Pos, Text_Delay).To_int32s()-Retrieve(Stream_Video, 0, "Delay").To_int32s());
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_Chapters(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_Image(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_StreamOnly_Menu(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_InterStreams()
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic()
{
    //Generic
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<Count_Get((stream_t)StreamKind); StreamPos++)
            Finalize_Cosmetic((stream_t)StreamKind, StreamPos);

    //For each kind of (*Stream)
    for (size_t Pos=0; Pos<Count_Get(Stream_General);  Pos++) Finalize_Cosmetic_General(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Video);    Pos++) Finalize_Cosmetic_Video(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Audio);    Pos++) Finalize_Cosmetic_Audio(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Text);     Pos++) Finalize_Cosmetic_Text(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Chapters); Pos++) Finalize_Cosmetic_Chapters(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Image);    Pos++) Finalize_Cosmetic_Image(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Menu);     Pos++) Finalize_Cosmetic_Menu(Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic(stream_t StreamKind, size_t Pos)
{
    //Bits/(Pixel*Frame)
    if (StreamKind==Stream_Video && (!Retrieve(Stream_Video, Pos, Video_BitRate).empty() || !Retrieve(Stream_Video, Pos, Video_BitRate_Nominal).empty()))
    {
        float32 BitRate=Retrieve(Stream_Video, Pos, Video_BitRate).To_float32();
        if (BitRate==0)
            BitRate=Retrieve(Stream_Video, Pos, Video_BitRate_Nominal).To_float32();
        float F1=(float)Retrieve(Stream_Video, Pos, "Width").To_int32s()*(float)Retrieve(Stream_Video, Pos, "Height").To_int32s()*Retrieve(Stream_Video, Pos, "FrameRate").To_float32();
        if (F1)
            Fill(Stream_Video, Pos, Video_Bits__Pixel_Frame_, BitRate/F1);
    }

}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_General(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_Video(size_t Pos)
{
    //Display Aspect Ratio and Pixel Aspect Ratio
    AspectRatio_AspectRatio(Pos, Video_DisplayAspectRatio, Video_PixelAspectRatio, Video_DisplayAspectRatio_String);
    AspectRatio_AspectRatio(Pos, Video_DisplayAspectRatio_Original, Video_PixelAspectRatio_Original, Video_DisplayAspectRatio_Original_String);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_Audio(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_Text(size_t Pos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_Chapters(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_Image(size_t StreamPos)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Cosmetic_Menu(size_t StreamPos)
{
}

} //NameSpace

