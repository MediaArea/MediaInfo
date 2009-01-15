// File_Dpg - Info for DPG files
// Copyright (C) 2009-2009 Jerome Martinez, Zen@MediaArea.net
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
// Format:
// * File header
// * MPEG Audio stream
// * MPEG Video stream
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
#if defined(MEDIAINFO_DPG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Dpg.h"
#if defined(MEDIAINFO_MPEGV_YES)
    #include "MediaInfo/Video/File_Mpegv.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Dpg::File_Dpg()
{
    //Data
    Parser=NULL;
}

//---------------------------------------------------------------------------
File_Dpg::~File_Dpg()
{
    delete Parser; //Parser=NULL;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpg::FileHeader_Parse()
{
    //Parsing
    int32u Signature, FrameCount, FrameRate, SamplingRate, Zero;
    Get_C4 (Signature,                                          "Signature");
    Get_L4 (FrameCount,                                         "Frame count");
    Get_L4 (FrameRate,                                          "Frame rate"); Param_Info(FrameRate/0x100, " fps");
    Get_L4 (SamplingRate,                                       "Sampling rate");
    Get_L4 (Zero,                                               "0x00000000");
    Get_L4 (Audio_Offset,                                       "Audio Offset");
    Get_L4 (Audio_Size,                                         "Audio Size");
    Get_L4 (Video_Offset,                                       "Video Offset");
    Get_L4 (Video_Size,                                         "Video Size");

    FILLING_BEGIN();
        //Integrity
        if (Signature!=0x44504730 || Zero!=0x00000000) //"DPG0"
        {
            Finished();
            return;
        }

        //Filling
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "DPG");
        Stream_Prepare(Stream_Video);
        Fill(Stream_Video, 0, Video_FrameRate, (float)(FrameRate/0x100), 3);
        Fill(Stream_Video, 0, Video_FrameCount, FrameCount);
        Fill(Stream_Video, 0, Video_StreamSize, Video_Size);
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_SamplingRate, SamplingRate);
        Fill(Stream_Audio, 0, Audio_StreamSize, Audio_Size);

        //Positionning
        #if defined(MEDIAINFO_MPEGA_YES)
            Data_GoTo(Audio_Offset, "DPG");
            Parser=new File_Mpega();
        #elif defined(MEDIAINFO_MPEGV_YES)
            Audio_Size=0;
            Data_GoTo(Video_Offset, "DPG");
            Parser=new File_Mpegv();
        #else
            Finished();
        #endif
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpg::Read_Buffer_Continue()
{
    if (!Parser)
        return; //Not ready

    if (Audio_Size)
    {
        #if defined(MEDIAINFO_MPEGA_YES)
            Open_Buffer_Init(Parser, Audio_Offset+Audio_Size, File_Offset+Buffer_Offset);
            Open_Buffer_Continue(Parser, Buffer+Buffer_Offset, (size_t)((File_Offset+Buffer_Size<Audio_Offset+Audio_Size)?Buffer_Size:(Audio_Offset+Audio_Size-File_Offset)));
            if (Parser->File_Offset==Parser->File_Size || Parser->File_GoTo!=(int64u)-1)
            {
                Open_Buffer_Finalize(Parser);
                Merge(*Parser, Stream_Audio, 0, 0);
                #if defined(MEDIAINFO_MPEGV_YES)
                    Audio_Size=0;
                    Data_GoTo(Video_Offset, "DPG");
                    delete Parser; Parser=new File_Mpegv();
                #else
                    Info("DPG, parsing finished");
                    Finished();
                #endif
            }
        #endif
    }
    else
    {
        #if defined(MEDIAINFO_MPEGV_YES)
            Open_Buffer_Init(Parser, Video_Offset+Video_Size, File_Offset+Buffer_Offset);
            Open_Buffer_Continue(Parser, Buffer+Buffer_Offset, (size_t)((File_Offset+Buffer_Size<Video_Offset+Video_Size)?Buffer_Size:(Video_Offset+Video_Size-File_Offset)));
            if (Parser->File_Offset==Parser->File_Size || Parser->File_GoTo!=(int64u)-1)
            {
                //Get the container frame rate
                Ztring FrameRate=Retrieve(Stream_Video, 0, Video_FrameRate);

                //Merging
                Open_Buffer_Finalize(Parser);
                Merge(*Parser, Stream_Video, 0, 0);

                //Filling with container FrameRate
                const Ztring& FrameRate_Video=Retrieve(Stream_Video, 0, Video_FrameRate);
                if (FrameRate!=FrameRate_Video)
                {
                    Fill(Stream_Video, 0, Video_FrameRate_Original, FrameRate_Video);
                    Fill(Stream_Video, 0, Video_FrameRate, FrameRate, true);
                }

                //Finished
                Info("DPG, parsing finished");
                Finished();
            }
        #endif
    }

    //Positioning
    Buffer_Offset=Buffer_Size; //We have already parsed this data
}

} //NameSpace

#endif //MEDIAINFO_DPG_YES
