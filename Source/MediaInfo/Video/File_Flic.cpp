// File_Flic - Info for Flic files
// Copyright (C) 2006-2008 Jerome Martinez, Zen@MediaArea.net
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
// From : http://www.compuphase.com/flic.htm
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
#if defined(MEDIAINFO_FLIC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Flic.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flic::FileHeader_Parse()
{
    //Parsing
    int32u DelayBetweenFrames;
    int16u Type, Frames, Width, Height, BitsPerPixel, AspectX, AspectY;
    Skip_B4(                                                    "Size of FLIC including this header");
    Get_B2 (Type,                                               "File type");
    Get_B2 (Frames,                                             "Number of frames in first segment");
    Get_B2 (Width,                                              "Width");
    Get_B2 (Height,                                             "Height");
    Get_B2 (BitsPerPixel,                                       "Bits per pixel");
    Skip_B2(                                                    "Unknown");
    Get_B4 (DelayBetweenFrames,                                 "Delay between frames");
    if (Type!=0xAF11)
    {
        Skip_B2(                                                "Reserved");
        Skip_B4(                                                "Date of Creation)");
        Skip_B4(                                                "Serial number or compiler id");
        Skip_B4(                                                "Date of FLIC update");
        Skip_B4(                                                "Serial number");
        Get_B2 (AspectX,                                        "Width of square rectangle");
        Get_B2 (AspectY,                                        "Height of square rectangle");
    }
    else
        Skip_XX(22,                                             "Reserved");
    Skip_B2(                                                    "EGI: flags for specific EGI extensions");
    Skip_B2(                                                    "EGI: key-image frequency");
    Skip_B2(                                                    "EGI: total number of frames (segments)");
    Skip_B4(                                                    "EGI: maximum chunk size (uncompressed)");
    Skip_B2(                                                    "EGI: max. number of regions in a CHK_REGION chunk");
    Skip_B2(                                                    "EGI: number of transparent levels");
    if (Type!=0xAF11)
    {
        Skip_XX(24,                                             "Reserved");
        Skip_B4(                                                "Offset to frame 1");
        Skip_B4(                                                "Offset to frame 2");
        Skip_XX(40,                                             "Reserved");
    }
    else
        Skip_XX(72,                                             "Reserved");

    //Filling
    FILLING_BEGIN();
        switch (Type)
        {
            case 0xAF11 :
            case 0xAF12 :
            case 0xAF30 :
            case 0xAF31 :
            case 0xAF44 :
                            break;
            default     :
                            Finnished();
                            return;
        }

        Stream_Prepare(Stream_General);
        Fill("Format", "FLIC");
        Stream_Prepare(Stream_Video);
        if (Type==0xAF11)
        {
            Fill("Codec", "FLI");
            Fill("FrameRate", 1.0/DelayBetweenFrames); //ms per frame
            Fill("PlayTime", Frames*DelayBetweenFrames);
        }
        else
        {
            Fill("Codec", "FLC");
            Fill("FrameRate", DelayBetweenFrames*1000/70); //multiple of 1/70 per frame
            Fill("PlayTime", Frames*DelayBetweenFrames*1000/70);
            if (AspectY>0)
                Fill("DisplayAspectRatio", AspectX/AspectY);
        }
        Fill("FrameCount", Frames);
        Fill("Width", Width);
        Fill("Height", Height);
        Fill("Resolution", BitsPerPixel);

        Finnished();
    FILLING_END();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flic::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("DisplayAspectRatio", "R");
            Fill_HowTo("FrameRate", "R");
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

} //NameSpace

#endif //MEDIAINFO_FLIC_*

