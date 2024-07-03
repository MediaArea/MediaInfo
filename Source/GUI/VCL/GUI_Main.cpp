/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#include <Math.hpp>
#pragma hdrstop
#include "GUI/VCL/GUI_Main.h"
#ifndef MEDIAINFOGUI_PREFS_NO
    #include "GUI/VCL/GUI_Preferences.h"
#endif
#ifndef MEDIAINFOGUI_EXPORT_NO
    #include "GUI/VCL/GUI_Export.h"
#endif
#ifndef MEDIAINFOGUI_UPDATE_NO
    #include "GUI/VCL/GUI_Web.h"
#endif
#ifndef MEDIAINFOGUI_ABOUT_NO
    #include "GUI/VCL/GUI_About.h"
#endif
#ifndef MEDIAINFOGUI_PLUGIN_NO
    #include "GUI/VCL/GUI_Plugin.h"
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainF *MainF;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
#include "Common/Utils.h"
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
#include <ZenLib/File.h>
#include <ZenLib/FileName.h>
#include <ZenLib/OS_Utils.h>

#include <filesystem> // To remove when "Quick temporary fix for translations after an update" is removed

using namespace MediaInfoNameSpace;
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//Page specific
int Page_Position; //Index of MediaInfo file
//-Page_Easy
const int EASY_STREAM_MAX=3;
TGroupBox* Page_Easy_X           [Stream_Max][EASY_STREAM_MAX];
TLabel*    Page_Easy_X_Codec     [Stream_Max][EASY_STREAM_MAX];
TButton*   Page_Easy_X_Web       [Stream_Max][EASY_STREAM_MAX];
Ztring        Page_Easy_X_Web_Url   [Stream_Max][EASY_STREAM_MAX];
TLabel*    Page_Easy_X_List      [Stream_Max];
const int EASY_TAG_MAX=5;
TLabel*    Page_Easy_X_Tag       [EASY_TAG_MAX];
//-Page_Sheet
TComboBox* Page_Sheet_X          [Stream_Max];
TButton*   Page_Sheet_X_Web      [Stream_Max];
Ztring        Page_Sheet_X_Web_Url  [Stream_Max];
//-Page_System
bool Page_System_Already=false; //Already done?
int Page_System_Sheet_ColumnToSort=0;
ZtringListList Page_System_Video; //List of Video codecs
ZtringListList Page_System_Audio; //List of Audio codecs
ZtringListList Page_System_Text; //List of Text codecs

//Temporary file used for HTML presentation
Ztring FileName_Temp;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
extern const ZenLib::Char* MEDIAINFO_TITLE;
const size_t Title_Pos=156; //TODO: Position of Title in General.csv, should shange this...
MediaInfoList *I;

//---------------------------------------------------------------------------

//***************************************************************************
// Private functions
//***************************************************************************

//Functions for theme handling
//---------------------------------------------------------------------------
bool __fastcall TMainF::WindowsDarkModeEnabled()
{
    //Check Windows dark mode
    TRegistry* Reg_AppsUseLightTheme = new TRegistry;
    bool DarkModeEnabled = false;
    try {
        if (Reg_AppsUseLightTheme->OpenKey(
                __T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
                false))
        {
            if (Reg_AppsUseLightTheme->ValueExists("AppsUseLightTheme"))
                if (!Reg_AppsUseLightTheme->ReadInteger("AppsUseLightTheme"))
                    DarkModeEnabled = true;
            Reg_AppsUseLightTheme->CloseKey();
        }
    } catch (...) {
    }
    return DarkModeEnabled;
}

void __fastcall TMainF::ConfigTheme()
{
    switch(Prefs->Config(__T("Theme")).To_int32s()) {
        case 0:
            M_Options_Theme_System->Checked = true;
            if (WindowsDarkModeEnabled()) {
                TStyleManager::TrySetStyle(DARK_MODE_STYLE, false);
            } else {
                TStyleManager::TrySetStyle(LIGHT_MODE_STYLE, false);
            }
            break;
        case 1:
            M_Options_Theme_Light->Checked = true;
            TStyleManager::TrySetStyle(LIGHT_MODE_STYLE, false);
            break;
        case 2:
            M_Options_Theme_Dark->Checked = true;
            TStyleManager::TrySetStyle(DARK_MODE_STYLE, false);
            break;
        default:
            Prefs->Config(__T("Theme")) = __T("0");
            M_Options_Theme_System->Checked = true;
            if (WindowsDarkModeEnabled()) {
                TStyleManager::TrySetStyle(DARK_MODE_STYLE, false);
            } else {
                TStyleManager::TrySetStyle(LIGHT_MODE_STYLE, false);
            }
            break;
    }
}
//---------------------------------------------------------------------------

//Function to inject CSS styles into HTML documents to workaround GUI HTML rendering issues
//---------------------------------------------------------------------------
std::wstring __fastcall TMainF::InjectHTMLStyle(const wchar_t* HTMLDocument) {

    auto InsertText = [](std::wstring Text, const wchar_t* InsertPointText, const wchar_t* TextToInsert) -> std::wstring {
        auto InsertionPoint = std::search(Text.begin(), Text.end(), InsertPointText, InsertPointText + wcslen(InsertPointText));
        if (InsertionPoint != Text.end()) {
            size_t InsertionPos = InsertionPoint - Text.begin() + wcslen(InsertPointText);
            Text.insert(InsertionPos, TextToInsert);
        }
        return Text;
    };

    std::wstring modifiedHTML(HTMLDocument);
    const wchar_t* StyleContent = L"";

    //IE Engine workarounds
    if (TStyleManager::ActiveStyle == TStyleManager::Style[DARK_MODE_STYLE]) //app is currently in dark mode
        StyleContent = L"<meta http-equiv='X-UA-Compatible' content='IE=edge'><style>body { background-color: #121212; color: #FFFFFF; } table { border:1px solid blue; }</style>";
    else
        StyleContent = L"<meta http-equiv='X-UA-Compatible' content='IE=edge'><style>table { border:1px solid navy; }</style>";
    modifiedHTML = InsertText(modifiedHTML, L"<head>", StyleContent);

    return modifiedHTML;
}

//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TMainF::TMainF(TComponent* Owner)
    : TForm(Owner)
{
    //Configuration of internal pointers
    //-Easy
    for (int KindOfStream=0; KindOfStream<Stream_Max; KindOfStream++)
        for (int Pos=0; Pos<EASY_STREAM_MAX; Pos++)
        {
            Page_Easy_X      [KindOfStream][Pos]=NULL;
            Page_Easy_X_Codec[KindOfStream][Pos]=NULL;
            Page_Easy_X_Web  [KindOfStream][Pos]=NULL;
        }
    Page_Easy_X      [Stream_General][0]=Page_Easy_G1;
    Page_Easy_X      [Stream_Video  ][0]=Page_Easy_V1;
    Page_Easy_X      [Stream_Audio  ][0]=Page_Easy_A1;
    Page_Easy_X      [Stream_Audio  ][1]=Page_Easy_A2;
    Page_Easy_X      [Stream_Text   ][0]=Page_Easy_T1;
    Page_Easy_X      [Stream_Text   ][1]=Page_Easy_T2;
    Page_Easy_X      [Stream_Text   ][2]=Page_Easy_T3;
    Page_Easy_X_Codec[Stream_General][0]=Page_Easy_G1_Codec;
    Page_Easy_X_Codec[Stream_Video][0]  =Page_Easy_V1_Codec;
    Page_Easy_X_Codec[Stream_Audio][0]  =Page_Easy_A1_Codec;
    Page_Easy_X_Codec[Stream_Audio][1]  =Page_Easy_A2_Codec;
    Page_Easy_X_Codec[Stream_Text][0]   =Page_Easy_T1_Codec;
    Page_Easy_X_Codec[Stream_Text][1]   =Page_Easy_T2_Codec;
    Page_Easy_X_Codec[Stream_Text][2]   =Page_Easy_T3_Codec;
    Page_Easy_X_Web  [Stream_General][0]=Page_Easy_G1_Web;
    Page_Easy_X_Web  [Stream_Video][0]  =Page_Easy_V1_Web;
    Page_Easy_X_Web  [Stream_Audio][0]  =Page_Easy_A1_Web;
    Page_Easy_X_Web  [Stream_Audio][1]  =Page_Easy_A2_Web;
    Page_Easy_X_Web  [Stream_Text][0]   =Page_Easy_T1_Web;
    Page_Easy_X_Web  [Stream_Text][1]   =Page_Easy_T2_Web;
    Page_Easy_X_Web  [Stream_Text][2]   =Page_Easy_T3_Web;
    Page_Easy_X_List [Stream_General]   =Page_Easy_G1_Codec;
    Page_Easy_X_List [Stream_Video]     =Page_Easy_General_List_V;
    Page_Easy_X_List [Stream_Audio]     =Page_Easy_General_List_A;
    Page_Easy_X_List [Stream_Text]      =Page_Easy_General_List_T;
    Page_Easy_X_List [Stream_Other]  =Page_Easy_General_List_C;
    Page_Easy_X_Tag[0]=Page_Easy_General_Tag1;
    Page_Easy_X_Tag[1]=Page_Easy_General_Tag2;
    Page_Easy_X_Tag[2]=Page_Easy_General_Tag3;
    Page_Easy_X_Tag[3]=Page_Easy_General_Tag4;
    Page_Easy_X_Tag[4]=Page_Easy_General_Tag5;
    //-Sheet
    Page_Sheet_X[Stream_General]=Page_Sheet_G;
    Page_Sheet_X[Stream_Video]=Page_Sheet_V;
    Page_Sheet_X[Stream_Audio]=Page_Sheet_A;
    Page_Sheet_X[Stream_Text]=Page_Sheet_T;
    Page_Sheet_X[Stream_Other]=Page_Sheet_C;
    Page_Sheet_X_Web[Stream_General]=Page_Sheet_G_Web;
    Page_Sheet_X_Web[Stream_Video]=Page_Sheet_V_Web;
    Page_Sheet_X_Web[Stream_Audio]=Page_Sheet_A_Web;
    Page_Sheet_X_Web[Stream_Text]=Page_Sheet_T_Web;
    Page_Sheet_X_Web[Stream_Other]=Page_Sheet_C_Web;

    //Footer
    //Note: If Height is set after Parent, button will be too small on high-DPI
    //      If Font is set before Parent, button text will be too large on high-DPI
    Footer_Button=new TButton(this);
    Footer_Button->Height=32;
    Footer_Button->Parent=Page;
    Footer_Button->Font->Size=12;
    Footer_Button->Align=alBottom;
    Footer_Button->OnClick=&Footer_ButtonClick;
    Footer_Button->Visible=false;

    //Configuration of properties
    Page_Position=-1;
    Caption=MEDIAINFO_TITLE;

    //Configuration of MediaInfoLib
    if (I == NULL)
        I = new MediaInfoList;

    //Load GUI preferences
    GUI_Configure();

    //File(s) in command line
    #ifdef UNICODE
        if (IsWin9X())
        {
            for (int I1 = 1; I1 <= ParamCount(); I1++)
                I->Open(ParamStr(I1).c_str());
        }
        else
        {
            int argc;
            MediaInfoNameSpace::Char** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
            for (int I1 = 1; I1 < argc; I1++)
                I->Open(argv[I1]);
        }
    #else
        for (int I1 = 1; I1 < ParamCount(); I1++)
            I->Open(Ztring().From_Local(ParamStr(I1).c_str()));
    #endif
}

//***************************************************************************
// GUI - Form
//***************************************************************************

//---------------------------------------------------------------------------
void __fastcall TMainF::GUI_Configure()
{
    //Load Configuration
    if (Prefs->Config_Load()==2) //Showing options if no config
    {
        #ifndef MEDIAINFOGUI_PREFS_NO
        TPreferencesF* PreferencesF=new TPreferencesF(this);
        PreferencesF->Cancel->Enabled=false;
        PreferencesF->ShowModal();
        delete PreferencesF;
        Prefs->Config_Load(); //Again...
        #endif //MEDIAINFOGUI_PREFS_NO

        //Quick temporary fix for translations after an update
        //----------------------------------------------------------------------------------------------
        //Copies updated language files from install folder to appdata folder if appdata folder exists,
        //overwriting existing files
        //----------------------------------------------------------------------------------------------
        namespace fs = std::filesystem;
        fs::path appdata_path = std::getenv("APPDATA");
        fs::path language_path = appdata_path / "MediaInfo\\Plugin\\Language";
        ZenLib::Ztring BaseFolder;
        BaseFolder=Application->ExeName.c_str();
        fs::path language_source_path = BaseFolder.substr(0, BaseFolder.rfind(__T("\\"))).c_str();
        language_source_path = language_source_path / "Plugin\\Language";
        if (fs::exists(language_path) && fs::is_directory(language_path)) {
            try {
                for (const auto& entry : fs::directory_iterator(language_source_path)) {
                    fs::path current = entry.path();
                    fs::path target = language_path / current.filename();
                    if (fs::is_regular_file(current)) {
                        fs::copy_file(current, target, fs::copy_options::overwrite_existing);
                    }
                }
                //ShowMessage("Success updating translations!");
            } catch (const fs::filesystem_error& e) {
                ShowMessage("Error updating translations in %APPDATA%!");
            }
        }
        //----------------------------------------------------------------------------------------------
    }

    // Removed, no more needed
    //#ifndef MEDIAINFOGUI_UPDATE_NO
    //Web Updates
    //if (Prefs->Config(__T("CheckUpdate"))==__T("1"))
    //{
    //    WebF=new TWebF(this);
    //    WebF->Execute();
    //}
    //#endif //MEDIAINFOGUI_UPDATE_NO

    //Menu - View
         if (Prefs->Config(__T("Output"))==__T("Basic")) {M_View_EasyClick(NULL); M_View_Easy->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("Sheet")) {M_View_SheetClick(NULL); M_View_Sheet->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("Tree")) {M_View_TreeClick(NULL); M_View_Tree->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("Text")) {M_View_TextClick(NULL); M_View_Text->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("HTML")) {M_View_HTMLClick(NULL); M_View_HTML->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("XML")) {M_View_XMLClick(NULL); M_View_XML->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("MIXML")) {M_View_XMLClick(NULL); M_View_XML->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("JSON")) {M_View_JSONClick(NULL); M_View_JSON->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("Graph_Svg")) {M_View_Graph_SvgClick(NULL); M_View_Graph_Svg->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7_Strict")) {M_View_MPEG7_StrictClick(NULL); M_View_MPEG7_Strict->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7_Relaxed")) {M_View_MPEG7_RelaxedClick(NULL); M_View_MPEG7_Relaxed->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7_Extended")) {M_View_MPEG7_ExtendedClick(NULL); M_View_MPEG7_Extended->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("PBCore_1.2")) {M_View_PBCoreClick(NULL); M_View_PBCore->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("PBCore_2.0")) {M_View_PBCore2Click(NULL); M_View_PBCore2->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.5")) {M_View_EBUCore_1_5Click(NULL); M_View_EBUCore_1_5->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.6")) {M_View_EBUCore_1_6Click(NULL); M_View_EBUCore_1_6->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_ps")) {M_View_EBUCore_1_8_psClick(NULL); M_View_EBUCore_1_8_ps->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_sp")) {M_View_EBUCore_1_8_spClick(NULL); M_View_EBUCore_1_8_sp->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_ps_json")) {M_View_EBUCore_1_8_ps_jsonClick(NULL); M_View_EBUCore_1_8_ps_json->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_sp_json")) {M_View_EBUCore_1_8_sp_jsonClick(NULL); M_View_EBUCore_1_8_sp_json->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("FIMS_1.1")) {M_View_FIMS_1_1Click(NULL); M_View_FIMS_1_1->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("FIMS_1.2")) {M_View_FIMS_1_2Click(NULL); M_View_FIMS_1_2->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("FIMS_1.3")) {M_View_FIMS_1_3Click(NULL); M_View_FIMS_1_3->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("NISO_Z39.87")) {M_View_NISO_Z39_87Click(NULL); M_View_NISO_Z39_87->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("reVTMD")) {M_View_reVTMDClick(NULL); M_View_reVTMD->Checked=true;}
    else if (Prefs->Config(__T("Output"))==__T("Custom")) {M_View_CustomClick(NULL); M_View_Custom->Checked=true;}

    //Menu - Options
    M_Options_ShowToolBar->Checked=Prefs->Config(__T("ShowToolBar")).To_int32s();
    M_Options_ShowMenu->Checked=Prefs->Config(__T("ShowMenu")).To_int32s();
    M_Options_CloseAllAuto->Checked=Prefs->Config(__T("CloseAllAuto")).To_int32s();

    //Toolbar
    ToolBar->Visible=M_Options_ShowToolBar->Checked;

    // FFmpeg
    #ifndef MEDIAINFOGUI_PLUGIN_NO
    Ztring InstallFolder = Application->ExeName.c_str();
    InstallFolder = InstallFolder.substr(0, InstallFolder.rfind(__T("\\")) + 1);

    if (Prefs->Config(__T("EnableFfmpeg"), 1) == __T("1") && !File::Exists(InstallFolder + __T("\\Plugin\\FFmpeg\\version.txt"))) //Try to install plugin
    {
        TPluginF* P = new TPluginF(this, PLUGIN_FFMPEG);
        if (P->Configure())
            P->ShowModal();
        delete P;

        if (!File::Exists(InstallFolder + __T("\\Plugin\\FFmpeg\\version.txt")))
            MessageBox(NULL, __T("An error occured, please download and install the plugin manually from the MediaInfo download page."), __T("Error"), MB_OK);
    }
    #endif

    //Translation
    Translate();

    //Opt-out from styling dialogs and use native Windows dialogs for dark mode as well
    TStyleManager::SystemHooks = TStyleManager::SystemHooks -
                                 (TStyleManager::TSystemHooks() << TStyleManager::TSystemHook::shDialogs);

    //Configure theme
    ConfigTheme();
  
    //Set monospaced font to Cascadia Mono SemiBold if available on current system
    if (Screen->Fonts->IndexOf("Cascadia Mono SemiBold") != -1) {
        TFont* MonoFont = new TFont;
        MonoFont->Name = "Cascadia Mono SemiBold";
        MonoFont->Size = 10;
        Page_Text_Text->Font = MonoFont;
        Page_Custom_Text->Font = MonoFont;
        Page_Sheet_Text->Font = MonoFont;
    }

	//Set window size and position
	// todo: move all property assignments into FormCreate
	// note: VCL version? It's been having built-in DPI support since long ago. And it should be Monitor instead of Screen.
	// OSVERSIONINFO osvi;
	// ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	// osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	// GetVersionEx(&osvi);
	// int DPI;
	// if (osvi.dwMajorVersion >= 10 && (osvi.dwMajorVersion > 10 || osvi.dwMinorVersion > 0 || osvi.dwBuildNumber >= 14939))
	//	   DPI=GetDpiForWindow(WindowHandle);
	// else
	// 	   DPI=GetDeviceCaps(GetDC(NULL), LOGPIXELSX);
	// float DPIScale=static_cast<float>(DPI)/96;
	//
	// float fpUnscaledMonitorWidth=Monitor->Width/DPIScale;
	// float fpUnscaledMonitorHeight=Monitor->Height/DPIScale;

	if (Prefs->Config(__T("RememberWindowDimensions")) == __T("1")) {

		int nPrefWidth = Width;
		int nPrefHeight = Height;

		if (Prefs->Config(__T("FormWidth")).IsNumber())
			nPrefWidth = Prefs->Config(__T("FormWidth")).To_int32s();
		if (Prefs->Config(__T("FormHeight")).IsNumber())
			nPrefHeight = Prefs->Config(__T("FormHeight")).To_int32s();

		nPrefWidth = Min(Monitor->WorkareaRect.Width(), nPrefWidth);
		nPrefHeight = Min(Monitor->WorkareaRect.Height(), nPrefHeight);

		SetBounds(Left, Top, nPrefWidth, nPrefHeight);

	}
	else {

		// using VCL
		float fpUnscaledMonitorWidth = Monitor->Width / ScaleFactor;
		float fpUnscaledMonitorHeight = Monitor->Height / ScaleFactor;

		Width=500;
		Height=400;
		if (fpUnscaledMonitorWidth>=1024)
			Width=700;
		if (fpUnscaledMonitorWidth>=1280)
			Width=830;
		if (fpUnscaledMonitorHeight>=768)
			Height=500;
		if (fpUnscaledMonitorHeight>=1024)
			Height=600;
		if (fpUnscaledMonitorHeight>=1440)
			Height=900;
		Width *= ScaleFactor;
		Height *= ScaleFactor;
		// note: just use poScreenCenter
		// Left=(Screen->Width-Width)/2;
		// Top=(Screen->Height-Height)/2;

	}
	if (Prefs->Config(__T("RememberWindowPosition")) == __T("1")) {

		int nPrefTop = 0;
		int nPrefLeft = 0;

		if (Prefs->Config(__T("FormTop")).IsNumber())
			nPrefTop = Prefs->Config(__T("FormTop")).To_int32s();
		if (Prefs->Config(__T("FormLeft")).IsNumber())
			nPrefLeft = Prefs->Config(__T("FormLeft")).To_int32s();

		nPrefTop = Max(nPrefTop, 0);
		nPrefTop = Min(nPrefTop,  Monitor->WorkareaRect.Height() - Height);
		nPrefLeft = Max(nPrefLeft, 0);
		nPrefLeft = Min(nPrefLeft, Monitor->WorkareaRect.Width() - Width);
		SetBounds(nPrefLeft, nPrefTop, Width, Height);

		Position = poDesigned;

	}
	else {
		Position = poScreenCenter;
	}

    //Refresh global
    FormResize(NULL);
    Refresh();

}

//---------------------------------------------------------------------------
void __fastcall TMainF::FormClose(TObject *Sender, TCloseAction &Action)
{
    //Delete temp HTML file on close
    if (FileName_Temp!=__T(""))
        File::Delete(FileName_Temp);

	bool bCallPrefsSave = false;
	if (Prefs->Config(__T("RememberWindowPosition")) == __T("1")) {
		Prefs->Config(__T("FormTop")).From_Number((int)this->Top);
		Prefs->Config(__T("FormLeft")).From_Number((int)this->Left);
		bCallPrefsSave = true;
	}
	if (Prefs->Config(__T("RememberWindowDimensions")) == __T("1")) {
		Prefs->Config(__T("FormWidth")).From_Number((int)this->Width);
		Prefs->Config(__T("FormHeight")).From_Number((int)this->Height);
		bCallPrefsSave = true;
	}

	if (bCallPrefsSave)
		Prefs->Config.Save();

    //The form is closed and all allocated memory for the form is freed.
    Action = caFree;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::FormDestroy(TObject *Sender)
{
    delete Prefs; Prefs=NULL;
    delete I; I=NULL;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::FormResize(TObject *Sender)
{
    //If triggered on application close
    if(Prefs==NULL)
        return;

    //Main View
    Page->Left  =(ToolBar->Visible?ToolBar->Width:0)-2;
    Page->Width =ClientWidth-Page->Left+2;
    Page->Height=ClientHeight-Page->Top+(Page->TabHeight*1.15);

    //Page - Easy
         if (Page->ActivePage==Page_Easy)
    {
        //Main
        Page_Easy_File->Width=Page_Easy->ClientWidth-Page_Easy_FileSelect->Width;
        Page_Easy_FileSelect->Left=Page_Easy->ClientWidth-Page_Easy_FileSelect->Width;
        Page_Easy_FileSelect->Height=Page_Easy_File->Height;

        //GroupBoxes
        for (int KindOfStream=0; KindOfStream<Stream_Max; KindOfStream++)
        {
            //How many visible streams?
            int StreamCount=0;
            while (Page_Easy_X[KindOfStream][StreamCount]!=NULL && Page_Easy_X[KindOfStream][StreamCount]->Visible)
                StreamCount++;
            //Resize
            for (int StreamPos=0; StreamPos<StreamCount; StreamPos++)
            {
                Page_Easy_X    [KindOfStream][StreamPos]->Left =(Page_Sheet->ClientWidth-2)/StreamCount*StreamPos;
                Page_Easy_X    [KindOfStream][StreamPos]->Width=(Page_Sheet->ClientWidth-2)/StreamCount;
                Page_Easy_X_Web[KindOfStream][StreamPos]->Width=(Page_Sheet->ClientWidth-2)/StreamCount-8;
            }
        }

        //General list
        int Divider=Page_Easy_X_Tag[0]->Caption.Length()>0?2:1;
        for (int Pos=0; Pos<Stream_Max; Pos++)
            if (Page_Easy_X_List[Pos])
                Page_Easy_X_List[Pos]->Width=Page_Easy_G1->Width/Divider-6;

        //Tags
        for (int Pos=0; Pos<EASY_TAG_MAX; Pos++)
            if (Page_Easy_X_Tag[Pos])
            {
                Page_Easy_X_Tag[Pos]->Left=Page_Easy_G1->Width/2;
                Page_Easy_X_Tag[Pos]->Width=Page_Easy_G1->Width/2-6;
            }

        //Bottom
        Page_Easy_DifferentView->Left=Page_Sheet->ClientWidth-Page_Easy_DifferentView->Width;
        Page_Easy_Note->Width=Page_Easy_DifferentView->Left-Page_Easy_Note->Left;
    }

    //Page - Sheet
    else if (Page->ActivePage==Page_Sheet)
    {
        //Sheet
        Page_Sheet_Sheet->Width =Page_Sheet->ClientWidth;
        Page_Sheet_Sheet->Height=Page_Sheet_Panel1->Height;

        //Columns
        //-Calculate total width
        int Total=0;
        for (int Pos=0; Pos<Page_Sheet_Sheet->ColCount; Pos++)
        {
            Ztring Z1=__T("Column"); Z1+=Ztring::ToZtring(Pos);
            Total+=Prefs->Details[Prefs_Sheet](Z1, 4).To_int32s();
        }
        if (Total==0)
            Total=100;
        //Set column widthes
        for (int Pos=0; Pos<Page_Sheet_Sheet->ColCount; Pos++)
        {
            Ztring Z1=__T("Column"); Z1+=Ztring::ToZtring(Pos);
            Page_Sheet_Sheet->ColWidths[Pos]=Prefs->Details[Prefs_Sheet](Z1, 4).To_int32s()*Page_Sheet_Sheet->ClientWidth/Total-1;
        }

        //Streams
        for (int KindOfStream=0; KindOfStream<Stream_Max; ++KindOfStream)
        {
            if (Page_Sheet_X[KindOfStream])
            {
                if (KindOfStream!=0 && Page_Sheet_X[KindOfStream-1])
                    Page_Sheet_X[KindOfStream]->Top =Page_Sheet_X[KindOfStream-1]->Top+Page_Sheet_X[KindOfStream-1]->Height;
                else
                    Page_Sheet_X[KindOfStream]->Top =0; //1st stream, need reference
                Page_Sheet_X[KindOfStream]->Width   =Page_Sheet->ClientWidth-Page_Sheet_X_Web[KindOfStream]->Width;
                if (Page_Sheet_X_Web[KindOfStream])
                {
                    Page_Sheet_X_Web[KindOfStream]->Top =Page_Sheet_X[KindOfStream]->Top+1;
                    Page_Sheet_X_Web[KindOfStream]->Left=Page_Sheet_X[KindOfStream]->Width;
                }
                if (!Page_Sheet_X[KindOfStream+1]) //reached the bottom
                {
                    //Bottom
                    Page_Sheet_Text->Width =Page_Sheet->ClientWidth;
                    Page_Sheet_Text->Top   =Page_Sheet_X[KindOfStream]->Top+Page_Sheet_X[KindOfStream]->Height;
                    Page_Sheet_Text->Height=Page_Sheet_Panel2->Height-(Page_Sheet_X[KindOfStream]->Top+Page_Sheet_X[KindOfStream]->Height);
                }
            }
        }
    }

    //Page - Tree
    else if (Page->ActivePage==Page_Tree)
    {
        Page_Tree_Tree->Top   =-1;
        Page_Tree_Tree->Left  =-1;
        Page_Tree_Tree->Width =Page_Tree->ClientWidth+2;
        Page_Tree_Tree->Height=Page_Tree->ClientHeight+2;
    }

    //Page - Text
    else if (Page->ActivePage==Page_Text)
    {
        Page_Text_Text->Top   =-1;
        Page_Text_Text->Left  =-1;
        Page_Text_Text->Width =Page_Text->ClientWidth+2;
        Page_Text_Text->Height=Page_Text->ClientHeight+2;
    }

    //Page - HTML
    else if (Page->ActivePage==Page_HTML)
    {
        Page_HTML_HTML->Top   =-1;
        Page_HTML_HTML->Left  =-1;
        Page_HTML_HTML->Width =Page_HTML->ClientWidth+2;
        Page_HTML_HTML->Height=Page_HTML->ClientHeight+2;
    }

    //Page - Custom
    else if (Page->ActivePage==Page_Custom)
    {
        Page_Custom_Text->Top   =-1;
        Page_Custom_Text->Left  =-1;
        Page_Custom_Text->Width =Page_Custom->ClientWidth+2;
        Page_Custom_Text->Height=Page_Custom->ClientHeight+2;
        if (Page_Custom_HTML->Visible)
            Page_Custom_HTML->Left=Page_Custom_Text->Left;
        else
            Page_Custom_HTML->Left=ClientWidth; //Bug? I can't have the canvas hidden
        Page_Custom_HTML->Top   =Page_Custom_Text->Top;
        Page_Custom_HTML->Width =Page_Custom_Text->Width;
        Page_Custom_HTML->Height=Page_Custom_Text->Height;
    }

    //Page - System
    else if (Page->ActivePage==Page_System)
    {
        Page_System_Sheet->Width = Page_System->ClientWidth - 2;
        Page_System_Sheet->Height = Page_System->ClientHeight - Page_System_Sheet->Top - 2;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Translate()
{
    //Menu - Language
    M_Language->Clear();
    for (size_t Pos=0; Pos<Prefs->FilesList[Prefs_Language].size(); Pos++)
    {
        TMenuItem* MenuItem=new TMenuItem(NULL);
        MenuItem->Caption=Prefs->FilesList[Prefs_Language_List](Pos).c_str(); //Special case : Languages, should show the name of language in the local version
        MenuItem->OnClick=M_LanguageClick;
        M_Language->Add(MenuItem);
    }

    //Menu
    M_File->Caption=Prefs->Translate(__T("File")).c_str();
    M_File_Open->Caption=Prefs->Translate(__T("Open")).c_str();
    M_File_Open_File->Caption=Prefs->Translate(__T("File")).c_str();
    M_File_Open_Folder->Caption=(Prefs->Translate(__T("Folder"))+__T("...")).c_str();
    if (IsWin9X())
        M_File_Open_Folder->Visible=false;
    M_File_Close->Caption=Prefs->Translate(__T("Close")).c_str();
    M_File_Close_File->Caption=Prefs->Translate(__T("File")).c_str();
    M_File_Close_All->Caption=Prefs->Translate(__T("All")).c_str();
    M_File_Export->Caption=Prefs->Translate(__T("Export")).c_str();
    M_File_Exit->Caption=Prefs->Translate(__T("Exit")).c_str();
    M_View->Caption=Prefs->Translate(__T("View")).c_str();
    M_View_Easy->Caption=Prefs->Translate(__T("Basic")).c_str();
    M_View_Sheet->Caption=Prefs->Translate(__T("Sheet")).c_str();
    M_View_Tree->Caption=Prefs->Translate(__T("Tree")).c_str();
    M_View_Text->Caption=Prefs->Translate(__T("Text")).c_str();
    M_View_HTML->Caption=Prefs->Translate(__T("HTML")).c_str();
    M_View_Custom->Caption=Prefs->Translate(__T("Custom")).c_str();
    M_View_System->Caption=Prefs->Translate(__T("Your system")).c_str();
    M_Options->Caption=Prefs->Translate(__T("Options")).c_str();
    M_Options_ShowToolBar->Caption=Prefs->Translate(__T("Show toolbar")).c_str();
    M_Options_ShowMenu->Caption=Prefs->Translate(__T("Show menu")).c_str();
    M_Options_Theme->Caption=Prefs->Translate(__T("ColorTheme")).c_str();
    M_Options_Theme_System->Caption=Prefs->Translate(__T("ColorTheme_System")).c_str();
    M_Options_Theme_Light->Caption=Prefs->Translate(__T("ColorTheme_Light")).c_str();
    M_Options_Theme_Dark->Caption=Prefs->Translate(__T("ColorTheme_Dark")).c_str();
    M_Options_CloseAllAuto->Caption=Prefs->Translate(__T("Close all before open")).c_str();
    M_Options_Preferences->Caption=Prefs->Translate(__T("Preferences")).c_str();
    M_Debug->Caption=Prefs->Translate(__T("Debug")).c_str();
    M_Debug_Header->Caption=Prefs->Translate(__T("Header file")).c_str();
    M_Debug_Advanced->Caption=Prefs->Translate(__T("Advanced mode")).c_str();
    M_Help->Caption=Prefs->Translate(__T("Help")).c_str();
    M_Help_About->Caption=Prefs->Translate(__T("About")).c_str();
    M_Help_SupportedFormats->Caption=Prefs->Translate(__T("Known formats")).c_str();
    M_Help_SupportedCodecs->Caption=Prefs->Translate(__T("Known codecs")).c_str();
    M_Help_SupportedParameters->Caption=Prefs->Translate(__T("Known parameters")).c_str();
    M_Language->Caption=Prefs->Translate(__T("Language")).c_str();

    //ToolBar
    Tool_File->Hint=Prefs->Translate(__T("File_Hint")).c_str();
    Tool_Folder->Hint=Prefs->Translate(__T("Folder_Hint")).c_str();
    if (IsWin9X())
        Tool_Folder->Visible=false;
    Tool_Export->Hint=Prefs->Translate(__T("Export_Hint")).c_str();
    Tool_Options->Hint=Prefs->Translate(__T("Options_Hint")).c_str();
    Tool_About->Hint=Prefs->Translate(__T("About_Hint")).c_str();
    Tool_View->Hint=Prefs->Translate(__T("View_Hint")).c_str();
    //TODO : Tool_View Hint
    ToolBar_View_Easy->Caption=M_View_Easy->Caption;
    ToolBar_View_Sheet->Caption=M_View_Sheet->Caption;
    ToolBar_View_Tree->Caption=M_View_Tree->Caption;
    ToolBar_View_Text->Caption=M_View_Text->Caption;
    ToolBar_View_HTML->Caption=M_View_HTML->Caption;
    ToolBar_View_Custom->Caption=M_View_Custom->Caption;
    ToolBar_View_System->Caption=M_View_System->Caption;
    ToolBar_View_ShowMenu->Caption=M_Options_ShowMenu->Caption;

    //Main interface
    Page_Easy->Caption=Prefs->Translate(__T("Basic")).c_str();
    Page_Sheet->Caption=Prefs->Translate(__T("Sheet")).c_str();
    Page_Tree->Caption=Prefs->Translate(__T("Tree")).c_str();
    Page_Text->Caption=Prefs->Translate(__T("Text")).c_str();
    Page_HTML->Caption=Prefs->Translate(__T("HTML")).c_str();
    Page_Custom->Caption=Prefs->Translate(__T("Custom")).c_str();
    Page_System->Caption=Prefs->Translate(__T("Your system")).c_str();

    //Easy interface
    Page_Easy_G1_Web->Caption =Prefs->Translate(__T("WebSite_General")).c_str();
    Page_Easy_V1_Web->Caption=Prefs->Translate(__T("WebSite_Video")).c_str();
    Page_Easy_A1_Web->Caption=Prefs->Translate(__T("WebSite_Audio")).c_str();
    Page_Easy_A2_Web->Caption=Prefs->Translate(__T("WebSite_Audio")).c_str();
    Page_Easy_T1_Web->Caption=Prefs->Translate(__T("WebSite_Text")).c_str();
    Page_Easy_T2_Web->Caption=Prefs->Translate(__T("WebSite_Text")).c_str();
    Page_Easy_T3_Web->Caption=Prefs->Translate(__T("WebSite_Text")).c_str();
    Page_Easy_G1->Caption=Prefs->Translate(__T("Container and general information")).c_str();
    Page_Easy_V1->Caption=Prefs->Translate(__T("Video1")).c_str();
    Page_Easy_A1->Caption=Prefs->Translate(__T("Audio1")).c_str();
    Page_Easy_A2->Caption=Prefs->Translate(__T("Audio2")).c_str();
    Page_Easy_T1->Caption=Prefs->Translate(__T("Text1")).c_str();
    Page_Easy_T2->Caption=Prefs->Translate(__T("Text2")).c_str();
    Page_Easy_T3->Caption=Prefs->Translate(__T("Text3")).c_str();
    Page_Easy_Note->Caption=Prefs->Translate(__T("Basic_Note")).c_str();

    //Sheet interface
    Page_Sheet_G->Hint=Prefs->Translate(__T("Format")).c_str();
    Page_Sheet_V->Hint=Prefs->Translate(__T("Video")).c_str();
    Page_Sheet_A->Hint=Prefs->Translate(__T("Audio")).c_str();
    Page_Sheet_T->Hint=Prefs->Translate(__T("Text")).c_str();
    Page_Sheet_C->Hint=Prefs->Translate(__T("Chapters")).c_str();
    Page_Sheet_Text->Hint=Prefs->Translate(__T("Stream_MoreInfo")).c_str();

    //System interface
    Page_System_Buttons_Video->Caption=Prefs->Translate(__T("Video")).c_str();
    Page_System_Buttons_Audio->Caption=Prefs->Translate(__T("Audio")).c_str();
    Page_System_Buttons_Text->Caption=Prefs->Translate(__T("Text")).c_str();
    Page_System_Sheet->Columns->Items[0]->Caption=Prefs->Translate(__T("SystemId")).c_str();
    Page_System_Sheet->Columns->Items[1]->Caption=Prefs->Translate(__T("Name")).c_str();
    Page_System_Sheet->Columns->Items[2]->Caption=Prefs->Translate(__T("More")).c_str();
    Page_System_Sheet->Columns->Items[3]->Caption=Prefs->Translate(__T("Web")).c_str();
    Page_System_Sheet->Columns->Items[4]->Caption=Prefs->Translate(__T("Supported?")).c_str();

    //Title
    FileOpenDialog1->Title=Prefs->Translate(__T("Choose files")).c_str();

    //MediaInfo
    I->Option_Static(__T("Language"), Prefs->Details[Prefs_Language].Read());

    //New version
    M_NewVersion->Caption=(__T(" | ")+Prefs->Translate(__T("NewVersion_Menu"))).c_str();
    M_NewVersion->Visible=Prefs->NewVersion_Display;

    //Sponsor
    M_Sponsor->Visible=false;
    if (Prefs->Sponsored && !Prefs->Donated && !Prefs->Translate(__T("SponsorMessage")).empty() && !Prefs->Translate(__T("SponsorMessage")).empty())
    {
        M_Sponsor->Caption =  + Prefs->Translate(__T("SponsorMessage")).c_str();
        M_Sponsor->Visible=true;
    }

    //Footer
    Footer_Button->Caption=L"Go to conformance errors and warnings glossary page";
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Refresh(TTabSheet *Page)
{
    if (Page==NULL)
        Page=this->Page->ActivePage;
    size_t FilesCount=I->Count_Get();

    if (!Prefs->Config(__T("EnableFfmpeg")).empty())
        I->Option_Static(__T("Enable_Ffmpeg"), Prefs->Config(__T("EnableFfmpeg")));

    if (!Prefs->Config(__T("DisplayCaptions")).empty())
        I->Option(__T("File_DisplayCaptions"), Prefs->Config(__T("DisplayCaptions")));

    //Easy
         if (Page==Page_Easy)
    {
        size_t ItemIndex_Save=Page_Easy_File->ItemIndex;
        Page_Easy_File->Items->Clear();
        for (size_t FilePos=0; FilePos<FilesCount; FilePos++)
            Page_Easy_File->Items->Add(I->Get(FilePos, Stream_General, 0, __T("CompleteName")).c_str());
        if (ItemIndex_Save!=-1 && ItemIndex_Save<FilesCount)
            Page_Easy_File->ItemIndex=ItemIndex_Save;
        else if (FilesCount>0)
            Page_Easy_File->ItemIndex=0;

        Page_Easy_FileChange(NULL);
    }

    //Sheet
    else if (Page==Page_Sheet)
    {
        if (FilesCount)
        {
            //Configure
            Page_Sheet_Sheet->RowCount=1+FilesCount;
            Page_Sheet_Sheet->FixedRows=1;
            Page_Sheet_Sheet->ColCount=Prefs->Details[Prefs_Sheet](__T("ColumnsCount")).To_int32s();
            for (int Pos=0; Pos<Page_Sheet_Sheet->ColCount; Pos++)
            {
                Ztring Z1=__T("Column"); Z1+=Ztring::ToZtring(Pos);
                //Searching kind of stream
                stream_t S;
                ZenLib::Char C=__T('G');
                if (Prefs->Details[Prefs_Sheet](Z1, 1).size())
                    C=Prefs->Details[Prefs_Sheet](Z1, 1)[0];
                switch (C)
                {
                  case __T('G'): S=Stream_General; break;
                  case __T('V'): S=Stream_Video; break;
                  case __T('A'): S=Stream_Audio; break;
                  case __T('T'): S=Stream_Text; break;
                  case __T('C'): S=Stream_Other; break;
                  default: S=Stream_General;
                }
                Page_Sheet_Sheet->Cells[Pos][0]=I->Get(0, S, Prefs->Details[Prefs_Sheet](Z1, 2).To_int32u(), Prefs->Details[Prefs_Sheet](Z1, 3), Info_Name_Text).c_str();
                if (C!=__T('G'))
                    Page_Sheet_Sheet->Cells[Pos][0]=WideString((Prefs->Details[Prefs_Sheet](Z1, 1)+Prefs->Details[Prefs_Sheet](Z1, 2)).c_str())+WideString(' ')+Page_Sheet_Sheet->Cells[Pos][0];
                FormResize(NULL);
            }
            //Show all available files
            for (size_t FilePos=0; FilePos<FilesCount; FilePos++)
                for (int Pos=0; Pos<Page_Sheet_Sheet->ColCount; Pos++)
                {
                    Ztring Z1=__T("Column"); Z1+=Ztring::ToZtring(Pos);
                    //Searching Stream kind
                    stream_t S;
                    ZenLib::Char C=__T('G');
                    if (Prefs->Details[Prefs_Sheet](Z1, 1).size())
                        C=Prefs->Details[Prefs_Sheet](Z1, 1)[0];
                    switch (C)
                    {
                      case __T('G'): S=Stream_General; break;
                      case __T('V'): S=Stream_Video; break;
                      case __T('A'): S=Stream_Audio; break;
                      case __T('T'): S=Stream_Text; break;
                      case __T('C'): S=Stream_Other; break;
                      default: S=Stream_General;
                    }
                    //Showing
                    Page_Sheet_Sheet->Cells[Pos][1+FilePos]=I->Get(FilePos, S, Prefs->Details[Prefs_Sheet](Z1, 2).To_int32u(), Prefs->Details[Prefs_Sheet](Z1, 3)).c_str();
                }
        }
        else
        {
            Page_Sheet_Sheet->RowCount=1;
            Page_Sheet_Sheet->ColCount=1;
            Page_Sheet_Sheet->ColWidths[0]=Page_Sheet_Sheet->ClientWidth;
            Page_Sheet_Sheet->Cells[0][0]=Prefs->Translate(__T("At least one file")).c_str();
            Page_Sheet_G->Text=__T("");
        }
        bool B;
        Page_Sheet_SheetSelectCell(Page_Sheet_Sheet, Page_Sheet_Sheet->Col, Page_Sheet_Sheet->Row, B);
    }

    //Tree
    else if (Page==Page_Tree)
    {
        bool Commplete=I->Option_Static(__T("Complete_Get"))!=__T("");
        Page_Tree_Tree->Visible=false;
        Page_Tree_Tree->Items->Clear();
        TTreeNode* Top=NULL;

        I->Option(__T("File_ExpandSubs"), __T("1"));
        for (size_t FilePos=0; FilePos<FilesCount; FilePos++)
        {
            //Pour chaque fichier
            TTreeNode* Parent=Page_Tree_Tree->Items->Add(NULL, I->Get(FilePos, Stream_General, 0, __T("CompleteName")).c_str());
            if (Top==NULL)
                Top=Parent;

            for (int StreamKind=(int)Stream_General; StreamKind<(int)Stream_Max; StreamKind++)
            {
                //Pour chaque type de flux
                Ztring StreamKindText=I->Get(FilePos, (stream_t)StreamKind, 0, __T("StreamKind/String"), Info_Text).c_str();
                unsigned StreamsCount=I->Count_Get(FilePos, (stream_t)StreamKind);
                for (size_t StreamPos=Stream_General; StreamPos<StreamsCount; StreamPos++)
                {
                    //Pour chaque stream
                    Ztring A=StreamKindText;
                    Ztring B=I->Get(FilePos, (stream_t)StreamKind, StreamPos, __T("StreamKindPos"), Info_Text).c_str();
                    if (B!=__T(""))
                    {
                        A+=__T(" #");
                        A+=B;
                    }
                    TTreeNode* Node=Page_Tree_Tree->Items->AddChild(Parent, A.c_str());

                    std::vector<TTreeNode*> Tree;
                    Tree.push_back(Node);

                    unsigned ChampsCount=I->Count_Get(FilePos, (stream_t)StreamKind, StreamPos);
                    for (size_t Champ_Pos=0; Champ_Pos<ChampsCount; Champ_Pos++)
                    {
                        //Pour chaque champ
                        Ztring A=I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text);
                        A+=I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Measure_Text);
                        /*TEST wchar_t C=I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform];
                        */
                        if ((Commplete || I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]==__T('Y')) && I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text)!=__T(""))
                        {
                            //Quoi Refresh?
                            Ztring D=I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Name_Text);
                            if (D.empty())
                                D=I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Name); //Texte n'existe pas
                            //Affichage
                            size_t Level=D.find_first_not_of(__T(' '));

                            bool Hide=false;
                            if (A==__T("Yes") && Champ_Pos+1<ChampsCount)
                                Hide=I->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos+1, Info_Name_Text).find_first_not_of(__T(' '))>Level;

                            if (Level && Level!=(size_t)-1)
                                D=D.substr(Level);

                            if(Level==Tree.size() && Tree.back()->GetLastChild())
                                Tree.push_back(Tree.back()->GetLastChild());
                            else if(Level<Tree.size()-1)
                                Tree.resize(Level+1);

                            Page_Tree_Tree->Items->AddChild(Tree.back(), Hide?D.c_str():(D + __T(": ") + A).c_str());
                        }
                    }
                    Node->Expand(false);
                }
            }
            Parent->Expand(false);
        }
        I->Option(__T("File_ExpandSubs"), __T("0"));
        if (Top)
            Top->MakeVisible();
        Page_Tree_Tree->Visible=true;

        //Specific in case of no file
        if (I->Count_Get()==0)
            Page_Tree_Tree->Items->AddChild(NULL, Prefs->Translate(__T("At least one file")).c_str());
    }

    //Text
    else if (Page==Page_Text)
    {
        if (M_Debug_Details50->Checked)
            I->Option_Static(__T("Inform"), __T("Details;0.5"));
        else if (M_Debug_Details90->Checked)
            I->Option_Static(__T("Inform"), __T("Details;0.9"));
        else if (M_Debug_Details100->Checked)
            I->Option_Static(__T("Inform"), __T("Details;1"));
        else
            I->Option_Static(__T("Inform"));

        if (!Prefs->Config(__T("InformVersion")).empty())
            I->Option_Static(__T("Inform_Version"), Prefs->Config(__T("InformVersion")));

        if (!Prefs->Config(__T("InformTimestamp")).empty())
            I->Option_Static(__T("Inform_Timestamp"), Prefs->Config(__T("InformTimestamp")));

        Page_Text_Text->Text=I->Inform().c_str();

        //Specific in case of no file
        if (FilesCount==0)
            Page_Text_Text->Text=Prefs->Translate(__T("At least one file")).c_str();
    }

    //HTML
    else if (Page==Page_HTML)
    {
        I->Option_Static(__T("Inform"), __T("HTML"));
        if (FilesCount>0)
        {
            //Creating file
            Ztring S1=I->Inform().c_str();
            File F;
            S1=InjectHTMLStyle(I->Inform().c_str());
            if (FileName_Temp==__T(""))
            {
                FileName_Temp=FileName::TempFileName_Create(__T("MI_"));
                File::Delete(FileName_Temp);
                FileName_Temp+=__T(".html");
            }
            F.Create(FileName_Temp, true);
            F.Write(S1);
            F.Close();
            //Navigate
            Page_HTML_HTML->Navigate((MediaInfoNameSpace::Char*)FileName_Temp.c_str());
        }
        else
        {
            Ztring TempA; TempA=Prefs->Translate(__T("At least one file"));
            Ztring Temp;
            Temp+=L"about:<html><head></head><body>";
            Temp+=TempA.To_Unicode();
            Temp+=L"</body></html>";
            Temp=InjectHTMLStyle(Temp.c_str());
            Page_HTML_HTML->Navigate((MediaInfoNameSpace::Char*)Temp.c_str());
        }
    }

    //Custom
    else if (Page==Page_Custom)
    {
        if (M_View_XML->Checked)
            I->Option_Static(__T("Inform"), __T("MIXML"));
        else if (M_View_JSON->Checked)
            I->Option_Static(__T("Inform"), __T("JSON"));
        else if (M_View_Graph_Svg->Checked)
        {
            if (!Prefs->Config(__T("Graph_Adm_ShowTrackUIDs")).empty())
                I->Option_Static(__T("Graph_Adm_ShowTrackUIDs"), Prefs->Config(__T("Graph_Adm_ShowTrackUIDs")));

            if (!Prefs->Config(__T("Graph_Adm_ShowChannelFormats")).empty())
                I->Option_Static(__T("Graph_Adm_ShowChannelFormats"), Prefs->Config(__T("Graph_Adm_ShowChannelFormats")));

            I->Option_Static(__T("Inform"), __T("Graph_Svg"));
        }
        else if (M_View_MPEG7_Strict->Checked)
            I->Option_Static(__T("Inform"), __T("MPEG-7_Strict"));
        else if (M_View_MPEG7_Relaxed->Checked)
            I->Option_Static(__T("Inform"), __T("MPEG-7_Relaxed"));
        else if (M_View_MPEG7_Extended->Checked)
            I->Option_Static(__T("Inform"), __T("MPEG-7_Extended"));
        else if (M_View_PBCore->Checked)
            I->Option_Static(__T("Inform"), __T("PBCore_1.2"));
        else if (M_View_PBCore2->Checked)
            I->Option_Static(__T("Inform"), __T("PBCore_2.0"));
        else if (M_View_EBUCore_1_5->Checked)
            I->Option_Static(__T("Inform"), __T("EBUCore_1.5"));
        else if (M_View_EBUCore_1_6->Checked)
            I->Option_Static(__T("Inform"), __T("EBUCore_1.6"));
        else if (M_View_EBUCore_1_8_ps->Checked)
            I->Option_Static(__T("Inform"), __T("EBUCore_1.8_ps"));
        else if (M_View_EBUCore_1_8_sp->Checked)
            I->Option_Static(__T("Inform"), __T("EBUCore_1.8_sp"));
        else if (M_View_EBUCore_1_8_ps_json->Checked)
            I->Option_Static(__T("Inform"), __T("EBUCore_1.8_ps_JSON"));
        else if (M_View_EBUCore_1_8_sp_json->Checked)
            I->Option_Static(__T("Inform"), __T("EBUCore_1.8_sp_JSON"));
        else if (M_View_FIMS_1_1->Checked)
            I->Option_Static(__T("Inform"), __T("FIMS_1.1"));
        else if (M_View_FIMS_1_2->Checked)
            I->Option_Static(__T("Inform"), __T("FIMS_1.2"));
        else if (M_View_FIMS_1_3->Checked)
            I->Option_Static(__T("Inform"), __T("FIMS_1.3"));
        else if (M_View_reVTMD->Checked)
            I->Option_Static(__T("Inform"), __T("reVTMD"));
        else if (M_View_NISO_Z39_87->Checked)
            I->Option_Static(__T("Inform"), __T("NISO_Z39.87"));
        else
            I->Option_Static(__T("Inform"), Prefs->Details[Prefs_Custom].Read());
        Ztring S1=I->Inform();
        if (S1.empty())
            S1=Prefs->Translate(__T("At least one file")).c_str();

        if (I->Option_Static(__T("Inform_Get"), __T("")) == __T("Graph_Svg"))
        {
            S1 = Ztring();

            Ztring InstallFolder = Application->ExeName.c_str();
            InstallFolder = InstallFolder.substr(0, InstallFolder.rfind(__T("\\")) + 1);

            Ztring State=I->Option_Static(__T("Info_Graph_Svg_Plugin_State"), __T(""));
            if (State == __T("0") || !File::Exists(InstallFolder+__T("\\Plugin\\Graph\\Template.html"))) //Try to install plugin
            {
                TPluginF* P = new TPluginF(this, PLUGIN_GRAPH);
                if (P->Configure())
                    P->ShowModal();
                delete P;

                State = I->Option_Static(__T("Info_Graph_Svg_Plugin_State"), __T(""));
            }

            if (State == __T("1"))
            {
                for (size_t Pos = 0; Pos < I->Count_Get(); Pos++)
                {
                    Ztring Svg = I->Inform(Pos);
                    size_t Pos2 = Svg.find(__T("<svg"));
                    if (Pos2 != std::string::npos)
                        Svg = Svg.substr(Pos);
                    S1 += (Pos2 ? __T("<br/>") : __T("")) + Svg;
                }

                if (File::Exists(InstallFolder+__T("\\Plugin\\Graph\\Template.html")))
                {
                    File F(InstallFolder+__T("\\Plugin\\Graph\\Template.html"));
                    int8u* Buffer=new int8u[(size_t)F.Size_Get()+1];
                    size_t Count=F.Read(Buffer, (size_t)F.Size_Get());
                    if (Count==ZenLib::Error)
                    {
                        delete[] Buffer; //Buffer=NULL;
                        S1=__T("Unable to load graph template");
                    }
                    else
                    {
                        Buffer[Count]=(int8u)'\0';
                        Ztring Template=Ztring().From_UTF8((char*)Buffer);
                        if (Template.FindAndReplace(__T("@SVG@"), S1)==0)
                            S1=__T("Invalid template");
                        else
                            S1=Template;
                    }
                    delete[] Buffer; //Buffer=NULL;
                }
                else
                    S1=__T("Graph template not found");
            }
            else if (State == __T("0"))
                S1 = __T("Graph plugin not installed");
            else
                S1 = State;
        }

        if (S1.size()>1 && S1[0]=='<' && S1[1]=='h')
        {
            //Supposing this is HTML
            Page_Custom_Text->Visible=false;
            Page_Custom_HTML->Visible=true;
            //Creating file
            File F;
            if (FileName_Temp==__T(""))
            {
                FileName_Temp=FileName::TempFileName_Create(__T("MI_"));
                File::Delete(FileName_Temp);
                FileName_Temp+=__T(".html");
            }
            F.Create(FileName_Temp, true);
            F.Write(S1);
            F.Close();
            //Navigate
            Page_Custom_HTML->Navigate((MediaInfoNameSpace::Char*)FileName_Temp.c_str());
            FormResize(NULL);
        }
        else
        {
            //Text
            Page_Custom_Text->Text=S1.c_str();
            Page_Custom_Text->Visible=true;
            Page_Custom_HTML->Visible=false;
        }
    }

    //System
    else if (Page==Page_System)
    {
        if (!Page_System_Already)
        {
            //Know codecs
            ZtringListList ListT=I->Option_Static(__T("Info_Codecs")).c_str();
            ZtringListList List; //TODO : replace by Ztring::Delete1
            for (size_t Pos=0; Pos<ListT.size(); Pos++)
            {
                List(Pos, 0)=ListT(Pos, 0);
                List(Pos, 1)=ListT(Pos, 1);
                List(Pos, 2)=ListT(Pos, 6);
                List(Pos, 3)=ListT(Pos, 7);
                if (ListT(Pos, 2)==__T("2CC"))
                {
                    //Format 2CC
                    while (List(Pos, 0).size()<4)
                        List(Pos, 0)=Ztring(__T("0"))+List(Pos, 0);
                }
            }
            bool Audio=false;
            bool Subtitle=false;
            for (size_t I=0; I<List.size(); I++)
            {
                if (List(I, 0)==__T("0000"))
                    Audio=true;
                if (List(I, 0)==__T("S_TEXT/UTF8"))
                    Subtitle=true;
                if (Subtitle)
                    Page_System_Text.push_back(List.Read(I));
                else if (Audio)
                    Page_System_Audio.push_back(List.Read(I));
                else
                    Page_System_Video.push_back(List.Read(I));
            }

            //enumerate codecs in system
            Codecs_Enumerate(Page_System_Video, Page_System_Audio);

            //MAJ
            Page_System_Buttons_VideoClick(NULL);
            Page_System_Already=true;
        }
    }

    //Form title
    Ztring Title=GUI_Text(Caption);
    Title=Title.SubString(__T(""), __T(" - "));
         if (FilesCount==0)
        //0 fichier
    {
        Caption=MEDIAINFO_TITLE;
    }
    else if (FilesCount==1)
        //un fichier
        Caption=(Ztring(MEDIAINFO_TITLE)+__T(" - ")+I->Get(0, Stream_General, 0, __T("CompleteName"))).c_str();
    else
        //Plusieurs selections
        Caption=(Ztring(MEDIAINFO_TITLE)+__T(" - ")+Ztring::ToZtring(FilesCount)+Prefs->Translate(__T(" file2"))).c_str();

    //Configure Default options
    if (FilesCount>0)
    {
        M_File_Close->Visible=true;
        M_File_Close_File->Visible=true;
        M_File_Close_All->Visible=true;
        M_File_Export->Visible=true;
        Tool_Export->Enabled=true;
        if (Page==Page_Easy || Page==Page_Sheet)
            M_File_Close_File->Visible=true;
        else
            M_File_Close_File->Visible=false;
    }
    else
    {
        M_File_Close->Visible=false;
        M_File_Close_File->Visible=false;
        M_File_Close_All->Visible=false;
        M_File_Export->Visible=false;
        Tool_Export->Enabled=false;
    }

     // Show conformance glossary button
    if (FilesCount>0 && Page!=Page_System)
    {
        bool Display=false;
        size_t FilePos=0;
        size_t AudioCount=I->Count_Get(FilePos, Stream_Audio, -1);
        for (size_t AudioPos=0; AudioPos<AudioCount; AudioPos++)
        {
            if (I->Get(FilePos, Stream_Audio, AudioPos, __T("ConformanceErrors"))!=__T("") ||
                I->Get(FilePos, Stream_Audio, AudioPos, __T("ConformanceWarnings"))!=__T(""))
            {
                Display=true;
                break;
            }
        }
        Footer_Button->Visible=Display;
    }
}

//***************************************************************************
// GUI - Menu
//***************************************************************************

//---------------------------------------------------------------------------
void __fastcall TMainF::M_File_Open_FileClick(TObject *Sender)
{
    if (!FileOpenDialog1->Execute(Handle))
        return;

    if (M_Options_CloseAllAuto->Checked)
        M_File_Close_AllClick(Sender);

    //Retrieving filenames, manage them
    if (FileOpenDialog1->Files->Count==1)
        //un fichier
        I->Open(GUI_Text(FileOpenDialog1->FileName));
    else
        //Plusieurs selections
        for (int I1=0; I1<FileOpenDialog1->Files->Count; I1++)
                I->Open(GUI_Text(FileOpenDialog1->Files->Strings[I1]));

    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_File_Open_FolderClick(TObject *Sender)
{
    if (!FolderOpenDialog1->Execute(Handle))
        return;

    if (FolderOpenDialog1->FileName.Length != 0) {
        // First we clear the list
        if (M_Options_CloseAllAuto->Checked)
            M_File_Close_AllClick(Sender);

        I->Open(GUI_Text(FolderOpenDialog1->FileName));
        Refresh();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_File_Close_FileClick(TObject *Sender)
{
    size_t Position=-1;
    if (Page->ActivePage==Page_Easy)
        Position=Page_Easy_File->ItemIndex;

    if (Page->ActivePage==Page_Sheet)
        Position=Page_Sheet_Sheet->Row-1;

    if (Position==-1)
        return;

    I->Close(Position);
    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_File_Close_AllClick(TObject *Sender)
{
    I->Close();
    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Help_SupportedParametersClick(TObject *Sender)
{
    Page_Custom_Text->Visible=true;
    Page_Custom_HTML->Visible=false;
    Page->ActivePage=Page_Custom;
    FormResize(Sender);
    Page_Custom_Text->Text=I->Option_Static(__T("Info_Parameters")).c_str();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_File_ExportClick(TObject *Sender)
{
    #ifndef MEDIAINFOGUI_EXPORT_NO
    Ztring Name;
    if (!ExportF)
    {
        ExportF=new TExportF(this);
    }
    if (ExportF->Name->Text.Length()==0)
    {
        //No initial name
        if (FileOpenDialog1->DefaultFolder.Length()==0)
        {
            Name=Prefs->BaseFolder;
            Name.resize(Name.size()-1);
            Name=Name.substr(0, Name.rfind(__T("\\"))+1); //Folder of MediaInfo

            //If only one file, use folder and filename of that file with added suffix
            if (I->Count_Get()==1) {
                Name=I->Get(0, Stream_General, 0, __T("CompleteName")).c_str();
                Name.append(__T(".MediaInfo."));
            }
        }
        else
            Name=GUI_Text(FileOpenDialog1->DefaultFolder);
    }

    ExportF->Run(*I, Name);
    #endif
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_File_ExitClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EasyClick(TObject *Sender)
{
    I->Option_Static(__T("Inform"));
    M_View_Easy->Checked=true;
    ToolBar_View_Easy->Checked=true;
    ChangePage(Page_Easy);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_SheetClick(TObject *Sender)
{
    I->Option_Static(__T("Inform"));
    M_View_Sheet->Checked=true;
    ToolBar_View_Sheet->Checked=true;
    ChangePage(Page_Sheet);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_TreeClick(TObject *Sender)
{
    I->Option_Static(__T("Inform"));
    M_View_Tree->Checked=true;
    ToolBar_View_Tree->Checked=true;
    ChangePage(Page_Tree);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_TextClick(TObject *Sender)
{
    I->Option_Static(__T("Inform"));
    M_View_Text->Checked=true;
    ToolBar_View_Text->Checked=true;
    ChangePage(Page_Text);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_HTMLClick(TObject *Sender)
{
    I->Option_Static(__T("Inform"));
    M_View_HTML->Checked=true;
    ToolBar_View_HTML->Checked=true;
    ChangePage(Page_HTML);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_XMLClick(TObject *Sender)
{
    M_View_XML->Checked=true;
    ToolBar_View_XML->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_JSONClick(TObject *Sender)
{
    M_View_JSON->Checked=true;
    ToolBar_View_JSON->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_MPEG7_StrictClick(TObject *Sender)
{
    M_View_MPEG7_Strict->Checked=true;
    ToolBar_View_MPEG7_Strict->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_MPEG7_RelaxedClick(TObject *Sender)
{
    M_View_MPEG7_Relaxed->Checked=true;
    ToolBar_View_MPEG7_Relaxed->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_MPEG7_ExtendedClick(TObject *Sender)
{
    M_View_MPEG7_Extended->Checked=true;
    ToolBar_View_MPEG7_Extended->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_reVTMDClick(TObject *Sender)
{
    M_View_reVTMD->Checked=true;
    ToolBar_View_reVTMD->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_NISO_Z39_87Click(TObject *Sender)
{
    M_View_NISO_Z39_87->Checked=true;
    ToolBar_View_NISO_Z39_87->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_Graph_SvgClick(TObject *Sender)
{
    M_View_Graph_Svg->Checked=true;
    ToolBar_View_Graph_Svg->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_PBCoreClick(TObject *Sender)
{
    M_View_PBCore->Checked=true;
    ToolBar_View_PBCore->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_PBCore2Click(TObject *Sender)
{
    M_View_PBCore2->Checked=true;
    ToolBar_View_PBCore2->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EBUCore_1_5Click(TObject *Sender)
{
    M_View_EBUCore_1_5->Checked=true;
    ToolBar_View_EBUCore_1_5->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EBUCore_1_6Click(TObject *Sender)
{
    M_View_EBUCore_1_6->Checked=true;
    ToolBar_View_EBUCore_1_6->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EBUCore_1_8_psClick(TObject *Sender)
{
    M_View_EBUCore_1_8_ps->Checked=true;
    ToolBar_View_EBUCore_1_8_ps->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EBUCore_1_8_spClick(TObject *Sender)
{
    M_View_EBUCore_1_8_sp->Checked=true;
    ToolBar_View_EBUCore_1_8_sp->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EBUCore_1_8_ps_jsonClick(TObject *Sender)
{
    M_View_EBUCore_1_8_ps_json->Checked=true;
    ToolBar_View_EBUCore_1_8_ps_json->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_EBUCore_1_8_sp_jsonClick(TObject *Sender)
{
    M_View_EBUCore_1_8_sp_json->Checked=true;
    ToolBar_View_EBUCore_1_8_sp_json->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_FIMS_1_1Click(TObject *Sender)
{
    M_View_FIMS_1_1->Checked=true;
    ToolBar_View_FIMS_1_1->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_FIMS_1_2Click(TObject *Sender)
{
    M_View_FIMS_1_2->Checked=true;
    ToolBar_View_FIMS_1_2->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_FIMS_1_3Click(TObject *Sender)
{
    M_View_FIMS_1_3->Checked=true;
    ToolBar_View_FIMS_1_3->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_CustomClick(TObject *Sender)
{
    M_View_Custom->Checked=true;
    ToolBar_View_Custom->Checked=true;
    ChangePage(Page_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_View_SystemClick(TObject *Sender)
{
    ChangePage(Page_System);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_ShowToolBarClick(TObject *Sender)
{
    if (M_Options_ShowToolBar->Checked)
        ToolBar->Visible=true;
    else
        ToolBar->Visible=false;

    FormResize(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_ShowMenuClick(TObject *Sender)
{
    //ToolBar
    if (Sender==ToolBar_View_ShowMenu)
        M_Options_ShowMenu->Checked=!M_Options_ShowMenu->Checked;

    int Height_Save=ClientHeight; //To have the same client heigth with or without menu

    bool Visible=false;
    if (M_Options_ShowMenu->Checked)
        Visible=true;

    for (int Pos=MainMenu->Items->Count-1; Pos>=0; Pos--)
        MainMenu->Items->Items[Pos]->Visible=Visible;
    ToolBar_View_ShowMenu->Visible=!Visible;

    if (!M_Options_ShowToolBar->Checked)
    {
        M_Options_ShowToolBar->Checked=true;
        M_Options_ShowToolBarClick(NULL);
    }

    ClientHeight=Height_Save;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_Theme_SystemClick(TObject *Sender)
{
    Prefs->Config(__T("Theme")) = __T("0");
    Prefs->Config.Save();
    ConfigTheme();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_Theme_LightClick(TObject *Sender)
{
    Prefs->Config(__T("Theme")) = __T("1");
    Prefs->Config.Save();
    ConfigTheme();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_Theme_DarkClick(TObject *Sender)
{
    Prefs->Config(__T("Theme")) = __T("2");
    Prefs->Config.Save();
    ConfigTheme();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_PreferencesClick(TObject *Sender)
{
    #ifndef MEDIAINFOGUI_PREFS_NO
    TPreferencesF* PreferencesF=new TPreferencesF(this);
    PreferencesF->ShowModal();
    delete PreferencesF;

    GUI_Configure();
    FormResize(NULL);
    #endif //MEDIAINFOGUI_PREFS_NO
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Debug_HeaderClick(TObject *Sender)
{
    if (!FileOpenDialog1->Execute(Handle))
        return;
    Debug_Header_Create(GUI_Text(FileOpenDialog1->FileName), Handle);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Help_AboutClick(TObject *Sender)
{
    #ifndef MEDIAINFOGUI_PREFS_NO
    TAboutF* A=new TAboutF(this);
    A->ShowModal();
    delete A;
    #endif //MEDIAINFOGUI_PREFS_NO
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Debug_AdvancedClick(TObject *Sender)
{
    if (I->Option_Static(__T("Complete_Get"))!=__T(""))
    {
        I->Option_Static(__T("Complete"));
        M_Debug_Advanced->Checked=false;
        M_Debug_Avanced_More->Visible=false;
    }
    else
    {
        I->Option_Static(__T("Complete"), __T("1"));
        M_Debug_Advanced->Checked=true;
        M_Debug_Avanced_More->Visible=true;
    }

    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Options_FullParsingClick(TObject *Sender)
{
    M_Options_FullParsing->Checked=!M_Options_FullParsing->Checked;
    I->Option_Static(__T("ParseSpeed"), M_Options_FullParsing->Checked?__T("1"):__T("0.5"));
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Debug_DummyClick(TObject *Sender)
{
    I->Option(__T("Create_Dummy"));
    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Debug_Dummy_MovieClick(TObject *Sender)
{
    I->Option(__T("Create_Dummy"), __T("Movie"));
    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Help_SupportedCodecsClick(TObject *Sender)
{
    Page_Custom_Text->Visible=true;
    Page_Custom_HTML->Visible=false;
    Page->ActivePage=Page_Custom;
    FormResize(Sender);

    Page_Custom_Text->Text=I->Option_Static(__T("Info_Codecs")).c_str();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_Help_SupportedFormatsClick(TObject *Sender)
{
    Page_Custom_Text->Visible=true;
    Page_Custom_HTML->Visible=false;
    Page->ActivePage=Page_Custom;
    FormResize(Sender);

    Page_Custom_Text->Text=I->Option_Static(__T("Info_Capacities")).c_str();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_LanguageClick(TObject *Sender)
{
    //Special case : Languages, should show the name of language in the local version
    Ztring Title=Prefs->FilesList[Prefs_Language](((TMenuItem*)Sender)->MenuIndex);

    //Load
    Prefs->Load(Prefs_Language, Title);
    Translate();

    //Refresh global
    FormResize(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_NewVersionClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, (Ztring(__T("https://mediaarea.net/"))+Prefs->Translate(__T("  Language_ISO639"))+__T("/MediaInfo/?NewVersionRequested=true")).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::M_SponsorClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, Prefs->Translate(__T("SponsorUrl")).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//***************************************************************************
// GUI - User actions
//***************************************************************************

//---------------------------------------------------------------------------
void __fastcall TMainF::ChangePage(TTabSheet *Page)
{
    Refresh(Page);
    this->Page->ActivePage=Page;
    FormResize(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Easy_FileChange(TObject *Sender)
{
    Page_Position=Page_Easy_File->ItemIndex;
    if (Page_Position<0)
        Page_Position=0;

    I->Option(__T("Inform"), __T("Summary"));
    for (int KindOfStream=0; KindOfStream<Stream_Max; KindOfStream++)
    {
        //Inform
        for (int StreamPos=0; StreamPos<EASY_STREAM_MAX; StreamPos++)
        {
            if (Page_Easy_X_Codec[KindOfStream][StreamPos])
            {
                //Main
                Page_Easy_X_Codec[KindOfStream][StreamPos]->Caption=I->Get(Page_Position, (stream_t)KindOfStream, StreamPos, __T("Inform")).c_str();
                if (Page_Easy_File->ItemIndex!=-1) //To avoid refresh on the first GUI show
                    Page_Easy_X[KindOfStream][StreamPos]->Visible=Page_Easy_X_Codec[KindOfStream][StreamPos]->Caption.Length();
                //Web
                // Disabled because links are outdated
                //Page_Easy_X_Web_Url[KindOfStream][StreamPos]=I->Get(Page_Position, (stream_t)KindOfStream, StreamPos, __T("CodecID/Url")).c_str();
                //if (Page_Easy_X_Web_Url[KindOfStream][StreamPos].empty())
                //    Page_Easy_X_Web_Url[KindOfStream][StreamPos]=I->Get(Page_Position, (stream_t)KindOfStream, StreamPos, __T("Format/Url")).c_str();
                //Page_Easy_X_Web[KindOfStream][StreamPos]->Visible=Page_Easy_X_Web_Url[KindOfStream][StreamPos].size();
            }
        }
        //List
        if (Page_Easy_X_List[KindOfStream] && KindOfStream!=Stream_General)
        {
            if (I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind")).size()>0)
            {
                Ztring Z1=Ztring(I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text)+__T("Count"));
                Ztring Z2=Ztring(__T(" "))+I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text);
                Z2.MakeLowerCase();
                if (I->Count_Get(Page_Position, (stream_t)KindOfStream)>1)
                    Z2+=__T(" stream2");
                else
                    Z2+=__T(" stream1");
                Ztring Z3=Ztring(I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text)+__T("_Format_WithHint_List"));
                Page_Easy_X_List[KindOfStream]->Caption=(I->Get(Page_Position, Stream_General, 0, Z1)+Prefs->Translate(Z2)+Prefs->Translate(__T(": "))+I->Get(Page_Easy_File->ItemIndex, Stream_General, 0, Z3)).c_str();
            }
            else
                Page_Easy_X_List[KindOfStream]->Caption=__T("");
        }
    }

    //Tag
    for (int Tag_Pos=0; Tag_Pos<EASY_TAG_MAX; Tag_Pos++)
        if (Page_Easy_X_Tag[Tag_Pos])
            Page_Easy_X_Tag[Tag_Pos]->Visible=false;
    int Tag_Pos=0;
    int MI_Pos=Title_Pos; //Should begin with "Title"
    while(MI_Pos<I->Count_Get(Page_Position, Stream_General, 0))
    {
        Ztring Z1=I->Get(Page_Position, Stream_General, 0, MI_Pos);
        Ztring Options=I->Get(Page_Position, Stream_General, 0, MI_Pos, Info_Options);
        if (Options.size()>InfoOption_ShowInInform && Options[InfoOption_ShowInInform]==__T('Y') && Z1.size()>0)
        {
            Ztring Z2=I->Get(Page_Position, Stream_General, 0, MI_Pos, Info_Name_Text);
            if (Z2.size()==0)
                Z2=I->Get(Page_Position, Stream_General, 0, MI_Pos, Info_Name);
            Z2+=Prefs->Translate(__T(": "));
            Z2+=Z1;
            if (Tag_Pos<EASY_TAG_MAX)
            {
                Page_Easy_X_Tag[Tag_Pos]->Caption=Z2.c_str();
                Page_Easy_X_Tag[Tag_Pos]->Visible=true;
            }
            Tag_Pos++;
        }
        MI_Pos++;
    }
    FormResize(NULL); //Resize GroupBoxes

    //Specific in case of no file
    if (I->Count_Get()==0)
        Page_Easy_G1_Codec->Caption=Prefs->Translate(__T("At least one file")).c_str();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Easy_WebClick(TObject *Sender)
{
    //Find KindOfStream and StreamPos
    size_t KindOfStream=0;
    size_t StreamPos=0;
    while (Page_Easy_X_Web[KindOfStream][StreamPos]!=Sender)
    {
        KindOfStream++;
        if (KindOfStream>=Stream_Max)
        {
            StreamPos++;
            KindOfStream=0;
        }
    }

    Shell_Execute(Page_Easy_X_Web_Url[KindOfStream][StreamPos]);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Easy_DifferentViewClick(TObject *Sender)
{
    ToolBar_View_Menu->Popup(Left+Page->Left+Page_Easy->Left+Page_Easy_DifferentView->Left, Top+Page->Top+Page_Easy->Top+Page_Easy_DifferentView->Top+Page_Easy_DifferentView->Height);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Sheet_Splitter1Moved(TObject *Sender)
{
    FormResize(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Sheet_SheetSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    Page_Position=ARow-1;

    for (int KindOfStream=0; KindOfStream<Stream_Max; KindOfStream++)
    {
        if (Page_Sheet_X[KindOfStream])
        {
            int Stream_Count=I->Count_Get(Page_Position, (stream_t)KindOfStream);
            if (Stream_Count==0 || Stream_Count==-1)
            {
                    Page_Sheet_X[KindOfStream]->Enabled=false;
                    Page_Sheet_X[KindOfStream]->Style=csSimple;
                    Page_Sheet_X[KindOfStream]->Text=__T("");
                    Page_Sheet_X[KindOfStream]->Enabled=false;
            }
            else if (Stream_Count==1)
            {
                Page_Sheet_X[KindOfStream]->Style=Stdctrls::csDropDownList;
                Page_Sheet_X[KindOfStream]->Items->Clear();
                I->Option(__T("Inform"), __T("Summary"));
                Page_Sheet_X[KindOfStream]->Items->Add(I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("Inform")).c_str());
                Page_Sheet_X[KindOfStream]->ItemIndex=0;
                Page_Sheet_X[KindOfStream]->Enabled=true;
            }
            else
            {
                Page_Sheet_X[KindOfStream]->Style=Stdctrls::csDropDownList;
                Page_Sheet_X[KindOfStream]->Items->Clear();
                I->Option(__T("Inform"), __T("Summary"));
                Ztring Z1=Ztring(I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text)+__T("Count"));
                Ztring Z2=Ztring(__T(" "))+I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text)+__T(" streams"); Z2.MakeLowerCase();
                Page_Sheet_X[KindOfStream]->Items->Add((I->Get(Page_Position, Stream_General, 0, Z1.c_str())+Prefs->Translate(Z2)+Prefs->Translate(__T(", "))+Prefs->Translate(__T("see below"))).c_str());
                for (int A=0; A<Stream_Count; A++)
                    Page_Sheet_X[KindOfStream]->Items->Add(I->Get(Page_Position, (stream_t)KindOfStream, A, __T("Inform")).c_str());
                Page_Sheet_X[KindOfStream]->ItemIndex=0;
                Page_Sheet_X[KindOfStream]->Enabled=true;
            }
            Page_Sheet_Change(Page_Sheet_X[KindOfStream]);
        }
    }

    //Show General first
    Page_Sheet_Change(Page_Sheet_G);

    //Menu
    M_File_Close_File->Enabled=true;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Sheet_Change(TObject *Sender)
{
    //Find KindOfStream
    size_t KindOfStream=0;
    while (Page_Sheet_X[KindOfStream]!=Sender)
        KindOfStream++;

    if (Page_Position<0)
        return;

    //Get Stream Page_Position
    int I1;
    if (Page_Sheet_X[KindOfStream]->Items->Count==1)
        I1=0;
    else
        I1=Page_Sheet_X[KindOfStream]->ItemIndex-1;

    //Fill info
    Page_Sheet_X[KindOfStream]->Hint=I->Get(Page_Position, (stream_t)KindOfStream, I1, __T("CodecID/Info")).c_str();
    Page_Sheet_X_Web_Url[KindOfStream]=I->Get(Page_Position, (stream_t)KindOfStream, I1, __T("CodecID/Url")).c_str();
    if (Page_Sheet_X_Web_Url[KindOfStream].empty())
        Page_Sheet_X_Web_Url[KindOfStream]=I->Get(Page_Position, (stream_t)KindOfStream, I1, __T("Format/Url")).c_str();
    if (Page_Sheet_X_Web_Url[KindOfStream]==__T(""))
    {
        Page_Sheet_X_Web[KindOfStream]->Enabled=false;
        Page_Sheet_X_Web[KindOfStream]->Hint=__T("");
    }
    else
    {
        Page_Sheet_X_Web[KindOfStream]->Enabled=true;
        Ztring Z1=Ztring(__T("WebSite_"))+I->Get(Page_Position, (stream_t)KindOfStream, 0, __T("StreamKind"))+__T("_More");
        Z1=Prefs->Translate(Z1);
        Z1.FindAndReplace(__T("%Url%"), Page_Sheet_X_Web_Url[KindOfStream]);
        Page_Sheet_X_Web[KindOfStream]->Hint=Z1.c_str();
    }

    I->Option(__T("Inform"), __T(""));

    Page_Sheet_Text->Text=I->Get(Page_Position, (stream_t)KindOfStream, I1, __T("Inform")).c_str();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_Sheet_WebClick(TObject *Sender)
{
    //Find KindOfStream
    size_t KindOfStream=0;
    while (Page_Sheet_X_Web[KindOfStream]!=Sender)
        KindOfStream++;

    ShellExecute(NULL, __T("open"), Page_Sheet_X_Web_Url[KindOfStream].c_str(), NULL, NULL, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_System_Buttons_VideoClick(TObject *Sender)
{
    Page_System_Sheet_ColumnToSort=0;
    Page_System_Sheet->Clear();
    for (size_t count=0; count<Page_System_Video.size(); count++)
    {
        TListItem* Parent=Page_System_Sheet->Items->Add();
        Parent->Caption=Page_System_Video(count, 0).c_str();
        Parent->SubItems->Add(Page_System_Video(count, 1).c_str());
        Parent->SubItems->Add(Page_System_Video(count, 2).c_str());
        Parent->SubItems->Add(Page_System_Video(count, 3).c_str());
        Parent->SubItems->Add(Page_System_Video(count, 4).c_str());
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_System_Buttons_AudioClick(TObject *Sender)
{
    Page_System_Sheet_ColumnToSort=0;
    Page_System_Sheet->Clear();
    for (size_t count=0; count<Page_System_Audio.size(); count++)
    {
        TListItem* Parent=Page_System_Sheet->Items->Add();
        Parent->Caption=Page_System_Audio(count, 0).c_str();
        Parent->SubItems->Add(Page_System_Audio(count, 1).c_str());
        Parent->SubItems->Add(Page_System_Audio(count, 2).c_str());
        Parent->SubItems->Add(Page_System_Audio(count, 3).c_str());
        Parent->SubItems->Add(Page_System_Audio(count, 4).c_str());
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_System_Buttons_TextClick(TObject *Sender)
{
    Page_System_Sheet_ColumnToSort=0;
    Page_System_Sheet->Clear();
    for (size_t count=0; count<Page_System_Text.size(); count++)
    {
        TListItem* Parent=Page_System_Sheet->Items->Add();
        Parent->Caption=Page_System_Text(count, 0).c_str();
        Parent->SubItems->Add(Page_System_Text(count, 1).c_str());
        Parent->SubItems->Add(Page_System_Text(count, 2).c_str());
        Parent->SubItems->Add(Page_System_Text(count, 3).c_str());
        Parent->SubItems->Add(Page_System_Text(count, 4).c_str());
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_System_SheetColumnClick(TObject *Sender,
      TListColumn *Column)
{
    Page_System_Sheet_ColumnToSort = Column->Index;
    ((TCustomListView *)Sender)->AlphaSort();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Page_System_SheetCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
    if (Page_System_Sheet_ColumnToSort == 0)
        Compare = CompareText(Item1->Caption, Item2->Caption);
    else
        Compare = CompareText(Item1->SubItems->Strings[Page_System_Sheet_ColumnToSort-1], Item2->SubItems->Strings[Page_System_Sheet_ColumnToSort-1]);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::Footer_ButtonClick(TObject *Sender)
{
    const Ztring Inform_Save=I->Option(__T("Inform_Get"), __T(""));
    I->Option(__T("Inform"), __T("Conformance_JSON"));

    const Ztring URL=I->Inform();
    I->Option(__T("Inform"), Inform_Save);
    ShellExecute(NULL, NULL, URL.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//***************************************************************************
// System
//***************************************************************************

//---------------------------------------------------------------------------
MESSAGE void __fastcall TMainF::HandleDropFiles (TMessage& Msg)
{
    //Clear the existing filelist
    if (M_Options_CloseAllAuto->Checked)
        M_File_Close_AllClick(NULL);

    //Retrieving infos
    ZenLib::Char File[65536];
    HDROP hDrop=(HDROP)Msg.WParam;
    int    NbFiles=DragQueryFile(hDrop, -1, NULL, 0);
    for (int i=0; i<NbFiles; i++)
    {
        DragQueryFile(hDrop, i, File, 65536-1);
        I->Open(File);
    }
    DragFinish (hDrop);

    Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::CreateWnd()
{
    TForm::CreateWnd();
    if (HandleAllocated())
        DragAcceptFiles(Handle, true);
}

//---------------------------------------------------------------------------
void __fastcall TMainF::DestroyWnd()
{
    if (HandleAllocated())
        DragAcceptFiles(Handle, false);
    TForm::DestroyWnd();
}

//---------------------------------------------------------------------------
void __fastcall TMainF::ApplicationEvents1OnSettingChange(
    TObject* Sender, int Flag, const UnicodeString Section, int &Result)
{
    if (Section == "ImmersiveColorSet") {
        ConfigTheme();
    }
}
