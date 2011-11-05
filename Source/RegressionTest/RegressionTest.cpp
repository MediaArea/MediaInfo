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
    if (argc!=3)
        return 1;
    
    Ztring Files=argv[1];
    Ztring DataBaseDirectory=argv[2];

    cout<<"Basic"<<endl;
    RegressionTest_Basic(Files, DataBaseDirectory);

    //cout<<"Events"<<endl;
    //RegressionTest_Events(Files, DataBaseDirectory);
    
    return 0;
}

