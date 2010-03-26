// File_DtvccTransport - Info for DTVCC Transport strams
// Copyright (C) 2010-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_DTVCCTRANSPORT_YES)
#if defined(MEDIAINFO_EIA608_YES)
    #include "MediaInfo/Text/File_Eia608.h"
#endif
#if defined(MEDIAINFO_EIA708_YES)
    #include "MediaInfo/Text/File_Eia708.h"
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_DtvccTransport.h"

//---------------------------------------------------------------------------
namespace MediaInfoLib
{
//---------------------------------------------------------------------------

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const char* DtvccTransport_cc_type (int8u cc_type)
{
    switch (cc_type)
    {
        case  0 : return "CEA-608 line 21 field 1 closed captions"; //closed caption 3 if this is second field
        case  1 : return "CEA-608 line 21 field 2 closed captions"; //closed caption 4 if this is second field
        case  2 : return "DTVCC Channel Packet Data";
        case  3 : return "DTVCC Channel Packet Start";
        default : return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_DtvccTransport::File_DtvccTransport()
:File__Analyze()
{
    //Temp
    Streams.resize(3); //CEA-608 Field 1, CEA-608 Field 2, CEA-708 Channel
    Streams_Count=0;
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_DtvccTransport::Streams_Fill()
{
    //Filling
    for (size_t Pos=0; Pos<Streams.size(); Pos++)
        if (Streams[Pos].Parser && Streams[Pos].Parser->Status[IsFilled])
        {
            Merge(*Streams[Pos].Parser);
            if (Pos<2)
                Fill(Stream_Text, StreamPos_Last, Text_ID, _T("608-")+Ztring::ToZtring(Pos));
            Fill(Stream_Text, StreamPos_Last, "MuxingMode", _T("EIA-708"), Unlimited);
        }
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
void File_DtvccTransport::Read_Buffer_Unsynched()
{
    //Parsing
    for (size_t Pos=0; Pos<Streams.size(); Pos++)
        if (Streams[Pos].Parser)
            Streams[Pos].Parser->Open_Buffer_Unsynch();
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_DtvccTransport::Read_Buffer_Continue()
{
    //Parsing
    Element_Begin("DTVCC Transport");
    int8u  cc_count;
    bool   process_em_data_flag, process_cc_data_flag, additional_data_flag;
    BS_Begin();
    Get_SB (process_em_data_flag,                               "process_em_data_flag");
    Get_SB (process_cc_data_flag,                               "process_cc_data_flag");
    Get_SB (additional_data_flag,                               "additional_data_flag");
    Get_S1 (5, cc_count,                                        "cc_count");
    BS_End();
    Skip_B1(                                                    process_em_data_flag?"em_data":"reserved"); //Emergency message
    if (process_cc_data_flag)
    {
        for (int8u Pos=0; Pos<cc_count; Pos++)
        {
            Element_Begin("cc");
            int8u cc_type;
            bool  cc_valid;
            BS_Begin();
            Mark_1();
            Mark_1();
            Mark_1();
            Mark_1();
            Mark_1();
            Get_SB (   cc_valid,                                    "cc_valid");
            Get_S1 (2, cc_type,                                     "cc_type"); Param_Info(DtvccTransport_cc_type(cc_type));
            BS_End();
            if (cc_valid)
            {
                Element_Begin("cc_data");
                    //Calculating the parser position
                    int8u Parser_Pos=cc_type==3?2:cc_type; //cc_type 2 and 3 are for the same text

                    //Parsing
                    if (Streams[Parser_Pos].Parser==NULL)
                    {
                        if (cc_type<2)
                        {
                            Streams[Parser_Pos].Parser=new File_Eia608();
                        }
                        else
                        {
                            Streams[Parser_Pos].Parser=new File_Eia708();
                        }
                    }
                    if (!Streams[Parser_Pos].Parser->Status[IsFinished])
                    {
                        if (Parser_Pos==2)
                            ((File_Eia708*)Streams[2].Parser)->cc_type=cc_type;
                        Open_Buffer_Init(Streams[Parser_Pos].Parser);
                        Open_Buffer_Continue(Streams[Parser_Pos].Parser, Buffer+(size_t)(Buffer_Offset+Element_Offset), 2);
                        Element_Offset+=2;

                        //Filled
                        if (!Streams[Parser_Pos].IsFilled && Streams[Parser_Pos].Parser->Status[IsFilled])
                        {
                            if (Count_Get(Stream_General)==0)
                                Accept("DTVCC Transport");
                            Streams_Count++;
                            if (Streams_Count==3)
                                Fill("DTVCC Transport");
                        }
                    }
                    else
                        Skip_XX(2,                                  "Data");
                Element_End();
            }
            else
                Skip_XX(2,                                          "Junk");
            Element_End();
        }
    }
    else
        Skip_XX(cc_count*2,                                         "Junk");

    BS_Begin();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    BS_End();

    while (Element_Offset<Element_Size)
    {
        Skip_B1(                                                    "Zero"); //TODO: test Zero
    }

    Element_End();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_DTVCCTRANSPORT_YES

