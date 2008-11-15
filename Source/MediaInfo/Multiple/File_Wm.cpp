// File_Wm - Info for Windows Media files
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
// Main part
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
#ifdef MEDIAINFO_WM_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Wm.h"
#if defined(MEDIAINFO_MPEGPS_YES)
    #include "MediaInfo/Multiple/File_MpegPs.h"
#endif
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Wm::File_Wm()
:File__Analyze()
{
    //Configuration
    DataMustAlwaysBeComplete=false;

    //Stream
    Packet_Count=0;
    MaximumDataPacketSize=0;
    SizeOfMediaObject_BytesAlreadyParsed=0;
    FileProperties_Preroll=0;
    Codec_Description_Count=0;
    Stream_Number=0;
    Data_Parse_Padding=0;
    MaximumDataPacketSize=(int32u)-1;
    NumberPayloads=1;
    NumberPayloads_Pos=0;
    Data_Parse_Begin=true;
    IsDvrMs=false;
}

//---------------------------------------------------------------------------
void File_Wm::Read_Buffer_Finalize()
{
    //Encryption management
    /*const Ztring& Encryption=Retrieve(Stream_General, 0, General_Encryption);
    if (!Encryption.empty())
    {
        for (size_t StreamKind=Stream_General+1; StreamKind<Stream_Max; StreamKind++)
            for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
                Fill ((stream_t)StreamKind, 0, "Encryption", Encryption);
    }

        Fill("BitRate", CurrentBitRate[StreamNumber]);
    */

    std::map<int16u, stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        std::map<std::string, ZenLib::Ztring>::iterator Info_Temp=Temp->second.Info.begin();
        while (Info_Temp!=Temp->second.Info.end())
        {
            if (Codec_Description_Count==Stream.size() || Info_Temp->first!="CodecID_Description") //With some files, There are only x Codec desecription and !x streams, no coherancy
                Fill(Temp->second.StreamKind, Temp->second.StreamPos, Info_Temp->first.c_str(), Info_Temp->second, true);
            Info_Temp++;
        }

        if (Temp->second.StreamKind==Stream_Video)
        {
            //Some tests about the frame rate
            std::map<int32u, int32u> PresentationTime_Deltas_Problem;
            std::map<int32u, int32u> PresentationTime_Deltas_Most;
            for (std::map<ZenLib::int32u, ZenLib::int32u>::iterator PresentationTime_Delta=Temp->second.PresentationTime_Deltas.begin(); PresentationTime_Delta!=Temp->second.PresentationTime_Deltas.end(); PresentationTime_Delta++)
            {
                if (PresentationTime_Delta->second>=5)
                    PresentationTime_Deltas_Problem[PresentationTime_Delta->first]=PresentationTime_Delta->second;
                if (PresentationTime_Delta->second>=30)
                    PresentationTime_Deltas_Most[PresentationTime_Delta->first]=PresentationTime_Delta->second;
            }

            if (PresentationTime_Deltas_Most.size()==0
             || (PresentationTime_Deltas_Most.size()==1 && PresentationTime_Deltas_Problem.size()>1)
             || (PresentationTime_Deltas_Most.size()==2 && PresentationTime_Deltas_Problem.size()>2))
            {
                if (Temp->second.AverageTimePerFrame>0)
                    Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate", ((float)10000000)/Temp->second.AverageTimePerFrame, 3, true);
            }
            else if (PresentationTime_Deltas_Most.size()==1)
            {
                Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate", 1000/((float64)PresentationTime_Deltas_Most.begin()->first), 3, true);
                if (Temp->second.AverageTimePerFrame>0)
                    Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate_Nominal", ((float)10000000)/Temp->second.AverageTimePerFrame, 3, true);
            }
            else if (PresentationTime_Deltas_Most.size()==2)
            {
                std::map<int32u, int32u>::iterator PresentationTime_Delta_Most=PresentationTime_Deltas_Most.begin();
                float64 PresentationTime_Deltas_1_Value=(float64)PresentationTime_Delta_Most->first;
                float64 PresentationTime_Deltas_1_Count=(float64)PresentationTime_Delta_Most->second;
                PresentationTime_Delta_Most++;
                float64 PresentationTime_Deltas_2_Value=(float64)PresentationTime_Delta_Most->first;
                float64 PresentationTime_Deltas_2_Count=(float64)PresentationTime_Delta_Most->second;
                float64 FrameRate_Real=1000/(((PresentationTime_Deltas_1_Value*PresentationTime_Deltas_1_Count)+(PresentationTime_Deltas_2_Value*PresentationTime_Deltas_2_Count))/(PresentationTime_Deltas_1_Count+PresentationTime_Deltas_2_Count));
                     if (FrameRate_Real> 9.990 && FrameRate_Real<=10.010) FrameRate_Real=10.000;
                else if (FrameRate_Real>14.990 && FrameRate_Real<=15.010) FrameRate_Real=15.000;
                else if (FrameRate_Real>23.964 && FrameRate_Real<=23.988) FrameRate_Real=23.976;
                else if (FrameRate_Real>23.988 && FrameRate_Real<=24.012) FrameRate_Real=24.000;
                else if (FrameRate_Real>24.988 && FrameRate_Real<=25.012) FrameRate_Real=24.000;
                else if (FrameRate_Real>29.955 && FrameRate_Real<=29.985) FrameRate_Real=29.970;
                else if (FrameRate_Real>29.985 && FrameRate_Real<=30.015) FrameRate_Real=30.000;
                else if (FrameRate_Real>23.964*2 && FrameRate_Real<=23.988*2) FrameRate_Real=23.976;
                else if (FrameRate_Real>23.988*2 && FrameRate_Real<=24.012*2) FrameRate_Real=24.000;
                else if (FrameRate_Real>24.988*2 && FrameRate_Real<=25.012*2) FrameRate_Real=25.000;
                else if (FrameRate_Real>29.955*2 && FrameRate_Real<=29.985*2) FrameRate_Real=29.970;
                else if (FrameRate_Real>30.985*2 && FrameRate_Real<=30.015*2) FrameRate_Real=30.000;
                Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate", FrameRate_Real, 3, true);
                if (Temp->second.AverageTimePerFrame>0)
                    Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate_Nominal", ((float)10000000)/Temp->second.AverageTimePerFrame, 3, true);
            }
            else
            {
                Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate_Mode", "VFR");
                if (Temp->second.AverageTimePerFrame>0)
                    Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate_Nominal", ((float)10000000)/Temp->second.AverageTimePerFrame, 3, true);
            }
        }
        if (Temp->second.AverageBitRate>0)
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "BitRate", Temp->second.AverageBitRate, 10, true);
        if (Temp->second.LanguageID!=(int16u)-1 && Temp->second.LanguageID<(int16u)Languages.size())
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "Language", Languages[Temp->second.LanguageID]);
        else if (!Language_ForAll.empty())
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "Language", Language_ForAll);
        if (Temp->second.Parser)
        {
            if (Temp->second.StreamKind==Stream_Max)
                if (Temp->second.Parser->Count_Get(Stream_Audio))
                {
                    Stream_Prepare(Stream_Audio);
                    Temp->second.StreamKind=StreamKind_Last;
                    Temp->second.StreamPos=StreamPos_Last;
                }
            Open_Buffer_Finalize(Temp->second.Parser);
            Merge(*Temp->second.Parser, Temp->second.StreamKind, 0, Temp->second.StreamPos);
        }
        Temp++;
    }

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Stream.clear();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wm::Header_Parse()
{
    if (!MustUseAlternativeParser)
    {
        //Parsing
        int128u Name;
        int64u Size;
        Get_UUID(Name,                                              "Name");
        Get_L8 (Size,                                               "Size");

        //Filling
        #ifndef __BORLANDC__
            Header_Fill_Code(Name.hi, Ztring().From_UUID(Name));
        #else //__BORLANDC__
            Header_Fill_Code(Name.hi&0xFFFFFFFF, Ztring().From_UUID(Name)); //Borland does not like int64u for const?
        #endif //__BORLANDC__
        Header_Fill_Size(Size);
    }
    else
    {
        Header_Fill_Code(0, "Packet");
        Header_Fill_Size(MaximumDataPacketSize);
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_WM_YES
