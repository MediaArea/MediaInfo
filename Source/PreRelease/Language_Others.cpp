/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <ZenLib/ZtringListListF.h>
#include "PreRelease/Language_Others.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

void Run_File(ZtringListListF &Dest, Ztring Language)
{
    //Open Orig
    ZtringListListF Origin;
    if (!Origin.Load(Ztring(L"../Source/Resource/Plugin/Language/")+Language+L".csv"))
       return;

    //Find position in Destination
    int Lang=0;
    for (int LangPos=1; LangPos<50; LangPos++)
        if (Lang==0 && (Dest(L"  Language_ISO639", LangPos)==Language || Dest(L"  Language_ISO639", LangPos).size()==0))
           Lang=LangPos;

    //Delete BOM characters if present
    Origin(0, 0)=L"  Author_Email";

    //Complete
    for (size_t Pos=0; Pos<Origin.size(); Pos++)
    {
        auto Dest_Pos=Dest.Find(Origin(Pos, 0), 0, 0, L"==", Ztring_CaseSensitive);
        if (Dest_Pos==-1)
        {
            Dest.push_back(Origin.Read(Pos, 0)); //Append to end of file
            Dest_Pos=Dest.Find(Origin(Pos, 0), 0, 0, L"==", Ztring_CaseSensitive);
        }
        Dest(Dest_Pos, Lang)=Origin(Pos, 1);
    }
}

Ztring Language_Others_Run ()
{
    ZtringListListF Dest;
    Dest.Load(L"../Source/Resource/Language.csv");

    Run_File(Dest, L"ar");
    Run_File(Dest, L"be");
    Run_File(Dest, L"bg");
    Run_File(Dest, L"ca");
    Run_File(Dest, L"cs");
    Run_File(Dest, L"da");
    Run_File(Dest, L"de");
    Run_File(Dest, L"en");
    Run_File(Dest, L"es");
    Run_File(Dest, L"eu");
    Run_File(Dest, L"fa");
    Run_File(Dest, L"fr");
    Run_File(Dest, L"gl");
    Run_File(Dest, L"gr");
    Run_File(Dest, L"hr");
    Run_File(Dest, L"hu");
    Run_File(Dest, L"hy");
    Run_File(Dest, L"ka");
    Run_File(Dest, L"id");
    Run_File(Dest, L"it");
    Run_File(Dest, L"ja");
    Run_File(Dest, L"ka");
    Run_File(Dest, L"ko");
    Run_File(Dest, L"lt");
    Run_File(Dest, L"nl");
    Run_File(Dest, L"pl");
    Run_File(Dest, L"pt");
    Run_File(Dest, L"pt-BR");
    Run_File(Dest, L"ro");
    Run_File(Dest, L"ru");
    Run_File(Dest, L"sk");
    Run_File(Dest, L"sq");
    Run_File(Dest, L"sv");
    Run_File(Dest, L"th");
    Run_File(Dest, L"tr");
    Run_File(Dest, L"uk");
    Run_File(Dest, L"zh-CN");
    Run_File(Dest, L"zh-HK");
    Run_File(Dest, L"zh-TW");

    Dest.Save();

    return L"OK";
}
//---------------------------------------------------------------------------
