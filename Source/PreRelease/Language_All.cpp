/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <ZenLib/ZtringListListF.h>
#include "PreRelease/Language_All.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

Ztring Language_All_Run ()
{
    ZtringListListF Origin;
    Origin.Load(L"../Source/Resource/Language.csv");
    for (int Lang=1; Lang<50; Lang++)
    {
        Ztring Name=Origin.Read(L"  Language_ISO639", Lang);
        if (Name.size()>0)
        {
            ZtringListListF Dest;
            int Automatic=0; //count of words in file
            int Automatic_Language=0; //count of words in file without Language names
            for (size_t Pos=0; Pos<Origin.size(); Pos++)
            {
                Dest(Pos, 0)=Origin(Pos, 0);
                Dest(Pos, 1)=Origin(Pos, Lang);
                if (Dest(Pos, 1).size()>0)
                    Automatic++;
                if (Dest(Pos, 1).size()>0 || Dest(Pos, 0).find(__T("Language_"))==0 || Dest(Pos, 0).find(__T("Genre_"))==0 || Dest(Pos, 0).find(__T("OpenCandy_"))==0)
                    Automatic_Language++;
            }
            Dest((L"ZZ_Automatic_Percent"), 1).From_Number(Automatic*100/Origin.size());
            Dest((L"ZZ_AutomaticLanguage_Percent"), 1).From_Number(Automatic_Language*100/Origin.size());
            Dest.Save(Ztring(L"../Source/Resource/Plugin/Language/")+Name+L".csv");
        }
    }

    return L"OK";
}
//---------------------------------------------------------------------------
