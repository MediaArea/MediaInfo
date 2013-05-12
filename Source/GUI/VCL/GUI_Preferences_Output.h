/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
