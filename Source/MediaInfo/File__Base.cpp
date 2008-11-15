// File__Base - Base for other files
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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#include "ZenLib/File.h"
#include <cstring>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Gestion de la classe
//***************************************************************************

//---------------------------------------------------------------------------
//Constructeurs
File__Base::File__Base ()
{
    //Init pointers
    Details=NULL;
    Stream=NULL;
    Stream_More=NULL;
    Stream_MustBeDeleted=false;

    //File
    File_Size=(int64u)-1;
    File_Offset=0;
    File_Offset_FirstSynched=(int64u)-1;
    File_GoTo=(int64u)-1;
    File_MaximumOffset=MediaInfoLib::Config.FormatDetection_MaximumOffset_Get();

    //Optimization init
    StreamKind_Last=Stream_Max;
    StreamPos_Last=Error;

    //Config
    Config=NULL;
}

//---------------------------------------------------------------------------
//Constructeurs
File__Base::~File__Base ()
{
    if (Stream_MustBeDeleted)
    {
        delete Stream; //Stream=NULL;
        delete Stream_More; //Stream_More=NULL;
    }
}

//---------------------------------------------------------------------------
//Base
void File__Base::Init (MediaInfo_Config_MediaInfo * Config_, Ztring* Details_, std::vector<std::vector<ZtringList> > * Stream_, std::vector<std::vector<ZtringListList> > * Stream_More_)
{
    if (Config)
        return; //Already done
        
    if (Stream_)
    {
        Stream=Stream_;
        Stream_More=Stream_More_;
        Stream_MustBeDeleted=false;
    }
    else
    {
        Stream=new std::vector<std::vector<ZtringList> >;
        Stream->resize(Stream_Max);
        Stream_More=new std::vector<std::vector<ZtringListList> >;
        Stream_More->resize(Stream_Max);
        Stream_MustBeDeleted=true;
    }

    Config=Config_;
    Details=Details_;
}

//***************************************************************************
// Fonctions
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Base::Count_Get (stream_t StreamKind, size_t Pos) const
{
    //Integrity
    if (StreamKind>=Stream_Max)
        return 0;

    //Count of streams
    if (!Stream)
        return 0;
    if (Pos==Error)
        return (*Stream)[StreamKind].size();

    //Integrity
    if (Pos>=(*Stream)[StreamKind].size())
        return 0;

    //Count of piece of information in a stream
    return MediaInfoLib::Config.Info_Get(StreamKind).size()+(*Stream_More)[StreamKind][Pos].size();
}

//---------------------------------------------------------------------------
const Ztring &File__Base::Get (stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t KindOfInfo)
{
    //Check integrity
    if (StreamKind>=Stream_Max || StreamNumber>=(*Stream)[StreamKind].size() || Parameter>=MediaInfoLib::Config.Info_Get(StreamKind).size()+(*Stream_More)[StreamKind][StreamNumber].size() || KindOfInfo>=Info_Max)
        return MediaInfoLib::Config.EmptyString_Get(); //Parameter is unknown

    else if (Parameter<MediaInfoLib::Config.Info_Get(StreamKind).size())
    {
        //Optimization : KindOfInfo>Info_Text is in static lists
        if (KindOfInfo!=Info_Text)
            return MediaInfoLib::Config.Info_Get(StreamKind)[Parameter][KindOfInfo]; //look for static information only
        else if (Parameter<(*Stream)[StreamKind][StreamNumber].size())
            return (*Stream)[StreamKind][StreamNumber][Parameter];
        else
            return MediaInfoLib::Config.EmptyString_Get(); //This parameter is known, but not filled
    }
    else
        return (*Stream_More)[StreamKind][StreamNumber][Parameter-MediaInfoLib::Config.Info_Get(StreamKind).size()][KindOfInfo];
}

//---------------------------------------------------------------------------
const Ztring &File__Base::Get (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, info_t KindOfInfo, info_t KindOfSearch)
{
    size_t ParameterI=0;

    //Legacy
    if (Parameter.find(_T("_String"))!=Error)
    {
        Ztring S1=Parameter;
        S1.FindAndReplace(_T("_String"), _T("/String"));
        return Get(StreamKind, StreamNumber, S1, KindOfInfo, KindOfSearch);
    }
    if (Parameter==_T("Channels"))
        return Get(StreamKind, StreamNumber, _T("Channel(s)"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("Artist"))
        return Get(StreamKind, StreamNumber, _T("Performer"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("AspectRatio"))
        return Get(StreamKind, StreamNumber, _T("DisplayAspectRatio"), KindOfInfo, KindOfSearch);
    if (Parameter==_T("AspectRatio/String"))
        return Get(StreamKind, StreamNumber, _T("DisplayAspectRatio/String"), KindOfInfo, KindOfSearch);

    //Check integrity
    if (StreamKind>=Stream_Max || StreamNumber>=(*Stream)[StreamKind].size() || (ParameterI=MediaInfoLib::Config.Info_Get(StreamKind).Find(Parameter, KindOfSearch))==Error || KindOfInfo>=Info_Max)
        return MediaInfoLib::Config.EmptyString_Get(); //Parameter is unknown

    return Get(StreamKind, StreamNumber, ParameterI, KindOfInfo);
}

//---------------------------------------------------------------------------
int File__Base::Set (stream_t StreamKind, size_t StreamNumber, size_t Parameter, const Ztring &ToSet, const Ztring &OldValue)
{
    //Integrity
    if (Count_Get(StreamKind)<=StreamNumber)
        return 0;

    return Set(StreamKind, StreamNumber, Get(StreamKind, StreamNumber, Parameter, Info_Name), ToSet, OldValue);
}

//---------------------------------------------------------------------------
int File__Base::Set (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, const Ztring &ToSet, const Ztring &OldValue)
{
    //Integrity
    if (Count_Get(StreamKind)<=StreamNumber)
        return 0;

    //Fill(StreamKind, StreamNumber, Parameter.To_Local().c_str(), ToSet);

    return Write(StreamKind, StreamNumber, Parameter, ToSet, OldValue);
}

//---------------------------------------------------------------------------
void File__Base::Language_Set()
{
/*
    for (size_t StreamKind=(size_t)Stream_General; StreamKind<(size_t)Stream_Max; StreamKind++)//Note : Optimisation, only the first (*Stream) is, so StreamNumber is only 0
        for (size_t Pos=0; Pos<MediaInfoLib::Config.Info[StreamKind].size(); Pos++)
        {
             //Info_Name_Text
             const Ztring &Z1=MediaInfoLib::Config.Language_Get(MediaInfoLib::Config.Info_Get((stream_t)StreamKind, Pos, Info_Name));
             if (Z1.empty())
                Set((stream_t)StreamKind, 0, Pos, Info_Name_Text, MediaInfoLib::Config.Info_Get((stream_t) StreamKind, Pos, Info_Name));
             else
                Set((stream_t)StreamKind, 0, Pos, Info_Name_Text, Z1);
             //Info_Measure_Text
             const Ztring Z2=MediaInfoLib::Config.Language_Get(MediaInfoLib::Config.Info_Get((stream_t)StreamKind, Pos, Info_Measure));
             if (Z2.empty())
                Set((stream_t)StreamKind, 0, Pos, Info_Measure_Text, MediaInfoLib::Config.Info_Get((stream_t)StreamKind, Pos, Info_Measure);
             else
                Set((stream_t)StreamKind, 0, Pos, Info_Measure_Text, Z2);
        }
*/
}

//***************************************************************************
// Demux
//***************************************************************************

void File__Base::Demux (const int8u* Buffer, size_t Buffer_Size, const Ztring& StreamName, bool)
{
    if (!MediaInfoLib::Config.Demux_Get())
        return;

    if (File_Name.empty())
        return;

    File F;
    F.Open(File_Name+_T('.')+StreamName, File::Access_Write_Append);
    F.Write(Buffer, Buffer_Size);
}

//***************************************************************************
// Divers
//***************************************************************************

void File__Base::Clear()
{
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
        (*Stream)[StreamKind].clear();
}

//---------------------------------------------------------------------------
void File__Base::Read_Buffer_Init()
{
}

//---------------------------------------------------------------------------
void File__Base::Read_Buffer_Unsynched()
{
}

//---------------------------------------------------------------------------
void File__Base::Read_Buffer_Continue()
{
    File_GoTo=File_Size;
}

//---------------------------------------------------------------------------
void File__Base::Read_Buffer_Finalize()
{
}

//---------------------------------------------------------------------------
int File__Base::Write(stream_t, size_t, const Ztring &, const Ztring &, const Ztring &)
{
    return -1;
}

int File__Base::WriteToDisk()
{
    return -1;
}

} //NameSpace


