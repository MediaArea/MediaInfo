// File__Analyze - Base for analyze files
// Copyright (C) 2007-2009 Jerome Martinez, Zen@MediaArea.net
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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "ZenLib/File.h"
#include "ZenLib/BitStream_LE.h"
#include <cmath>
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Preparation des streams
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Analyze::Stream_Prepare (stream_t KindOfStream)
{
    //Integrity
    if (KindOfStream>Stream_Max)
        return Error;

    //Clear
    if (KindOfStream==Stream_Max)
    {
        StreamKind_Last=Stream_Max;
        StreamPos_Last=(size_t)-1;
        return 0;
    }

    //Add a stream
    (*Stream)[KindOfStream].resize((*Stream)[KindOfStream].size()+1);
    (*Stream_More)[KindOfStream].resize((*Stream_More)[KindOfStream].size()+1);

    StreamKind_Last=KindOfStream;
    StreamPos_Last=(*Stream)[KindOfStream].size()-1;

    //Filling basic info
    Fill(StreamKind_Last, StreamPos_Last, "Count", Count_Get(StreamKind_Last, StreamPos_Last));
    Fill(StreamKind_Last, StreamPos_Last, General_StreamKind, MediaInfoLib::Config.Info_Get(StreamKind_Last).Read(General_StreamKind, Info_Text));
    Fill(StreamKind_Last, StreamPos_Last, General_StreamKind_String, MediaInfoLib::Config.Language_Get(MediaInfoLib::Config.Info_Get(StreamKind_Last).Read(General_StreamKind, Info_Text)), true);
    Fill(StreamKind_Last, StreamPos_Last, General_StreamKindID, StreamPos_Last);
    for (size_t Pos=0; Pos<=StreamPos_Last; Pos++)
    {
        Fill(StreamKind_Last, Pos, General_StreamCount, Count_Get(StreamKind_Last), 10, true);
        if (Count_Get(StreamKind_Last)>1)
            Fill(StreamKind_Last, Pos, General_StreamKindPos, Pos+1, 10, true);
    }

    //Filling Lists & Counts
    if (!IsSub && KindOfStream!=Stream_General)
    {
        const Ztring& StreamKind_Text=Get(KindOfStream, 0, General_StreamKind, Info_Text);
        if (Count_Get(KindOfStream)>1)
        {
            Ztring Temp;
            Temp=Retrieve(Stream_General, 0, Ztring(StreamKind_Text+_T("_Codec_List")).To_Local().c_str())+_T(" / ");
            Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Codec_List")).To_Local().c_str(), Temp, true);
            Temp=Retrieve(Stream_General, 0, Ztring(StreamKind_Text+_T("_Language_List")).To_Local().c_str())+_T(" / ");
            Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Language_List")).To_Local().c_str(), Temp, true);
            Temp=Retrieve(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_List")).To_Local().c_str())+_T(" / ");
            Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_List")).To_Local().c_str(), Temp, true);
            Temp=Retrieve(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_WithHint_List")).To_Local().c_str())+_T(" / ");
            Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_WithHint_List")).To_Local().c_str(), Temp, true);
        }

        Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("Count")).To_Local().c_str(), StreamPos_Last+1, 10, true);
    }

    //Fill with already ready data
    for (size_t Pos=0; Pos<Fill_Temp.size(); Pos++)
        if (Fill_Temp(Pos, 0).IsNumber())
            Fill(StreamKind_Last, StreamPos_Last, Fill_Temp(Pos, 0).To_int32u(), Fill_Temp(Pos, 1));
        else
            Fill(StreamKind_Last, StreamPos_Last, Fill_Temp(Pos, 0).To_UTF8().c_str(), Fill_Temp(Pos, 1));
    Fill_Temp.clear();

    return (*Stream)[KindOfStream].size()-1; //The position in the stream count
}

//***************************************************************************
// Filling
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Fill (stream_t StreamKind, size_t StreamPos, size_t Parameter, const Ztring &Value, bool Replace)
{
    //Integrity
    if (StreamKind>Stream_Max)
        return;

    //Handling values with \r\n inside
    if (Value.find(_T('\r'))!=string::npos || Value.find(_T('\n'))!=string::npos)
    {
        Ztring NewValue=Value;
        NewValue.FindAndReplace(_T("\r\n"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\r"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\n"), _T(" / "), 0, Ztring_Recursive);
        if (NewValue.size()>=3 && NewValue.rfind(_T(" / "))==NewValue.size()-3)
            NewValue.resize(NewValue.size()-3);
        Fill(StreamKind, StreamPos, Parameter, NewValue, Replace);
        return;
    }

    //Handle Value before StreamKind
    if (StreamKind==Stream_Max || StreamPos>=(*Stream)[StreamKind].size())
    {
        ZtringList NewList;
        NewList.push_back(Ztring().From_Number(Parameter));
        NewList.push_back(Value);
        Fill_Temp.push_back(NewList);
        return; //No streams
    }

    Ztring &Target=(*Stream)[StreamKind][StreamPos](Parameter);
    if (Target.empty() || Replace)
        Target=Value; //First value
    else if (Value.empty())
        Target.clear(); //Empty value --> clear other values
    else
    {
        Target+=MediaInfoLib::Config.TagSeparator_Get();
        Target+=Value;
    }

    //Human readable
    if (!IsSub)
    {
        if (MediaInfoLib::Config.ReadByHuman_Get())
        {
            //Strings
            const Ztring &List_Measure_Value=MediaInfoLib::Config.Info_Get(StreamKind).Read(Parameter, Info_Measure);
            const Ztring &List_Name_Value=MediaInfoLib::Config.Info_Get(StreamKind).Read(Parameter, Info_Name);
                 if (List_Measure_Value==_T(" byte"))
                FileSize_FileSize123(List_Name_Value, StreamKind, StreamPos);
            else if (List_Measure_Value==_T(" bps") || List_Measure_Value==_T(" Hz"))
                Kilo_Kilo123(List_Name_Value, StreamKind, StreamPos);
            else if (List_Measure_Value==_T(" ms"))
                Duration_Duration123(List_Name_Value, StreamKind, StreamPos);
            else if (List_Measure_Value==_T("Yes"))
                YesNo_YesNo(List_Name_Value, StreamKind, StreamPos);
            else
                Value_Value123(List_Name_Value, StreamKind, StreamPos);

            //Lists
            if (StreamKind!=Stream_General)
            {
                const Ztring& Parameter_Text=MediaInfoLib::Config.Info_Get(StreamKind).Read(Parameter, Info_Name);
                if (Parameter_Text==_T("Codec/String")
                 || Parameter_Text==_T("Language/String")
                 || Parameter_Text==_T("Format")
                 || Parameter_Text==_T("CodecID/Hint"))
                {
                    Ztring Temp1, Temp2;
                    for (size_t StreamPos=0; StreamPos<(*Stream)[StreamKind].size(); StreamPos++)
                    {
                        if (Parameter_Text==_T("CodecID/Hint"))
                            Temp1+=Retrieve(StreamKind, StreamPos, "Format")+_T(" / ");
                        else
                            Temp1+=Retrieve(StreamKind, StreamPos, Parameter)+_T(" / ");
                        if (Parameter_Text==_T("Format")
                         || Parameter_Text==_T("CodecID/Hint"))
                        {
                            Temp2+=Retrieve(StreamKind, StreamPos, "Format");
                            if (!Retrieve(StreamKind, StreamPos, "CodecID/Hint").empty())
                            {
                                Temp2+=_T(" (");
                                Temp2+=Retrieve(StreamKind, StreamPos, "CodecID/Hint");
                                Temp2+=_T(")");
                            }
                            Temp2+=_T(" / ");
                        }
                    }
                    if (!Temp1.empty())
                        Temp1.resize(Temp1.size()-3); //Delete extra " / "
                    if (!Temp2.empty())
                        Temp2.resize(Temp2.size()-3); //Delete extra " / "
                    Ztring StreamKind_Text=Get(StreamKind, 0, General_StreamKind, Info_Text);
                    if (Parameter_Text==_T("Codec/String"))
                        Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Codec_List")).To_Local().c_str(), Temp1, true);
                    if (Parameter_Text==_T("Language/String"))
                        Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Language_List")).To_Local().c_str(), Temp1, true);
                    if (Parameter_Text==_T("Format")
                     || Parameter_Text==_T("CodecID/Hint"))
                    {
                        Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_List")).To_Local().c_str(), Temp1, true);
                        Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_WithHint_List")).To_Local().c_str(), Temp2, true);
                    }
                }
            }

            //BitRate_Mode / OverallBitRate_Mode
            if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==(StreamKind==Stream_General?_T("OverallBitRate_Mode"):_T("BitRate_Mode")) && MediaInfoLib::Config.ReadByHuman_Get())
            {
                Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("BitRate_Mode_"))+Value);
                Fill(StreamKind, StreamPos, StreamKind==Stream_General?"OverallBitRate_Mode/String":"BitRate_Mode/String", Translated.find(_T("BitRate_Mode_"))?Translated:Value, true);
            }

            //Encoded_Library
            if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Encoded_Library")
             || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Encoded_Library/Name")
             || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Encoded_Library/Version")
             || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Encoded_Library/Date"))
            {
                const Ztring& Name=Retrieve(StreamKind, StreamPos, "Encoded_Library/Name");
                const Ztring& Version=Retrieve(StreamKind, StreamPos, "Encoded_Library/Version");
                const Ztring& Date=Retrieve(StreamKind, StreamPos, "Encoded_Library/Date");
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
                    Fill(StreamKind, StreamPos, "Encoded_Library/String", String, true);
                }
                else
                    Fill(StreamKind, StreamPos, "Encoded_Library/String", Retrieve(StreamKind, StreamPos, "Encoded_Library"), true);
            }

            //Format_Settings_Matrix
            if (StreamKind==Stream_Video && Parameter==Video_Format_Settings_Matrix)
            {
                Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("Format_Settings_Matrix_"))+Value);
                Fill(Stream_Video, StreamPos, Video_Format_Settings_Matrix_String, Translated.find(_T("Format_Settings_Matrix_"))?Translated:Value, true);
            }

            //Scan type
            if (StreamKind==Stream_Video && Parameter==Video_ScanType)
            {
                Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("Interlaced_"))+Value);
                Fill(Stream_Video, StreamPos, Video_ScanType_String, Translated.find(_T("Interlaced_"))?Translated:Value, true);
            }

            //Scan order
            if (StreamKind==Stream_Video && Parameter==Video_ScanOrder)
            {
                Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("Interlaced_"))+Value);
                Fill(Stream_Video, StreamPos, Video_ScanOrder_String, Translated.find(_T("Interlaced_"))?Translated:Value, true);
            }

            //Interlacement
            if (StreamKind==Stream_Video && Parameter==Video_Interlacement)
            {
                const Ztring &Z1=Retrieve(Stream_Video, StreamPos, Video_Interlacement);
                if (Z1.size()==3)
                    Fill(Stream_Video, StreamPos, Video_Interlacement_String, MediaInfoLib::Config.Language_Get(Ztring(_T("Interlaced_"))+Z1), true);
                else
                    Fill(Stream_Video, StreamPos, Video_Interlacement_String, MediaInfoLib::Config.Language_Get(Z1), true);
                if (Retrieve(Stream_Video, StreamPos, Video_Interlacement_String).empty())
                    Fill(Stream_Video, StreamPos, Video_Interlacement_String, Z1, true);
            }

            //FrameRate_Mode
            if (StreamKind==Stream_Video && Parameter==Video_FrameRate_Mode)
            {
                Ztring Translated=MediaInfoLib::Config.Language_Get(Ztring(_T("FrameRate_Mode_"))+Value);
                Fill(Stream_Video, StreamPos, Video_FrameRate_Mode_String, Translated.find(_T("FrameRate_Mode_"))?Translated:Value, true);
            }
        }

        //Filling Lists & Counts
        if (StreamKind!=Stream_General && (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Codec")
                                        || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Language/String")
                                        || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Format")
                                        || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("CodecID/Hint")
            ))
        {
            const Ztring& StreamKind_Text=Get(StreamKind, 0, General_StreamKind, Info_Text);
            Ztring Temp1, Temp2;
            for (size_t Pos=0; Pos<Count_Get(StreamKind); Pos++)
            {
                if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Format")
                 || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("CodecID/Hint"))
                {
                    Temp1+=Retrieve(StreamKind, Pos, "Format")+_T(" / ");
                    Temp2+=Retrieve(StreamKind, Pos, "Format");
                    if (!Retrieve(StreamKind, Pos, "CodecID/Hint").empty())
                    {
                        Temp2+=_T(" (");
                        Temp2+=Retrieve(StreamKind, Pos, "CodecID/Hint");
                        Temp2+=_T(")");
                    }
                    Temp2+=_T(" / ");
                }
                else
                    Temp1+=Retrieve(StreamKind, Pos, Parameter)+_T(" / ");
            }
            Temp1.resize(Temp1.size()-3); //Removing last " / "
            if (!Temp2.empty())
                Temp2.resize(Temp2.size()-3); //Removing last " / "

            if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Codec"))
                Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Codec_List")).To_Local().c_str(), Temp1, true);
            if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Language/String"))
                Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Language_List")).To_Local().c_str(), Temp1, true);
            if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Format")
             || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("CodecID/Hint"))
            {
                Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_List")).To_Local().c_str(), Temp1, true);
                Fill(Stream_General, 0, Ztring(StreamKind_Text+_T("_Format_WithHint_List")).To_Local().c_str(), Temp2, true);
            }
        }

        //ID
        if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("ID"))
            Fill(StreamKind, StreamPos, "ID/String", Value, true);

        //Format
        if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Format"))
        {
            Fill(StreamKind, StreamPos, "Format/Info", MediaInfoLib::Config.Format_Get(Retrieve(StreamKind, StreamPos, "Format"), InfoFormat_Info), true);
            Fill(StreamKind, StreamPos, "Format/Url" , MediaInfoLib::Config.Format_Get(Retrieve(StreamKind, StreamPos, "Format"), InfoFormat_Url ), true);
            if (StreamKind==Stream_General)
            {
                Fill(Stream_General, 0, General_Format_Extensions, MediaInfoLib::Config.Format_Get(Value, InfoFormat_Extensions), true);
                Fill(Stream_General, 0, General_Format_String, Value, true);
                Fill(Stream_General, 0, General_Codec, Value, true);
                Fill(Stream_General, 0, General_Codec_String, Value, true);
            }
        }
        if (StreamKind==Stream_General && Parameter==General_Format_Info)
            (*Stream)[Stream_General][0](General_Codec_Info)=Value;
        if (StreamKind==Stream_General && Parameter==General_Format_Url)
            (*Stream)[Stream_General][0](General_Codec_Url)=Value;
        if (StreamKind==Stream_General && Parameter==General_Format_Extensions)
            (*Stream)[Stream_General][0](General_Codec_Extensions)=Value;
        if (StreamKind==Stream_General && Parameter==General_Format_Settings)
            (*Stream)[Stream_General][0](General_Codec_Settings)=Value;

        //Codec
        if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Codec"))
        {
            const Ztring &C1=MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, StreamPos, "Codec"), InfoCodec_Name, (stream_t)StreamKind);
            if (C1.empty())
                Fill(StreamKind, StreamPos, "Codec/String", Value, StreamKind!=Stream_Menu); //Specific for Menu: multiple codecs
            else
            {
                Fill(StreamKind, StreamPos, "Codec/String", C1, StreamKind!=Stream_Menu); //Specific for Menu: multiple codecs
                Fill(StreamKind, StreamPos, "Codec/Family", MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, StreamPos, "Codec"), InfoCodec_KindofCodec, StreamKind), true);
                Fill(StreamKind, StreamPos, "Codec/Info"  , MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, StreamPos, "Codec"), InfoCodec_Description, StreamKind), true);
                Fill(StreamKind, StreamPos, "Codec/Url"   , MediaInfoLib::Config.Codec_Get(Retrieve(StreamKind, StreamPos, "Codec"), InfoCodec_Url,         StreamKind), true);
            }
        }

        //CodecID_Description
        if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("CodecID/Info") && Retrieve(StreamKind, StreamPos, "CodecID_Description")==Value)
            Clear(StreamKind, StreamPos, "CodecID_Description");

        //BitRate from BitRate_Nominal
        if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("BitRate")
         || Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("BitRate_Nominal"))
        {
            float32 BitRate=Retrieve(StreamKind, StreamPos, "BitRate").To_float32();
            float32 BitRate_Nominal=Retrieve(StreamKind, StreamPos, "BitRate_Nominal").To_float32();
            if (BitRate_Nominal>BitRate*0.995 && BitRate_Nominal<BitRate*1.005)
            {
                Ztring Temp=Retrieve(StreamKind, StreamPos, "BitRate_Nominal");
                Clear(StreamKind, StreamPos, "BitRate_Nominal");
                Fill(StreamKind, StreamPos, "BitRate", Temp, true);
            }
        }

        //Language
        //-Find 2-digit language
        if (Retrieve(StreamKind, StreamPos, Parameter, Info_Name)==_T("Language"))
        {
            ZtringList Languages, Languages_Translated;
            Languages.Separator_Set(0, _T(" / "));
            Languages_Translated.Separator_Set(0, _T(" / "));
            size_t Languages_Count=1, Languages_Pos=0;
            Ztring C=(*Stream)[StreamKind][StreamPos][Parameter];
            while ((Languages_Pos=(*Stream)[StreamKind][StreamPos][Parameter].find(_T(" / "), Languages_Pos))!=string::npos)
            {
                Languages_Pos+=3;
                Languages_Count++;
            }
            Languages.Write((*Stream)[StreamKind][StreamPos][Parameter]);
            Languages.resize(Languages_Count);
            for (size_t Pos=0; Pos<Languages.size(); Pos++)
            {
                Languages[Pos].MakeLowerCase();
                if (Languages[Pos].size()==3 && (Languages[Pos]==_T("mis") || Languages[Pos]==_T("und") || Languages[Pos]==_T("???") || Languages[Pos]==_T("   "))
                 || Languages[Pos].size()==2 && Languages[Pos]==_T("  "))
                {
                    Languages[Pos].clear();
                }
                if (!Languages[Pos].empty() && !IsSub && MediaInfoLib::Config.ReadByHuman_Get())
                {
                    if (Languages[Pos].size()==3 && !MediaInfoLib::Config.Iso639_Get(Languages[Pos]).empty())
                        Languages[Pos]=MediaInfoLib::Config.Iso639_Get(Languages[Pos]);
                    if (Languages[Pos].size()>3 && !MediaInfoLib::Config.Iso639_Find(Languages[Pos]).empty())
                        Languages[Pos]=MediaInfoLib::Config.Iso639_Find(Languages[Pos]);

                    //Translate
                    if (Languages[Pos].size()==2 || Languages[Pos].size()==3)
                    {
                        Ztring Temp=_T("Language_"); Temp+=Languages[Pos];
                        const Ztring& Z3=MediaInfoLib::Config.Language_Get(Temp);
                        Languages_Translated(Pos)=Z3.find(_T("Language_"))==0?Languages[Pos]:Z3;
                    }
                    /*else if (Languages[Pos].size()==5 && Languages[Pos][2]==_T('-'))
                    {
                        Ztring Temp=_T("Language_"); Temp+=Languages[Pos].substr(0, 2);
                        const Ztring& Z3=MediaInfoLib::Config.Language_Get(Temp);
                        Languages_Translated(Pos)=Z3.find(_T("Language_"))==0?Languages[Pos].substr(0, 2):Z3;
                        Languages_Translated(Pos)+=_T(" (");
                        Languages_Translated(Pos)+=Ztring(Languages[Pos].substr(3, 2)).MakeUpperCase();
                        Languages_Translated(Pos)+=_T(")");
                    }
                    else if (Languages[Pos].size()>2)
                    {
                        Ztring Temp=_T("Language_"); Temp+=Languages[Pos].substr(0, 2);
                        const Ztring& Z3=MediaInfoLib::Config.Language_Get(Temp);
                        Languages_Translated(Pos)=Z3.find(_T("Language_"))==0?Languages[Pos].substr(0, 2):Z3;
                    }*/
                    else
                        Languages_Translated(Pos)=Languages[Pos];
                }
                else
                    Languages_Translated(Pos).clear();
            }
            Ztring Languages_Temp, Languages_Translated_Temp;
            for (size_t Pos=0; Pos<Languages.size(); Pos++)
            {
                Languages_Temp+=Languages[Pos]+_T(" / ");
                Languages_Translated_Temp+=Languages_Translated[Pos]+_T(" / ");
            }
            if (Languages_Temp.size()>=3)
                Languages_Temp.resize(Languages_Temp.size()-3);
            if (Languages_Translated_Temp.size()>=3)
                Languages_Translated_Temp.resize(Languages_Translated_Temp.size()-3);
            (*Stream)[StreamKind][StreamPos](Parameter)=Languages_Temp;
            Fill(StreamKind, StreamPos, Parameter+1, Languages_Translated_Temp); //"Language/String"
        }

        //FrameRate Nominal
        if (StreamKind==Stream_Video && (Parameter==Video_FrameRate || Parameter==Video_FrameRate_Nominal))
        {
            float32 FrameRate=Retrieve(Stream_Video, StreamPos, Video_FrameRate).To_float32();
            float32 FrameRate_Nominal=Retrieve(Stream_Video, StreamPos, Video_FrameRate_Nominal).To_float32();
            if (FrameRate_Nominal>FrameRate*0.995 && FrameRate_Nominal<FrameRate*1.005)
            {
                Ztring Temp=Retrieve(StreamKind, StreamPos, Video_FrameRate_Nominal);
                Clear(StreamKind, StreamPos, Video_FrameRate_Nominal);
                if (Parameter==Video_FrameRate)
                    Fill(StreamKind, StreamPos, Parameter, Temp, true);
            }
        }

        //Well known framerate values
        if (StreamKind==Stream_Video && (Parameter==Video_FrameRate || Parameter==Video_FrameRate_Nominal || Parameter==Video_FrameRate_Original))
        {
            Finalize_Video_FrameRate(StreamPos, (video)Parameter);
            if (Retrieve(Stream_Video, StreamPos, Video_FrameRate_Nominal)==Retrieve(Stream_Video, StreamPos, Video_FrameRate))
                Clear(Stream_Video, StreamPos, Video_FrameRate_Nominal);
            if (Retrieve(Stream_Video, StreamPos, Video_FrameRate_Original)==Retrieve(Stream_Video, StreamPos, Video_FrameRate))
                Clear(Stream_Video, StreamPos, Video_FrameRate_Original);
        }

        //Display Aspect Ratio and Pixel Aspect Ratio
        if (StreamKind==Stream_Video && Parameter==Video_DisplayAspectRatio && Retrieve(Stream_Video, StreamPos, Video_DisplayAspectRatio_Original)==Value)
            Clear(Stream_Video, StreamPos, Video_DisplayAspectRatio_Original);
        if (StreamKind==Stream_Video && Parameter==Video_DisplayAspectRatio_String && Retrieve(Stream_Video, StreamPos, Video_DisplayAspectRatio_Original_String)==Value)
            Clear(Stream_Video, StreamPos, Video_DisplayAspectRatio_Original_String);
        if (StreamKind==Stream_Video && Parameter==Video_DisplayAspectRatio_Original_String && Retrieve(Stream_Video, StreamPos, Video_DisplayAspectRatio_String)==Value)
            Clear(Stream_Video, StreamPos, Video_DisplayAspectRatio_Original_String);
        if (StreamKind==Stream_Video && Parameter==Video_PixelAspectRatio && Retrieve(Stream_Video, StreamPos, Video_PixelAspectRatio_Original)==Value)
            Clear(Stream_Video, StreamPos, Video_PixelAspectRatio_Original);
        if (StreamKind==Stream_Video && Parameter==Video_PixelAspectRatio_String && Retrieve(Stream_Video, StreamPos, Video_PixelAspectRatio_Original_String)==Value)
            Clear(Stream_Video, StreamPos, Video_PixelAspectRatio_Original_String);
        if (StreamKind==Stream_Video && Parameter==Video_PixelAspectRatio_Original_String && Retrieve(Stream_Video, StreamPos, Video_PixelAspectRatio_String)==Value)
            Clear(Stream_Video, StreamPos, Video_PixelAspectRatio_Original_String);

        //Special audio cases
        if (StreamKind==Stream_Audio && Parameter==Audio_CodecID
         && Retrieve(Stream_Audio, StreamPos, Audio_Channel_s_).empty()
         &&(Value==_T("samr")
         || Value==_T("sawb")
         || Value==_T("7A21")
         || Value==_T("7A22"))
            )
            Fill(Stream_Audio, StreamPos, Audio_Channel_s_, 1, 10, true); //AMR is always with 1 channel

        //Well known bitrate values
        if (StreamKind==Stream_Audio && (Parameter==Audio_BitRate || Parameter==Audio_BitRate_Nominal))
            Finalize_Audio_BitRate(StreamPos, (audio)Parameter);
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const Ztring &Value, bool Replace)
{
    //Integrity
    if (StreamKind>Stream_Max || Parameter==NULL || Parameter[0]=='\0')
        return;

    //Handling values with \r\n inside
    if (Value.find(_T('\r'))!=string::npos || Value.find(_T('\n'))!=string::npos)
    {
        Ztring NewValue=Value;
        NewValue.FindAndReplace(_T("\r\n"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\r"), _T(" / "), 0, Ztring_Recursive);
        NewValue.FindAndReplace(_T("\n"), _T(" / "), 0, Ztring_Recursive);
        if (NewValue.size()>=3 && NewValue.rfind(_T(" / "))==NewValue.size()-3)
            NewValue.resize(NewValue.size()-3);
        Fill(StreamKind, StreamPos, Parameter, NewValue, Replace);
        return;
    }

    //Handle Value before StreamKind
    if (StreamKind==Stream_Max || StreamPos>=(*Stream)[StreamKind].size())
    {
        ZtringList NewList;
        NewList.push_back(Ztring().From_UTF8(Parameter));
        NewList.push_back(Value);
        Fill_Temp.push_back(NewList);
        return; //No streams
    }

    //Handling of well known parameters
    size_t Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Ztring().From_Local(Parameter));
    if (Pos!=Error)
    {
        Fill(StreamKind, StreamPos, Pos, Value, Replace);
        return;
    }

    //Handling of unknown parameters
    if (Value.empty())
    {
        if (Replace)
        {
            size_t Pos=(*Stream_More)[StreamKind][StreamPos].Find(Ztring().From_UTF8(Parameter), Info_Name);
            if (Pos!=(size_t)-1)
                (*Stream_More)[StreamKind][StreamPos].erase((*Stream_More)[StreamKind][StreamPos].begin()+Pos); //Empty value --> remove the line
        }
    }
    else
    {
        Ztring &Target=(*Stream_More)[StreamKind][StreamPos](Ztring().From_Local(Parameter), Info_Text);
        if (Target.empty() || Replace)
        {
            Target=Value; //First value
            (*Stream_More)[StreamKind][StreamPos](Ztring().From_Local(Parameter), Info_Options)=_T("Y NT");
        }
        else
        {
            Target+=MediaInfoLib::Config.TagSeparator_Get();
            Target+=Value;
        }
    }
    Fill(StreamKind, StreamPos, "Count", Count_Get(StreamKind, StreamPos), 10, true);
}

//---------------------------------------------------------------------------
const Ztring &File__Analyze::Retrieve (stream_t StreamKind, size_t StreamPos, size_t Parameter, info_t KindOfInfo)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size()
     || Parameter>=(*Stream)[StreamKind][StreamPos].size())
        return MediaInfoLib::Config.EmptyString_Get();

    if (KindOfInfo!=Info_Text)
        return MediaInfoLib::Config.Info_Get(StreamKind, Parameter, KindOfInfo);
    return (*Stream)[StreamKind][StreamPos](Parameter);
}

//---------------------------------------------------------------------------
const Ztring &File__Analyze::Retrieve (stream_t StreamKind, size_t StreamPos, const char* Parameter, info_t KindOfInfo)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size()
     || Parameter==NULL
     || Parameter[0]=='\0')
        return MediaInfoLib::Config.EmptyString_Get();

    if (KindOfInfo!=Info_Text)
        return MediaInfoLib::Config.Info_Get(StreamKind, Parameter, KindOfInfo);
    size_t Parameter_Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Ztring().From_Local(Parameter));
    if (Parameter_Pos==Error)
    {
        Parameter_Pos=(*Stream_More)[StreamKind][StreamPos].Find(Ztring().From_Local(Parameter));
        if (Parameter_Pos==Error)
            return MediaInfoLib::Config.EmptyString_Get();
        return (*Stream_More)[StreamKind][StreamPos](Parameter_Pos, 1);
    }
    return (*Stream)[StreamKind][StreamPos](Parameter_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind, size_t StreamPos, const char* Parameter)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size()
     || Parameter==NULL
     || Parameter[0]=='\0')
        return;

    size_t Parameter_Pos=MediaInfoLib::Config.Info_Get(StreamKind).Find(Ztring().From_Local(Parameter));
    if (Parameter_Pos==Error)
    {
        Parameter_Pos=(*Stream_More)[StreamKind][StreamPos].Find(Ztring().From_Local(Parameter));
        if (Parameter_Pos==Error)
            return;
        (*Stream_More)[StreamKind][StreamPos](Parameter_Pos, 1).clear();
        return;
    }

    Clear(StreamKind, StreamPos, Parameter_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind, size_t StreamPos, size_t Parameter)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size())
        return;

    //Normal
    if (Parameter<(*Stream)[StreamKind][StreamPos].size())
    {
        (*Stream)[StreamKind][StreamPos][Parameter].clear();

        //Human readable
        if (MediaInfoLib::Config.ReadByHuman_Get())
        {
            //Strings
            const Ztring &List_Measure_Value=MediaInfoLib::Config.Info_Get(StreamKind).Read(Parameter, Info_Measure);
                 if (List_Measure_Value==_T(" byte"))
            {
                for (size_t Pos=Parameter+1; Pos<=Parameter+5; Pos++)
                    if (Pos<(*Stream)[StreamKind][StreamPos].size())
                        (*Stream)[StreamKind][StreamPos][Pos].clear();
            }
            else if (List_Measure_Value==_T(" bps") || List_Measure_Value==_T(" Hz"))
            {
                if (Parameter+1<(*Stream)[StreamKind][StreamPos].size())
                    (*Stream)[StreamKind][StreamPos][Parameter+1].clear();
            }
            else if (List_Measure_Value==_T(" ms"))
            {
                for (size_t Pos=Parameter+1; Pos<=Parameter+4; Pos++)
                    if (Pos<(*Stream)[StreamKind][StreamPos].size())
                        (*Stream)[StreamKind][StreamPos][Pos].clear();
            }
            else if (List_Measure_Value==_T("Yes"))
            {
                if (Parameter+1<(*Stream)[StreamKind][StreamPos].size())
                    (*Stream)[StreamKind][StreamPos][Parameter+1].clear();
            }
            else if (!List_Measure_Value.empty())
            {
                if (Parameter+1<(*Stream)[StreamKind][StreamPos].size())
                    (*Stream)[StreamKind][StreamPos][Parameter+1].clear();
            }
        }

        return;
    }

    //More
    Parameter-=(*Stream)[StreamKind][StreamPos].size(); //For having Stream_More position
    if (Parameter<(*Stream_More)[StreamKind][StreamPos].size())
    {
        (*Stream_More)[StreamKind][StreamPos].erase((*Stream_More)[StreamKind][StreamPos].begin()+Parameter);
        return;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind, size_t StreamPos)
{
    //Integrity
    if (StreamKind>=Stream_Max
     || StreamPos>=(*Stream)[StreamKind].size())
        return;

    (*Stream)[StreamKind].erase((*Stream)[StreamKind].begin()+StreamPos);
}

//---------------------------------------------------------------------------
void File__Analyze::Clear (stream_t StreamKind)
{
    //Integrity
    if (StreamKind>=Stream_Max)
        return;

    (*Stream)[StreamKind].clear();
}

//---------------------------------------------------------------------------
void File__Analyze::Fill_Flush()
{
    Stream_Prepare(Stream_Max); //clear filling
    Fill_Temp.clear();
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(MediaInfo_Internal &ToAdd, bool Erase)
{
    size_t Count=0;
    for (size_t StreamKind=(size_t)Stream_General; StreamKind<(size_t)Stream_Max; StreamKind++)
    {
        size_t StreamPos_Count=ToAdd.Count_Get((stream_t)StreamKind);
        for (size_t StreamPos=0; StreamPos<StreamPos_Count; StreamPos++)
        {
            //Prepare a new stream
            Stream_Prepare((stream_t)StreamKind);

            //Merge
            size_t Pos_Count=ToAdd.Count_Get((stream_t)StreamKind, StreamPos);
            for (size_t Pos=0; Pos<Pos_Count; Pos++)
            {
                Fill((stream_t)StreamKind, StreamPos, Ztring(ToAdd.Get((stream_t)StreamKind, StreamPos, Pos, Info_Name)).To_UTF8().c_str(), ToAdd.Get((stream_t)StreamKind, StreamPos, Pos), true);
            }

            //Clearing duplicates
            Clear((stream_t)StreamKind, StreamPos, "Count");
            Clear((stream_t)StreamKind, StreamPos, "StreamCount");
            Clear((stream_t)StreamKind, StreamPos, "StreamKind");
            Clear((stream_t)StreamKind, StreamPos, "StreamKind/String");
            Clear((stream_t)StreamKind, StreamPos, "StreamKindID");
            Clear((stream_t)StreamKind, StreamPos, "StreamKindPos");

            if ((stream_t)StreamKind==Stream_General)
            {
                Clear(Stream_General, StreamPos, (size_t)General_GeneralCount);
                Clear(Stream_General, StreamPos, (size_t)General_VideoCount);
                Clear(Stream_General, StreamPos, (size_t)General_AudioCount);
                Clear(Stream_General, StreamPos, (size_t)General_TextCount);
                Clear(Stream_General, StreamPos, (size_t)General_ChaptersCount);
                Clear(Stream_General, StreamPos, (size_t)General_ImageCount);
                Clear(Stream_General, StreamPos, (size_t)General_MenuCount);
                Clear(Stream_General, StreamPos, (size_t)General_CompleteName);
                Clear(Stream_General, StreamPos, (size_t)General_FolderName);
                Clear(Stream_General, StreamPos, (size_t)General_FileName);
                Clear(Stream_General, StreamPos, (size_t)General_FileExtension);
                Clear(Stream_General, StreamPos, (size_t)General_File_Created_Date);
                Clear(Stream_General, StreamPos, (size_t)General_File_Created_Date_Local);
                Clear(Stream_General, StreamPos, (size_t)General_File_Modified_Date);
                Clear(Stream_General, StreamPos, (size_t)General_File_Modified_Date_Local);
            }

            Count++;
        }
    }

    return Count;
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(File__Analyze &ToAdd, bool Erase)
{
    size_t Count=0;
    for (size_t StreamKind=(size_t)Stream_General+1; StreamKind<(size_t)Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<(*ToAdd.Stream)[StreamKind].size(); StreamPos++)
        {
            //Prepare a new stream
            Stream_Prepare((stream_t)StreamKind);

            //Merge
            Merge(ToAdd, (stream_t)StreamKind, StreamPos, StreamPos_Last, Erase);

            Count++;
        }
    return Count;
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(File__Analyze &ToAdd, stream_t StreamKind, size_t StreamPos_From, size_t StreamPos_To, bool Erase)
{
    //Integrity
    if (&ToAdd==NULL || StreamKind>=Stream_Max || !ToAdd.Stream || StreamPos_From>=(*ToAdd.Stream)[StreamKind].size())
        return 0;

    //Destination
    while (StreamPos_To>=(*Stream)[StreamKind].size())
        Stream_Prepare(StreamKind);

    //Specific stuff
    Ztring FrameRate_Temp, PixelAspectRatio_Temp, DisplayAspectRatio_Temp;
    if (StreamKind==Stream_Video)
    {
        PixelAspectRatio_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_PixelAspectRatio); //We want to keep the PixelAspectRatio_Temp of the video stream
        DisplayAspectRatio_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio); //We want to keep the DisplayAspectRatio_Temp of the video stream
        FrameRate_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate); //We want to keep the FrameRate of AVI 120 fps
    }

    //Merging
    size_t Count=0;
    size_t Size=ToAdd.Count_Get(StreamKind, StreamPos_From);
    for (size_t Pos=General_Inform; Pos<Size; Pos++)
    {
        const Ztring &ToFill_Value=ToAdd.Get(StreamKind, StreamPos_From, Pos);
        if (!ToFill_Value.empty() && (Erase || Get(StreamKind, StreamPos_From, Pos).empty()))
        {
            if (Pos<MediaInfoLib::Config.Info_Get(StreamKind).size())
                Fill(StreamKind, StreamPos_To, Pos, ToFill_Value, true);
            else
                Fill(StreamKind, StreamPos_To, ToAdd.Get(StreamKind, StreamPos_From, Pos, Info_Name).To_UTF8().c_str(), ToFill_Value, true);

            Count++;
        }
    }

    //Clearing
    for (size_t Pos=Size-1; Pos>=General_Inform; Pos--) //Descendant because Clear() remove the unknown fields
    {
        const Ztring &ToFill_Value=ToAdd.Get(StreamKind, StreamPos_From, Pos);
        if (!ToFill_Value.empty())
            ToAdd.Clear(StreamKind, StreamPos_From, Pos);
    }

    //Specific stuff
    if (StreamKind==Stream_Video)
    {
        if (!PixelAspectRatio_Temp.empty() || !DisplayAspectRatio_Temp.empty())
        {
            if (PixelAspectRatio_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_PixelAspectRatio))
            {
                Ztring Temp=Retrieve(Stream_Video, StreamPos_Last, Video_PixelAspectRatio);
                Clear(Stream_Video, StreamPos_Last, Video_PixelAspectRatio);
                Fill(Stream_Video, StreamPos_Last, Video_PixelAspectRatio_Original, Temp, true);
            }
            Fill(Stream_Video, StreamPos_Last, Video_PixelAspectRatio, PixelAspectRatio_Temp, true);
        }
        if (!DisplayAspectRatio_Temp.empty() || !PixelAspectRatio_Temp.empty())
        {
            if (DisplayAspectRatio_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio))
            {
                Ztring Temp=Retrieve(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio);
                Clear(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio);
                Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio_Original, Temp, true);
            }
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, DisplayAspectRatio_Temp, true);
        }
        if (!FrameRate_Temp.empty())
        {
            if (FrameRate_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate))
            {
                Ztring Temp=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate);
                Clear(Stream_Video, StreamPos_Last, Video_FrameRate);
                Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Original, Temp, true);
            }
            Fill(Stream_Video, StreamPos_Last, Video_FrameRate, FrameRate_Temp, true);
        }
    }

    Fill(StreamKind, StreamPos_To, "Count", Count_Get(StreamKind, StreamPos_To), 10, true);
    return 1;
}

} //NameSpace

