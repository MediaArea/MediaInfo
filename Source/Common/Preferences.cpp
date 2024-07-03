/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#undef NO_WIN32_LEAN_AND_MEAN
#include <ZenLib/Conf.h>
#pragma hdrstop
#include <Forms.hpp>
#include <Registry.hpp>
#include <System.Net.HTTPClientComponent.hpp>
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
int ExplorerShell_Edit  (const AnsiString &Name, bool ShellExtension, bool &IsChanged);
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
Preferences::Preferences()
{
    //Name of folders
    FolderNames=__T(
        "Language;"
        "Sheet;"
        "Tree;"
        "Custom;"
        "Language");
    //Name of default for kind of list
    DefaultNames=__T(
        "en;"
        "Example;"
        "Example;"
        "Example;"
        "en;");

    //New version
    NewVersion_Display=false;

    //Donate
    Donated=false;
    Donate_Display=true;

    //Sponsor
    Sponsored=false;
    SponsorMessage=__T("");
    SponsorUrl=__T("");

    //Plugins
    GraphPluginURL=__T("");
    GraphPlugin64URL=__T("");
    GraphPluginVersion=__T("");
    FFmpegPluginURL=__T("");
    FFmpegPluginVersion=__T("");
}

//***************************************************************************
// Config
//***************************************************************************

//---------------------------------------------------------------------------
int Preferences::Config_Create()
{
    if (File::Exists(BaseFolder+__T("MediaInfo.cfg")))
        Config.Load(BaseFolder+__T("MediaInfo.cfg"));

    Config(__T("Version"))=MediaInfo_Version_GUI;
    Config(__T("NewestVersion"))=MediaInfo_Version_GUI;
    Config(__T("Install")).From_Number((int64u)time(NULL));
    return Config_Save();
}

//---------------------------------------------------------------------------
int Preferences::Config_Load()
{
    int Retour=1;

    //Base folder
    TCHAR szPath[MAX_PATH];
    if(FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
        return 1;
    BaseFolder=szPath;
    BaseFolder+=__T("\\MediaInfo");
    if (!Dir::Exists(BaseFolder) || !Dir::Exists(BaseFolder+__T("\\Plugin")))
    {
        BaseFolder=Application->ExeName.c_str();
        BaseFolder=BaseFolder.substr(0, BaseFolder.rfind(__T("\\"))+1);
        if (!Dir::Exists(BaseFolder+__T("Plugin")))
            Dir::Create(BaseFolder+__T("Plugin"));
        BaseFolder+=__T("Plugin\\");
    }
    else
        BaseFolder+=__T("\\Plugin\\");

    //Directories
    for (size_t Pos=0; Pos<FolderNames.size(); Pos++)
        if (!Dir::Exists(BaseFolder+FolderNames[Pos]))
            Dir::Create(BaseFolder+FolderNames[Pos]);

    //Default language
    if (!File::Exists(BaseFolder+__T("Language\\en.csv")))
        Create(Prefs_Language, __T("en"));
    Load(Prefs_Language_English, __T("en"));

    //Configuration
    if (File::Exists(BaseFolder+__T("MediaInfo.cfg")))
        Config.Load(BaseFolder+__T("MediaInfo.cfg"));
    if (Config.size()==0 || Config(__T("Version"))!=MediaInfo_Version_GUI)
    {
        Config_Create();
        Retour=2;
    }

    RefreshFilesList(Prefs_Language);
    Load(Prefs_Language, Config(FolderNames[Prefs_Language]));
	{
		//Now we prioritize language files in the app dir instead of the language files in the user dir
		//TODO: forbid edit of default language files
		ZenLib::Ztring BaseFolderOld=BaseFolder;
		BaseFolder=Application->ExeName.c_str();
		BaseFolder=BaseFolder.substr(0, BaseFolder.rfind(__T("\\"))+1);
		if (!Dir::Exists(BaseFolder+__T("Plugin")))
			Dir::Create(BaseFolder+__T("Plugin"));
		BaseFolder+=__T("Plugin\\");
		Load(Prefs_Language, Config(FolderNames[Prefs_Language]));
		BaseFolder=BaseFolderOld;
	}
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
    if (Retour!=2 && Config(__T("CheckUpdate"))==__T("1"))
        Prefs->InternetCheck();

    //New version menu
    if (!Prefs->Config(__T("Version")).empty() && Prefs->Config(__T("Version"))!=Prefs->Config(__T("NewestVersion")))
        Prefs->NewVersion_Display=true;

    //Donate display
    TRegistry* Reg_User=new TRegistry;
    try
    {
        if (Reg_User->OpenKey(__T("Software\\MediaArea.net\\MediaInfo"), false))
        {
            //Test if donation was given
            if (Reg_User->ValueExists("Donated"))
                Donated=Reg_User->ReadInteger("Donated");
            if (Donated)
                Donate_Display=false;
        }
    }
    catch (...){}
    try
    {
        if (Reg_User->OpenKey(__T("Software\\MediaArea\\MediaInfo"), false))
        {
            //Test if donation was given
            if (Reg_User->ValueExists("Donated"))
                Donated=Reg_User->ReadInteger("Donated");
            if (Donated)
                Donate_Display=false;
        }
    }
    catch (...){}
    if (Config(__T("Donated"))==__T("1"))
    {
        Donated=true;
        Donate_Display=false;
    }
    if (Config(__T("Donate_Display"))==__T("0"))
        Donate_Display=false;
    if ((int64u)time(NULL)-Config(__T("Install")).To_int64u()<7*24*60*60)
        Donate_Display=false;

    // Sponsor
    if (Config(__T("Sponsored"))==__T("1"))
    {
        Sponsored=true;

        Ztring Saved=Config(__T("SponsorMessage"));
        Saved.FindAndReplace(__T("\\r\\n"), __T("\r\n"), 0, Ztring_Recursive);
        SponsorMessage.Write(Saved);

        Saved=Config(__T("SponsorUrl"));
        Saved.FindAndReplace(__T("\\r\\n"), __T("\r\n"), 0, Ztring_Recursive);
        SponsorUrl.Write(Saved);
    }

    if (!Config(__T("GraphPlugin64URL")).empty())
        GraphPlugin64URL=Config(__T("GraphPlugin64URL"));

    if (!Config(__T("GraphPluginURL")).empty())
        GraphPluginURL=Config(__T("GraphPluginURL"));

    if (!Config(__T("GraphPluginVersion")).empty())
        GraphPluginVersion=Config(__T("GraphPluginVersion"));

    if (!Config(__T("FFmpegPluginURL")).empty())
        FFmpegPluginURL=Config(__T("FFmpegPluginURL"));

    if (!Config(__T("FFmpegPluginVersion")).empty())
        FFmpegPluginVersion=Config(__T("FFmpegPluginVersion"));

    delete Reg_User; Reg_User=NULL;

    return Retour;
}

//---------------------------------------------------------------------------
int Preferences::Config_Save()
{
    if (Config(__T("Version")).empty()) Config(__T("Version"))=MediaInfo_Version_GUI;
    if (Config(__T("NewestVersion")).empty()) Config(__T("NewestVersion"))=MediaInfo_Version_GUI;
    if (Config(__T("Install")).empty()) Config(__T("Install")).From_Number((int64u)time(NULL));
    if (Config(__T("Output")).empty()) Config(__T("Output"))=__T("Basic");
    if (Config(__T("Language")).empty()) Config(__T("Language"))=__T("en");
    if (Config(__T("Sheet")).empty()) Config(__T("Sheet"))=__T("Example");
    if (Config(__T("Tree")).empty()) Config(__T("Tree"))=__T("Example");
    if (Config(__T("Custom")).empty()) Config(__T("Custom"))=__T("Example");
    if (Config(__T("CheckUpdate")).empty()) Config(__T("CheckUpdate"))=__T("1");
    if (Config(__T("ShellExtension")).empty()) Config(__T("ShellExtension"))=__T("1");
    if (Config(__T("ShellExtension_Folder")).empty()) Config(__T("ShellExtension_Folder"))=__T("0");
    if (Config(__T("ShellInfoTip")).empty()) Config(__T("ShellInfoTip"))=__T("0");
	if (Config(__T("RememberWindowPosition")).empty()) Config(__T("RememberWindowPosition"))=__T("0");
	if (Config(__T("RememberWindowDimensions")).empty()) Config(__T("RememberWindowDimensions"))=__T("1");
    if (Config(__T("ShowToolBar")).empty()) Config(__T("ShowToolBar"))=__T("1");
    if (Config(__T("ShowMenu")).empty()) Config(__T("ShowMenu"))=__T("1");
    if (Config(__T("CloseAllAuto")).empty()) Config(__T("CloseAllAuto"))=__T("1");
    if (Config(__T("FirstInstall")).empty()) Config(__T("FirstInstall")).From_Number((int64u)time(NULL));
    if (Config(__T("Donated")).empty()) Config(__T("Donated"))=__T("0");
    if (Config(__T("Donate_Display")).empty()) Config(__T("Donate_Display"))=__T("1");
    if (Config(__T("Sponsored")).empty()) Config(__T("Sponsored"))=__T("0");
    if (Config(__T("Theme")).empty()) Config(__T("Theme"))=__T("0");

    HANDLE Temp=CreateFile((BaseFolder+__T("MediaInfo.cfg")).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (Temp==INVALID_HANDLE_VALUE)
    {
        //Base folder
        TCHAR szPath[MAX_PATH];
        if(FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
            return 1;
        Ztring NewBaseFolder=szPath;
        NewBaseFolder+=__T("\\MediaInfo");
        if (!Dir::Exists(NewBaseFolder) || !Dir::Exists(NewBaseFolder+__T("\\Plugin")))
        {
            Dir::Create(BaseFolder);

            Ztring From=BaseFolder+__T('\0');
            Ztring To=NewBaseFolder+__T("\\Plugin\0");

            SHFILEOPSTRUCT s = { 0 };
            s.hwnd = NULL;
            s.wFunc = FO_COPY;
            s.fFlags = FOF_SILENT;
            s.pFrom = From.c_str();
            s.pTo = To.c_str();
            SHFileOperation(&s);
        }

        BaseFolder=NewBaseFolder+__T("\\Plugin\\");
    }
    else
        CloseHandle(Temp);

    return Config.Save(BaseFolder+__T("MediaInfo.cfg"));;
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
            Details[KindOfList]=MediaInfoNameSpace::MediaInfo::Option_Static(__T("Language_Get")).c_str();
            if (Name!=__T("en"))
                for (size_t Pos=0; Pos<Details[KindOfList].size(); Pos++)
                    Details[KindOfList](Pos, 1)=__T("");
            break;
        case Prefs_Sheet :
            Details[KindOfList]=__T(
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
            Details[KindOfList]=__T(
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
    Details[KindOfList].Save(BaseFolder+FolderNames[KindOfList]+__T("\\")+Name+__T(".csv"));

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::Copy(Prefs_t KindOfList, const ZenLib::Ztring &From, const ZenLib::Ztring &To)
{
    return File::Copy(BaseFolder+FolderNames[KindOfList]+__T("\\")+From+__T(".csv"), BaseFolder+__T("MediaInfo.")+FolderNames[KindOfList]+__T(".")+To+__T(".csv"));
}

//---------------------------------------------------------------------------
int Preferences::Load(Prefs_t KindOfList, const ZenLib::Ztring &Name)
{
    //Test if the file exists
    if (!File::Exists(BaseFolder+FolderNames[KindOfList]+__T("\\")+Name+__T(".csv")))
        return -1;

    Details[KindOfList].Load(BaseFolder+FolderNames[KindOfList]+__T("\\")+Name+__T(".csv"));

    return 1;
}

//---------------------------------------------------------------------------
int Preferences::Remove(Prefs_t KindOfList, const ZenLib::Ztring &Name)
{
    Ztring Lang; if (KindOfList==Prefs_Custom) Lang=Config(__T("Language"))+__T("."); //Special case : if Custom; we add Language in the file name
    return File::Delete(BaseFolder+FolderNames[KindOfList]+__T("\\")+Lang+Name+__T(".csv"));
}

//***************************************************************************
// FilesLists
//***************************************************************************

//---------------------------------------------------------------------------
int Preferences::RefreshFilesList(Prefs_t KindOfList)
{
    //Default available?
    if (!File::Exists(BaseFolder+FolderNames[KindOfList]+__T("\\")+DefaultNames[KindOfList]+__T(".csv")))
        Create(KindOfList, DefaultNames[KindOfList]);

    //Listing
    FilesList[KindOfList]=Dir::GetAllFileNames(BaseFolder+FolderNames[KindOfList]+__T("\\")+__T("*.csv"));
    if (KindOfList==Prefs_Language)
    {
        //Special case : Languages, should show the name of language in the local version
        FilesList[Prefs_Language_List].clear();
        for (size_t Pos=0; Pos<FilesList[KindOfList].size(); Pos++)
        {
            ZtringListListF Lang;
            Lang.Load(FilesList[KindOfList][Pos]);
            Ztring Local=Lang(__T("  Language_Name"));
            if (Local.size()==0)
                Local=FilesList[KindOfList][Pos].SubString(BaseFolder+FolderNames[KindOfList]+__T("\\"), __T(".csv"));
            if (Local.find(__T("(Chris)"))==Ztring::npos) //quick method for no more showing Chris translation (deprecated)
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
        FilesList[KindOfList][Pos]=FilesList[KindOfList][Pos].SubString(BaseFolder+FolderNames[KindOfList]+__T("\\"), __T(".csv"));

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

    Ztring Url = Ztring(__T("https://MediaArea.net/mediainfo_check/changelog_"))+MediaInfo_Version_GUI+__T(".bin");
    TNetHTTPClient* Client = new TNetHTTPClient(NULL);
    Client->Asynchronous = false;
    Client->AllowCookies = false;
    Client->HandleRedirects = true;
    _di_IHTTPResponse Response;

    try
    {
        Response = Client->Get(Url.c_str());
    }
    catch(...)
    {
        delete Client;
        return;
    }

    if (Response->GetStatusCode()>=400)
    {
        delete Client;
        return;
    }

    Ztring Z = Ztring().From_Unicode(Response->ContentAsString().c_str());

    delete Client;

    if (Z.empty())
    return;

    ZtringListList Download(Z);

    //Verification de la version
    Ztring Version=Prefs->Config.FindValue(__T("NewestVersion"), 1);
    Ztring NewestVersion=Download(__T("NewVersion"));
    if (NewestVersion!=__T("") && NewestVersion>Version)
    {
        //Affichage d'un message
        ZtringListList C3=Download.SubSheet(__T("NewMessage"));
        int Pos=C3.Find(Prefs->Config(__T("Language")), 1);
        if (Pos==-1)
            Pos=C3.Find(__T("en"), 1);
        if (Pos==-1)
            return;
        C3(Pos, 2).FindAndReplace(__T("\\r\\n"), __T("\r\n"));
        Ztring Message=Prefs->Translate(__T("NewVersion_Question_Content"));
        Message.FindAndReplace(__T("%Version%"), NewestVersion);
        switch(Application->MessageBox(Message.c_str(), Prefs->Translate(__T("NewVersion_Question_Title")).c_str(), MB_YESNO))
        {
            case IDYES : ShellExecute(NULL, NULL, (Ztring(__T("https://mediaarea.net/"))+Prefs->Translate(__T("  Language_ISO639"))+__T("/MediaInfo?NewVersionRequested=true")).c_str(), NULL, NULL, SW_SHOWNORMAL);
            default    : ;
        }
        //Inscription version connue pour pas repeter l'avertissement
        int Version_Pos=Prefs->Config.Find(__T("NewestVersion"));
        Prefs->Config(Version_Pos, 1)=NewestVersion;
        Prefs->Config_Save();
    }

    //Donation button
    if (Prefs->Config(__T("Donate_Display"))==__T("0") && Download(__T("DonateButton"))==__T("1"))
    {
        Prefs->Config(__T("Donate_Display"))=__T("1");
        Prefs->Config_Save();
    }
    if (Prefs->Config(__T("Donate_Display"))==__T("1") && Download(__T("DonateButton"))==__T("0"))
    {
        Prefs->Config(__T("Donate_Display"))=__T("0");
        Prefs->Config_Save();
    }

    //Sponsored
    Prefs->Config(__T("Sponsored"))=__T("0");
    Prefs->Config(__T("SponsorMessage"))=__T("");
    Prefs->Config(__T("SponsorUrl"))=__T("");

    ZtringListList Sponsor=Download.SubSheet(__T("ShowSponsor"));
    unsigned int En=Sponsor.Find(__T("en"), 1);
    if (En!=(unsigned int)-1 && Sponsor[En](2)!=__T("") && Sponsor[En](3)!=__T(""))
    {
        Prefs->Config(__T("Sponsored"))=__T("1");
        Ztring Message;
        Ztring Url;
        for (size_t Pos=0; Pos<Sponsor.size(); Pos++)
        {
            if (Sponsor[Pos](1)!=__T(""))
            {
                if (Sponsor[Pos](2)!=__T(""))
                    Message+=(Message.empty()?__T(""):__T("\\r\\n"))+Sponsor[Pos](1)+__T(";")+Sponsor[Pos](2);
                if (Sponsor[Pos](3)!=__T(""))
                    Url+=(Url.empty()?__T(""):__T("\\r\\n"))+Sponsor[Pos](1)+__T(";")+Sponsor[Pos](3);
            }
        }
        Prefs->Config(__T("SponsorMessage"))=Message.Quote();
        Prefs->Config(__T("SponsorUrl"))=Url.Quote();
    }
    Prefs->Config_Save();

    //Plugins
    Ztring GraphPlugin64URL=Download(__T("GraphPlugin64URL"));
    if (!GraphPlugin64URL.empty())
    {
        Prefs->Config(__T("GraphPlugin64URL"))=GraphPlugin64URL;
        Prefs->Config_Save();
    }

    Ztring GraphPluginURL=Download(__T("GraphPluginURL"));
    if (!GraphPluginURL.empty())
    {
        Prefs->Config(__T("GraphPluginURL"))=GraphPluginURL;
        Prefs->Config_Save();
    }

    Ztring GraphPluginVersion=Download(__T("GraphPluginVersion"));
    if (!GraphPluginVersion.empty())
    {
        Prefs->Config(__T("GraphPluginVersion"))=GraphPluginVersion;
        Prefs->Config_Save();
    }

    Ztring FFmpegPluginURL=Download(__T("FFmpegPluginURL"));
    if (!FFmpegPluginURL.empty())
    {
        Prefs->Config(__T("FFmpegPluginURL"))=FFmpegPluginURL;
        Prefs->Config_Save();
    }

    Ztring FFmpegPluginVersion=Download(__T("FFmpegPluginVersion"));
    if (!FFmpegPluginVersion.empty())
    {
        Prefs->Config(__T("FFmpegPluginVersion"))=FFmpegPluginVersion;
        Prefs->Config_Save();
    }

    //Chargement de pages
    ZtringListList Pages=Download.SubSheet(__T("Url"));
    for (size_t Pos=0; Pos<Pages.size(); Pos++)
    {
        //Doit-on charger ou pas?
        if (Pages.Read(Pos, 1)==__T("1"))
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

    ZtringListList List;
    List=__T(
        ".264;H264File\r\n"
        ".3g2;mpeg4File\r\n"
        ".3ga;mpeg4File\r\n"
        ".3gp;mpeg4File\r\n"
        ".3gpa;mpeg4File\r\n"
        ".3gpp;mpeg4File\r\n"
        ".aa3;ATRACFile\r\n"
        ".aac;AACFile\r\n"
        ".aacp;AACFile\r\n"
        ".adts;AACFile\r\n"
        ".ac3;AC3File\r\n"
        ".act;WAVFile\r\n"
        ".aif;AIFFFile\r\n"
        ".aifc;AIFFFile\r\n"
        ".aiff;AIFFFile\r\n"
        ".amr;AMRFile\r\n"
        ".ape;APEFile\r\n"
        ".asf;ASFFile\r\n"
        ".at3;ATRACFile\r\n"
        ".au;AUFile\r\n"
        ".aud;AUDFile\r\n"
        ".aue;AUEFile\r\n"
        ".avi;AVIFile\r\n"
        ".bdmv;BDMVFile\r\n"
        ".bmp;BMPFile\r\n"
        ".bms;BMPFile\r\n"
        ".braw;BRAWFile\r\n"
        ".caf;CAFFile\r\n"
        ".clpi;CLPIFile\r\n"
        ".dde;DolbyEFile\r\n"
        ".divx;AVIFile\r\n"
        ".dpg;DPGFile\r\n"
        ".dff;DFFFile\r\n"
        ".dsf;DSFFile\r\n"
        ".dts;DTSFile\r\n"
        ".dtshd;DTSFile\r\n"
        ".dv;EAC3File\r\n"
        ".dvr;DVRFile\r\n"
        ".dvr-ms;DVRMSFile\r\n"
        ".eac3;EAC3File\r\n"
        ".evo;EVOFile\r\n"
        ".f4a;mpeg4File\r\n"
        ".f4b;mpeg4File\r\n"
        ".f4v;mpeg4File\r\n"
        ".fla;FLACFile\r\n"
        ".flc;FLICFile\r\n"
        ".fli;FLICFile\r\n"
        ".flac;FLACFile\r\n"
        ".flv;FlashVideoFile\r\n"
        ".gvi;AVIFile\r\n"
        ".gif;GIFFile\r\n"
        ".gis;GIFFile\r\n"
        ".h264;H264File\r\n"
        ".h3d;JPEGFile\r\n"
        ".hdmov;MOVFile\r\n"
        ".ifo;IfoFile\r\n"
        ".ism;ISMFile\r\n"
        ".isma;ISMAFile\r\n"
        ".ismv;ISMVFile\r\n"
        ".j2k;JPEGFile\r\n"
        ".jp2;JPEGFile\r\n"
        ".jpeg;JPEGFile\r\n"
        ".jpg;JPEGFile\r\n"
        ".jps;JPEGFile\r\n"
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
        ".mk3d;MK3DFile\r\n"
        ".mka;MKAFile\r\n"
        ".mks;MKSFile\r\n"
        ".mkv;MKVFile\r\n"
        ".mlp;MLPFile\r\n"
        ".mod;modFile\r\n"
        ".mov;MOVFile\r\n"
        ".mp+;mpcFile\r\n"
        ".mp2;mp3File\r\n"
        ".mp3;mp3File\r\n"
        ".mp4;mpeg4File\r\n"
        ".mpc;mpcFile\r\n"
        ".mpd;mpdFile\r\n"
        ".mpe;mpegFile\r\n"
        ".mpeg;mpegFile\r\n"
        ".mpg;mpegFile\r\n"
        ".mpgv;mpegFile\r\n"
        ".mpgx;mpegFile\r\n"
        ".mpls;MPLSFile\r\n"
        ".mpm;mpegFile\r\n"
        ".mpo;JPEGFile\r\n"
        ".mpv;mpegFile\r\n"
        ".mts;mpegFile\r\n"
        ".mxf;MXFFile\r\n"
        ".oga;OGAFile\r\n"
        ".ogg;OGGFile\r\n"
        ".ogm;OGMFile\r\n"
        ".ogv;OGVFile\r\n"
        ".ogx;OGXFile\r\n"
        ".oma;ATRACFile\r\n"
        ".opus;OpusFile\r\n"
        ".png;PNGFile\r\n"
        ".pns;PNGFile\r\n"
        ".qcp;QCPFile\r\n"
        ".qt;QTFile\r\n"
        ".ra;RMFile\r\n"
        ".rm;RMFile\r\n"
        ".rmvb;FlashVideoFile\r\n"
        ".shn;ShortenFile\r\n"
        ".smv;SMVFile\r\n"
        ".spdif;SPDIFFile\r\n"
        ".spx;SpeexFile\r\n"
        ".stl;STLFile\r\n"
        ".swf;FlashFile\r\n"
        ".tak;TakVideoFile\r\n"
        ".thd;THDFile\r\n"
        ".thd+ac3;THDFile\r\n"
        ".tif;TIFFFile\r\n"
        ".tiff;TIFFFile\r\n"
        ".tmf;mpegFile\r\n"
        ".tp;TPFile\r\n"
        ".trec;mpeg4File\r\n"
        ".trp;TRPFile\r\n"
        ".ts;tsFile\r\n"
        ".tta;TTAFile\r\n"
        ".ty;mpegFile\r\n"
        ".vob;mpegFile\r\n"
        ".vqf;VQFFile\r\n"
        ".vro;VROFile\r\n"
        ".w64;W64File\r\n"
        ".wav;WAVFile\r\n"
        ".webM;WebMFile\r\n"
        ".wma;WMAFile\r\n"
        ".wmv;WMVFile\r\n"
        ".wtv;WTVFile\r\n"
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
        Result=RegOpenKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\*\\Shell\\Media Info\\Command"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\*\\Shell\\Media Info\\Command"));
        }
        Result=RegOpenKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\*\\Shell\\Media Info"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\*\\Shell\\Media Info"));
        }
        Result=RegOpenKeyEx(HKEY_CLASSES_ROOT, __T("*\\Shell\\MediaInfo\\Command"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            RegDeleteKey(HKEY_CLASSES_ROOT, __T("*\\Shell\\MediaInfo\\Command"));
        }
        Result=RegOpenKeyEx(HKEY_CLASSES_ROOT, __T("*\\Shell\\MediaInfo"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            RegDeleteKey(HKEY_CLASSES_ROOT, __T("*\\Shell\\MediaInfo"));
        }
        Result=RegOpenKeyEx(HKEY_CURRENT_USER, __T("Software\\MediaInfo"), 0, KEY_READ, &Key);
        if (Result==0)
        {
            RegCloseKey(Key);
            RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\MediaInfo"));
        }
    }

    bool IsChanged=false;
    if ( MajorVersion>=6 //Windows Vista or more
     || (MajorVersion==5 && MinorVersion>=1)) //WinXP or more in 5.x family
    {
        //Removing old stuff
        for (size_t I1=0; I1<List.size(); I1++)
        {
            //Remove shell ext except "Folder"
            if (Reg->OpenKey(List(I1, 0).c_str(), false))
            {
                //test if extension is known
                AnsiString Player;
                try {Player=Reg->ReadString("");} catch (...){}
                Reg->CloseKey();

                //Test if old Media Info shell extension is known
                if (Player!="" && Reg->OpenKey(Player+__T("\\Shell\\Media Info\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\Media Info");
                    IsChanged=true;
                    Reg->CloseKey();
                }

                //Test if MediaInfo shell extension is known
                if (Player!="" && Reg->OpenKey(Player+__T("\\Shell\\MediaInfo\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
                    IsChanged=true;
                    Reg->CloseKey();
                }
            }

            //Remove shell ext except "Folder" (user part)
            if (Reg_User->OpenKey((Ztring(__T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\"))+List(I1, 0)+__T("\\UserChoice")).c_str(), false))
            {
                //test if extension is known
                AnsiString Player;
                try {Player=Reg_User->ReadString("Progid");} catch (...){}
                Reg_User->CloseKey();

                //Test if MediaInfo shell extension is known
                if (Player!="" && Reg->OpenKey(Player+__T("\\Shell\\MediaInfo\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\MediaInfo");
                    IsChanged=true;
                    Reg->CloseKey();
                }
            }
        }
        ExplorerShell_Edit("SystemFileAssociations\\audio", 0, IsChanged);
        ExplorerShell_Edit("SystemFileAssociations\\Directory.Audio", 0, IsChanged);
        ExplorerShell_Edit("SystemFileAssociations\\Directory.Video", 0, IsChanged);
        ExplorerShell_Edit("SystemFileAssociations\\video", 0, IsChanged);

        //Adding/removing to SystemFileAssociations
        int32s ShellExtension=Config.Read(__T("ShellExtension")).To_int32s();
        for (size_t I1=0; I1<List.size(); I1++)
        {
            //Remove shell ext except "Folder"
            if (List(I1, 0)!=__T("Folder"))
                ExplorerShell_Edit((__T("Software\\Classes\\SystemFileAssociations\\")+List(I1, 0)).c_str(), ShellExtension, IsChanged);
        }
        ExplorerShell_Edit("Software\\Classes\\Directory", Config.Read(__T("ShellExtension_Folder")).To_int32s(), IsChanged);
    }
    else
    {
        int32s ShellExtension=Config.Read(__T("ShellExtension")).To_int32s();
        for (size_t I1=0; I1<List.size(); I1++)
        {
            if (List(I1, 0)==__T("Folder"))
                ShellExtension=Config.Read(__T("ShellExtension_Folder")).To_int32s();

            //Open (or create) a extension. Create only if Shell extension is wanted
            if (Reg->OpenKey(List(I1, 0).c_str(), ShellExtension))
            {
                //test if extension is known
                AnsiString Player;
                try {Player=Reg->ReadString("");} catch (...){}
                if (Player=="")
                {
                    //extension not known, will use our default
                    Player=List(I1, 1).c_str();
                    try {Reg->WriteString(__T(""), Player);} catch (...){}
                    IsChanged=true;
                }
                Reg->CloseKey();

                if (List(I1, 0)==__T("Folder"))
                    Player="Folder";

                //Test if old Media Info shell extension is known
                /*
                if (Reg->OpenKey(Player+__T("\\Shell\\Media Info\\Command"), false))
                {
                    //Should not be here, deleting
                    Reg->CloseKey();
                    Reg->DeleteKey(Player+"\\Shell\\Media Info");
                    IsChanged=true;
                    Reg->CloseKey();
                }
                */

                //Test if MediaInfo shell extension is known
                if (Reg->OpenKey(Player+__T("\\Shell\\MediaInfo\\Command"), false))
                {
                    //MediaInfo shell extension is known
                    if (ShellExtension)
                    {
                        //test if good writing
                        AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                        AnsiString ShellExtension;
                        try {ShellExtension=Reg->ReadString("");} catch (...){}
                        if (ShellExtension!=ShellExtensionToWtrite)
                        {
                            //This is not the good shell extension, writing new one
                            try {Reg->WriteString(__T(""), ShellExtensionToWtrite);} catch (...){}
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
                        Reg->OpenKey(Player+__T("\\Shell\\MediaInfo\\Command"), true);
                        AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                        try {Reg->WriteString(__T(""), ShellExtensionToWtrite);} catch (...){}
                        Reg->CloseKey();
                        IsChanged=true;
                    }
                }
            }

            //Open (or create) a extension (user). Create only if Shell extension is wanted
            Ztring A=List(I1, 0);
            if (Reg_User->OpenKey((Ztring(__T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\"))+List(I1, 0)+__T("\\UserChoice")).c_str(), false))
            {
                //test if extension is known
                AnsiString Player;
                try {Player=Reg_User->ReadString("Progid");} catch (...){}
                Reg_User->CloseKey();

                //Test if MediaInfo shell extension is known
                if (Player!="")
                {
                    if (Reg->OpenKey(Player+__T("\\Shell\\MediaInfo\\Command"), false))
                    {
                        //MediaInfo shell extension is known
                        if (ShellExtension)
                        {
                            //test if good writing
                            AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                            AnsiString ShellExtension;
                            try {ShellExtension=Reg->ReadString("");} catch (...){}
                            if (ShellExtension!=ShellExtensionToWtrite)
                            {
                                //This is not the good shell extension, writing new one
                                try {Reg->WriteString(__T(""), ShellExtensionToWtrite);} catch (...){}
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
                            Reg->OpenKey(Player+__T("\\Shell\\MediaInfo\\Command"), true);
                            AnsiString ShellExtensionToWtrite="\"" + Application->ExeName +"\" \"%1\"";
                            try {Reg->WriteString(__T(""), ShellExtensionToWtrite);} catch (...){}
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
    Kernel32=LoadLibrary(__T("Kernel32.dll"));
    if (Kernel32)
        _GetNativeSystemInfo=(_GetNativeSystemInfo_Prototype)GetProcAddress(Kernel32 , "GetNativeSystemInfo");
    AdvAPI32=LoadLibrary(__T("AdvAPI32.dll"));
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
int ExplorerShell_Edit(const AnsiString &Player, bool ShellExtension, bool &IsChanged)
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
            Ztring ShellExtensionToWrite=__T("\"") + Ztring(Application->ExeName.c_str()) +__T("\" \"%1\"");
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
                IsChanged=true;
            }
            RegCloseKey(Key);
        }
        else// if (Player!=__T("Folder"))
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
            Ztring ShellExtensionToWrite=__T("\"") + Ztring(Application->ExeName.c_str()) +__T("\" \"%1\"");
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

    //Icon part
    Result=RegOpenKeyEx(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell\\MediaInfo").c_str()).c_str(), 0, KEY_READ|KEY_WRITE, &Key);
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
            Ztring ShellExtensionToWrite=__T("\"") + Ztring(Application->ExeName.c_str()) +__T("\"");
            DWORD ShellExtension_Size=65536;
            ZenLib::Char ShellExtension_Chars[65536];
            Result=RegQueryValueEx(Key, __T("Icon"), 0, NULL, (LPBYTE)&ShellExtension_Chars, &ShellExtension_Size);
            if (Result!=ERROR_SUCCESS || ShellExtensionToWrite!=ShellExtension_Chars)
            {
                //This is not the good shell extension, writing new one
                Result=RegSetValueEx(Key, __T("Icon"), 0, REG_SZ, (LPBYTE)ShellExtensionToWrite.c_str(), (ShellExtensionToWrite.size()+1)*sizeof(ZenLib::Char));
                if (Result!=ERROR_SUCCESS)
                {
                    RegCloseKey(Key);
                    return 0;
                }
                IsChanged=true;
            }
            RegCloseKey(Key);
        }
        else// if (Player!=__T("Folder"))
        {
            //Should not be here, deleting
            RegCloseKey(Key);
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
    }
    else
    {
        //MediaInfo Shell extension is not known
        if (ShellExtension)
        {
            //Create it
            Result=RegCreateKeyEx(HKEY_CURRENT_USER, Ztring().From_Local((Player+"\\Shell\\MediaInfo").c_str()).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &Key, NULL);
            if (Result!=ERROR_SUCCESS)
            {
                char lpMsgBuf[1000];
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, Result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMsgBuf, 1000, NULL);
                RegCloseKey(Key);
                return 0;
            }
            Ztring ShellExtensionToWrite=__T("\"") + Ztring(Application->ExeName.c_str()) +__T("\"");
            Result=RegSetValueEx(Key, __T("Icon"), 0, REG_SZ, (LPBYTE)ShellExtensionToWrite.c_str(), (ShellExtensionToWrite.size()+1)*sizeof(ZenLib::Char));
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


    int32s ShellInfoTip=Config.Read(__T("ShellInfoTip")).To_int32s();

    ZtringListList List;
    List=__T(
        ".264;H264File\r\n"
        ".3g2;mpeg4File\r\n"
	".3ga;mpeg4File\r\n"
        ".3gp;mpeg4File\r\n"
	".3gpa;mpeg4File\r\n"
        ".3gpp;mpeg4File\r\n"
        ".aac;AACFile\r\n"
        ".ac3;AC3File\r\n"
        ".aiff;AIFFFile\r\n"
        ".amr;AMRFile\r\n"
        ".ape;APEFile\r\n"
        ".asf;ASFFile\r\n"
        ".aud;AUDFile\r\n"
        ".aue;AUEFile\r\n"
        ".avi;AVIFile\r\n"
        ".bdmv;BDMVFile\r\n"
        ".bmp;BMPFile\r\n"
        ".braw;BRAWFile\r\n"
        ".clpi;CLPIFile\r\n"
        ".dde;DolbyEFile\r\n"
        ".divx;AVIFile\r\n"
        ".dpg;DPGFile\r\n"
        ".dsf;DFFFile\r\n"
        ".dsf;DSFFile\r\n"
        ".dts;DTSFile\r\n"
        ".dtshd;DTSFile\r\n"
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
        ".ism;ISMFile\r\n"
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
        ".mlp;MLPFile\r\n"
        ".mk3d;MK3DFile\r\n"
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
        ".mpd;mpdFile\r\n"
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
        ".ogx;OGXFile\r\n"
        ".opus;OpusFile\r\n"
        ".png;PNGFile\r\n"
        ".qcp;QCPFile\r\n"
        ".qt;QTFile\r\n"
        ".ra;RMFile\r\n"
        ".rm;RMFile\r\n"
        ".rmvb;FlashVideoFile\r\n"
        ".smv;SMVFile\r\n"
        ".stl;STLFile\r\n"
        ".swf;FlashFile\r\n"
        ".tak;TAKFile\r\n"
        ".tif;TIFFFile\r\n"
        ".tiff;TIFFFile\r\n"
        ".tp;TPFile\r\n"
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
            DLL_Name+=__T("\\MediaInfo_InfoTip.dll");
            Ztring ShellInfoTipToWrite=__T("\"") + DLL_Name + __T("\"");
            DWORD ShellInfoTip_Size=65536;
            ZenLib::Char ShellInfoTip_Chars[65536];
            RegQueryValueEx(Key, NULL, 0, NULL, (LPBYTE)&ShellInfoTip_Chars, &ShellInfoTip_Size);
            if (Result!=ERROR_SUCCESS)
            {
                RegCloseKey(Key);
                return 0;
            }
            if (ShellInfoTipToWrite!=ShellInfoTip_Chars)
            {
                RegCloseKey(Key);

                //This is not the good shell extension, writing new one
                AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), Ztring(), ShellInfoTipToWrite);
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
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID"), KEY_WOW64_64KEY, 0);
                _RegDeleteKeyEx(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1"), KEY_WOW64_64KEY, 0);
                for (size_t I1=0; I1<List.size(); I1++)
                {
                    //Remove
                    if (!List(I1, 0).empty())
                    {
                        _RegDeleteKeyEx(HKEY_CURRENT_USER, (__T("Software\\Classes\\")+List(I1, 0)+__T("\\shellex\\{00021500-0000-0000-C000-000000000046}")).c_str(), KEY_WOW64_64KEY, 0);
                        //_RegDeleteKeyEx(HKEY_CURRENT_USER, (__T("Software\\Classes\\")+List(I1, 0)+__T("\\shellex")).c_str(), KEY_WOW64_64KEY, 0);
                    }
                }
            }
            else
            {
                //Should not be here, deleting
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID"));
                RegDeleteKey(HKEY_CURRENT_USER, __T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1"));
                for (size_t I1=0; I1<List.size(); I1++)
                {
                    //Remove
                    if (!List(I1, 0).empty())
                    {
                        RegDeleteKey(HKEY_CURRENT_USER, (__T("Software\\Classes\\")+List(I1, 0)+__T("\\shellex\\{00021500-0000-0000-C000-000000000046}")).c_str());
                        //RegDeleteKey(HKEY_CURRENT_USER, (__T("Software\\Classes\\")+List(I1, 0)+__T("\\shellex")).c_str());
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
            AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}"), Ztring(), __T("MediaInfoShellExt_ Class"));
            AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), Ztring(), Ztring().From_Local(ShellInfoTipToWtrite));
            AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\InprocServer32"), __T("ThreadingModel"), __T("Apartment"));
            AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\ProcID"), __T(""), __T("MediaInfoShellExt.MediaInfoShellExt_.1"));
            AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\TypeLib"), __T(""), __T("{BC1AAA9F-D8C5-4EB2-A10A-61B86B7EA77C}"));
            AddKey(__T("Software\\Classes\\CLSID\\{869C14C8-1830-491F-B575-5F9AB40D2B42}\\VersionIndependentProgID"), __T(""), __T("MediaInfoShellExt.MediaInfoShellExt_"));
            AddKey(__T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_"), __T(""), __T("MediaInfoShellExt_ Class"));
            AddKey(__T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CLSID"), __T(""), __T("{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
            AddKey(__T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_\\CurVer"), __T(""), __T("MediaInfoShellExt.MediaInfoShellExt_.1"));
            AddKey(__T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1"), __T(""), __T("MediaInfoShellExt_ Class"));
            AddKey(__T("Software\\Classes\\MediaInfoShellExt.MediaInfoShellExt_.1\\CLSID"), __T(""), __T("{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
            for (size_t I1=0; I1<List.size(); I1++)
            {
                //Add
                if (!List(I1, 0).empty())
                    AddKey(__T("Software\\Classes\\")+List(I1, 0)+__T("\\shellex\\{00021500-0000-0000-C000-000000000046}"), __T(""), __T("{869C14C8-1830-491F-B575-5F9AB40D2B42}"));
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
    if (Name==__T("SponsorMessage") || Name==__T("SponsorUrl"))
    {
        Ztring Language=Translate(__T("  Language_ISO639"));
        if (Name==__T("SponsorMessage"))
        {
            int Index=SponsorMessage.Find(Language, 0);
            if (Index==-1 || SponsorMessage(Index)(1)==__T(""))
                Index=SponsorMessage.Find(__T("en"), 0);

            return SponsorMessage(Index)(1);
        }
        else if (Name==__T("SponsorUrl"))
        {
            int Index=SponsorUrl.Find(Language, 0);
            if (Index==-1 || SponsorUrl(Index)(1)==__T(""))
                Index=SponsorUrl.Find(__T("en"), 0);

            return SponsorUrl(Index)(1);
        }
    }

    size_t Pos=Details[Prefs_Language].Find(Name, 0, 0, __T("=="), Ztring_CaseSensitive);

    //If not in the language, search for English language
    if (Pos==-1 || Details[Prefs_Language][Pos].size()<2)
        return Details[Prefs_Language_English](Name);
    else
        return Details[Prefs_Language](Pos)(1);
}
