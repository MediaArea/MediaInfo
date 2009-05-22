// GUI_Preferences - Preferences interface of MediaInfo
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
// Preferences interface of MediaInfo
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
#pragma package(smart_init)
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
    : TTntForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::ComboBox_Update(TTntComboBox *CB, Prefs_t List)
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
void __fastcall TPreferencesF::ComboBox_SelChange(TTntComboBox *CB, Prefs_t List)
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
        Prefs->Config(_T("Output"))=_T("Basic");
        MainF->Page->ActivePage=MainF->Page_Easy;
        MainF->Page_Easy_FileChange(NULL);
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==1)
    {
        Prefs->Config(_T("Output"))=_T("Sheet");
        MainF->Page->ActivePage=MainF->Page_Sheet;
        General_Output_More->Visible=true;
    }
    if (General_Output_Sel->ItemIndex==2)
    {
        Prefs->Config(_T("Output"))=_T("Tree");
        MainF->Page->ActivePage=MainF->Page_Tree;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==3)
    {
        Prefs->Config(_T("Output"))=_T("Text");
        MainF->Page->ActivePage=MainF->Page_Tree;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==4)
    {
        Prefs->Config(_T("Output"))=_T("HTML");
        MainF->Page->ActivePage=MainF->Page_HTML;
        General_Output_More->Visible=false;
    }
    if (General_Output_Sel->ItemIndex==5)
    {
        Prefs->Config(_T("Output"))=_T("Custom");
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
    Prefs->Config(_T("Sheet"))=GUI_Text(Sheet_Sel->Text);
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
    AnsiString S1="New";
    if (!InputQuery(_T("New sheet"), _T("Enter name of new sheet"), S1))
        return;

    Prefs->Create(Prefs_Sheet, Ztring().From_Local(S1.c_str()));
    ComboBox_Update(Sheet_Sel, Prefs_Sheet);

    //Selecting and edit
    Sheet_Sel->ItemIndex=Prefs->FilesList[Prefs_Sheet].Find(Ztring().From_Local(S1.c_str()));
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
    Prefs->Config(_T("Custom"))=GUI_Text(Custom_Sel->Text);
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
        Prefs->Config(_T("CheckUpdate"), 1)=_T("1");
    }
    else
        Prefs->Config(_T("CheckUpdate"), 1)=_T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_InscrireShellClick(TObject *Sender)
{
    //Shell extension
    if (CB_InscrireShell->Checked)
        Prefs->Config(_T("ShellExtension"), 1)=_T("1");
    else
        Prefs->Config(_T("ShellExtension"), 1)=_T("0");
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_InscrireShell_FolderClick(TObject *Sender)
{
    //Shell extension
    if (CB_InscrireShell_Folder->Checked)
        Prefs->Config(_T("ShellExtension_Folder"), 1)=_T("1");
    else
        Prefs->Config(_T("ShellExtension_Folder"), 1)=_T("0");

}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_InfoTipClick(TObject *Sender)
{
    //ToolTip
    if (CB_InfoTip->Checked)
    {
        Prefs->Config(_T("ShellInfoTip"), 1)=_T("1");
        ShellExecute (NULL, _T("open"), _T("regsvr32"), _T("MediaInfo_InfoTip.dll /s"), NULL, 0);
    }
    else
    {
        Prefs->Config(_T("ShellInfoTip"), 1)=_T("0");
        ShellExecute (NULL, _T("open"), _T("regsvr32"), _T("MediaInfo_InfoTip.dll /s /u"), NULL, 0);
    }
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_ShowToolBarClick(TObject *Sender)
{
    Prefs->Config(_T("ShowToolBar"), 1)=CB_ShowToolBar->Checked?_T("1"):_T("0");
    MainF->M_Options_ShowToolBar->Checked=CB_ShowToolBar->Checked;
    MainF->M_Options_ShowToolBarClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::CB_ShowMenuClick(TObject *Sender)
{
    Prefs->Config(_T("ShowMenu"), 1)=CB_ShowMenu->Checked?_T("1"):_T("0");
    MainF->M_Options_ShowMenu->Checked=CB_ShowMenu->Checked;
    MainF->M_Options_ShowMenuClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Advanced_CloseAllAutoClick(TObject *Sender)
{
    if (Advanced_CloseAllAuto->Checked)
        Prefs->Config(_T("CloseAllAuto"), 1)=_T("1");
    else
        Prefs->Config(_T("CloseAllAuto"), 1)=_T("0");
    MainF->M_Options_CloseAllAuto->Checked=Advanced_CloseAllAuto->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Language_NewClick(TObject *Sender)
{
    AnsiString S1="New";
    if (!InputQuery(_T("New language"), _T("Enter name of new language"), S1))
        return;

    Prefs->Create(Prefs_Language, Ztring().From_Local(S1.c_str()));
    ComboBox_Update(General_Language_Sel, Prefs_Language);
    ComboBox_Update(Language_Sel, Prefs_Language);

    //Selecting and edit
    Language_Sel->ItemIndex=Prefs->FilesList[Prefs_Language].Find(Ztring().From_Local(S1.c_str()));
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
    AnsiString S1=_T("New");
    if (!InputQuery(_T("New Output"), _T("Enter name of new Output"), S1))
        return;

    Prefs->Create(Prefs_Custom, Ztring().From_Local(S1.c_str()));
    ComboBox_Update(Custom_Sel, Prefs_Custom);

    //Selecting and edit
    Custom_Sel->ItemIndex=Prefs->FilesList[Prefs_Custom].Find(Ztring().From_Local(S1.c_str()));
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
    else if (General_Output_Sel->ItemIndex==5)
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
    CB_CheckUpdate->Checked=Prefs->Config(_T("CheckUpdate")).To_int32s();
    CB_InscrireShell->Checked=Prefs->Config(_T("ShellExtension")).To_int32s(); //Lecture Shell extension
    CB_InscrireShell_Folder->Checked=Prefs->Config(_T("ShellExtension_Folder")).To_int32s(); //Lecture Shell extension
    CB_InfoTip->Checked=Prefs->Config(_T("ShellInfoTip")).To_int32s(); //Lecture Shell extension
}

//---------------------------------------------------------------------------
void __fastcall TPreferencesF::Setup_AdvancedShow(TObject *Sender)
{
    CB_ShowToolBar->Checked=Prefs->Config(_T("ShowToolBar")).To_int32s();
    CB_ShowMenu->Checked=Prefs->Config(_T("ShowMenu")).To_int32s();
    Advanced_CloseAllAuto->Checked=Prefs->Config(_T("CloseAllAuto")).To_int32s();
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
    Caption=Prefs->Translate(_T("Preferences")).c_str();
    //Buttons
    OK->Caption=Prefs->Translate(_T("OK")).c_str();
    Cancel->Caption=Prefs->Translate(_T("Cancel")).c_str();
    //-Left panel
    Tree->Items->Item[0]->Text=Prefs->Translate(_T("Setup")).c_str();
    Tree->Items->Item[1]->Text=Prefs->Translate(_T("Advanced")).c_str();
    Tree->Items->Item[2]->Text=Prefs->Translate(_T("Customize")).c_str();
    Tree->Items->Item[3]->Text=Prefs->Translate(_T("Sheet")).c_str();
    Tree->Items->Item[4]->Text=Prefs->Translate(_T("Tree & Text")).c_str();
    Tree->Items->Item[5]->Text=Prefs->Translate(_T("Custom")).c_str();
    Tree->Items->Item[6]->Text=Prefs->Translate(_T("Language")).c_str();
    //-General
    General_Language_More->Caption=(Prefs->Translate(_T("More"))+_T("...")).c_str();
    Langue_C->Caption=Prefs->Translate(_T("Language")).c_str();
    Output_C->Caption=Prefs->Translate(_T("Output format")).c_str();
    General_Output_More->Caption=(Prefs->Translate(_T("More"))+_T("...")).c_str();
    CB_CheckUpdate->Caption=Prefs->Translate(_T("Newest version")).c_str();
    //--Special case : Output
    General_Output_Sel->Items->Clear();
    General_Output_Sel->Items->Add(Prefs->Translate(_T("Basic")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(_T("Sheet")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(_T("Tree")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(_T("Text")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(_T("HTML")).c_str());
    General_Output_Sel->Items->Add(Prefs->Translate(_T("Custom")).c_str());
         if (Prefs->Config(_T("Output"))==_T("Basic")) General_Output_Sel->ItemIndex=0;
    else if (Prefs->Config(_T("Output"))==_T("Sheet"))  General_Output_Sel->ItemIndex=1;
    else if (Prefs->Config(_T("Output"))==_T("Tree"))   General_Output_Sel->ItemIndex=2;
    else if (Prefs->Config(_T("Output"))==_T("Text"))   General_Output_Sel->ItemIndex=3;
    else if (Prefs->Config(_T("Output"))==_T("HTML"))   General_Output_Sel->ItemIndex=4;
    else if (Prefs->Config(_T("Output"))==_T("Custom")) General_Output_Sel->ItemIndex=5;
    //-Advanced
    Setup_Advanced->Caption=Prefs->Translate(_T("Advanced")).c_str();
    CB_InscrireShell->Caption=Prefs->Translate(_T("Shell extension")).c_str();
    CB_InscrireShell_Folder->Caption=Prefs->Translate(_T("Shell extension, folder")).c_str();
    CB_InfoTip->Caption=Prefs->Translate(_T("Shell InfoTip")).c_str();
    CB_ShowToolBar->Caption=Prefs->Translate(_T("Show toolbar")).c_str();
    CB_ShowMenu->Caption=Prefs->Translate(_T("Show menu")).c_str();
    Advanced_CloseAllAuto->Caption=Prefs->Translate(_T("Close all before open")).c_str();
    //-Language
    Language_Caption->Caption=Prefs->Translate(_T("Choose language")).c_str();
    Language_Edit->Caption=(Prefs->Translate(_T("Edit"))+_T("...")).c_str();
    Language_Delete->Caption=Prefs->Translate(_T("Delete")).c_str();
    Language_New->Caption=(Prefs->Translate(_T("New"))+_T("...")).c_str();
    //-Sheet
    Sheet_Caption->Caption=Prefs->Translate(_T("Choose custom sheet")).c_str();
    Sheet_Edit->Caption=(Prefs->Translate(_T("Edit"))+_T("...")).c_str();
    Sheet_Delete->Caption=Prefs->Translate(_T("Delete")).c_str();
    Sheet_New->Caption=(Prefs->Translate(_T("New"))+_T("...")).c_str();
    //-Tree
    Tree_NotYet->Caption=(Prefs->Translate(_T("Not yet"))+_T("...")).c_str();
    //-Custom
    Custom_Caption->Caption=Prefs->Translate(_T("Choose custom text")).c_str();
    Custom_Edit->Caption=(Prefs->Translate(_T("Edit"))+_T("...")).c_str();
    Custom_Delete->Caption=Prefs->Translate(_T("Delete")).c_str();
    Custom_New->Caption=(Prefs->Translate(_T("New"))+_T("...")).c_str();
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

