// MediaInfo_Config_MediaInfo - Configuration class
// Copyright (C) 2005-2008 Jerome Martinez, Zen@MediaArea.net
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
// Configuration of MediaInfo (per MediaInfo block)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Config_MediaInfoH
#define MediaInfo_Config_MediaInfoH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Internal_Const.h"
#include "ZenLib/CriticalSection.h"
#include "ZenLib/ZtringListList.h"
#include "ZenLib/Translation.h"
#include "ZenLib/InfoMap.h"
#include <map>
#include <vector>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class MediaInfo_Config_MediaInfo
//***************************************************************************

class MediaInfo_Config_MediaInfo
{
public :
    //Constructor/Destructor
    MediaInfo_Config_MediaInfo();

    //General
    Ztring Option (const String &Option, const String &Value=_T(""));

    void          File_IsSeekable_Set (bool NewValue);
    bool          File_IsSeekable_Get ();

    void          File_ForceParser_Set (const Ztring &NewValue);
    Ztring        File_ForceParser_Get ();

    void          File_IsSub_Set (bool NewValue);
    bool          File_IsSub_Get ();

    void          File_Filter_Set     (int64u NewValue);
    bool          File_Filter_Get     (const int16u  Value);
    bool          File_Filter_Get     ();
    bool          File_Filter_HasChanged();

    Ztring        File_Duplicate_Set  (const Ztring &Value);
    Ztring        File_Duplicate_Get  (size_t AlreadyRead_Pos); //Requester must say how many Get() it already read
    bool          File_Duplicate_Get_AlwaysNeeded (size_t AlreadyRead_Pos); //Requester must say how many Get() it already read

    size_t        File__Duplicate_Memory_Indexes_Get (const Ztring &ToFind);
    void          File__Duplicate_Memory_Indexes_Erase (const Ztring &ToFind);

    //Specific
    void          File_MpegTs_ForceMenu_Set (bool NewValue);
    bool          File_MpegTs_ForceMenu_Get ();

private :
    bool                    FileIsSeekable;
    Ztring                  File_ForceParser;
    bool                    File_IsSub;

    //Extra
    std::map<int16u, bool>  File_Filter_16;
    bool                    File_Filter_HasChanged_;

    std::vector<Ztring>     File__Duplicate_List;
    ZtringList              File__Duplicate_Memory_Indexes;

    //Specific
    bool                    File_MpegTs_ForceMenu;

    ZenLib::CriticalSection CS;
};

} //NameSpace

#endif

