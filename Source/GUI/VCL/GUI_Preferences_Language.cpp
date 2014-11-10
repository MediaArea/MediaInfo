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
#include "GUI/VCL/GUI_Preferences_Language.h"
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TPreferences_LanguageF::TPreferences_LanguageF(TComponent* Owner)
    : TForm(Owner)
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
    Default.Load(Prefs->BaseFolder+Prefs->FolderNames[Prefs_Language]+__T("\\")+Prefs->DefaultNames[Prefs_Language]+__T(".csv"));
    EditedLanguage.Load(Prefs->BaseFolder+Prefs->FolderNames[Prefs_Language]+__T("\\")+Name+__T(".csv"));
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

