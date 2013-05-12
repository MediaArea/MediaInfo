/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
