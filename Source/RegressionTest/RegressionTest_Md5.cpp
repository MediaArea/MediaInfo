/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// RegressionTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include "tchar.h"
#include "MediaInfoDLL/MediaInfoDLL.h"
#include "ZenLib/ZtringListListF.h"
#include "ZenLib/File.h"
#include "ZenLib/Dir.h"
extern "C"
{
#include "md5.h"
}
#include "RegressionTest/RegressionTest.h"
using namespace MediaInfoDLL;
using namespace ZenLib;
using namespace std;

void WriteMd5Info(const Ztring &FileName, MediaInfo &MI, const Ztring &DataBaseDirectory)
{
    int8u* Buffer=new int8u[0x1000000];
    File F;

    String MD5_FromMI=MI.Get(Stream_General, 0, __T("MD5"));
    String Format=MI.Get(Stream_General, 0, __T("Format"));

    Ztring MD5_FromFile;
    if (!MD5_FromMI.empty())
    {
        F.Open(FileName);
        MD5Context Context;
        MD5Init(&Context);
        size_t Size;
        do
        {
            Size=F.Read(Buffer, 0x1000000);
            MD5Update(&Context, Buffer, Size);
        }
        while (Size==0x1000000);
        unsigned char Digest[16];
        MD5Final(Digest, &Context);
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+ 0));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+ 2));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+ 4));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+ 6));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+ 8));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+10));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+12));
        MD5_FromFile+=Ztring().From_CC2(BigEndian2int16u(Digest+14));
        MD5_FromFile.MakeLowerCase();
    }

    bool EmptyNormal=false;
    if (MD5_FromMI.empty() && Format==__T("MXF") && MI.Get(Stream_General, 0, __T("Format_Settings")).find(__T("Open / "))!=string::npos)
    {
        Format+=__T(" ")+MI.Get(Stream_General, 0, __T("Format_Settings"));
        EmptyNormal=true;
    }
    if (MD5_FromMI.empty() && Format.empty())
    {
        EmptyNormal=true;
    }
    if (MD5_FromMI.empty() || EmptyNormal)
    {
        F.Open(DataBaseDirectory+(EmptyNormal?__T("\\MD5.EmptyNormal.csv"):__T("\\MD5.________________Missing.csv")), File::Access_Write_Append);
        F.Write(FileName+__T(";")+MI.Get(Stream_General, 0, __T("Format"))+EOL);
    }
    else if (MD5_FromMI==MD5_FromFile)
    {
        F.Open(DataBaseDirectory+__T("\\MD5.Success.csv"), File::Access_Write_Append);
        F.Write(FileName+__T(";")+Format+__T(";")+MD5_FromMI+EOL);
    }
    else
    {
        F.Open(DataBaseDirectory+__T("\\MD5.________________Fail.csv"), File::Access_Write_Append);
        F.Write(FileName+__T(";")+MI.Get(Stream_General, 0, __T("Format"))+__T(";")+MD5_FromMI+__T(";")+MD5_FromFile+EOL);
    }

    delete[] Buffer;
}

void RegressionTest_Md5(Ztring Files, Ztring DataBaseDirectory, int32u Scenario)
{
    cout<<" Analyzing"<<endl;

    ZtringList FilesList=Dir::GetAllFileNames(Files);
    if (File::Exists(DataBaseDirectory+__T("\\MD5.z_______________Parsed.csv")) && !File::Delete(DataBaseDirectory+__T("\\MD5.z_______________Parsed.csv"))) return;
    if (File::Exists(DataBaseDirectory+__T("\\MD5.________________Fail.csv")) && !File::Delete(DataBaseDirectory+__T("\\MD5.________________Fail.csv"))) return;
    if (File::Exists(DataBaseDirectory+__T("\\MD5.________________Missing.csv")) && !File::Delete(DataBaseDirectory+__T("\\MD5.________________Missing.csv"))) return;
    if (File::Exists(DataBaseDirectory+__T("\\MD5.EmptyNormal.csv")) && !File::Delete(DataBaseDirectory+__T("\\MD5.EmptyNormal.csv"))) return;
    if (File::Exists(DataBaseDirectory+__T("\\MD5.Success.csv")) && !File::Delete(DataBaseDirectory+__T("\\MD5.Success.csv"))) return;

    int8u Percentage=0;
    cout<<"   0%"<<endl;
    for (size_t FilesList_Pos=0; FilesList_Pos<FilesList.size(); FilesList_Pos++)
    {
        int8u Percentage_Current=(int8u)(((float)FilesList_Pos)/FilesList.size()*100);
        if (Percentage_Current>Percentage)
        {
            cout<<"  "<<setw(2)<<(int)Percentage_Current<<"%"<<endl;
            Percentage=Percentage_Current;
        }

        File F;
        F.Open(DataBaseDirectory+__T("\\MD5.z_______________Parsed.csv"), File::Access_Write_Append);
        F.Write(FilesList[FilesList_Pos]+EOL);
        F.Close();

        MediaInfo MI;
        MI.Option(__T("ParseSpeed"), __T("1"));
        MI.Option(__T("File_Source_List"), __T("1"));
        MI.Option(__T("File_MD5"), __T("1"));
        MI.Open(FilesList[FilesList_Pos]);

        WriteMd5Info(FilesList[FilesList_Pos], MI, DataBaseDirectory);
    }
}
