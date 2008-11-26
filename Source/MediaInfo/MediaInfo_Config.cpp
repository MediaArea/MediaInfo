// MediaInfo_Config - Configuration class
// Copyright (C) 2005-2008 Jerome Martinez, Zen@MediaArea.net
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
// Global configuration of MediaInfo
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
#include "MediaInfo/MediaInfo_Config.h"
#include "ZenLib/ZtringListListF.h"
#include "ZenLib/File.h"
#include <algorithm>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const Char*  MediaInfo_Version=_T("MediaInfoLib - v0.7.7.9BETA");
const Char*  MediaInfo_Url=_T("http://mediainfo.sourceforge.net");
      Ztring EmptyZtring;       //Use it when we can't return a reference to a true Ztring
const Ztring EmptyZtring_Const; //Use it when we can't return a reference to a true Ztring, const version
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void File__Base_Container               (InfoMap &Info);
void File__Base_CodecID_General_Mpeg4   (InfoMap &Info);
void File__Base_CodecID_Video_Matroska  (InfoMap &Info);
void File__Base_CodecID_Video_Mpeg4     (InfoMap &Info);
void File__Base_CodecID_Video_Ogg       (InfoMap &Info);
void File__Base_CodecID_Video_Real      (InfoMap &Info);
void File__Base_CodecID_Video_Riff      (InfoMap &Info);
void File__Base_CodecID_Audio_Matroska  (InfoMap &Info);
void File__Base_CodecID_Audio_Mpeg4     (InfoMap &Info);
void File__Base_CodecID_Audio_Ogg       (InfoMap &Info);
void File__Base_CodecID_Audio_Real      (InfoMap &Info);
void File__Base_CodecID_Audio_Riff      (InfoMap &Info);
void File__Base_CodecID_Text_Matroska   (InfoMap &Info);
void File__Base_CodecID_Text_Mpeg4      (InfoMap &Info);
void File__Base_CodecID_Text_Riff       (InfoMap &Info);
void File__Base_Codec                   (InfoMap &Info);
void File__Base_DefaultLanguage         (Translation &Info);
void File__Base_Iso639                  (InfoMap &Info);
void File__Base_General                 (ZtringListList &Info);
void File__Base_Video                   (ZtringListList &Info);
void File__Base_Audio                   (ZtringListList &Info);
void File__Base_Text                    (ZtringListList &Info);
void File__Base_Chapters                (ZtringListList &Info);
void File__Base_Image                   (ZtringListList &Info);
void File__Base_Menu                    (ZtringListList &Info);
void File__Base_Summary                 (ZtringListList &Info);
void File__Base_Format                  (InfoMap &Info);
void File__Base_Library_DivX            (InfoMap &Info);
void File__Base_Library_XviD            (InfoMap &Info);
void File__Base_Library_MainConcept_Avc (InfoMap &Info);
void File__Base_Library_VorbisCom       (InfoMap &Info);
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

void MediaInfo_Config::Init()
{
    CS.Enter();
    //We use Init() instead of COnstructor because for some backends (like WxWidgets...) does NOT like constructor of static object with Unicode conversion

    //Test
    if (!LineSeparator.empty())
    {
        CS.Leave();
        return; //Already done
    }

    //Filling
    FormatDetection_MaximumOffset=1*1024*1024;
    MpegTs_MaximumOffset=16*1024*1024;
    Complete=0;
    BlockMethod=0;
    Internet=0;
    MultipleValues=0;
    ParseUnknownExtensions=1;
    ShowFiles_Nothing=1;
    ShowFiles_VideoAudio=1;
    ShowFiles_VideoOnly=1;
    ShowFiles_AudioOnly=1;
    ShowFiles_TextOnly=1;
    ParseSpeed=(float32)0.01;
    Details=0;
    Language_Raw=false;
    ReadByHuman=true;
    Demux=0;
    LineSeparator=_T("\r\n");
    ColumnSeparator=_T(";");
    TagSeparator=_T(" / ");
    Quote=_T("\"");
    DecimalPoint=_T(".");
    ThousandsPoint=_T("");

    CS.Leave();

    ZtringListList ZLL1; Language_Set(ZLL1);
}

//***************************************************************************
// Info
//***************************************************************************

Ztring MediaInfo_Config::Option (const String &Option, const String &Value_Raw)
{
    String Option_Lower(Option);
    size_t Egal_Pos=Option_Lower.find(_T('='));
    if (Egal_Pos==string::npos)
        Egal_Pos=Option_Lower.size();
    transform(Option_Lower.begin(), Option_Lower.begin()+Egal_Pos, Option_Lower.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix

    //Parsing pointer to a file
    Ztring Value;
    if (Value_Raw.find(_T("file://"))==0)
    {
        //Open
        Ztring FileName(Value_Raw, 7, Ztring::npos);
        File F(FileName.c_str());

        //Read
        int8u* Buffer=new int8u[(size_t)F.Size_Get()+1];
        Buffer[(size_t)F.Size_Get()]='\0';
        F.Read(Buffer, (size_t)F.Size_Get());
        F.Close();
        Ztring FromFile; FromFile.From_UTF8((char*)Buffer);
        if (FromFile.empty())
             FromFile.From_Local((char*)Buffer);
        delete[] Buffer; //Buffer=NULL;

        //Merge
        Value=FromFile;
    }
    else
        Value=Value_Raw;

         if (Option_Lower.empty())
    {
        return _T("");
    }
    else if (Option_Lower==_T("charset_config"))
    {
        return _T(""); //Only used in DLL, no Library action
    }
    else if (Option_Lower==_T("charset_output"))
    {
        return _T(""); //Only used in DLL, no Library action
    }
    else if (Option_Lower==_T("complete"))
    {
        if (Value.empty())
            Complete_Set(0);
        else
            Complete_Set(1);
        return _T("");
    }
    else if (Option_Lower==_T("complete_get"))
    {
        if (Complete_Get())
            return _T("1");
        else
            return _T("");
    }
    else if (Option_Lower==_T("blockmethod"))
    {
        if (Value.empty())
            BlockMethod_Set(0);
        else
            BlockMethod_Set(1);
        return _T("");
    }
    else if (Option_Lower==_T("blockmethod_get"))
    {
        if (BlockMethod_Get())
            return _T("1");
        else
            return _T("");
    }
    else if (Option_Lower==_T("internet"))
    {
        if (Value.empty())
            Internet_Set(0);
        else
            Internet_Set(1);
        return _T("");
    }
    else if (Option_Lower==_T("demux"))
    {
        if (Value.empty())
            Demux_Set(0);
        else if (Value==_T("all"))
            Demux_Set(2);
        else
            Demux_Set(1);
        return _T("");
    }
    else if (Option_Lower==_T("internet_get"))
    {
        if (Internet_Get())
            return _T("1");
        else
            return _T("");
    }
    else if (Option_Lower==_T("multiplevalues"))
    {
        if (Value.empty())
            MultipleValues_Set(0);
        else
            MultipleValues_Set(1);
        return _T("");
    }
    else if (Option_Lower==_T("multiplevalues_get"))
    {
        if (MultipleValues_Get())
            return _T("1");
        else
            return _T("");
    }
    else if (Option_Lower==_T("parseunknownextensions"))
    {
        if (Value.empty())
            ParseUnknownExtensions_Set(0);
        else
            ParseUnknownExtensions_Set(1);
        return _T("");
    }
    else if (Option_Lower==_T("parseunknownextensions_get"))
    {
        if (ParseUnknownExtensions_Get())
            return _T("1");
        else
            return _T("");
    }
    else if (Option_Lower==_T("showfiles_set"))
    {
        ShowFiles_Set(Value.c_str());
        return _T("");
    }
    else if (Option_Lower==_T("readbyhuman"))
    {
        ReadByHuman_Set(Value.To_int8u());
        return _T("");
    }
    else if (Option_Lower==_T("readbyhuman_get"))
    {
        return ReadByHuman_Get()?_T("1"):_T("0");
    }
    else if (Option_Lower==_T("lineseparator"))
    {
        LineSeparator_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("lineseparator_get"))
    {
        return LineSeparator_Get();
    }
    else if (Option_Lower==_T("version"))
    {
        Version_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("version_get"))
    {
        return Version_Get();
    }
    else if (Option_Lower==_T("columnseparator"))
    {
        ColumnSeparator_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("columnseparator_get"))
    {
        return ColumnSeparator_Get();
    }
    else if (Option_Lower==_T("tagseparator"))
    {
        TagSeparator_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("tagseparator_get"))
    {
        return TagSeparator_Get();
    }
    else if (Option_Lower==_T("quote"))
    {
        Quote_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("quote_get"))
    {
        return Quote_Get();
    }
    else if (Option_Lower==_T("decimalpoint"))
    {
        DecimalPoint_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("decimalpoint_get"))
    {
        return DecimalPoint_Get();
    }
    else if (Option_Lower==_T("thousandspoint"))
    {
        ThousandsPoint_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("thousandspoint_get"))
    {
        return ThousandsPoint_Get();
    }
    else if (Option_Lower==_T("streammax"))
    {
        ZtringListList StreamMax=Value.c_str();
        StreamMax_Set(StreamMax);
        return _T("");
    }
    else if (Option_Lower==_T("streammax_get"))
    {
        return StreamMax_Get();
    }
    else if (Option_Lower==_T("language"))
    {
        ZtringListList Language=Value.c_str();
        Language_Set(Language);
        return _T("");
    }
    else if (Option_Lower==_T("language_get"))
    {
        return Language_Get();
    }
    else if (Option_Lower==_T("inform"))
    {
        Inform_Set(Value.c_str());
        return _T("");
    }
    else if (Option_Lower==_T("inform_get"))
    {
        return Inform_Get();
    }
    else if (Option_Lower==_T("info_parameters"))
    {
        ZtringListList ToReturn=Info_Parameters_Get();

        //Adapt first column
        for (size_t Pos=0; Pos<ToReturn.size(); Pos++)
        {
            Ztring &C1=ToReturn(Pos, 0);
            if (!ToReturn(Pos, 1).empty())
            {
                C1.resize(25, ' ');
                ToReturn(Pos, 0)=C1 + _T(" :");
            }
        }

        ToReturn.Separator_Set(0, LineSeparator_Get());
        ToReturn.Separator_Set(1, _T(" "));
        ToReturn.Quote_Set(_T(""));
        return ToReturn.Read();
    }
    else if (Option_Lower==_T("info_parameters_csv"))
    {
        return Info_Parameters_Get();
    }
    else if (Option_Lower==_T("info_codecs"))
    {
        return Info_Codecs_Get();
    }
    else if (Option_Lower==_T("info_version"))
    {
        return Info_Version_Get();
    }
    else if (Option_Lower==_T("info_url"))
    {
        return Info_Url_Get();
    }
    else if (Option_Lower==_T("formatdetection_maximumoffset"))
    {
        FormatDetection_MaximumOffset_Set(Value==_T("-1")?(int64u)-1:((Ztring*)&Value)->To_int64u());
        return _T("");
    }
    else if (Option_Lower==_T("formatdetection_maximumoffset_get"))
    {
        return FormatDetection_MaximumOffset_Get()==(int64u)-1?Ztring(_T("-1")):Ztring::ToZtring(FormatDetection_MaximumOffset_Get());
    }
    else if (Option_Lower==_T("mpegts_maximumoffset"))
    {
        MpegTs_MaximumOffset_Set(Value==_T("-1")?(int64u)-1:((Ztring*)&Value)->To_int64u());
        return _T("");
    }
    else if (Option_Lower==_T("mpegts_maximumoffset_get"))
    {
        return MpegTs_MaximumOffset_Get()==(int64u)-1?Ztring(_T("-1")):Ztring::ToZtring(MpegTs_MaximumOffset_Get());
    }
    else
        return _T("Option not known");
}

//***************************************************************************
// Info
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config::Complete_Set (size_t NewValue)
{
    CriticalSectionLocker CSL(CS);;
    Complete=NewValue;
}

size_t MediaInfo_Config::Complete_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return Complete;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::BlockMethod_Set (size_t NewValue)
{
    CriticalSectionLocker CSL(CS);;
    BlockMethod=NewValue;
}

size_t MediaInfo_Config::BlockMethod_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return BlockMethod;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Internet_Set (size_t NewValue)
{
    CriticalSectionLocker CSL(CS);;
    Internet=NewValue;
}

size_t MediaInfo_Config::Internet_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return Internet;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::MultipleValues_Set (size_t NewValue)
{
    CriticalSectionLocker CSL(CS);;
    MultipleValues=NewValue;
}

size_t MediaInfo_Config::MultipleValues_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return MultipleValues;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::ParseUnknownExtensions_Set (size_t NewValue)
{
    CriticalSectionLocker CSL(CS);;
    ParseUnknownExtensions=NewValue;
}

size_t MediaInfo_Config::ParseUnknownExtensions_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ParseUnknownExtensions;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::ShowFiles_Set (const ZtringListList &NewShowFiles)
{
    CriticalSectionLocker CSL(CS);;
    for  (size_t Pos=0; Pos<NewShowFiles.size(); Pos++)
    {
        const Ztring& Object=NewShowFiles.Read(Pos, 0);
             if (Object==_T("Nothing"))
            ShowFiles_Nothing=NewShowFiles.Read(Pos, 1).empty()?1:0;
        else if (Object==_T("VideoAudio"))
            ShowFiles_VideoAudio=NewShowFiles.Read(Pos, 1).empty()?1:0;
        else if (Object==_T("VideoOnly"))
            ShowFiles_VideoOnly=NewShowFiles.Read(Pos, 1).empty()?1:0;
        else if (Object==_T("AudioOnly"))
            ShowFiles_AudioOnly=NewShowFiles.Read(Pos, 1).empty()?1:0;
        else if (Object==_T("TextOnly"))
            ShowFiles_TextOnly=NewShowFiles.Read(Pos, 1).empty()?1:0;
    }
}

size_t MediaInfo_Config::ShowFiles_Nothing_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ShowFiles_Nothing;
}

size_t MediaInfo_Config::ShowFiles_VideoAudio_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ShowFiles_VideoAudio;
}

size_t MediaInfo_Config::ShowFiles_VideoOnly_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ShowFiles_VideoOnly;
}

size_t MediaInfo_Config::ShowFiles_AudioOnly_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ShowFiles_AudioOnly;
}

size_t MediaInfo_Config::ShowFiles_TextOnly_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ShowFiles_TextOnly;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::ParseSpeed_Set (float32 NewValue)
{
    CriticalSectionLocker CSL(CS);;
    ParseSpeed=NewValue;
}

float32 MediaInfo_Config::ParseSpeed_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ParseSpeed;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::ReadByHuman_Set (bool NewValue)
{
    CriticalSectionLocker CSL(CS);;
    ReadByHuman=NewValue;
}

bool MediaInfo_Config::ReadByHuman_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ReadByHuman;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Details_Set (float NewValue)
{
    CriticalSectionLocker CSL(CS);;
    Details=NewValue;
}

float MediaInfo_Config::Details_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return Details;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Demux_Set (int8u NewValue)
{
    CriticalSectionLocker CSL(CS);;
    Demux=NewValue;
}

int8u MediaInfo_Config::Demux_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return Demux;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::LineSeparator_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);;
    LineSeparator=NewValue;
}

Ztring MediaInfo_Config::LineSeparator_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return LineSeparator;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Version_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);
    Version=ZtringListList(NewValue).Read(0); //Only the 1st value
}

Ztring MediaInfo_Config::Version_Get ()
{
    CriticalSectionLocker CSL(CS);
    return Version;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::ColumnSeparator_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);;
    ColumnSeparator=NewValue;
}

Ztring MediaInfo_Config::ColumnSeparator_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ColumnSeparator;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::TagSeparator_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);;
    TagSeparator=NewValue;
}

Ztring MediaInfo_Config::TagSeparator_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return TagSeparator;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Quote_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);;
    Quote=NewValue;
}

Ztring MediaInfo_Config::Quote_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return Quote;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::DecimalPoint_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);;
    DecimalPoint=NewValue;
}

Ztring MediaInfo_Config::DecimalPoint_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return DecimalPoint;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::ThousandsPoint_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);;
    ThousandsPoint=NewValue;
}

Ztring MediaInfo_Config::ThousandsPoint_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return ThousandsPoint;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::StreamMax_Set (const ZtringListList &)
{
    CriticalSectionLocker CSL(CS);;
    //TODO : implementation
}

Ztring MediaInfo_Config::StreamMax_Get ()
{
    CriticalSectionLocker CSL(CS);;
    ZtringListList StreamMax;
    //TODO : implementation
    return StreamMax.Read();
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Language_Set (const ZtringListList &NewValue)
{
    CriticalSectionLocker CSL(CS);;

    //Which language to choose?
    //-Raw
         if (NewValue.size()==1 && NewValue[0].size()==1 && NewValue[0][0]==_T("raw"))
    {
        Language_Raw=true;
        Language.clear();
        //Exceptions
        Language.Write(_T("  Config_Text_ColumnSize"), _T("32"));
        Language.Write(_T("  Config_Text_Separator"), _T(" : "));
        Language.Write(_T("  Config_Text_NumberTag"), _T(" #"));
        Language.Write(_T("  Config_Text_FloatSeparator"), _T("."));
        Language.Write(_T("  Config_Text_ThousandsSeparator"), _T(""));
    }
    //-Add custom language to English language
    else
    {
        //Fill base words (with English translation)
        File__Base_DefaultLanguage(Language);
        //Add custom language to English language
        for (size_t Pos=0; Pos<NewValue.size(); Pos++)
            if (NewValue[Pos].size()>=2)
                Language.Write(NewValue[Pos][0], NewValue[Pos][1]);
    }

    //Fill Info
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
        if (!Info[StreamKind].empty())
            Language_Set((stream_t)StreamKind);
}

void MediaInfo_Config::Language_Set (stream_t StreamKind)
{
    //CriticalSectionLocker CSL(CS); //No, only used internaly

    //Fill Info
    for (size_t Pos=0; Pos<Info[StreamKind].size(); Pos++)
    {
        //Strings - Info_Name_Text
        Ztring ToReplace=Info[StreamKind](Pos, Info_Name);
        if (!Language_Raw && ToReplace.find(_T("/String"))!=Error)
        {
            ToReplace.FindAndReplace(_T("/String1"), _T(""));
            ToReplace.FindAndReplace(_T("/String2"), _T(""));
            ToReplace.FindAndReplace(_T("/String3"), _T(""));
            ToReplace.FindAndReplace(_T("/String4"), _T(""));
            ToReplace.FindAndReplace(_T("/String5"), _T(""));
            ToReplace.FindAndReplace(_T("/String6"), _T(""));
            ToReplace.FindAndReplace(_T("/String7"), _T(""));
            ToReplace.FindAndReplace(_T("/String8"), _T(""));
            ToReplace.FindAndReplace(_T("/String9"), _T(""));
            ToReplace.FindAndReplace(_T("/String"),  _T(""));
        }
        if (!Language_Raw && ToReplace.find(_T("/"))!=Error) //Complex values, like XXX/YYY --> We translate both XXX and YYY
        {
            Ztring ToReplace1=ToReplace.SubString(_T(""), _T("/"));
            Ztring ToReplace2=ToReplace.SubString(_T("/"), _T(""));
            Info[StreamKind](Pos, Info_Name_Text)=Language.Get(ToReplace1);
            Info[StreamKind](Pos, Info_Name_Text)+=_T("/");
            Info[StreamKind](Pos, Info_Name_Text)+=Language.Get(ToReplace2);
        }
        else
            Info[StreamKind](Pos, Info_Name_Text)=Language.Get(ToReplace);
        //Strings - Info_Measure_Text
        Info[StreamKind](Pos, Info_Measure_Text).clear(); //I don(t know why, but if I don't do this Delphi/C# debugger make crashing the calling program
        Info[StreamKind](Pos, Info_Measure_Text)=Language.Get(Info[StreamKind](Pos, Info_Measure));
        //Slashes

    }
}

Ztring MediaInfo_Config::Language_Get ()
{
    CriticalSectionLocker CSL(CS);;
    Ztring ToReturn;//TODO =Language.Read();
    return ToReturn;
}

Ztring MediaInfo_Config::Language_Get (const Ztring &Value)
{
    CriticalSectionLocker CSL(CS);;
    return Language.Get(Value);
}

//---------------------------------------------------------------------------
Ztring MediaInfo_Config::Language_Get (const Ztring &Count, const Ztring &Value, bool ValueIsAlwaysSame)
{
    //Integrity
    if (Count.empty())
        return EmptyString_Get();

    //Different Plurals are available or not?
    if (Language_Get(Value+_T("1")).empty())
    {
        //if (Count==_T("0") || Count==_T("1"))
            return Count+Language_Get(Value);
        //else
            //return Count+Language_Get(Value+_T("s"));
    }

    //Detecting plural form for multiple plurals
    int8u  Form=(int8u)-1;

    if (!ValueIsAlwaysSame)
    {
        //Polish has 2 plurial, Algorithm of Polish
        size_t CountI=Count.To_int32u();
        size_t Pos3=CountI/100;
        int8u  Pos2=(int8u)((CountI-Pos3)/10);
        int8u  Pos1=(int8u)(CountI-Pos3*100-Pos2*10);
        if (Pos3==0)
        {
            if (Pos2==0)
            {
                     if (Pos1==0 && Count.size()==1) //Only "0", not "0.xxx"
                    Form=0; //000 to 000 kanal?
                else if (Pos1<=1)
                    Form=1; //001 to 001 kanal
                else if (Pos1<=4)
                    Form=2; //002 to 004 kanaly
                else //if (Pos1>=5)
                    Form=3; //005 to 009 kanalow
            }
            else if (Pos2==1)
                    Form=3; //010 to 019 kanalow
            else //if (Pos2>=2)
            {
                     if (Pos1<=1)
                    Form=3; //020 to 021, 090 to 091 kanalow
                else if (Pos1<=4)
                    Form=2; //022 to 024, 092 to 094 kanali
                else //if (Pos1>=5)
                    Form=3; //025 to 029, 095 to 099 kanalow
            }
        }
        else //if (Pos3>=1)
        {
            if (Pos2==0)
            {
                     if (Pos1<=1)
                    Form=3; //100 to 101 kanalow
                else if (Pos1<=4)
                    Form=2; //102 to 104 kanaly
                else //if (Pos1>=5)
                    Form=3; //105 to 109 kanalow
            }
            else if (Pos2==1)
                    Form=3; //110 to 119 kanalow
            else //if (Pos2>=2)
            {
                     if (Pos1<=1)
                    Form=3; //120 to 121, 990 to 991 kanalow
                else if (Pos1<=4)
                    Form=2; //122 to 124, 992 to 994 kanali
                else //if (Pos1>=5)
                    Form=3; //125 to 129, 995 to 999 kanalow
            }
        }
    }

    //Replace dot and thousand separator
    Ztring ToReturn=Count;
    size_t DotPos=ToReturn.find(_T('.'));
    if (DotPos!=string::npos)
        ToReturn.FindAndReplace(_T("."), Language_Get(_T("  Config_Text_FloatSeparator")), DotPos);
    else
        DotPos=ToReturn.size();
    if (DotPos>3)
        ToReturn.insert(DotPos-3, Language_Get(_T("  Config_Text_ThousandsSeparator")));

    //Selecting the form
         if (Form==0)
        ToReturn =Language_Get(Value+_T("0")); //Only the translation
    else if (Form==1)
        ToReturn+=Language_Get(Value+_T("1"));
    else if (Form==2)
        ToReturn+=Language_Get(Value+_T("2"));
    else if (Form==3)
        ToReturn+=Language_Get(Value+_T("3"));
    else
        ToReturn+=Language_Get(Value);
    return ToReturn;
}

//---------------------------------------------------------------------------
void MediaInfo_Config::Inform_Set (const ZtringListList &NewValue)
{
    if (NewValue.Read(0, 0)==_T("Details"))
        Details_Set(NewValue.Read(0, 1).To_float32());
    else
    {
        Details_Set(0);

        CriticalSectionLocker CSL(CS);;

        //Inform
        if (NewValue==_T("Summary"))
            File__Base_Summary(Custom_View);
        else
            Custom_View=NewValue;
    }

    CriticalSectionLocker CSL(CS);;

    //Parsing pointers to files in streams
    for (size_t Pos=0; Pos<Custom_View.size(); Pos++)
    {
        if (Custom_View(Pos, 1).find(_T("file://"))==0)
        {
            //Open
            Ztring FileName(Custom_View(Pos, 1), 7, Ztring::npos);
            File F(FileName.c_str());

            //Read
            int8u* Buffer=new int8u[(size_t)F.Size_Get()+1];
            Buffer[(size_t)F.Size_Get()+1]='\0';
            F.Read(Buffer, (size_t)F.Size_Get());
            F.Close();
            Ztring FromFile; FromFile.From_Local((char*)Buffer);
            delete[] Buffer; //Buffer=NULL;

            //Merge
            FromFile.FindAndReplace(_T("\r\n"), _T("\\r\\n"), 0, Ztring_Recursive);
            FromFile.FindAndReplace(_T("\n"), _T("\\r\\n"), 0, Ztring_Recursive);
            Custom_View(Pos, 1)=FromFile;
        }
    }
}

Ztring MediaInfo_Config::Inform_Get ()
{
    CriticalSectionLocker CSL(CS);;
    return Custom_View.Read();
}

Ztring MediaInfo_Config::Inform_Get (const Ztring &Value)
{
    CriticalSectionLocker CSL(CS);;
    size_t Pos=Custom_View.Find(Value);
    if (Pos==Error || 1>=Custom_View[Pos].size())
        return EmptyString_Get();
    return Custom_View[Pos][1];
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Container_Get (const Ztring &Value, infoformat_t KindOfFormatInfo)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Container.empty())
        File__Base_Container(Container);
    CS.Leave();

    return Container.Get(Value, KindOfFormatInfo);
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Format_Get (const Ztring &Value, infoformat_t KindOfFormatInfo)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Format.empty())
        File__Base_Format(Format);
    CS.Leave();

    return Format.Get(Value, KindOfFormatInfo);
}

//---------------------------------------------------------------------------
InfoMap &MediaInfo_Config::Format_Get ()
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Format.empty())
        File__Base_Format(Format);
    CS.Leave();

    return Format;
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Codec_Get (const Ztring &Value, infocodec_t KindOfCodecInfo)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Codec.empty())
        File__Base_Codec(Codec);
    CS.Leave();

    return Codec.Get(Value, KindOfCodecInfo);
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Codec_Get (const Ztring &Value, infocodec_t KindOfCodecInfo, stream_t KindOfStream)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Codec.empty())
        File__Base_Codec(Codec);
    CS.Leave();

    //Transform to text
    Ztring KindOfStreamS;
    switch (KindOfStream)
    {
        case Stream_General  : KindOfStreamS=_T("G"); break;
        case Stream_Video    : KindOfStreamS=_T("V"); break;
        case Stream_Audio    : KindOfStreamS=_T("A"); break;
        case Stream_Text     : KindOfStreamS=_T("T"); break;
        case Stream_Image    : KindOfStreamS=_T("I"); break;
        case Stream_Chapters : KindOfStreamS=_T("C"); break;
        case Stream_Menu     : KindOfStreamS=_T("M"); break;
        case Stream_Max      : KindOfStreamS=_T(" "); break;
    }

    return Codec.Get(Value, KindOfCodecInfo, KindOfStreamS, InfoCodec_KindOfStream);
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::CodecID_Get (stream_t KindOfStream, infocodecid_format_t Format, const Ztring &Value, infocodecid_t KindOfCodecIDInfo)
{
    CS.Enter();
    if (CodecID[Format][KindOfStream].empty())
    {
        switch (KindOfStream)
        {
            case Stream_General :
                                    switch (Format)
                                    {
                                        case InfoCodecID_Format_Mpeg4 : File__Base_CodecID_General_Mpeg4(CodecID[Format][KindOfStream]); break;
                                        default: ;
                                    }
                                    break;
            case Stream_Video   :
                                    switch (Format)
                                    {
                                        case InfoCodecID_Format_Matroska : File__Base_CodecID_Video_Matroska(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Mpeg4    : File__Base_CodecID_Video_Mpeg4(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Real     : File__Base_CodecID_Video_Real(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Riff     : File__Base_CodecID_Video_Riff(CodecID[Format][KindOfStream]); break;
                                        default: ;
                                    }
                                    break;
            case Stream_Audio   :
                                    switch (Format)
                                    {
                                        case InfoCodecID_Format_Matroska : File__Base_CodecID_Audio_Matroska(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Mpeg4    : File__Base_CodecID_Audio_Mpeg4(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Real     : File__Base_CodecID_Audio_Real(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Riff     : File__Base_CodecID_Audio_Riff(CodecID[Format][KindOfStream]); break;
                                        default: ;
                                    }
                                    break;
            case Stream_Text    :
                                    switch (Format)
                                    {
                                        case InfoCodecID_Format_Matroska : File__Base_CodecID_Text_Matroska(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Mpeg4    : File__Base_CodecID_Text_Mpeg4(CodecID[Format][KindOfStream]); break;
                                        case InfoCodecID_Format_Riff     : File__Base_CodecID_Text_Riff(CodecID[Format][KindOfStream]); break;
                                        default: ;
                                    }
                                    break;
            default: ;
        }
    }
    CS.Leave();
    return CodecID[Format][KindOfStream].Get(Value, KindOfCodecIDInfo);
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Library_Get (infolibrary_format_t Format, const Ztring &Value, infolibrary_t KindOfLibraryInfo)
{
    CS.Enter();
    if (Library[Format].empty())
    {
        switch (Format)
        {
            case InfoLibrary_Format_DivX : File__Base_Library_DivX(Library[Format]); break;
            case InfoLibrary_Format_XviD : File__Base_Library_XviD(Library[Format]); break;
            case InfoLibrary_Format_MainConcept_Avc : File__Base_Library_MainConcept_Avc(Library[Format]); break;
            case InfoLibrary_Format_VorbisCom : File__Base_Library_VorbisCom(Library[Format]); break;
            default: ;
        }
    }
    CS.Leave();
    return Library[Format].Get(Value, KindOfLibraryInfo);
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Iso639_Get (const Ztring &Value)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Iso639.empty())
        File__Base_Iso639(Iso639);
    CS.Leave();

    return Iso639.Get(Value, 1);
}

//---------------------------------------------------------------------------
const Ztring MediaInfo_Config::Iso639_Find (const Ztring &Value)
{
    Translation Info;
    File__Base_DefaultLanguage (Info);
    Ztring Value_Lower(Value);
    transform(Value_Lower.begin(), Value_Lower.end(), Value_Lower.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix

    for (Translation::iterator Trans=Info.begin(); Trans!=Info.end(); Trans++)
    {
        transform(Trans->second.begin(), Trans->second.end(), Trans->second.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix
        if (Trans->second==Value_Lower && Trans->first.find(_T("Language_"))==0)
            return Trans->first.substr(9, string::npos);
    }
    return Ztring();
}

//---------------------------------------------------------------------------
const Ztring &MediaInfo_Config::Info_Get (stream_t KindOfStream, const Ztring &Value, info_t KindOfInfo)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Info[KindOfStream].empty())
        switch (KindOfStream)
        {
            case Stream_General :   File__Base_General(Info[Stream_General]);   Language_Set(Stream_General); break;
            case Stream_Video :     File__Base_Video(Info[Stream_Video]);       Language_Set(Stream_Video); break;
            case Stream_Audio :     File__Base_Audio(Info[Stream_Audio]);       Language_Set(Stream_Audio); break;
            case Stream_Text :      File__Base_Text(Info[Stream_Text]);         Language_Set(Stream_Text); break;
            case Stream_Chapters :  File__Base_Chapters(Info[Stream_Chapters]); Language_Set(Stream_Chapters); break;
            case Stream_Image :     File__Base_Image(Info[Stream_Image]);       Language_Set(Stream_Image); break;
            case Stream_Menu :      File__Base_Menu(Info[Stream_Menu]);         Language_Set(Stream_Menu); break;
            default:;
        }
    CS.Leave();

    if (KindOfStream>=Stream_Max)
        return EmptyString_Get();
    size_t Pos=Info[KindOfStream].Find(Value);
    if (Pos==Error || (size_t)KindOfInfo>=Info[KindOfStream][Pos].size())
        return EmptyString_Get();
    return Info[KindOfStream][Pos][KindOfInfo];
}

const Ztring &MediaInfo_Config::Info_Get (stream_t KindOfStream, size_t Pos, info_t KindOfInfo)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Info[KindOfStream].empty())
        switch (KindOfStream)
        {
            case Stream_General :   File__Base_General(Info[Stream_General]);   Language_Set(Stream_General); break;
            case Stream_Video :     File__Base_Video(Info[Stream_Video]);       Language_Set(Stream_Video); break;
            case Stream_Audio :     File__Base_Audio(Info[Stream_Audio]);       Language_Set(Stream_Audio); break;
            case Stream_Text :      File__Base_Text(Info[Stream_Text]);         Language_Set(Stream_Text); break;
            case Stream_Chapters :  File__Base_Chapters(Info[Stream_Chapters]); Language_Set(Stream_Chapters); break;
            case Stream_Image :     File__Base_Image(Info[Stream_Image]);       Language_Set(Stream_Image); break;
            case Stream_Menu :      File__Base_Menu(Info[Stream_Menu]);         Language_Set(Stream_Menu); break;
            default:;
        }
    CS.Leave();

    if (KindOfStream>=Stream_Max)
        return EmptyString_Get();
    if (Pos>=Info[KindOfStream].size() || (size_t)KindOfInfo>=Info[KindOfStream][Pos].size())
        return EmptyString_Get();
    return Info[KindOfStream][Pos][KindOfInfo];
}

const ZtringListList &MediaInfo_Config::Info_Get(stream_t KindOfStream)
{
    //Loading codec table if not yet done
    CS.Enter();
    if (Info[KindOfStream].empty())
        switch (KindOfStream)
        {
            case Stream_General :   File__Base_General(Info[Stream_General]);   Language_Set(Stream_General); break;
            case Stream_Video :     File__Base_Video(Info[Stream_Video]);       Language_Set(Stream_Video); break;
            case Stream_Audio :     File__Base_Audio(Info[Stream_Audio]);       Language_Set(Stream_Audio); break;
            case Stream_Text :      File__Base_Text(Info[Stream_Text]);         Language_Set(Stream_Text); break;
            case Stream_Chapters :  File__Base_Chapters(Info[Stream_Chapters]); Language_Set(Stream_Chapters); break;
            case Stream_Image :     File__Base_Image(Info[Stream_Image]);       Language_Set(Stream_Image); break;
            case Stream_Menu :      File__Base_Menu(Info[Stream_Menu]);         Language_Set(Stream_Menu); break;
            default:;
        }
    CS.Leave();

    return Info[KindOfStream];
}

//---------------------------------------------------------------------------
Ztring MediaInfo_Config::Info_Parameters_Get ()
{
    CriticalSectionLocker CSL(CS);;

    //Loading all
    File__Base_General(Info[Stream_General]);
    File__Base_Video(Info[Stream_Video]);
    File__Base_Audio(Info[Stream_Audio]);
    File__Base_Text(Info[Stream_Text]);
    File__Base_Chapters(Info[Stream_Chapters]);
    File__Base_Image(Info[Stream_Image]);
    File__Base_Menu(Info[Stream_Menu]);

    //Building
    ZtringListList ToReturn;
    size_t ToReturn_Pos=0;

    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        ToReturn(ToReturn_Pos, 0)=Info[StreamKind].Read(_T("StreamKind"), Info_Text);
        ToReturn_Pos++;
        for (size_t Pos=0; Pos<Info[StreamKind].size(); Pos++)
            if (!Info[StreamKind].Read(Pos, Info_Name).empty())
            {
                ToReturn(ToReturn_Pos, 0)=Info[StreamKind].Read(Pos, Info_Name);
                ToReturn(ToReturn_Pos, 1)=Info[StreamKind].Read(Pos, Info_Info);
                ToReturn_Pos++;
            }
        ToReturn_Pos++;
    }
    return ToReturn.Read();
}

//---------------------------------------------------------------------------
Ztring MediaInfo_Config::Info_Tags_Get () const
{
    return _T("");
}

Ztring MediaInfo_Config::Info_Codecs_Get ()
{
    CriticalSectionLocker CSL(CS);;

    //Loading
    File__Base_Codec(Codec);

    //Building
    Ztring ToReturn;
    InfoMap::iterator Temp=Codec.begin();
    while (Temp!=Codec.end())
    {
        ToReturn+=Temp->second.Read();
        ToReturn+=EOL;
        Temp++;
    }    

    return ToReturn;
}

Ztring MediaInfo_Config::Info_Version_Get () const
{
    return MediaInfo_Version;
}

Ztring MediaInfo_Config::Info_Url_Get () const
{
    return MediaInfo_Url;
}

const Ztring &MediaInfo_Config::EmptyString_Get () const
{
    return EmptyZtring_Const;
}

void MediaInfo_Config::FormatDetection_MaximumOffset_Set (int64u Value)
{
    CriticalSectionLocker CSL(CS);
    FormatDetection_MaximumOffset=Value;
}

int64u MediaInfo_Config::FormatDetection_MaximumOffset_Get ()
{
    CriticalSectionLocker CSL(CS);
    return FormatDetection_MaximumOffset;
}

void MediaInfo_Config::MpegTs_MaximumOffset_Set (int64u Value)
{
    CriticalSectionLocker CSL(CS);
    MpegTs_MaximumOffset=Value;
}

int64u MediaInfo_Config::MpegTs_MaximumOffset_Get ()
{
    CriticalSectionLocker CSL(CS);
    return MpegTs_MaximumOffset;
}

} //NameSpace

