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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#include "ZenLib/ZtringListListF.h"
#include <algorithm>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Info
//***************************************************************************

MediaInfo_Config_MediaInfo::MediaInfo_Config_MediaInfo()
{
    FileIsSeekable=true;
    File_Filter_HasChanged_=false;
    File_IsSub=false;

    //Specific
    File_MpegTs_ForceMenu=false;
}

//***************************************************************************
// Info
//***************************************************************************

Ztring MediaInfo_Config_MediaInfo::Option (const String &Option, const String &Value)
{
    String Option_Lower(Option);
    size_t Egal_Pos=Option_Lower.find(_T('='));
    if (Egal_Pos==string::npos)
        Egal_Pos=Option_Lower.size();
    transform(Option_Lower.begin(), Option_Lower.begin()+Egal_Pos, Option_Lower.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix

    if (Option_Lower==_T("file_isseekable"))
    {
        File_IsSeekable_Set(!(Value==_T("0") || Value.empty()));
        return _T("");
    }
    else if (Option_Lower==_T("file_isseekable_get"))
    {
        return File_IsSeekable_Get()?"1":"0";
    }
    else if (Option_Lower==_T("file_forceparser"))
    {
        File_ForceParser_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("file_forceparser_get"))
    {
        return File_ForceParser_Get();
    }
    else if (Option_Lower==_T("file_issub"))
    {
        File_IsSub_Set(!(Value==_T("0") || Value.empty()));
        return _T("");
    }
    else if (Option_Lower==_T("file_issub_get"))
    {
        return File_IsSub_Get()?"1":"0";
    }
    else if (Option_Lower==_T("file_filter"))
    {
        File_Filter_Set(Ztring(Value).To_int64u());
        return _T("");
    }
    else if (Option_Lower==_T("file_filter_get"))
    {
        return Ztring();//.From_Number(File_Filter_Get());
    }
    else if (Option_Lower==_T("file_duplicate"))
    {
        File_Duplicate_Set(Value);
        return _T("");
    }
    else if (Option_Lower==_T("file_duplicate_get"))
    {
        //if (File_Duplicate_Get())
            return _T("1");
        //else
        //    return _T("");
    }
    else if (Option_Lower==_T("file_mpegts_forcemenu"))
    {
        File_MpegTs_ForceMenu_Set(!(Value==_T("0") || Value.empty()));
        return _T("");
    }
    else if (Option_Lower==_T("file_mpegts_forcemenu_get"))
    {
        return File_MpegTs_ForceMenu_Get()?"1":"0";
    }
    else
        return _T("Option not known");
}

//***************************************************************************
// File Is Seekable
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_IsSeekable_Set (bool NewValue)
{
    Enter(true);
    FileIsSeekable=NewValue;
    Leave();
}

bool MediaInfo_Config_MediaInfo::File_IsSeekable_Get ()
{
    Enter();
    Leave();
    return FileIsSeekable;
}

//***************************************************************************
// Force Parser
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_ForceParser_Set (const Ztring &NewValue)
{
    Enter(true);
    File_ForceParser=NewValue;
    Leave();
}

const Ztring &MediaInfo_Config_MediaInfo::File_ForceParser_Get ()
{
    Enter();
    Leave();
    return File_ForceParser;
}

//***************************************************************************
// Is a sub
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_IsSub_Set (bool NewValue)
{
    Enter(true);
    File_IsSub=NewValue;
    Leave();
}

bool MediaInfo_Config_MediaInfo::File_IsSub_Get ()
{
    Enter();
    Leave();
    return File_IsSub;
}

//***************************************************************************
// Filter
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_Filter_Set (int64u NewValue)
{
    Enter(true);
    File_Filter_16[(int16u)NewValue]=true;
    File_Filter_HasChanged_=true;
    Leave();
}

bool MediaInfo_Config_MediaInfo::File_Filter_Get (const int16u Value)
{
    Enter();
    //Test
    bool Exists;
    if (File_Filter_16.empty())
        Exists=true;
    else
        Exists=(File_Filter_16.find(Value)!=File_Filter_16.end());
    Leave();
    return Exists;
}

bool MediaInfo_Config_MediaInfo::File_Filter_Get ()
{
    Enter();
    bool Exist=!File_Filter_16.empty();
    Leave();
    return Exist;
}

bool MediaInfo_Config_MediaInfo::File_Filter_HasChanged ()
{
    Enter();
    bool File_Filter_HasChanged_Temp=File_Filter_HasChanged_;
    File_Filter_HasChanged_=false;
    Leave();
    return File_Filter_HasChanged_Temp;
}

//***************************************************************************
// Duplicate
//***************************************************************************

void MediaInfo_Config_MediaInfo::File_Duplicate_Set (const Ztring &Value)
{
    //Preparing for File__Duplicate...
    Enter(true);
    File__Duplicate_List.push_back(Value);
    File_IsSeekable_Set(false); //If duplicateion, we can not seek anymore
    Leave();
}

Ztring MediaInfo_Config_MediaInfo::File_Duplicate_Get (size_t AlreadyRead_Pos)
{
    Enter();
    if (AlreadyRead_Pos>=File__Duplicate_List.size())
        return Ztring(); //Nothing or not more than the last time
    Ztring Temp=File__Duplicate_List[AlreadyRead_Pos];
    Leave();
    return Temp;
}

bool MediaInfo_Config_MediaInfo::File_Duplicate_Get_AlwaysNeeded (size_t AlreadyRead_Pos)
{
    Enter();
    bool Temp=AlreadyRead_Pos>=File__Duplicate_List.size();
    Leave();
    return !Temp; //True if there is something to read
}

//***************************************************************************
// Force Parser
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_MpegTs_ForceMenu_Set (bool NewValue)
{
    Enter(true);
    File_MpegTs_ForceMenu=NewValue;
    Leave();
}

bool MediaInfo_Config_MediaInfo::File_MpegTs_ForceMenu_Get ()
{
    Enter();
    bool Temp=File_MpegTs_ForceMenu;
    Leave();
    return Temp;
}

//***************************************************************************
// Enter/Leave
//***************************************************************************

void MediaInfo_Config_MediaInfo::Enter (bool)
{
}

void MediaInfo_Config_MediaInfo::Leave ()
{
}

} //NameSpace

