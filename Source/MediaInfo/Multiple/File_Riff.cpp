// File_Riff - Info for RIFF files
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_RIFF_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Riff.h"
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_DVDIF_YES)
    #include "MediaInfo/Multiple/File_DvDif.h"
#endif
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

namespace Elements
{
    const int32u AVI_=0x41564920;
    const int32u AVI__hdlr_strl_strh_txts=0x74787473;
    const int32u FORM=0x464F524D;
    const int32u LIST=0x4C495354;
    const int32u MThd=0x4D546864;
    const int32u ON2_=0x4F4E3220;
    const int32u ON2f=0x4F4E3266;
    const int32u RIFF=0x52494646;
    const int32u W3DI=0x57334449;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Riff::File_Riff()
:File__Analyze()
{
    //Configuration
    DataMustAlwaysBeComplete=false;

    //Data
    Interleaved0_1=0;
    Interleaved0_10=0;
    Interleaved1_1=0;
    Interleaved1_10=0;

    //Temp
    avih_FrameRate=0;
    avih_TotalFrame=0;
    dmlh_TotalFrame=0;
    Idx1_Offset=(int64u)-1;
    movi_Size=0;
    stream_Count=0;
    rec__Present=false;
    NeedOldIndex=true;
    IsBigEndian=false;
    SecondPass=false;
    DV_FromHeader=NULL;

    //Pointers
    Stream_Structure_Temp=Stream_Structure.end();
}

//---------------------------------------------------------------------------
File_Riff::~File_Riff()
{
    #ifdef MEDIAINFO_DVDIF_YES
        delete (File_DvDif*)DV_FromHeader; //DV_FromHeader=NULL
    #endif //MEDIAINFO_DVDIF_YES
}

//---------------------------------------------------------------------------
void File_Riff::Read_Buffer_Finalize ()
{
    //For each stream
    std::map<int32u, stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        //Preparing
        StreamKind_Last=Temp->second.StreamKind;
        StreamPos_Last=Temp->second.StreamPos;

        //StreamSize
        if (Temp->second.StreamSize>0)
            Fill(StreamKind_Last, StreamPos_Last, "StreamSize", Temp->second.StreamSize);

        //Parser specific
        if (Temp->second.Parser)
        {
            //Finalizing and Merging (except Video codec and 120 fps hack)
            Open_Buffer_Finalize(Temp->second.Parser);
            Ztring Codec_Temp;
            Ztring FrameRate_Temp;

            //Hack - Before
            if (StreamKind_Last==Stream_Video)
            {
                Codec_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_Codec); //We want to keep the 4CC of AVI
                FrameRate_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate); //We want to keep the FrameRate of AVI 120 fps
            }

            //Merging
            Merge(*Temp->second.Parser, StreamKind_Last, 0, StreamPos_Last);

            //Hacks - After
            if (StreamKind_Last==Stream_Video)
            {
                if (!Codec_Temp.empty())
                    Fill(Stream_Video, StreamPos_Last, Video_Codec, Codec_Temp, true);
                if (FrameRate_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate))
                    Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Original, Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate), true);
                if (!FrameRate_Temp.empty())
                    Fill(Stream_Video, StreamPos_Last, Video_FrameRate, FrameRate_Temp, true);

                //120 fps hack
                int32u FrameRate=FrameRate_Temp.To_int32u();
                if (FrameRate==120)
                {
                    Fill(Stream_Video, StreamPos_Last, Video_FrameRate_String, MediaInfoLib::Config.Language_Get(FrameRate_Temp+_T(" (24/30)"), _T(" fps")));
                    Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Minimum, 24, 10, true);
                    Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Maximum, 30, 10, true);
                    Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Mode, "VFR");
                }
            }

            //Alignment
            if (StreamKind_Last==Stream_Audio)
            {
                if (Count_Get(Stream_Video)>0) //Only if this is not a WAV file
                    Fill(Stream_Audio, StreamPos_Last, Audio_Alignment, Temp->second.ChunksAreComplete?"Aligned":"Split");
                    Fill(Stream_Audio, StreamPos_Last, Audio_Alignment_String, MediaInfoLib::Config.Language_Get(Temp->second.ChunksAreComplete?_T("Alignment_Aligned"):_T("Alignment_Split")));
            }

            //Delay
            if (StreamKind_Last==Stream_Audio && Count_Get(Stream_Video)==1 && Temp->second.Rate!=0 && Temp->second.Parser->Count_Get(Stream_General)>0)
            {
                int64u Delay=(int64u)-1;
                #if defined(MEDIAINFO_MPEGA_YES)
                    if (MediaInfoLib::Config.Codec_Get(Ztring().From_Number(Temp->second.Compression, 16), InfoCodec_KindofCodec).find(_T("MPEG-"))==0)
                        Delay=((File_Mpega*)Temp->second.Parser)->Delay;
                #endif
                #if defined(MEDIAINFO_AC3_YES)
                    if (Temp->second.Compression==0x2000)
                        Delay=((File_Ac3*)Temp->second.Parser)->Delay;
                #endif
                #if defined(MEDIAINFO_DTS_YES)
                    if (Temp->second.Compression==0x2001 || Temp->second.Compression==0x1)
                        Delay=((File_Dts*)Temp->second.Parser)->Delay;
                #endif
                if (Delay==0)
                {
                    Fill(Stream_Audio, StreamPos_Last, Audio_Delay, 0, 0, true);
                    Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
                }
                else if (Delay!=(int64u)-1)
                {
                    if (Temp->second.Rate!=0)
                    {
                        Fill(Stream_Audio, StreamPos_Last, Audio_Delay, ((float)Delay)*1000/Temp->second.Rate, 0, true);
                        Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
                    }
                    else if (Temp->second.Parser->Retrieve(Stream_Audio, 0, Audio_BitRate).To_int64u()!=0)
                    {
                        Fill(Stream_Audio, StreamPos_Last, Audio_Delay, ((float)Delay)*1000/Temp->second.Parser->Retrieve(Stream_Audio, 0, Audio_BitRate).To_int64u(), 0, true);
                        Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
                    }
                    else if (Temp->second.Parser->Retrieve(Stream_Audio, 0, Audio_BitRate_Nominal).To_int64u()!=0)
                    {
                        Fill(Stream_Audio, StreamPos_Last, Audio_Delay, ((float)Delay)*1000/Temp->second.Parser->Retrieve(Stream_Audio, 0, Audio_BitRate_Nominal).To_int64u(), 0, true);
                        Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
                    }
                }
            }

            //Format specific
            #if defined(MEDIAINFO_MPEG4V_YES)
                if (StreamKind_Last==Stream_Video && MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Temp->second.Compression), InfoCodec_KindofCodec).find(_T("MPEG-4V"))==0)
                {
                    if (((File_Mpeg4v*)Temp->second.Parser)->Frame_Count_InThisBlock>1)
                    {
                        Fill(Stream_Video, StreamPos_Last, Video_MuxingMode, MediaInfoLib::Config.Language_Get("MuxingMode_PackedBitstream"));
                        Fill(Stream_Video, StreamPos_Last, Video_Codec_Settings, "Packed Bitstream");
                        Fill(Stream_Video, StreamPos_Last, Video_Codec_Settings_PacketBitStream, "Yes");
                    }
                    else
                    {
                        Fill(Stream_Video, StreamPos_Last, Video_Codec_Settings_PacketBitStream, "No");
                    }
                }
            #endif
            #if defined(MEDIAINFO_DVDIF_YES)
                if (StreamKind_Last==Stream_Video && (MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Temp->second.Compression), InfoCodec_KindofCodec).find(_T("DV"))==0
                                                   || Retrieve(Stream_Video, StreamPos_Last, Video_Format)==_T("Digital Video")
                                                   || Retrieve(Stream_Video, StreamPos_Last, Video_Codec)==_T("DV")))
                {
                    if (Retrieve(Stream_General, 0, General_Recorded_Date).empty())
                        Fill(Stream_General, 0, General_Recorded_Date, Temp->second.Parser->Retrieve(Stream_General, 0, General_Recorded_Date));
                    for (size_t Pos=0; Pos<Temp->second.Parser->Count_Get(Stream_Audio); Pos++)
                    {
                        Stream_Prepare(Stream_Audio);
                        Open_Buffer_Finalize(Temp->second.Parser);
                        Merge(*Temp->second.Parser, Stream_Audio, Pos, StreamPos_Last);
                    }
                }
            #endif
        }
        #if defined(MEDIAINFO_DVDIF_YES)
        if (StreamKind_Last==Stream_Video && DV_FromHeader && Retrieve(Stream_Video, StreamPos_Last, Video_Format).empty() && Retrieve(Stream_Video, StreamPos_Last, Video_Codec).empty()) //Sometimes, there is a problem with the parser
        {
            Clear(Stream_Video);
            Clear(Stream_Audio);
            Open_Buffer_Finalize(DV_FromHeader);
            Merge(*DV_FromHeader);
            Fill(Stream_Video, 0, Video_Format, "Digital Video");
            Fill(Stream_Video, 0, Video_Codec_CC, "dvsd");
            Fill(Stream_Audio, 0, Audio_Format, "PCM");
            Fill(Stream_Audio, 0, Audio_Codec_CC, "PCM");
        }
        #endif

        //Duration
        if (Temp->second.PacketCount>0)
        {
            if (StreamKind_Last==Stream_Video)
            {
                //Duration in case it is missing from header (malformed header...)
                float32 FrameRate=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate).To_float32();
                if (FrameRate>0)
                    Fill(Stream_Video, StreamPos_Last, Video_Duration, Temp->second.PacketCount*1000/FrameRate, 0, true);
            }
            if (StreamKind_Last==Stream_Audio)
            {
                //Duration in case it is missing from header (malformed header...)
                int64u SamplingCount=0;
                if (Retrieve(Stream_Audio, StreamPos_Last, Audio_Format)==_T("MPEG Audio"))
                {
                    if (Temp->second.Parser && Temp->second.PacketPos==((File_Mpega*)Temp->second.Parser)->Frame_Count_Valid) //Only for stream with one frame per chunk
                    {
                        if (Retrieve(Stream_Audio, StreamPos_Last, Audio_Format_Profile)==_T("Layer 1"))
                            SamplingCount=Temp->second.PacketCount*384;  //Layer 1
                        else
                            SamplingCount=Temp->second.PacketCount*1152; //Layer 2 and 3
                    }
                }
                if (Retrieve(Stream_Audio, StreamPos_Last, Audio_Format)==_T("PCM"))
                {
                    int64u Resolution=Retrieve(Stream_Audio, StreamPos_Last, Audio_Resolution).To_int64u();
                    int64u Channels=Retrieve(Stream_Audio, StreamPos_Last, Audio_Channel_s_).To_int64u();
                    if (Resolution>0 && Channels>0)
                        SamplingCount=Temp->second.StreamSize*8/Resolution/Channels;
                }
                if (Retrieve(Stream_Audio, StreamPos_Last, Audio_Format)==_T("ADPCM"))
                {
                    int64u Resolution=Retrieve(Stream_Audio, StreamPos_Last, Audio_Resolution).To_int64u();
                    int64u Channels=Retrieve(Stream_Audio, StreamPos_Last, Audio_Channel_s_).To_int64u();
                    if (Resolution>0 && Channels>0)
                        SamplingCount=(int64u)(Temp->second.StreamSize*8/Resolution/Channels*0.98); //0.98 is not precise!

                    //ADPCM estimation is not precise, if container sampling count is around our value, using it rather than the estimation
                    float32 SamplingRate=Retrieve(Stream_Audio, StreamPos_Last, Audio_SamplingRate).To_float32();
                    if (SamplingRate>0
                     && SamplingCount*1000/SamplingRate<((float32)avih_TotalFrame)/avih_FrameRate*1000*1.10
                     && SamplingCount*1000/SamplingRate>((float32)avih_TotalFrame)/avih_FrameRate*1000*0.10)
                        SamplingCount=0; //Value disabled
                }
                //One AC-3 frame is 32 ms
                //One DTS frame is 21 ms

                float32 SamplingRate=Retrieve(Stream_Audio, StreamPos_Last, Audio_SamplingRate).To_float32();
                if (SamplingCount>0 && SamplingRate>0)
                    Fill(Stream_Audio, StreamPos_Last, Audio_Duration, SamplingCount*1000/SamplingRate, 0, true);

                //Interleave
                if (Stream[0x30300000].PacketCount && Temp->second.PacketCount)
                {
                    Fill(Stream_Audio, StreamPos_Last, "Interleave_VideoFrames", (float)Stream[0x30300000].PacketCount/Temp->second.PacketCount, 2);
                    if (Retrieve(Stream_Video, 0, Video_FrameRate).To_float32())
                    {
                        Fill(Stream_Audio, StreamPos_Last, "Interleave_Duration", (float)Stream[0x30300000].PacketCount/Temp->second.PacketCount*1000/Retrieve(Stream_Video, 0, Video_FrameRate).To_float32(), 0);
                        Fill(Stream_Audio, StreamPos_Last, "Interleave_Duration/String", Retrieve(Stream_Audio, StreamPos_Last, "Interleave_Duration")+_T(" ")+MediaInfoLib::Config.Language_Get(_T("ms"))+(Retrieve(Stream_Audio, StreamPos_Last, "Interleave_VideoFrames").empty()?Ztring():(_T(" (")+MediaInfoLib::Config.Language_Get(Retrieve(Stream_Audio, StreamPos_Last, "Interleave_VideoFrames"), _T(" video frames"))+_T(")"))), 0);
                    }
                    int64u Audio_FirstBytes=0;
                    for (std::map<int64u, stream_structure>::iterator Stream_Structure_Temp=Stream_Structure.begin(); Stream_Structure_Temp!=Stream_Structure.end(); Stream_Structure_Temp++)
                    {
                        if (Stream_Structure_Temp->second.Name==0x30300000)
                            break;
                        if (Stream_Structure_Temp->second.Name==Temp->first)
                            Audio_FirstBytes+=Stream_Structure_Temp->second.Size;
                    }
                    if (Audio_FirstBytes && Retrieve(Stream_Audio, StreamPos_Last, Audio_BitRate).To_int32u())
                    {
                        Fill(Stream_Audio, StreamPos_Last, "Interleave_Preload", Audio_FirstBytes*8*1000/Retrieve(Stream_Audio, StreamPos_Last, Audio_BitRate).To_int32u());
                        Fill(Stream_Audio, StreamPos_Last, "Interleave_Preload/String", Retrieve(Stream_Audio, StreamPos_Last, "Interleave_Preload")+_T(" ms"));
                    }
                }
            }
        }
        
        Temp++;
    }

    //Some work on the first video stream
    if (Count_Get(Stream_Video))
    {
        //ODML
        if (dmlh_TotalFrame!=0)
        {
            Fill(Stream_Video, 0, Video_FrameCount, dmlh_TotalFrame, 10, true);
            float32 FrameRate=Retrieve(Stream_Video, 0, Video_FrameRate).To_float32();
            if (FrameRate!=0)
                Fill(Stream_Video, 0, Video_Duration, (int64u)(dmlh_TotalFrame*1000/FrameRate), 10, true);
        }
    }

    //Rec
    if (rec__Present)
        Fill(Stream_General, 0, General_Format_Settings, "rec");

    //Interleaved
    if (Interleaved0_1 && Interleaved0_10 && Interleaved1_1 && Interleaved1_10)
        Fill(Stream_General, 0, General_Interleaved, (Interleaved0_1<Interleaved1_1 && Interleaved0_10>Interleaved1_1
                                              || Interleaved1_1<Interleaved0_1 && Interleaved1_10>Interleaved0_1)?"Yes":"No");

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
    {
        Stream.clear();
        Stream_Structure.clear();
        delete DV_FromHeader; DV_FromHeader=NULL;
    }
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Riff::Header_Parse()
{
    //Special case : W3DI tags (unknown format!) are at the end of the file
    if (Element_Level==2 && File_Offset+Buffer_Size==File_Size && Buffer_Size>8)
    {
        if (CC4(Buffer+Buffer_Size-4)==Elements::W3DI)
        {
            int32u Size=LittleEndian2int32u(Buffer+Buffer_Size-8);
            if (Size>8 && Size<=Buffer_Size && Buffer_Offset+Size==Buffer_Size)
            {
                //Filling
                Header_Fill_Code(Elements::W3DI, "W3DI");
                Header_Fill_Size(Size);
                return;
            }
        }
    }

    //Parsing
    int32u Size, Name;
    Get_C4 (Name,                                               "Name");
    if (Name==Elements::FORM
     || Name==Elements::MThd)
        IsBigEndian=true; //Swap from Little to Big Endian for "FORM" files (AIFF...)
    if (IsBigEndian)
        Get_B4 (Size,                                           "Size");
    else
        Get_L4 (Size,                                           "Size");
    if (Name==Elements::LIST
     || Name==Elements::RIFF
     || Name==Elements::ON2_
     || Name==Elements::FORM)
        Get_C4 (Name,                                           "Real Name");

    //Integrity
    if (Name==0x00000000)
    {
        //Filling
        Header_Fill_Code(0, "Junk");
        Header_Fill_Size(File_Size-(File_Offset+Buffer_Offset));
        Alignement_ExtraByte=false;
        return;
    }

    //Specific
    if (Name==Elements::ON2f)
        Name=Elements::AVI_;

    //Filling
    Header_Fill_Code(Name, Ztring().From_CC4(Name));
    int64u Size_Complete=Size;
    if (Size_Complete%2==1)
    {
        Size_Complete++; //Always 2-byte aligned
        Alignement_ExtraByte=true;
    }
    else
        Alignement_ExtraByte=false;
    Header_Fill_Size(Size_Complete+8);
}

//---------------------------------------------------------------------------
bool File_Riff::BookMark_Needed()
{
    //Go to the first usefull chunk
    if (stream_Count==0 && Stream_Structure.empty())
        return false; //No need

    Stream_Structure_Temp=Stream_Structure.begin();
    if (!Stream_Structure.empty())
        File_GoTo=Stream_Structure_Temp->first;
    NeedOldIndex=false;
    SecondPass=true;
    Index_Pos.clear(); //We didn't succeed to find theses indexes :(
    return true;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_RIFF_YES
