// MediaInfo
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
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
