// File__Analyze - Base for analyze files
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
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
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "ZenLib/File.h"
#include "ZenLib/BitStream_LE.h"
#include <math.h>
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
    Stream[KindOfStream]->push_back(ZtringListList());
    Optimized[KindOfStream].push_back(true);

    //Fill the stream
    ZtringListList* ZLL=&(*Stream[KindOfStream])[Stream[KindOfStream]->size()-1];
    ZLL->reserve(Config.Info_Get(KindOfStream).size());
    for (size_t Pos=0; Pos<Config.Info_Get(KindOfStream).size(); Pos++)
    {
        (*ZLL)(Pos, Info_Name)      =Config.Info_Get(KindOfStream)[Pos][Info_Name];
        /*TEST (*ZLL)(Pos, Info_Options)   =Config.Info_Get(KindOfStream)(Pos, Info_Options);*/
        //Note: if you add a stream here, don't forget to put an exception in ::Get()
    }
    ZLL->Write(Config.Language_Get(Config.Info_Get(KindOfStream, 2, Info_Text)), 2, Info_Text);

    //Special cases
    if (KindOfStream==Stream_General)
        General_Fill();
    else
        ZLL->Write(Ztring::ToZtring(Stream[KindOfStream]->size()), 3, Info_Text);

    StreamKind_Last=KindOfStream;
    StreamPos_Last=Stream[KindOfStream]->size()-1;

    //Fill with already ready data
    for (size_t Pos=0; Pos<Fill_Temp.size(); Pos++)
        Fill(Fill_Temp(Pos, 0).To_UTF8().c_str(), Fill_Temp(Pos, 1));
    Fill_Temp.clear();

    return Stream[KindOfStream]->size()-1; //The position in the stream count
}

//***************************************************************************
// Filling
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const Ztring &Value, bool Replace)
{
    //Integrity
    if (StreamKind>Stream_Max || Parameter==NULL || Parameter[0]=='\0')
        return;

    //Fill details
    //if (!Value.empty())
    //    Details_Add_Info(Error, (std::string("--> ")+Parameter).c_str(), Value);

    //Handle Value before StreamKind
    if (StreamKind==Stream_Max || StreamPos>=(*Stream[StreamKind]).size())
    {
        ZtringList NewList;
        NewList.push_back(Ztring().From_UTF8(Parameter));
        NewList.push_back(Value);
        Fill_Temp.push_back(NewList);
        return; //No streams
    }

    Ztring &Target=(*Stream[StreamKind])[StreamPos](Ztring().From_Local(Parameter));
    if (Target.empty() || Replace)
        Target=Value; //First value
    else if (Value.empty())
        Target.clear(); //Empty value --> clear other values
    else
    {
        Target+=Config.TagSeparator_Get();
        Target+=Value;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Fill (stream_t StreamKind, size_t StreamPos, int32u Parameter, const Ztring &Value, bool Replace)
{
    std::string ParameterS;
    ParameterS.append(1, (char)((Parameter&0xFF000000)>>24));
    ParameterS.append(1, (char)((Parameter&0x00FF0000)>>16));
    ParameterS.append(1, (char)((Parameter&0x0000FF00)>> 8));
    ParameterS.append(1, (char)((Parameter&0x000000FF)>> 0));
    Fill(StreamKind, StreamPos, ParameterS.c_str(), Value, Replace);
}

//---------------------------------------------------------------------------
void File__Analyze::Fill_HowTo (stream_t StreamKind, size_t StreamPos, const char* Parameter, const char* Value)
{
    //Integrity
    if (StreamKind>Stream_Max)
        return;

    Ztring Param; Param.From_UTF8(Parameter);

    //Test if stream is already created
    while (Count_Get(StreamKind)<=StreamPos)
        Stream_Prepare(StreamKind);

    //Test if parameter exists
    if ((*Stream[StreamKind])[StreamPos].Find(Param)==Error)
        return;

   (*Stream[StreamKind])[StreamPos](Param, Info_HowTo).From_UTF8(Value);
}

//---------------------------------------------------------------------------
void File__Analyze::Fill_Flush()
{
    Stream_Prepare(Stream_Max); //clear filling
    Fill_Temp.clear();
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(const File__Base &ToAdd)
{
    size_t Count=0;
    for (size_t StreamKind=(size_t)Stream_General+1; StreamKind<(size_t)Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<ToAdd.Stream[StreamKind]->size(); StreamPos++)
        {
            //Prepare a new stream
            Stream_Prepare((stream_t)StreamKind);

            //Merge
            if (!ToAdd.Stream[StreamKind]->at(StreamPos).empty())
                Stream[StreamKind]->at(StreamPos_Last)=ToAdd.Stream[StreamKind]->at(StreamPos);
            Count++;
        }
    return Count;
}

//---------------------------------------------------------------------------
size_t File__Analyze::Merge(File__Base &ToAdd, stream_t StreamKind, size_t StreamPos_From, size_t StreamPos_To)
{
    //Integrity
    if (&ToAdd==NULL || StreamKind>=Stream_Max || StreamPos_From>=ToAdd.Stream[StreamKind]->size())
        return 0;

    //Destination
    while (StreamPos_To>=Stream[StreamKind]->size())
        Stream_Prepare(StreamKind);

    //Merging
    size_t Count=0;
    for (size_t Pos=0; Pos<ToAdd.Stream[StreamKind]->at(StreamPos_From).size(); Pos++)
    {
        Ztring ToFill_Name=ToAdd.Get(StreamKind, StreamPos_From, Pos, Info_Name);
        Ztring ToFill_Value=ToAdd.Get(StreamKind, StreamPos_From, Pos);
        if (!ToFill_Value.empty())
        {
            //Ztring &Target=Get(StreamKind, StreamPos_To, Pos);
            //Target=ToFill_Value;
            (Stream[StreamKind]->at(StreamPos_To))(ToFill_Name)=ToFill_Value;
            //Fill(StreamKind, StreamPos_To, ToFill_Name.To_Local().c_str(), ToFill_Value);
            Count++;
        }
    }

    return 1;
}

} //NameSpace

