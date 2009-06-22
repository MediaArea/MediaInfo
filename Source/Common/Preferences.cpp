// Preferences - Manage preferences of MediaInfo
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
// Manage preferences of MediaInfo
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#undef NO_WIN32_LEAN_AND_MEAN
#include <ZenLib/Conf.h>
#pragma hdrstop
#include <Forms.hpp>
#include <Registry.hpp>
#include <Wininet.h>
#include <Shlobj.h>
#include "Preferences.h"
#include <ZenLib/Dir.h>
#include <ZenLib/File.h>
#include <ZenLib/HTTP_Client.h>
using namespace ZenLib;
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
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Preferences* Prefs=new Preferences;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Preferences::Preferences()
{
    //Name of folders
    FolderNames=_T(
        "Language;"
        "Sheet;"
        "Tree;"
        "Custom;"
        "Language");
    //Name of default for kind of list
    DefaultNames=_T(
        "en;"
        "Example;"
        "Example;"
        "Example;"
        "en;");
}

//***************************************************************************
// Config
//***************************************************************************

//---------------------------------------------------------------------------
int Preferences::Config_Create()
{
    Config.Load(BaseFolder+_T("MediaInfo.cfg"));
    Config(_T("Version"))=MediaInfo_Version_GUI;
    Config(_T("NewestVersion"))=MediaInfo_Version_GUI;
    if (Config(_T("Output")).empty()) Config(_T("Output"))=_T("Basic");
    if (Config(_T("Language")).empty()) Config(_T("Language"))=_T("en");
    if (Config(_T("Sheet")).empty()) Config(_T("Sheet"))=_T("Example");
    if (Config(_T("Tree")).empty()) Config(_T("Tree"))=_T("Example");
    if (Config(_T("Custom")).empty()) Config(_T("Custom"))=_T("Example");
    if (Config(_T("CheckUpdate")).empty()) Config(_T("CheckUpdate"))=_T("1");
    if (Config(_T("ShellExtension")).empty()) Config(_T("ShellExtension"))=_T("1");
    if (Config(_T("ShellExtension_Folder")).empty()) Config(_T("ShellExtension_Folder"))=_T("0");
    if (Config(_T("ShellInfoTip")).empty()) Config(_T("ShellInfoTip"))=_T("0");
    if (Config(_T("ShowToolBar")).empty()) Config(_T("ShowToolBar"))=_T("1");
    if (Config(_T("ShowMenu")).empty()) Config(_T("ShowMenu"))=_T("1");
    if (Config(_T("CloseAllAuto")).empty()) Config(_T("CloseAllAuto"))=_T("1");
    Config.Save(BaseFolder+_T("MediaInfo.cfg"));

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::Config_Load()
{
    int Retour=1;

    //Base folder
    BaseFolder.From_Local(Application->ExeName.c_str());
    BaseFolder=BaseFolder.substr(0, BaseFolder.rfind(_T("\\"))+1);
    if (!Dir::Exists(BaseFolder+_T("Plugin")))
        Dir::Create(BaseFolder+_T("Plugin"));
    BaseFolder+=_T("Plugin\\");

    //Directories
    for (size_t Pos=0; Pos<FolderNames.size(); Pos++)
        if (!Dir::Exists(BaseFolder+FolderNames[Pos]))
            Dir::Create(BaseFolder+FolderNames[Pos]);

    //Default language
    if (!File::Exists(BaseFolder+_T("Language\\en.csv")))
        Create(Prefs_Language, _T("en"));
    Load(Prefs_Language_English, _T("en"));

    //Configuration
    Config.Load(BaseFolder+_T("MediaInfo.cfg"));
    if (Config.size()==0 || Config(_T("Version"))!=MediaInfo_Version_GUI)
    {
        Config_Create();
        Retour=2;
    }

    RefreshFilesList(Prefs_Language);
    Load(Prefs_Language, Config(FolderNames[Prefs_Language]));
    RefreshFilesList(Prefs_Sheet);
    Load(Prefs_Sheet, Config(FolderNames[Prefs_Sheet]));
    RefreshFilesList(Prefs_Tree);
    Load(Prefs_Tree, Config(FolderNames[Prefs_Tree]));
    RefreshFilesList(Prefs_Custom);
    Load(Prefs_Custom, Config(FolderNames[Prefs_Custom]));

    //Shell Extension
    ExplorerShell();
    ShellToolTip();

    //Internet
    if (Retour!=2 && Config(_T("CheckUpdate"))==_T("1"))
        Prefs->InternetCheck();

    return Retour;
}

//---------------------------------------------------------------------------
int Preferences::Config_Save()
{
    return Config.Save();
}

//---------------------------------------------------------------------------
int Preferences::Config_Remove()
{
    return -1;
}

//***************************************************************************
// KindOfList management
//***************************************************************************

//---------------------------------------------------------------------------
int Preferences::Create(Prefs_t KindOfList, const ZenLib::Ztring &Name)
{
    switch (KindOfList)
    {
        case Prefs_Language :
            Details[KindOfList]=MediaInfoNameSpace::MediaInfo::Option_Static(_T("Language_Get")).c_str();
            if (Name!=_T("en"))
                for (size_t Pos=0; Pos<Details[KindOfList].size(); Pos++)
                    Details[KindOfList](Pos, 1)=_T("");
            break;
        case Prefs_Sheet :
            Details[KindOfList]=_T(
                "ColumnsCount;5\r\n"
                "Column0;General;0;CompleteName;30\r\n"
                "Column1;General;0;Format;10\r\n"
                "Column2;General;0;Video_Codec_List;10\r\n"
                "Column3;General;0;Audio_Codec_List;10\r\n"
                "Column4;General;0;Text_Codec_List;10");
            break;
        case Prefs_Tree :
            break;
        case Prefs_Custom :
            Details[KindOfList]=_T(
                "General;General           : %FileName%\\r\\nFormat            : %Format%$if(%OveralBitRate%, at %OveralBitRate/String%)\\r\\nLength            : %FileSize/String1% for %PlayTime/String1%\\r\\n\\r\\n\r\n"
                "Video;Video #%StreamKindID%          : %Codec/String%$if(%BitRate%, at %BitRate/String%)\\r\\nAspect            : %Width% x %Height% (%AspectRatio%) at %fps% fps\\r\\n\\r\\n\r\n"
                "Audio;Audio #%StreamKindID%          : %Codec/String%$if(%BitRate%, at %BitRate/String%)\\r\\nInfos             : %Channel(s)/String%, %SamplingRate/String%\\r\\n$if(%Language%,Language          : %Language%\\r\\n)\\r\\n\r\n"
                "Text;Text #%StreamKindID%           : %Codec/String%\\r\\n$if(%Language%,Language          : %Language%\\r\\n)\\r\\n\r\n"
                "Chapters;Chapters #%StreamKindID%       : %Total% chapters\\r\\n\\r\\n\r\n"
                "File_Begin\r\n"
                "File_End\r\n"
                "Page_Begin\r\n"
                "Page_Middle\r\n"
                "Page_End\r\n"
                "General_Begin\r\n"
                "General_End\r\n"
                "Video_Begin\r\n"
                "Video_Middle\r\n"
                "Video_End\r\n"
                "Audio_Begin\r\n"
                "Audio_Middle\r\n"
                "Audio_End\r\n"
                "Text_Begin\r\n"
                "Text_Middle\r\n"
                "Text_End\r\n"
                "Chapters_Begin\r\n"
                "Chapters_Middle\r\n"
                "Chapters_End");
            break;
        default:
            return -1;
    }
    Details[KindOfList].Save(BaseFolder+FolderNames[KindOfList]+_T("\\")+Name+_T(".csv"));

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::Copy(Prefs_t KindOfList, const ZenLib::Ztring &From, const ZenLib::Ztring &To)
{
    return File::Copy(BaseFolder+FolderNames[KindOfList]+_T("\\")+From+_T(".csv"), BaseFolder+_T("MediaInfo.")+FolderNames[KindOfList]+_T(".")+To+_T(".csv"));
}

//---------------------------------------------------------------------------
int Preferences::Load(Prefs_t KindOfList, const ZenLib::Ztring &Name)
{
    //Test if the file exists
    if (!File::Exists(BaseFolder+FolderNames[KindOfList]+_T("\\")+Name+_T(".csv")))
        return -1;

    Details[KindOfList].Load(BaseFolder+FolderNames[KindOfList]+_T("\\")+Name+_T(".csv"));

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::Remove(Prefs_t KindOfList, const ZenLib::Ztring &Name)
{
    Ztring Lang; if (KindOfList==Prefs_Custom) Lang=Config(_T("Language"))+_T("."); //Special case : if Custom; we add Language in the file name
    return File::Delete(BaseFolder+FolderNames[KindOfList]+_T("\\")+Lang+Name+_T(".csv"));
}

//***************************************************************************
// FilesLists
//***************************************************************************

//---------------------------------------------------------------------------
int Preferences::RefreshFilesList(Prefs_t KindOfList)
{
    //Default available?
    if (!File::Exists(BaseFolder+FolderNames[KindOfList]+_T("\\")+DefaultNames[KindOfList]+_T(".csv")))
        Create(KindOfList, DefaultNames[KindOfList]);

    //Listing
    FilesList[KindOfList]=Dir::GetAllFileNames(BaseFolder+FolderNames[KindOfList]+_T("\\")+_T("*.csv"));
    if (KindOfList==Prefs_Language)
    {
        //Special case : Languages, should show the name of language in the local version
        FilesList[Prefs_Language_List].clear();
        for (size_t Pos=0; Pos<FilesList[KindOfList].size(); Pos++)
        {
            ZtringListListF Lang;
            Lang.Load(FilesList[KindOfList][Pos]);
            Ztring Local=Lang(_T("  Language_Name"));
            if (Local.size()==0)
                Local=FilesList[KindOfList][Pos].SubString(BaseFolder+FolderNames[KindOfList]+_T("\\"), _T(".csv"));
            if (Local.find(_T("(Chris)"))==Ztring::npos) //quick method for no more showing Chris translation (deprecated)
                FilesList[Prefs_Language_List].push_back(Local);
            else
            {
                //quick method for no more showing Chris translation (deprecated)
                FilesList[KindOfList].erase(FilesList[KindOfList].begin()+Pos);
                Pos--;
            }
        }
    }

    //Only name
    for (size_t Pos=0; Pos<FilesList[KindOfList].size(); Pos++)
        FilesList[KindOfList][Pos]=FilesList[KindOfList][Pos].SubString(BaseFolder+FolderNames[KindOfList]+_T("\\"), _T(".csv"));

    return 1;
}

//***************************************************************************
// System
//***************************************************************************

class ThreadInternetCheck:public TThread
{
public:
    __fastcall ThreadInternetCheck(bool CreateSuspended): TThread(CreateSuspended){}
    void __fastcall Execute();
};

void __fastcall ThreadInternetCheck::Execute()
{
    //Connexion test
    if (InternetGetConnectedState(NULL, 0)==0)
        return; //No internet connexion

    HTTP_Client H;
    if (H.Open(Ztring(_T("http://mediaarea.net/mediainfo_check/changelog_"))+MediaInfo_Version_GUI+_T(".bin"))==0)
        return;

    Ztring Z=H.Read();
    ZtringListList Download(Z);

    //Verification de la version
    Ztring Version=Prefs->Config.FindValue(_T("NewestVersion"), 1);
    Ztring NewestVersion=Download(_T("NewVersion"));
    if (NewestVersion!=_T("") && NewestVersion>Version)
    {
        //Affichage d'un message
        ZtringListList C3=Download.SubSheet(_T("NewMessage"));
        int Pos=C3.Find(Prefs->Config(_T("Language")), 1);
        if (Pos==-1)
            Pos=C3.Find(_T("en"), 1);
        if (Pos==-1)
            return;
        C3(Pos, 2).FindAndReplace(_T("\\r\\n"), _T("\r\n"));
        Application->MessageBox(AnsiString(C3(Pos, 2).c_str()).c_str(), "New version released!");
        //Inscription version connue pour pas repeter l'avertissement
        int Version_Pos=Prefs->Config.Find(_T("NewestVersion"));
        Prefs->Config(Version_Pos, 1)=NewestVersion;
        Prefs->Config.Save();
    }

    //Chargement de pages
    ZtringListList Pages=Download.SubSheet(_T("Url"));
    for (size_t Pos=0; Pos<Pages.size(); Pos++)
    {
        //Doit-on charger ou pas?
        if (Pages.Read(Pos, 1)==_T("1"))
        {
            HTTP_Client H2;
            if (H2.Open(Pages.Read(Pos, 2))==0)
                return;
            Ztring Image=H2.Read();
        }
    }

    return;
}

//---------------------------------------------------------------------------
int Preferences::InternetCheck()
{
    new ThreadInternetCheck(false);
    return 1;
}

//---------------------------------------------------------------------------
int Preferences::ExplorerShell()
{
    TRegistry* Reg=new TRegistry;
    Reg->RootKey = HKEY_CLASSES_ROOT;
    TRegistry* Reg_User=new TRegistry;

    ZtringListList Liste;
    Liste=_T(
        ".mkv;MKVFile\r\n"
        ".mka;MKAFile\r\n"
        ".mks;MKSFile\r\n"
        ".ogg;OGGFile\r\n"
        ".ogm;OGMFile\r\n"
        ".oga;OGAFile\r\n"
        ".ogv;OGVFile\r\n"
        ".wav;WAVFile\r\n"
        ".avi;AVIFile\r\n"
        ".divx;AVIFile\r\n"
        ".gvi;AVIFile\r\n"
        ".mpeg;mpegFile\r\n"
        ".mpg;mpegFile\r\n"
        ".mpe;mpegFile\r\n"
        ".mpgx;mpegFile\r\n"
        ".mpm;mpegFile\r\n"
        ".m1s;mpegFile\r\n"
        ".m1t;mpegFile\r\n"
        ".vob;mpegFile\r\n"
        ".m2s;mpegFile\r\n"
        ".m2t;mpegFile\r\n"
        ".mp4;mpeg4File\r\n"
        ".m4a;mpeg4File\r\n"
        ".m4v;mpeg4File\r\n"
        ".mpgv;mpegFile\r\n"
        ".mpv;mpegFile\r\n"
        ".m1v;mpegFile\r\n"
        ".m2v;mpegFile\r\n"
        ".ts;tsFile\r\n"
        ".m2ts;m2tsFile\r\n"
        ".mp2;mp3File\r\n"
        ".mp3;mp3File\r\n"
        ".mpc;mpcFile\r\n"
        ".mp+;mpcFile\r\n"
        ".asf;ASFFile\r\n"
        ".wmv;WMVFile\r\n"
        ".wma;WMAFile\r\n"
        ".mov;MOVFile\r\n"
        ".qt;QTFile\r\n"
        ".rm;RMFile\r\n"
        ".ra;RMFile\r\n"
        ".ifo;IfoFile\r\n"
        ".ac3;AC3File\r\n"
        ".dts;DTSFile\r\n"
        ".aac;AACFile\r\n"
        ".ape;APEFile\r\n"
        ".mac;APEFile\r\n"
        ".flac;FLACFile\r\n"
        ".3gp;mpeg4File\r\n"
        ".3gpp;mpeg4File\r\n"
        ".swf;FlashFile\r\n"
        ".flv;FlashVideoFile\r\n"
        ".rmvb;FlashVideoFile\r\n"
        ".tp;FlashVideoFile\r\n"
        ".smv;SMVFile\r\n"
        ".dpg;DPGFile\r\n"
        ".evo;EVOFile\r\n"
        ".eac3;EAC3File\r\n"
        ".dv;EAC3File\r\n"
        ".jp2;EAC3File\r\n"
        ".j2k;EAC3File\r\n"
        ".h264;H264File\r\n"
        ".dvr;DVRFile\r\n"
        ".dvr-ms;DVRMSFile\r\n"
        ".mod;modFile\r\n"
        ".isma;ISMAFile\r\n"
        ".ismv;ISMVFile\r\n"
        ".trp;TRPFile\r\n"
        ".tta;TTAFile\r\n"
        ".w64;W64File\r\n"
        ".wv;WCFile\r\n"
        ".wvc;WVCFile\r\n"
        ".bdmv;BDMVFile\r\n"
        ".mpls;MPLSFile\r\n"
        ".clpi;CLPIFile\r\n"
        "Folder;Folder");

    bool IsChanged=false;
    int32s ShellExtension=Config.Read(_T("ShellExtension")).To_int32s();
    for (size_t I1=0; I1<Liste.size(); I1++)
    {
        if (Liste(I1, 0)==_T("Folder"))
            ShellExtension=Config.Read(_T("ShellExtension_Folder")).To_int32s();

        //Open (or create) a extension. Create only if Sheel extension is wanted
        if (Reg->OpenKey(Liste(I1, 0).c_str(), ShellExtension))
        {
            //test if extension is known
            AnsiString Player=Reg->ReadString(_T(""));
            if (Player=="")
            {
                //extension not known, will use our default
                Player=Liste(I1, 1).c_str();
                try {Reg->WriteString(_T(""), Player);} catch (...){}
                IsChanged=true;
            }
            Reg->CloseKey();

            if (Liste(I1, 0)==_T("Folder"))
                Player="Folder";

            //Test if MediaInfo shell extension is known
            if (Reg->OpenKey(Player+_T("\\Shell\\MediaInfo\\Command"), false))
            {
                //MediaInfo shell extension is known
                if (ShellExtension)
                {
                    //test if good writing
                    AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                    AnsiString ShellExtension=Reg->ReadString(_T("")).c_str();
                    if (ShellExtension!=ShellExtensionToWtrite)
                    {
                        //This is not the good shell extension, writing new one
                        try {Reg->WriteString(_T(""), ShellExtensionToWtrite);} catch (...){}
                        IsChanged=true;
                    }
                }
                else
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
                    Reg->DeleteKey(Player+"\\Shell\\Media Info");
                    IsChanged=true;
                }
                Reg->CloseKey();
            }
            else
            {
                //MediaInfo Shell extension is not known
                if (ShellExtension)
                {
                    //Create it
                    Reg->DeleteKey(Player+"\\Shell\\Media Info"); //Delete the lod version if it exists
                    Reg->OpenKey(Player+_T("\\Shell\\MediaInfo\\Command"), true);
                    AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                    try {Reg->WriteString(_T(""), ShellExtensionToWtrite);} catch (...){}
                    Reg->CloseKey();
                    IsChanged=true;
                }
            }
        }

        //Open (or create) a extension (user). Create only if Sheel extension is wanted
        Ztring A=Liste(I1, 0);
        if (Reg_User->OpenKey((Ztring(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\"))+Liste(I1, 0)+_T("\\UserChoice")).c_str(), false))
        {
            //test if extension is known
            AnsiString Player=Reg_User->ReadString("Progid");
            Reg_User->CloseKey();

            //Test if MediaInfo shell extension is known
            if (Player!="")
            {
                if (Reg->OpenKey(Player+_T("\\Shell\\MediaInfo\\Command"), false))
                {
                    //MediaInfo shell extension is known
                    if (ShellExtension)
                    {
                        //test if good writing
                        AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                        AnsiString ShellExtension=Reg->ReadString(_T("")).c_str();
                        if (ShellExtension!=ShellExtensionToWtrite)
                        {
                            //This is not the good shell extension, writing new one
                            try {Reg->WriteString(_T(""), ShellExtensionToWtrite);} catch (...){}
                            IsChanged=true;
                        }
                    }
                    else
                    {
                        //Should not be here, deleting
                        Reg->CloseKey();
                        Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
                        IsChanged=true;
                    }
                    Reg->CloseKey();
                }
                else
                {
                    //MediaInfo Shell extension is not known
                    if (ShellExtension)
                    {
                        //Create it
                        Reg->OpenKey(Player+_T("\\Shell\\MediaInfo\\Command"), true);
                        AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                        try {Reg->WriteString(_T(""), ShellExtensionToWtrite);} catch (...){}
                        Reg->CloseKey();
                        IsChanged=true;
                    }
                }
            }
        }
    }

    //end
    Reg->Free();
    if (IsChanged)
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::ShellToolTip()
{
    //Done in the GUI for performance. TODO : analyse only if this has changed
    return 1;
}

//***************************************************************************
// Translations
//***************************************************************************

//---------------------------------------------------------------------------
ZenLib::Ztring &Preferences::Translate(ZenLib::Ztring Name)
{
    size_t Pos=Details[Prefs_Language].Find(Name, 0, 0, _T("=="), Ztring_CaseSensitive);

    //If not in the language, search for English language
    if (Pos==-1 || Details[Prefs_Language][Pos].size()<2)
        return Details[Prefs_Language_English](Name);
    else
        return Details[Prefs_Language](Pos)(1);
}


