/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
// Compilation condition
#ifndef MEDIAINFOGUI_UPDATE_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GUI_Web.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
TWebF *WebF;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TWebF::TWebF(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TWebF::Execute()
{
    Show();
    Browser->Navigate((UnicodeString(L"https://MediaArea.net/MediaInfo/ChangeLog?Name=MediaInfoGUI&Version=")+MediaInfo_Version_GUI+L"&Lib="+MediaInfo_Version_GUI).w_str());
}
//---------------------------------------------------------------------------
void __fastcall TWebF::BrowserNavigateComplete2(TObject *ASender, IDispatch * const pDisp,
          const OleVariant &URL)
{
    Hide();
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_UPDATE_NO
