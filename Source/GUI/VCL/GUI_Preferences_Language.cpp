// GUI_Preferences_Language - Preferences interface of MediaInfo
// Copyright (C) 2002-2005 Jerome Martinez, Zen@MediaArea.net
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
// Compilation condition
#ifndef MEDIAINFOGUI_PREFS_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GUI/VCL/GUI_Preferences_Language.h"
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TntGrids"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TPreferences_LanguageF::TPreferences_LanguageF(TComponent* Owner)
    : TTntForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPreferences_LanguageF::GrilleKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    EditedLanguage(GUI_Text(Grille->Cells[0][Grille->Row]))=GUI_Text(Grille->Cells[2][Grille->Row]);
}
//---------------------------------------------------------------------------
void __fastcall TPreferences_LanguageF::OKClick(TObject *Sender)
{
    EditedLanguage.Save();
}
//---------------------------------------------------------------------------
int TPreferences_LanguageF::Run(const Ztring &Name)
{
    ZtringListListF Default;
    Default.Load(Prefs->BaseFolder+Prefs->FolderNames[Language]+_T("\\")+Prefs->DefaultNames[Language]+_T(".csv"));
    EditedLanguage.Load(Prefs->BaseFolder+Prefs->FolderNames[Language]+_T("\\")+Name+_T(".csv"));
    Grille->RowCount=Default.size()+1;
    Grille->Cells[0][0]="Program name";
    Grille->Cells[1][0]="Default translation";
    Grille->Cells[2][0]="Your translation";
    for (size_t Pos=0; Pos<Default.size(); Pos++)
    {
        Grille->Cells[0][Pos+1]=Default(Pos, 0).c_str();
        Grille->Cells[1][Pos+1]=Default(Pos, 1).c_str();
        Grille->Cells[2][Pos+1]=EditedLanguage(Default(Pos, 0), 1).c_str();
    }

    return ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_LanguageF::FormResize(TObject *Sender)
{
    Grille->Width=ClientWidth-Grille->Left;
    Grille->Height=ClientHeight-Grille->Top; //Why 40? I don't know!
    Grille->ColWidths[0]=Grille->Width*1/7;
    Grille->ColWidths[1]=Grille->Width*3/7;
    Grille->ColWidths[2]=Grille->Width*3/7;
    OK->Left=ClientWidth-OK->Width;
    Cancel->Left=OK->Left-Cancel->Width;
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

