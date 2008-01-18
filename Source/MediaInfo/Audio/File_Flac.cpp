// File_Flac - Info for Flac files
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
#if defined(MEDIAINFO_FLAC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Flac.h"
#include "MediaInfo/Tag/File_VorbisCom.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Flac::File_Flac()
:File__Analyze()
{
    Last_metadata_block=false;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::Header_Parse()
{
    //Parsing
    int32u Length;
    int8u BLOCK_TYPE;
    BS_Begin();
    Get_SB (   Last_metadata_block,                             "Last-metadata-block");
    Get_S1 (7, BLOCK_TYPE,                                      "BLOCK_TYPE");
    BS_End();
    Get_B3 (Length,                                             "Length");

    //Filling
    Header_Fill_Code(BLOCK_TYPE, Ztring().From_CC1(BLOCK_TYPE));
    Header_Fill_Size(Element_Offset+Length);
}

//---------------------------------------------------------------------------
void File_Flac::FileHeader_Parse ()
{
    //Parsing
    int32u Header;
    Get_C4 (Header,                                             "Header");

    FILLING_BEGIN();
        if (Header!=CC4("fLaC"))
            Finnished();
    FILLING_END();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::Data_Parse()
{
    #define ELEMENT_CASE(_NAME) \
        case Flac::_NAME : _NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(STREAMINFO);
        ELEMENT_CASE(PADDING);
        ELEMENT_CASE(APPLICATION);
        ELEMENT_CASE(SEEKTABLE);
        ELEMENT_CASE(VORBIS_COMMENT);
        ELEMENT_CASE(CUESHEET);
        ELEMENT_CASE(PICTURE);
        default : Skip_XX(Element_Size,                         "Data");
    }

    if (Last_metadata_block)
        Finnished();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::STREAMINFO()
{
    Element_Info("STREAMINFO");

    //Parsing
    int64u Samples;
    int32u FrameSize_Min, FrameSize_Max, SampleRate;
    int8u  Channels, BitPerSample;
    Skip_B2(                                                    "BlockSize_Min"); //The minimum block size (in samples) used in the stream.
    Skip_B2(                                                    "BlockSize_Max"); //The maximum block size (in samples) used in the stream. (Minimum blocksize == maximum blocksize) implies a fixed-blocksize stream.
    Get_B3 (    FrameSize_Min,                                  "FrameSize_Min"); //The minimum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
    Get_B3 (    FrameSize_Max,                                  "FrameSize_Max"); //The maximum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
    BS_Begin();
    Get_S3 (20, SampleRate,                                     "SampleRate"); //Sample rate in Hz. Though 20 bits are available, the maximum sample rate is limited by the structure of frame headers to 1048570Hz. Also, a value of 0 is invalid.
    Get_S1 ( 3, Channels,                                       "Channels"); Param_Info(Channels+1, " channels"); //(number of channels)-1. FLAC supports from 1 to 8 channels
    Get_S1 ( 5, BitPerSample,                                   "BitPerSample"); Param_Info(BitPerSample+1, " bits"); //(bits per sample)-1. FLAC supports from 4 to 32 bits per sample. Currently the reference encoder and decoders only support up to 24 bits per sample.
    Get_S5 (36, Samples,                                        "Samples");
    BS_End();
    Skip_B16(                                                   "MD5 signature of the unencoded audio data");

    //Filling
    if (SampleRate==0)
        return;
    Stream_Prepare(Stream_General);
    Fill("Format", "FLAC");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "FLAC");
    if (FrameSize_Min==FrameSize_Max && FrameSize_Min!=0 ) // 0 means it is unknown
        Fill("BitRate_Mode", "CBR");
     else
        Fill("BitRate_Mode", "VBR");
    Fill("SamplingRate", SampleRate);
    Fill("Channel(s)", Channels);
    Fill("Resolution", BitPerSample+1);
    Fill(Stream_General, 0, "PlayTime", Samples*1000/SampleRate);
}

//---------------------------------------------------------------------------
void File_Flac::PADDING()
{
    Element_Info("PADDING");

    //Parsing
    Skip_XX(Element_Size,                                       "Padding");
}

//---------------------------------------------------------------------------
void File_Flac::APPLICATION()
{
    Element_Info("APPLICATION");

    //Parsing
    Skip_C4(                                                    "Application");
    if (Element_Size>4)
        Skip_XX(Element_Size-4,                                 "(Application specific)");
}

//---------------------------------------------------------------------------
void File_Flac::SEEKTABLE()
{
    Element_Info("SEEKTABLE");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Flac::VORBIS_COMMENT()
{
    Element_Info("VORBIS_COMMENT");

    //Parsing
    File_VorbisCom VorbisCom;
    VorbisCom.StreamKind=Stream_Audio;
    Open_Buffer_Init(&VorbisCom, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(&VorbisCom, Buffer+Buffer_Offset, (size_t)Element_Size);
    Merge(VorbisCom, Stream_General, 0, 0);
}

//---------------------------------------------------------------------------
void File_Flac::CUESHEET()
{
    Element_Info("CUESHEET");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Flac::PICTURE()
{
    Element_Info("PICTURE");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

//***************************************************************************
// Helpers
//***************************************************************************

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::HowTo(stream_t StreamKind)
{
         if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
        General[0](_T("BitRate"), Info_HowTo)=_T("R");
        General[0](_T("Title"), Info_HowTo)=_T("R");
        General[0](_T("Title/More"), Info_HowTo)=_T("R");
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

#endif //MEDIAINFO_FLAC_YES

