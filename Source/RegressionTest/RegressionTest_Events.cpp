/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// RegressionTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <map>
#include "tchar.h"
#include "MediaInfoDLL/MediaInfoDLL.h"
#include "MediaInfo/MediaInfo_Events.h"
#include "ZenLib/ZtringListListF.h"
#include "ZenLib/Dir.h"
#include "ZenLib/File.h"
#include "ZenLib/FileName.h"
#include "RegressionTest/RegressionTest.h"
using namespace MediaInfoDLL;
using namespace ZenLib;
using namespace std;

const char* F_FileName;
#define echoF(_format) Events_UserHandle_struct::perevent &PerEvent=UserHandle->PerEvent[Event->EventCode]; if (!PerEvent.F_MoreThanOnce) {fprintf(PerEvent.F, _format); PerEvent.F_MoreThanOnce=true; } fprintf(PerEvent.F, "\n");
#define echo0(_format) fprintf(PerEvent.F, _format)
#define echo1(_format, Arg1) fprintf(PerEvent.F, _format, Arg1)
#define echo2(_format, Arg1, Arg2) fprintf(PerEvent.F, _format, Arg1, Arg2)
#define echo4(_format, Arg1, Arg2, Arg3, Arg4) fprintf(PerEvent.F, _format, Arg1, Arg2, Arg3, Arg4)

struct Events_UserHandle_struct
{
    FileName Name;
    Ztring Files;
    Ztring DataBaseDirectory;
    struct perevent
    {
        FILE* F;
        bool  F_MoreThanOnce;
        bool  Custom_MoreThanOnce;
    };
    map<int32u, perevent> PerEvent;
    int32u Scenario;
    bool Custom;
    bool ParseSpeed;
    bool NextPacket;
    bool DemuxContainerOnly;
    bool Seek;

    Events_UserHandle_struct()
    {
        Custom=false;
        ParseSpeed=false;
        NextPacket=false;
        DemuxContainerOnly=false;
        Seek=false;
    }

    void Clear()
    {
        for (map<int32u, perevent>::iterator Item=PerEvent.begin(); Item!=PerEvent.end(); ++Item)
            fclose (Item->second.F);
        PerEvent.clear();
    }

};

void General_Start_0 (struct MediaInfo_Event_General_Start_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo starts\n");

    echo1("Stream_Size=%i\n", Event->Stream_Size);
}

void General_End_0 (struct MediaInfo_Event_General_End_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo ends\n");

    echo1("Stream_Bytes_Analyzed=%i\n", Event->Stream_Bytes_Analyzed);
}

void General_Parser_Selected_0 (struct MediaInfo_Event_General_Parser_Selected_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo has selected the parser\n");

    echo1("Name=%s\n", Event->Name);
}

void General_Move_Request_0 (struct MediaInfo_Event_General_Move_Request_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo has requested to seek\n");

    echo1("StreamOffset=%08x\n", Event->StreamOffset);
}

void General_Move_Done_0 (struct MediaInfo_Event_General_Move_Done_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo has seek\n");

    echo1("StreamOffset=%08x\n", Event->StreamOffset);
}

void General_SubFile_Start_0 (struct MediaInfo_Event_General_SubFile_Start_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo is parsing a new file from the source file\n");

    echo1("FileName_Relative=%s\n", Event->FileName_Relative);
    echo1("FileName_Absolute=%s\n", Event->FileName_Absolute);
}

void General_SubFile_End_0 (struct MediaInfo_Event_General_SubFile_End_0* Event, struct Events_UserHandle_struct* UserHandle)
{
    echoF("MediaInfo has finished the parsing a new file from the source file\n");
}

void Global_Demux_4(struct MediaInfo_Event_Global_Demux_4 *Event, struct Events_UserHandle_struct* UserHandle)
{
    if (!UserHandle->DemuxContainerOnly)
        return;

    echoF("MediaInfo Demux\n");

    echo1("StreamOffset=%08x,", Event->StreamOffset);
    echo1(" Frame_Number=%u\n", Event->FrameNumber);
    echo0("IDs=");
    for (size_t Pos=0; Pos<Event->StreamIDs_Size; Pos++)
        switch (Event->StreamIDs_Width[Pos])
        {
            case 2: echo1("%02x, ", Event->StreamIDs[Pos]); break;
            case 4: echo1("%04x, ", Event->StreamIDs[Pos]); break;
            case 8: echo1("%08x, ", Event->StreamIDs[Pos]); break;
            default: echo1("%08x, ", Event->StreamIDs[Pos]); break;
        }
    echo0("\n");
    if (Event->PCR!=(int64u)-1)
        echo1("PCR=%s, ", Ztring().Duration_From_Milliseconds(Event->PCR/1000000).To_Local().c_str());
    if (Event->PTS!=(int64u)-1)
        echo1("PTS=%s, ", Ztring().Duration_From_Milliseconds(Event->PTS/1000000).To_Local().c_str());
    if (Event->DTS!=(int64u)-1)
        echo1("DTS=%s, ", Ztring().Duration_From_Milliseconds(Event->DTS/1000000).To_Local().c_str());
    if (Event->DUR!=(int64u)-1)
        echo1("DUR=%s, ", Ztring().Duration_From_Milliseconds(Event->DUR/1000000).To_Local().c_str());
    if (Event->PCR!=(int64u)-1 || Event->PTS!=(int64u)-1 || Event->DTS!=(int64u)-1 || Event->DUR!=(int64u)-1)
        echo0("\n");
    echo1("Content_Type=%i,", Event->Content_Type);
    echo1(" Content_Size=%i,", Event->Content_Size);
    echo1(" Flags=%08x\n", Event->Flags);
}

void Video_SliceInfo_0(struct MediaInfo_Event_Video_SliceInfo_0 *Event, struct Events_UserHandle_struct* UserHandle)
{
    if (!UserHandle->DemuxContainerOnly)
        return;

    echoF("MediaInfo Demux\n");

    echo1("StreamOffset=%08x,", Event->StreamOffset);
    echo1(" FramePosition=%u,", Event->FrameNumber);
    echo1(" FieldPosition=%u,", Event->FieldPosition);
    echo1(" SlicePosition=%u,", Event->SlicePosition);
    echo0("IDs=");
    for (size_t Pos=0; Pos<Event->StreamIDs_Size; Pos++)
        switch (Event->StreamIDs_Width[Pos])
        {
            case 2: echo1("%02x, ", Event->StreamIDs[Pos]); break;
            case 4: echo1("%04x, ", Event->StreamIDs[Pos]); break;
            case 8: echo1("%08x, ", Event->StreamIDs[Pos]); break;
            default: echo1("%08x, ", Event->StreamIDs[Pos]); break;
        }
    echo0("\n");
    if (Event->PCR!=(int64u)-1)
        echo1("PCR=%s, ", Ztring().Duration_From_Milliseconds(Event->PCR/1000000).To_Local().c_str());
    if (Event->PTS!=(int64u)-1)
        echo1("PTS=%s, ", Ztring().Duration_From_Milliseconds(Event->PTS/1000000).To_Local().c_str());
    if (Event->DTS!=(int64u)-1)
        echo1("DTS=%s, ", Ztring().Duration_From_Milliseconds(Event->DTS/1000000).To_Local().c_str());
    if (Event->DUR!=(int64u)-1)
        echo1("DUR=%s, ", Ztring().Duration_From_Milliseconds(Event->DUR/1000000).To_Local().c_str());
    if (Event->PCR!=(int64u)-1 || Event->PTS!=(int64u)-1 || Event->DTS!=(int64u)-1 || Event->DUR!=(int64u)-1)
        echo0("\n");
    echo1("SliceType=%i,", Event->SliceType);
    echo1(" Flags=%08x\n", Event->Flags);
}

/***************************************************************************/
/* The callback function                                                   */
/***************************************************************************/

#define CASE(_PARSER,_EVENT,_VERSION) \
    case MediaInfo_Event_##_PARSER##_##_EVENT : if (EventVersion==_VERSION && Data_Size>=sizeof(struct MediaInfo_Event_##_PARSER##_##_EVENT##_##_VERSION)) _PARSER##_##_EVENT##_##_VERSION((struct MediaInfo_Event_##_PARSER##_##_EVENT##_##_VERSION*)Data_Content, UserHandle); break;

void __stdcall Event_CallBackFunction(unsigned char* Data_Content, size_t Data_Size, void* UserHandle_Void)
{
    /*Retrieving UserHandle*/
    struct Events_UserHandle_struct*           UserHandle=(struct Events_UserHandle_struct*)UserHandle_Void;
    struct MediaInfo_Event_Generic*     Event_Generic=(struct MediaInfo_Event_Generic*) Data_Content;
    unsigned char                       ParserID;
    unsigned short                      EventID;
    unsigned char                       EventVersion;

    /*integrity tests*/
    if (Data_Size<4)
        return; //There is a problem

    if (UserHandle->PerEvent[Event_Generic->EventCode].F==NULL)
    {
        Ztring Number; Number.From_Number(Event_Generic->EventCode, 16);
        while (Number.size()<8)
            Number.insert(0, 1, __T('0'));
        Ztring Name=Ztring(UserHandle->DataBaseDirectory+__T("\\Events\\New\\")+Ztring::ToZtring(UserHandle->Scenario)+__T("\\")+Number+__T("\\")+UserHandle->Name.Name_Get()+__T(".txt"));
        if (!Dir::Exists(UserHandle->DataBaseDirectory+__T("\\Events\\New")))
            Dir::Create(UserHandle->DataBaseDirectory+__T("\\Events\\New"));
       if (!Dir::Exists(UserHandle->DataBaseDirectory+__T("\\Events\\New\\")+Ztring::ToZtring(UserHandle->Scenario)))
            Dir::Create(UserHandle->DataBaseDirectory+__T("\\Events\\New\\")+Ztring::ToZtring(UserHandle->Scenario));
       if (!Dir::Exists(UserHandle->DataBaseDirectory+__T("\\Events\\New\\")+Ztring::ToZtring(UserHandle->Scenario)+__T("\\")+Number))
            Dir::Create(UserHandle->DataBaseDirectory+__T("\\Events\\New\\")+Ztring::ToZtring(UserHandle->Scenario)+__T("\\")+Number);
        if (!Dir::Exists(FileName(Name).Path_Get()))
            Dir::Create(FileName(Name).Path_Get());
        UserHandle->PerEvent[Event_Generic->EventCode].F=fopen(Name.To_Local().c_str(), "w");
    }

    /*Retrieving EventID*/
    ParserID    =(unsigned char) ((Event_Generic->EventCode&0xFF000000)>>24);
    EventID     =(unsigned short)((Event_Generic->EventCode&0x00FFFF00)>>8 );
    EventVersion=(unsigned char) ( Event_Generic->EventCode&0x000000FF     );


    //*Global to all parsers
    switch (EventID)
    {
                    CASE (Global, Demux, 4)
                    CASE (Video, SliceInfo, 0)
                    default                                                                     : ;
    }

    switch (ParserID)
    {
        case MediaInfo_Parser_None :
                switch (EventID)
                {
                    case MediaInfo_Event_General_Start                                          : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Start_0)) General_Start_0((struct MediaInfo_Event_General_Start_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_End                                            : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_End_0)) General_End_0((struct MediaInfo_Event_General_End_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_Parser_Selected                                : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Parser_Selected_0)) General_Parser_Selected_0((struct MediaInfo_Event_General_Parser_Selected_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_Move_Request                                   : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Move_Request_0)) General_Move_Request_0((struct MediaInfo_Event_General_Move_Request_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_Move_Done                                      : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Move_Done_0)) General_Move_Done_0((struct MediaInfo_Event_General_Move_Done_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_SubFile_Start                                  : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_SubFile_Start_0)) General_SubFile_Start_0((struct MediaInfo_Event_General_SubFile_Start_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_SubFile_End                                    : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_SubFile_End_0)) General_SubFile_End_0((struct MediaInfo_Event_General_SubFile_End_0*)Data_Content, UserHandle); break;
                    default                                                                     : ;
                }
                break;
        default : ; //ParserID is unknown
    }
}

void RegressionTest_Events(Ztring Files, Ztring DataBaseDirectory, int32u Scenario)
{
    // Scenarios:
    // bit  0 : quick parsing / full parsing
    // bit  1 : next packet interface
    // bit  2 : demux (by container only)
    // bit  3 : do some seeks


    cout<<" Analyzing"<<endl;
    ZtringListListF FilesList_Source;
    if (FileName(Files).Extension_Get()==__T("csv"))
        FilesList_Source.Load(DataBaseDirectory+__T("\\Events\\FilesList.csv"));
    else
    {
        if (File::Exists(Files))
            FilesList_Source.push_back(Files);
        else
            FilesList_Source.push_back(Files+__T("\\*.*"));
    }
    vector<Events_UserHandle_struct> FilesList;
    for (size_t FilesList_Source_Pos=0; FilesList_Source_Pos<FilesList_Source.size(); FilesList_Source_Pos++)
    {
        ZtringList Temp=Dir::GetAllFileNames(FilesList_Source[FilesList_Source_Pos](0));
        for (size_t Temp_Pos=0; Temp_Pos<Temp.size(); Temp_Pos++)
        {
            struct Events_UserHandle_struct ToAdd;
            ToAdd.Name=Temp[Temp_Pos];
            ToAdd.DataBaseDirectory=DataBaseDirectory;
            ToAdd.Files=Files;
            ToAdd.Scenario=Scenario;
            if (Scenario&(1<<0))
                ToAdd.ParseSpeed=true;
            if (Scenario&(1<<1))
                ToAdd.NextPacket=true;
            if (Scenario&(1<<2))
                ToAdd.DemuxContainerOnly=true;
            if (Scenario&(1<<3))
                ToAdd.Seek=true;

            FilesList.push_back(ToAdd);
        }
    }


    for (size_t FilesList_Pos=0; FilesList_Pos<FilesList.size(); FilesList_Pos++)
    {
        cout<<" "<<FilesList_Pos+1<<"/"<<FilesList.size()<<" "<<FilesList[FilesList_Pos].Name.To_Local()<<endl;

        MediaInfo MI;
        Ztring MI_Result;

        //**********************************************************************
        // Configuring
        //**********************************************************************

        // CallBack configuration
        // MediaInfo need pointer as text (for compatibility with older version) + 64-bit OS handling
        // form is "CallBack=memory://handlerInDecimal;UserHandler=memory://handlerInDecimal"
        // UserHandler is a unique value wich will be provided to the callback function, in order to know which MediaInfo instance send the event
        wostringstream Event_CallBackFunction_Text;
        Event_CallBackFunction_Text<<__T("CallBack=memory://")<<(MediaInfo_int64u)Event_CallBackFunction<<__T(";UserHandler=memory://")<<(MediaInfo_int64u)&FilesList[FilesList_Pos];
        MI_Result=MI.Option(__T("File_Event_CallBackFunction"), Event_CallBackFunction_Text.str());
        if (!MI_Result.empty())
        {
            wcout<<__T("MediaInfo error: ")<<MI_Result<<endl;
            return;
        }

        //Retrieiving basic data
        MI.Open(FilesList[FilesList_Pos].Name);
        Ztring Delay_10s=Ztring().Duration_From_Milliseconds(Ztring(MI.Get(Stream_Video, 0, __T("Delay"))).To_int64u()+10000);

        if (FilesList[FilesList_Pos].ParseSpeed)
        {
            MI_Result=MI.Option(__T("ParseSpeed"), __T("1.0"));
            if (!MI_Result.empty())
            {
                wcout<<__T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }
        }

        if (FilesList[FilesList_Pos].DemuxContainerOnly)
        {
            MI_Result=MI.Option(__T("Demux"), __T("container"));
            if (!MI_Result.empty())
            {
                wcout<<__T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }

            MI_Result=MI.Option(__T("File_Demux_Unpacketize"), __T("1"));
            if (!MI_Result.empty())
            {
                wcout<<__T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }

            MI_Result=MI.Option(__T("File_Demux_PCM_20bitTo16bit"), __T("1"));
            if (!MI_Result.empty())
            {
                wcout<<__T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }
        }

        if (FilesList[FilesList_Pos].NextPacket)
        {
            MI_Result=MI.Option(__T("File_NextPacket"), __T("1"));
            if (!MI_Result.empty())
            {
                wcout<<__T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }
        }

        MI.Open(FilesList[FilesList_Pos].Name);

        if (FilesList[FilesList_Pos].NextPacket)
        {
            int Counter=0;
            while (MI.Open_NextPacket()&0x100)
            {
                if (FilesList[FilesList_Pos].Seek)
                {
                    Counter++;
                    if (Counter==0)
                        MI.Option(__T("File_Seek"), __T("0"));
                    if (Counter==100)
                        MI.Option(__T("File_Seek"), Delay_10s);
                    if (Counter==200)
                        MI.Option(__T("File_Seek"), __T("Frame=100"));
                    if (Counter==300)
                        MI.Option(__T("File_Seek"), __T("95%"));
                }
            }
        }

        FilesList[FilesList_Pos].Clear();
    }

    cout<<" Diff"<<endl;
    ZtringList Ref=Dir::GetAllFileNames(DataBaseDirectory+__T("\\Events\\Ref\\")+FileName(Files).Name_Get()+__T("\\")+Ztring::ToZtring(Scenario)+__T("*.*"));
    ZtringList New=Dir::GetAllFileNames(DataBaseDirectory+__T("\\Events\\New\\")+FileName(Files).Name_Get()+__T("\\")+Ztring::ToZtring(Scenario)+__T("*.*"));
    for (size_t Ref_Pos=0; Ref_Pos<Ref.size(); Ref_Pos++)
    {
        Ztring Ref_ToFind=Ref[Ref_Pos];
        Ref_ToFind.FindAndReplace(__T("\\Events\\Ref\\"), __T("\\Events\\New\\"));
        size_t New_RefPos=New.Find(Ref_ToFind);
        bool IsDiff=false;
        if (New_RefPos!=(size_t)-1)
        {
            File F_Ref; F_Ref.Open(Ref[Ref_Pos]);
            File F_New; F_New.Open(New[New_RefPos]);
            if (F_Ref.Size_Get()==F_New.Size_Get())
            {
                int64u Size=F_Ref.Size_Get();
                if (Size>100000000)
                    Size=100000000;
                int8u* F_Ref_Buffer=new int8u[(size_t)Size];
                F_Ref.Read(F_Ref_Buffer, (size_t)Size);
                int8u* F_New_Buffer=new int8u[(size_t)Size];
                F_New.Read(F_New_Buffer, (size_t)Size);

                if (memcmp(F_Ref_Buffer, F_New_Buffer, (size_t)Size))
                    IsDiff=true;

                delete[] F_Ref_Buffer;
                delete[] F_New_Buffer;
            }
            else
                IsDiff=true;
        }
        if (New_RefPos==(size_t)-1 || IsDiff)
        {
            //Not in new or is different
            Ztring Diff_Name=Ref[Ref_Pos];
            Diff_Name.FindAndReplace(__T("\\Events\\Ref\\"), __T("\\Events\\Diff\\"));
            if (!Dir::Exists(DataBaseDirectory+__T("\\Events\\Diff")))
                Dir::Create(DataBaseDirectory+__T("\\Events\\Diff"));
            if (!Dir::Exists(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get()))
                Dir::Create(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get());
            if (!Dir::Exists(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get()+__T("\\")+Ztring::ToZtring(Scenario)))
                Dir::Create(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get()+__T("\\")+Ztring::ToZtring(Scenario));
            if (!Dir::Exists(FileName(Diff_Name).Path_Get()))
                Dir::Create(FileName(Diff_Name).Path_Get());
            if (!IsDiff)
                File::Copy(Ref[Ref_Pos], Diff_Name+__T(".RefAlone.txt")); //Not in new
            else
            {
                File::Copy(Ref[Ref_Pos], Diff_Name+__T(".Ref.txt")); //Diff
                File::Copy(New[New_RefPos], Diff_Name+__T(".New.txt"));
            }
        }
        if (New_RefPos!=(size_t)-1)
            New.erase(New.begin()+New_RefPos);
    }
    for (size_t New_Pos=0; New_Pos<New.size(); New_Pos++)
    {
        //Not in ref
        Ztring Diff_Name=New[New_Pos];
        Diff_Name.FindAndReplace(__T("\\Events\\New\\"), __T("\\Events\\Diff\\"));
        if (!Dir::Exists(DataBaseDirectory+__T("\\Events\\Diff")))
            Dir::Create(DataBaseDirectory+__T("\\Events\\Diff"));
        if (!Dir::Exists(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get()))
            Dir::Create(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get());
        if (!Dir::Exists(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get()+__T("\\")+Ztring::ToZtring(Scenario)))
            Dir::Create(DataBaseDirectory+__T("\\Events\\Diff\\")+FileName(Files).Name_Get()+__T("\\")+Ztring::ToZtring(Scenario));
        if (!Dir::Exists(FileName(Diff_Name).Path_Get()))
            Dir::Create(FileName(Diff_Name).Path_Get());
         File::Copy(New[New_Pos], Diff_Name+__T(".NewAlone.txt")); //Not in new
    }
}
