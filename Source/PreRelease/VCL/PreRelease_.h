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
__published:    // IDE-managed Components
    TMainMenu *MainMenu1;
    TMemo *Memo1;
    TMenuItem *File1;
    TMenuItem *Testiftherearetoooldfiles1;
    TMenuItem *Updateresourcefiles1;
    void __fastcall Testiftherearetoooldfiles1Click(TObject *Sender);
    void __fastcall Updateresourcefiles1Click(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// For Widgets with DEBUG in BCB
// Here only because this is useful everywhere
#ifdef _DEBUG
    void wxAssert(int, const wchar_t*, int, const wchar_t*, const wchar_t*) {}
#endif //_DEBUG
//---------------------------------------------------------------------------


#endif
