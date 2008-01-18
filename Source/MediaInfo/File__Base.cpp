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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include <ZenLib/File.h>
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
    Stream[Stream_General]=&General;
    Stream[Stream_Video]=&Video;
    Stream[Stream_Audio]=&Audio;
    Stream[Stream_Text]=&Text;
    Stream[Stream_Chapters]=&Chapters;
    Stream[Stream_Image]=&Image;
    Stream[Stream_Menu]=&Menu;

    //File
    File_Size=(int64u)-1;
    File_Offset=0;
    File_GoTo=(int64u)-1;
    File_MaximumOffset=1024*1024;

    //Optimization init
    StreamKind_Last=Stream_Max;
    StreamPos_Last=Error;

    //Debug
    Synched=false;
    Trusted=Error;
}

//---------------------------------------------------------------------------
//Constructeurs
File__Base::~File__Base ()
{
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
    if (Pos==Error)
        return (*Stream[StreamKind]).size();

    //Integrity
    if (Pos>=(*Stream[StreamKind]).size())
        return 0;

    //Count of piece of information in a stream
    return (*Stream[StreamKind])[Pos].size();
}

//---------------------------------------------------------------------------
const Ztring &File__Base::Get (stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t KindOfInfo)
{
    //Check integrity
    if (StreamKind>=Stream_Max || StreamNumber>=(*Stream[StreamKind]).size() || Parameter>=(*Stream[StreamKind])[StreamNumber].size() || KindOfInfo>=Info_Max)
        return Config.EmptyString_Get(); //Parameter is unknown

    //OK for Optimization?
    if (Optimized[StreamKind][StreamNumber] && (*Stream[StreamKind])[StreamNumber].size()-Config.Info_Get(StreamKind).size()!=0)
    {
        //Can't be optimized
        /*TEST
        Ztring A=Audio[0](_T("Video0_Delay"), Info_Options);
        A.clear();
        */
        for (size_t Pos=0; Pos<(*Stream[StreamKind])[StreamNumber].size(); Pos++)
        {
            size_t Pos_Info=Config.Info_Get(StreamKind).Find((*Stream[StreamKind])[StreamNumber][Pos][0]);
            if (Pos_Info!=Error)
                for (size_t Pos1=Config.Info_Get(StreamKind)[Pos_Info].size()-1; Pos1>=Info_Measure; Pos1--) //-- for optimization of ZtringList
                {
                    /*TEST if (Pos1!=Info_Options)  */
                         (*Stream[StreamKind])[StreamNumber][Pos](Pos1)=Config.Info_Get(StreamKind)[Pos_Info][Pos1];
                }
        }
        /*TEST
        A=Audio[0](_T("Video0_Delay"), Info_Options);
        A.clear();
        */
        Optimized[StreamKind][StreamNumber]=false;
    }

    /*TEST
    if (Count_Get(Stream_Audio)>0 && StreamKind==Stream_Audio && Audio[0](Parameter, 0)==_T("Video0_Delay"))
    {
        Ztring A=Audio[0](_T("Video0_Delay"), Info_Options);
        A.clear();
    }
    */
    //Verify validity of strings
    //-Info_Options
     if (!Optimized[StreamKind][StreamNumber] && KindOfInfo==Info_Options && (*Stream[StreamKind])[StreamNumber](Parameter, Info_Options).empty())
        (*Stream[StreamKind])[StreamNumber](Parameter, Info_Options)=_T("Y YT");

    //Optimization : KindOfInfo>Info_Text is in static lists
    if (KindOfInfo!=Info_HowTo && Optimized[StreamKind][StreamNumber] && KindOfInfo>=Info_Measure && (size_t)KindOfInfo<Config.Info_Get(StreamKind)[Parameter].size())
        return Config.Info_Get(StreamKind)[Parameter][KindOfInfo]; //look for static information only
    else
        return (*Stream[StreamKind])[StreamNumber][Parameter](KindOfInfo);
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
    if (StreamKind>=Stream_Max || StreamNumber>=(*Stream[StreamKind]).size() || (ParameterI=(*Stream[StreamKind])[StreamNumber].Find(Parameter, KindOfSearch))==Error || KindOfInfo>=Info_Max)
        return Config.EmptyString_Get(); //Parameter is unknown

    //Special cases
    //-Inform for a stream
    if (Parameter==_T("Inform"))
        (*Stream[StreamKind])[StreamNumber](_T("Inform"))=Inform(StreamKind, StreamNumber);

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
ZtringListList File__Base::Info_Capacities()
{
    //Ask to format parser to fill information
    Clear();

    //Fill
    ZtringListList ToReturn;
    size_t ToReturn_Pos;
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        HowTo((stream_t)StreamKind);
        ToReturn_Pos=0;
        for (size_t Pos=0; Pos<Config.Info_Get((stream_t)StreamKind).size(); Pos++)
            if (Pos<Config.Info_Get((stream_t)StreamKind).size() && ((size_t)Info_Options)<Config.Info_Get((stream_t)StreamKind)[Pos].size() && Config.Info_Get((stream_t)StreamKind)[Pos][Info_Options][InfoOption_ShowInSupported]==_T('Y'))
            {
                ToReturn(StreamKind, ToReturn_Pos)=Get((stream_t)StreamKind, 0, Pos, Info_HowTo);
                ToReturn_Pos++;
            }
    }

    return ToReturn;
}

//---------------------------------------------------------------------------
ZtringListList File__Base::Info_Capacities_Parameters()
{
    //Fill
    ZtringListList ToReturn;
    size_t ToReturn_Pos;
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        ToReturn_Pos=0;
        for (size_t Pos=0; Pos<Config.Info_Get((stream_t)StreamKind).size(); Pos++)
            if (Pos<Config.Info_Get((stream_t)StreamKind).size() && ((size_t)Info_Options)<Config.Info_Get((stream_t)StreamKind)[Pos].size() && Config.Info_Get((stream_t)StreamKind)[Pos][Info_Options][InfoOption_ShowInSupported]==_T('Y'))
            {
                ToReturn(StreamKind, ToReturn_Pos)=Config.Info_Get((stream_t)StreamKind)[Pos][Info_Name];
                ToReturn_Pos++;
            }
    }

    return ToReturn;
}

//---------------------------------------------------------------------------
void File__Base::Language_Set()
{
/*
    for (size_t StreamKind=(size_t)Stream_General; StreamKind<(size_t)Stream_Max; StreamKind++)//Note : Optimisation, only the first Stream is, so StreamNumber is only 0
        for (size_t Pos=0; Pos<Config.Info[StreamKind].size(); Pos++)
        {
             //Info_Name_Text
             const Ztring &Z1=Config.Language_Get(Config.Info_Get((stream_t)StreamKind, Pos, Info_Name));
             if (Z1.empty())
                Set((stream_t)StreamKind, 0, Pos, Info_Name_Text, Config.Info_Get((stream_t) StreamKind, Pos, Info_Name));
             else
                Set((stream_t)StreamKind, 0, Pos, Info_Name_Text, Z1);
             //Info_Measure_Text
             const Ztring Z2=Config.Language_Get(Config.Info_Get((stream_t)StreamKind, Pos, Info_Measure));
             if (Z2.empty())
                Set((stream_t)StreamKind, 0, Pos, Info_Measure_Text, Config.Info_Get((stream_t)StreamKind, Pos, Info_Measure);
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
    if (!Config.Demux_Get())
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
    {
        (*Stream[StreamKind]).clear();
        Optimized[StreamKind].clear();
    }
}

//---------------------------------------------------------------------------
void File__Base::Read_File()
{
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

//---------------------------------------------------------------------------
void File__Base::HowTo(stream_t)
{
    return;
}

} //NameSpace


