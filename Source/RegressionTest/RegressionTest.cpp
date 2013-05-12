/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// RegressionTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <tchar.h>
#include "ZenLib/Ztring.h"
#include "RegressionTest/RegressionTest.h"
using namespace std;
using namespace ZenLib;


int _tmain(int argc, _TCHAR* argv[])
{
    if (argc<4)
        return 1;

    Ztring Files=argv[1];
    Ztring DataBaseDirectory=argv[2];
    int32u Scenario=Ztring(argv[3]).To_int32u();

    bool Flag_All=true, Flag_Basic=false, Flag_Events=false, Flag_Md5=false;
    for (int Pos=3; Pos<argc; Pos++)
    {
        if (Ztring(argv[Pos]).Compare(__T("Basic")))
        {
            Flag_All=false;
            Flag_Basic=true;
        }
        if (Ztring(argv[Pos]).Compare(__T("Events")))
        {
            Flag_All=false;
            Flag_Events=true;
        }
        if (Ztring(argv[Pos]).Compare(__T("Md5")))
        {
            Flag_All=false;
            Flag_Md5=true;
        }
    }

    if (Flag_All || Flag_Basic)
    {
        cout<<"Basic"<<endl;
        RegressionTest_Basic(Files, DataBaseDirectory, Scenario);
    }

    if (Flag_All || Flag_Events)
    {
        cout<<"Events"<<endl;
        RegressionTest_Events(Files, DataBaseDirectory, Scenario);
    }

    if (Flag_All || Flag_Md5)
    {
        cout<<"Md5"<<endl;
        RegressionTest_Md5(Files, DataBaseDirectory, Scenario);
    }

    return 0;
}
