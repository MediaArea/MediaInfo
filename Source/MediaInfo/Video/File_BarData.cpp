// File_BarData - Info for Bar Data Video files
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
#if defined(MEDIAINFO_BARDATA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_BarData.h"

//---------------------------------------------------------------------------
namespace MediaInfoLib
{
//---------------------------------------------------------------------------

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const char* BarData_[]=
{
    "",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_BarData::File_BarData()
:File__Analyze()
{
    //In
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_BarData::Streams_Fill()
{
    //Filling
    Stream_Prepare(Stream_Video);
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_BarData::Read_Buffer_Continue()
{
    //Parsing
    Element_Begin("Bar Data");
    int8u AFD;
    bool AR, Top, Bottom, Left, Right;
    BS_Begin();
    Skip_SB(                                                    "Reserved");
    Get_S1 (4, AFD,                                             "AFD");
    Get_SB (   AR,                                              "Aspect Ratio");
    Skip_S1(2,                                                  "Reserved");
    BS_End();
    Skip_B1(                                                    "Reserved");
    Skip_B1(                                                    "Reserved");
    BS_Begin();
    Get_SB (Top,                                                "Top");
    Get_SB (Bottom,                                             "Bottom");
    Get_SB (Left,                                               "Left");
    Get_SB (Right,                                              "Right");
    Skip_S1(4,                                                  "Reserved");
    Mark_1();
    Mark_1();
    Skip_S2(14,                                                 "Value1");
    Mark_1();
    Mark_1();
    Skip_S2(14,                                                 "Value2");
    BS_End();
    Element_End();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_BARDATA_YES

