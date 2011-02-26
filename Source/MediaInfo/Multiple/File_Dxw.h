// File_P2_Clip - Info for P2 Clip (XML) files
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
// Information about DXW files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_DxwH
#define MediaInfo_File_DxwH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <vector>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Dxw
//***************************************************************************

class File_Dxw : public File__Analyze
{
private :
    //Streams management
    void Streams_Finish ();
    void Streams_Finish_ParseReference ();
    void Streams_Finish_ParseReference_Finalize ();

    //Buffer - File header
    bool FileHeader_Begin();

    //Temp
    struct reference
    {
        Ztring      FileName;
        stream_t    StreamKind;

        reference()
        {
            StreamKind=Stream_Max;
        }
    };
    typedef std::vector<reference> references;
    references              References;
    references::iterator    Reference;
    MediaInfo_Internal*     MI;
    int64u                  File_Size_Total;
};

} //NameSpace

#endif

