// File_Bdmv - Info for Blu-ray Movie files
// Copyright (C) 2009-2010 MediaArea.net SARL, Info@MediaArea.net
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
// Information about Blu-ray Movie files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_BdmvH
#define MediaInfo_File_BdmvH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Bdmv
//***************************************************************************

class File_Bdmv : public File__Analyze
{
public :
    void BDMV(); //The BDMV directory

private :
    //Buffer - File header
    bool FileHeader_Begin();

    //Buffer - Global
    void Read_Buffer_Continue ();

    //Elements
    void Clpi_Streams();
    void Clpi_Streams_Video();
    void Clpi_Streams_Audio();
    void Clpi_Streams_Text();
    void Indx_AppInfoBDMV();
    void Indx_Indexes();
    void Indx_Indexes_Index(int8u object_type);
    void Indx_ExtensionData();
    void Indx_ExtensionData_IDEX();
    void Indx_ExtensionData_IDEX_UIAppInfoAVCHD();
    void Indx_ExtensionData_IDEX_TableOfPlayLists();
    void Indx_ExtensionData_IDEX_MakersPrivateData();
    void Mobj_MovieObjects();
    void Mobj_ExtensionData();
    void Mpls_AppInfoPlayList();
    void Mpls_PlayList();
    void Mpls_PlayList_PlayItem_STN_Video();
    void Mpls_PlayList_PlayItem_STN_Audio();
    void Mpls_PlayList_PlayItem_STN_Text();
    void Mpls_PlayListMarks();

    //Temp
    int8u stream_type;
    std::map<int32u, size_t> Types; //Key is the start adress
};

} //NameSpace

#endif

