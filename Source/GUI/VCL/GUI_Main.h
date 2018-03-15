/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Dialogs.hpp>
#include "SHDocVw_OCX.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TMainF : public TForm
{
__published:    // IDE-managed Components
    TPageControl *Page;
    TTabSheet *Page_Easy;
    TLabel *Page_Easy_Note;
    TGroupBox *Page_Easy_A2;
    TLabel *Page_Easy_A2_Codec;
    TButton *Page_Easy_A2_Web;
    TComboBox *Page_Easy_File;
    TGroupBox *Page_Easy_V1;
    TLabel *Page_Easy_V1_Codec;
    TButton *Page_Easy_V1_Web;
    TGroupBox *Page_Easy_A1;
    TLabel *Page_Easy_A1_Codec;
    TButton *Page_Easy_A1_Web;
    TGroupBox *Page_Easy_T2;
    TLabel *Page_Easy_T2_Codec;
    TButton *Page_Easy_T2_Web;
    TGroupBox *Page_Easy_T1;
    TLabel *Page_Easy_T1_Codec;
    TButton *Page_Easy_T1_Web;
    TButton *Page_Easy_FileSelect;
    TGroupBox *Page_Easy_G1;
    TLabel *Page_Easy_G1_Codec;
    TLabel *Page_Easy_General_List_V;
    TLabel *Page_Easy_General_List_A;
    TLabel *Page_Easy_General_List_T;
    TLabel *Page_Easy_General_List_C;
    TLabel *Page_Easy_General_Tag1;
    TLabel *Page_Easy_General_Tag2;
    TLabel *Page_Easy_General_Tag3;
    TLabel *Page_Easy_General_Tag4;
    TLabel *Page_Easy_General_Tag5;
    TButton *Page_Easy_G1_Web;
    TButton *Page_Easy_DifferentView;
    TTabSheet *Page_Sheet;
    TButton *Page_Sheet_A_Web;
    TButton *Page_Sheet_V_Web;
    TButton *Page_Sheet_G_Web;
    TComboBox *Page_Sheet_T;
    TButton *Page_Sheet_T_Web;
    TComboBox *Page_Sheet_C;
    TButton *Page_Sheet_C_Web;
    TMemo *Page_Sheet_Text;
    TComboBox *Page_Sheet_G;
    TComboBox *Page_Sheet_V;
    TComboBox *Page_Sheet_A;
    TStringGrid *Page_Sheet_Sheet;
    TTabSheet *Page_Tree;
    TTreeView *Page_Tree_Tree;
    TTabSheet *Page_Text;
    TMemo *Page_Text_Text;
    TTabSheet *Page_HTML;
    TCppWebBrowser *Page_HTML_HTML;
    TTabSheet *Page_Custom;
    TMemo *Page_Custom_Text;
    TCppWebBrowser *Page_Custom_HTML;
    TTabSheet *Page_System;
    TGroupBox *Page_System_Buttons;
    TRadioButton *Page_System_Buttons_Video;
    TRadioButton *Page_System_Buttons_Audio;
    TRadioButton *Page_System_Buttons_Text;
    TListView *Page_System_Sheet;
    TToolBar *ToolBar;
    TToolButton *Tool_File;
    TToolButton *Tool_Folder;
    TToolButton *Tool_Export;
    TToolButton *Tool_Options;
    TToolButton *Tool_About;
    TToolButton *Tool_View;
    TMainMenu *MainMenu;
    TMenuItem *M_File;
    TMenuItem *M_File_Open;
    TMenuItem *M_File_Open_File;
    TMenuItem *M_File_Open_Folder;
    TMenuItem *M_File_Close;
    TMenuItem *M_File_Close_File;
    TMenuItem *M_File_Close_All;
    TMenuItem *M_File_Z4;
    TMenuItem *M_File_Export;
    TMenuItem *M_File_Z5;
    TMenuItem *M_File_Exit;
    TMenuItem *M_View;
    TMenuItem *M_View_Easy;
    TMenuItem *M_View_Sheet;
    TMenuItem *M_View_Tree;
    TMenuItem *M_View_Text;
    TMenuItem *M_View_HTML;
    TMenuItem *M_View_Custom;
    TMenuItem *N1;
    TMenuItem *M_View_System;
    TMenuItem *M_Options;
    TMenuItem *M_Options_ShowToolBar;
    TMenuItem *N5;
    TMenuItem *M_Options_CloseAllAuto;
    TMenuItem *M_Options_Z1;
    TMenuItem *M_Options_Preferences;
    TMenuItem *M_Debug;
    TMenuItem *M_Debug_Header;
    TMenuItem *M_Debug_Advanced;
    TMenuItem *M_Help;
    TMenuItem *M_Help_About;
    TMenuItem *N2;
    TMenuItem *M_Help_SupportedFormats;
    TMenuItem *M_Help_SupportedCodecs;
    TMenuItem *M_Help_SupportedParameters;
    TMenuItem *M_Language;
    TMenuItem *ToDo1;
    TImageList *Menu_Image;
    TImageList *ToolBar_Image;
    TPopupMenu *ToolBar_View_Menu;
    TMenuItem *ToolBar_View_Easy;
    TMenuItem *ToolBar_View_Sheet;
    TMenuItem *ToolBar_View_Tree;
    TMenuItem *ToolBar_View_Text;
    TMenuItem *ToolBar_View_HTML;
    TMenuItem *ToolBar_View_Custom;
    TMenuItem *N3;
    TMenuItem *ToolBar_View_System;
    TImageList *ToolBar_Image_Disabled;
    TMenuItem *M_Options_ShowMenu;
    TMenuItem *N4;
    TMenuItem *ToolBar_View_ShowMenu;
    TMenuItem *N6;
    TMenuItem *M_Debug_Dummy_Movie;
    TMenuItem *M_Debug_Dummy;
    TGroupBox *Page_Easy_T3;
    TLabel *Page_Easy_T3_Codec;
    TButton *Page_Easy_T3_Web;
    TMenuItem *M_Debug_Avanced_More;
    TOpenDialog *OpenDialog1;
    TMenuItem *M_Debug_Details0;
    TMenuItem *N7;
    TMenuItem *M_Debug_Details50;
    TMenuItem *M_Debug_Details90;
    TMenuItem *M_Debug_Details100;
    TSpeedButton *SpeedButton1;
    TBitBtn *Donate_en;
    TBitBtn *Donate_fr;
    TButton *Donate___;
    TBitBtn *Donate_de;
    TBitBtn *Donate_es;
    TBitBtn *Donate_it;
    TBitBtn *Donate_ja;
    TBitBtn *Donate_pl;
    TBitBtn *Donate_zh_CN;
    TBitBtn *Donate_zh_TW;
    TMenuItem *M_NewVersion;
    TMenuItem *M_View_XML;
    TMenuItem *M_View_JSON;
    TMenuItem *M_View_MPEG7;
    TMenuItem *M_View_PBCore;
    TMenuItem *M_View_PBCore2;
    TMenuItem *M_View_EBUCore_1_5;
    TMenuItem *M_View_EBUCore_1_6;
    TMenuItem *M_View_EBUCore_1_8_ps;
    TMenuItem *M_View_EBUCore_1_8_sp;
    TMenuItem *M_View_EBUCore_1_8_ps_json;
    TMenuItem *M_View_EBUCore_1_8_sp_json;
    TMenuItem *M_View_FIMS_1_1;
    TMenuItem *M_View_FIMS_1_2;
    TMenuItem *M_View_FIMS_1_3;
    TMenuItem *M_Tektronix;
    TToolButton *Tool_Tektronix;
    TMenuItem *M_View_reVTMD;
    TMenuItem *ToolBar_View_XML;
    TMenuItem *ToolBar_View_JSON;
    TMenuItem *ToolBar_View_MPEG7;
    TMenuItem *ToolBar_View_PBCore;
    TMenuItem *ToolBar_View_PBCore2;
    TMenuItem *ToolBar_View_EBUCore_1_5;
    TMenuItem *ToolBar_View_EBUCore_1_6;
    TMenuItem *ToolBar_View_EBUCore_1_8_ps;
    TMenuItem *ToolBar_View_EBUCore_1_8_sp;
    TMenuItem *ToolBar_View_EBUCore_1_8_ps_json;
    TMenuItem *ToolBar_View_EBUCore_1_8_sp_json;
    TMenuItem *ToolBar_View_FIMS_1_1;
    TMenuItem *ToolBar_View_FIMS_1_2;
    TMenuItem *ToolBar_View_FIMS_1_3;
    TMenuItem *ToolBar_View_reVTMD;
    TMenuItem *M_Debug_FullParsing;
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
    void __fastcall Donate_enClick(TObject *Sender);
    void __fastcall Donate_deClick(TObject *Sender);
    void __fastcall Donate_esClick(TObject *Sender);
    void __fastcall Donate_frClick(TObject *Sender);
    void __fastcall Donate_itClick(TObject *Sender);
    void __fastcall Donate_jaClick(TObject *Sender);
    void __fastcall Donate_plClick(TObject *Sender);
    void __fastcall Donate_zh_CNClick(TObject *Sender);
    void __fastcall Donate_zh_TWClick(TObject *Sender);
    void __fastcall Donate___Click(TObject *Sender);
    void __fastcall M_NewVersionClick(TObject *Sender);
    void __fastcall M_View_MPEG7Click(TObject *Sender);
    void __fastcall M_View_PBCoreClick(TObject *Sender);
    void __fastcall M_View_PBCore2Click(TObject *Sender);
    void __fastcall M_View_EBUCore_1_5Click(TObject *Sender);
    void __fastcall M_View_EBUCore_1_6Click(TObject *Sender);
    void __fastcall M_View_EBUCore_1_8_psClick(TObject *Sender);
    void __fastcall M_View_EBUCore_1_8_spClick(TObject *Sender);
    void __fastcall M_View_EBUCore_1_8_ps_jsonClick(TObject *Sender);
    void __fastcall M_View_EBUCore_1_8_sp_jsonClick(TObject *Sender);
    void __fastcall M_View_FIMS_1_1Click(TObject *Sender);
    void __fastcall M_View_FIMS_1_2Click(TObject *Sender);
    void __fastcall M_View_FIMS_1_3Click(TObject *Sender);
    void __fastcall M_View_XMLClick(TObject *Sender);
    void __fastcall M_View_JSONClick(TObject *Sender);
    void __fastcall M_TektronixClick(TObject *Sender);
    void __fastcall M_View_reVTMDClick(TObject *Sender);
    void __fastcall M_Debug_FullParsingClick(TObject *Sender);
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
