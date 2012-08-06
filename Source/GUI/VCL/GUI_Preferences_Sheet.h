// GUI_Preferences_Sheet - Preferences interface of MediaInfo
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
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

#ifndef GUI_Preferences_SheetH
#define GUI_Preferences_SheetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ZenLib/ZtringListListF.h>
using namespace ZenLib;
#define SHEETF_COLUMNS 100
//---------------------------------------------------------------------------
class TPreferences_SheetF : public TForm
{
__published:    // IDE-managed Components
    TLabel *Column_Text0;
    TComboBox *Column_Parameter0;
    TComboBox *Column_Kind0;
    TLabel *Label6;
    TComboBox *ColumnsCount;
    TButton *OK;
    TButton *Cancel;
    TStringGrid *Sheet_Show;
    TComboBox *Column_Size0;
    TComboBox *Column_Pos0;
    void __fastcall ColumnsCountChange(TObject *Sender);
    void __fastcall Column_Kind0Change(TObject *Sender);
    void __fastcall Column_Parameter0Change(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall Column_Size0Change(TObject *Sender);
    void __fastcall Column_Pos0Change(TObject *Sender);
private:    // User declarations
    ZtringListListF EditedSheet;
    void Columns_Adapt();
    int Columns_Count;
    TComboBox *Column_Kind[SHEETF_COLUMNS];
    TComboBox *Column_Pos[SHEETF_COLUMNS];
    TComboBox *Column_Parameter[SHEETF_COLUMNS];
    TComboBox *Column_Size[SHEETF_COLUMNS];
    TLabel *Column_Text[SHEETF_COLUMNS];
    ZtringListList Infos_Parameters[SHEETF_COLUMNS];
public:        // User declarations
    __fastcall TPreferences_SheetF(TComponent* Owner);
    int Run(const Ztring &Name);
};
//---------------------------------------------------------------------------
extern PACKAGE TPreferences_SheetF *Preferences_SheetF;
//---------------------------------------------------------------------------
#endif
