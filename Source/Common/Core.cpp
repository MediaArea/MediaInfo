// Main - Main functions
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
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
#include "Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <sstream>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const MediaInfoNameSpace::Char* MEDIAINFO_TITLE=_T("MediaInfo - http://mediainfo.sourceforge.net");
ZenLib::Ztring Text_Temp;
using namespace ZenLib;
//---------------------------------------------------------------------------


//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Core::Core()
{
    MI=new MediaInfoNameSpace::MediaInfoList;

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
void Core::Menu_File_Open_Files_Begin (bool close)
{
    if(close)
        MI->Close();
}

//---------------------------------------------------------------------------
size_t Core::Menu_File_Open_Files_Continue (const String &FileName)
{
    return MI->Open(FileName);
}

//---------------------------------------------------------------------------
void Core::Menu_File_Open_Directory (const String &DirectoryName)
{
    MI->Open(DirectoryName);
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
    MI->Option(_T("Inform"), String());
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
void Core::Menu_View_XML ()
{
    MI->Option(_T("Inform"), _T("XML"));
    Kind=Kind_XML;
}

//---------------------------------------------------------------------------
void Core::Menu_View_PBCore ()
{
    MI->Option(_T("Inform"), _T("PBCore"));
    Kind=Kind_PBCore;
}

//---------------------------------------------------------------------------
void Core::Menu_View_MPEG7 ()
{
    MI->Option(_T("Inform"), _T("MPEG-7"));
    Kind=Kind_MPEG7;
}

//---------------------------------------------------------------------------
void Core::Menu_View_Text ()
{
    MI->Option(_T("Inform"), String());
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
String Core::Menu_Option_Preferences_Option (const String& Param, const String& Value)
{
    return MI->Option(Param, Value);
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
    MI->Option(_T("Inform"), String());
    return MI->Get(Pos, StreamKind, StreamPos, _T("Inform")).c_str();
}

// Accessors
size_t Core::Count_Get() {
    return MI->Count_Get();
}

//---------------------------------------------------------------------------
size_t Core::Count_Get(size_t File_Pos, stream_t StreamKind, size_t StreamNumber)
{
    return MI->Count_Get(File_Pos, StreamKind, StreamNumber);
}

//---------------------------------------------------------------------------
String Core::Summary_Get(int File_Pos, stream_t StreamKind, size_t StreamPos)
{
    MI->Option(_T("Inform"), _T("Summary"));
    return MI->Get(File_Pos, StreamKind, StreamPos, _T("Inform")).c_str();
}

//---------------------------------------------------------------------------
String Core::Get (size_t FilePos, stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t InfoKind) {
    return MI->Get(FilePos,StreamKind,StreamNumber,Parameter,InfoKind);
}

//---------------------------------------------------------------------------
String Core::Get (size_t FilePos, stream_t StreamKind, size_t StreamNumber, const String &Parameter, info_t InfoKind) {
    return MI->Get(FilePos,StreamKind,StreamNumber,Parameter,InfoKind);
}

String Core::Parameters () {
    return MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"));
}
