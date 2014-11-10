/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
