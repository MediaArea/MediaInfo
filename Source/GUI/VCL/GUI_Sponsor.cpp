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
#include <ZenLib/FileName.h>
#include <algorithm>
#include <ctime>

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

    // Clean olds banners
    ZtringList FileNames=Dir::GetAllFileNames(Prefs->BaseFolder + __T("\\Sponsor\\"), Dir::Include_Files);
    for (size_t Pos=0; Pos<FileNames.size();Pos++)
    {
        ZenLib::FileName Name(FileNames[Pos]);
        if (Name.Name_Get().size()==32 && Name.Extension_Get()==__T("jpg")) // hash.ext
        {
            bool Found=false;
            for (size_t Pos2=0; Pos2<Prefs->SponsorBanner.size(); Pos2++)
            {
                for (size_t Pos3=0; Pos3<Prefs->SponsorBanner[Pos2].size(); Pos3++)
                {
                    Ztring Hash=Ztring().From_Unicode(THashMD5::GetHashString(Prefs->SponsorBanner[Pos2][Pos3].c_str()).c_str());
                    if (Name.Name_Get()==Hash)
                    {
                        std::string name=Name.Name_Get().To_UTF8();
                        std::string name2=Ztring().From_Unicode(THashMD5::GetHashString(Prefs->SponsorBanner[Pos2][Pos3].c_str()).c_str()).To_UTF8();
                        Found=true;
                        break;
                    }
                }
                if (Found)
                    break;
            }

            if (!Found)
                File::Delete(FileNames[Pos]);
        }
    }

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
    if (CurBanner == NewBanner && File::Exists(BannerPath))
    {
        Finalize();
        return;
    }

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

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::OnResize(TObject *Sender)
{
    if (!Visible || !BannerImage->Picture || BannerImage->Picture->Width == 0)
        return;

    double ImgAspect = (double)BannerImage->Picture->Width / BannerImage->Picture->Height;
    double CtrlWidth = Parent ? Parent->ClientWidth : ClientWidth;
    double MaxHeight = BannerImage->Picture->Height;
    int NewHeight = (int)(CtrlWidth / ImgAspect);

    if (Main)
        MaxHeight = min(MaxHeight, (double)Main->ClientHeight / 4);

    MaxHeight = min(MaxHeight, (double)Screen->Height / 8);

    if (NewHeight > MaxHeight)
        NewHeight = MaxHeight;

    if (Height != NewHeight)
        Height = NewHeight;
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

//---------------------------------------------------------------------------
void __fastcall TSponsorFrame::BannerImageMouseInter(TObject *Sender)
{
    Screen->Cursor = crHandPoint;
}
//---------------------------------------------------------------------------

void __fastcall TSponsorFrame::BannerImageMouseLeave(TObject *Sender)
{
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
