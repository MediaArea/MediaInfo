// File__Duplicate - Duplication of some formats
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
//
// Duplication helper for some specific formats
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "MediaInfo/File__Duplicate.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#include "ZenLib/ZtringList.h"
#include "ZenLib/File.h"
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

File__Duplicate::File__Duplicate ()
:File__Analyze()
{
    //Temp
    File__Duplicate_HasChanged_=false;
    Config_File_Duplicate_Get_AlwaysNeeded_Count=0;
}

File__Duplicate::~File__Duplicate ()
{
    std::map<const String, File__Duplicate_MpegTs*>::iterator Duplicates_Temp=Duplicates.begin();
    while (Duplicates_Temp!=Duplicates.end())
    {
        delete Duplicates_Temp->second; //Duplicates_Temp->second=NULL
        Duplicates_Temp++;
    }
    Duplicates.clear();
}

//***************************************************************************
// Format
//***************************************************************************

void File__Duplicate::Read_Buffer_Finalize ()
{
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Duplicates_Speed_FromPID.clear();
}

//***************************************************************************
// Set
//***************************************************************************

//---------------------------------------------------------------------------
void File__Duplicate::File__Duplicate_Set (const Ztring &Value)
{
    //Form: "Code;Target"                           <--Generic
    //Form: "program_number" or                     <--clear it
    //Form: "program_number;file" or                <--the exported filename is filename.program_number
    //Form: "program_number;file://filename" or     <--the exported filename is specified by user
    //Form: "program_number;memory" or              <--This will be a MediaInfo memory block
    //Form: "program_number;memory://pointer:size"  <--Memory block is specified by user
    //WARNING: program_number & pointer must be in ***DECIMAL*** format.
    //Example: "451;memory://123456789:1316"
    ZtringList List(Value);
    bool Orders_ToRemove_Global=false;

    //Searching Target
    std::vector<ZtringList::iterator> Targets_ToAdd;
    std::vector<ZtringList::iterator> Targets_ToRemove;
    std::vector<ZtringList::iterator> Orders_ToAdd;
    std::vector<ZtringList::iterator> Orders_ToRemove;
    for (ZtringList::iterator Current=List.begin(); Current<List.end(); Current++)
    {
        //Detecting if we want to remove
        bool ToRemove=false;
        if (Current->find(_T('-'))==0)
        {
            ToRemove=true;
            Current->erase(Current->begin());
        }

        //Managing targets
        if (Current->find(_T("file:"))==0
         || Current->find(_T("memory:"))==0)
            (ToRemove?Targets_ToRemove:Targets_ToAdd).push_back(Current);
        //Backward compatibility
        else if (*Current==_T("0"))
            Orders_ToRemove_Global=true;
        //Managing orders
        else
            (ToRemove?Orders_ToRemove:Orders_ToAdd).push_back(Current);
    }

    //Backward compatibility
    if (Orders_ToRemove_Global) //with "0"
    {
        for (std::vector<ZtringList::iterator>::iterator Order=Orders_ToAdd.begin(); Order<Orders_ToAdd.end(); Order++)
            Orders_ToRemove.push_back(*Order);
        Orders_ToAdd.clear();
    }

    //For each target to add
    for (std::vector<ZtringList::iterator>::iterator Target=Targets_ToAdd.begin(); Target<Targets_ToAdd.end(); Target++)
    {
        //Adding the target if it does not exist yet
        if (Duplicates.find(**Target)==Duplicates.end())
        {
            Duplicates[**Target]=new File__Duplicate_MpegTs(**Target);
            size_t Pos=Config->File__Duplicate_Memory_Indexes_Get(**Target);
            if (Pos!=Error)
            {
                if (Pos>=Duplicates_Speed.size())
                    Duplicates_Speed.resize(Pos+1);
                Duplicates_Speed[Pos]=Duplicates[**Target];
            }
        }

        //For each order to add
        for (std::vector<ZtringList::iterator>::iterator Order=Orders_ToAdd.begin(); Order<Orders_ToAdd.end(); Order++)
            Duplicates[**Target]->Configure(**Order, false);

        //For each order to remove
        for (std::vector<ZtringList::iterator>::iterator Order=Orders_ToRemove.begin(); Order<Orders_ToRemove.end(); Order++)
            Duplicates[**Target]->Configure(**Order, true);
    }

    //For each target to remove
    for (std::vector<ZtringList::iterator>::iterator Target=Targets_ToRemove.begin(); Target<Targets_ToRemove.end(); Target++)
    {
        std::map<const String, File__Duplicate_MpegTs*>::iterator Pointer=Duplicates.find(**Target);
        if (Pointer!=Duplicates.end())
        {
            //Duplicates_Speed
            for (std::vector<File__Duplicate_MpegTs*>::iterator Duplicate=Duplicates_Speed.begin(); Duplicate<Duplicates_Speed.end(); Duplicate++)
                if (*Duplicate==Pointer->second)
                    *Duplicate=NULL;

            //Duplicates_Speed_FromPID
            for (std::vector<std::vector<File__Duplicate_MpegTs*> >::iterator Duplicate_FromPID=Duplicates_Speed_FromPID.begin(); Duplicate_FromPID<Duplicates_Speed_FromPID.end(); Duplicate_FromPID++)
                for (std::vector<File__Duplicate_MpegTs*>::iterator Duplicate=Duplicate_FromPID->begin(); Duplicate<Duplicate_FromPID->end(); Duplicate++)
                    if (*Duplicate==Pointer->second)
                        *Duplicate=NULL;

            //Duplicate
            Duplicates.erase(**Target);
        }
    }

    //Informing the status has changed
    File__Duplicate_HasChanged_=true;
    if (Duplicates_Speed_FromPID.empty())
        Duplicates_Speed_FromPID.resize(0x2000);

    Duplicates_Speed_FromPID[0x00]=Duplicates_Speed;
}

//***************************************************************************
// Get
//***************************************************************************

bool File__Duplicate::File__Duplicate_Get ()
{
    return !Duplicates.empty();
}

bool File__Duplicate::File__Duplicate_Get_From_PID (int16u PID)
{
    if (Duplicates_Speed_FromPID.empty())
        return false;
    return !Duplicates_Speed_FromPID[PID].empty();
}

bool File__Duplicate::File__Duplicate_HasChanged ()
{
    //Retrieving general configuration
    while (Config->File_Duplicate_Get_AlwaysNeeded(Config_File_Duplicate_Get_AlwaysNeeded_Count))
    {
        File__Duplicate_Set(Config->File_Duplicate_Get(Config_File_Duplicate_Get_AlwaysNeeded_Count));
        File__Duplicate_HasChanged_=true;
        Config_File_Duplicate_Get_AlwaysNeeded_Count++;
    }

    bool File__Duplicate_HasChanged_Temp=File__Duplicate_HasChanged_;
    File__Duplicate_HasChanged_=false;
    return File__Duplicate_HasChanged_Temp;
}

//***************************************************************************
// Write
//***************************************************************************

void File__Duplicate::File__Duplicate_Write (int16u PID)
{
    const int8u* ToAdd=Buffer+Buffer_Offset-(size_t)Header_Size;
    size_t ToAdd_Size=(size_t)(Element_Size+Header_Size);

    std::vector<File__Duplicate_MpegTs*> &Dup_FromPID=Duplicates_Speed_FromPID[PID];
    size_t Size=Duplicates_Speed_FromPID[PID].size();
    bool ToUpdate=false;
    for (size_t Pos=0; Pos<Size; Pos++)
        if (Dup_FromPID[Pos] && Dup_FromPID[Pos]->Write(PID, ToAdd, ToAdd_Size))
            ToUpdate=true;
    if (ToUpdate)
    {
        Duplicates_Speed_FromPID.clear();
        Duplicates_Speed_FromPID.resize(0x2000);
        Duplicates_Speed_FromPID[0x0000]=Duplicates_Speed;
        size_t Size=Duplicates_Speed.size();
        for (size_t Pos=0; Pos<Size; Pos++)
        {
            File__Duplicate_MpegTs* Dup=Duplicates_Speed[Pos];
            size_t program_map_PIDs_Size=Duplicates_Speed[Pos]->program_map_PIDs.size();
            for (size_t program_map_PIDs_Pos=0; program_map_PIDs_Pos<program_map_PIDs_Size; program_map_PIDs_Pos++)
                if (Dup->program_map_PIDs[program_map_PIDs_Pos])
                    Duplicates_Speed_FromPID[program_map_PIDs_Pos].push_back(Dup);
            size_t elementary_PIDs_Size=Duplicates_Speed[Pos]->program_map_PIDs.size();
            for (size_t elementary_PIDs_Pos=0; elementary_PIDs_Pos<elementary_PIDs_Size; elementary_PIDs_Pos++)
                if (Dup->elementary_PIDs[elementary_PIDs_Pos])
                    Duplicates_Speed_FromPID[elementary_PIDs_Pos].push_back(Dup);
        }
    }
}

//***************************************************************************
// Output_Buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Duplicate::Output_Buffer_Get (const String &Code_)
{
    std::map<const String, File__Duplicate_MpegTs*>::iterator Stream=Duplicates.find(Code_);
    if (Stream==Duplicates.end())
        return 0;

    return Stream->second->Output_Buffer_Get();
}

//---------------------------------------------------------------------------
size_t File__Duplicate::Output_Buffer_Get (size_t Pos)
{
    if (Pos>=Duplicates_Speed.size() || Duplicates_Speed[Pos]==NULL)
        return 0;

    return Duplicates_Speed[Pos]->Output_Buffer_Get();
}

} //NameSpace

