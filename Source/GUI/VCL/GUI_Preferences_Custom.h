/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
