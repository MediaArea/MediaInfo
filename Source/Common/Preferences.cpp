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
#include <ctime>
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

    //New version
    NewVersion_Display=false;

    //Donate
    Donate_Display=true;
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
    if (Config(_T("FirstInstall")).empty()) Config(_T("FirstInstall")).From_Number((int64u)time(NULL));
    Config(_T("Install")).From_Number((int64u)time(NULL));
    if (Config(_T("Donated")).empty()) Config(_T("Donated"))=_T("0");
    if (Config(_T("Donate_Display")).empty()) Config(_T("Donate_Display"))=_T("1");
    Config.Save(BaseFolder+_T("MediaInfo.cfg"));

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::Config_Load()
{
    int Retour=1;

    //Base folder
    BaseFolder=Application->ExeName.c_str();
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

    //New version menu
    if (!Prefs->Config(_T("Version")).empty() && Prefs->Config(_T("Version"))!=Prefs->Config(_T("NewestVersion")))
        Prefs->NewVersion_Display=true;

    //Donate display
    TRegistry* Reg_User=new TRegistry;
    try
    {
        if (Reg_User->OpenKey(_T("Software\\MediaArea.net\\MediaInfo"), false))
        {
            //Test if donation was given
            int Donated=0;
            if (Reg_User->ValueExists("Donated"))
                Donated=Reg_User->ReadInteger("Donated");
            if (Donated)
                Donate_Display=false;
        }
    }
    catch (...){}
    if (Config(_T("Donated"))==_T("1"))
        Donate_Display=false;
    if (Config(_T("Donate_Display"))==_T("0"))
        Donate_Display=false;
    if ((int64u)time(NULL)-Config(_T("Install")).To_int64u()<7*24*60*60)
        Donate_Display=false;


    delete Reg_User; Reg_User=NULL;

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
        Ztring Message=Prefs->Translate(_T("NewVersion_Question_Content"));
        Message.FindAndReplace(_T("%Version%"), NewestVersion);
        switch(Application->MessageBox(Message.c_str(), Prefs->Translate(_T("NewVersion_Question_Title")).c_str(), MB_YESNO))
        {
            case IDYES : ShellExecute(NULL, NULL, (Ztring(_T("http://mediainfo.sourceforge.net/"))+Prefs->Translate(_T("  Language_ISO639"))+_T("?NewVersionRequested=true")).c_str(), NULL, NULL, SW_SHOWNORMAL);
            default    : ;
        }
        //Inscription version connue pour pas repeter l'avertissement
        int Version_Pos=Prefs->Config.Find(_T("NewestVersion"));
        Prefs->Config(Version_Pos, 1)=NewestVersion;
        Prefs->Config.Save();
    }

    //Donation button
    if (Prefs->Config(_T("Donate_Display"))==_T("0") && Download(_T("DonateButton"))==_T("1"))
    {
        Prefs->Config(_T("Donate_Display"))=_T("1");
        Prefs->Config.Save();
    }
    if (Prefs->Config(_T("Donate_Display"))==_T("1") && Download(_T("DonateButton"))==_T("0"))
    {
        Prefs->Config(_T("Donate_Display"))=_T("0");
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
        ".264;H264File\r\n"
        ".3g2;mpeg4File\r\n"
        ".3gp;mpeg4File\r\n"
        ".3gpp;mpeg4File\r\n"
        ".aac;AACFile\r\n"
        ".ac3;AC3File\r\n"
        ".amr;AMRFile\r\n"
        ".ape;APEFile\r\n"
        ".asf;ASFFile\r\n"
        ".avi;AVIFile\r\n"
        ".bdmv;BDMVFile\r\n"
        ".bmp;BMPFile\r\n"
        ".clpi;CLPIFile\r\n"
        ".dde;DolbyEFile\r\n"
        ".divx;AVIFile\r\n"
        ".dpg;DPGFile\r\n"
        ".dts;DTSFile\r\n"
        ".dv;EAC3File\r\n"
        ".dvr;DVRFile\r\n"
        ".dvr-ms;DVRMSFile\r\n"
        ".eac3;EAC3File\r\n"
        ".evo;EVOFile\r\n"
        ".f4v;mpeg4File\r\n"
        ".flc;FLICFile\r\n"
        ".fli;FLICFile\r\n"
        ".flac;FLACFile\r\n"
        ".flv;FlashVideoFile\r\n"
        ".gvi;AVIFile\r\n"
        ".gif;GIFFile\r\n"
        ".h264;H264File\r\n"
        ".hdmov;MOVFile\r\n"
        ".ifo;IfoFile\r\n"
        ".isma;ISMAFile\r\n"
        ".ismv;ISMVFile\r\n"
        ".j2k;JPEGFile\r\n"
        ".jp2;JPEGFile\r\n"
        ".jpeg;JPEGFile\r\n"
        ".jpg;JPEGFile\r\n"
        ".m1s;mpegFile\r\n"
        ".m1t;mpegFile\r\n"
        ".m1v;mpegFile\r\n"
        ".m2p;mpegFile\r\n"
        ".m2s;mpegFile\r\n"
        ".m2t;mpegFile\r\n"
        ".m2ts;m2tsFile\r\n"
        ".m2v;mpegFile\r\n"
        ".m4a;mpeg4File\r\n"
        ".m4b;mpeg4File\r\n"
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
        ".png;PNGFile\r\n"
        ".qcp;QCPFile\r\n"
        ".qt;QTFile\r\n"
        ".ra;RMFile\r\n"
        ".rm;RMFile\r\n"
        ".rmvb;FlashVideoFile\r\n"
        ".smv;SMVFile\r\n"
        ".stl;STLFile\r\n"
        ".swf;FlashFile\r\n"
        ".tp;FlashVideoFile\r\n"
        ".trp;TRPFile\r\n"
        ".ts;tsFile\r\n"
        ".tta;TTAFile\r\n"
        ".vob;mpegFile\r\n"
        ".w64;W64File\r\n"
        ".wav;WAVFile\r\n"
        ".webM;WebMFile\r\n"
        ".wma;WMAFile\r\n"
        ".wmv;WMVFile\r\n"
        ".wv;WCFile\r\n"
        ".wvc;WVCFile\r\n"
        ".y4m;Y4MFile\r\n"
        );

    // Get the Windows version.
    DWORD Version = GetVersion();
    DWORD MajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));
    DWORD MinorVersion = (DWORD)(HIBYTE(LOWORD(Version)));


    //Removing Atak_Snajpera's "MediaInfoLite" stuff in registry (not supported by MediaArea.net, but MediaInfo official website has bug reports about this software)
    {
        ::HKEY Key;
        LONG WINAPI Result;
        Result=RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\*\\Shell\\Media Info\\Command"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\*\\Shell\\Media Info\\Command"));
        }
        Result=RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\*\\Shell\\Media Info"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\*\\Shell\\Media Info"));
        }
        Result=RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("*\\Shell\\MediaInfo\\Command"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            Result=RegDeleteKey(HKEY_CLASSES_ROOT, _T("*\\Shell\\MediaInfo\\Command"));
        }
        Result=RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("*\\Shell\\MediaInfo"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            Result=RegDeleteKey(HKEY_CLASSES_ROOT, _T("*\\Shell\\MediaInfo"));
        }
        Result=RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\MediaInfo"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\MediaInfo"));
        }
    }

    bool IsChanged=false;
    if ( MajorVersion>=6 //Windows Vista or more
     || (MajorVersion==5 && MinorVersion>=1)) //WinXP or more in 5.x family
    {
        //Removing old stuff
        for (size_t I1=0; I1<Liste.size(); I1++)
        {
            //Remove shell ext except "Folder"
            if (Reg->OpenKey(Liste(I1, 0).c_str(), false))
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
            if (Reg_User->OpenKey((Ztring(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\"))+Liste(I1, 0)+_T("\\UserChoice")).c_str(), false))
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

        //Adding/removing to SystemFileAssociations
        int32s ShellExtension=Config.Read(_T("ShellExtension")).To_int32s();
        for (size_t I1=0; I1<Liste.size(); I1++)
        {
            //Remove shell ext except "Folder"
            if (Liste(I1, 0)!=_T("Folder"))
                ExplorerShell_Edit((_T("Software\\Classes\\SystemFileAssociations\\")+Liste(I1, 0)).c_str(), ShellExtension, IsChanged, Reg_User);
        }
        ExplorerShell_Edit("Software\\Classes\\Directory", Config.Read(_T("ShellExtension_Folder")).To_int32s(), IsChanged, Reg_User);
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
                /*
                if (Reg->OpenKey(Player+_T("\\Shell\\Media Info\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\Media Info");
                    IsChanged=true;
                    Reg->CloseKey();
                }
                */

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
                        //Reg->DeleteKey(Player+"\\Shell\\Media Info"); //Delete the lod version if it exists
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
// RegDeleteKeyEx dynamic load for WinXP 32-bit
typedef void (WINAPI* _GetNativeSystemInfo_Prototype)(LPSYSTEM_INFO lpSystemInfo);
typedef LONG (WINAPI* _RegDeleteKeyEx_Prototype)(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired, DWORD Reserved);
_GetNativeSystemInfo_Prototype _GetNativeSystemInfo;
_RegDeleteKeyEx_Prototype _RegDeleteKeyEx;
HINSTANCE Kernel32;
HINSTANCE AdvAPI32;
void Dynamic_Load()
{
    Kernel32=LoadLibrary(_T("Kernel32.dll"));
    if (Kernel32)
        _GetNativeSystemInfo=(_GetNativeSystemInfo_Prototype)GetProcAddress(Kernel32 , "GetNativeSystemInfo");
    AdvAPI32=LoadLibrary(_T("AdvAPI32.dll"));
    if (AdvAPI32)
    {
        #ifdef _UNICODE
            _RegDeleteKeyEx=(_RegDeleteKeyEx_Prototype)GetProcAddress(AdvAPI32 , "RegDeleteKeyExW");
        #else //_UNICODE
            _RegDeleteKeyEx=(_RegDeleteKeyEx_Prototype)GetProcAddress(AdvAPI32 , "RegDeleteKeyExA");
        #endif //_UNICODE
    }
}

void Dynamic_Free()
{
   FreeLibrary(Kernel32);
   FreeLibrary(AdvAPI32);
}

//---------------------------------------------------------------------------
int ExplorerShell_Edit(const AnsiString &Player, bool ShellExtension, bool &IsChanged, TRegistry* Reg)
{
    ::HKEY Key;
    LONG WINAPI Result;
    Result=RegOpenKeyEx(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell\\MediaInfo\\Command").c_str()).c_str(), 0, KEY_READ|KEY_WRITE, &Key);
    if (Result!=ERROR_SUCCESS && Result!=2) //2=not found
    {
        char lpMsgBuf[1000];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMsgBuf, 1000, NULL);
        return 0;
    }

    if (Result==ERROR_SUCCESS)
    {
        //MediaInfo shell extension is known
        if (ShellExtension)
        {
            //test if good writing
            Ztring ShellExtensionToWrite=_T("\"") + Ztring(Application->ExeName.c_str()) +_T("\" \"%1\"");
            DWORD ShellExtension_Size=65536;
            ZenLib::Char ShellExtension_Chars[65536];
            Result=RegQueryValueEx(Key, NULL, 0, NULL, (LPBYTE)&ShellExtension_Chars, &ShellExtension_Size);
            if (Result!=ERROR_SUCCESS)
            {
                char lpMsgBuf[1000];
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMsgBuf, 1000, NULL);
                RegCloseKey(Key);
                return 0;
            }
            if (ShellExtensionToWrite!=ShellExtension_Chars)
            {
                //This is not the good shell extension, writing new one
                Result=RegSetValueEx(Key, NULL, 0, REG_SZ, (LPBYTE)ShellExtensionToWrite.c_str(), (ShellExtensionToWrite.size()+1)*sizeof(ZenLib::Char));
                if (Result!=ERROR_SUCCESS)
                {
                    RegCloseKey(Key);
                    return 0;
                }
                RegCloseKey(Key);
                IsChanged=true;
            }
        }
        else// if (Player!=_T("Folder"))
        {
            //Should not be here, deleting
            RegCloseKey(Key);
            Result=RegDeleteKey(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell\\MediaInfo\\Command").c_str()).c_str());
            if (Result!=ERROR_SUCCESS)
            {
                char lpMsgBuf[1000];
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMsgBuf, 1000, NULL);
                return 0;
            }
            Result=RegDeleteKey(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell\\MediaInfo").c_str()).c_str());
            if (Result!=ERROR_SUCCESS)
            {
                char lpMsgBuf[1000];
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMsgBuf, 1000, NULL);
                return 0;
            }
            Result=RegDeleteKey(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell").c_str()).c_str()); //Clear it if empty

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
            Result=RegCreateKeyEx(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell\\MediaInfo\\Command").c_str()).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &Key, NULL);
            if (Result!=ERROR_SUCCESS)
            {
                char lpMsgBuf[1000];
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMsgBuf, 1000, NULL);
                RegCloseKey(Key);
                return 0;
            }
            Ztring ShellExtensionToWrite=_T("\"") + Ztring(Application->ExeName.c_str()) +_T("\" \"%1\"");
            Result=RegSetValueEx(Key, NULL, 0, REG_SZ, (LPBYTE)ShellExtensionToWrite.c_str(), (ShellExtensionToWrite.size()+1)*sizeof(ZenLib::Char));
            if (Result!=ERROR_SUCCESS)
            {
                RegCloseKey(Key);
                return 0;
            }
            RegCloseKey(Key);
            IsChanged=true;
        }
    }

    return 1;
}

int AddKey(const Ztring &SubKey, const Ztring &Value, const Ztring &Data)
{
    ::HKEY Key;
    LONG WINAPI Result;

    Result=RegCreateKeyEx(HKEY_CURRENT_USER, SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_WOW64_64KEY, NULL, &Key, NULL);
    if (Result)
        return 0;

    Result=RegSetValueEx(Key, Value.c_str(), 0, REG_SZ, (LPBYTE)Data.c_str(), (Data.size()+1)*sizeof(ZenLib::Char));
    if (Result)
        return 0;

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::ShellToolTip()
{
    // Get the Windows version.
    DWORD Version = GetVersion();
    DWORD MajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));
    DWORD MinorVersion = (DWORD)(HIBYTE(LOWORD(Version)));

    bool IsChanged=false;
    if (!( MajorVersion>=6 //Windows Vista or more
     || (MajorVersion==5 && MinorVersion>=1))) //WinXP or more in 5.x family
        return 0; //No supported


    int32s ShellInfoTip=Config.Read(_T("ShellInfoTip")).To_int32s();

    ZtringListList Liste;
    Liste=_T(
        ".264;H264File\r\n"
        ".3g2;mpeg4File\r\n"
        ".3gp;mpeg4File\r\n"
        ".3gpp;mpeg4File\r\n"
        ".aac;AACFile\r\n"
        ".ac3;AC3File\r\n"
        ".amr;AMRFile\r\n"
        ".ape;APEFile\r\n"
        ".asf;ASFFile\r\n"
        ".avi;AVIFile\r\n"
        ".bdmv;BDMVFile\r\n"
        ".bmp;BMPFile\r\n"
        ".clpi;CLPIFile\r\n"
        ".dde;DolbyEFile\r\n"
        ".divx;AVIFile\r\n"
        ".dpg;DPGFile\r\n"
        ".dts;DTSFile\r\n"
        ".dv;EAC3File\r\n"
        ".dvr;DVRFile\r\n"
        ".dvr-ms;DVRMSFile\r\n"
        ".eac3;EAC3File\r\n"
        ".evo;EVOFile\r\n"
        ".f4v;mpeg4File\r\n"
        ".flc;FLICFile\r\n"
        ".fli;FLICFile\r\n"
        ".flac;FLACFile\r\n"
        ".flv;FlashVideoFile\r\n"
        ".gvi;AVIFile\r\n"
        ".gif;GIFFile\r\n"
        ".h264;H264File\r\n"
        ".hdmov;MOVFile\r\n"
        ".ifo;IfoFile\r\n"
        ".isma;ISMAFile\r\n"
        ".ismv;ISMVFile\r\n"
        ".j2k;JPEGFile\r\n"
        ".jp2;JPEGFile\r\n"
        ".jpeg;JPEGFile\r\n"
        ".jpg;JPEGFile\r\n"
        ".m1s;mpegFile\r\n"
        ".m1t;mpegFile\r\n"
        ".m1v;mpegFile\r\n"
        ".m2p;mpegFile\r\n"
        ".m2s;mpegFile\r\n"
        ".m2t;mpegFile\r\n"
        ".m2ts;m2tsFile\r\n"
        ".m2v;mpegFile\r\n"
        ".m4a;mpeg4File\r\n"
        ".m4b;mpeg4File\r\n"
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
        ".png;PNGFile\r\n"
        ".qcp;QCPFile\r\n"
        ".qt;QTFile\r\n"
        ".ra;RMFile\r\n"
        ".rm;RMFile\r\n"
        ".rmvb;FlashVideoFile\r\n"
        ".smv;SMVFile\r\n"
        ".stl;STLFile\r\n"
        ".swf;FlashFile\r\n"
        ".tp;FlashVideoFile\r\n"
        ".trp;TRPFile\r\n"
        ".ts;tsFile\r\n"
        ".tta;TTAFile\r\n"
        ".vob;mpegFile\r\n"
        ".w64;W64File\r\n"
        ".wav;WAVFile\r\n"
        ".webM;WebMFile\r\n"
        ".wma;WMAFile\r\n"
        ".wmv;WMVFile\r\n"
        ".wv;WCFile\r\n"
        ".wvc;WVCFile\r\n"
        ".y4m;Y4MFile\r\n"
        );

    ::HKEY Key;
    LONG WINAPI Result;
    Result=RegOpenKeyEx(HKEY_CURRENT_USER, Ztring().From_Local("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32").c_str(), 0, KEY_READ|KEY_WOW64_64KEY, &Key);
    if (Result!=ERROR_SUCCESS && Result!=2) //2=not found
        return 0;

    if (Result==ERROR_SUCCESS)
    {
        //MediaInfo shell extension is known
        if (ShellInfoTip)
        {
            //test if good writing
            Ztring DLL_Name=Application->ExeName.c_str();
            DLL_Name.resize(DLL_Name.rfind('\\')); //Removing ".exe"
            DLL_Name+=_T("\\MediaInfo_InfoTip.dll");
            Ztring ShellInfoTipToWrite=_T("\"") + DLL_Name + _T("\"");
            DWORD ShellInfoTip_Size=65536;
            ZenLib::Char ShellInfoTip_Chars[65536];
            Result=RegQueryValueEx(Key, NULL, 0, NULL, (LPBYTE)&ShellInfoTip_Chars, &ShellInfoTip_Size);
            if (Result!=ERROR_SUCCESS)
            {
                RegCloseKey(Key);
                return 0;
            }
            if (ShellInfoTipToWrite!=ShellInfoTip_Chars)
            {
                RegCloseKey(Key);

                //This is not the good shell extension, writing new one
                AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), Ztring(), ShellInfoTipToWrite);
                IsChanged=true;
            }
        }
        else
        {
            RegCloseKey(Key);

            //Retrieving if it is 64-bit OS
            bool RegDeleteKeyEx_ShouldUse=false;
            Dynamic_Load();
            if (Kernel32)
            {
                SYSTEM_INFO NativeSystemInfo;
                _GetNativeSystemInfo(&NativeSystemInfo);
                if (NativeSystemInfo.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_INTEL //32-bit
                 && _RegDeleteKeyEx)
                {
                    RegDeleteKeyEx_ShouldUse=true;
                }
            }
            if (RegDeleteKeyEx_ShouldUse)
            {
                //Should not be here, deleting
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID"), KEY_WOW64_64KEY, 0);
                Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1"), KEY_WOW64_64KEY, 0);
                for (size_t I1=0; I1<Liste.size(); I1++)
                {
                    //Remove
                    if (!Liste(I1, 0).empty())
                    {
                        Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, (_T("Software\\Classes\\")+Liste(I1, 0)+_T("\\shellex\\{00021500-0000-0000-C000-000000000046}")).c_str(), KEY_WOW64_64KEY, 0);
                        //Result=_RegDeleteKeyEx(HKEY_CURRENT_USER, (_T("Software\\Classes\\")+Liste(I1, 0)+_T("\\shellex")).c_str(), KEY_WOW64_64KEY, 0);
                    }
                }
            }
            else
            {
                //Should not be here, deleting
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID"));
                Result=RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1"));
                for (size_t I1=0; I1<Liste.size(); I1++)
                {
                    //Remove
                    if (!Liste(I1, 0).empty())
                    {
                        Result=RegDeleteKey(HKEY_CURRENT_USER, (_T("Software\\Classes\\")+Liste(I1, 0)+_T("\\shellex\\{00021500-0000-0000-C000-000000000046}")).c_str());
                        //Result=RegDeleteKey(HKEY_CURRENT_USER, (_T("Software\\Classes\\")+Liste(I1, 0)+_T("\\shellex")).c_str());
                    }
                }
            }

            IsChanged=true;
        }
    }
    else
    {
        RegCloseKey(Key);

        //MediaInfo Shell extension is not known
        if (ShellInfoTip)
        {
            //Create it
            std::string DLL_Name=Ztring(Application->ExeName.c_str()).To_Local();
            DLL_Name.resize(DLL_Name.rfind('\\')); //Removing ".exe"
            DLL_Name+="\\MediaInfo_InfoTip.dll";
            std::string ShellInfoTipToWtrite="\"" + DLL_Name +"\"";
            AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"), Ztring(), _T("MediaInfoShellExt_ Class"));
            AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), Ztring(), Ztring().From_Local(ShellInfoTipToWtrite));
            AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), _T("ThreadingModel"), _T("Apartment"));
            AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID"), _T(""), _T("MediaInfoShellExt.MediaInfoShellExt_.1"));
            AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib"), _T(""), _T("{BC1AAA9F-D8C5-4EB2-A10A-61B86B7EA77C}"));
            AddKey(_T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID"), _T(""), _T("MediaInfoShellExt.MediaInfoShellExt_"));
            AddKey(_T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_"), _T(""), _T("MediaInfoShellExt_ Class"));
            AddKey(_T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID"), _T(""), _T("{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
            AddKey(_T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer"), _T(""), _T("MediaInfoShellExt.MediaInfoShellExt_.1"));
            AddKey(_T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1"), _T(""), _T("MediaInfoShellExt_ Class"));
            AddKey(_T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID"), _T(""), _T("{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
            for (size_t I1=0; I1<Liste.size(); I1++)
            {
                //Add
                if (!Liste(I1, 0).empty())
                    AddKey(_T("Software\\Classes\\")+Liste(I1, 0)+_T("\\shellex\\{00021500-0000-0000-C000-000000000046}"), _T(""), _T("{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
            }
            IsChanged=true;
        }
    }

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


