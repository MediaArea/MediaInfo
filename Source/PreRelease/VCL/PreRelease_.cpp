/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PreRelease/VCL/PreRelease_.h"
#include "PreRelease/Language_Others.h"
#include "PreRelease/Language_All.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateotherlanguagefilesfromAllcsv1Click(
      TObject *Sender)
{
    Memo1->Text=Language_All_Run().c_str();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateAllcsvfromotherlanguagefiles1Click(
      TObject *Sender)
{
    Memo1->Text=Language_Others_Run().c_str();
}
//---------------------------------------------------------------------------
