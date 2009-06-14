// Core - Main functions
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
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
//
// Core functions
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef CoreH
#define CoreH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_DLL_RUNTIME
    #include "MediaInfoDLL/MediaInfoDLL.h"
    #define MediaInfoNameSpace MediaInfoDLL
#elif defined MEDIAINFO_DLL_STATIC
    #include "MediaInfoDLL/MediaInfoDLL_Static.h"
    #define MediaInfoNameSpace MediaInfoDLL
#else
    #include "MediaInfo/MediaInfoList.h"
    #define MediaInfoNameSpace MediaInfoLib
#endif
using namespace MediaInfoNameSpace;
//---------------------------------------------------------------------------

//***************************************************************************
// Class Core
//***************************************************************************

class Core
{
public:
    //Constructor/Destructor
    Core();
    ~Core();

    //Menu
    size_t  Menu_File_Open_File             (const MediaInfoNameSpace::String &FileName);
    void    Menu_File_Open_Files_Begin      ();
    size_t  Menu_File_Open_Files_Continue   (const MediaInfoNameSpace::String &FileName);
    void    Menu_File_Open_Directory        (const MediaInfoNameSpace::String &DirectoryName);
    void    Menu_View_Easy                  ();
    void    Menu_View_Sheet                 ();
    void    Menu_View_Tree                  ();
    void    Menu_View_HTML                  ();
    void    Menu_View_Text                  ();
    void    Menu_Option_Preferences_Inform  (const MediaInfoNameSpace::String &Inform);
    void    Menu_Option_Preferences_Option  (const MediaInfoNameSpace::String &Param, const MediaInfoNameSpace::String &Value);
    void    Menu_Debug_Complete             (bool Value);
    void    Menu_Debug_Details              (float Value);
    void    Menu_Debug_Demux                (const MediaInfoNameSpace::String &Value);
    void    Menu_Help_Version               ();
    void    Menu_Help_Info_Formats          ();
    void    Menu_Help_Info_Codecs           ();
    void    Menu_Help_Info_Parameters       ();
    void    Menu_Language                   (const MediaInfoNameSpace::String &Language);

    //Datas
    MediaInfoNameSpace::String &Inform_Get();
    enum kind {Kind_Easy, Kind_Sheet, Kind_Tree, Kind_HTML, Kind_Text};
    kind    Kind_Get();
    MediaInfoNameSpace::String  Inform_Get          (size_t Pos, stream_t StreamKind, size_t StreamPos);

    //Temp
    void    Data_Prepare();
    MediaInfoNameSpace::String &Text_Get() {return Text;};

protected:
    MediaInfoNameSpace::MediaInfoList* MI;
    MediaInfoNameSpace::String  Text;
    kind    Kind;
    float   Details;
    MediaInfoNameSpace::String  Demux;

    friend class GUI_Main_Common_Core;
    friend class GUI_Main_Easy_Core;
    friend class GUI_Main_Easy_Box_Core;
};

#endif
