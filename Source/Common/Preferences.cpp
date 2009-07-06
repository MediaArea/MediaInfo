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
int ExplorerShell_Edit  (const AnsiString &Name, bool ShellExtension, bool &IsChanged, TRegistry* Reg);
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
		Application->MessageBox(C3(Pos, 2).c_str(), _T("New version released!"));
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
        /*".3gp;mpeg4File\r\n"
        ".3gpp;mpeg4File\r\n"
        ".aac;AACFile\r\n"
        ".ac3;AC3File\r\n"
        ".ape;APEFile\r\n"
        ".asf;ASFFile\r\n"
        ".avi;AVIFile\r\n"
        ".bdmv;BDMVFile\r\n"
        ".clpi;CLPIFile\r\n"
        ".divx;AVIFile\r\n"
        ".dpg;DPGFile\r\n"
        ".dts;DTSFile\r\n"
        ".dv;EAC3File\r\n"
        ".dvr;DVRFile\r\n"
        ".dvr-ms;DVRMSFile\r\n"
        ".eac3;EAC3File\r\n"
        ".evo;EVOFile\r\n"
        ".flac;FLACFile\r\n"
        ".flv;FlashVideoFile\r\n"
        ".gvi;AVIFile\r\n"
        ".h264;H264File\r\n"
        ".ifo;IfoFile\r\n"
        ".isma;ISMAFile\r\n"
        ".ismv;ISMVFile\r\n"
        ".j2k;EAC3File\r\n"
        ".jp2;EAC3File\r\n"
        ".m1s;mpegFile\r\n"
        ".m1t;mpegFile\r\n"
        ".m1v;mpegFile\r\n"
        ".m2s;mpegFile\r\n"
        ".m2t;mpegFile\r\n"
        ".m2ts;m2tsFile\r\n"
        ".m2v;mpegFile\r\n"
        ".m4a;mpeg4File\r\n"
        ".m4v;mpeg4File\r\n"
        ".mac;APEFile\r\n"
        ".mka;MKAFile\r\n"
        ".mks;MKSFile\r\n"
        ".mkv;MKVFile\r\n"
        ".mod;modFile\r\n"
        ".mov;MOVFile\r\n"
        ".mp+;mpcFile\r\n"
        ".mp2;mp3File\r\n"
        ".mp3;mp3File\r\n"
        ".mp4;mpeg4File\r\n"
        ".mpc;mpcFile\r\n"
        ".mpe;mpegFile\r\n"
        ".mpeg;mpegFile\r\n"
        ".mpg;mpegFile\r\n"
        ".mpgv;mpegFile\r\n"
        ".mpgx;mpegFile\r\n"
        ".mpls;MPLSFile\r\n"
        ".mpm;mpegFile\r\n"
        ".mpv;mpegFile\r\n"
        ".mts;mpegFile\r\n"
        ".mxf;MXFFile\r\n"
        ".oga;OGAFile\r\n"
        ".ogg;OGGFile\r\n"
        ".ogm;OGMFile\r\n"
        ".ogv;OGVFile\r\n"
        ".qt;QTFile\r\n"
        ".ra;RMFile\r\n"
        ".rm;RMFile\r\n"
        ".rmvb;FlashVideoFile\r\n"
        ".smv;SMVFile\r\n"
        ".swf;FlashFile\r\n"
        ".tp;FlashVideoFile\r\n"
        ".trp;TRPFile\r\n"
        ".ts;tsFile\r\n"
        ".tta;TTAFile\r\n"
        ".vob;mpegFile\r\n"
        ".w64;W64File\r\n"
        ".wav;WAVFile\r\n"
        ".wma;WMAFile\r\n"
        ".wmv;WMVFile\r\n"
		".wv;WCFile\r\n"*/
        ".wvc;WVCFile\r\n"
        "Folder;Folder");

    // Get the Windows version.
    DWORD Version = GetVersion();
    DWORD MajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));
    DWORD MinorVersion = (DWORD)(HIBYTE(LOWORD(Version)));

    bool IsChanged=false;
    if ( MajorVersion>=6 //Windows Vista or more
     || (MajorVersion==5 && MinorVersion>=1)) //WinXP or more in 5.x family
    {
        //Removing old stuff
        for (size_t I1=0; I1<Liste.size(); I1++)
        {
            //Remove shell ext except "Folder"
            if (Liste(I1, 0)!=_T("Folder") && Reg->OpenKey(Liste(I1, 0).c_str(), false))
            {
                //test if extension is known
                AnsiString Player=Reg->ReadString(_T(""));
				Reg->CloseKey();

                //Test if old Media Info shell extension is known
                if (Player!="" && Reg->OpenKey(Player+_T("\\Shell\\Media Info\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\Media Info");
                    IsChanged=true;
                    Reg->CloseKey();
                }

                //Test if MediaInfo shell extension is known
                if (Player!="" && Reg->OpenKey(Player+_T("\\Shell\\MediaInfo\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
                    IsChanged=true;
                    Reg->CloseKey();
                }
            }

            //Remove shell ext except "Folder" (user part)
            if (Liste(I1, 0)!=_T("Folder") && Reg_User->OpenKey((Ztring(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\"))+Liste(I1, 0)+_T("\\UserChoice")).c_str(), false))
            {
                //test if extension is known
                AnsiString Player=Reg_User->ReadString("Progid");
                Reg_User->CloseKey();

                //Test if MediaInfo shell extension is known
                if (Player!="" && Reg->OpenKey(Player+_T("\\Shell\\MediaInfo\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
                    IsChanged=true;
                    Reg->CloseKey();
                }
            }
        }
        ExplorerShell_Edit("SystemFileAssociations\\audio", 0, IsChanged, Reg);
        ExplorerShell_Edit("SystemFileAssociations\\Directory.Audio", 0, IsChanged, Reg);
        ExplorerShell_Edit("SystemFileAssociations\\Directory.Video", 0, IsChanged, Reg);
        ExplorerShell_Edit("SystemFileAssociations\\video", 0, IsChanged, Reg);
        ExplorerShell_Edit("Folder", 0, IsChanged, Reg);

        //Adding/removing to SystemFileAssociations
		int32s ShellExtension=Config.Read(_T("ShellExtension")).To_int32s();
        for (size_t I1=0; I1<Liste.size(); I1++)
        {
            //Remove shell ext except "Folder"
            if (Liste(I1, 0)!=_T("Folder"))
                ExplorerShell_Edit((_T("Software\\Classes\\SystemFileAssociations\\")+Liste(I1, 0)).c_str(), ShellExtension, IsChanged, Reg_User);
        }
        ExplorerShell_Edit("Folder", Config.Read(_T("ShellExtension_Folder")).To_int32s(), IsChanged, Reg);
    }
    else
    {
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

                //Test if old Media Info shell extension is known
                if (Reg->OpenKey(Player+_T("\\Shell\\Media Info\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\Media Info");
                    IsChanged=true;
                    Reg->CloseKey();
                }

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

            //Open (or create) a extension (user). Create only if Shell extension is wanted
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
    }

    //end
    delete Reg; //Reg=NULL;
    delete Reg_User; //Reg_User=NULL;
    if (IsChanged)
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return 1;
}

//---------------------------------------------------------------------------
int ExplorerShell_Edit(const AnsiString &Player, bool ShellExtension, bool &IsChanged, TRegistry* Reg)
{
    if (Reg->OpenKey(Player+"\\Shell\\MediaInfo\\Command", false))
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
                try {Reg->WriteString(_T(""), ShellExtensionToWtrite);} catch (...){MessageBox(NULL, L"11 - Exception", L"MI", MB_OK);}
                IsChanged=true;
            }
        }
        else
        {
            //Should not be here, deleting
            Reg->CloseKey();
            Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
            if (Reg->OpenKey(Player+"\\Shell", false) && !Reg->HasSubKeys())
            {
                Reg->CloseKey();
                Reg->DeleteKey(Player+"\\Shell");
            }
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
            try {Reg->WriteString(_T(""), ShellExtensionToWtrite);} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            IsChanged=true;
            Reg->CloseKey();
        }
    }

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::ShellToolTip()
{
    TRegistry* Reg_User=new TRegistry;

    // Get the Windows version.
    DWORD Version = GetVersion();
    DWORD MajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));
    DWORD MinorVersion = (DWORD)(HIBYTE(LOWORD(Version)));

    bool IsChanged=false;
    if (!( MajorVersion>=6 //Windows Vista or more
     || (MajorVersion==5 && MinorVersion>=1))) //WinXP or more in 5.x family
        return 0; //No supported


    int32s ShellInfoTip=Config.Read(_T("ShellInfoTip")).To_int32s();

    ZtringList Liste;
    Liste=_T(
        ".3gp;"
        ".3gpp;"
        ".aac;"
        ".ac3;"
        ".ape;"
        ".asf;"
        ".avi;"
        ".bdmv;"
        ".clpi;"
        ".divx;"
        ".dpg;"
        ".dts;"
        ".dv;"
        ".dvr;"
        ".dvr-ms;"
        ".eac3;"
        ".evo;"
        ".flac;"
        ".flv;"
        ".gvi;"
        ".h264;"
        ".ifo;"
        ".isma;"
        ".ismv;"
        ".j2k;"
        ".jp2;"
        ".m1s;"
        ".m1t;"
        ".m1v;"
        ".m2s;"
        ".m2t;"
        ".m2ts;"
        ".m2v;"
        ".m4a;"
        ".m4v;"
        ".mac;"
        ".mka;"
        ".mks;"
        ".mkv;"
        ".mod;"
        ".mov;"
        ".mp+;"
        ".mp2;"
        ".mp3;"
        ".mp4;"
        ".mpc;"
        ".mpe;"
        ".mpeg;"
        ".mpg;"
        ".mpgv;"
        ".mpgx;"
        ".mpls;"
        ".mpm;"
        ".mpv;"
        ".mts;"
        ".mxf;"
        ".oga;"
        ".ogg;"
        ".ogm;"
        ".ogv;"
        ".qt;"
        ".ra;"
        ".rm;"
        ".rmvb;"
        ".smv;"
        ".swf;"
        ".tp;"
        ".trp;"
        ".ts;"
        ".tta;"
        ".vob;"
        ".w64;"
        ".wav;"
        ".wma;"
        ".wmv;"
        ".wv;"
        ".wvc;"
                );

    if (Reg_User->OpenKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32", false))
    {
        //MediaInfo shell extension is known
		if (ShellInfoTip)
		{
            //test if good writing
			std::string DLL_Name=Application->ExeName.c_str();
			DLL_Name.resize(DLL_Name.rfind('\\')); //Removing ".exe"
			DLL_Name+="\\MediaInfo_InfoTip.dll";
            std::string ShellInfoTipToWtrite="\"" + DLL_Name +"\"";
			std::string ShellInfoTip_Existing=Reg_User->ReadString(_T("")).c_str();
            if (ShellInfoTip_Existing!=ShellInfoTipToWtrite)
            {
                //This is not the good shell extension, writing new one
                try {Reg_User->WriteString("", ShellInfoTipToWtrite.c_str());} catch (...){MessageBox(NULL, L"11 - Exception", L"MI", MB_OK);}
                IsChanged=true;
            }
        }
        else
        {
            //Should not be here, deleting
            Reg_User->CloseKey();
			Reg_User->DeleteKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32");
            Reg_User->DeleteKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_");
            Reg_User->DeleteKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1");
            for (size_t I1=0; I1<Liste.size(); I1++)
            {
                //Remove
                if (!Liste(I1).empty())
                    Reg_User->DeleteKey((_T("Software\\Classes\\")+Liste(I1)+_T("\\shellex\\{00021500-0000-0000-C000-000000000046}")).c_str());
            }

            IsChanged=true;
        }
        Reg_User->CloseKey();
    }
    else
    {
        //MediaInfo Shell extension is not known
        if (ShellInfoTip)
        {
            //Create it
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}", true);
            try {Reg_User->WriteString("", "MediaInfoShellExt_ Class");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
			Reg_User->CloseKey();
			Reg_User->OpenKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32", true);
			std::string DLL_Name=Application->ExeName.c_str();
            DLL_Name.resize(DLL_Name.rfind('\\')); //Removing ".exe"
            DLL_Name+="\\MediaInfo_InfoTip.dll";
            std::string ShellInfoTipToWtrite="\"" + DLL_Name +"\"";
            try {Reg_User->WriteString("", ShellInfoTipToWtrite.c_str());} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
			try {Reg_User->WriteString("ThreadingModel", "Apartment");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID", true);
            try {Reg_User->WriteString("", "MediaInfoShellExt.MediaInfoShellExt_.1");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib", true);
            try {Reg_User->WriteString("", "{BC1AAA9F-D8C5-4EB2-A10A-61B86B7EA77C}");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID", true);
            try {Reg_User->WriteString("", "MediaInfoShellExt.MediaInfoShellExt_");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_", true);
            try {Reg_User->WriteString("", "MediaInfoShellExt_ Class");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID", true);
            try {Reg_User->WriteString("", "{869C14C8-1830-491F-B575-5F9AB40D2B42}");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer", true);
            try {Reg_User->WriteString("", "MediaInfoShellExt.MediaInfoShellExt_.1");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1", true);
            try {Reg_User->WriteString("", "MediaInfoShellExt_ Class");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            Reg_User->OpenKey("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID", true);
            try {Reg_User->WriteString("", "{869C14C8-1830-491F-B575-5F9AB40D2B42}");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
            Reg_User->CloseKey();
            for (size_t I1=0; I1<Liste.size(); I1++)
            {
                //Add
                if (!Liste(I1).empty())
                {
                    Reg_User->OpenKey((_T("Software\\Classes\\")+Liste(I1)+_T("\\shellex\\{00021500-0000-0000-C000-000000000046}")).c_str(), true);
                    try {Reg_User->WriteString("", "{869C14C8-1830-491F-B575-5F9AB40D2B42}");} catch (...){MessageBox(NULL, L"21 - Exception", L"MI", MB_OK);}
                    Reg_User->CloseKey();
                }
            }
            IsChanged=true;
        }
    }

    delete Reg_User; //Reg_User=NULL;
    if (IsChanged)
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
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


