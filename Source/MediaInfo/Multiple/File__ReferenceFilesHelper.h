// File__ReferenceFilesHelper - class for analyzing/demuxing reference files
// Copyright (C) 2011-2011 MediaArea.net SARL, Info@MediaArea.net
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef File__ReferenceFilesHelperH
#define File__ReferenceFilesHelperH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <vector>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__ReferenceFilesHelper
//***************************************************************************

class File__ReferenceFilesHelper
{
public :
    //In
    struct reference
    {
        ZtringList          FileNames;
        stream_t            StreamKind;
        size_t              StreamPos;
        Ztring              StreamID;
        float64             FrameRate;
        MediaInfo_Internal* MI;
        #if MEDIAINFO_NEXTPACKET
            std::bitset<32> Status;
        #endif //MEDIAINFO_NEXTPACKET

        reference()
        {
            FileNames.Separator_Set(0, _T(","));
            StreamKind=Stream_Max;
            StreamPos=(size_t)-1;
            FrameRate=0;
            MI=NULL;
        }
    };
    typedef std::vector<reference>  references;
    references                      References;

    //Streams management
    void ParseReferences();

    //Constructor / Destructor
    File__ReferenceFilesHelper(File__Analyze* MI, MediaInfo_Config_MediaInfo* Config);

private :
    //Streams management
    void ParseReference ();
    void ParseReference_Finalize ();

    //temp
    File__Analyze*                  MI;
    MediaInfo_Config_MediaInfo*     Config;
    references::iterator            Reference;
    int64u                          File_Size_Total;
    bool                            Init_Done;
    bool                            Demux_Interleave;
    size_t                          CountOfReferencesToParse;
};

} //NameSpace

#endif

