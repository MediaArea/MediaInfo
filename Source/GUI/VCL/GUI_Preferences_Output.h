// IPreferences_Output - Preferences interface of MediaInfo
// Copyright (C) 2002-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Version 0.4.0
// -------------
// Preferences interface of MediaInfo (Output definition part)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 0.4.0 (Zen@MediaArea.net, 2003-07-21)
// Adapt to new architecture and being GPL
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------

#ifndef GUI_Preferences_OutputH
#define GUI_Preferences_OutputH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ZenLib/ZtringListListF.h>
using namespace ZenLib;
//---------------------------------------------------------------------------
class TPreferences_OutputF : public TForm
{
__published:    // IDE-managed Components
    TComboBox *Liste;
    TMemo *Memo;
    TButton *OK;
    TComboBox *Infos;
    TButton *Button1;
    TButton *Cancel;
    void __fastcall ListeChange(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall InfosChange(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TPreferences_OutputF(TComponent* Owner);
    ZtringListListF Output;
    int Afficher(const Ztring &Folder, const Ztring &Name);
    Ztring Nom; //Nom en cours
    Ztring Repertoire; //Repertoire en cours

    int AncienItemIndex;
};
//---------------------------------------------------------------------------
extern PACKAGE TPreferences_OutputF *Preferences_OutputF;
//---------------------------------------------------------------------------
#endif
