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
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <sstream>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const MediaInfoLib::Char* MEDIAINFO_TITLE=_T("MediaInfo - http://mediainfo.sourceforge.net");
ZenLib::Ztring Text_Temp;
using namespace ZenLib;
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
size_t Core::Menu_File_Open_File (const String& FileName)
{
    Menu_File_Open_Files_Begin();
    return Menu_File_Open_Files_Continue(FileName);
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Files_Begin ()
{
    MI->Close();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Continue (const String &FileName)
{
    //MI->Option(_T("File_Filter"), _T("452"));
    //MI->Option(_T("File_Duplicate"), _T("452;file")); //Activate it
    //MI->Open(FileName);
    return MI->Open(FileName);
    /*#ifndef WIN32
    for (size_t Pos=0; Pos<128; Pos++)
        MI->Open(FileName);
    #endif */
    //return;

    //Initializing MediaInfo
    MediaInfo MI;

    //From: preparing an example file for reading
    ZenLib::File From; From.Open(FileName, ZenLib::File::Access_Read); //You can use something else than a file
    From.GoTo(From.Size_Get()/2);

    //From: preparing a memory buffer for reading
    ZenLib::int8u* From_Buffer=new ZenLib::int8u[32768*4]; //Note: you can do your own buffer
    size_t From_Buffer_Size; //The size of the read file buffer

    //To (Output 1): preparing a memory buffer for writing
    ZenLib::int8u* To_Buffer_1=new ZenLib::int8u[1024*1024]; //Note: you can do your own buffer
    std::basic_stringstream<ZenLib::Char> To_Buffer_1_Name_Temp;
    To_Buffer_1_Name_Temp<<_T("memory://")<<(size_t)To_Buffer_1<<_T(":")<<1024*1024; //"memory://pointer:size"
    MediaInfoLib::String To_Buffer_1_Name=To_Buffer_1_Name_Temp.str();

    //Registering for duplication
    MI.Option(_T("File_Extract"), To_Buffer_1_Name);

    //Preparing to fill MediaInfo with a buffer
    bool CanWrite_OnlyIfParsingIsOk=false;
    ZenLib::int64u Size_Parsed=0;
    MI.Open_Buffer_Init();


    int32u F_Previous=51;
    int32u F_LastFrame=0;
    int32u F_Buffer_Size=0;
    int32u F_FrameCount=0;

    //FLV
    int8u Header[9];
    Header[0x00]='F';
    Header[0x01]='L';
    Header[0x02]='V';
    Header[0x03]=0x01;
    Header[0x04]=0x01;
    Header[0x05]=0x00;
    Header[0x06]=0x00;
    Header[0x07]=0x00;
    Header[0x08]=0x09;
    F_Previous=0;



    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size=From.Read(From_Buffer, 32768*4);

        //Sending the buffer to MediaInfo
        if (MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size)==0 && !CanWrite_OnlyIfParsingIsOk)
        {
            int C=MI.Count_Get(Stream_Video);
            CanWrite_OnlyIfParsingIsOk=true;
        }

        //if (CanWrite_OnlyIfParsingIsOk)
        {
            //Retrieving data written in memory
            size_t To_Buffer_Size_1=MI.Output_Buffer_Get(To_Buffer_1_Name);

            //Writing data to somewhere, do what you want for this.
            size_t Pos=0;
            while (Pos<To_Buffer_Size_1)
            {
                int32u Size=(int32u)BigEndian2int64u(To_Buffer_1+Pos+16);
                int64u Type=BigEndian2int8u(To_Buffer_1+Pos+24);

                if (Type==1)
                {
                    //TEMP
                    int8u Header[0x14];
                    Header[0x00]=int8u(F_Previous>>24);
                    Header[0x01]=int8u(F_Previous>>16);
                    Header[0x02]=int8u(F_Previous>> 8);
                    Header[0x03]=int8u(F_Previous>> 0);
            
                    Header[0x04]=0x09; //Type
                    Header[0x05]=int8u((5+Size)>>16); //BodyLength
                    Header[0x06]=int8u((5+Size)>> 8);
                    Header[0x07]=int8u((5+Size)>> 0);
                    Header[0x08]=0; //Timestamp_Base
                    Header[0x09]=0;
                    Header[0x0A]=0;
                    Header[0x0B]=0; //Timestamp_Extended
                    Header[0x0C]=0; //StreamID
                    Header[0x0D]=0;
                    Header[0x0E]=0;
                    Header[0x0F]=0x17; //FrameType + codecID : KeyFrame + AVC
            
                    Header[0x10]=0; //AVC PacketType: AVCSeqHeader
                    Header[0x11]=0; //Composition time
                    Header[0x12]=0;
                    Header[0x13]=0;
            
                    F_Previous=11+(5+Size);
                }
                else
                {
                    //TEMP
                    int32u Time=F_FrameCount*40; //in ms
                    //Time=Stream[(int32u)Element_Code].stts[F_FrameCount]/1000000;
            
                    int8u Header[0x14];
                    Header[0x00]=int8u(F_Previous>>24);
                    Header[0x01]=int8u(F_Previous>>16);
                    Header[0x02]=int8u(F_Previous>> 8);
                    Header[0x03]=int8u(F_Previous>> 0);
            
                    Header[0x04]=0x09; //Type
                    Header[0x05]=int8u((5+Size)>>16); //BodyLength
                    Header[0x06]=int8u((5+Size)>> 8);
                    Header[0x07]=int8u((5+Size)>> 0);
                    Header[0x08]=int8u((Time)>>16); //Timestamp_Base
                    Header[0x09]=int8u((Time)>> 8);
                    Header[0x0A]=int8u((Time)>> 0);
                    Header[0x0B]=0; //Timestamp_Extended
                    Header[0x0C]=0; //StreamID
                    Header[0x0D]=0;
                    Header[0x0E]=0;
            
                    if (F_FrameCount==0)
                        Header[0x0F]=0x17; //FrameType + codecID : KeyFrame + AVC
                    else
                        Header[0x0F]=0x27; //FrameType + codecID : AVC
            
            
                    int32s Time_Sav[]=
                    {
                        40, //I
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        80, //P
                        0,  //B
                        40, //P
                    };
            
                    Time=Time_Sav[F_FrameCount%24];
            
                    Header[0x10]=1; //AVC PacketType: NALU
                    //int32s Compo=Stream[(int32u)Element_Code].ctts[F_FrameCount];
                    Header[0x11]=int8u((Time)>>16); //Composition time
                    Header[0x12]=int8u((Time)>> 8);
                    Header[0x13]=int8u((Time)>> 0);

                    F_Previous=11+Size;
                    F_FrameCount++;
                }

                Pos+=32+Size;

            }
        }

    }
    while (From_Buffer_Size>0);
    //Text=MI.Inform();

    //Clean up
    delete[] From_Buffer;
    delete[] To_Buffer_1;

    return false;

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


