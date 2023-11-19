/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef GUI_PluginH
#define GUI_PluginH
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

//---------------------------------------------------------------------------
enum plugin {
    PLUGIN_GRAPH = 0,
    PLUGIN_FFMPEG,
    PLUGIN_MAX
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class TPluginF;
//---------------------------------------------------------------------------

//***************************************************************************
// DownloadInstallerThread Class
//***************************************************************************
class DownloadInstallerThread :public TThread
{
public:
    __fastcall DownloadInstallerThread(TPluginF* Parent, ZenLib::Ztring Url, ZenLib::Ztring Dst);

    void __fastcall Execute();
    void __fastcall Error();
    void __fastcall Finalize();

    void __fastcall UpdateDownloadProgress();
    void __fastcall OnReceiveData(TObject* const Sender, __int64 AContentLength, __int64 AReadCount, bool& Abort);
    void __fastcall OnRequestError(TObject* const Sender, System::UnicodeString Message) { Synchronize(&Error); };

private:
    TPluginF* Parent;
    float Progress;
    ZenLib::Ztring Url;
    ZenLib::Ztring Dst;
};

//***************************************************************************
// RunInstallerThread Class
//***************************************************************************
class RunInstallerThread :public TThread
{
public:
    __fastcall RunInstallerThread(TPluginF* Parent, ZenLib::Ztring Installer, ZenLib::Ztring Parameters);

    void __fastcall Execute();
    void __fastcall Error();
    void __fastcall Finalize();

private:
    TPluginF* Parent;
    ZenLib::Ztring Installer;
    ZenLib::Ztring Parameters;
};

//***************************************************************************
// Class
//***************************************************************************

class TPluginF : public TForm
{
__published:    // IDE-managed Components
    TLabel* InfoLabel;
    TLabel* NameLabel;
    TLabel* AskLabel;
    TLabel* DownloadLabel;
    TLabel* InstallLabel;
    TButton* Cancel;
    TButton* Install;
    void __fastcall FormShow(TObject* Sender);
    void __fastcall CancelClick(TObject* Sender);
    void __fastcall InstallClick(TObject* Sender);
public:     // User declarations
    __fastcall TPluginF(TComponent* Owner, plugin Plugin);

    bool __fastcall Configure();
    void __fastcall DownloadInstaller();
    void __fastcall RunInstaller();
    void __fastcall Error();
    void __fastcall FinalizeDownload();
    void __fastcall FinalizeInstall();
private:    // User declarations
    plugin Plugin;
    ZenLib::Ztring InstallFolder;
    ZenLib::Ztring SourceURL;
    ZenLib::Ztring TempPath;
    DownloadInstallerThread* DownloadThread;
    RunInstallerThread* RunThread;
};
//---------------------------------------------------------------------------
extern PACKAGE TPluginF *PluginF;
//---------------------------------------------------------------------------
#endif
