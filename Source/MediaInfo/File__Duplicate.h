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

//---------------------------------------------------------------------------
#ifndef File__DuplicateH
#define File__DuplicateH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <ZenLib/ZtringListList.h>
#include <map>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__Duplicate
//***************************************************************************

class File__Duplicate : public File__Analyze
{
public :
    //Constructor/Destructor
    File__Duplicate();
    virtual ~File__Duplicate() {};

protected :
    //Formats
    virtual void Read_Buffer_Finalize ();

    //Set
    void   File__Duplicate_Set  (const Ztring &Value); //Fill a new File__Duplicate value

    //Get
    bool   File__Duplicate_Get  ();
    bool   File__Duplicate_Get  (int16u  Value);
    Ztring File__Duplicate_Codes_Get (); //Get the list of registerd codes

    //Modifications
    bool   File__Duplicate_HasChanged();

    //Write
    void   File__Duplicate_Write (const Ztring &Target, const int8u* ToAdd=NULL, size_t ToAdd_Size=0);
    void   File__Duplicate_Write (int16u        Code,   const int8u* ToAdd=NULL, size_t ToAdd_Size=0);

    //Output buffer
    size_t Output_Buffer_Get (const Ztring &Value, unsigned char** Output_Buffer=NULL);

private :
    struct file_duplicate
    {
        enum method
        {
            method_none,
            method_buffer,
            method_buffer_nodelete,
            method_filename,
            method_filename_ID,         //FileName is the file_name.id (id is the program number)
            method_filename_Extract,    //FileName is the file_name.Extract
        };
        method  Method;
        int8u*  Buffer;
        size_t  Buffer_Size;
        size_t  Buffer_Size_Max;
        Ztring  File_Name;
        void*   File; //ZenLib::File*
        size_t  Registered_Count;

        file_duplicate()
        {
            Method=method_none;
            Buffer=NULL;
            Buffer_Size=0;
            Buffer_Size_Max=0;
            File=NULL;
            Registered_Count=0; //Count of registerd streams
        }
    };
    std::map<Ztring, file_duplicate >   File__Duplicate_Targets; //List of all registered streams
    std::map<int16u, file_duplicate*>   File__Duplicate_Targets_From_Codes_16;
    bool                                File__Duplicate_HasChanged_;
    size_t                              Config_File_Duplicate_Get_AlwaysNeeded_Count;

    //Write
    void   File__Duplicate_Write (file_duplicate* Target, const int8u* ToAdd, size_t ToAdd_Size);
};


} //NameSpace

#endif
