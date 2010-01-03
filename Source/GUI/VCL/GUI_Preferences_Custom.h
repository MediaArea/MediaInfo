// GUI_Preferences_Custom - Preferences interface of MediaInfo
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
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
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------

#ifndef GUI_Preferences_CustomH
#define GUI_Preferences_CustomH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ZenLib/ZtringListListF.h>
using namespace ZenLib;
//---------------------------------------------------------------------------
class TPreferences_CustomF : public TForm
{
__published:    // IDE-managed Components
    TComboBox *Liste;
    TMemo *Memo;
    TButton *OK;
    TComboBox *Infos;
    TButton *Repeat;
    TButton *Cancel;
    TButton *Help;
    void __fastcall ListeChange(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall InfosChange(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall HelpClick(TObject *Sender);
private:    // User declarations
    ZtringListListF EditedCustom;
    int ItemIndex;
    ZtringListList Infos_Parameters;
public:        // User declarations
    __fastcall TPreferences_CustomF(TComponent* Owner);
    int Run(const Ztring &Name);
};
#endif
