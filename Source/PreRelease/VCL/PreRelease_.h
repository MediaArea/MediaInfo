//---------------------------------------------------------------------------

#ifndef PreRelease_H
#define PreRelease_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMemo *Memo1;
    TMenuItem *File1;
    TMenuItem *CreateotherlanguagefilesfromAllcsv1;
    TMenuItem *UpdateAllcsvfromotherlanguagefiles1;
    void __fastcall CreateotherlanguagefilesfromAllcsv1Click(
          TObject *Sender);
    void __fastcall UpdateAllcsvfromotherlanguagefiles1Click(
          TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
