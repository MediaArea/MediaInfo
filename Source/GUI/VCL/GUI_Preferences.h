// GUI_Preferences - Preferences interface of MediaInfo
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
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

//---------------------------------------------------------------------------
#ifndef GUI_PreferencesH
#define GUI_PreferencesH
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <TntForms.hpp>
#include <TntComCtrls.hpp>
#include <TntStdCtrls.hpp>
#include "GUI_Main.h"
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TPreferencesF : public TTntForm
{
__published:    // IDE-managed Components
    TTntButton *OK;
    TTntTreeView *Tree;
    TTntButton *Cancel;
    TTntPageControl *Page;
    TTntTabSheet *Setup;
    TTntLabel *Langue_C;
    TTntLabel *Output_C;
    TTntComboBox *General_Language_Sel;
    TTntComboBox *General_Output_Sel;
    TTntCheckBox *CB_CheckUpdate;
    TTntButton *General_Language_More;
    TTntButton *General_Output_More;
    TTntTabSheet *Setup_Advanced;
    TTntCheckBox *CB_ShowToolBar;
    TTntCheckBox *Advanced_CloseAllAuto;
    TTntTabSheet *Customize_Language;
    TTntLabel *Language_Caption;
    TTntButton *Language_New;
    TTntButton *Language_Delete;
    TTntButton *Language_Edit;
    TTntComboBox *Language_Sel;
    TTntTabSheet *Customize_Sheet;
    TTntLabel *Sheet_Caption;
    TTntComboBox *Sheet_Sel;
    TTntButton *Sheet_Delete;
    TTntButton *Sheet_Edit;
    TTntButton *Sheet_New;
    TTntTabSheet *Customize_TreeText;
    TTntLabel *Tree_NotYet;
    TTntTabSheet *Customize_Custom;
    TTntLabel *Custom_Caption;
    TTntButton *Custom_New;
    TTntButton *Custom_Edit;
    TTntButton *Custom_Delete;
    TTntComboBox *Custom_Sel;
    TTntTabSheet *Customize;
    TTntCheckBox *CB_ShowMenu;
    TTntCheckBox *CB_InfoTip;
    TTntCheckBox *CB_InscrireShell;
    void __fastcall General_Language_SelChange(TObject *Sender);
    void __fastcall General_Output_SelChange(TObject *Sender);
    void __fastcall Custom_EditClick(TObject *Sender);
    void __fastcall Language_EditClick(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall Custom_SelChange(TObject *Sender);
    void __fastcall CB_InscrireShellClick(TObject *Sender);
    void __fastcall CB_CheckUpdateClick(TObject *Sender);
    void __fastcall Language_NewClick(TObject *Sender);
    void __fastcall Language_DeleteClick(TObject *Sender);
    void __fastcall Custom_NewClick(TObject *Sender);
    void __fastcall Custom_DeleteClick(TObject *Sender);
    void __fastcall TreeChange(TObject *Sender, TTreeNode *Node);
    void __fastcall General_Language_MoreClick(TObject *Sender);
    void __fastcall General_Output_MoreClick(TObject *Sender);
    void __fastcall CB_ShowToolBarClick(TObject *Sender);
    void __fastcall Language_SelChange(TObject *Sender);
    void __fastcall Advanced_CloseAllAutoClick(TObject *Sender);
    void __fastcall Setup_GeneralShow(TObject *Sender);
    void __fastcall Setup_AdvancedShow(TObject *Sender);
    void __fastcall Customize_LanguageShow(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Customize_CustomShow(TObject *Sender);
    void __fastcall Sheet_SelChange(TObject *Sender);
    void __fastcall Sheet_DeleteClick(TObject *Sender);
    void __fastcall Sheet_EditClick(TObject *Sender);
    void __fastcall Sheet_NewClick(TObject *Sender);
    void __fastcall Customize_SheetShow(TObject *Sender);
    void __fastcall CB_ShowMenuClick(TObject *Sender);
    void __fastcall CB_InfoTipClick(TObject *Sender);
private:    // User declarations
public:        // User declarations
    __fastcall TPreferencesF(TComponent* Owner);
    void __fastcall ComboBox_Update(TTntComboBox *CB, Prefs_t List);
    void __fastcall ComboBox_SelChange(TTntComboBox *CB, Prefs_t List);
    void __fastcall GUI_Configure();
};
//---------------------------------------------------------------------------
extern PACKAGE TPreferencesF *PreferencesF;
//---------------------------------------------------------------------------
#endif
