// File_Aac_Adif - Info for AAC (ADIF) files
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
#ifndef MediaInfo_File_AdifH
#define MediaInfo_File_AdifH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/Tag/File__Tags.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Adif
//***************************************************************************

class File_Adif : public File__Analyze, public File__Tags_Helper
{
protected :
    //Format
    void Read_Buffer_Continue ();

public :
    File_Adif();

private :
    //Buffer
    bool FileHeader_Begin();
    void FileHeader_Parse();
    void Data_Parse_Fill();

    //Temp
    Ztring comment_field_data;
    int32u bitrate;
    int8u  object_type;
    int8u  sampling_frequency_index;
    int8u  num_front_channel_elements;
    int8u  num_side_channel_elements;
    int8u  num_back_channel_elements;
    int8u  num_lfe_channel_elements;
    int8u  num_assoc_data_elements;
    int8u  num_valid_cc_elements;
    bool   bitstream_type;
};

} //NameSpace

#endif
