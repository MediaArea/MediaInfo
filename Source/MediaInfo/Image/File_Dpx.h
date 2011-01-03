// File_Dpx - Info for DPX (SMPTE 268M) files
// Copyright (C) 2010-2011 MediaArea.net SARL, Info@MediaArea.net
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
//
// Information about DPX files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_DpxH
#define MediaInfo_File_DpxH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Dpx
//***************************************************************************

class File_Dpx : public File__Analyze
{
public :
    //Constructor/Destructor
    File_Dpx();
    
private :
    //Streams management
    void Streams_Finish();

    //Buffer - File header
    bool FileHeader_Begin();

    //Buffer - Per element
    void Header_Parse();
    void Data_Parse();

    //Elements
    void FileInformationHeader();
    void GenericSectionHeader();
    void GenericSectionHeader_ImageElement();
    void IndustrySpecificHeader();
    void UserDefinedHeader();

    //Temp
    std::vector<int32u> Sizes;
    size_t              Sizes_Pos;
};

} //NameSpace

#endif
