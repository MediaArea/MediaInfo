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
#include "MediaInfo/Setup.h"
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
        return File_Duplicate_Set(Value);
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
    CriticalSectionLocker CSL(CS);
    FileIsSeekable=NewValue;
}

bool MediaInfo_Config_MediaInfo::File_IsSeekable_Get ()
{
    CriticalSectionLocker CSL(CS);
    return FileIsSeekable;
}

//***************************************************************************
// Force Parser
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_ForceParser_Set (const Ztring &NewValue)
{
    CriticalSectionLocker CSL(CS);
    File_ForceParser=NewValue;
}

Ztring MediaInfo_Config_MediaInfo::File_ForceParser_Get ()
{
    CriticalSectionLocker CSL(CS);
    return File_ForceParser;
}

//***************************************************************************
// Is a sub
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_IsSub_Set (bool NewValue)
{
    CriticalSectionLocker CSL(CS);
    File_IsSub=NewValue;
}

bool MediaInfo_Config_MediaInfo::File_IsSub_Get ()
{
    CriticalSectionLocker CSL(CS);
    return File_IsSub;
}

//***************************************************************************
// Filter
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_Filter_Set (int64u NewValue)
{
    CriticalSectionLocker CSL(CS);
    File_Filter_16[(int16u)NewValue]=true;
    File_Filter_HasChanged_=true;
}

bool MediaInfo_Config_MediaInfo::File_Filter_Get (const int16u Value)
{
    CriticalSectionLocker CSL(CS);
    //Test
    bool Exists;
    if (File_Filter_16.empty())
        Exists=true;
    else
        Exists=(File_Filter_16.find(Value)!=File_Filter_16.end());
    return Exists;
}

bool MediaInfo_Config_MediaInfo::File_Filter_Get ()
{
    CriticalSectionLocker CSL(CS);
    bool Exist=!File_Filter_16.empty();
    return Exist;
}

bool MediaInfo_Config_MediaInfo::File_Filter_HasChanged ()
{
    CriticalSectionLocker CSL(CS);
    bool File_Filter_HasChanged_Temp=File_Filter_HasChanged_;
    File_Filter_HasChanged_=false;
    return File_Filter_HasChanged_Temp;
}

//***************************************************************************
// Duplicate
//***************************************************************************

Ztring MediaInfo_Config_MediaInfo::File_Duplicate_Set (const Ztring &Value)
{
    //Preparing for File__Duplicate...
    CS.Enter();
    File__Duplicate_List.push_back(Value);

    //Handling Memory index
    Ztring ToReturn;
    ZtringList List=Value;
    for (size_t Pos=0; Pos<List.size(); Pos++)
    {
        //Form= "(-)Data", if "-" the value will be removed
        Ztring &Value=List[Pos];
        bool ToRemove=false;
        if (Value.find(_T('-'))==0)
        {
            Value.erase(Value.begin());
            ToRemove=true;
        }

        //Testing if this is information about a target
        if (List[Pos].find(_T("memory:"))==0 || List[Pos].find(_T("file:"))==0)
        {
            //Searching if already exist
            size_t Memory_Pos=File__Duplicate_Memory_Indexes.Find(List[Pos]);
            if (!ToRemove && Memory_Pos==Error)
            {
                //Does not exist yet (and adding is wanted)
                Memory_Pos=File__Duplicate_Memory_Indexes.Find(_T(""));
                if (Memory_Pos!=Error)
                    File__Duplicate_Memory_Indexes[Memory_Pos]=List[Pos]; //A free place is found
                else
                {
                    //Adding the place at the end
                    Memory_Pos=File__Duplicate_Memory_Indexes.size();
                    File__Duplicate_Memory_Indexes.push_back(List[Pos]);
                }
            }
            else if (ToRemove)
            {
                //Exists yet but Removal is wanted
                File__Duplicate_Memory_Indexes[Memory_Pos].clear();
                Memory_Pos=(size_t)-1;
            }

            ToReturn+=_T(";")+Ztring().From_Number(Memory_Pos);
        }
    }
    if (!ToReturn.empty())
        ToReturn.erase(ToReturn.begin()); //Remove first ";"

    CS.Leave();
    File_IsSeekable_Set(false); //If duplication, we can not seek anymore

    return ToReturn;
}

Ztring MediaInfo_Config_MediaInfo::File_Duplicate_Get (size_t AlreadyRead_Pos)
{
    CriticalSectionLocker CSL(CS);
    if (AlreadyRead_Pos>=File__Duplicate_List.size())
        return Ztring(); //Nothing or not more than the last time
    Ztring Temp=File__Duplicate_List[AlreadyRead_Pos];
    return Temp;
}

bool MediaInfo_Config_MediaInfo::File_Duplicate_Get_AlwaysNeeded (size_t AlreadyRead_Pos)
{
    CriticalSectionLocker CSL(CS);
    bool Temp=AlreadyRead_Pos>=File__Duplicate_List.size();
    return !Temp; //True if there is something to read
}

size_t MediaInfo_Config_MediaInfo::File__Duplicate_Memory_Indexes_Get (const Ztring &Value)
{
    CriticalSectionLocker CSL(CS);
    return File__Duplicate_Memory_Indexes.Find(Value);
}

void MediaInfo_Config_MediaInfo::File__Duplicate_Memory_Indexes_Erase (const Ztring &Value)
{
    CriticalSectionLocker CSL(CS);
    size_t Pos=File__Duplicate_Memory_Indexes.Find(Value);
    if (Pos!=Error)
        File__Duplicate_Memory_Indexes[Pos].clear();
}

//***************************************************************************
// Force Parser
//***************************************************************************

//---------------------------------------------------------------------------
void MediaInfo_Config_MediaInfo::File_MpegTs_ForceMenu_Set (bool NewValue)
{
    CriticalSectionLocker CSL(CS);
    File_MpegTs_ForceMenu=NewValue;
}

bool MediaInfo_Config_MediaInfo::File_MpegTs_ForceMenu_Get ()
{
    CriticalSectionLocker CSL(CS);
    bool Temp=File_MpegTs_ForceMenu;
    return Temp;
}

} //NameSpace

