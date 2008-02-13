// File__Duplicate_MpegTs - Duplication of some formats
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
#ifndef File__Duplicate_MpegTsH
#define File__Duplicate_MpegTsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Duplicate/File__Duplicate__Base.h"
#include "MediaInfo/Duplicate/File__Duplicate__Writer.h"
#include <ZenLib/ZtringListList.h>
#include <set>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__Duplicate_MpegTs
//***************************************************************************

class File__Duplicate_MpegTs : public File__Duplicate__Base
{
public :
    //Constructor/Destructor
    File__Duplicate_MpegTs(const Ztring &Target);

    //Set
    void   Configure (const Ztring &Value);

    //Write
    void   Write (const int8u* ToAdd=NULL, size_t ToAdd_Size=0);

    //Output buffer
    size_t Output_Buffer_Get (const Ztring &Target, unsigned char** Output_Buffer=NULL);

//private :
    File__Duplicate__Writer Writer;

    std::set<int16u> Wanted_program_numbers;
    std::set<int16u> Wanted_program_map_PIDs;
    std::set<int16u> Wanted_elementary_PIDs;
    std::set<int16u> program_map_PIDs;
    std::set<int16u> elementary_PIDs;

    //Temp
    int8u* Buffer;
    size_t Buffer_Pos;
    size_t Buffer_End;
    size_t Buffer_Size;
    size_t adaptation_field_length;
    size_t pointer_field;
    size_t section_length;
    int8u                   PAT_version_number;
    int8u                   PAT_version_number_ToBuffer;
    std::map<int16u, int8u> PMT_version_numbers;
    std::map<int16u, int8u> PMT_version_numbers_ToBuffer;
    void Manage_PAT(const int8u* ToAdd, size_t ToAdd_Size);
    void Manage_PMT(const int8u* ToAdd, size_t ToAdd_Size);
    bool Parsing_Begin(const int8u* ToAdd, size_t ToAdd_Size);
    void Parsing_End();
};


} //NameSpace

#endif
