/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef GUI_AboutH
#define GUI_AboutH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <ZenLib/Ztring.h>
//---------------------------------------------------------------------------

//***************************************************************************
// Class
//***************************************************************************

class TAboutF : public TForm
{
__published:    // IDE-managed Components
    TLabel *Translator;
    TMemo *Memo;
    TButton *OK;
    TButton *WebSite;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TButton *NewVersion;
    TButton *Donate;
    TToolBar *ToolBar2;
    TToolButton *ToolButton2;
    TButton *WriteMe;
    TButton *WriteToTranslator;
    TImageList *ImageList1;
    TImageList *ImageList2;
    TImageList *ImageList3;
    TLinkLabel *Sponsor_Label;
    void __fastcall WebSiteClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall NewVersionClick(TObject *Sender);
    void __fastcall DonateClick(TObject *Sender);
    void __fastcall WriteMeClick(TObject *Sender);
    void __fastcall WriteToTranslatorClick(TObject *Sender);
    void __fastcall SponsorClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);
private:    // User declarations
public:        // User declarations
    __fastcall TAboutF(TComponent* Owner);
    ZenLib::Ztring Translator_Url;
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutF *AboutF;
//---------------------------------------------------------------------------
#endif
