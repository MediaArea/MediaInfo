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
    if (File_Name.size()>0)
    {
        //FileName
        Fill (Stream_General, 0, General_CompleteName, File_Name);
        Fill (Stream_General, 0, General_FolderName, FileName::Path_Get(File_Name));
        Fill (Stream_General, 0, General_FileName, FileName::Name_Get(File_Name));
        Fill (Stream_General, 0, General_FileExtension, FileName::Extension_Get(File_Name).MakeLowerCase());

        //FileSize
        File F(File_Name);
        if (File_Size!=(int64u)-1)
            Fill (Stream_General, 0, General_FileSize, File_Size);
        Fill (Stream_General, 0, General_File_Created_Date, F.Created_Get());
        Fill (Stream_General, 0, General_File_Modified_Date, F.Modified_Get());
    }
}

//***************************************************************************
// Finalize
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Finalize()
{
    //For all streams (Generic)
    if (IsSub)
        return;

    General_Fill();

    //Generic
    //ELEMENT(0, "Finalizing (Generic)");
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        Finalize__All((stream_t)StreamKind);

    //For each kind of (*Stream)
    for (size_t Pos=0; Pos<Count_Get(Stream_General);  Pos++) Finalize_General(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Video);    Pos++) Finalize_Video(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Audio);    Pos++) Finalize_Audio(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Text);     Pos++) Finalize_Text(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Chapters); Pos++) Finalize_Chapters(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Image);    Pos++) Finalize_Image(Pos);
    for (size_t Pos=0; Pos<Count_Get(Stream_Menu);     Pos++) Finalize_Menu(Pos);

    //Final parsing
    Finalize_Final();

    //For all streams (Generic)
    for (size_t StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++)
        Finalize_Final_All((stream_t)StreamKind);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize__All(stream_t StreamKind)
{
    for (size_t Pos=0; Pos<Count_Get(StreamKind); Pos++)
        Finalize__All(StreamKind, Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize__All(stream_t StreamKind, size_t Pos)
{
    //BitRate
    if (StreamKind!=Stream_Chapters && Retrieve(StreamKind, Pos, "BitRate").empty() && !Retrieve(StreamKind, Pos, "StreamSize").empty() && !Retrieve(StreamKind, Pos, "Duration").empty())
    {
        int64u Duration=Retrieve(StreamKind, Pos, "Duration").To_int64u();
        int64u StreamSize=Retrieve(StreamKind, Pos, "StreamSize").To_int64u();
        if (Duration>0 && StreamSize>0)
            Fill(StreamKind, Pos, "BitRate", StreamSize*8*1000/Duration);
    }

    //BitRate Nominal
    float32 BitRate=Retrieve(StreamKind, Pos, "BitRate").To_float32();
    float32 BitRate_Nominal=Retrieve(StreamKind, Pos, "BitRate_Nominal").To_float32();
    if (BitRate_Nominal>BitRate*0.995 && BitRate_Nominal<BitRate*1.005)
    {
        Fill(StreamKind, Pos, "BitRate", Retrieve(StreamKind, Pos, "BitRate_Nominal"), true);
        Clear(StreamKind, Pos, "BitRate_Nominal");
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Final_All(stream_t StreamKind)
{
    Ztring Z1, Z2, Z3, Z4; //For Codec_List
    for (size_t Pos=0; Pos<(*Stream)[StreamKind].size(); Pos++)
        Finalize_Final_All(StreamKind, Pos, Z1, Z2, Z3, Z4);

    //Codec_List
    if (StreamKind!=Stream_General && !Z1.empty() && MediaInfoLib::Config.ReadByHuman_Get())
    {
        Z1.resize(Z1.size()-3); //Delete extra " / "
        Z2.resize(Z2.size()-3); //Delete extra " / "
        Z3.resize(Z3.size()-3); //Delete extra " / "
        Z4.resize(Z4.size()-3); //Delete extra " / "
        Ztring Z9=Get((stream_t)StreamKind, 0, _T("StreamKind"), Info_Text);
        Fill(Stream_General, 0, Ztring(Z9+_T("_Format_List")).To_Local().c_str(), Z3);
        Fill(Stream_General, 0, Ztring(Z9+_T("_Format_WithHint_List")).To_Local().c_str(), Z4);
        Fill(Stream_General, 0, Ztring(Z9+_T("_Codec_List")).To_Local().c_str(), Z1);
        Fill(Stream_General, 0, Ztring(Z9+_T("_Language_List")).To_Local().c_str(), Z2);
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Final_All(stream_t StreamKind, size_t Pos, Ztring &Z1, Ztring &Z2, Ztring &Z3, Ztring &Z4)
{
    //Format
    if (!Retrieve(StreamKind, Pos, "Format").empty())
    {
        if (Retrieve(StreamKind, Pos, "Format/Info"  ).empty()) Fill(StreamKind, Pos, "Format/Info"  , MediaInfoLib::Config.Format_Get(Retrieve(StreamKind, Pos, "Format"), InfoFormat_Info));
        if (Retrieve(StreamKind, Pos, "Format/Url"   ).empty()) Fill(StreamKind, Pos, "Format/Url"   , MediaInfoLib::Config.Format_Get(Retrieve(StreamKind, Pos, "Format"), InfoFormat_Url));
    }

    if (StreamKind!=Stream_General)
    {
        //Codec
        if (!Retrieve(StreamKind, Pos, "Codec").empty())
        {
            const Ztring &C1=MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, Pos, "Codec"), InfoCodec_Name, (stream_t)StreamKind);
            if (C1.empty())
            {
                if (Retrieve(StreamKind, Pos, "Codec/String").empty()) Fill(StreamKind, Pos, "Codec/String", Retrieve(StreamKind, Pos, "Codec"));
            }
            else
            {
                if (Retrieve(StreamKind, Pos, "Codec/String").empty()) Fill(StreamKind, Pos, "Codec/String", C1);
                if (Retrieve(StreamKind, Pos, "Codec/Family").empty()) Fill(StreamKind, Pos, "Codec/Family", MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, Pos, "Codec"), InfoCodec_KindofCodec, (stream_t)StreamKind));
                if (Retrieve(StreamKind, Pos, "Codec/Info"  ).empty()) Fill(StreamKind, Pos, "Codec/Info"  , MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, Pos, "Codec"), InfoCodec_Description, (stream_t)StreamKind));
                if (Retrieve(StreamKind, Pos, "Codec/Url"   ).empty()) Fill(StreamKind, Pos, "Codec/Url"   , MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, Pos, "Codec"), InfoCodec_Url, (stream_t)StreamKind));
            }
        }

        //Special cases
        if (StreamKind==Stream_Audio
         && Retrieve(StreamKind, Pos, "Channel(s)").empty()
         &&(Retrieve(StreamKind, Pos, "Codec")==_T("samr")
         || Retrieve(StreamKind, Pos, "Codec")==_T("sawb")
         || Retrieve(StreamKind, Pos, "Codec")==_T("7A21")
         || Retrieve(StreamKind, Pos, "Codec")==_T("7A22"))
            )
            Fill(StreamKind, Pos, "Channel(s)", 1); //AMR is always with 1 channel

        //CodecID_Description
        if (!Retrieve(StreamKind, Pos, "CodecID_Description").empty() && Retrieve(StreamKind, Pos, "CodecID_Description")==Retrieve(StreamKind, Pos, "CodecID/Info"))
            Fill(StreamKind, Pos, "CodecID_Description", "", Unlimited, true, true);

        //Language
        //-Find 2-digit language
        if (!Retrieve(StreamKind, Pos, "Language").empty())
        {
            Ztring Language=Retrieve(StreamKind, Pos, "Language");
            Language.MakeLowerCase();
            if (Language.size()==3 && Language==_T("und"))
            {
                Clear(StreamKind, Pos, "Language");
                Language.clear();
            }
            if (!Language.empty() && MediaInfoLib::Config.ReadByHuman_Get())
            {
                if (Language.size()==3 && !MediaInfoLib::Config.Iso639_Get(Language).empty())
                    Language=MediaInfoLib::Config.Iso639_Get(Language);
                if (Language.size()>3 && !MediaInfoLib::Config.Iso639_Find(Language).empty())
                {
                    Language=MediaInfoLib::Config.Iso639_Find(Language);
                    Fill(StreamKind, Pos, "Language", Language, true); //Forcing ISO-639
                }
                //Translate
                if (Language.size()==2)
                {
                    Ztring Temp=_T("Language_"); Temp+=Language;
                    const Ztring& Z3=MediaInfoLib::Config.Language_Get(Temp);
                    Fill(StreamKind, Pos, "Language/String", Z3.find(_T("Language_"))==0?Language:Z3);
                }
                else
                {
                    Language=Retrieve(StreamKind, Pos, "Language");
                    Fill(StreamKind, Pos, "Language/String", Language);
                }
            }
        }

        //Codec_List
        if (MediaInfoLib::Config.ReadByHuman_Get())
        {
            Z1+=Retrieve(StreamKind, Pos, "Codec/String")+_T(" / ");
            Z2+=Retrieve(StreamKind, Pos, "Language/String")+_T(" / ");
            Z3+=Retrieve(StreamKind, Pos, "Format")+_T(" / ");
            Z4+=Retrieve(StreamKind, Pos, "Format");
            if (!Retrieve(StreamKind, Pos, "CodecID/Hint").empty())
            {
                Z4+=_T(" (");
                Z4+=Retrieve(StreamKind, Pos, "CodecID/Hint");
                Z4+=_T(")");
            }
            Z4+=_T(" / ");
        }
    }

    //Counts
    Fill(StreamKind, Pos, "Count", Count_Get(StreamKind, Pos));
    Fill(StreamKind, Pos, "StreamCount", Count_Get(StreamKind));
    Fill(StreamKind, Pos, "StreamKindID", Pos);
    if (Count_Get(StreamKind)>1)
        Fill(StreamKind, Pos, "StreamKindPos", Pos+1);

    //BitRate / OverallBitRate
    if (!Retrieve(StreamKind, Pos, StreamKind==Stream_General?"OverallBitRate_Mode":"BitRate_Mode").empty() && MediaInfoLib::Config.ReadByHuman_Get())
    {
        const Ztring &UnTranslated=Retrieve(StreamKind, Pos, StreamKind==Stream_General?"OverallBitRate_Mode":"BitRate_Mode");
        Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("BitRate_Mode_"))+UnTranslated);
        Fill(StreamKind, Pos, StreamKind==Stream_General?"OverallBitRate_Mode/String":"BitRate_Mode/String", Translated.find(_T("BitRate_Mode_"))?Translated:UnTranslated);
    }

    //Strings
    if (MediaInfoLib::Config.ReadByHuman_Get())
    {
        size_t List_Measure_Pos=Error;
        do
        {
            List_Measure_Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find_Filled(Info_Measure, List_Measure_Pos+1);
            if (List_Measure_Pos!=Error)
            {
                const Ztring &List_Measure_Value=MediaInfoLib::Config.Info_Get(StreamKind).Read(List_Measure_Pos, Info_Measure);
                const Ztring &List_Name_Value=MediaInfoLib::Config.Info_Get(StreamKind).Read(List_Measure_Pos, Info_Name);
                     if (List_Measure_Value==_T(" byte"))
                    FileSize_FileSize123(List_Name_Value, StreamKind, Pos);
                else if (List_Measure_Value==_T(" bps") || List_Measure_Value==_T(" Hz"))
                    Kilo_Kilo123(List_Name_Value, StreamKind, Pos);
                else if (List_Measure_Value==_T(" ms"))
                    Duration_Duration123(List_Name_Value, StreamKind, Pos);
                else if (List_Measure_Value==_T("Yes"))
                    YesNo_YesNo(List_Name_Value, StreamKind, Pos);
                else
                    Value_Value123(List_Name_Value, StreamKind, Pos);
            }

        }
        while (List_Measure_Pos!=Error);
    }

    //StreamKind
    Fill(StreamKind, Pos, General_StreamKind, MediaInfoLib::Config.Info_Get(StreamKind).Read(General_StreamKind, Info_Text));
    Fill(StreamKind, Pos, General_StreamKind_String, MediaInfoLib::Config.Language_Get(MediaInfoLib::Config.Info_Get(StreamKind).Read(General_StreamKind, Info_Text)), true);

    //Encoded_Library
    if (!Retrieve(StreamKind, Pos, "Encoded_Library").empty() && MediaInfoLib::Config.ReadByHuman_Get())
    {
        const Ztring& Name=Retrieve(StreamKind, Pos, "Encoded_Library/Name");
        const Ztring& Version=Retrieve(StreamKind, Pos, "Encoded_Library/Version");
        const Ztring& Date=Retrieve(StreamKind, Pos, "Encoded_Library/Date");
        if (!Name.empty())
        {
            Ztring String=Name;
            if (!Version.empty())
            {
                String+=_T(" ");
                String+=Version;
            }
            if (!Date.empty())
            {
                String+=_T(" (");
                String+=Date;
                String+=_T(")");
            }
            Fill(StreamKind, Pos, "Encoded_Library/String", String);
        }
        else
            Fill(StreamKind, Pos, "Encoded_Library/String", Retrieve(StreamKind, Pos, "Encoded_Library"));
    }

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
void File__Analyze::Finalize_General(size_t)
{
    //Integrity
    if (0>=Count_Get(Stream_General) || IsSub)
        return;

    //General, bases
    const Ztring Format=Retrieve(Stream_General, 0, "Format");
    if (!MediaInfoLib::Config.Format_Get(Format, InfoFormat_Name).empty())
    {
        Fill(Stream_General, 0, General_Format_Info, MediaInfoLib::Config.Format_Get(Format, InfoFormat_Info));
        Fill(Stream_General, 0, General_Format_Url, MediaInfoLib::Config.Format_Get(Format, InfoFormat_Url));
        Fill(Stream_General, 0, General_Format_Extensions, MediaInfoLib::Config.Format_Get(Format, InfoFormat_Extensions));
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Video(size_t Pos)
{
    //Integrity
    if (Pos>=Count_Get(Stream_Video))
        return;

    //Format_Settings_Matrix
    if (!Retrieve(Stream_Video, Pos, Video_Format_Settings_Matrix).empty())
    {
        const Ztring &UnTranslated=Retrieve(Stream_Video, Pos, Video_Format_Settings_Matrix);
        Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("Format_Settings_Matrix_"))+UnTranslated);
        Fill(Stream_Video, Pos, Video_Format_Settings_Matrix_String, Translated.find(_T("Format_Settings_Matrix_"))?Translated:UnTranslated);
    }

    //Scan type
    if (!Retrieve(Stream_Video, Pos, Video_ScanType).empty())
    {
        const Ztring &UnTranslated=Retrieve(Stream_Video, Pos, Video_ScanType);
        Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("Interlaced_"))+UnTranslated);
        Fill(Stream_Video, Pos, Video_ScanType_String, Translated.find(_T("Interlaced_"))?Translated:UnTranslated);
    }
    //Scan order
    if (!Retrieve(Stream_Video, Pos, Video_ScanOrder).empty())
    {
        const Ztring &UnTranslated=Retrieve(Stream_Video, Pos, Video_ScanOrder);
        Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("Interlaced_"))+UnTranslated);
        Fill(Stream_Video, Pos, Video_ScanOrder_String, Translated.find(_T("Interlaced_"))?Translated:UnTranslated);
    }
    //Interlacement
    if (!Retrieve(Stream_Video, Pos, "Interlacement").empty() && MediaInfoLib::Config.ReadByHuman_Get())
    {
        const Ztring &Z1=Retrieve(Stream_Video, Pos, "Interlacement");
        if (Z1.size()==3)
            Fill(Stream_Video, Pos, "Interlacement/String", MediaInfoLib::Config.Language_Get(Ztring(_T("Interlaced_"))+Z1));
        else
            Fill(Stream_Video, Pos, "Interlacement/String", MediaInfoLib::Config.Language_Get(Z1));
        if (Retrieve(Stream_Video, Pos, "Interlacement/String").empty())
            Fill(Stream_Video, Pos, "Interlacement/String", Z1);
    }
    //Display Aspect Ratio from Pixel Aspect Ratio
    if (Retrieve(Stream_Video, Pos, "DisplayAspectRatio").empty() && !Retrieve(Stream_Video, Pos, "PixelAspectRatio").empty())
    {
        float PAR   =Retrieve(Stream_Video, Pos, "PixelAspectRatio").To_float32();
        float Width =Retrieve(Stream_Video, Pos, "Width"           ).To_float32();
        float Height=Retrieve(Stream_Video, Pos, "Height"          ).To_float32();
        if (PAR && Height && Width)
            Fill(Stream_Video, Pos, "DisplayAspectRatio", ((float32)Width)/Height*PAR, 3);
    }
    //Pixel Aspect Ratio from Display Aspect Ratio
    if (Retrieve(Stream_Video, Pos, "PixelAspectRatio").empty() && !Retrieve(Stream_Video, Pos, "DisplayAspectRatio").empty())
    {
        float DAR   =Retrieve(Stream_Video, Pos, "DisplayAspectRatio").To_float32();
        float Width =Retrieve(Stream_Video, Pos, "Width"             ).To_float32();
        float Height=Retrieve(Stream_Video, Pos, "Height"            ).To_float32();
        if (DAR && Height && Width)
            Fill(Stream_Video, Pos, "PixelAspectRatio", DAR/(((float32)Width)/Height));
    }
    //Display Aspect Ratio by default (thinking that PAR is 1.000)
    if (Retrieve(Stream_Video, Pos, "DisplayAspectRatio").empty())
    {
        float Width =Retrieve(Stream_Video, Pos, "Width" ).To_float32();
        float Height=Retrieve(Stream_Video, Pos, "Height").To_float32();
        if (Height && Width)
            Fill(Stream_Video, Pos, "DisplayAspectRatio", ((float32)Width)/Height);
    }
    //Display Aspect Ratio
    if (!Retrieve(Stream_Video, Pos, "DisplayAspectRatio").empty() && MediaInfoLib::Config.ReadByHuman_Get())
    {
        float F1=Retrieve(Stream_Video, Pos, "DisplayAspectRatio").To_float32();
        Ztring C1;
             if (0);
        else if (F1>1.23 && F1<1.27) C1=_T("5/4");
        else if (F1>1.30 && F1<1.37) C1=_T("4/3");
        else if (F1>1.70 && F1<1.85) C1=_T("16/9");
        else if (F1>2.10 && F1<2.22) C1=_T("2.2");
        else if (F1>2.23 && F1<2.30) C1=_T("2.25");
        else if (F1>2.30 && F1<2.40) C1=_T("2.35");
        else              C1.From_Number(F1);
        Fill(Stream_Video, Pos, "DisplayAspectRatio/String", C1);
    }
    //Standard
    if (Retrieve(Stream_Video, Pos, "Standard").empty() && Retrieve(Stream_Video, Pos, "Width")==_T("720"))
    {
             if (Retrieve(Stream_Video, Pos, "Height")==_T("576"))
            Fill(Stream_Video, Pos, "Standard", "PAL");
        else if (Retrieve(Stream_Video, Pos, "Height")==_T("480"))
            Fill(Stream_Video, Pos, "Standard", "NTSC");
    }
    //FrameRate_Mode
    if (!Retrieve(Stream_Video, Pos, Video_FrameRate_Mode).empty())
    {
        const Ztring &UnTranslated=Retrieve(Stream_Video, Pos, Video_FrameRate_Mode);
        Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("FrameRate_Mode_"))+UnTranslated);
        Fill(Stream_Video, Pos, Video_FrameRate_Mode_String, Translated.find(_T("FrameRate_Mode_"))?Translated:UnTranslated);
    }
    //FrameRate Nominal
    float32 FrameRate=Retrieve(Stream_Video, Pos, "FrameRate").To_float32();
    float32 FrameRate_Nominal=Retrieve(Stream_Video, Pos, "FrameRate_Nominal").To_float32();
    if (FrameRate_Nominal>FrameRate*0.995 && FrameRate_Nominal<FrameRate*1.005)
    {
        Fill(Stream_Video, Pos, "FrameRate", Retrieve(Stream_Video, Pos, "FrameRate_Nominal"), true);
        Clear(Stream_Video, Pos, "FrameRate_Nominal");
    }
    //FrameCount
    if (Retrieve(Stream_Video, Pos, "FrameCount").empty())
    {
        int64s Duration=Retrieve(Stream_Video, Pos, "Duration").To_int64s();
        if (Duration==0)
            Duration=Retrieve(Stream_General, 0, General_Duration).To_int64s();
        float FrameRate=Retrieve(Stream_Video, Pos, "FrameRate").To_float32();
        if (Duration && FrameRate)
           Fill(Stream_Video, Pos, "FrameCount", ((float64)Duration)/1000*FrameRate, 0);
    }
    //Duration
    if (Retrieve(Stream_Video, Pos, "Duration").empty())
    {
        int64u Duration=Retrieve(Stream_Video, Pos, "Duration").To_int64u();
        int64u FrameRate=Retrieve(Stream_Video, Pos, "FrameRate").To_int64u();
        if (Duration==0 && FrameRate!=0)
            Duration=Retrieve(Stream_Video, Pos, "FrameCount").To_int64u()*1000/FrameRate;
        if (Duration)
           Fill(Stream_Video, Pos, "Duration", Duration);
    }
    //Format_Settings_CABAC
    if (!Retrieve(Stream_Video, Pos, Video_Format_Settings_CABAC).empty())
    {
        const Ztring &UnTranslated=Retrieve(Stream_Video, Pos, Video_Format_Settings_CABAC);
        Ztring Translated=MediaInfoLib::Config.Language_Get(UnTranslated);
        Fill(Stream_Video, Pos, Video_Format_Settings_CABAC_String, Translated);
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Audio(size_t Pos)
{
    //Integrity
    if (Pos>=Count_Get(Stream_Audio))
        return;

    //SamplingCount
    if (Retrieve(Stream_Audio, Pos, Audio_SamplingCount).empty())
    {
        int64s Duration=Retrieve(Stream_Audio, Pos, Audio_Duration).To_int64s();
        if (Duration==0)
            Duration=Retrieve(Stream_General, 0, General_Duration).To_int64s();
        float SamplingRate=Retrieve(Stream_Audio, Pos, Audio_SamplingRate).To_float32();
        if (Duration && SamplingRate)
           Fill(Stream_Audio, Pos, Audio_SamplingCount, ((float64)Duration)/1000*SamplingRate, 0);
    }
    //Duration
    if (Retrieve(Stream_Audio, Pos, Audio_Duration).empty() && Retrieve(Stream_Audio, Pos, Audio_SamplingRate).To_int64s()!=0)
    {
        int64u Duration=Retrieve(Stream_Audio, Pos, Audio_SamplingCount).To_int64u()*1000/Retrieve(Stream_Audio, Pos, "SamplingRate").To_int64u();
        if (Duration)
           Fill(Stream_Audio, Pos, Audio_Duration, Duration);
    }
    //Delay/Video
    if (Count_Get(Stream_Video)>0 && !Retrieve(Stream_Audio, Pos, Audio_Delay).empty() && !Retrieve(Stream_Video, 0, Video_Delay).empty())
        Fill(Stream_Audio, Pos, Audio_Video_Delay, Retrieve(Stream_Audio, Pos, Audio_Delay).To_int64s()-Retrieve(Stream_Video, 0, Video_Delay).To_int64s());
    //Delay/Video0
    if (Count_Get(Stream_Video)>0 && !Retrieve(Stream_Audio, Pos, Audio_Delay).empty() && !Retrieve(Stream_Video, 0, Video_Delay).empty())
        Fill(Stream_Audio, Pos, Audio_Video0_Delay, Retrieve(Stream_Audio, Pos, Audio_Delay).To_int64s()-Retrieve(Stream_Video, 0, Video_Delay).To_int64s());
    //CBR/VBR
    if (Retrieve(Stream_Audio, Pos, Audio_BitRate_Mode).empty() && !Retrieve(Stream_Audio, Pos, Audio_Codec).empty())
    {
        Ztring Z1=MediaInfoLib::Config.Codec_Get(Retrieve(Stream_Audio, Pos, Audio_Codec), InfoCodec_BitRate_Mode, Stream_Audio);
        if (!Z1.empty())
            Fill(Stream_Audio, Pos, Audio_BitRate_Mode, Z1);
    }

    //Well known bitrate values
    Finalize_Audio_BitRate(Pos, Audio_BitRate);
    Finalize_Audio_BitRate(Pos, Audio_BitRate_Nominal);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Audio_BitRate(size_t Pos, audio Parameter)
{
    const Ztring& Format=Retrieve(Stream_Audio, Pos, Audio_Format);
    const Ztring& Codec=Retrieve(Stream_Audio, Pos, Audio_Codec);
    int32u BitRate=Retrieve(Stream_Audio, Pos, Parameter).To_int32u();
    int32u BitRate_Sav=BitRate;
    if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec, Stream_Audio).find(_T("MPEG-"))==0
     || Retrieve(Stream_Audio, Pos, "Codec/String").find(_T("MPEG-"))==0)
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
        if (Retrieve(Stream_Audio, Pos, "BitRate_Mode")==_T("VBR"))
            BitRate=BitRate_Sav; //If VBR, we want the exact value
    }

    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("AC3"))==0)
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

    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("DTS"))==0)
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

    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("AAC"))==0)
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

    else if (Codec==_T("PCM") || MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("PCM"))==0)
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

    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("ADPCM"))==0
          || MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_Name, Stream_Audio).find(_T("U-Law"))==0
          || MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec, Stream_Audio)==_T("ADPCM")
          || MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec, Stream_Audio)==_T("U-Law")
          || Format==_T("ADPCM"))
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
        Fill(Stream_Audio, Pos, Parameter, BitRate, 0, true);
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Text(size_t Pos)
{
    //Integrity
    if (Pos>=Count_Get(Stream_Text))
        return;

    //Delay/Video
    if (Count_Get(Stream_Video)>0 && !Retrieve(Stream_Text, Pos, Text_Delay).empty())
        Fill(Stream_Text, Pos, Text_Video_Delay, Retrieve(Stream_Text, Pos, Text_Delay).To_int32s()-Retrieve(Stream_Video, 0, "Delay").To_int32s());
    //Delay/Video0
    if (Count_Get(Stream_Video)>0 && !Retrieve(Stream_Text, Pos, Text_Delay).empty())
        Fill(Stream_Text, Pos, Text_Video0_Delay, Retrieve(Stream_Text, Pos, Text_Delay).To_int32s()-Retrieve(Stream_Video, 0, "Delay").To_int32s());
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
    if (0>=Count_Get(Stream_General))
        return;

    //Corelation
    //-Duration if General not filled
    if (Retrieve(Stream_General, 0, General_Duration).empty())
    {
        int64u Duration=0;
        //For all streams (Generic)
        for (size_t StreamKind=Stream_Video; StreamKind<Stream_Max; StreamKind++)
            for (size_t Pos=0; Pos<Count_Get((stream_t)StreamKind); Pos++)
            {
                if (!Retrieve((stream_t)StreamKind, Pos, "Duration").empty())
                {
                    int64u Duration_Stream=Retrieve((stream_t)StreamKind, Pos, "Duration").To_int64u();
                    if (Duration_Stream>Duration)
                        Duration=Duration_Stream;
                }
            }

        //Filling
        if (Duration>0)
            Fill(Stream_General, 0, "Duration", Duration);
    }
    //-(*Stream) size if all stream sizes are OK
    if (Retrieve(Stream_General, 0, General_StreamSize).empty())
    {
        int64u StreamSize_Total=0;
        bool IsOK=true;
        //For all streams (Generic)
        for (size_t StreamKind=Stream_Video; StreamKind<Stream_Max; StreamKind++)
        {
            if (StreamKind!=Stream_Chapters && StreamKind!=Stream_Menu) //They have no big size, we never calculate them
                for (size_t Pos=0; Pos<Count_Get((stream_t)StreamKind); Pos++)
                {
                    int64u StreamSize=Retrieve((stream_t)StreamKind, Pos, "StreamSize").To_int64u();
                    if (StreamSize>0)
                        StreamSize_Total+=StreamSize;
                    else if (IsOK)
                        IsOK=false; //StreamSize not available for 1 stream, we can't calculate
                }
        }

        //Filling
        if (IsOK && StreamSize_Total>0 && StreamSize_Total<File_Size)
            Fill(Stream_General, 0, General_StreamSize, File_Size-StreamSize_Total);
    }

    //-OverallBitRate if we have one Audio stream with bitrate
    if (Retrieve(Stream_General, 0, General_Duration).empty() && Retrieve(Stream_General, 0, General_OverallBitRate).empty() && Count_Get(Stream_Video)==0 && Count_Get(Stream_Audio)==1 && Retrieve(Stream_Audio, 0, Audio_BitRate).To_int64u()!=0)
        Fill(Stream_General, 0, General_OverallBitRate, Retrieve(Stream_Audio, 0, Audio_BitRate));
    //-OverallBitRate if Duration
    if (Retrieve(Stream_General, 0, General_OverallBitRate).empty() && Retrieve(Stream_General, 0, General_Duration).To_int64u()!=0)
        Fill(Stream_General, 0, General_OverallBitRate, Retrieve(Stream_General, 0, General_FileSize).To_int64u()*8*1000/Retrieve(Stream_General, 0, General_Duration).To_int64u());
    //-Duration if OverallBitRate
    if (Retrieve(Stream_General, 0, General_Duration).empty() && Retrieve(Stream_General, 0, General_OverallBitRate).To_int64u()!=0)
        Fill(Stream_General, 0, General_Duration, Retrieve(Stream_General, 0, General_FileSize).To_int64u()*8*1000/Retrieve(Stream_General, 0, General_OverallBitRate).To_int64u());
    //-Video bitrate can be the nominal one if <10s (bitrate estimation is not enough precise
    if (Count_Get(Stream_Video)==1 && Retrieve(Stream_Video, 0, Video_BitRate).empty() && Retrieve(Stream_General, 0, General_Duration).To_int64u()<10000)
    {
        Fill(Stream_Video, 0, Video_BitRate, Retrieve(Stream_Video, 0, Video_BitRate_Nominal));
        Fill(Stream_Video, 0, Video_BitRate_Nominal, "", Unlimited, true, true);
    }
    //-Video bitrate if we have all audio bitrates and overal bitrate
    if (Count_Get(Stream_Video)==1 && Retrieve(Stream_General, 0, General_OverallBitRate).size()>4 && Retrieve(Stream_Video, 0, Video_BitRate).empty() && Retrieve(Stream_General, 0, General_Duration).To_int64u()>=1000) //BitRate is > 10 000 and Duration>10s, to avoid strange behavior
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
        if (Get(Stream_General, 0, _T("Format"))==_T("MPEG-PS"))
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
        if (Get(Stream_General, 0, _T("Format"))==_T("MPEG-TS"))
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

        if (MediaInfoLib::Config.Format_Get(Retrieve(Stream_General, 0, General_Format), InfoFormat_KindofFormat)==_T("MPEG-4"))
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
        float64 VideoBitRate=Retrieve(Stream_General, 0, General_OverallBitRate).To_float64()*GeneralBitRate_Ratio-GeneralBitRate_Minus;
        bool VideobitRateIsValid=true;
        for (size_t Pos=0; Pos<Count_Get(Stream_Audio); Pos++)
        {
            float64 AudioBitRate=Retrieve(Stream_Audio, Pos, Audio_BitRate).To_float64();
            if (AudioBitRate>0 && AudioBitRate_Ratio)
                VideoBitRate-=AudioBitRate/AudioBitRate_Ratio+AudioBitRate_Minus;
            else
                VideobitRateIsValid=false;
        }
        for (size_t Pos=0; Pos<Count_Get(Stream_Text); Pos++)
        {
            float64 TextBitRate=Retrieve(Stream_Text, Pos, Text_BitRate).To_float64();
            if (TextBitRate>0 && TextBitRate_Ratio)
                VideoBitRate-=TextBitRate/TextBitRate_Ratio+TextBitRate_Minus;
            else
                VideoBitRate-=1000; //Estimation: Text stream are not often big
        }
        if (VideobitRateIsValid && VideoBitRate>=10000) //to avoid strange behavior
            Fill(Stream_Video, 0, Video_BitRate, VideoBitRate*VideoBitRate_Ratio-VideoBitRate_Minus, 0); //Default container overhead=2%
    }
    //-General stream size if we have all streamsize
    if (File_Size!=(int64u)-1 && Retrieve(Stream_General, 0, General_StreamSize).empty())
    {
        //Testing
        int64s StreamSize=File_Size;
        bool StreamSizeIsValid=true;
        for (size_t StreamKind_Pos=Stream_General+1; StreamKind_Pos<Stream_Max; StreamKind_Pos++)
            for (size_t Pos=0; Pos<Count_Get((stream_t)StreamKind_Pos); Pos++)
            {
                int64u StreamXX_StreamSize=Retrieve((stream_t)StreamKind_Pos, Pos, "StreamSize").To_int64u();
                if (StreamXX_StreamSize>0)
                    StreamSize-=StreamXX_StreamSize;
                else
                    StreamSizeIsValid=false;
            }
        if (StreamSizeIsValid && StreamSize>=0) //to avoid strange behavior
            Fill(Stream_General, 0, General_StreamSize, StreamSize);
    }

    //Counts
    if (Count_Get(Stream_Video))
        Fill(Stream_General, 0, "VideoCount", Count_Get(Stream_Video));
    if (Count_Get(Stream_Audio))
        Fill(Stream_General, 0, "AudioCount", Count_Get(Stream_Audio));
    if (Count_Get(Stream_Text))
        Fill(Stream_General, 0, "TextCount", Count_Get(Stream_Text));
    if (Count_Get(Stream_Image))
        Fill(Stream_General, 0, "ImageCount", Count_Get(Stream_Image));
    if (Count_Get(Stream_Chapters))
        Fill(Stream_General, 0, "ChaptersCount", Count_Get(Stream_Chapters));
    if (Count_Get(Stream_Menu))
        Fill(Stream_General, 0, "MenuCount", Count_Get(Stream_Menu));

    //Format
    Fill(Stream_General, 0, "Format/String", Retrieve(Stream_General, 0, General_Format));
    Fill(Stream_General, 0, "Codec", Retrieve(Stream_General, 0, General_Format));
    Fill(Stream_General, 0, "Codec/String", Retrieve(Stream_General, 0, General_Format));
    Fill(Stream_General, 0, "Codec/Info", Retrieve(Stream_General, 0, General_Format_Info));
    Fill(Stream_General, 0, "Codec/Url", Retrieve(Stream_General, 0, General_Format_Url));
    Fill(Stream_General, 0, "Codec/Extensions", Retrieve(Stream_General, 0, General_Format_Extensions));
    Fill(Stream_General, 0, "Codec_Settings", Retrieve(Stream_General, 0, General_Format_Settings));

    //Tags
    Finalize_Tags();
}

//---------------------------------------------------------------------------
void File__Analyze::Finalize_Tags()
{
    //Integrity
    if (!Count_Get(Stream_General))
        return;

    //-Movie/Album
    if (!Retrieve(Stream_General, 0, General_Title).empty() && Retrieve(Stream_General, 0, General_Movie).empty() && Retrieve(Stream_General, 0, General_Track).empty())
    {
        if (Count_Get(Stream_Video))
            Fill(Stream_General, 0, "Movie", Retrieve(Stream_General, 0, General_Title));
        else
            Fill(Stream_General, 0, "Track", Retrieve(Stream_General, 0, General_Title));
    }
    if (!Retrieve(Stream_General, 0, General_Title_More).empty() && Retrieve(Stream_General, 0, General_Movie_More).empty() && Retrieve(Stream_General, 0, General_Track_More).empty())
    {
        if (Count_Get(Stream_Video))
            Fill(Stream_General, 0, "Movie/More", Retrieve(Stream_General, 0, General_Title_More));
        else
            Fill(Stream_General, 0, "Track/More", Retrieve(Stream_General, 0, General_Title_More));
    }
    if (!Retrieve(Stream_General, 0, General_Title_Url).empty() && Retrieve(Stream_General, 0, General_Movie_Url).empty() && Retrieve(Stream_General, 0, General_Track_Url).empty())
    {
        if (Count_Get(Stream_Video))
            Fill(Stream_General, 0, "Movie/Url", Retrieve(Stream_General, 0, General_Title_Url));
        else
            Fill(Stream_General, 0, "Track/Url", Retrieve(Stream_General, 0, General_Title_Url));
    }
    //-Title
    if (Retrieve(Stream_General, 0, General_Title).empty() && !Retrieve(Stream_General, 0, General_Movie).empty())
        Fill(Stream_General, 0, "Title", Retrieve(Stream_General, 0, General_Movie));
    if (Retrieve(Stream_General, 0, General_Title).empty() && !Retrieve(Stream_General, 0, General_Track).empty())
        Fill(Stream_General, 0, "Title", Retrieve(Stream_General, 0, General_Track));
    if (Retrieve(Stream_General, 0, General_Title_More).empty() && !Retrieve(Stream_General, 0, General_Movie_More).empty())
        Fill(Stream_General, 0, "Title/More", Retrieve(Stream_General, 0, General_Movie_More));
    if (Retrieve(Stream_General, 0, General_Title_More).empty() && !Retrieve(Stream_General, 0, General_Track_More).empty())
        Fill(Stream_General, 0, "Title/More", Retrieve(Stream_General, 0, General_Track_More));
    if (Retrieve(Stream_General, 0, General_Title_Url).empty() && !Retrieve(Stream_General, 0, General_Movie_Url).empty())
        Fill(Stream_General, 0, "Title/Url", Retrieve(Stream_General, 0, General_Movie_Url));
    if (Retrieve(Stream_General, 0, General_Title_Url).empty() && !Retrieve(Stream_General, 0, General_Track_Url).empty())
        Fill(Stream_General, 0, "Title/Url", Retrieve(Stream_General, 0, General_Track_Url));

    //-Genre
    if (!Retrieve(Stream_General, 0, General_Genre).empty() && Retrieve(Stream_General, 0, General_Genre).size()<4 && Retrieve(Stream_General, 0, General_Genre)[0]>=_T('0') && Retrieve(Stream_General, 0, General_Genre)[0]<=_T('9'))
    {
        Ztring Genre;
        if (Retrieve(Stream_General, 0, General_Genre).size()==1) Genre=Ztring(_T("Genre_00"))+Retrieve(Stream_General, 0, General_Genre);
        if (Retrieve(Stream_General, 0, General_Genre).size()==2) Genre=Ztring(_T("Genre_0" ))+Retrieve(Stream_General, 0, General_Genre);
        if (Retrieve(Stream_General, 0, General_Genre).size()==3) Genre=Ztring(_T("Genre_"  ))+Retrieve(Stream_General, 0, General_Genre);
        Fill(Stream_General, 0, "Genre", MediaInfoLib::Config.Language_Get(Genre), true);
    }
}

//***************************************************************************
// Internal Functions
//***************************************************************************

//---------------------------------------------------------------------------
//Duration
void File__Analyze::Duration_Duration123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    const ZtringListList &List=MediaInfoLib::Config.Info_Get(StreamKind);
    size_t List_Pos=List.Find(Value+_T("/String"));
    if (List_Pos==Error)
        return;
    const Ztring &Target=Retrieve(StreamKind, StreamPos, List_Pos);
    if (!Target.empty())
        return;
    List_Pos=List.Find(Value);
    if (List_Pos==Error || List[List_Pos].size()<=Info_Measure || List[List_Pos][Info_Measure].empty())
        return;

    int32s HH, MM, SS, MS;
    Ztring DurationString1, DurationString2, DurationString3;
    bool Negative=false;
    MS=Retrieve(StreamKind, StreamPos, Value.To_Local().c_str()).To_int32s(); //en ms
    if (MS==0)
        return;

    if (MS<0)
    {
        Negative=true;
        MS=-MS;
    }
    if (MS==0)
        return;

    //Hours
    HH=MS/1000/60/60; //h
    if (HH>0)
    {
        DurationString1+=Ztring::ToZtring(HH)+MediaInfoLib::Config.Language_Get(_T("h"));
        DurationString2+=Ztring::ToZtring(HH)+MediaInfoLib::Config.Language_Get(_T("h"));
        if (HH<10)
            DurationString3+=Ztring(_T("0"))+Ztring::ToZtring(HH)+_T(":");
        else
            DurationString3+=Ztring::ToZtring(HH)+_T(":");
        MS-=HH*60*60*1000;
    }
    else
    {
        DurationString3+=_T("00:");
    }

    //Minutes
    MM=MS/1000/60; //mn
    if (MM>0 || HH>0)
    {
        if (DurationString1.size()>0)
            DurationString1+=_T(" ");
        DurationString1+=Ztring::ToZtring(MM)+MediaInfoLib::Config.Language_Get(_T("mn"));
        if (DurationString2.size()<5)
        {
            if (DurationString2.size()>0)
                DurationString2+=_T(" ");
            DurationString2+=Ztring::ToZtring(MM)+MediaInfoLib::Config.Language_Get(_T("mn"));
        }
        if (MM<10)
            DurationString3+=Ztring(_T("0"))+Ztring::ToZtring(MM)+_T(":");
        else
            DurationString3+=Ztring::ToZtring(MM)+_T(":");
        MS-=MM*60*1000;
    }
    else
    {
        DurationString3+=_T("00:");
    }

    //Seconds
    SS=MS/1000; //s
    if (SS>0 || MM>0 || HH>0)
    {
        if (DurationString1.size()>0)
            DurationString1+=_T(" ");
        DurationString1+=Ztring::ToZtring(SS)+MediaInfoLib::Config.Language_Get(_T("s"));
        if (DurationString2.size()<5)
        {
            if (DurationString2.size()>0)
                DurationString2+=_T(" ");
            DurationString2+=Ztring::ToZtring(SS)+MediaInfoLib::Config.Language_Get(_T("s"));
        }
        else if (DurationString2.size()==0)
            DurationString2+=Ztring::ToZtring(SS)+MediaInfoLib::Config.Language_Get(_T("s"));
        if (SS<10)
            DurationString3+=Ztring(_T("0"))+Ztring::ToZtring(SS)+_T(".");
        else
            DurationString3+=Ztring::ToZtring(SS)+_T(".");
        MS-=SS*1000;
    }
    else
    {
        DurationString3+=_T("00.");
    }

    //Milliseconds
    if (MS>0 || SS>0 || MM>0 || HH>0)
    {
        if (DurationString1.size()>0)
            DurationString1+=_T(" ");
        DurationString1+=Ztring::ToZtring(MS)+MediaInfoLib::Config.Language_Get(_T("ms"));
        if (DurationString2.size()<5)
        {
            if (DurationString2.size()>0)
                DurationString2+=_T(" ");
            DurationString2+=Ztring::ToZtring(MS)+MediaInfoLib::Config.Language_Get(_T("ms"));
        }
        if (MS<10)
            DurationString3+=Ztring(_T("00"))+Ztring::ToZtring(MS);
        else if (MS<100)
            DurationString3+=Ztring(_T("0"))+Ztring::ToZtring(MS);
        else
            DurationString3+=Ztring::ToZtring(MS);
    }
    else
    {
        DurationString3+=_T("000");
    }

    if (Negative)
    {
        DurationString1=Ztring(_T("-"))+DurationString1;
        DurationString2=Ztring(_T("-"))+DurationString2;
        DurationString3=Ztring(_T("-"))+DurationString3;
    }

    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(),  DurationString2, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String1")).To_Local().c_str(), DurationString1, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String2")).To_Local().c_str(), DurationString2, true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String3")).To_Local().c_str(), DurationString3, true);
}

//---------------------------------------------------------------------------
//FileSize
void File__Analyze::FileSize_FileSize123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    const ZtringListList &List=MediaInfoLib::Config.Info_Get(StreamKind);
    size_t List_Pos=List.Find(Value+_T("/String"));
    if (List_Pos==Error)
        return;
    const Ztring &Target=Retrieve(StreamKind, StreamPos, List_Pos);
    if (!Target.empty())
        return;
    List_Pos=List.Find(Value);
    if (List_Pos==Error || List[List_Pos].size()<=Info_Measure || List[List_Pos][Info_Measure].empty())
        return;

    float F1=(float)Retrieve(StreamKind, StreamPos, Value.To_Local().c_str()).To_int64s(); //Video C++ 6 patch, should be int64u
    if (F1==0)
        return;

    //--Bytes, KiB, MiB or GiB...
    int32u Pow3=0;
    while(F1>=1024)
    {
        F1/=1024;
        Pow3++;
    }
    //--Count of digits
    int8u I2, I3, I4;
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
    Ztring Measure; bool MeasureIsAlwaysSame;
    switch (Pow3)
    {
        case  0 : Measure=_T(" Byte"); MeasureIsAlwaysSame=false; break;
        case  1 : Measure=_T(" KiB");  MeasureIsAlwaysSame=true;  break;
        case  2 : Measure=_T(" MiB");  MeasureIsAlwaysSame=true;  break;
        case  3 : Measure=_T(" GiB");  MeasureIsAlwaysSame=true;  break;
        default : Measure=_T(" ?iB");  MeasureIsAlwaysSame=true;
    }
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String1")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1,  0), Measure, MeasureIsAlwaysSame), true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String2")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1, I2), Measure, MeasureIsAlwaysSame), true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String3")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1, I3), Measure, MeasureIsAlwaysSame), true);
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String4")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1, I4), Measure, MeasureIsAlwaysSame), true);
    if (File_Size>0 && File_Size<(int64u)-1 && Value==_T("StreamSize"))
    {
        float F2=(float)Retrieve(StreamKind, StreamPos, Value.To_Local().c_str()).To_int64s(); //Video C++ 6 patch, should be int64u
        Fill(StreamKind, StreamPos, "StreamSize_Proportion", F2/File_Size, 5, true);
        Fill(StreamKind, StreamPos, "StreamSize/String5", MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1, I3), Measure, MeasureIsAlwaysSame)+_T(" (")+Ztring::ToZtring(F2*100/File_Size, 0)+_T("%)"), true);
        Fill(StreamKind, StreamPos, "StreamSize/String",  MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1, I3), Measure, MeasureIsAlwaysSame)+_T(" (")+Ztring::ToZtring(F2*100/File_Size, 0)+_T("%)"), true);
    }
    else
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(),  MediaInfoLib::Config.Language_Get(Ztring::ToZtring(F1, I3), Measure, MeasureIsAlwaysSame), true);
}

//---------------------------------------------------------------------------
//FileSize
void File__Analyze::Kilo_Kilo123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    const ZtringListList &List=MediaInfoLib::Config.Info_Get(StreamKind);
    size_t List_Pos=List.Find(Value+_T("/String"));
    if (List_Pos==Error)
        return;
    const Ztring &Target=Retrieve(StreamKind, StreamPos, List_Pos);
    if (!Target.empty())
        return;
    List_Pos=List.Find(Value);
    if (List_Pos==Error || List[List_Pos].size()<=Info_Measure || List[List_Pos][Info_Measure].empty())
        return;

    int32u BitRate=Retrieve(StreamKind, StreamPos, Value.To_Local().c_str()).To_int32u();
    if (BitRate==0)
        return;

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
        Ztring Measure=MediaInfoLib::Config.Info_Get(StreamKind).Read(Value, Info_Measure);
        Measure.insert(1, _T("K"));
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(BitRateS, Measure, true), true);
        return;
    }

    //Standard
    if (BitRate>10000000)
    {
        Ztring Measure=MediaInfoLib::Config.Info_Get(StreamKind).Read(Value, Info_Measure);
        Measure.insert(1, _T("M"));
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(((float)BitRate)/1000000, BitRate>100000000?0:1), Measure, true), true);
    }
    else if (BitRate>10000)
    {
        Ztring Measure=MediaInfoLib::Config.Info_Get(StreamKind).Read(Value, Info_Measure);
        Measure.insert(1, _T("K"));
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(((float)BitRate)/1000, BitRate>100000?0:1), Measure, true), true);
    }
    else if (BitRate>0)
        Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Ztring::ToZtring(BitRate), MediaInfoLib::Config.Info_Get(StreamKind).Read(Value, Info_Measure), true), true);
}

//---------------------------------------------------------------------------
//Value --> Value with measure
void File__Analyze::Value_Value123(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    const ZtringListList &List=MediaInfoLib::Config.Info_Get(StreamKind);
    size_t List_Pos=List.Find(Value+_T("/String"));
    if (List_Pos==Error)
        return;
    const Ztring &Target=Retrieve(StreamKind, StreamPos, List_Pos);
    if (!Target.empty())
        return;
    List_Pos=List.Find(Value);
    if (List_Pos==Error || List[List_Pos].size()<=Info_Measure || List[List_Pos][Info_Measure].empty())
        return;

    //Filling
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Retrieve(StreamKind, StreamPos, List_Pos), List[List_Pos][Info_Measure]), true);
}

//---------------------------------------------------------------------------
//Value --> Yes or No
void File__Analyze::YesNo_YesNo(const Ztring &Value, stream_t StreamKind, size_t StreamPos)
{
    const ZtringListList &List=MediaInfoLib::Config.Info_Get(StreamKind);
    size_t List_Pos=List.Find(Value+_T("/String"));
    if (List_Pos==Error)
        return;
    const Ztring &Target=Retrieve(StreamKind, StreamPos, List_Pos);
    if (!Target.empty())
        return;
    List_Pos=List.Find(Value);
    if (List_Pos==Error || List[List_Pos].size()<=Info_Measure || List[List_Pos][Info_Measure].empty())
        return;

    //Filling
    Fill(StreamKind, StreamPos, Ztring(Value+_T("/String")).To_Local().c_str(), MediaInfoLib::Config.Language_Get(Retrieve(StreamKind, StreamPos, List_Pos)), true);
}

//---------------------------------------------------------------------------
void File__Analyze::CodecID_Fill(const Ztring &Value, stream_t StreamKind, size_t StreamPos, infocodecid_format_t Format)
{
    Fill(StreamKind, StreamPos, "CodecID", Value, true);
    const Ztring &C1=MediaInfoLib::Config.CodecID_Get(StreamKind, Format, Value, InfoCodecID_Format);
    Fill(StreamKind, StreamPos, "Format", C1.empty()?Value:C1, true);
    Fill(StreamKind, StreamPos, "CodecID/Info"  , MediaInfoLib::Config.CodecID_Get(StreamKind, Format, Value, InfoCodecID_Description), true);
    Fill(StreamKind, StreamPos, "CodecID/Hint"  , MediaInfoLib::Config.CodecID_Get(StreamKind, Format, Value, InfoCodecID_Hint), true);
    Fill(StreamKind, StreamPos, "CodecID/Url"   , MediaInfoLib::Config.CodecID_Get(StreamKind, Format, Value, InfoCodecID_Url), true);
    if (StreamKind==Stream_General)
        Fill(StreamKind, StreamPos, "Format_Profile", MediaInfoLib::Config.CodecID_Get(StreamKind, Format, Value, InfoCodecID_Profile), true);
}

} //NameSpace

