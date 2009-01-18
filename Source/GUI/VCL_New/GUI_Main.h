// GUI_Main - VCL GUI for MediaInfo
// Copyright (C) 2007-2009 Jerome Martinez, Zen@MediaArea.net
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
// VCL GUI for MediaInfo
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_MainH
#define GUI_MainH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Classes.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TGUI_Main_ : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *Menu_File;
    TMenuItem *Menu_File_Open;
    TMenuItem *Menu_File_Open_File;
    TOpenDialog *OpenDialog1;
    TMenuItem *Menu_View;
    TMenuItem *Menu_View_Easy;
    TMenuItem *Menu_View_Sheet;
    TMenuItem *Menu_View_Tree;
    TMenuItem *Menu_View_HTML;
    TMenuItem *Menu_View_Text;
    TMenuItem *Menu_File_Open_File_Directory;
    TMenuItem *N1;
    TMenuItem *Menu_File_Exit;
    TMenuItem *Menu_Debug_Debug;
    TMenuItem *Menu_Debug_Complete;
    TMenuItem *Menu_Debug_Details;
    TMenuItem *Menu_Debug_Demux;
    TMenuItem *Menu_Help;
    TMenuItem *Menu_Help_About;
    TMenuItem *Menu_Debug_Demux_No;
    TMenuItem *Menu_Debug_Demux_Element;
    TMenuItem *Menu_Debug_Demux_All;
    void __fastcall Menu_File_Open_FileClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall Menu_View_EasyClick(TObject *Sender);
    void __fastcall Menu_View_SheetClick(TObject *Sender);
    void __fastcall Menu_View_TreeClick(TObject *Sender);
    void __fastcall Menu_View_HTMLClick(TObject *Sender);
    void __fastcall Menu_View_TextClick(TObject *Sender);
    void __fastcall Menu_File_ExitClick(TObject *Sender);
    void __fastcall Menu_Debug_DetailsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TGUI_Main_(TComponent* Owner);
    __fastcall ~TGUI_Main_();

private:
    //Non-GUI Elements
    Core* C;

    //GUI
    GUI_Main_Common_Core* View;
};
//---------------------------------------------------------------------------
extern PACKAGE TGUI_Main_ *GUI_Main_;
//---------------------------------------------------------------------------
#endif
