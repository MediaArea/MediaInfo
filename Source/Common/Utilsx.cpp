/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>                        // WideCharToString
#undef NO_WIN32_LEAN_AND_MEAN
#include <ZenLib/Conf.h>
#pragma hdrstop
#include <windows.h>
#include <shlobj.h>
#include <vfw.h>                        // ICINFO
#include "Utils.h"
#include "Preferences.h"
#include "tchar.h"
#include <ZenLib/Ztring.h>
#include <ZenLib/File.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ZtringListList *Audio_Temp;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//Used to know audio
bool CALLBACK acmDriverEnumCallback (HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
    Audio_Temp->Separator_Set(0, __T("\r\n"));
    Audio_Temp->Separator_Set(1, __T("aaa"));
    HACMDRIVER had;
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
        if (acmFormatTagDetails (had, &aftd, ACM_FORMATTAGDETAILSF_INDEX))                        // Error
            break;

        _TCHAR C1[30]; _itot(aftd.dwFormatTag, C1, 16);
        Ztring Codec=C1;
        while (Codec.size()<4)
            Codec=Ztring(__T("0"))+Codec;
        Codec.MakeUpperCase();
        if (Audio_Temp->Find(Codec)<0)
        {
            (*Audio_Temp)(Audio_Temp->size(), 0)=Codec;
            (*Audio_Temp)(Audio_Temp->size()-1, 2)=aftd.szFormatTag;
        }
        (*Audio_Temp)(Codec, 4)=__T("Yes");
    }
    acmDriverClose (had, 0);
    return TRUE;                        // Finished with this driver, go to next
}


//---------------------------------------------------------------------------
void Codecs_Enumerate(ZenLib::ZtringListList &Video, ZenLib::ZtringListList &Audio)
{
    Audio_Temp=&Audio;

    //Enumerate VFW Video codecs
    ICINFO  icinfo;
    int     count=0;

     while (ICInfo (ICTYPE_VIDEO, count, &icinfo))
    {
        Ztring Codec;
        Codec.From_Local((char *)&icinfo.fccHandler, 0, 4);
        Codec.MakeUpperCase();
        if (Video.Find(Codec)<0)
        {
            Video(Video.size(), 0).From_Local((char *)&icinfo.fccHandler, 0, 4);
            Video(Video.size()-1, 2).From_Local((char *)&icinfo.fccType, 0, 4);
            Video(Video.size()-1, 3).From_Unicode(icinfo.szDriver);
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
void Debug_Header_Create(const ZenLib::Ztring &FileName, void* Handle)
{
    //Reading file
    File FB1;
    FB1.Open(FileName);
    int8u* Tout=new int8u[524288];
    int Capture=FB1.Read(Tout, 524288);

    //Writing file
    File FB2;
    FB2.Create(FileName+__T(".header"), true);
    FB2.Write(Tout, Capture);

    //Purge
    delete [] Tout;
    Application->MessageBox(Prefs->Translate(__T("Send HeaderFile")).c_str(), Prefs->Translate(__T("Header file")).c_str());
}
