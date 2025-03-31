/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
// Compilation condition
#ifndef MEDIAINFOGUI_PLUGIN_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <System.Net.HTTPClientComponent.hpp>
#include "GUI/VCL/GUI_Plugin.h"
#include <ZenLib/Dir.h>
#include <ZenLib/File.h>
#include <ZenLib/HTTP_Client/HTTPClient.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPluginF *PluginF;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ZenLib::Char* PluginInfo[PLUGIN_MAX][2] = {
    {__T("GraphPlugin"), __T("Graph visualization plugin")},
    {__T("FFmpegPlugin"), __T("FFmpeg plugin")},
};
//---------------------------------------------------------------------------

//***************************************************************************
// DownloadInstallerThread Class
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall DownloadInstallerThread::DownloadInstallerThread(TPluginF* Parent, Ztring Url, Ztring Dst)
    : TThread(false), Parent(Parent), Progress(0), Url(Url), Dst(Dst)
{
}

//---------------------------------------------------------------------------
void __fastcall DownloadInstallerThread::Execute()
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
void __fastcall DownloadInstallerThread::Error()
{
    Parent->Error();
}

//---------------------------------------------------------------------------
void __fastcall DownloadInstallerThread::Finalize()
{
    Parent->FinalizeDownload();
}

//---------------------------------------------------------------------------
void __fastcall DownloadInstallerThread::UpdateDownloadProgress()
{
    Ztring Caption = Ztring(Parent->DownloadLabel->Caption.c_str());

    size_t Pos = Caption.rfind(__T("..."));
    if (Pos == string::npos)
        return;

    Caption.erase(Pos + 3);
    Caption += __T(" ") + Ztring().From_Number(Progress, 0) + __T("%");

    Parent->DownloadLabel->Caption = Caption.c_str();
}

//---------------------------------------------------------------------------
void __fastcall DownloadInstallerThread::OnReceiveData(TObject* const Sender, __int64 AContentLength, __int64 AReadCount, bool& Abort)
{
    if (AContentLength)
    {
        Progress = AReadCount / AContentLength * 100.0;
        Synchronize(&UpdateDownloadProgress);
    }

    if (Terminated)
        Abort = true;
}

//***************************************************************************
// RunInstallerThread Class
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall RunInstallerThread::RunInstallerThread(TPluginF* Parent, Ztring Installer, Ztring Parameters)
    : TThread(false), Parent(Parent), Installer(Installer), Parameters(Parameters)
{
}

//---------------------------------------------------------------------------
void __fastcall RunInstallerThread::Execute()
{
    // Run installer
    SHELLEXECUTEINFO Info = { 0 };
    Info.cbSize = sizeof(Info);
    Info.fMask = SEE_MASK_NOCLOSEPROCESS;
    Info.hwnd = 0;
    Info.lpVerb = __T("runas");
    Info.lpFile = Installer.c_str();
    Info.lpParameters = Parameters.c_str();
    Info.lpDirectory = 0;
    Info.nShow = SW_SHOW;
    Info.hInstApp = 0;

    if (!ShellExecuteEx(&Info))
    {
        CloseHandle(Info.hProcess);
        Synchronize(&Error);
        return;
    }

    while (WaitForSingleObject(Info.hProcess, 500) == WAIT_TIMEOUT)
    {
        if (Terminated) // Canceled by user
        {
            TerminateProcess(Info.hProcess, 1);
            CloseHandle(Info.hProcess);
            return;
        }
    }

    DWORD Code;
    if (!GetExitCodeProcess(Info.hProcess, &Code) || Code != 0)
    {
        CloseHandle(Info.hProcess);
        Synchronize(&Error);
        return;
    }

    CloseHandle(Info.hProcess);
    Synchronize(&Finalize);
}

//---------------------------------------------------------------------------
void __fastcall RunInstallerThread::Error()
{
    Parent->Error();
}

//---------------------------------------------------------------------------
void __fastcall RunInstallerThread::Finalize()
{
    Parent->FinalizeInstall();
}

//***************************************************************************
// Class
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TPluginF::TPluginF(TComponent* Owner, plugin Plugin)
    : TForm(Owner), Plugin(Plugin)
{
    // Internal
    DownloadThread = NULL;
    RunThread = NULL;
}

//---------------------------------------------------------------------------
bool __fastcall TPluginF::Configure(bool Update)
{
    // Get MediaInfo installation path
    InstallFolder = Application->ExeName.c_str();
    InstallFolder = InstallFolder.substr(0, InstallFolder.rfind(__T("\\")) + 1);

    // Check requested plugin
    if (Plugin >= PLUGIN_MAX)
    {
        if (!Update)
            MessageBox(NULL, __T("Unable to find installer for the requested plugin, please download it manually from the MediaInfo download page."), __T("Error"), MB_OK);
        return false;
    }

    // Get latest plugin installer URL
    switch (Plugin)
    {
    case PLUGIN_GRAPH:
        #ifdef _WIN64
        SourceURL = Prefs->Config(__T("GraphPlugin64URL"));
        #else
        SourceURL = Prefs->Config(__T("GraphPluginURL"));
        #endif
        break;
    case PLUGIN_FFMPEG:
        SourceURL = Prefs->Config(__T("FFmpegPluginURL"));
        break;
    default:;
    }
    if (SourceURL.empty())
    {
        if (!Update)
            MessageBox(NULL, __T("Unable to find installer for the requested plugin, please download it manually from the MediaInfo download page."), __T("Error"), MB_OK);
        return false;
    }

    // Get temporary local file name for the installer
    TCHAR TempPathBuffer[MAX_PATH + 1];
    TCHAR TempFileName[MAX_PATH + 1];
    if (GetTempPath(MAX_PATH, TempPathBuffer) == 0 || GetTempFileName(TempPathBuffer, TEXT("MI_"), 0, TempFileName) == 0)
    {
        if (!Update)
            MessageBox(NULL, __T("Unable to find installer for the requested plugin, please download it manually from the MediaInfo download page."), __T("Error"), MB_OK);
        return false;
    }

    TempPath = Ztring(TempFileName) + __T(".exe");
    File::Move(Ztring(TempFileName), TempPath);

    if (Update)
    {
        InfoLabel->Caption = __T("A new version of this plugin is available:");
        AskLabel->Caption = __T("Would you like to update this plugin?");
    }

    return true;
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::FormShow(TObject *Sender)
{
    NameLabel->Caption = PluginInfo[Plugin][1];
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::InstallClick(TObject* Sender)
{
    Install->Enabled = false;
    DownloadLabel->Visible = true;

    DownloadInstaller();
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::CancelClick(TObject* Sender)
{
    Cancel->Enabled = false;

    if (DownloadThread)
    {
        DownloadThread->Terminate();
        DownloadThread = NULL;
    }

    if (RunThread)
    {
        RunThread->Terminate();
        RunThread = NULL;
    }

    if (TempPath.size() && File::Exists(TempPath))
        File::Delete(TempPath);

    ModalResult = 2;
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::DownloadInstaller()
{
    DownloadThread = new DownloadInstallerThread(this, SourceURL, TempPath);
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::RunInstaller()
{
    RunThread = new RunInstallerThread(this, TempPath, Ztring(__T("/S /D=")) + InstallFolder);
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::Error()
{
    if (TempPath.size() && File::Exists(TempPath))
        File::Delete(TempPath);

    MessageBox(NULL, __T("An error occured, please download and install the plugin manually from the MediaInfo download page."), __T("Error"), MB_OK);

    ModalResult = 3;
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::FinalizeDownload()
{
    DownloadThread = NULL;

    InstallLabel->Visible = true;
    RunInstaller();
}

//---------------------------------------------------------------------------
void __fastcall TPluginF::FinalizeInstall()
{
    RunThread = NULL;

    if (TempPath.size() && File::Exists(TempPath))
        File::Delete(TempPath);

    ModalResult = 1;
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PLUGIN_NO