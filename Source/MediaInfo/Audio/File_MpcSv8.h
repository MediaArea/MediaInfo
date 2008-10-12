// File_MpcSv8 - Info for Musepack (SV8) files
// Copyright (C) 2008-2008 Jerome Martinez, Zen@MediaArea.net
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
// Information about Musepack files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_MpcSv8H
#define MediaInfo_File_MpcSv8H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/Tag/File__Tags.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_MpcSv8
//***************************************************************************

class File_MpcSv8 : public File__Analyze, public File__Tags_Helper
{
protected :
    //Format
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

public :
    File_MpcSv8();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void FileHeader_Parse();
    void Data_Parse();

    //Elements
    void AP();
    void CT();
    void EI();
    void RG();
    void SE();
    void SH();
    void SO();
    void ST();
};

} //NameSpace

#endif

