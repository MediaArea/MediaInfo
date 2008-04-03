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
#ifndef MediaInfo_ConfigH
#define MediaInfo_ConfigH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <MediaInfo/MediaInfo_Const.h>
#include <ZenLib/ZtringListList.h>
#include <ZenLib/Translation.h>
#include <ZenLib/InfoMap.h>
#include <map>
#include <vector>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class MediaInfo_Config
//***************************************************************************

class MediaInfo_Config
{
public :
    //Constructor/Destructor
    void Init(); //Must be called instead of constructor

    //General
    Ztring Option (const Ztring &Option, const Ztring &Value=_T(""));

    //Info
          void      Complete_Set (size_t NewValue);
          size_t    Complete_Get ();

          void      BlockMethod_Set (size_t NewValue);
          size_t    BlockMethod_Get ();

          void      Internet_Set (size_t NewValue);
          size_t    Internet_Get ();

          void      MultipleValues_Set (size_t NewValue);
          size_t    MultipleValues_Get ();

          void      ParseUnknownExtensions_Set (size_t NewValue);
          size_t    ParseUnknownExtensions_Get ();

          void      ShowFiles_Set (const ZtringListList &NewShowFiles);
          size_t    ShowFiles_Nothing_Get ();
          size_t    ShowFiles_VideoAudio_Get ();
          size_t    ShowFiles_VideoOnly_Get ();
          size_t    ShowFiles_AudioOnly_Get ();
          size_t    ShowFiles_TextOnly_Get ();

          void      ParseSpeed_Set (float32 NewValue);
          float32   ParseSpeed_Get ();

          void      Details_Set (float NewValue);
          float     Details_Get ();

          void      Demux_Set (int8u NewValue);
          int8u     Demux_Get ();

          void      LineSeparator_Set (const Ztring &NewValue);
    const Ztring   &LineSeparator_Get ();

          void      ColumnSeparator_Set (const Ztring &NewValue);
    const Ztring   &ColumnSeparator_Get ();

          void      TagSeparator_Set (const Ztring &NewValue);
    const Ztring   &TagSeparator_Get ();

          void      Quote_Set (const Ztring &NewValue);
    const Ztring   &Quote_Get ();

          void      DecimalPoint_Set (const Ztring &NewValue);
    const Ztring   &DecimalPoint_Get ();

          void      ThousandsPoint_Set (const Ztring &NewValue);
    const Ztring   &ThousandsPoint_Get ();

          void      StreamMax_Set (const ZtringListList &NewValue);
          Ztring    StreamMax_Get ();

          void      Language_Set (const ZtringListList &NewLanguage);
          Ztring    Language_Get ();
    const Ztring   &Language_Get (const Ztring &Value);
          Ztring    Language_Get (const Ztring &Count, const Ztring &Value);

          void      Inform_Set (const ZtringListList &NewInform);
          Ztring    Inform_Get ();
    const Ztring   &Inform_Get (const Ztring &Value);

    const Ztring   &Format_Get (const Ztring &Value, infoformat_t KindOfFormatInfo=InfoFormat_Name);
          InfoMap  &Format_Get() {return Format;}; //Should not be, but too difficult to hide it

    const Ztring   &Codec_Get (const Ztring &Value, infocodec_t KindOfCodecInfo=InfoCodec_Name);
    const Ztring   &Codec_Get (const Ztring &Value, infocodec_t KindOfCodecInfo, stream_t KindOfStream);

    const Ztring   &Encoder_Get (const Ztring &Value, infoencoder_t KindOfEncoderInfo=InfoEncoder_LongName) const;

    const Ztring   &Iso639_Get (const Ztring &Value);

    const Ztring   &Info_Get (stream_t KindOfStream, const Ztring &Value, info_t KindOfInfo=Info_Text) const;
    const Ztring   &Info_Get (stream_t KindOfStream, size_t Pos, info_t KindOfInfo=Info_Text) const;
    const ZtringListList &Info_Get(stream_t KindOfStream) const {return Info[KindOfStream];}; //Should not be, but too difficult to hide it

          Ztring    Info_Parameters_Get () const;
          Ztring    Info_Tags_Get       () const;
          Ztring    Info_Codecs_Get     () const;
          Ztring    Info_Version_Get    () const;
          Ztring    Info_Url_Get        () const;

          Ztring   &EmptyString_Get(); //Use it when we can't return a reference to a true string
    const Ztring   &EmptyString_Get() const; //Use it when we can't return a reference to a true string

          void      File_Filter_Set     (int64u NewValue);
          bool      File_Filter_Get     (const int16u  Value);
          bool      File_Filter_Get     ();
          bool      File_Filter_HasChanged();

          void      File_Duplicate_Set  (const Ztring &Value);
          Ztring    File_Duplicate_Get  (size_t AlreadyRead_Pos); //Requester must say how many Get() it already read
          bool      File_Duplicate_Get_AlwaysNeeded (size_t AlreadyRead_Pos); //Requester must say how many Get() it already read

          void      File_IsSeekable_Set (bool NewValue);
          bool      File_IsSeekable_Get ();

private :
    void Enter (bool Set=false);
    void Leave ();

    size_t          Complete;
    size_t          BlockMethod;
    size_t          Internet;
    size_t          MultipleValues;
    size_t          ParseUnknownExtensions;
    size_t          ShowFiles_Nothing;
    size_t          ShowFiles_VideoAudio;
    size_t          ShowFiles_VideoOnly;
    size_t          ShowFiles_AudioOnly;
    size_t          ShowFiles_TextOnly;
    float32         ParseSpeed;
    float           Details;
    int8u           Demux;
    bool            FileIsSeekable;
    Ztring          ColumnSeparator;
    Ztring          LineSeparator;
    Ztring          TagSeparator;
    Ztring          Quote;
    Ztring          DecimalPoint;
    Ztring          ThousandsPoint;
    size_t          StreamsMax[Stream_Max];
    Translation     Language; //ex. : "KB;Ko"
    ZtringListList  Custom_View; //Definition of "General", "Video", "Audio", "Text", "Chapters", "Image"

    InfoMap         Format;
    InfoMap         Codec;
    ZtringListList  Encoder;
    InfoMap         Iso639;
    ZtringListList  Info[Stream_Max]; //General info

    //Extra
    std::map<int16u, bool> File_Filter_16;
    bool                   File_Filter_HasChanged_;

    std::vector<Ztring>    File__Duplicate_List;
};

extern MediaInfo_Config Config;

} //NameSpace

#endif
