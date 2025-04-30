/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Manage preferences of MediaInfo
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef PreferencesH
#define PreferencesH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "ZenLib/ZtringListListF.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// For Unicode management
// Here only because this is useful in all MediaInfo
#ifdef _UNICODE
    static std::wstring String_Void;
    #define GUI_Text(A) A.w_str()
#else
    static std::string String_Void;
    #define GUI_Text(A) (A?Ztring().From_Unicode(A.c_bstr()).c_str():String_Void.c_str())
#endif //_UNICODE
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#define MediaInfo_Version_GUI __T("25.04")
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
enum Prefs_t
{
    Prefs_Language,           //List of words in the good language ("Name;Translation")
    Prefs_Sheet,              //Customized Sheet ("StreamKind|Name;Percentage of view")
    Prefs_Tree,               //Customized Tree  ("StreamKind|Name;Y or N")
    Prefs_Custom,             //Customized Text  ("StreamKind;Text for this streamKind")
    Prefs_Language_English,   //List of words in English ("Name;Translation")
    Prefs_Language_List,      //List of language translation in their language("Name;Translation")
    Prefs_Max,
};

//---------------------------------------------------------------------------

class Preferences
{
public:
    Preferences();

    //Infos
    ZenLib::Ztring          BaseFolder; //Folder of the Configuration files
    ZenLib::ZtringListListF Config;     //Configuration file
    ZenLib::ZtringList      FilesList[Prefs_Max];  //List of available files for an Item
    ZenLib::ZtringListListF Details[Prefs_Max];
    ZenLib::ZtringListList  SponsorMessage;
    ZenLib::ZtringListList  SponsorUrl;

    //Useful functions about preferences
    int Config_Create       ();
    int Config_Load         (); //Load the configuration, or create it if no exist. 1=Loaded, 2=Created
    int Config_Save         ();
    int Config_Remove       ();
    int RefreshFilesList    (Prefs_t KindOfList);
    int Create              (Prefs_t KindOfList, const ZenLib::Ztring &Name);
    int Copy                (Prefs_t KindOfList, const ZenLib::Ztring &From, const ZenLib::Ztring &To);
    int Load                (Prefs_t KindOfList, const ZenLib::Ztring &Name);
    int Remove              (Prefs_t KindOfList, const ZenLib::Ztring &Name);

    //Related to user preferences - Action only if Preferences are OK
    int InternetCheck       (); //Test if there is new update
    int ExplorerShell       (); //To be available with right click in Explorer
    int ShellToolTip        (); //To be available with mouse in Explorer

    //Special things
    ZenLib::Ztring &Translate (ZenLib::Ztring Name); //return in the good language, or in English if not found

    //Info about Lists
    ZenLib::ZtringList FolderNames;  //List of list_t --> Foldernames
    ZenLib::ZtringList DefaultNames; //List of list_t --> Default Filenames for a kind

    //Donate
    bool Donated;
    bool Donate_Display;
    bool NewVersion_Display;

    //Sponsor
    bool Sponsored;

    //Plugins
    ZenLib::Ztring GraphPluginURL;
    ZenLib::Ztring GraphPlugin64URL;
    ZenLib::Ztring GraphPluginVersion;
    ZenLib::Ztring FFmpegPluginURL;
    ZenLib::Ztring FFmpegPluginVersion;
};

//General preference for the whole program
extern Preferences* Prefs;

#endif
