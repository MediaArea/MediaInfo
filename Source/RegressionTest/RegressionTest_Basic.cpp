// RegressionTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "tchar.h"
#include "MediaInfoDLL\MediaInfoDLL.h"
#include "ZenLib\ZtringListListF.h"
#include "ZenLib\File.h"
#include "ZenLib\Dir.h"
#include "RegressionTest/RegressionTest.h"
using namespace MediaInfoDLL;
using namespace ZenLib;
using namespace std;

void RegressionTest_Basic(Ztring Files, Ztring DataBaseDirectory, int32u Scenario)
{
    ZtringListListF* Ref=new ZtringListListF[Stream_Max];
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
        if (File::Exists(DataBaseDirectory+_T("\\Basic\\Ref\\")+Ztring::ToZtring(StreamKind)+_T(".csv")))
            Ref[StreamKind].Load(DataBaseDirectory+_T("\\Basic\\Ref\\")+Ztring::ToZtring(StreamKind)+_T(".csv"));

    ZtringListListF* New=new ZtringListListF[Stream_Max];

    cout<<" Analyzing"<<endl;
    MediaInfoList MIL;
    MIL.Open(Files);

    cout<<" Retrieving new data"<<endl;
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        for (size_t FilePos=0; FilePos<MIL.Count_Get(); FilePos++)
            if (MIL.Count_Get(FilePos, (stream_t)StreamKind))
            {
                New[StreamKind].push_back(ZtringList());
                New[StreamKind].at(New[StreamKind].size()-1).push_back(Ztring()); //CompleteFileName
                for (size_t LinePos=0; LinePos<MIL.Count_Get(FilePos, (stream_t)StreamKind, 0); LinePos++)
                    New[StreamKind].at(New[StreamKind].size()-1).push_back(MIL.Get(FilePos, (stream_t)StreamKind, 0, LinePos, Info_Name));
                break;
            }
    }
    for (size_t FilePos=0; FilePos<MIL.Count_Get(); FilePos++)
        for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
            for (size_t StreamPos=0; StreamPos<MIL.Count_Get(FilePos, (stream_t)StreamKind); StreamPos++)
            {
                New[StreamKind].push_back(ZtringList());
                New[StreamKind].at(New[StreamKind].size()-1).push_back(MIL.Get(FilePos, Stream_General, 0, _T("CompleteName")));
                for (size_t LinePos=0; LinePos<MIL.Count_Get(FilePos, (stream_t)StreamKind, StreamPos); LinePos++)
                    New[StreamKind].at(New[StreamKind].size()-1).push_back(MIL.Get(FilePos, (stream_t)StreamKind, StreamPos, LinePos));
            }

    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        if (!Dir::Exists(DataBaseDirectory+_T("\\Basic\\New")))
            Dir::Create(DataBaseDirectory+_T("\\Basic\\New"));
        New[StreamKind].Save(DataBaseDirectory+_T("\\Basic\\New\\")+Ztring::ToZtring(StreamKind)+_T(".csv"));
    }
    
    cout<<" Diff"<<endl;
    ZtringListListF* Diff=new ZtringListListF[Stream_Max];
    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        size_t Ref_Pos=1;
        size_t New_Pos=1;
        while (Ref_Pos<Ref[StreamKind].size() && New_Pos<New[StreamKind].size())
        {
            if (New[StreamKind](New_Pos, 0)!=Ref[StreamKind](Ref_Pos, 0))
            {
                size_t Ref_PosNext=Ref_Pos;
                size_t New_PosNext=New_Pos;
                while (Ref_PosNext<Ref[StreamKind].size() && New[StreamKind](New_Pos, 0)!=Ref[StreamKind](Ref_PosNext, 0))
                    Ref_PosNext++;
                if (Ref_PosNext==Ref[StreamKind].size()) //Ref file not found
                {
                    Diff[StreamKind].push_back(New[StreamKind](New_Pos));
                    New_Pos++;
                }
                else
                {
                    for (; Ref_Pos<Ref_PosNext; Ref_Pos++)
                        Diff[StreamKind].push_back(Ref[StreamKind](Ref_Pos));
                }
            }
            else
            {
                bool IsDiff=false;
                size_t LinePos_Max=max(New[StreamKind](New_Pos).size(), Ref[StreamKind](Ref_Pos).size());
                for (size_t LinePos=1; LinePos<LinePos_Max; LinePos++)
                    if (New[StreamKind](New_Pos, LinePos)!=Ref[StreamKind](Ref_Pos, LinePos))
                    {
                        if (!IsDiff)
                        {
                            Diff[StreamKind].push_back(ZtringList());
                            Diff[StreamKind](Diff[StreamKind].size()-1, 0)=New[StreamKind](New_Pos, 0);
                            IsDiff=true;
                        }
                        Diff[StreamKind](Diff[StreamKind].size()-1, LinePos)=New[StreamKind](New_Pos, LinePos)+_T(" --- ")+Ref[StreamKind](Ref_Pos, LinePos);
                    }
                
                Ref_Pos++;
                New_Pos++;
            }

        }

        for (; Ref_Pos<Ref[StreamKind].size(); Ref_Pos++)
        {
            Diff[StreamKind].push_back(ZtringList());
            Diff[StreamKind](Diff[StreamKind].size()-1, 0)=Ref[StreamKind](Ref_Pos, 0);
            for (size_t LinePos=1; LinePos<Ref[StreamKind][Ref_Pos].size(); LinePos++)
                if (!Ref[StreamKind](Ref_Pos, LinePos).empty())
                    Diff[StreamKind](Diff[StreamKind].size()-1, LinePos)=_T(" --- ")+Ref[StreamKind](Ref_Pos, LinePos);
        }
        for (; New_Pos<New[StreamKind].size(); New_Pos++)
        {
            Diff[StreamKind].push_back(ZtringList());
            Diff[StreamKind](Diff[StreamKind].size()-1, 0)=New[StreamKind](New_Pos, 0);
            for (size_t LinePos=1; LinePos<New[StreamKind][New_Pos].size(); LinePos++)
                if (!New[StreamKind](New_Pos, LinePos).empty())
                    Diff[StreamKind](Diff[StreamKind].size()-1, LinePos)=New[StreamKind](New_Pos, LinePos)+_T(" --- ");
        }
    }

    for (size_t StreamKind=0; StreamKind<Stream_Max; StreamKind++)
    {
        if (!Diff[StreamKind].empty())
        {
            Diff[StreamKind].insert(Diff[StreamKind].begin(), New[StreamKind](0));
            if (!Dir::Exists(DataBaseDirectory+_T("\\Basic\\Diff")))
                Dir::Create(DataBaseDirectory+_T("\\Basic\\Diff"));
            Diff[StreamKind].Save(DataBaseDirectory+_T("\\Basic\\Diff\\")+Ztring::ToZtring(StreamKind)+_T(".csv"));
        }
    }
}


