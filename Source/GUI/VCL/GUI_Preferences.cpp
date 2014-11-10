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
        Prefs->Config(__T("Output"))=__T("MPEG-7");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==7)
    {
        Prefs->Config(__T("Output"))=__T("PBCore_1.2");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==8)
    {
        Prefs->Config(__T("Output"))=__T("PBCore_2.0");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==9)
    {
        Prefs->Config(__T("Output"))=__T("reVTMD");
        MainF->Page->ActivePage=MainF->Page_Custom;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==10)
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
        Page->ActivePage=Customize_Language;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::General_Language_MoreClick(TObject *Sender)
{
    Tree->Select(Tree->Items->Item[6]);
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
void __fastcall TPreferencesF::FormShow(TObject *Sender)
{
    GUI_Configure();
    General_Output_SelChange(NULL);

    //Not done with BCB because I want to easy select tabs in it
    Page->Top=-6;
    Page->TabHeight=1;
    Cancel->Top=Page->Top+Page->Height;
    OK->Top=Cancel->Top;
    ClientHeight=OK->Top+OK->Height;
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
    CB_ShowToolBar->Checked=Prefs->Config(__T("ShowToolBar")).To_int32s();
    CB_ShowMenu->Checked=Prefs->Config(__T("ShowMenu")).To_int32s();
    Advanced_CloseAllAuto->Checked=Prefs->Config(__T("CloseAllAuto")).To_int32s();
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
void __fastcall TPreferencesF::GUI_Configure()
{
    //Preparation of GUI
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
    Tree->Items->Item[6]->Text=Prefs->Translate(__T("Language")).c_str();
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
    General_Output_Sel->Items->Add("MPEG-7");
    General_Output_Sel->Items->Add("PBCore_1.2");
    General_Output_Sel->Items->Add("PBCore_2.0");
    General_Output_Sel->Items->Add("reVTMD");
    General_Output_Sel->Items->Add(Prefs->Translate(__T("Custom")).c_str());
         if (Prefs->Config(__T("Output"))==__T("Basic"))      General_Output_Sel->ItemIndex=0;
    else if (Prefs->Config(__T("Output"))==__T("Sheet"))      General_Output_Sel->ItemIndex=1;
    else if (Prefs->Config(__T("Output"))==__T("Tree"))       General_Output_Sel->ItemIndex=2;
    else if (Prefs->Config(__T("Output"))==__T("Text"))       General_Output_Sel->ItemIndex=3;
    else if (Prefs->Config(__T("Output"))==__T("HTML"))       General_Output_Sel->ItemIndex=4;
    else if (Prefs->Config(__T("Output"))==__T("XML"))        General_Output_Sel->ItemIndex=5;
    else if (Prefs->Config(__T("Output"))==__T("MPEG-7"))     General_Output_Sel->ItemIndex=6;
    else if (Prefs->Config(__T("Output"))==__T("PBCore_1.2")) General_Output_Sel->ItemIndex=7;
    else if (Prefs->Config(__T("Output"))==__T("PBCore_2.0")) General_Output_Sel->ItemIndex=8;
    else if (Prefs->Config(__T("Output"))==__T("reVTMD"))     General_Output_Sel->ItemIndex=9;
    else if (Prefs->Config(__T("Output"))==__T("Custom"))     General_Output_Sel->ItemIndex=10;
    //-Advanced
    Setup_Advanced->Caption=Prefs->Translate(__T("Advanced")).c_str();
    CB_InscrireShell->Caption=Prefs->Translate(__T("Shell extension")).c_str();
    CB_InscrireShell_Folder->Caption=Prefs->Translate(__T("Shell extension, folder")).c_str();
    CB_InfoTip->Caption=Prefs->Translate(__T("Shell InfoTip")).c_str();
    CB_ShowToolBar->Caption=Prefs->Translate(__T("Show toolbar")).c_str();
    CB_ShowMenu->Caption=Prefs->Translate(__T("Show menu")).c_str();
    Advanced_CloseAllAuto->Caption=Prefs->Translate(__T("Close all before open")).c_str();
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
    //-Custom
    Custom_Caption->Caption=Prefs->Translate(__T("Choose custom text")).c_str();
    Custom_Edit->Caption=(Prefs->Translate(__T("Edit"))+__T("...")).c_str();
    Custom_Delete->Caption=Prefs->Translate(__T("Delete")).c_str();
    Custom_New->Caption=(Prefs->Translate(__T("New"))+__T("...")).c_str();
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

