// GUI_About - About interface of MediaInfo
// Copyright (C) 2002-2005 Jerome Martinez, Zen@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
    void __fastcall WebSiteClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall NewVersionClick(TObject *Sender);
    void __fastcall DonateClick(TObject *Sender);
    void __fastcall WriteMeClick(TObject *Sender);
    void __fastcall WriteToTranslatorClick(TObject *Sender);
private:    // User declarations
public:        // User declarations
    __fastcall TAboutF(TComponent* Owner);
    ZenLib::Ztring Translator_Url;
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutF *AboutF;
//---------------------------------------------------------------------------
#endif
