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
    if (argc!=4)
        return 1;
    
    Ztring Files=argv[1];
    Ztring DataBaseDirectory=argv[2];
    int32u Scenario=Ztring(argv[3]).To_int32u();

    cout<<"Basic"<<endl;
    RegressionTest_Basic(Files, DataBaseDirectory, Scenario);

    cout<<"Events"<<endl;
    RegressionTest_Events(Files, DataBaseDirectory, Scenario);
    
    return 0;
}

