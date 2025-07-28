//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Vcl.Imaging.Jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <System.Net.HTTPClientComponent.hpp>
#include <System.Hash.hpp>
#include <algorithm>
#include <ctime>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <ZenLib/Dir.h>
#include <ZenLib/File.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI_Sponsor.h"
#include "GUI_Main.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSponsorFrame *SponsorFrame;

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// DownloadBannerThread Class
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall DownloadBannerThread::DownloadBannerThread(TSponsorFrame* Parent, Ztring Url, Ztring Dst)
    : TThread(false), Parent(Parent), Progress(0), Url(Url), Dst(Dst)
{
}

//---------------------------------------------------------------------------
void __fastcall DownloadBannerThread::Execute()
{
    File Output(Dst, File::Access_Write);
    if (!Output.Opened_Get())
    {
        Synchronize(&Error);
        return;
    }

    THandleStream* Stream = new THandleStream((INT_PTR)Output.File_Handle);

    TNetHTTPClient* Client = new TNetHTTPClient(NULL);
    Client->Asynchronous = false;
    Client->AllowCookies = false;
    Client->HandleRedirects = true;
    Client->OnRequestError = OnRequestError;
    Client->OnReceiveData = OnReceiveData;
    Client->Get(Url.c_str(), Stream);

    Stream->Free();
    Output.Close();

    if (!Terminated)
        Synchronize(&Finalize);
}

//---------------------------------------------------------------------------
void __fastcall DownloadBannerThread::Error()
{
    Parent->Error();
}

//---------------------------------------------------------------------------
void __fastcall DownloadBannerThread::Finalize()
{
    Parent->Finalize();
}

//---------------------------------------------------------------------------
void __fastcall DownloadBannerThread::OnReceiveData(TObject* const Sender, __int64 AContentLength, __int64 AReadCount, bool& Abort)
{
    if (AContentLength)
        Progress = AReadCount / AContentLength * 100.0;

    if (Terminated)
        Abort = true;
}

//---------------------------------------------------------------------------
__fastcall TSponsorFrame::TSponsorFrame(TComponent* Owner, TMainF * Main)
    : TFrame(Owner), Main(Main)
{
    // Internal
    DownloadThread = NULL;
    Visible = false;
}

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::Init()
{
    //Check display conditions for the banner
    int64s CurrentDate = (int64s)time(0);
    int64s InstallDate = Ztring(Prefs->Config(__T("Install"))).To_int64s();
    int64s ClosedDate = Ztring(Prefs->Config(__T("SponsorBannerClose"))).To_int64s();
    int64s InstallGracePeriod = Ztring(Prefs->Config(__T("SponsorBannerInstallGracePeriod"))).To_int64s();
    int64s ClosedGracePeriod = Ztring(Prefs->Config(__T("SponsorBannerCloseGracePeriod"))).To_int64s();

    SourceURL = Prefs->Translate(__T("SponsorBanner"));
    BannerClickUrl = Prefs->Translate(__T("SponsorBannerClickUrl"));

    if (!Prefs->Sponsored ||
        Prefs->Donated ||
        SourceURL.empty() ||
        BannerClickUrl.empty() ||
        CurrentDate - InstallDate < InstallGracePeriod ||
        CurrentDate - ClosedDate < ClosedGracePeriod)
           return;

    if (!Dir::Exists(Prefs->BaseFolder + __T("\\Sponsor")))
        Dir::Create(Prefs->BaseFolder + __T("\\Sponsor"));

    Ztring CurBanner = Prefs->Config(__T("SponsorBannerCurrent"));
    Ztring NewBanner = Ztring().From_Unicode(THashMD5::GetHashString(SourceURL.c_str()).c_str()) + __T(".jpg");

    BannerPath = Prefs->BaseFolder + __T("\\Sponsor\\") + NewBanner;
    if (CurBanner == NewBanner)
    {
        Finalize();
        return;
    }

    if (!CurBanner.empty())
        File::Delete(Prefs->BaseFolder + __T("\\Sponsor\\") + CurBanner);

    DownloadThread = new DownloadBannerThread(this, SourceURL, BannerPath);
}

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::Translate()
{
    Init();
}

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::Error()
{
    if (BannerPath.size() && File::Exists(BannerPath))
        File::Delete(BannerPath);
}

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::Finalize()
{
    DownloadThread = NULL;

    Prefs->Config(__T("SponsorBannerCurrent")) = Ztring().From_Unicode(THashMD5::GetHashString(SourceURL.c_str()).c_str()) + __T(".jpg");
    Prefs->Config_Save();

    try
    {
        BannerImage->Picture->LoadFromFile(BannerPath.c_str());
        Height = BannerImage->Picture->Height;

        if (Height)
        {
            Visible = true;
            Main->FormResize(NULL);
        }
    } catch(...) {}
}

void __fastcall TSponsorFrame::FormResize(TObject *Sender)
{   if (!Visible)
        return;

    int ImgWidth = BannerImage->Picture->Width;
    int ImgHeight = BannerImage->Picture->Height;
    float ImgAspect = (float)ImgWidth / ImgHeight;

    int CtrlWidth = BannerImage->Width;
    int CtrlHeight = BannerImage->Height;
    float CtrlAspect = (float)CtrlWidth / CtrlHeight;

    int DisplayedHeight;
    if (ImgAspect > CtrlAspect)
        DisplayedHeight = (int)(CtrlWidth / ImgAspect);
    else
        DisplayedHeight = CtrlHeight;

    int NewHeight = min(DisplayedHeight, BannerImage->Picture->Height);
    if (DisplayedHeight != NewHeight)
    {
        Height = NewHeight;
        Main->FormResize(NULL);
    }
}

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::CloseButtonClick(TObject *Sender)
{
    Visible = false;
    Main->FormResize(NULL);

    int64s CurrentDate = (int64s)time(0);
    Prefs->Config(__T("SponsorBannerClose")) = Ztring().From_Number(CurrentDate);
    Prefs->Config_Save();
}

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::BannerImageClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, BannerClickUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
}