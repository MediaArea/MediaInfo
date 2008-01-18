// File_Ape - Info for Ape files
// Copyright (C) 2003-2008 Jasper van de Gronde, th.v.d.gronde@hccnet.nl
// Copyright (C) 2003-2008 Jerome Martinez, zen@mediaarea.net
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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_APE_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Ape.h"

using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
int32u Ape_SamplesPerFrame(int16u Version, int16u CompressionLevel)
{
    if (Version>=3950)
        return 73728*4;
    else if (Version>=3900)
        return 73728;
    else if (Version>=3800 && CompressionLevel==4000)
        return 73728;
    else
        return 9216;
}

const char* Ape_Codec_Settings(int16u Setting)
{
    switch (Setting)
    {
        case 1000 : return "fast";
        case 2000 : return "normal";
        case 3000 : return "high";
        case 4000 : return "extra-high";
        case 5000 : return "insane";
        default   : return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ape::File_Ape()
:File__Analyze(), File__Tags_Helper()
{
    //File__Tags_Helper
    Base=this;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ape::Read_Buffer_Continue()
{
    //Tags
    if (!File__Tags_Helper::Read_Buffer_Continue())
        return;
}

//---------------------------------------------------------------------------
void File_Ape::Read_Buffer_Finalize()
{
    //Tags
    File__Tags_Helper::Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ape::Header_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Ape::FileHeader_Parse()
{
    //Parsing
    Element_Begin("APE header");
    int32u Identifier, SampleRate=0, TotalFrames=0, FinalFrameSamples=0, SamplesPerFrame=0, SeekElements;
    int16u Version, CompressionLevel=0, Flags=0, Channels=0, Resolution=0;
    bool Resolution8=false, Resolution24=false, no_wav_header;
    Get_C4 (Identifier,                                         "Identifier");
    Get_L2 (Version,                                            "Version");
    if (Version<3980) //<3.98
    {
        Get_L2 (CompressionLevel,                               "CompressionLevel"); Param_Info(Ape_Codec_Settings(CompressionLevel));
        Get_L2 (Flags,                                          "FormatFlags");
            Get_Flags (Flags, 0, Resolution8,                   "8-bit");
            Skip_Flags(Flags, 1,                                "crc-32");
            Skip_Flags(Flags, 2,                                "peak_level");
            Get_Flags (Flags, 3, Resolution24,                  "24-bit");
            Skip_Flags(Flags, 4,                                "seek_elements");
            Get_Flags (Flags, 5, no_wav_header,                 "no_wav_header");
        if (Resolution8)
            Resolution=8;
        else if (Resolution24)
            Resolution=24;
        else
            Resolution=16;
        Get_L2 (Channels,                                       "Channels");
        Get_L4 (SampleRate,                                     "SampleRate");
        Skip_L4(                                                "WavHeaderDataBytes");
        Skip_L4(                                                "WavTerminatingBytes");
        Get_L4 (TotalFrames,                                    "TotalFrames");
        Get_L4 (FinalFrameSamples,                              "FinalFrameSamples");
        SamplesPerFrame=Ape_SamplesPerFrame(Version, CompressionLevel);
        Skip_L4(                                                "PeakLevel");
        Get_L4 (SeekElements,                                   "SeekElements");
        if (!no_wav_header)
            Skip_XX(44,                                         "RIFF header");
        Skip_XX(SeekElements*4,                                 "Seek table");
    }
    else
    {
        Skip_L2(                                                "Version_High");
        Skip_L4(                                                "DescriptorBytes");
        Skip_L4(                                                "HeaderBytes");
        Skip_L4(                                                "SeekTableBytes");
        Skip_L4(                                                "WavHeaderDataBytes");
        Skip_L4(                                                "APEFrameDataBytes");
        Skip_L4(                                                "APEFrameDataBytesHigh");
        Skip_L4(                                                "WavTerminatingDataBytes");
        Skip_L16(                                               "FileMD5");
        Get_L2 (CompressionLevel,                               "CompressionLevel"); Param_Info(Ape_Codec_Settings(CompressionLevel));
        Get_L2 (Flags,                                          "FormatFlags");
        Get_L4 (SamplesPerFrame,                                "BlocksPerFrame");
        Get_L4 (FinalFrameSamples,                              "FinalFrameBlocks");
        Get_L4 (TotalFrames,                                    "TotalFrames");
        Get_L2 (Resolution,                                     "BitsPerSample");
        Get_L2 (Channels,                                       "Channels");
        Get_L4 (SampleRate,                                     "SampleRate");
    }
    Element_End();

    FILLING_BEGIN();
        //Integrity
        if (Identifier!=CC4("MAC "))
        {
            Finnished();
            return;
        }

        //Coherancy
        if (SampleRate==0 || Channels==0 || Resolution==0)
        {
            Finnished();
            return;
        }

        //Filling
        int32u Samples=(TotalFrames-1)*SamplesPerFrame+FinalFrameSamples;
        int64u PlayTime=((int64u)Samples)*1000/SampleRate;
        if (PlayTime==0)
        {
            Finnished();
            return;
        }
        int64u UncompressedSize=Samples*Channels*(Resolution/8);
        if (UncompressedSize==0)
        {
            Finnished();
            return;
        }
        float32 CompressionRatio=((float32)File_Size)/UncompressedSize;
        int32u BitRate=(int32u)(Samples*Channels*Resolution*1000/PlayTime*CompressionRatio);

        Stream_Prepare(Stream_General);
        Fill("Format", "APE");
        Stream_Prepare(Stream_Audio);
        Fill("Codec", "APE");
        Fill("Resolution", Resolution);
        Fill("Channel(s)", Channels);
        Fill("SamplingRate", SampleRate);
        Fill("PlayTime", PlayTime);
        Fill("CompressionRatio", CompressionRatio);
        Fill("BitRate", BitRate);

        File__Tags_Helper::Data_GoTo(File_Size, "APE");
    FILLING_END();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ape::HowTo(stream_t StreamKind)
{
         if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
        General[0](_T("BitRate"), Info_HowTo)=_T("R");
        General[0](_T("PlayTime"), Info_HowTo)=_T("R");
        General[0](_T("Author"), Info_HowTo)=_T("R");
        General[0](_T("Album"), Info_HowTo)=_T("R");
        General[0](_T("Track"), Info_HowTo)=_T("R");
        General[0](_T("Comment"), Info_HowTo)=_T("R");
    }
    else if (StreamKind==Stream_Audio)
    {
        Audio[0](_T("BitRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Channel(s)"), Info_HowTo)=_T("R");
        Audio[0](_T("SamplingRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Codec"), Info_HowTo)=_T("R");
    }
}

} //NameSpace

#endif //MEDIAINFO_APE_YES

