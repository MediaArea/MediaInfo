// File__Base_Inform - Base for other files
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
//
// Init and Finalize part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <ZenLib/Utils.h>
#include <ZenLib/File.h>
#include <ZenLib/FileName.h>
#include "MediaInfo/File__Analyze.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Init
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::General_Fill()
{
    //Coherancy
    if (Count_Get(Stream_General)==0)
        return;

    //FileName and FileSize
    //if (File_Name.size()>0 && (General[0](_T("CompleteName")).empty() || General[0](_T("FileSize")).empty())) //Do it even if already, some merges can modify it
    if (File_Name.size()>0)
    {
        //FileName
        General[0](_T("CompleteName"))=File_Name;
        General[0](_T("FolderName"))=FileName::Path_Get(File_Name);
        General[0](_T("FileName"))=FileName::Name_Get(File_Name);
        General[0](_T("FileExtension"))=FileName::Extension_Get(File_Name).MakeLowerCase();

        //FileSize
        File F(File_Name);
        int64u FileSize=F.Size_Get();
        if (FileSize)
            General[0](_T("FileSize")).From_Number(FileSize);
        General[0](_T("File_Created_Date"))=F.Created_Get();
        General[0](_T("File_Modified_Date"))=F.Modified_Get();
    }
}

//***************************************************************************
// Finalize
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Finalize()
{
    General_Fill();

    //Generic
    //ELEMENT(0, "Finalizing (Generic)");

    //For all streams (Generic)
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        Finalize__All((stream_t)StreamKind);

    //For each kind of Stream
    for (size_t Pos=0; Pos<General.size();  Pos++) Finalize_General(Pos);
    for (size_t Pos=0; Pos<Video.size();    Pos++) Finalize_Video(Pos);
    for (size_t Pos=0; Pos<Audio.size();    Pos++) Finalize_Audio(Pos);
    for (size_t Pos=0; Pos<Text.size();     Pos++) Finalize_Text(Pos);
    for (size_t Pos=0; Pos<Chapters.size(); Pos++) Finalize_Chapters(Pos);
    for (size_t Pos=0; Pos<Image.size();    Pos++) Finalize_Image(Pos);
    for (size_t Pos=0; Pos<Menu.size();     Pos++) Finalize_Menu(Pos);

    //Final parsing
    Finalize_Final();

    //For all streams (Generic)
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        Finalize_Final_All((stream_t)StreamKind);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize__All(stream_t StreamKind)
{
    for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
        Finalize__All(StreamKind, Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize__All(stream_t StreamKind, size_t Pos)
{
    //BitRate
    if ((*Stream[StreamKind])[Pos](_T("BitRate")).empty() && !(*Stream[StreamKind])[Pos](_T("StreamSize")).empty() && !(*Stream[StreamKind])[Pos](_T("PlayTime")).empty())
    {
        int64u PlayTime=(*Stream[StreamKind])[Pos](_T("PlayTime")).To_int64u();
        int64u StreamSize=(*Stream[StreamKind])[Pos](_T("StreamSize")).To_int64u();
        if (PlayTime>0 && StreamSize>0)
            (*Stream[StreamKind])[Pos](_T("BitRate")).From_Number(StreamSize*8*1000/PlayTime);
    }

    //BitRate Nominal
    float32 BitRate=(*Stream[StreamKind])[Pos](_T("BitRate")).To_float32();
    float32 BitRate_Nominal=(*Stream[StreamKind])[Pos](_T("BitRate_Nominal")).To_float32();
    if (BitRate_Nominal>BitRate*0.995 && BitRate_Nominal<BitRate*1.005)
    {
        (*Stream[StreamKind])[Pos](_T("BitRate"))=(*Stream[StreamKind])[Pos](_T("BitRate_Nominal"));
        (*Stream[StreamKind])[Pos](_T("BitRate_Nominal")).clear();
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Final_All(stream_t StreamKind)
{
    Ztring Z1, Z2; //For Codec_List
    for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
        Finalize_Final_All(StreamKind, Pos, Z1, Z2);

    //Codec_List
    if (StreamKind!=Stream_General && !Z1.empty())
    {
        Z1.resize(Z1.size()-3); //Delete extra " / "
        Z2.resize(Z2.size()-3); //Delete extra " / "
        Ztring Z3=Get((stream_t)StreamKind, 0, _T("StreamKind"), Info_Measure);
        Fill(Stream_General, 0, Ztring(Z3+_T("_Codec_List")).To_Local().c_str(), Z1);
        Fill(Stream_General, 0, Ztring(Z3+_T("_Language_List")).To_Local().c_str(), Z2);
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Final_All(stream_t StreamKind, size_t Pos, Ztring &Z1, Ztring &Z2)
{
    if (StreamKind!=Stream_General)
    {
        //Codec
        if (!(*Stream[StreamKind])[Pos](_T("Codec")).empty())
        {
            const Ztring &C1=Config.Codec_Get((*Stream[StreamKind])[Pos](_T("Codec")), InfoCodec_Name, (stream_t)StreamKind);
            if (C1.empty())
            {
                if ((*Stream[StreamKind])[Pos](_T("Codec/String")).empty()) (*Stream[StreamKind])[Pos](_T("Codec/String"))=(*Stream[StreamKind])[Pos](_T("Codec"));
            }
            else
            {
                if ((*Stream[StreamKind])[Pos](_T("Codec/String")).empty()) (*Stream[StreamKind])[Pos](_T("Codec/String"))=C1;
                if ((*Stream[StreamKind])[Pos](_T("Codec/Family")).empty()) (*Stream[StreamKind])[Pos](_T("Codec/Family"))=Config.Codec_Get((*Stream[StreamKind])[Pos](_T("Codec")), InfoCodec_KindofCodec, (stream_t)StreamKind);
                if ((*Stream[StreamKind])[Pos](_T("Codec/Info"  )).empty()) (*Stream[StreamKind])[Pos](_T("Codec/Info"  ))=Config.Codec_Get((*Stream[StreamKind])[Pos](_T("Codec")), InfoCodec_Description, (stream_t)StreamKind);
                if ((*Stream[StreamKind])[Pos](_T("Codec/Url"   )).empty()) (*Stream[StreamKind])[Pos](_T("Codec/Url"   ))=Config.Codec_Get((*Stream[StreamKind])[Pos](_T("Codec")), InfoCodec_Url, (stream_t)StreamKind);
            }
        }
        //Special cases
        if ((*Stream[StreamKind])[Pos](_T("Channel(s)")).empty()
         &&((*Stream[StreamKind])[Pos](_T("Codec"))==_T("samr")
         || (*Stream[StreamKind])[Pos](_T("Codec"))==_T("sawb")
         || (*Stream[StreamKind])[Pos](_T("Codec"))==_T("7A21")
         || (*Stream[StreamKind])[Pos](_T("Codec"))==_T("7A22"))
            )
            (*Stream[StreamKind])[Pos](_T("Channel(s)")).From_Number(1); //AMR is always with 1 channel

        //Language
        //-Find 2-digit language
        (*Stream[StreamKind])[Pos](_T("Language")).MakeLowerCase();
        if ((*Stream[StreamKind])[Pos](_T("Language")).size()==3 && (*Stream[StreamKind])[Pos](_T("Language"))==_T("und"))
            (*Stream[StreamKind])[Pos](_T("Language")).clear();
        if ((*Stream[StreamKind])[Pos](_T("Language")).size()==3 && !Config.Iso639_Get((*Stream[StreamKind])[Pos](_T("Language"))).empty())
           (*Stream[StreamKind])[Pos](_T("Language"))=Config.Iso639_Get((*Stream[StreamKind])[Pos](_T("Language")));
        //-Translate
        Ztring Temp=_T("Language_"); Temp+=(*Stream[StreamKind])[Pos](_T("Language"));
        const Ztring& Z3=Config.Language_Get(Temp);
        if (!Z3.empty())
            (*Stream[StreamKind])[Pos](_T("Language/String"))=Z3;
        else
            (*Stream[StreamKind])[Pos](_T("Language/String"))=(*Stream[StreamKind])[Pos](_T("Language"));

        //Codec_List
        Z1+=(*Stream[StreamKind])[Pos](_T("Codec/String"))+_T(" / ");
        Z2+=(*Stream[StreamKind])[Pos](_T("Language/String"))+_T(" / ");
    }

    //Counts
    (*Stream[StreamKind])[Pos](_T("Count")).From_Number((*Stream[StreamKind])[Pos].size());
    (*Stream[StreamKind])[Pos](_T("StreamCount")).From_Number((*Stream[StreamKind]).size());
    (*Stream[StreamKind])[Pos](_T("StreamKindID")).From_Number(Pos);

    //Strings
    size_t List_Measure_Pos=Error;
    do
    {
        List_Measure_Pos=Config.Info_Get(StreamKind).Find_Filled(Info_Measure, List_Measure_Pos+1);
        if (List_Measure_Pos!=Error)
        {
            const Ztring &List_Measure_Value=Config.Info_Get(StreamKind).Read(List_Measure_Pos, Info_Measure);
            const Ztring &List_Name_Value=Config.Info_Get(StreamKind).Read(List_Measure_Pos, Info_Name);
                 if (List_Measure_Value==_T(" byte"))
                FileSize_FileSize123(List_Name_Value, StreamKind, Pos);
            else if (List_Measure_Value==_T(" bps") || List_Measure_Value==_T(" Hz"))
                Kilo_Kilo123(List_Name_Value, StreamKind, Pos);
            else if (List_Measure_Value==_T(" ms"))
                PlayTime_PlayTime123(List_Name_Value, StreamKind, Pos);
            else
                Value_Value123(List_Name_Value, StreamKind, Pos);
        }

    }
    while (List_Measure_Pos!=Error);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_General(size_t)
{
    //Integrity
    if (0>=General.size())
        return;

    //General, bases
    General_Fill(); //Not always done by File_*
    if (!Config.Format_Get(General[0](_T("Format")), InfoFormat_Name).empty())
    {
        Fill(Stream_General, 0, "Format/String", Config.Format_Get(General[0](_T("Format")), InfoFormat_LongName));
        Fill(Stream_General, 0, "Format/Family", Config.Format_Get(General[0](_T("Format")), InfoFormat_Family));
        Fill(Stream_General, 0, "Format/Info", Config.Format_Get(General[0](_T("Format")), InfoFormat_Info));
        Fill(Stream_General, 0, "Format/Url", Config.Format_Get(General[0](_T("Format")), InfoFormat_Url));
        Fill(Stream_General, 0, "Format/Extensions", Config.Format_Get(General[0](_T("Format")), InfoFormat_Extensions));
    }
    else if (Get(Stream_General, 0, "Format/String").empty())
        Fill(Stream_General, 0, "Format/String", General[0](_T("Format")));
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Video(size_t Pos)
{
    //Integrity
    if (Pos>=Video.size())
        return;

    //Interlacement
    if (!Video[Pos](_T("Interlacement")).empty())
    {
        Ztring &Z1=Video[Pos](_T("Interlacement"));
        if (Z1.size()==3)
            Video[Pos](_T("Interlacement/String"))=Config.Language_Get(Ztring(_T("Interlaced_"))+Z1);
        else
            Video[Pos](_T("Interlacement/String"))=Config.Language_Get(Z1);
        if (Video[Pos](_T("Interlacement/String")).empty())
            Video[Pos](_T("Interlacement/String"))=Z1;
    }
    //Display Aspect Ratio from Pixel Aspect Ratio
    if (Video[Pos](_T("DisplayAspectRatio")).empty() && !Video[Pos](_T("PixelAspectRatio")).empty())
    {
        float PAR   =Video[Pos](_T("PixelAspectRatio")).To_float32();
        float Width =Video[Pos](_T("Width"           )).To_float32();
        float Height=Video[Pos](_T("Height"          )).To_float32();
        if (PAR && Height && Width)
            Video[Pos](_T("DisplayAspectRatio")).From_Number(Width/Height*PAR);
    }
    //Pixel Aspect Ratio from Display Aspect Ratio
    if (Video[Pos](_T("PixelAspectRatio")).empty() && !Video[Pos](_T("DisplayAspectRatio")).empty())
    {
        float DAR   =Video[Pos](_T("DisplayAspectRatio")).To_float32();
        float Width =Video[Pos](_T("Width"             )).To_float32();
        float Height=Video[Pos](_T("Height"            )).To_float32();
        if (DAR && Height && Width)
            Video[Pos](_T("PixelAspectRatio")).From_Number(Width/Height/DAR);
    }
    //Display Aspect Ratio by default (thinking that PAR is 1.000)
    if (Video[Pos](_T("DisplayAspectRatio")).empty())
    {
        float Width =Video[Pos](_T("Width" )).To_float32();
        float Height=Video[Pos](_T("Height")).To_float32();
        if (Height && Width)
            Video[Pos](_T("DisplayAspectRatio")).From_Number(Width/Height);
    }
    //Display Aspect Ratio
    if (!Video[Pos](_T("DisplayAspectRatio")).empty())
    {
        float F1=Video[Pos](_T("DisplayAspectRatio")).To_float32();
        Ztring C1;
             if (0);
        else if (F1>1.23 && F1<1.27) C1=_T("5/4");
        else if (F1>1.30 && F1<1.37) C1=_T("4/3");
        else if (F1>1.70 && F1<1.85) C1=_T("16/9");
        else if (F1>2.10 && F1<2.22) C1=_T("2.2");
        else if (F1>2.23 && F1<2.30) C1=_T("2.25");
        else if (F1>2.30 && F1<2.40) C1=_T("2.35");
        else              C1.From_Number(F1);
        Video[Pos](_T("DisplayAspectRatio/String"))=C1;
    }
    //Standard
    if (Video[Pos](_T("Standard")).empty() && Video[Pos](_T("Width"))==_T("720"))
    {
             if (Video[Pos](_T("Height"))==_T("576"))
            Video[Pos](_T("Standard"))=_T("PAL");
        else if (Video[Pos](_T("Height"))==_T("480"))
            Video[Pos](_T("Standard"))=_T("NTSC");
    }
    //FrameRate Nominal
    float32 FrameRate=(*Stream[Stream_Video])[Pos](_T("FrameRate")).To_float32();
    float32 FrameRate_Nominal=(*Stream[Stream_Video])[Pos](_T("FrameRate_Nominal")).To_float32();
    if (FrameRate_Nominal>FrameRate*0.995 && FrameRate_Nominal<FrameRate*1.005)
    {
        (*Stream[Stream_Video])[Pos](_T("FrameRate"))=(*Stream[Stream_Video])[Pos](_T("FrameRate_Nominal"));
        (*Stream[Stream_Video])[Pos](_T("FrameRate_Nominal")).clear();
    }
    //Bits/(Pixel*Frame)
    if (!Video[Pos](_T("BitRate")).empty())
    {
        float F1=(float)Video[Pos](_T("Width")).To_int32s()*(float)Video[Pos](_T("Height")).To_int32s()*Video[Pos](_T("FrameRate")).To_float32();
        if (F1)
            Video[Pos](_T("Bits-(Pixel*Frame)")).From_Number(Video[Pos](_T("BitRate")).To_float32()/F1);
    }
    //FrameCount
    if (Video[Pos](_T("FrameCount")).empty())
    {
        int64s PlayTime=Video[Pos](_T("PlayTime")).To_int64s();
        if (PlayTime==0)
            PlayTime=General[0](_T("PlayTime")).To_int64s();
        float FrameRate=Video[Pos](_T("FrameRate")).To_float32();
        if (PlayTime && FrameRate)
           Video[Pos](_T("FrameCount")).From_Number(int64u(((float64)PlayTime)/1000*FrameRate));
    }
    //Playtime
    if (Video[Pos](_T("PlayTime")).empty())
    {
        int64u PlayTime=Video[Pos](_T("PlayTime")).To_int64u();
        int64u FrameRate=Video[Pos](_T("FrameRate")).To_int64u();
        if (PlayTime==0 && FrameRate!=0)
            PlayTime=Video[Pos](_T("FrameCount")).To_int64u()*1000/FrameRate;
        if (PlayTime)
           Video[Pos](_T("PlayTime")).From_Number(PlayTime);
    }
    //Encoded_Library
    if (!Video[Pos](_T("Encoded_Library")).empty())
    {
        Ztring Encoder=Video[Pos](_T("Encoded_Library"));
        const Ztring& Name=Config.Encoder_Get(Video[Pos](_T("Encoded_Library")));
        if (!Name.empty())
        {
            Video[Pos](_T("Encoded_Library/String"))=Name;
            Video[Pos](_T("Encoded_Library/Date"))=Config.Encoder_Get(Encoder, InfoEncoder_Date);
        }
        else
            Video[Pos](_T("Encoded_Library/String"))=Video[Pos](_T("Encoded_Library"));
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Audio(size_t Pos)
{
    //Integrity
    if (Pos>=Audio.size())
        return;

    //SamplingCount
    if (Audio[Pos](_T("SamplingCount")).empty())
    {
        int64s PlayTime=Audio[Pos](_T("PlayTime")).To_int64s();
        if (PlayTime==0)
            PlayTime=General[0](_T("PlayTime")).To_int64s();
        float SamplingRate=Audio[Pos](_T("SamplingRate")).To_float32();
        if (PlayTime && SamplingRate)
           Audio[Pos](_T("SamplingCount")).From_Number(int64u(((float64)PlayTime)/1000*SamplingRate));
    }
    //Playtime
    if (Audio[Pos](_T("PlayTime")).empty() && Audio[Pos](_T("SamplingRate")).To_int64s()!=0)
    {
        int64u PlayTime=Audio[Pos](_T("SamplingCount")).To_int64u()*1000/Audio[Pos](_T("SamplingRate")).To_int64u();
        if (PlayTime)
           Audio[Pos](_T("PlayTime")).From_Number(PlayTime);
    }
    //Delay/Video0
    if (Video.size()>0 && !Audio[Pos](_T("Delay")).empty())
    {
        Audio[Pos](_T("Video0_Delay")).From_Number(Audio[Pos](_T("Delay")).To_int32s()-Video[0](_T("Delay")).To_int32s());
    }
    //CBR/VBR
    if (Audio[Pos](_T("BitRate_Mode")).empty() && !Audio[Pos](_T("Codec")).empty())
    {
        Ztring Z1=Config.Codec_Get(Audio[Pos](_T("Codec")), InfoCodec_BitRate_Mode, Stream_Audio);
        if (!Z1.empty())
            Audio[Pos](_T("BitRate_Mode"))=Z1;
    }

    //Well known bitrate values
    Finalize_Audio_BitRate(Pos, _T("BitRate"));
    Finalize_Audio_BitRate(Pos, _T("BitRate_Nominal"));
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Audio_BitRate(size_t Pos, Char* Parameter)
{
    Ztring& Codec=Audio[Pos](_T("Codec"));
    int32u BitRate=Audio[Pos](Parameter).To_int32u();
    int32u BitRate_Sav=BitRate;
    if (Config.Codec_Get(Codec, InfoCodec_KindofCodec, Stream_Audio).find(_T("MPEG-"))==0
     || Audio[Pos](_T("Codec/String")).find(_T("MPEG-"))==0)
    {
        if (BitRate>=   7500 && BitRate<=   8500) BitRate=   8000;
        if (BitRate>=  15000 && BitRate<=  17000) BitRate=  16000;
        if (BitRate>=  23000 && BitRate<=  25000) BitRate=  24000;
        if (BitRate>=  31000 && BitRate<=  33000) BitRate=  32000;
        if (BitRate>=  38000 && BitRate<=  42000) BitRate=  40000;
        if (BitRate>=  46000 && BitRate<=  50000) BitRate=  48000;
        if (BitRate>=  54000 && BitRate<=  58000) BitRate=  56000;
        if (BitRate>=  62720 && BitRate<=  65280) BitRate=  64000;
        if (BitRate>=  78400 && BitRate<=  81600) BitRate=  80000;
        if (BitRate>=  94080 && BitRate<=  97920) BitRate=  96000;
        if (BitRate>= 109760 && BitRate<= 114240) BitRate= 112000;
        if (BitRate>= 125440 && BitRate<= 130560) BitRate= 128000;
        if (BitRate>= 156800 && BitRate<= 163200) BitRate= 160000;
        if (BitRate>= 156800 && BitRate<= 163200) BitRate= 160000;
        if (BitRate>= 188160 && BitRate<= 195840) BitRate= 192000;
        if (BitRate>= 219520 && BitRate<= 228480) BitRate= 224000;
        if (BitRate>= 219520 && BitRate<= 228480) BitRate= 224000;
        if (BitRate>= 250880 && BitRate<= 261120) BitRate= 256000;
        if (BitRate>= 282240 && BitRate<= 293760) BitRate= 288000;
        if (BitRate>= 313600 && BitRate<= 326400) BitRate= 320000;
        if (BitRate>= 344960 && BitRate<= 359040) BitRate= 352000;
        if (BitRate>= 376320 && BitRate<= 391680) BitRate= 384000;
        if (BitRate>= 407680 && BitRate<= 424320) BitRate= 416000;
        if (BitRate>= 439040 && BitRate<= 456960) BitRate= 448000;
        if (Audio[Pos](_T("BitRate_Mode"))==_T("VBR"))
            BitRate=BitRate_Sav; //If VBR, we want the exact value
    }

    else if (Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("AC3"))==0)
    {
        if (BitRate>=  31000 && BitRate<=  33000) BitRate=  32000;
        if (BitRate>=  39000 && BitRate<=  41000) BitRate=  40000;
        if (BitRate>=  46000 && BitRate<=  50000) BitRate=  48000;
        if (BitRate>=  54000 && BitRate<=  58000) BitRate=  56000;
        if (BitRate>=  62720 && BitRate<=  65280) BitRate=  64000;
        if (BitRate>=  78400 && BitRate<=  81600) BitRate=  80000;
        if (BitRate>=  94080 && BitRate<=  97920) BitRate=  96000;
        if (BitRate>= 109760 && BitRate<= 114240) BitRate= 112000;
        if (BitRate>= 125440 && BitRate<= 130560) BitRate= 128000;
        if (BitRate>= 156800 && BitRate<= 163200) BitRate= 160000;
        if (BitRate>= 188160 && BitRate<= 195840) BitRate= 192000;
        if (BitRate>= 219520 && BitRate<= 228480) BitRate= 224000;
        if (BitRate>= 250880 && BitRate<= 261120) BitRate= 256000;
        if (BitRate>= 313600 && BitRate<= 326400) BitRate= 320000;
        if (BitRate>= 376320 && BitRate<= 391680) BitRate= 384000;
        if (BitRate>= 439040 && BitRate<= 456960) BitRate= 448000;
        if (BitRate>= 501760 && BitRate<= 522240) BitRate= 512000;
        if (BitRate>= 564480 && BitRate<= 587520) BitRate= 576000;
        if (BitRate>= 627200 && BitRate<= 652800) BitRate= 640000;
  }

    else if (Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("DTS"))==0)
    {
        if (BitRate>=  31000 && BitRate<=  33000) BitRate=  32000;
        if (BitRate>=  54000 && BitRate<=  58000) BitRate=  56000;
        if (BitRate>=  62720 && BitRate<=  65280) BitRate=  64000;
        if (BitRate>=  94080 && BitRate<=  97920) BitRate=  96000;
        if (BitRate>= 109760 && BitRate<= 114240) BitRate= 112000;
        if (BitRate>= 125440 && BitRate<= 130560) BitRate= 128000;
        if (BitRate>= 188160 && BitRate<= 195840) BitRate= 192000;
        if (BitRate>= 219520 && BitRate<= 228480) BitRate= 224000;
        if (BitRate>= 250880 && BitRate<= 261120) BitRate= 256000;
        if (BitRate>= 313600 && BitRate<= 326400) BitRate= 320000;
        if (BitRate>= 376320 && BitRate<= 391680) BitRate= 384000;
        if (BitRate>= 439040 && BitRate<= 456960) BitRate= 448000;
        if (BitRate>= 501760 && BitRate<= 522240) BitRate= 512000;
        if (BitRate>= 564480 && BitRate<= 587520) BitRate= 576000;
        if (BitRate>= 627200 && BitRate<= 652800) BitRate= 640000;
        if (BitRate>= 752640 && BitRate<= 783360) BitRate= 768000;
        if (BitRate>= 940800 && BitRate<= 979200) BitRate= 960000;
        if (BitRate>=1003520 && BitRate<=1044480) BitRate=1024000;
        if (BitRate>=1128960 && BitRate<=1175040) BitRate=1152000;
        if (BitRate>=1254400 && BitRate<=1305600) BitRate=1280000;
        if (BitRate>=1317120 && BitRate<=1370880) BitRate=1344000;
        if (BitRate>=1379840 && BitRate<=1436160) BitRate=1408000;
        if (BitRate>=1382976 && BitRate<=1439424) BitRate=1411200;
        if (BitRate>=1442560 && BitRate<=1501440) BitRate=1472000;
        if (BitRate>=1505280 && BitRate<=1566720) BitRate=1536000;
        if (BitRate>=1881600 && BitRate<=1958400) BitRate=1920000;
        if (BitRate>=2007040 && BitRate<=2088960) BitRate=2048000;
        if (BitRate>=3010560 && BitRate<=3133440) BitRate=3072000;
        if (BitRate>=3763200 && BitRate<=3916800) BitRate=3840000;
    }

    else if (Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("AAC"))==0)
    {
        if (BitRate>=  46000 && BitRate<=  50000) BitRate=  48000;
        if (BitRate>=  64827 && BitRate<=  67473) BitRate=  66150;
        if (BitRate>=  70560 && BitRate<=  73440) BitRate=  72000;
        if (BitRate>=  94080 && BitRate<=  97920) BitRate=  96000;
        if (BitRate>=  94080 && BitRate<=  97920) BitRate=  96000;
        if (BitRate>= 129654 && BitRate<= 134946) BitRate= 132300;
        if (BitRate>= 141120 && BitRate<= 146880) BitRate= 144000;
        if (BitRate>= 188160 && BitRate<= 195840) BitRate= 192000;
        if (BitRate>= 259308 && BitRate<= 269892) BitRate= 264600;
        if (BitRate>= 282240 && BitRate<= 293760) BitRate= 288000;
        if (BitRate>= 376320 && BitRate<= 391680) BitRate= 384000;
        if (BitRate>= 518616 && BitRate<= 539784) BitRate= 529200;
        if (BitRate>= 564480 && BitRate<= 587520) BitRate= 576000;
        if (BitRate>= 648270 && BitRate<= 674730) BitRate= 661500;
    }

    else if (Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("PCM"))==0)
    {
        if (BitRate>=  62720 && BitRate<=  65280) BitRate=  64000;
        if (BitRate>=  86436 && BitRate<=  89964) BitRate=  88200;
        if (BitRate>= 125440 && BitRate<= 130560) BitRate= 128000;
        if (BitRate>= 172872 && BitRate<= 179928) BitRate= 176400;
        if (BitRate>= 188160 && BitRate<= 195840) BitRate= 192000;
        if (BitRate>= 250880 && BitRate<= 261120) BitRate= 256000;
        if (BitRate>= 345744 && BitRate<= 359856) BitRate= 352800;
        if (BitRate>= 376320 && BitRate<= 391680) BitRate= 384000;
        if (BitRate>= 501760 && BitRate<= 522240) BitRate= 512000;
        if (BitRate>= 691488 && BitRate<= 719712) BitRate= 705600;
        if (BitRate>= 752640 && BitRate<= 783360) BitRate= 768000;
        if (BitRate>=1003520 && BitRate<=1044480) BitRate=1024000;
        if (BitRate>=1128960 && BitRate<=1175040) BitRate=1152000;
        if (BitRate>=1382976 && BitRate<=1439424) BitRate=1411200;
        if (BitRate>=1505280 && BitRate<=1566720) BitRate=1536000;
    }

    else if (Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("ADPCM"))==0
          || Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("U-Law"))==0
          || Config.Codec_Get(Codec, InfoCodec_KindofCodec, Stream_Audio)==_T("ADPCM")
          || Config.Codec_Get(Codec, InfoCodec_KindofCodec, Stream_Audio)==_T("U-Law"))
    {
        if (BitRate>=  42000 && BitRate<=  46000) BitRate=  44100;
        if (BitRate>=  62720 && BitRate<=  65280) BitRate=  64000;
        if (BitRate>=  86436 && BitRate<=  89964) BitRate=  88200;
        if (BitRate>= 125440 && BitRate<= 130560) BitRate= 128000;
        if (BitRate>= 172872 && BitRate<= 179928) BitRate= 176400;
        if (BitRate>= 188160 && BitRate<= 195840) BitRate= 192000;
        if (BitRate>= 250880 && BitRate<= 261120) BitRate= 256000;
        if (BitRate>= 345744 && BitRate<= 359856) BitRate= 352800;
        if (BitRate>= 376320 && BitRate<= 391680) BitRate= 384000;
    }

    if (BitRate!=BitRate_Sav)
        Audio[Pos](Parameter).From_Number(BitRate);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Text(size_t Pos)
{
    //Integrity
    if (Pos>=Text.size())
        return;

    //Delay/Video0
    if (Video.size()>0 && !Text[Pos](_T("Delay")).empty())
    {
        Text[Pos](_T("Video0_Delay")).From_Number(Text[Pos](_T("Delay")).To_int32s()-Video[0](_T("Delay")).To_int32s());
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Chapters(size_t)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Image(size_t)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Menu(size_t)
{
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Final()
{
    //Integrity
    if (0>=General.size())
        return;

    //Corelation
    //-PlayTime if General not filled
    if (General[0](_T("PlayTime")).empty())
    {
        int64u PlayTime=0;
        //For all streams (Generic)
        for (size_t StreamKind=Stream_Video; StreamKind<Stream_Max; StreamKind++)
            for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
            {
                if (!(*Stream[StreamKind])[Pos](_T("PlayTime")).empty())
                {
                    int64u PlayTime_Stream=(*Stream[StreamKind])[Pos](_T("PlayTime")).To_int64u();
                    if (PlayTime_Stream>PlayTime)
                        PlayTime=PlayTime_Stream;
                }
            }

        //Filling
        if (PlayTime>0)
            General[0](_T("PlayTime")).From_Number(PlayTime);
    }
    //-Stream size if all stream sizes are OK
    if (General[0](_T("StreamSize")).empty())
    {
        int64u StreamSize_Total=0;
        bool IsOK=true;
        //For all streams (Generic)
        for (size_t StreamKind=Stream_Video; StreamKind<Stream_Max; StreamKind++)
        {
            if (StreamKind!=Stream_Chapters && StreamKind!=Stream_Menu) //They have no big size, we never calculate them
                for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
                {
                    int64u StreamSize=(*Stream[StreamKind])[Pos](_T("StreamSize")).To_int64u();
                    if (StreamSize>0)
                        StreamSize_Total+=StreamSize;
                    else if (IsOK)
                        IsOK=false; //StreamSize not available for 1 stream, we can't calculate
                }
        }

        //Filling
        if (IsOK && StreamSize_Total>0 && StreamSize_Total<File_Size)
            General[0](_T("StreamSize")).From_Number(File_Size-StreamSize_Total);
    }

    //-BitRate if we have one Audio stream with bitrate
    if (General[0](_T("PlayTime")).empty() && General[0](_T("BitRate")).empty() && Video.empty() && Audio.size()==1 && Audio[0](_T("BitRate")).To_int64u()!=0)
        General[0](_T("BitRate"))=Audio[0](_T("BitRate"));
    //-BitRate if PlayTime
    if (General[0](_T("BitRate")).empty() && General[0](_T("PlayTime")).To_int64u()!=0)
        General[0](_T("BitRate")).From_Number(General[0](_T("FileSize")).To_int64u()*8*1000/General[0](_T("PlayTime")).To_int64u());
    //-PlayTime if BitRate
    if (General[0](_T("PlayTime")).empty() && General[0](_T("BitRate")).To_int64u()!=0)
        General[0](_T("PlayTime")).From_Number(General[0](_T("FileSize")).To_int64u()*8*1000/General[0](_T("BitRate")).To_int64u());
    //-Video bitrate can be the nominal one if <10s (bitrate estimation is not enough precise
    if (Video.size()==1 && Video[0](_T("BitRate")).empty() && Video[0](_T("PlayTime")).To_int64u()<10000)
        Video[0](_T("BitRate"))=Video[0](_T("BitRate_Nominal"));
    //-Video bitrate if we have all audio bitrates and overal bitrate
    if (Video.size()==1 && General[0](_T("BitRate")).size()>4 && Video[0](_T("BitRate")).empty() && Video[0](_T("PlayTime")).To_int64u()>=10000) //BitRate is > 10 000 and playtime>10s, to avoid strange behavior
    {
        double GeneralBitRate_Ratio=0.98;  //Default container overhead=2%
        int32u GeneralBitRate_Minus=5000;  //5000 bps because of a "classic" stream overhead
        double VideoBitRate_Ratio  =0.98;  //Default container overhead=2%
        int32u VideoBitRate_Minus  =2000;  //2000 bps because of a "classic" stream overhead
        double AudioBitRate_Ratio  =0.98;  //Default container overhead=2%
        int32u AudioBitRate_Minus  =2000;  //2000 bps because of a "classic" stream overhead
        double TextBitRate_Ratio   =0.98;  //Default container overhead=2%
        int32u TextBitRate_Minus   =2000;  //2000 bps because of a "classic" stream overhead
        //Specific value depends of Container
        if (Get(Stream_General, 0, _T("Format"))==_T("MPEG-1PS")
         || Get(Stream_General, 0, _T("Format"))==_T("MPEG-2PS")
         || Get(Stream_General, 0, _T("Format"))==_T("MPEG-4PS"))
        {
            GeneralBitRate_Ratio=0.99;
            GeneralBitRate_Minus=0;
            VideoBitRate_Ratio  =0.94;
            VideoBitRate_Minus  =0;
            AudioBitRate_Ratio  =0.94;
            AudioBitRate_Minus  =0;
            TextBitRate_Ratio   =0.94;
            TextBitRate_Minus   =0;
        }
        if (Get(Stream_General, 0, _T("Format"))==_T("MPEG-1TS")
         || Get(Stream_General, 0, _T("Format"))==_T("MPEG-2TS")
         || Get(Stream_General, 0, _T("Format"))==_T("MPEG-4TS"))
        {
            GeneralBitRate_Ratio=0.99;
            GeneralBitRate_Minus=0;
            VideoBitRate_Ratio  =0.97;
            VideoBitRate_Minus  =0;
            AudioBitRate_Ratio  =0.97;
            AudioBitRate_Minus  =0;
            TextBitRate_Ratio   =0.97;
            TextBitRate_Minus   =0;
        }

        if (Config.Format_Get((*Stream[Stream_General])[0](_T("Format")), InfoFormat_KindofFormat)==_T("MPEG-4"))
        {
            GeneralBitRate_Ratio=1;
            GeneralBitRate_Minus=0;
            VideoBitRate_Ratio  =1;
            VideoBitRate_Minus  =0;
            AudioBitRate_Ratio  =1;
            AudioBitRate_Minus  =0;
            TextBitRate_Ratio   =1;
            TextBitRate_Minus   =0;
        }

        //Testing
        float64 VideoBitRate=General[0](_T("BitRate")).To_float64()*GeneralBitRate_Ratio-GeneralBitRate_Minus;
        bool VideobitRateIsValid=true;
        for (size_t Pos=0; Pos<Audio.size(); Pos++)
        {
            float64 AudioBitRate=Audio[Pos](_T("BitRate")).To_float64();
            if (AudioBitRate>0 && AudioBitRate_Ratio)
                VideoBitRate-=AudioBitRate/AudioBitRate_Ratio+AudioBitRate_Minus;
            else
                VideobitRateIsValid=false;
        }
        for (size_t Pos=0; Pos<Text.size(); Pos++)
        {
            float64 TextBitRate=Text[Pos](_T("BitRate")).To_float64();
            if (TextBitRate>0 && TextBitRate_Ratio)
                VideoBitRate-=TextBitRate/TextBitRate_Ratio+TextBitRate_Minus;
            else
                VideoBitRate-=1000; //Estimation: Text stream are not often big
        }
        if (VideobitRateIsValid && VideoBitRate>=10000) //to avoid strange behavior
            Video[0](_T("BitRate")).From_Number(VideoBitRate*VideoBitRate_Ratio-VideoBitRate_Minus, 0); //Default container overhead=2%
    }

    //Counts
    General[0](_T("Count")).From_Number(General[0].size());
    General[0](_T("VideoCount")).From_Number(Video.size());
    General[0](_T("AudioCount")).From_Number(Audio.size());
    General[0](_T("TextCount")).From_Number(Text.size());
    General[0](_T("ChaptersCount")).From_Number(Chapters.size());
    General[0](_T("MenuCount")).From_Number(Menu.size());

    //Format
    General[0](_T("Codec"))=General[0](_T("Format"));
    General[0](_T("Codec/String"))=General[0](_T("Format/String"));
    General[0](_T("Codec/Info"))=General[0](_T("Format/Info"));
    General[0](_T("Codec/Family"))=General[0](_T("Format/Family"));
    General[0](_T("Codec/Url"))=General[0](_T("Format/Url"));
    General[0](_T("Codec/Extensions"))=General[0](_T("Format/Extensions"));
    General[0](_T("Codec_Settings"))=General[0](_T("Format_Settings"));
    General[0](_T("Codec_Settings_Automatic"))=General[0](_T("Format_Settings_Automatic"));

    //Tags
    Finalize_Tags();
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Tags()
{
    //Integrity
    if (General.size()<1)
        return;

    //-Movie/Album
    if (!General[0](_T("Title")).empty() && General[0](_T("Movie")).empty() && General[0](_T("Track")).empty())
    {
        if (Video.size()!=0)
            General[0](_T("Movie"))=General[0](_T("Title"));
        else
            General[0](_T("Track"))=General[0](_T("Title"));
    }
    if (!General[0](_T("Title/More")).empty() && General[0](_T("Movie/More")).empty() && General[0](_T("Track/More")).empty())
    {
        if (Video.size()!=0)
            General[0](_T("Movie/More"))=General[0](_T("Title/More"));
        else
            General[0](_T("Track/More"))=General[0](_T("Title/More"));
    }
    if (!General[0](_T("Title/Url")).empty() && General[0](_T("Movie/Url")).empty() && General[0](_T("Track/Url")).empty())
    {
        if (Video.size()!=0)
            General[0](_T("Movie/Url"))=General[0](_T("Title/Url"));
        else
            General[0](_T("Track/Url"))=General[0](_T("Title/Url"));
    }
    //-Title
    if (General[0](_T("Title")).empty() && !General[0](_T("Movie")).empty())
        General[0](_T("Title"))=General[0](_T("Movie"));
    if (General[0](_T("Title")).empty() && !General[0](_T("Track")).empty())
        General[0](_T("Title"))=General[0](_T("Track"));
    if (General[0](_T("Title/More")).empty() && !General[0](_T("Movie/More")).empty())
        General[0](_T("Title/More"))=General[0](_T("Movie/More"));
    if (General[0](_T("Title/More")).empty() && !General[0](_T("Track/More")).empty())
        General[0](_T("Title/More"))=General[0](_T("Track/More"));
    if (General[0](_T("Title/Url")).empty() && !General[0](_T("Movie/Url")).empty())
        General[0](_T("Title/Url"))=General[0](_T("Movie/Url"));
    if (General[0](_T("Title/Url")).empty() && !General[0](_T("Track/Url")).empty())
        General[0](_T("Title/Url"))=General[0](_T("Track/Url"));

    //-Genre
    if (!General[0](_T("Genre")).empty() && General[0](_T("Genre")).size()<4 && General[0](_T("Genre"))[0]>=_T('0') && General[0](_T("Genre"))[0]<=_T('9'))
    {
        Ztring Genre;
        if (General[0](_T("Genre")).size()==1) Genre=Ztring(_T("Genre_00"))+General[0](_T("Genre"));
        if (General[0](_T("Genre")).size()==2) Genre=Ztring(_T("Genre_0" ))+General[0](_T("Genre"));
        if (General[0](_T("Genre")).size()==3) Genre=Ztring(_T("Genre_"  ))+General[0](_T("Genre"));
        General[0](_T("Genre"))=Config.Language_Get(Genre);
    }
}

//***************************************************************************
// Internal Functions
//***************************************************************************

//---------------------------------------------------------------------------
//PlayTime
void File__Analyze::PlayTime_PlayTime123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    ZtringListList &List=(*Stream[StreamKind])[StreamPos];
    if (List.Find(Value+_T("/String"))==Error || List(Value).empty())
        return;

    int32s HH, MM, SS, MS;
    Ztring PlayTimeString1, PlayTimeString2, PlayTimeString3;
    bool Negative=false;
    MS=List(Value).To_int32s(); //en ms
    if (MS<0)
    {
        Negative=true;
        MS=-MS;
    }
    if (MS==0)
        return;
    HH=MS/1000/60/60; //h
    if (HH>0)
    {
        PlayTimeString1+=Ztring::ToZtring(HH)+Config.Language_Get(_T("h"));
        PlayTimeString2+=Ztring::ToZtring(HH)+Config.Language_Get(_T("h"));
        if (HH<10)
            PlayTimeString3+=Ztring(_T("0"))+Ztring::ToZtring(HH)+_T(":");
        else
            PlayTimeString3+=Ztring::ToZtring(HH)+_T(":");
        MS-=HH*60*60*1000;
    }
    if (HH==0)
    {
        PlayTimeString3+=_T("00:");
    }

    MM=MS/1000/60; //mn
    if (MM>0)
    {
        if (PlayTimeString1.size()>0)
            PlayTimeString1+=_T(" ");
        PlayTimeString1+=Ztring::ToZtring(MM)+Config.Language_Get(_T("mn"));
        if (PlayTimeString2.size()<5)
        {
            if (PlayTimeString2.size()>0)
                PlayTimeString2+=_T(" ");
            PlayTimeString2+=Ztring::ToZtring(MM)+Config.Language_Get(_T("mn"));
        }
        if (MM<10)
            PlayTimeString3+=Ztring(_T("0"))+Ztring::ToZtring(MM)+_T(":");
        else
            PlayTimeString3+=Ztring::ToZtring(MM)+_T(":");
        MS-=MM*60*1000;
    }
    if (MM==0)
    {
        PlayTimeString3+=_T("00:");
    }

    SS=MS/1000; //s
    if (SS>0)
    {
        if (PlayTimeString1.size()>0)
            PlayTimeString1+=_T(" ");
        PlayTimeString1+=Ztring::ToZtring(SS)+Config.Language_Get(_T("s"));
        if (PlayTimeString2.size()<5)
        {
            if (PlayTimeString2.size()>0)
                PlayTimeString2+=_T(" ");
            PlayTimeString2+=Ztring::ToZtring(SS)+Config.Language_Get(_T("s"));
        }
        else if (PlayTimeString2.size()==0)
            PlayTimeString2+=Ztring::ToZtring(SS)+Config.Language_Get(_T("s"));
        if (SS<10)
            PlayTimeString3+=Ztring(_T("0"))+Ztring::ToZtring(SS)+_T(".");
        else
            PlayTimeString3+=Ztring::ToZtring(SS)+_T(".");
        MS-=SS*1000;
    }
    if (SS==0)
    {
        PlayTimeString3+=_T("00.");
    }

    if (MS>0) //ms
    {
        if (PlayTimeString1.size()>0)
            PlayTimeString1+=_T(" ");
        PlayTimeString1+=Ztring::ToZtring(MS)+Config.Language_Get(_T("ms"));
        if (PlayTimeString2.size()<5)
        {
            if (PlayTimeString2.size()>0)
                PlayTimeString2+=_T(" ");
            PlayTimeString2+=Ztring::ToZtring(MS)+Config.Language_Get(_T("ms"));
        }
        if (MS<10)
            PlayTimeString3+=Ztring(_T("00"))+Ztring::ToZtring(MS);
        else if (MS<100)
            PlayTimeString3+=Ztring(_T("0"))+Ztring::ToZtring(MS);
        else
            PlayTimeString3+=Ztring::ToZtring(MS);
    }
    if (MS==0)
    {
        PlayTimeString3+=_T("000");
    }

    if (Negative)
    {
        PlayTimeString1=Ztring(_T("-"))+PlayTimeString1;
        PlayTimeString2=Ztring(_T("-"))+PlayTimeString2;
        PlayTimeString3=Ztring(_T("-"))+PlayTimeString3;
    }

    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(),  PlayTimeString2, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String1")).To_Local().c_str(), PlayTimeString1, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String2")).To_Local().c_str(), PlayTimeString2, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String3")).To_Local().c_str(), PlayTimeString3, true);
}

//---------------------------------------------------------------------------
//FileSize
void File__Analyze::FileSize_FileSize123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    ZtringListList &List=(*Stream[StreamKind])[StreamPos];
    if (List.Find(Value+_T("/String"))==Error || List(Value).empty())
        return;

    float F1=(float)List(Value).To_int64s(); //Video C++ 6 patch, should be int64u
    //--Bytes, KiB, MiB or GiB...
    int32u Pow3=0;
    while(F1>=1024)
    {
        F1/=1024;
        Pow3++;
    }
    //--Count of digits
    int32u I2, I3, I4;
         if (F1>=100)
    {
        I2=0;
        I3=0;
        I4=1;
    }
    else if (F1>=10)
    {
        I2=0;
        I3=1;
        I4=2;
    }
    else //if (F1>=1)
    {
        I2=1;
        I3=2;
        I4=3;
    }
    Ztring Measure;
    switch (Pow3)
    {
        case  0 : Measure=Config.Language_Get(Ztring::ToZtring(F1, 0), _T(" Byte")); Measure.FindAndReplace(Ztring::ToZtring(F1, 0), _T("")); break; //This is only to have measure with multiple plurals
        case  1 : Measure=Config.Language_Get(_T(" KiB")); break;
        case  2 : Measure=Config.Language_Get(_T(" MiB")); break;
        case  3 : Measure=Config.Language_Get(_T(" GiB")); break;
        default : Measure=Config.Language_Get(_T(" ????Bytes"));
    }
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(),  Ztring::ToZtring(F1, I3)+Measure, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String1")).To_Local().c_str(), Ztring::ToZtring(F1,  0)+Measure, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String2")).To_Local().c_str(), Ztring::ToZtring(F1, I2)+Measure, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String3")).To_Local().c_str(), Ztring::ToZtring(F1, I3)+Measure, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String4")).To_Local().c_str(), Ztring::ToZtring(F1, I4)+Measure, true);
}

//---------------------------------------------------------------------------
//FileSize
void File__Analyze::Kilo_Kilo123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    ZtringListList &List=(*Stream[StreamKind])[StreamPos];
    if (List.Find(Value+_T("/String"))==Error || List(Value).empty())
        return;

    int32u BitRate=List(Value).To_int32u();

    //Well known values
    Ztring BitRateS;
    if (BitRate==  11024) BitRateS=  "11.024";
    if (BitRate==  11025) BitRateS=  "11.025";
    if (BitRate==  22050) BitRateS=  "22.05";
    if (BitRate==  44100) BitRateS=  "44.1";
    if (BitRate==  66150) BitRateS=  "66.15";
    if (BitRate==  88200) BitRateS=  "88.2";
    if (BitRate== 132300) BitRateS= "132.3";
    if (BitRate== 176400) BitRateS= "176.4";
    if (BitRate== 264600) BitRateS= "264.6";
    if (BitRate== 352800) BitRateS= "352.8";
    if (BitRate== 529200) BitRateS= "529.2";
    if (BitRate== 705600) BitRateS= "705.6";
    if (BitRate==1411200) BitRateS="1411.2";
    if (!BitRateS.empty())
    {
        Ztring Measure=Config.Info_Get(StreamKind).Read(Value, Info_Measure);
        Measure.insert(1, _T("K"));
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), BitRateS+Config.Language_Get(Measure), true);
        return;
    }

    //Standard
    if (BitRate>10000000)
    {
        Ztring Measure=Config.Info_Get(StreamKind).Read(Value, Info_Measure);
        Measure.insert(1, _T("M"));
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), Ztring::ToZtring(((float)BitRate)/1000000, BitRate>100000000?0:1)+Config.Language_Get(Measure), true);
    }
    else if (BitRate>10000)
    {
        Ztring Measure=Config.Info_Get(StreamKind).Read(Value, Info_Measure);
        Measure.insert(1, _T("K"));
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), Ztring::ToZtring(((float)BitRate)/1000, BitRate>100000?0:1)+Config.Language_Get(Measure), true);
    }
    else if (BitRate>0)
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), Ztring::ToZtring(BitRate)+Config.Language_Get(Config.Info_Get(StreamKind).Read(Value, Info_Measure)), true);
}

//---------------------------------------------------------------------------
//Value --> Value with measure
void File__Analyze::Value_Value123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    //String wanted?
    ZtringListList &List=(*Stream[StreamKind])[StreamPos];
    if (List.Find(Value+_T("/String"))==Error || List(Value).empty() || !List(Value+_T("/String")).empty())
        return;

    //String needed?
    if (List(Value).empty())
        return;

    //Filling
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), Config.Language_Get(List(Value), Config.Info_Get(StreamKind).Read(Value, Info_Measure)), true);
}

} //NameSpace

