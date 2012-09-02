// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#pragma hdrstop

#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>                        // WideCharToString
#include <windows.h>
#include <shlobj.h>
#include <vfw.h>                        // ICINFO
#include <wx/string.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/strconv.h>
#include "Utils.h"
#include "Preferences.h"

using namespace ZenLib;
ZtringListList *Audio_Temp;

//---------------------------------------------------------------------------

//Used to know audio
bool CALLBACK acmDriverEnumCallback (HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
    Audio_Temp->Separator_Set(0, __T("\r\n"));
    Audio_Temp->Separator_Set(1, __T("aaa"));
    HACMDRIVER  had;
    ACMDRIVERDETAILS    add;
    ACMFORMATTAGDETAILS aftd;

    memset (&add, 0, sizeof (ACMDRIVERDETAILS));
    add.cbStruct = sizeof (ACMDRIVERDETAILS);
    acmDriverDetails (hadid, &add, 0);

    if (acmDriverOpen (&had, hadid, 0))
        return TRUE;                    // Error, but go to next

    for (DWORD i=0 ; i<add.cFormatTags ; i++)
    {
        memset (&aftd, 0, sizeof (ACMFORMATTAGDETAILS));
        aftd.cbStruct = sizeof (ACMFORMATTAGDETAILS);
        aftd.dwFormatTagIndex = i;
        if (acmFormatTagDetails (had, &aftd, ACM_FORMATTAGDETAILSF_INDEX))                      // Error
            break;

        _TCHAR C1[30]; _itot(aftd.dwFormatTag, C1, 16);
        Ztring Codec=C1;
        while (Codec.size()<4)
            Codec=Ztring(__T("0"))+Codec;
        Codec.MakeUpperCase();
        if (Audio_Temp->Find(Codec)==Error)
        {
            (*Audio_Temp)(Audio_Temp->size(), 0)=Codec;
            (*Audio_Temp)(Audio_Temp->size()-1, 2)=aftd.szFormatTag;
        }
        (*Audio_Temp)(Codec, 4)=__T("Yes");
    }
    acmDriverClose (had, 0);
    return TRUE;                        // Finished with this driver, go to next
}


void Codecs_Enumerate(ZenLib::ZtringListList &Video, ZenLib::ZtringListList &Audio)
{
    Audio_Temp=&Audio;

    //Enumerate VFW Video codecs
    ICINFO  icinfo;
    int     count=0;

    while (ICInfo (ICTYPE_VIDEO, count, &icinfo))
    {
        Ztring Codec=Ztring(wxString((char *)&icinfo.fccHandler, wxConvLocal).c_str(), 4);
        Codec.MakeUpperCase();
        if (Video.Find(Codec)==Error)
        {
            Video(Video.size(), 0)= Ztring(wxString((char *)&icinfo.fccHandler, wxConvLocal).c_str(), 4);
            Video(Video.size()-1, 2)= Ztring(wxString((char *)&icinfo.fccType, wxConvLocal).c_str(), 4);
            Video(Video.size()-1, 3)= wxString(WideCharToString(icinfo.szDriver).c_str(), wxConvLocal).c_str();
            Video(Video.size()-1, 4)=__T("Yes");
        }
        else
        {
            Video(Codec, 4)=__T("Yes");
        }
        count++;
    }

    //Enumerate VFW Audio codecs with a callback
    acmDriverEnum ((ACMDRIVERENUMCB)&acmDriverEnumCallback, 0, 0);
    Sleep (200); //We MUST wait for acmDriverEnum :(

}



//---------------------------------------------------------------------------

// Select directory code
// Extracted from TBffolder by Torsten Johann (t-johann@gmx.de)

_TCHAR szInitDir[MAX_PATH];

int __stdcall BffolderCallbackProc (HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED)
    {
        SetWindowText (hwnd, __T("Media Info")); // Caption
        SendMessage (hwnd, BFFM_ENABLEOK, 0, TRUE);
        SendMessage (hwnd, BFFM_SETSELECTION, true, (LPARAM)&szInitDir);
    }
    return 0;
}

ZenLib::Ztring ShowOpenFolder()
{
   //Browse
    LPMALLOC        lpMalloc;
    LPSHELLFOLDER   lpShellFolder;
    BROWSEINFO      bi;
    LPITEMIDLIST    ItemID;

    // We initialize the SHBrowseForFolder function

    if (SHGetMalloc (&lpMalloc) != NOERROR)
        return __T("");
    if (SHGetDesktopFolder (&lpShellFolder) != NOERROR)
        return __T("");
    ZeroMemory (&bi, sizeof (BROWSEINFO));
    bi.ulFlags += BIF_RETURNONLYFSDIRS;

    bi.hwndOwner = Application->Handle;
    bi.pszDisplayName = szInitDir;
    bi.lpszTitle = __T("Select your directory");
    bi.lpfn = BffolderCallbackProc;

    ItemID = SHBrowseForFolder (&bi);   // Display the search directory dialog

    lpShellFolder->Release ();
    if (ItemID != NULL)
    {
        SHGetPathFromIDList (ItemID, szInitDir);
        lpMalloc->Free (ItemID);
        lpMalloc->Release ();

        //Open
        return wxString(szInitDir, wxConvLocal).c_str();;
    }
    else
        return __T("");
}

//---------------------------------------------------------------------------
void Debug_Header_Create(const ZenLib::Ztring &FileName, void* Handle)
{
    //Reading file
    wxFile FB1;
    FB1.Open(FileName.c_str());
    char* Tout=new char[262144];
    int Capture=FB1.Read(Tout, 262144);

    //Writing file
    wxFile FB2;
    FB2.Create((FileName+__T(".header")).c_str(), true);
    FB2.Write(Tout, Capture);

    //Purge
    delete [] Tout;
    MessageBox(Handle, Prefs->Translate(__T("Send HeaderFile")).c_str(), Prefs->Translate(__T("Header file")).c_str(), 0);
}

