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
#include <MediaInfo/Setup.h>
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
    const int32u AVI__hdlr_strl_strh_txts=0x74787473;
    const int32u FORM=0x464F524D;
    const int32u LIST=0x4C495354;
    const int32u MThd=0x4D546864;
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
            if (StreamKind_Last==Stream_Video)
            {
                Codec_Temp=Retrieve(Stream_Video, 0, Video_Codec); //We want to keep the 4CC of AVI
                FrameRate_Temp=Retrieve(Stream_Video, 0, Video_FrameRate); //We want to keep the FrameRate of AVI 120 fps
            }
            Merge(*Temp->second.Parser, Temp->second.StreamKind, 0, Temp->second.StreamPos);
            if (StreamKind_Last==Stream_Video)
            {
                if (!Codec_Temp.empty())
                    Fill(Stream_Video, StreamPos_Last, Video_Codec, Codec_Temp, true);
                if (FrameRate_Temp!=Retrieve(Stream_Video, 0, Video_FrameRate))
                    Fill(Stream_Video, 0, Video_FrameRate_Original, Retrieve(Stream_Video, 0, Video_FrameRate), true);
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

            //Specific
            #if defined(MEDIAINFO_MPEG4V_YES)
                if (StreamKind_Last==Stream_Video && MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Temp->second.Compression), InfoCodec_KindofCodec).find(_T("MPEG-4V"))==0)
                {
                    if (((File_Mpeg4v*)Temp->second.Parser)->RIFF_VOP_Count_Max>1)
                    {
                        Fill(Stream_Video, StreamPos_Last, Video_Format_Settings_MuxingMode, "Packet Bitstream");
                        Fill(Stream_Video, StreamPos_Last, Video_Format_Settings, "Packed Bitstream");
                        Fill(Stream_Video, StreamPos_Last, Video_Codec_Settings_PacketBitStream, "Yes");
                        Fill(Stream_Video, StreamPos_Last, Video_Codec_Settings, "Packed Bitstream");
                    }
                    else
                    {
                        Fill(Stream_Video, StreamPos_Last, Video_Format_Settings_PacketBitStream, "No");
                        Fill(Stream_Video, StreamPos_Last, Video_Codec_Settings_PacketBitStream, "No");
                    }
                }
            #endif
            #if defined(MEDIAINFO_MPEGA_YES)
                if (StreamKind_Last==Stream_Audio && MediaInfoLib::Config.Codec_Get(Ztring().From_Number(Temp->second.Compression, 16), InfoCodec_KindofCodec).find(_T("MPEG-"))==0 && Temp->second.Parser->Count_Get(Stream_General)>0)
                {
                    if (((File_Mpega*)Temp->second.Parser)->Delay>100 && Temp->second.Rate!=0)
                    {
                        Fill(Stream_Audio, StreamPos_Last, Audio_Delay, (float)((File_Mpega*)Temp->second.Parser)->Delay*1000/Temp->second.Rate, 10, true);
                        Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
                    }
                }
            #endif
            #if defined(MEDIAINFO_AC3_YES)
                if (StreamKind_Last==Stream_Audio && Temp->second.Compression==0x2000 && Temp->second.Parser->Count_Get(Stream_General)>0)
                {
                    if (((File_Ac3*)Temp->second.Parser)->Delay>100 && Temp->second.Rate!=0)
                    {
                        Fill(Stream_Audio, StreamPos_Last, Audio_Delay, ((File_Ac3*)Temp->second.Parser)->Delay*1000/Temp->second.Rate, 10, true);
                        Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
                    }
                }
            #endif
            #if defined(MEDIAINFO_DTS_YES)
                if (StreamKind_Last==Stream_Audio && (Temp->second.Compression==0x2001 || Temp->second.Compression==0x1) && Temp->second.Parser->Count_Get(Stream_General)>0)
                {
                    if (((File_Dts*)Temp->second.Parser)->Delay>100 && Temp->second.Rate!=0)
                    {
                        Fill(Stream_Audio, StreamPos_Last, Audio_Delay, ((File_Dts*)Temp->second.Parser)->Delay*1000/Temp->second.Rate, 10, true);
                        Fill(Stream_Video, 0, Video_Delay, 0, 10, true);
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
            Merge(*DV_FromHeader);
            Fill(Stream_Video, 0, Video_Format, "Digital Video");
            Fill(Stream_Video, 0, Video_Codec_CC, "dvsd");
            Fill(Stream_Audio, 0, Audio_Format, "PCM");
            Fill(Stream_Audio, 0, Audio_Codec_CC, "PCM");
        }
        #endif

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
        Stream_Pos.clear();
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

    //Filling
    Header_Fill_Code(Name, Ztring().From_CC4(Name));
    if (Size%2==1)
    {
        Size++; //Always 2-byte aligned
        Alignement_ExtraByte=true;
    }
    else
        Alignement_ExtraByte=false;
    Header_Fill_Size(8+Size);
}

//---------------------------------------------------------------------------
bool File_Riff::BookMark_Needed()
{
    //For each stream
    std::map<int32u, stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        if (!Temp->second.Parser && Temp->second.fccType!=Elements::AVI__hdlr_strl_strh_txts)
            AVI__movi_StreamClear(Temp->first);
        Temp++;
    }

    //Go to the first usefull chunk
    if (stream_Count==0 && Stream_Pos.empty())
        return false; //No need

    if (!Stream_Pos.empty())
        File_GoTo=Stream_Pos.begin()->first;
    NeedOldIndex=false;
    SecondPass=true;
    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Riff::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Duration", "R");
            Fill_HowTo("Movie", "R|INAM");
            Fill_HowTo("Track", "N|INAM");
            Fill_HowTo("Track/Position", "N|IPRT");
            Fill_HowTo("Album/Track_Total", "N|IFRM");
            Fill_HowTo("Composer", "R|IMUS");
            Fill_HowTo("WrittenBy", "R|IWRI");
            Fill_HowTo("Director", "R|IART");
            Fill_HowTo("DirectorOfPhotography", "R|ICNM");
            Fill_HowTo("EditedBy", "R|IEDT");
            Fill_HowTo("Producer", "R|IPRO");
            Fill_HowTo("ProductionDesigner", "R|IPDS");
            Fill_HowTo("CostumeDesigner", "R|ICDS");
            Fill_HowTo("MasteredBy", "R|IENG");
            Fill_HowTo("ProductionStudio", "R|ISTD");
            Fill_HowTo("DistributedBy", "R|IDST");
            Fill_HowTo("EncodedBy", "R|ITCH");
            Fill_HowTo("CommissionedBy", "R|ICMS");
            Fill_HowTo("Encoded_Original/DistributedBy", "R|ISRC");
            Fill_HowTo("Subject", "R|ISBJ");
            Fill_HowTo("Keywords", "R|IKEY");
            Fill_HowTo("LawRating", "R|IRTD");
            Fill_HowTo("Language", "R|ILNG");
            Fill_HowTo("Medium", "R|IMED");
            Fill_HowTo("Product", "R|IPRD");
            Fill_HowTo("Country", "R|ICNT");
            Fill_HowTo("Written_Date", "R|ICRD");
            Fill_HowTo("Mastered_Date", "R|IDIT");
            Fill_HowTo("Archival_Location", "R|IARL");
            Fill_HowTo("Genre", "R|IGNR");
            Fill_HowTo("Comment", "R|ICMT");
            Fill_HowTo("Encoded_Application", "R|ISFT");
            Fill_HowTo("Encoded_Original", "R|ISRF");
            Fill_HowTo("Copyright", "R|ICOP");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("FrameRate", "R");
            Fill_HowTo("FrameCount", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("DisplayAspectRatio", "R");
            Fill_HowTo("BitRate", "R");
            break;
        case (Stream_Audio) :
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Codec", "R");
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

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_RIFF_YES
