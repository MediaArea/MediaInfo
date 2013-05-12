/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef GUI_WebH
#define GUI_WebH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TWebF : public TForm
{
__published:    // IDE-managed Components
    TCppWebBrowser *Browser;
    void __fastcall BrowserNavigateComplete2(TObject *Sender, LPDISPATCH pDisp,
          Variant *URL);
private:    // User declarations
public:        // User declarations
    __fastcall TWebF(TComponent* Owner);
    void __fastcall Execute();
};
//---------------------------------------------------------------------------
extern PACKAGE TWebF *WebF;
//---------------------------------------------------------------------------
#endif
