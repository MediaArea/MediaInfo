// File_Mpegts - Info for MPEG Transport Stream files
// Copyright (C) 2006-2009 Jerome Martinez, Zen@MediaArea.net
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
// Information about MPEG Transport Stream files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_MpegTsH
#define MediaInfo_MpegTsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Duplicate.h"
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
#include "MediaInfo/Duplicate/File__Duplicate_MpegTs.h"
#include "MediaInfo/Duplicate/File__Duplicate__Writer.h"
#include <map>
#include <vector>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_MpegTs
//***************************************************************************

class File_MpegTs : public File__Duplicate
{
public :
    //In
    size_t BDAV_Size;
    size_t TSP_Size;

    //Constructor/Destructor
    File_MpegTs();
    ~File_MpegTs();

private :
    //Buffer - File header
    bool FileHeader_Begin();

    //Buffer - Synchro
    bool Synchronize();
    bool Synched_Test();
    void Synched_Init();

    //Buffer - Global
    void Read_Buffer_Finalize ();

    //Options
    void Option_Manage ();

    //Buffer - Per element
    void Header_Parse();
    void Header_Parse_AdaptationField();
    void Data_Parse();

    int16u                      pid;
    bool                        payload_unit_start_indicator;

    //Global infos
    complete_stream* Complete_Stream;

    //Elements
    void PSI();
    void PES();

    //Helpers
    bool Header_Parser_QuickSearch();
    void Detect_EOF();

    //Temp
    size_t TS_Size;
    int64u MpegTs_JumpTo_Begin;
    int64u MpegTs_JumpTo_End;

    //File__Duplicate
    void   File__Duplicate_Delete();
    void   File__Duplicate_Read_Buffer_Finalize ();
    bool   File__Duplicate_Set  (const Ztring &Value); //Fill a new File__Duplicate value
    bool   File__Duplicate_Get_From_PID (int16u PID);
    void   File__Duplicate_Write (int16u PID);

    //Output buffer
    size_t Output_Buffer_Get (const String &Value);
    size_t Output_Buffer_Get (size_t Pos);
    std::vector<int16u> Output_Buffer_Get_Pos;
};

} //NameSpace

#endif
