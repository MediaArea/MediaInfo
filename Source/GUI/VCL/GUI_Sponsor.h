/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef GUI_SponsorH
#define GUI_SponsorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
#include <ZenLib/Ztring.h>
#include <ZenLib/OS_Utils.h>
//---------------------------------------------------------------------------

class TMainF;
class TSponsorFrame;

//***************************************************************************
// DownloadBannerThread Class
//***************************************************************************
class DownloadBannerThread :public TThread
{
public:
    __fastcall DownloadBannerThread(TSponsorFrame* Parent, ZenLib::Ztring Url, ZenLib::Ztring Dst);

    void __fastcall Execute();
    void __fastcall Error();
    void __fastcall Finalize();

    void __fastcall UpdateDownloadProgress();
    void __fastcall OnReceiveData(TObject* const Sender, __int64 AContentLength, __int64 AReadCount, bool& Abort);
    void __fastcall OnRequestError(TObject* const Sender, System::UnicodeString Message) { Synchronize(&Error); };

private:
    TSponsorFrame* Parent;
    float Progress;
    ZenLib::Ztring Url;
    ZenLib::Ztring Dst;
};

//***************************************************************************
// TSponsorFrame Class
//***************************************************************************
class TSponsorFrame : public TFrame
{
__published:
    TButton *CloseButton;
    TImage *BannerImage;
    void __fastcall CloseButtonClick(TObject *Sender);
    void __fastcall BannerImageClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);

public:
    __fastcall TSponsorFrame(TComponent* Owner, TMainF* Main);

    void __fastcall Init();
    void __fastcall Translate();
    void __fastcall Error();
    void __fastcall Finalize();

private:
    TMainF* Main;
    ZenLib::Ztring SourceURL;
    ZenLib::Ztring BannerPath;
    ZenLib::Ztring BannerClickUrl;
    DownloadBannerThread* DownloadThread;
};
//---------------------------------------------------------------------------
extern PACKAGE TSponsorFrame *SponsorFrame;
//---------------------------------------------------------------------------
#endif
