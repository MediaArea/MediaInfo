/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
// Compilation condition
#ifndef MEDIAINFOGUI_PREFS_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GUI/VCL/GUI_Preferences.h"
#include "GUI/VCL/GUI_Preferences_Language.h"
#include "GUI/VCL/GUI_Preferences_Sheet.h"
#include "GUI/VCL/GUI_Preferences_Custom.h"
#ifndef MEDIAINFOGUI_PLUGIN_NO
    #include "GUI/VCL/GUI_Plugin.h"
    #include <ZenLib/File.h>
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPreferencesF *PreferencesF;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void GUI_Preferences_Sheet_Kind_Fill(TComboBox* Box);
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TPreferencesF::TPreferencesF(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::ComboBox_Update(TComboBox *CB, Prefs_t List)
{
    //List Update
    Prefs->RefreshFilesList(List);

    //Special case : Languages, should show the name of language in the local version
    if (CB==General_Language_Sel)
        List=Prefs_Language_List;

    //ComboBox Listing
    CB->Items->Clear();
    for (size_t Pos=0; Pos<Prefs->FilesList[List].size(); Pos++)
        CB->Items->Add(Prefs->FilesList[List][Pos].c_str());

    //Special case : Languages, should show the name of language in the local version
    if (CB==General_Language_Sel)
        List=Prefs_Language;

    //Selecting
    CB->ItemIndex=Prefs->FilesList[List].Find(Prefs->Config(Prefs->FolderNames(List)));
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::ComboBox_SelChange(TComboBox *CB, Prefs_t List)
{
    Prefs->Config(Prefs->FolderNames(List))=GUI_Text(CB->Text);
    Prefs->Load(List, GUI_Text(CB->Text));
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::General_Language_SelChange(TObject *Sender)
{
    //Config
    size_t Language_Pos=Prefs->FilesList[Prefs_Language_List].Find(GUI_Text(General_Language_Sel->Text)); //Special case : Languages, should show the name of language in the local version
    if (Language_Pos==(size_t)-1)
        return;
    Prefs->Config(Prefs->FolderNames(Prefs_Language))=Prefs->FilesList[Prefs_Language](Language_Pos); //ComboBox_SelChange(General_Language_Sel, Language);
    Prefs->Load(Prefs_Language, Prefs->FilesList[Prefs_Language](Language_Pos)); //ComboBox_SelChange(General_Language_Sel, Language);

    Language_Sel->ItemIndex=General_Language_Sel->ItemIndex;

    //Translation of Preferences
    GUI_Configure();

    //Translation of Main GUI
    MainF->Translate();
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::General_Output_SelChange(TObject *Sender)
{
    if (General_Output_Sel->ItemIndex==0)
    {
        Prefs->Config(__T("Output"))=__T("Basic");
        MainF->Page->ActivePage=MainF->Page_Easy;
        MainF->Page_Easy_FileChange(NULL);
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==1)
    {
        Prefs->Config(__T("Output"))=__T("Sheet");
        MainF->Page->ActivePage=MainF->Page_Sheet;
        General_Output_More->Visible=true;
    }
    if (General_Output_Sel->ItemIndex==2)
    {
        Prefs->Config(__T("Output"))=__T("Tree");
        MainF->Page->ActivePage=MainF->Page_Tree;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==3)
    {
        Prefs->Config(__T("Output"))=__T("Text");
        MainF->Page->ActivePage=MainF->Page_Tree;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==4)
    {
        Prefs->Config(__T("Output"))=__T("HTML");
        MainF->Page->ActivePage=MainF->Page_HTML;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==5)
    {
        Prefs->Config(__T("Output"))=__T("XML");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==6)
    {
        Prefs->Config(__T("Output"))=__T("JSON");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==7)
    {
        Prefs->Config(__T("Output"))=__T("Graph_Svg");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==8)
    {
        Prefs->Config(__T("Output"))=__T("MPEG-7_Strict");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==9)
    {
        Prefs->Config(__T("Output"))=__T("MPEG-7_Relaxed");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==10)
    {
        Prefs->Config(__T("Output"))=__T("MPEG-7_Extended");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==11)
    {
        Prefs->Config(__T("Output"))=__T("PBCore_1.2");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==12)
    {
        Prefs->Config(__T("Output"))=__T("PBCore_2.0");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==13)
    {
        Prefs->Config(__T("Output"))=__T("EBUCore_1.5");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==14)
    {
        Prefs->Config(__T("Output"))=__T("EBUCore_1.6");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==15)
    {
        Prefs->Config(__T("Output"))=__T("EBUCore_1.8_ps");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==16)
    {
        Prefs->Config(__T("Output"))=__T("EBUCore_1.8_sp");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==17)
    {
        Prefs->Config(__T("Output"))=__T("EBUCore_1.8_ps_json");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==18)
    {
        Prefs->Config(__T("Output"))=__T("EBUCore_1.8_sp_json");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==19)
    {
        Prefs->Config(__T("Output"))=__T("FIMS_1.1");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==20)
    {
        Prefs->Config(__T("Output"))=__T("FIMS_1.2");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==21)
    {
        Prefs->Config(__T("Output"))=__T("FIMS_1.3");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==22)
    {
        Prefs->Config(__T("Output"))=__T("NISO_Z39.87");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==23)
    {
        Prefs->Config(__T("Output"))=__T("reVTMD");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==24)
    {
        Prefs->Config(__T("Output"))=__T("Custom");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Language_SelChange(TObject *Sender)
{
    //Delete button
    if (GUI_Text(Language_Sel->Text)==Prefs->DefaultNames[Prefs_Language])
        Language_Delete->Enabled=false;
    else
        Language_Delete->Enabled=true;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Customize_SheetShow(TObject *Sender)
{
    ComboBox_Update(Sheet_Sel, Prefs_Sheet);
    Sheet_SelChange(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Sheet_SelChange(TObject *Sender)
{
    //Delete button
    if (GUI_Text(Sheet_Sel->Text)==Prefs->DefaultNames[Prefs_Sheet])
        Sheet_Delete->Enabled=false;
    else
        Sheet_Delete->Enabled=true;

    //Save choice
    Prefs->Config(__T("Sheet"))=GUI_Text(Sheet_Sel->Text);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Sheet_DeleteClick(TObject *Sender)
{
    Prefs->Remove(Prefs_Sheet, GUI_Text(Sheet_Sel->Text));
    ComboBox_Update(Sheet_Sel, Prefs_Sheet);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Sheet_EditClick(TObject *Sender)
{
    TPreferences_SheetF *C=new TPreferences_SheetF(NULL);
    C->Run(GUI_Text(Sheet_Sel->Text));
    delete C;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Sheet_NewClick(TObject *Sender)
{
    UnicodeString S1=__T("New");
    if (!InputQuery(__T("New sheet"), __T("Enter name of new sheet"), S1))
        return;

    Prefs->Create(Prefs_Sheet, S1.c_str());
    ComboBox_Update(Sheet_Sel, Prefs_Sheet);

    //Selecting and edit
    Sheet_Sel->ItemIndex=Prefs->FilesList[Prefs_Sheet].Find(S1.c_str());
    Sheet_EditClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Custom_SelChange(TObject *Sender)
{
    //Delete button
    if (GUI_Text(Custom_Sel->Text)==Prefs->DefaultNames[Prefs_Custom])
        Custom_Delete->Enabled=false;
    else
        Custom_Delete->Enabled=true;

    //Save choice
    Prefs->Config(__T("Custom"))=GUI_Text(Custom_Sel->Text);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Custom_EditClick(TObject *Sender)
{
    TPreferences_CustomF *C=new TPreferences_CustomF(NULL);
    C->Run(GUI_Text(Custom_Sel->Text));
    delete C;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Language_EditClick(TObject *Sender)
{
    TPreferences_LanguageF *L=new TPreferences_LanguageF(NULL);
    L->Run(GUI_Text(Language_Sel->Text));
    delete L;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::OKClick(TObject *Sender)
{
    Prefs->Config.Save();
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_CheckUpdateClick(TObject *Sender)
{
    //Internet
    if (CB_CheckUpdate->Checked)
    {
        Prefs->Config(__T("CheckUpdate"), 1)=__T("1");
    }
    else
        Prefs->Config(__T("CheckUpdate"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_InscrireShellClick(TObject *Sender)
{
    //Shell extension
    if (CB_InscrireShell->Checked)
        Prefs->Config(__T("ShellExtension"), 1)=__T("1");
    else
        Prefs->Config(__T("ShellExtension"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_InscrireShell_FolderClick(TObject *Sender)
{
    //Shell extension
    if (CB_InscrireShell_Folder->Checked)
        Prefs->Config(__T("ShellExtension_Folder"), 1)=__T("1");
    else
        Prefs->Config(__T("ShellExtension_Folder"), 1)=__T("0");

}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_InfoTipClick(TObject *Sender)
{
    //ToolTip
    if (CB_InfoTip->Checked)
        Prefs->Config(__T("ShellInfoTip"), 1)=__T("1");
    else
        Prefs->Config(__T("ShellInfoTip"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_RememberWindowPositionClick(TObject *Sender)
{
	Prefs->Config(__T("RememberWindowPosition"), 1) = CB_RememberWindowPosition->Checked ? __T("1") : __T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_RememberWindowDimensionsClick(TObject *Sender)
{
	Prefs->Config(__T("RememberWindowDimensions"), 1) = CB_RememberWindowDimensions->Checked ? __T("1") : __T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_ShowToolBarClick(TObject *Sender)
{
    Prefs->Config(__T("ShowToolBar"), 1)=CB_ShowToolBar->Checked?__T("1"):__T("0");
    MainF->M_Options_ShowToolBar->Checked=CB_ShowToolBar->Checked;
    MainF->M_Options_ShowToolBarClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_ShowMenuClick(TObject *Sender)
{
    Prefs->Config(__T("ShowMenu"), 1)=CB_ShowMenu->Checked?__T("1"):__T("0");
    MainF->M_Options_ShowMenu->Checked=CB_ShowMenu->Checked;
    MainF->M_Options_ShowMenuClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Advanced_CloseAllAutoClick(TObject *Sender)
{
    if (Advanced_CloseAllAuto->Checked)
        Prefs->Config(__T("CloseAllAuto"), 1)=__T("1");
    else
        Prefs->Config(__T("CloseAllAuto"), 1)=__T("0");
    MainF->M_Options_CloseAllAuto->Checked=Advanced_CloseAllAuto->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Advanced_InformVersionClick(TObject *Sender)
{
    if (Advanced_InformVersion->Checked)
        Prefs->Config(__T("InformVersion"), 1)=__T("1");
    else
        Prefs->Config(__T("InformVersion"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Advanced_InformTimestampClick(TObject *Sender)

{
    if (Advanced_InformTimestamp->Checked)
        Prefs->Config(__T("InformTimestamp"), 1)=__T("1");
    else
        Prefs->Config(__T("InformTimestamp"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Advanced_EnableFfmpegClick(TObject *Sender)
{
    if (Advanced_EnableFfmpeg->Checked)
    {
        #ifndef MEDIAINFOGUI_PLUGIN_NO
        if (Prefs->Config(__T("EnableFfmpeg"), 1) != __T("1"))
        {
            Ztring InstallFolder = Application->ExeName.c_str();
            InstallFolder = InstallFolder.substr(0, InstallFolder.rfind(__T("\\")) + 1);

            if (!File::Exists(InstallFolder+__T("\\Plugin\\FFmpeg\\version.txt"))) //Try to install plugin
            {
                TPluginF* P = new TPluginF(this, PLUGIN_FFMPEG);
                if (P->Configure())
                    P->ShowModal();
                delete P;

                if (!File::Exists(InstallFolder+__T("\\Plugin\\FFmpeg\\version.txt")))
                    MessageBox(NULL, __T("An error occured, please download and install the plugin manually from the MediaInfo download page."), __T("Error"), MB_OK);
            }
        }
        #endif
        Prefs->Config(__T("EnableFfmpeg"), 1)=__T("1");
    }
    else
    {
        Prefs->Config(__T("EnableFfmpeg"), 1)=__T("0");
    }
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Advanced_DisplayCaptions_SelChange(TObject *Sender)
{
    if (Advanced_DisplayCaptions_Sel->ItemIndex==0)
        Prefs->Config(__T("DisplayCaptions"))=__T("Content");
    else if (Advanced_DisplayCaptions_Sel->ItemIndex==1)
        Prefs->Config(__T("DisplayCaptions"))=__T("Command");
    else if (Advanced_DisplayCaptions_Sel->ItemIndex==2)
        Prefs->Config(__T("DisplayCaptions"))=__T("Stream");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Language_NewClick(TObject *Sender)
{
    UnicodeString S1=__T("New");
    if (!InputQuery(__T("New language"), __T("Enter name of new language"), S1))
        return;

    Prefs->Create(Prefs_Language, S1.c_str());
    ComboBox_Update(General_Language_Sel, Prefs_Language);
    ComboBox_Update(Language_Sel, Prefs_Language);

    //Selecting and edit
    Language_Sel->ItemIndex=Prefs->FilesList[Prefs_Language].Find(S1.c_str());
    Language_EditClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Language_DeleteClick(TObject *Sender)
{
    Prefs->Remove(Prefs_Language, GUI_Text(Language_Sel->Text));
    ComboBox_Update(General_Language_Sel, Prefs_Language);
    ComboBox_Update(Language_Sel, Prefs_Language);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Graph_Adm_ShowTrackUIDsClick(TObject *Sender)
{
    if (Graph_Adm_ShowTrackUIDs->Checked)
        Prefs->Config(__T("Graph_Adm_ShowTrackUIDs"), 1)=__T("1");
    else
        Prefs->Config(__T("Graph_Adm_ShowTrackUIDs"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Graph_Adm_ShowChannelFormatsClick(TObject *Sender)

{
    if (Graph_Adm_ShowChannelFormats->Checked)
        Prefs->Config(__T("Graph_Adm_ShowChannelFormats"), 1)=__T("1");
    else
        Prefs->Config(__T("Graph_Adm_ShowChannelFormats"), 1)=__T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Custom_NewClick(TObject *Sender)
{
    UnicodeString S1=__T("New");
    if (!InputQuery(__T("New Output"), __T("Enter name of new Output"), S1))
        return;

    Prefs->Create(Prefs_Custom, S1.c_str());
    ComboBox_Update(Custom_Sel, Prefs_Custom);

    //Selecting and edit
    Custom_Sel->ItemIndex=Prefs->FilesList[Prefs_Custom].Find(S1.c_str());
    Custom_EditClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Custom_DeleteClick(TObject *Sender)
{
    Prefs->Remove(Prefs_Custom, GUI_Text(Custom_Sel->Text));
    ComboBox_Update(Custom_Sel, Prefs_Custom);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::TreeChange(TObject *Sender,
      TTreeNode *Node)
{
    if (Node==Tree->Items->Item[0])
    {
        Page->ActivePage=Setup;
    }
    if (Node==Tree->Items->Item[1])
    {
        Page->ActivePage=Setup_Advanced;
    }
    if (Node==Tree->Items->Item[2])
    {
        General_Output_MoreClick(Sender);
    }
    if (Node==Tree->Items->Item[3])
    {
        Page->ActivePage=Customize_Sheet;
    }
    if (Node==Tree->Items->Item[4])
    {
        Page->ActivePage=Customize_TreeText;
    }
    if (Node==Tree->Items->Item[5])
    {
        Page->ActivePage=Customize_Custom;
    }
    if (Node==Tree->Items->Item[6])
    {
        Page->ActivePage=Customize_Graph;
    }
    if (Node==Tree->Items->Item[7])
    {
        Page->ActivePage=Customize_Language;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::General_Language_MoreClick(TObject *Sender)
{
    Page->ActivePage = Customize_Language;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::General_Output_MoreClick(TObject *Sender)
{
         if (General_Output_Sel->ItemIndex==1)
        Tree->Select(Tree->Items->Item[3]);
    else if (General_Output_Sel->ItemIndex==2)
        Tree->Select(Tree->Items->Item[4]);
    else if (General_Output_Sel->ItemIndex==3)
        Tree->Select(Tree->Items->Item[4]);
    else if (General_Output_Sel->ItemIndex==4)
        Tree->Select(Tree->Items->Item[4]);
    else if (General_Output_Sel->ItemIndex==9)
        Tree->Select(Tree->Items->Item[5]);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::FormAfterMonitorDpiChanged(TObject *Sender, int OldDPI, int NewDPI)
{
    General_Language_More->Height=General_Language_Sel->Height;
    General_Output_More->Height=General_Output_Sel->Height;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::FormShow(TObject *Sender)
{
    // review: GUI_Configure should be in FormCreate
    GUI_Configure();
    General_Output_SelChange(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Setup_GeneralShow(TObject *Sender)
{
    ComboBox_Update(General_Language_Sel, Prefs_Language);
    CB_CheckUpdate->Checked=Prefs->Config(__T("CheckUpdate")).To_int32s();
    CB_InscrireShell->Checked=Prefs->Config(__T("ShellExtension")).To_int32s(); //Lecture Shell extension
    CB_InscrireShell_Folder->Checked=Prefs->Config(__T("ShellExtension_Folder")).To_int32s(); //Lecture Shell extension
    CB_InfoTip->Checked=Prefs->Config(__T("ShellInfoTip")).To_int32s(); //Lecture Shell extension
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Setup_AdvancedShow(TObject *Sender)
{
	CB_RememberWindowPosition->Checked=Prefs->Config(__T("RememberWindowPosition")).To_int32s();
	CB_RememberWindowDimensions->Checked=Prefs->Config(__T("RememberWindowDimensions")).To_int32s();
    CB_ShowToolBar->Checked=Prefs->Config(__T("ShowToolBar")).To_int32s();
    CB_ShowMenu->Checked=Prefs->Config(__T("ShowMenu")).To_int32s();
    Advanced_CloseAllAuto->Checked=Prefs->Config(__T("CloseAllAuto")).To_int32s();
    Advanced_InformVersion->Checked=Prefs->Config(__T("InformVersion")).To_int32s();
    Advanced_InformTimestamp->Checked=Prefs->Config(__T("InformTimestamp")).To_int32s();
    Advanced_EnableFfmpeg->Checked=Prefs->Config(__T("EnableFfmpeg")).To_int32s();

    //Display captions
    if (Prefs->Config(__T("DisplayCaptions"))==__T("Content"))
        Advanced_DisplayCaptions_Sel->ItemIndex=0;
    else if (Prefs->Config(__T("DisplayCaptions"))==__T("Stream"))
        Advanced_DisplayCaptions_Sel->ItemIndex=2;
    else // Command (default)
        Advanced_DisplayCaptions_Sel->ItemIndex=1;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Customize_LanguageShow(TObject *Sender)
{
    ComboBox_Update(Language_Sel, Prefs_Language);
    Language_SelChange(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Customize_CustomShow(TObject *Sender)
{
    ComboBox_Update(Custom_Sel, Prefs_Custom);
    Custom_SelChange(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Customize_GraphShow(TObject *Sender)
{
    Graph_Adm_ShowTrackUIDs->Checked=Prefs->Config(__T("Graph_Adm_ShowTrackUIDs")).To_int32s();
    Graph_Adm_ShowChannelFormats->Checked=Prefs->Config(__T("Graph_Adm_ShowChannelFormats")).To_int32s();

}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::GUI_Configure()
{
    //Preparation of GUI
    
	for (int cTabIndex = 0; cTabIndex <= Page->PageCount-1; cTabIndex++)
		Page->Pages[cTabIndex]->TabVisible = False;

    Tree->FullExpand();
    Page->ActivePage=Setup;

    //Translation
    //Title
    Caption=Prefs->Translate(__T("Preferences")).c_str();
    //Buttons
    OK->Caption=Prefs->Translate(__T("OK")).c_str();
    Cancel->Caption=Prefs->Translate(__T("Cancel")).c_str();
    //-Left panel
    Tree->Items->Item[0]->Text=Prefs->Translate(__T("Setup")).c_str();
    Tree->Items->Item[1]->Text=Prefs->Translate(__T("Advanced")).c_str();
    Tree->Items->Item[2]->Text=Prefs->Translate(__T("Customize")).c_str();
    Tree->Items->Item[3]->Text=Prefs->Translate(__T("Sheet")).c_str();
    Tree->Items->Item[4]->Text=Prefs->Translate(__T("Tree & Text")).c_str();
    Tree->Items->Item[5]->Text=Prefs->Translate(__T("Custom")).c_str();
    Tree->Items->Item[7]->Text=Prefs->Translate(__T("Graph")).c_str();
    Tree->Items->Item[7]->Text=Prefs->Translate(__T("Language")).c_str();
    //-General
    General_Language_More->Caption=(Prefs->Translate(__T("More"))+__T("...")).c_str();
    Langue_C->Caption=Prefs->Translate(__T("Language")).c_str();
    Output_C->Caption=Prefs->Translate(__T("Output format")).c_str();
    General_Output_More->Caption=(Prefs->Translate(__T("More"))+__T("...")).c_str();
    CB_CheckUpdate->Caption=Prefs->Translate(__T("Newest version")).c_str();
    //--Special case : Output
    General_Output_Sel->Items->Clear();
    General_Output_Sel->Items->Add(Prefs->Translate(__T("Basic")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(__T("Sheet")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(__T("Tree")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(__T("Text")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(__T("HTML")).c_str());
    General_Output_Sel->Items->Add("XML");
    General_Output_Sel->Items->Add("JSON");
    General_Output_Sel->Items->Add("Graph");
    General_Output_Sel->Items->Add("MPEG-7 (strict)");
    General_Output_Sel->Items->Add("MPEG-7 (relaxed)");
    General_Output_Sel->Items->Add("MPEG-7 (extended)");
    General_Output_Sel->Items->Add("PBCore_1.2");
    General_Output_Sel->Items->Add("PBCore_2.0");
    General_Output_Sel->Items->Add("EBUCore_1.5");
    General_Output_Sel->Items->Add("EBUCore_1.6");
    General_Output_Sel->Items->Add("EBUCore_1.8 (segment then parameter)");
    General_Output_Sel->Items->Add("EBUCore_1.8 (parameter then segment)");
    General_Output_Sel->Items->Add("EBUCore_1.8 (segment then parameter, json output)");
    General_Output_Sel->Items->Add("EBUCore_1.8 (parameter then segment, json output)");
    General_Output_Sel->Items->Add("FIMS_1.1");
    General_Output_Sel->Items->Add("FIMS_1.2");
    General_Output_Sel->Items->Add("FIMS_1.3");
    General_Output_Sel->Items->Add("NISO_Z39.87");
    General_Output_Sel->Items->Add("reVTMD");
    General_Output_Sel->Items->Add(Prefs->Translate(__T("Custom")).c_str());
         if (Prefs->Config(__T("Output"))==__T("Basic"))      General_Output_Sel->ItemIndex=0;
    else if (Prefs->Config(__T("Output"))==__T("Sheet"))      General_Output_Sel->ItemIndex=1;
    else if (Prefs->Config(__T("Output"))==__T("Tree"))       General_Output_Sel->ItemIndex=2;
    else if (Prefs->Config(__T("Output"))==__T("Text"))       General_Output_Sel->ItemIndex=3;
    else if (Prefs->Config(__T("Output"))==__T("HTML"))       General_Output_Sel->ItemIndex=4;
    else if (Prefs->Config(__T("Output"))==__T("XML"))        General_Output_Sel->ItemIndex=5;
    else if (Prefs->Config(__T("Output"))==__T("JSON"))       General_Output_Sel->ItemIndex=6;
    else if (Prefs->Config(__T("Output"))==__T("Graph_Svg")) General_Output_Sel->ItemIndex=7;
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7_Strict")) General_Output_Sel->ItemIndex=8;
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7_Relaxed")) General_Output_Sel->ItemIndex=9;
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7_Extended")) General_Output_Sel->ItemIndex=10;
    else if (Prefs->Config(__T("Output"))==__T("PBCore_1.2")) General_Output_Sel->ItemIndex=11;
    else if (Prefs->Config(__T("Output"))==__T("PBCore_2.0")) General_Output_Sel->ItemIndex=12;
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.5")) General_Output_Sel->ItemIndex=13;
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.6")) General_Output_Sel->ItemIndex=14;
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_ps")) General_Output_Sel->ItemIndex=15;
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_sp")) General_Output_Sel->ItemIndex=16;
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_ps_json")) General_Output_Sel->ItemIndex=17;
    else if (Prefs->Config(__T("Output"))==__T("EBUCore_1.8_sp_json")) General_Output_Sel->ItemIndex=18;
    else if (Prefs->Config(__T("Output"))==__T("FIMS_1.1")) General_Output_Sel->ItemIndex=19;
    else if (Prefs->Config(__T("Output"))==__T("FIMS_1.2")) General_Output_Sel->ItemIndex=20;
    else if (Prefs->Config(__T("Output"))==__T("FIMS_1.3")) General_Output_Sel->ItemIndex=21;
    else if (Prefs->Config(__T("Output"))==__T("NISO_Z39.87")) General_Output_Sel->ItemIndex=22;
    else if (Prefs->Config(__T("Output"))==__T("reVTMD"))     General_Output_Sel->ItemIndex=23;
    else if (Prefs->Config(__T("Output"))==__T("Custom"))     General_Output_Sel->ItemIndex=24;
    //-Advanced
    Setup_Advanced->Caption=Prefs->Translate(__T("Advanced")).c_str();
    CB_InscrireShell->Caption=Prefs->Translate(__T("Shell extension")).c_str();
    CB_InscrireShell_Folder->Caption=Prefs->Translate(__T("Shell extension, folder")).c_str();
    CB_InfoTip->Caption=Prefs->Translate(__T("Shell InfoTip")).c_str();
    CB_ShowToolBar->Caption=Prefs->Translate(__T("Show toolbar")).c_str();
    CB_ShowMenu->Caption=Prefs->Translate(__T("Show menu")).c_str();
    Advanced_CloseAllAuto->Caption=Prefs->Translate(__T("Close all before open")).c_str();
    Advanced_InformVersion->Caption=Prefs->Translate(__T("Add version to text output")).c_str();
    Advanced_InformTimestamp->Caption=Prefs->Translate(__T("Add creation date to text output")).c_str();
    Advanced_EnableFfmpeg->Caption=Prefs->Translate(__T("Enable FFmpeg plugin")).c_str();
    Advanced_DisplayCaptions_Caption->Caption=Prefs->Translate(__T("DisplayCaptions")).c_str();
    Advanced_DisplayCaptions_Sel->Items->Clear();
    Advanced_DisplayCaptions_Sel->Items->Add(Prefs->Translate(__T("DisplayCaptions_Content")).c_str());
    Advanced_DisplayCaptions_Sel->Items->Add(Prefs->Translate(__T("DisplayCaptions_Command")).c_str());
    Advanced_DisplayCaptions_Sel->Items->Add(Prefs->Translate(__T("DisplayCaptions_Stream")).c_str());
    //-Language
    Language_Caption->Caption=Prefs->Translate(__T("Choose language")).c_str();
    Language_Edit->Caption=(Prefs->Translate(__T("Edit"))+__T("...")).c_str();
    Language_Delete->Caption=Prefs->Translate(__T("Delete")).c_str();
    Language_New->Caption=(Prefs->Translate(__T("New"))+__T("...")).c_str();
    //-Sheet
    Sheet_Caption->Caption=Prefs->Translate(__T("Choose custom sheet")).c_str();
    Sheet_Edit->Caption=(Prefs->Translate(__T("Edit"))+__T("...")).c_str();
    Sheet_Delete->Caption=Prefs->Translate(__T("Delete")).c_str();
    Sheet_New->Caption=(Prefs->Translate(__T("New"))+__T("...")).c_str();
    //-Tree
    Tree_NotYet->Caption=(Prefs->Translate(__T("Not yet"))+__T("...")).c_str();
    //-Graph
    Graph_Adm_ShowTrackUIDs->Caption=Prefs->Translate(__T("ADM: Show TrackUIDs")).c_str();
    Graph_Adm_ShowChannelFormats->Caption=Prefs->Translate(__T("ADM: Show ChannelFormats")).c_str();
    //-Custom
    Custom_Caption->Caption=Prefs->Translate(__T("Choose custom text")).c_str();
    Custom_Edit->Caption=(Prefs->Translate(__T("Edit"))+__T("...")).c_str();
    Custom_Delete->Caption=Prefs->Translate(__T("Delete")).c_str();
    Custom_New->Caption=(Prefs->Translate(__T("New"))+__T("...")).c_str();

    General_Language_More->Height=General_Language_Sel->Height;
    General_Output_More->Height=General_Output_Sel->Height;

}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

