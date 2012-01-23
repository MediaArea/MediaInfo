//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PreRelease/VCL/PreRelease_.h"
#include "PreRelease/OldFiles.h"
#include "PreRelease/Resources.h"
#include "PreRelease/Enums.h"
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
void __fastcall TForm1::Testiftherearetoooldfiles1Click(TObject *Sender)
{
    Memo1->Text=OldFiles_Test().c_str();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Updateresourcefiles1Click(TObject *Sender)
{
    Memo1->Text=Resources_Create().c_str();
    Memo1->Text=Enums_Create().c_str();
}
//---------------------------------------------------------------------------

