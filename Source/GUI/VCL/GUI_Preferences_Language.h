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
//
// Preferences interface of MediaInfo (Language definition part)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//---------------------------------------------------------------------------
#ifndef GUI_Preferences_LanguageH
#define GUI_Preferences_LanguageH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <Grids.hpp>
#include <Forms.hpp>
#include <ZenLib/ZtringListListF.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

class TPreferences_LanguageF : public TForm
{
__published:    // IDE-managed Components
    TStringGrid *Grille;
    TButton *OK;
    TButton *Cancel;
    void __fastcall GrilleKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:    // User declarations
    ZtringListListF EditedLanguage;
public:     // User declarations
    __fastcall TPreferences_LanguageF(TComponent* Owner);
    int Run(const Ztring &Name);
};
#endif
