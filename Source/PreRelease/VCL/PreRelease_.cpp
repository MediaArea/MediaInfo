#pragma link "ThemeMgr"
#pragma link "TntThemeMgr"
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PReRelease/VCL/PreRelease_.h"
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
