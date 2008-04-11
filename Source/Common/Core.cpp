// Main - Main functions
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
#include <ZenLib/Ztring.h>
#include <ZenLib/File.h>
#include <sstream>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const MediaInfoLib::Char* MEDIAINFO_TITLE=_T("MediaInfo - http://mediainfo.sourceforge.net");
const size_t Title_Pos=66; //TODO: Position of Title in General.csv, should shange this...
ZenLib::Ztring Text_Temp;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Core::Core()
{
    MI=new MediaInfoLib::MediaInfoList;

    Details=0;
}

Core::~Core()
{
    delete MI;
}

//---------------------------------------------------------------------------
void Core::Data_Prepare()
{
    //Inform
    Text=MI->Inform((size_t)-1).c_str();
}

//***************************************************************************
// Menu
//***************************************************************************

//---------------------------------------------------------------------------
void Core::Menu_File_Open_File (const String& FileName)
{
    Menu_File_Open_Files_Begin();
    Menu_File_Open_Files_Continue(FileName);
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Files_Begin ()
{
    MI->Close();
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Files_Continue (const String &FileName)
{
    //MI->Option(_T("File_Filter"), _T("452"));
    //MI->Option(_T("File_Duplicate"), _T("452;file")); //Activate it
    //MI->Open(FileName);
    MI->Open(FileName);
    /*#ifndef WIN32
    for (size_t Pos=0; Pos<128; Pos++)
        MI->Open(FileName);
    #endif */
    return;

    //Option "File_Filter": filter a part of the file
    //if no filter --> All the file is parser
    //if one or more filter --> Only the filtered streams are parsed
    //Form: "program_number"

    //Option "File_Duplicate": duplicate in the same format a part of a file
    //Form: "program_number" or                     <--clear it
    //Form: "program_number;file://filename" or     <--The exported filename is specified by user
    //Form: "program_number;memory://pointer:size"  <--Memory block is specified by user
    //WARNING: program_number & pointer must be in ***DECIMAL*** format.
    //Example: "451;memory://123456789:1316"
    //You can add as many program_number as you want

    //How to get the buffer if this is a memory handler:
    //MediaInfo::Output_Buffer_Get(Value);
    //Form: "memory://pointer:size"                  <--The desired memory part you want
    //Return the count of written bytes

    //EXAMPLE
    //-------

    /*
    //Initilaizing MediaInfo
    MediaInfo MI;

    //From: preparing an example file for reading
    ZenLib::File From; From.Open(FileName, ZenLib::File::Access_Read); //You can use something else than a file

    //From: preparing a memory buffer for reading
    ZenLib::int8u* From_Buffer=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    size_t From_Buffer_Size; //The size of the read file buffer

    //To (Output 1): preparing an example file for writing
    ZenLib::File To_1;
    To_1.Open(FileName+_T(".Extract1"), ZenLib::File::Access_Write_Append);

    //To (Output 1): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_1=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_1_Name_Temp;
    To_Buffer_1_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_1<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_1_Name=To_Buffer_1_Name_Temp.str();

    //To (Output 2): preparing an example file for writing
    ZenLib::File To_2;
    To_2.Open(FileName+_T(".Extract2"), ZenLib::File::Access_Write_Append);

    //To (Output 2): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_2=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_2_Name_Temp;
    To_Buffer_2_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_2<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_2_Name=To_Buffer_2_Name_Temp.str();

    //To (Output 3): preparing an example file for writing
    ZenLib::File To_3;
    To_3.Open(FileName+_T(".Extract3"), ZenLib::File::Access_Write_Append);

    //To (Output 3): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_3=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_3_Name_Temp;
    To_Buffer_3_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_3<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_3_Name=To_Buffer_3_Name_Temp.str();

    //To (Output 4): preparing an example file for writing
    ZenLib::File To_4;
    To_4.Open(FileName+_T(".Extract4"), ZenLib::File::Access_Write_Append);

    //To (Output 4): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_4=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_4_Name_Temp;
    To_Buffer_4_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_4<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_4_Name=To_Buffer_4_Name_Temp.str();

    //To (Output 5): preparing an example file for writing
    ZenLib::File To_5;
    To_5.Open(FileName+_T(".Extract5"), ZenLib::File::Access_Write_Append);

    //To (Output 5): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_5=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_5_Name_Temp;
    To_Buffer_5_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_5<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_5_Name=To_Buffer_5_Name_Temp.str();

    //To (Output 6): preparing an example file for writing
    ZenLib::File To_6;
    To_6.Open(FileName+_T(".Extract6"), ZenLib::File::Access_Write_Append);

    //To (Output 6): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_6=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_6_Name_Temp;
    To_Buffer_6_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_6<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_6_Name=To_Buffer_6_Name_Temp.str();

    //To (Output 7): preparing an example file for writing
    ZenLib::File To_7;
    To_7.Open(FileName+_T(".Extract7"), ZenLib::File::Access_Write_Append);

    //To (Output 7): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_7=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_7_Name_Temp;
    To_Buffer_7_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_7<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_7_Name=To_Buffer_7_Name_Temp.str();

    //To (Output 8): preparing an example file for writing
    ZenLib::File To_8;
    To_8.Open(FileName+_T(".Extract8"), ZenLib::File::Access_Write_Append);

    //To (Output 8): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_8=new ZenLib::int8u[7*188]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_8_Name_Temp;
    To_Buffer_8_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_8<<_T(":")<<7*188; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_8_Name=To_Buffer_8_Name_Temp.str();

    //Preparing the Program numbers we want
    MediaInfoLib::String ProgramNumber1=_T("451");
    MediaInfoLib::String ProgramNumber2=_T("452");
    MediaInfoLib::String ProgramNumber3=_T("453");
    MediaInfoLib::String ProgramNumber4=_T("454");
    MediaInfoLib::String ProgramNumber5=_T("455");
    MediaInfoLib::String ProgramNumber6=_T("456");
    MediaInfoLib::String ProgramNumber7=_T("457");
    MediaInfoLib::String ProgramNumber8=_T("459");
    //MediaInfoLib::String ProgramNumber1=_T("1");
    //MediaInfoLib::String ProgramNumber2=_T("2");
    //MediaInfoLib::String ProgramNumber3=_T("1");

    //Optional (aminly for speed improvement): filtering
    //MI.Option(_T("File_Filter"), ProgramNumber1);
    //MI.Option(_T("File_Filter"), ProgramNumber2);
    //MI.Option(_T("File_Filter"), ProgramNumber3);

    //Registering for duplication
    MI.Option(_T("File_ForceParser"), _T("MpegTs"));
    MI.Option(_T("File_Duplicate"), To_Buffer_1_Name+_T(";program_number=")+ProgramNumber1); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_2_Name+_T(";program_number=")+ProgramNumber2); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_3_Name+_T(";program_number=")+ProgramNumber3); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_4_Name+_T(";program_number=")+ProgramNumber4); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_5_Name+_T(";program_number=")+ProgramNumber5); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_6_Name+_T(";program_number=")+ProgramNumber6); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_7_Name+_T(";program_number=")+ProgramNumber7); //"memory://pointer:size;program_number=..."
    MI.Option(_T("File_Duplicate"), To_Buffer_8_Name+_T(";program_number=")+ProgramNumber8); //"memory://pointer:size;program_number=..."

    //Preparing to fill MediaInfo with a buffer
    MI.Open_Buffer_Init();
    bool CanWrite_OnlyIfParsingIsOk=false;
    MI.Option(_T("File_IsSeekable"), _T("0"));

    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size=From.Read(From_Buffer, 7*188);

        //Sending the buffer to MediaInfo
        if (MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size)==0 && !CanWrite_OnlyIfParsingIsOk)
        {
            CanWrite_OnlyIfParsingIsOk=true;
        }

        //Testing if MediaInfo request to go elsewhere
        //if (MI.Open_Buffer_Continue_GoTo_Get()!=(ZenLib::int64u)-1)
        //{
        //    From_Buffer_Size=0; //Get out of the loop, no more data is needed by the parser
        //    MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work
        //    Text=MI.Inform(); //Inform is ready!
        //}

        if (CanWrite_OnlyIfParsingIsOk)
        {
            //Retrieving data written in memory
            size_t To_Buffer_Size_1=MI.Output_Buffer_Get(0);
            size_t To_Buffer_Size_2=MI.Output_Buffer_Get(1);
            size_t To_Buffer_Size_3=MI.Output_Buffer_Get(2);
            size_t To_Buffer_Size_4=MI.Output_Buffer_Get(3);
            size_t To_Buffer_Size_5=MI.Output_Buffer_Get(4);
            size_t To_Buffer_Size_6=MI.Output_Buffer_Get(5);
            size_t To_Buffer_Size_7=MI.Output_Buffer_Get(6);
            size_t To_Buffer_Size_8=MI.Output_Buffer_Get(7);

            //Writing data to somewhere, do what you want for this.
            To_1.Write(To_Buffer_1, To_Buffer_Size_1);
            To_2.Write(To_Buffer_2, To_Buffer_Size_2);
            To_3.Write(To_Buffer_3, To_Buffer_Size_3);
            To_4.Write(To_Buffer_4, To_Buffer_Size_4);
            To_5.Write(To_Buffer_5, To_Buffer_Size_5);
            To_6.Write(To_Buffer_6, To_Buffer_Size_6);
            To_7.Write(To_Buffer_7, To_Buffer_Size_7);
            To_8.Write(To_Buffer_8, To_Buffer_Size_8);
        }

        //Optional at 30 MB, we decide to dynamicly change the ProgramNumber of the first output
        static ZenLib::int64u Size_Parsed=0;
        if (Size_Parsed!=(ZenLib::int64u)-1)
        {
            Size_Parsed+=From_Buffer_Size;
            if (Size_Parsed>30*1024*1024)
            {
                //Stop duplicating the first ProgramNumber
                //MI.Option(_T("File_Duplicate"), To_Buffer_1_Name+_T(";program_number=")+ProgramNumber1+_T(";0")); //"program_number=..."

                //Registering for duplication
                //MI.Option(_T("File_Duplicate"), To_Buffer_1_Name+_T(";program_number=")+ProgramNumber3); //"memory://pointer:size;program_number=..."
                Size_Parsed=(ZenLib::int64u)-1; //Disabling this for not doing this twice
            }
        }

    }
    while (From_Buffer_Size>0);
    //Text=MI.Inform();

    //Clean up
    delete[] From_Buffer;
    delete[] To_Buffer_1;
    delete[] To_Buffer_2;
    */
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Directory (const String &DirectoryName)
{
    MI->Open(DirectoryName, FileOption_Recursive);
}

//---------------------------------------------------------------------------
void Core::Menu_View_Easy ()
{
    Text=_T("Easy");
    Kind=Kind_Easy;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Sheet ()
{
    Text=_T("Sheet");
    Kind=Kind_Sheet;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Tree ()
{
    Text=_T("Root\n Item");
    Kind=Kind_Tree;
    if (Details>0)
        Menu_Debug_Details(Details);
}

//---------------------------------------------------------------------------
void Core::Menu_View_HTML ()
{
    MI->Option(_T("Inform"), _T("HTML"));
    Kind=Kind_HTML;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Text ()
{
    MI->Option(_T("Inform"), _T(""));
    Kind=Kind_Text;
    if (Details>0)
        Menu_Debug_Details(Details);
}

//---------------------------------------------------------------------------
void Core::Menu_Option_Preferences_Inform (const String& Inform)
{
    MI->Option(_T("Inform"), Inform);
}

//---------------------------------------------------------------------------
void Core::Menu_Option_Preferences_Option (const String& Param, const String& Value)
{
    MI->Option(Param, Value);
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Complete (bool Value)
{
    if (Value)
        MI->Option(_T("Complete"), _T("1"));
    else
        MI->Option(_T("Complete"), _T("0"));
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Details (float Value)
{
    Details=Value;

    StringStream ToSend;
    ToSend<<_T("Details;");
    ToSend<<Value;

    if (Value==0)
        MI->Option(_T("Inform"), String(_T("Details;0")));
    else
        MI->Option(_T("Inform"), String(_T("Details;0.9")));
}

//---------------------------------------------------------------------------
void Core::Menu_Debug_Demux (const String &Value)
{
    if (Demux==Value)
        return;
    Demux=Value;

    MI->Option(_T("Demux"), Value);
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Version ()
{
    Text=MI->Option(_T("Info_Version"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Formats ()
{
    Text=MI->Option(_T("Info_Capacities"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Codecs ()
{
    Text=MI->Option(_T("Info_Codecs"));
}

//---------------------------------------------------------------------------
void Core::Menu_Help_Info_Parameters ()
{
    Text=MI->Option(_T("Info_Parameters"));
}

//---------------------------------------------------------------------------
void Core::Menu_Language (const String& Language)
{
    MI->Option(_T("Language"), Language);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
String& Core::Inform_Get ()
{
    if (Text_Temp.empty())
        Text=MI->Inform((size_t)-1).c_str();
    else
        Text=Text_Temp;
    return Text;
}

//---------------------------------------------------------------------------
Core::kind Core::Kind_Get ()
{
    return Kind;
}

//---------------------------------------------------------------------------
String Core::Inform_Get(size_t Pos, stream_t StreamKind, size_t StreamPos)
{
    MI->Option(_T("Inform"), _T(""));
    return MI->Get(Pos, StreamKind, StreamPos, _T("Inform")).c_str();
}


