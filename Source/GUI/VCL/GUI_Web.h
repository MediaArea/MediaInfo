// GUI_Web - Web page display
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
