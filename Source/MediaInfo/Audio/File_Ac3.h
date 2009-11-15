// File_Ac3 - Info for AC3 files
// Copyright (C) 2004-2009 Jerome Martinez, Zen@MediaArea.net
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
#ifndef MediaInfo_Ac3H
#define MediaInfo_Ac3H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Ac3
//***************************************************************************

class File_Ac3 : public File__Analyze
{
public :
    //In
    size_t Frame_Count_Valid;
    bool   MustParse_dac3;
    bool   MustParse_dec3;

    //Constructor/Destructor
    File_Ac3();

private :
    //Streams management
    void Streams_Fill();

    //Buffer - File header
    bool FileHeader_Begin();

    //Buffer - Synchro
    bool Synchronize();
    bool Synched_Test();

    //Buffer - Global
    void Read_Buffer_Continue ();

    //Buffer - Per element
    void Header_Parse();
    void Data_Parse();

    //Elements
    void dac3();
    void dec3();

    //Temp
    size_t Frame_Count;
    size_t HD_Count;
    int16u chanmap;
    int16u frmsiz;
    int8u  fscod;
    int8u  fscod2;
    int8u  frmsizecod;
    int8u  bsid;
    int8u  bsmod;
    int8u  acmod;
    int8u  dsurmod;
    int8u  numblks;
    int8u  dialnorm;
    int8u  dialnorm2;
    int8u  compr;
    int8u  compr2;
    int8u  dynrng;  //This is only the first occurence of aufblk
    int8u  dynrng2; //This is only the first occurence of aufblk
    bool   lfeon;
    bool   dxc3_Parsed;
    bool   compre;
    bool   compr2e;
    bool   dynrnge;  //This is only the first occurence of aufblk
    bool   dynrng2e; //This is only the first occurence of aufblk
};

} //NameSpace

#endif
