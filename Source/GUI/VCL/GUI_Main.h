// GUI_Main - Main interface of MediaInfo
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

//---------------------------------------------------------------------------
#ifndef GUI_MainH
#define GUI_MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <OleCtrls.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <TntForms.hpp>
#include <TntComCtrls.hpp>
#include <TntGrids.hpp>
#include <TntMenus.hpp>
#include <TntStdCtrls.hpp>
#include <TntDialogs.hpp>
#include "SHDocVw_OCX.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TMainF : public TTntForm
{
__published:    // IDE-managed Components
    TTntPageControl *Page;
    TTntTabSheet *Page_Easy;
    TTntLabel *Page_Easy_Note;
    TTntGroupBox *Page_Easy_A2;
    TTntLabel *Page_Easy_A2_Codec;
    TTntButton *Page_Easy_A2_Web;
    TTntComboBox *Page_Easy_File;
    TTntGroupBox *Page_Easy_V1;
    TTntLabel *Page_Easy_V1_Codec;
    TTntButton *Page_Easy_V1_Web;
    TTntGroupBox *Page_Easy_A1;
    TTntLabel *Page_Easy_A1_Codec;
    TTntButton *Page_Easy_A1_Web;
    TTntGroupBox *Page_Easy_T2;
    TTntLabel *Page_Easy_T2_Codec;
    TTntButton *Page_Easy_T2_Web;
    TTntGroupBox *Page_Easy_T1;
    TTntLabel *Page_Easy_T1_Codec;
    TTntButton *Page_Easy_T1_Web;
    TTntButton *Page_Easy_FileSelect;
    TTntGroupBox *Page_Easy_G1;
    TTntLabel *Page_Easy_G1_Codec;
    TTntLabel *Page_Easy_General_List_V;
    TTntLabel *Page_Easy_General_List_A;
    TTntLabel *Page_Easy_General_List_T;
    TTntLabel *Page_Easy_General_List_C;
    TTntLabel *Page_Easy_General_Tag1;
    TTntLabel *Page_Easy_General_Tag2;
    TTntLabel *Page_Easy_General_Tag3;
    TTntLabel *Page_Easy_General_Tag4;
    TTntLabel *Page_Easy_General_Tag5;
    TTntButton *Page_Easy_G1_Web;
    TTntButton *Page_Easy_DifferentView;
    TTntTabSheet *Page_Sheet;
    TTntButton *Page_Sheet_A_Web;
    TTntButton *Page_Sheet_V_Web;
    TTntButton *Page_Sheet_G_Web;
    TTntComboBox *Page_Sheet_T;
    TTntButton *Page_Sheet_T_Web;
    TTntComboBox *Page_Sheet_C;
    TTntButton *Page_Sheet_C_Web;
    TTntMemo *Page_Sheet_Text;
    TTntComboBox *Page_Sheet_G;
    TTntComboBox *Page_Sheet_V;
    TTntComboBox *Page_Sheet_A;
    TTntStringGrid *Page_Sheet_Sheet;
    TTntTabSheet *Page_Tree;
    TTntTreeView *Page_Tree_Tree;
    TTntTabSheet *Page_Text;
    TTntMemo *Page_Text_Text;
    TTntTabSheet *Page_HTML;
    TCppWebBrowser *Page_HTML_HTML;
    TTntTabSheet *Page_Custom;
    TTntMemo *Page_Custom_Text;
    TCppWebBrowser *Page_Custom_HTML;
    TTntTabSheet *Page_System;
    TTntGroupBox *Page_System_Buttons;
    TTntRadioButton *Page_System_Buttons_Video;
    TTntRadioButton *Page_System_Buttons_Audio;
    TTntRadioButton *Page_System_Buttons_Text;
    TTntListView *Page_System_Sheet;
    TTntToolBar *ToolBar;
    TTntToolButton *Tool_File;
    TTntToolButton *Tool_Folder;
    TTntToolButton *Tool_Export;
    TTntToolButton *Tool_Options;
    TTntToolButton *Tool_About;
    TTntToolButton *Tool_View;
    TTntMainMenu *MainMenu;
    TTntMenuItem *M_File;
    TTntMenuItem *M_File_Open;
    TTntMenuItem *M_File_Open_File;
    TTntMenuItem *M_File_Open_Folder;
    TTntMenuItem *M_File_Close;
    TTntMenuItem *M_File_Close_File;
    TTntMenuItem *M_File_Close_All;
    TTntMenuItem *M_File_Z4;
    TTntMenuItem *M_File_Export;
    TTntMenuItem *M_File_Z5;
    TTntMenuItem *M_File_Exit;
    TTntMenuItem *M_View;
    TTntMenuItem *M_View_Easy;
    TTntMenuItem *M_View_Sheet;
    TTntMenuItem *M_View_Tree;
    TTntMenuItem *M_View_Text;
    TTntMenuItem *M_View_HTML;
    TTntMenuItem *M_View_Custom;
    TTntMenuItem *N1;
    TTntMenuItem *M_View_System;
    TTntMenuItem *M_Options;
    TTntMenuItem *M_Options_ShowToolBar;
    TTntMenuItem *N5;
    TTntMenuItem *M_Options_CloseAllAuto;
    TTntMenuItem *M_Options_Z1;
    TTntMenuItem *M_Options_Preferences;
    TTntMenuItem *M_Debug;
    TTntMenuItem *M_Debug_Header;
    TTntMenuItem *M_Debug_Advanced;
    TTntMenuItem *M_Help;
    TTntMenuItem *M_Help_About;
    TTntMenuItem *N2;
    TTntMenuItem *M_Help_SupportedFormats;
    TTntMenuItem *M_Help_SupportedCodecs;
    TTntMenuItem *M_Help_SupportedParameters;
    TTntMenuItem *M_Language;
    TTntMenuItem *ToDo1;
    TImageList *Menu_Image;
    TImageList *ToolBar_Image;
    TTntPopupMenu *ToolBar_View_Menu;
    TTntMenuItem *ToolBar_View_Easy;
    TTntMenuItem *ToolBar_View_Sheet;
    TTntMenuItem *ToolBar_View_Tree;
    TTntMenuItem *ToolBar_View_Text;
    TTntMenuItem *ToolBar_View_HTML;
    TTntMenuItem *ToolBar_View_Custom;
    TTntMenuItem *N3;
    TTntMenuItem *ToolBar_View_System;
    TImageList *ToolBar_Image_Disabled;
    TTntMenuItem *M_Options_ShowMenu;
    TTntMenuItem *N4;
    TTntMenuItem *ToolBar_View_ShowMenu;
    TTntMenuItem *N6;
    TTntMenuItem *M_Debug_Dummy_Movie;
    TTntMenuItem *M_Debug_Dummy;
    TTntGroupBox *Page_Easy_T3;
    TTntLabel *Page_Easy_T3_Codec;
    TTntButton *Page_Easy_T3_Web;
    TTntMenuItem *M_Debug_Avanced_More;
    TTntOpenDialog *OpenDialog1;
    TTntMenuItem *M_Debug_Details0;
    TTntMenuItem *N7;
    TTntMenuItem *M_Debug_Details50;
    TTntMenuItem *M_Debug_Details90;
    TTntMenuItem *M_Debug_Details100;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall M_Help_AboutClick(TObject *Sender);
    void __fastcall M_Options_PreferencesClick(TObject *Sender);
    void __fastcall M_Debug_HeaderClick(TObject *Sender);
    void __fastcall M_File_ExitClick(TObject *Sender);
    void __fastcall M_File_Open_FileClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall M_Debug_AdvancedClick(TObject *Sender);
    void __fastcall M_Options_ShowToolBarClick(TObject *Sender);
    void __fastcall M_View_EasyClick(TObject *Sender);
    void __fastcall M_View_SheetClick(TObject *Sender);
    void __fastcall M_View_TreeClick(TObject *Sender);
    void __fastcall M_View_TextClick(TObject *Sender);
    void __fastcall M_View_HTMLClick(TObject *Sender);
    void __fastcall M_View_CustomClick(TObject *Sender);
    void __fastcall M_View_SystemClick(TObject *Sender);
    void __fastcall M_Help_SupportedCodecsClick(TObject *Sender);
    void __fastcall M_Help_SupportedFormatsClick(TObject *Sender);
    void __fastcall Page_System_Buttons_VideoClick(TObject *Sender);
    void __fastcall Page_System_Buttons_AudioClick(TObject *Sender);
    void __fastcall Page_System_Buttons_TextClick(TObject *Sender);
    void __fastcall Page_System_SheetColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall Page_System_SheetCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
    void __fastcall M_File_Open_FolderClick(TObject *Sender);
    void __fastcall M_File_Close_FileClick(TObject *Sender);
    void __fastcall M_File_Close_AllClick(TObject *Sender);
    void __fastcall M_LanguageClick(TObject *Sender);
    void __fastcall M_Help_SupportedParametersClick(TObject *Sender);
    void __fastcall M_File_ExportClick(TObject *Sender);
    void __fastcall Page_Sheet_SheetSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall Page_Easy_FileChange(TObject *Sender);
    void __fastcall Page_Easy_DifferentViewClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Page_Sheet_Change(TObject *Sender);
    void __fastcall Page_Sheet_WebClick(TObject *Sender);
    void __fastcall Page_Easy_WebClick(TObject *Sender);
    void __fastcall M_Options_ShowMenuClick(TObject *Sender);
    void __fastcall M_Debug_Dummy_MovieClick(TObject *Sender);
    void __fastcall M_Debug_DummyClick(TObject *Sender);
private:    // User declarations
public:        // User declarations
    MESSAGE void __fastcall HandleDropFiles (TMessage&);
    BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_DROPFILES, TMessage, HandleDropFiles);
    END_MESSAGE_MAP(TForm);

    __fastcall TMainF(TComponent* Owner);
    void __fastcall GUI_Configure();
    void __fastcall Translate();
    void __fastcall Refresh(TTabSheet *Page=NULL);
    void __fastcall ChangePage(TTabSheet *Page);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainF *MainF;
//---------------------------------------------------------------------------
#endif
