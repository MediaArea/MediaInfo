// File_Ivf - Info for Ivf files
// Copyright (C) 2006-2010 MediaArea.net SARL, Info@MediaArea.net
// Copyright (C) 2010 Lionel DUCHATEAU, kurtnoise at free dot fr
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
//
// From : http://www.webmproject.org/
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_IVF_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Ivf.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ivf::FileHeader_Begin()
{
    //Synchro
    if (Buffer_Offset+4>Buffer_Size)
        return false;
    if (CC4(Buffer+Buffer_Offset)!=0x444B4946) //"DKIF"
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Ivf::FileHeader_Parse()
{
    //Parsing
    int32u frame_rate_num, frame_rate_den, frame_count, unused, fourcc;
    int16u version, header_size, width, height;

    Skip_C4 (                                                   "DKIF");
    Get_L2 (version,                                            "Version");
    Get_L2 (header_size,                                        "Header Size");
    Get_C4 (fourcc,                                             "Fourcc");
    Get_L2 (width,                                              "Width");
    Get_L2 (height,                                             "Height");
    Get_L4 (frame_rate_num,                                     "FrameRate Numerator");
    Get_L4 (frame_rate_den,                                     "FrameRate Denominator");
    Get_L4 (frame_count,                                        "Frame Count");
    Get_L4 (unused,                                             "Unused");

    FILLING_BEGIN();
        Accept("IVF");

        Fill(Stream_General, 0, General_Format, "Ivf");

        Stream_Prepare(Stream_Video);
        Fill(Stream_Video, 0, Video_Format, _T("VP8"));
        Fill(Stream_Video, 0, Video_Codec, _T("VP8"));
        Fill(Stream_Video, 0, Video_CodecID, Ztring().From_CC4(fourcc));
        Fill(Stream_Video, StreamPos_Last, Video_FrameRate, frame_rate_num / frame_rate_den);
        Fill(Stream_Video, 0, Video_FrameCount, frame_count);
        Fill(Stream_Video, StreamPos_Last, Video_Width, width);
        Fill(Stream_Video, StreamPos_Last, Video_Height, height);

        //No more need data
        Finish("IVF");
    FILLING_END();
}

} //NameSpace

#endif //MEDIAINFO_IVF_*

