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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "MediaInfo/File__Duplicate.h"
#include "MediaInfo/MediaInfo_Config.h"
#include <ZenLib/ZtringList.h>
#include <ZenLib/File.h>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

File__Duplicate::File__Duplicate ()
: File__Analyze()
{
    //Temp
    File__Duplicate_HasChanged_=false;
    Config_File_Duplicate_Get_AlwaysNeeded_Count=0;
}

//***************************************************************************
// Format
//***************************************************************************

void File__Duplicate::Read_Buffer_Finalize ()
{
    std::map<Ztring, file_duplicate >::iterator File__Duplicate_Target_Temp=File__Duplicate_Targets.begin();
    while (File__Duplicate_Target_Temp!=File__Duplicate_Targets.end())
    {
        delete (File*)File__Duplicate_Target_Temp->second.File; //(File*)File__Duplicate_Target_Temp->second.File=NULL
        File__Duplicate_Target_Temp++;
    }
    File__Duplicate_Targets.clear();

    File__Duplicate_Targets_From_Codes_16.clear();
}

//***************************************************************************
// Set
//***************************************************************************

//---------------------------------------------------------------------------
void File__Duplicate::File__Duplicate_Set (const Ztring &Value)
{
    //Form: "program_number" or                     <--clear it
    //Form: "program_number;file" or                <--the exported filename is filename.program_number
    //Form: "program_number;file://filename" or     <--the exported filename is specified by user
    //Form: "program_number;memory" or              <--This will be a MediaInfo memory block
    //Form: "program_number;memory://pointer:size"  <--Memory block is specified by user
    //WARNING: program_number & pointer must be in ***DECIMAL*** format.
    //Example: "451;memory://123456789:1316"
    ZtringList List(Value);
    Ztring &Target=List(1);
    Ztring &Code=List(0);

    //Filling File__Duplicate_Targets (if not alreay done)
    if (File__Duplicate_Targets.find(Target)==File__Duplicate_Targets.end())
    {
        //Form: "program_number;memory"
        //Form: "program_number;memory://pointer:size"
        if (Target.find(_T("memory"))==0)
        {
             //Form: "program_number;memory://pointer:size"
             if (Target.size()>9 && Target.find(_T("memory://"))==0 && Target.find(_T(":"), 9)!=string::npos)
            {
                size_t SemiColumn_Pos=Target.find(_T(":"), 9);
                Ztring Address=Target.substr(9, SemiColumn_Pos-9);
                Ztring Size=Target.substr(SemiColumn_Pos+1);
                File__Duplicate_Targets[Target].Method=file_duplicate::method_buffer_nodelete;
                File__Duplicate_Targets[Target].Buffer=(int8u*)Address.To_int64u();
                File__Duplicate_Targets[Target].Buffer_Size_Max=(size_t)Size.To_int64u();
            }
            //Form: "program_number;memory"
            else
            {
                Target+=Code;
                File__Duplicate_Targets[Target].Method=file_duplicate::method_buffer;
            }
        }

        //Form: "program_number;file"
        //Form: "program_number;file://filename"
        else if (Target.find(_T("file"))==0)
        {
            //Form: "program_number;file://filename"
            if (Target.size()>7 && Target.find(_T("file://"))==0)
            {
                File__Duplicate_Targets[Target].Method=file_duplicate::method_filename;
                File__Duplicate_Targets[Target].File_Name=Target.substr(7, string::npos);
            }
            //Form: "program_number;file_extract"
            else if (Target.find(_T("file_extract"))==0)
            {
                Target+=Code;
                File__Duplicate_Targets[Target].Method=file_duplicate::method_filename_Extract;
            }
            //Form: "program_number;file"
            else if (Target.size()==4)
            {
                Target+=Code;
                File__Duplicate_Targets[Target].Method=file_duplicate::method_filename_ID;
            }
        }
    }

    //Removing code reference (if needed)
    int16u Code_16=Code.To_int16u();
    if (File__Duplicate_Targets_From_Codes_16.find(Code_16)!=File__Duplicate_Targets_From_Codes_16.end())
    {   //Code already exist, removing this reference
        //Retrieving the original target
        std::map<Ztring, file_duplicate >::iterator File__Duplicate_Target_Temp=File__Duplicate_Targets.begin();
        while (&File__Duplicate_Target_Temp->second!=File__Duplicate_Targets_From_Codes_16[Code_16])
            File__Duplicate_Target_Temp++;

        //Counting how much it is used, removing it if no more useful
        if (File__Duplicate_Target_Temp->second.Registered_Count>0)
            File__Duplicate_Target_Temp->second.Registered_Count--;
        if (File__Duplicate_Target_Temp->second.Registered_Count==0)
        {
            delete (File*)File__Duplicate_Target_Temp->second.File;
            File__Duplicate_Targets.erase(File__Duplicate_Target_Temp);
        }

        //Code is no more used
        File__Duplicate_Targets_From_Codes_16.erase(Code_16);
    }

    //Adding code reference
    if (File__Duplicate_Targets.find(Target)!=File__Duplicate_Targets.end()) //Only if there is something understable
    {
        //Target was understable
        File__Duplicate_Targets_From_Codes_16[Code_16]=&File__Duplicate_Targets[Target];
        File__Duplicate_Targets_From_Codes_16[Code_16]->Registered_Count++;
    }

    //Informing the status has changed
    File__Duplicate_HasChanged_=true;
}

//***************************************************************************
// Get
//***************************************************************************

bool File__Duplicate::File__Duplicate_Get ()
{
    return !File__Duplicate_Targets_From_Codes_16.empty();
}

bool File__Duplicate::File__Duplicate_Get (int16u Value)
{
    return File__Duplicate_Targets_From_Codes_16.find(Value)!=File__Duplicate_Targets_From_Codes_16.end();
}

Ztring File__Duplicate::File__Duplicate_Codes_Get ()
{
    ZtringListList Codes;

    std::map<Ztring, file_duplicate >::iterator File__Duplicate_Target_Temp=File__Duplicate_Targets.begin();
    while (File__Duplicate_Target_Temp!=File__Duplicate_Targets.end())
    {
        ZtringList Temp;
        Temp.push_back(File__Duplicate_Target_Temp->first);
        std::map<int16u, file_duplicate*>::iterator Code_Temp=File__Duplicate_Targets_From_Codes_16.begin();
        while(Code_Temp!=File__Duplicate_Targets_From_Codes_16.end())
        {
            if(Code_Temp->second==&File__Duplicate_Target_Temp->second)
                Temp.push_back(Ztring::ToZtring(Code_Temp->first));
            Code_Temp++;
        }

        Codes.push_back(Temp);
        File__Duplicate_Target_Temp++;
    }

    return Codes.Read();
}

bool File__Duplicate::File__Duplicate_HasChanged ()
{
    //Retrieving general configuration
    Ztring Temp;
    while (Config.File_Duplicate_Get_AlwaysNeeded(Config_File_Duplicate_Get_AlwaysNeeded_Count))
    {
        File__Duplicate_Set(Config.File_Duplicate_Get(Config_File_Duplicate_Get_AlwaysNeeded_Count));
        File__Duplicate_HasChanged_=true;
        Config_File_Duplicate_Get_AlwaysNeeded_Count++;
    }

    //Configuring output filenames if needed
    std::map<Ztring, file_duplicate >::iterator File__Duplicate_Target_Temp=File__Duplicate_Targets.begin();
    while (File__Duplicate_Target_Temp!=File__Duplicate_Targets.end())
    {
        switch (File__Duplicate_Target_Temp->second.Method)
        {
            case file_duplicate::method_filename_ID :
                    {
                        Ztring List=File__Duplicate_Target_Temp->first;
                        Ztring Code=List.substr(4);
                        File__Duplicate_Target_Temp->second.File_Name=File_Name+_T('.')+Code;
                        File__Duplicate_Target_Temp->second.Method=file_duplicate::method_filename;
                    }
                    break;
            case file_duplicate::method_filename_Extract :
                    {
                        Ztring Code;
                        File__Duplicate_Target_Temp->second.File_Name=File_Name+_T(".Extract");
                        File__Duplicate_Target_Temp->second.Method=file_duplicate::method_filename;
                    }
                    break;
            default: ;
        }
        File__Duplicate_Target_Temp++;
    }

    bool File__Duplicate_HasChanged_Temp=File__Duplicate_HasChanged_;
    File__Duplicate_HasChanged_=false;
    return File__Duplicate_HasChanged_Temp;
}

//***************************************************************************
// Write
//***************************************************************************

void File__Duplicate::File__Duplicate_Write (file_duplicate* Target, const int8u* ToAdd, size_t ToAdd_Size)
{
    //Default values
    if (ToAdd==NULL)
    {
        ToAdd=Buffer+Buffer_Offset-(size_t)Header_Size;
        ToAdd_Size=(size_t)(Element_Size+Header_Size);
    }

    //Integrity
    if (Target==NULL || ToAdd_Size==0)
        return;

    //Writing
    switch (Target->Method)
    {
        //File based
        case file_duplicate::method_filename :
            //Open the file if not yet done
            if (Target->File==NULL)
            {
                Target->File=new File;
                ((File*)Target->File)->Open(Target->File_Name, File::Access_Write_Append);
            }
            //Write to the file
            ((File*)Target->File)->Write(ToAdd, ToAdd_Size);
            break;

        //Memory based
        case file_duplicate::method_buffer :
        case file_duplicate::method_buffer_nodelete :
            //Allocating Buffer
            if (Target->Buffer_Size+ToAdd_Size>Target->Buffer_Size_Max)
            {
                if (Target->Method==file_duplicate::method_buffer_nodelete)
                    return; //The buffer is not from us, and buffer underrun

                int8u* Old=Target->Buffer;
                size_t Output_Buffer_Size_Max_ToAdd=ToAdd_Size>32768?ToAdd_Size:32768;
                if (Output_Buffer_Size_Max_ToAdd<Target->Buffer_Size_Max) Output_Buffer_Size_Max_ToAdd=Target->Buffer_Size_Max;
                Target->Buffer_Size_Max+=Output_Buffer_Size_Max_ToAdd;
                Target->Buffer=new int8u[Target->Buffer_Size_Max];
                std::memcpy(Target->Buffer, Old, Target->Buffer_Size);
                delete[] Old; //Old=NULL;
            }

            //Copying buffer
            std::memcpy(Target->Buffer+Target->Buffer_Size, ToAdd, ToAdd_Size);
            Target->Buffer_Size+=ToAdd_Size;
            break;
        default : ;
    }
}

void File__Duplicate::File__Duplicate_Write (const Ztring &Target, const int8u* ToAdd, size_t ToAdd_Size)
{
    if (File__Duplicate_Targets.find(Target)!=File__Duplicate_Targets.end())
        File__Duplicate_Write(&File__Duplicate_Targets[Target], ToAdd, ToAdd_Size);
}

void File__Duplicate::File__Duplicate_Write (int16u  Code, const int8u* ToAdd, size_t ToAdd_Size)
{
    if (File__Duplicate_Targets_From_Codes_16.find(Code)!=File__Duplicate_Targets_From_Codes_16.end())
        File__Duplicate_Write(File__Duplicate_Targets_From_Codes_16[Code], ToAdd, ToAdd_Size);
}

//***************************************************************************
// Output_Buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Duplicate::Output_Buffer_Get (const Ztring &Code_, unsigned char** Output_Buffer)
{
    file_duplicate* Target;

    //First searching for a target
    if (File__Duplicate_Targets.find(Code_)==File__Duplicate_Targets.end())
    {
        //There is not target, searching for a code
        int16u Code=Code_.To_int16u();
        if (File__Duplicate_Targets_From_Codes_16.find(Code)==File__Duplicate_Targets_From_Codes_16.end())
        {
            if (Output_Buffer)
                *Output_Buffer=NULL;
            return 0; //Problem, this stream is not registered (in target or code)
        }

        //This is from a code
        Target=File__Duplicate_Targets_From_Codes_16[Code];
    }
    else
    {
        //This is from a code
        Target=&File__Duplicate_Targets[Code_];
    }

    size_t Output_Buffer_Size=Target->Buffer_Size;
    Target->Buffer_Size=0; //Can be erased later...
    if (Output_Buffer)
        *Output_Buffer=Target->Buffer;
    return Output_Buffer_Size;
}

} //NameSpace

