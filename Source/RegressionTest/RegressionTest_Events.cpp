// RegressionTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <map>
#include "tchar.h"
#include "MediaInfoDLL\MediaInfoDLL.h"
#include "MediaInfo\MediaInfo_Events.h"
#include "ZenLib\ZtringListListF.h"
#include "ZenLib\Dir.h"
#include "ZenLib\File.h"
#include "ZenLib\FileName.h"
#include "RegressionTest/RegressionTest.h"
using namespace MediaInfoDLL;
using namespace ZenLib;
using namespace std;

const char* F_FileName;
#define echo0(_format) fprintf(UserHandle->F[Event->EventCode], _format)
#define echo1(_format, Arg1) fprintf(UserHandle->F[Event->EventCode], _format, Arg1)
#define echo2(_format, Arg1, Arg2) fprintf(UserHandle->F[Event->EventCode], _format, Arg1, Arg2)
#define echo4(_format, Arg1, Arg2, Arg3, Arg4) fprintf(UserHandle->F[Event->EventCode], _format, Arg1, Arg2, Arg3, Arg4)
#define wecho0(_format) //wfprintf(F, _format)
#define wecho1(_format, Arg1) //wfprintf(F, _format, Arg1)
#define wecho4(_format, Arg1, Arg2, Arg3, Arg4) //wfprintf(F, _format, Arg1, Arg2, Arg3, Arg4)

struct UserHandle_struct
{
    FileName Name;
    Ztring DataBaseDirectory;
    map<int32u, FILE*> F;
    bool Custom;
    bool ParseSpeed;
    bool NextPacket;

    UserHandle_struct()
    {
        Custom=false;
        ParseSpeed=false;
        NextPacket=false;
    }

    void Clear()
    {
        for (map<int32u, FILE*>::iterator Item=F.begin(); Item!=F.end(); Item++)
            fclose (Item->second);
        F.clear();
    }

};

void General_Start_0 (struct MediaInfo_Event_General_Start_0* Event, struct UserHandle_struct* UserHandle)
{
    if (UserHandle->Custom)
    {
        return;
    }

    echo1("MediaInfo starts, Size=%i bytes\n", Event->Stream_Size);
}

void General_End_0 (struct MediaInfo_Event_General_End_0* Event, struct UserHandle_struct* UserHandle)
{
    if (UserHandle->Custom)
    {
        return;
    }

    echo1("MediaInfo ends, %i bytes read\n", Event->Stream_Bytes_Analyzed);
}

void General_Parser_Selected_0 (struct MediaInfo_Event_General_Parser_Selected_0* Event, struct UserHandle_struct* UserHandle)
{
    if (UserHandle->Custom)
    {
        return;
    }

    echo1("MediaInfo has selected the parser: %s\n", Event->Name);
}

void General_Move_Request_0 (struct MediaInfo_Event_General_Move_Request_0* Event, struct UserHandle_struct* UserHandle)
{
    if (UserHandle->Custom)
    {
        return;
    }

    echo1("MediaInfo has requested to seek, Offset=%08x\n", Event->Stream_Offset);
}

void General_Move_Done_0 (struct MediaInfo_Event_General_Move_Done_0* Event, struct UserHandle_struct* UserHandle)
{
    if (UserHandle->Custom)
    {
        return;
    }

    echo1("MediaInfo has seek, Offset=%08x\n", Event->Stream_Offset);
}

void Global_Demux_3(struct MediaInfo_Event_Global_Demux_3 *Event, struct UserHandle_struct* UserHandle)
{
    if (UserHandle->Custom)
    {
        return;
    }

    echo1("MediaInfo Demux, Offset=%08x\n", Event->Stream_Offset);
    echo1("    Stream_Offset=%08x,", Event->Stream_Offset);
    echo1(   " Frame_Number=%u\n", Event->FrameNumber);
        echo0("    IDs=");
    for (size_t Pos=0; Pos<Event->StreamIDs_Size; Pos++)
        switch (Event->StreamIDs_Width[Pos])
        {
            case 2: echo1("%02x, ", Event->StreamIDs[Pos]); break;
            case 4: echo1("%04x, ", Event->StreamIDs[Pos]); break;
            case 8: echo1("%08x, ", Event->StreamIDs[Pos]); break;
            default: echo1("%08x, ", Event->StreamIDs[Pos]); break;
        }
    echo0(   "\n");
    echo1("    PCR=%s,", Ztring().Duration_From_Milliseconds(Event->PCR/1000000).To_Local().c_str());
    echo1(   " PTS=%s,", Ztring().Duration_From_Milliseconds(Event->PTS/1000000).To_Local().c_str());
    echo1(   " DTS=%s,", Ztring().Duration_From_Milliseconds(Event->DTS/1000000).To_Local().c_str());
    echo1(   " DUR=%s\n", Ztring().Duration_From_Milliseconds(Event->DUR/1000000).To_Local().c_str());
    echo1("    Content_Type=%i,", Event->Content_Type);
    echo1(   " Content_Size=%i,", Event->Content_Size);
    echo1(   " Flags=%08x\n", Event->Flags);
}

/***************************************************************************/
/* The callback function                                                   */
/***************************************************************************/

#define CASE(_PARSER,_EVENT,_VERSION) \
    case MediaInfo_Event_##_PARSER##_##_EVENT : if (EventVersion==_VERSION && Data_Size>=sizeof(struct MediaInfo_Event_##_PARSER##_##_EVENT##_##_VERSION)) _PARSER##_##_EVENT##_##_VERSION((struct MediaInfo_Event_##_PARSER##_##_EVENT##_##_VERSION*)Data_Content, UserHandle); break;

void __stdcall Event_CallBackFunction(unsigned char* Data_Content, size_t Data_Size, void* UserHandle_Void)
{
    /*Retrieving UserHandle*/
    struct UserHandle_struct*           UserHandle=(struct UserHandle_struct*)UserHandle_Void;
    struct MediaInfo_Event_Generic*     Event_Generic=(struct MediaInfo_Event_Generic*) Data_Content;
    unsigned char                       ParserID;
    unsigned short                      EventID;
    unsigned char                       EventVersion;

    /*integrity tests*/
    if (Data_Size<4)
        return; //There is a problem

    if (UserHandle->F[Event_Generic->EventCode]==NULL)
    {    
        Ztring Number; Number.From_Number(Event_Generic->EventCode, 16);
        while (Number.size()<8)
            Number.insert(0, 1, _T('0'));
        Ztring Name=Ztring(UserHandle->DataBaseDirectory+_T("\\New\\")+Number+_T("\\")+UserHandle->Name.Name_Get()+_T(".txt"));
        if (!Dir::Exists(UserHandle->DataBaseDirectory+_T("\\New")))
            Dir::Create(UserHandle->DataBaseDirectory+_T("\\New"));
        if (!Dir::Exists(FileName(Name).Path_Get()))
            Dir::Create(FileName(Name).Path_Get());
        UserHandle->F[Event_Generic->EventCode]=fopen(Name.To_Local().c_str(), "w");
    }
    
    /*Retrieving EventID*/
    ParserID    =(unsigned char) ((Event_Generic->EventCode&0xFF000000)>>24);
    EventID     =(unsigned short)((Event_Generic->EventCode&0x00FFFF00)>>8 );
    EventVersion=(unsigned char) ( Event_Generic->EventCode&0x000000FF     );


    //*Global to all parsers
    switch (EventID)
    {
                    case MediaInfo_Event_Global_Demux                                           : if (EventVersion == 3 && Data_Size >= sizeof(struct MediaInfo_Event_Global_Demux_3)) Global_Demux_3((struct MediaInfo_Event_Global_Demux_3 *)Data_Content, UserHandle);  break;
                    default                                                                     : ;
    }

    switch (ParserID)
    {
           /*DCA number in comment*/
        case MediaInfo_Parser_None :    
                switch (EventID)
                {
                    case MediaInfo_Event_General_Start                                          : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Start_0)) General_Start_0((struct MediaInfo_Event_General_Start_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_End                                            : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_End_0)) General_End_0((struct MediaInfo_Event_General_End_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_Parser_Selected                                : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Parser_Selected_0)) General_Parser_Selected_0((struct MediaInfo_Event_General_Parser_Selected_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_Move_Request                                   : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Move_Request_0)) General_Move_Request_0((struct MediaInfo_Event_General_Move_Request_0*)Data_Content, UserHandle); break;
                    case MediaInfo_Event_General_Move_Done                                      : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_Move_Done_0)) General_Move_Done_0((struct MediaInfo_Event_General_Move_Done_0*)Data_Content, UserHandle); break;
                    //case MediaInfo_Event_General_SubFile_Start                                  : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_SubFile_Start_0)) General_SubFile_Start_0((struct MediaInfo_Event_General_SubFile_Start_0*)Data_Content, UserHandle); break;
                    //case MediaInfo_Event_General_SubFile_End                                    : if (EventVersion==0 && Data_Size==sizeof(struct MediaInfo_Event_General_SubFile_End_0)) General_SubFile_End_0((struct MediaInfo_Event_General_SubFile_End_0*)Data_Content, UserHandle); break;
                    default                                                                     : ;
                }
                break;
        default : ;
    }
}

void RegressionTest_Events(Ztring Files, Ztring DataBaseDirectory)
{
    ZtringListListF FilesList_Source; FilesList_Source.Load(DataBaseDirectory+_T("\\Events\\FilesList.csv"));
    vector<UserHandle_struct> FilesList;
    for (size_t FilesList_Source_Pos=1; FilesList_Source_Pos<FilesList_Source.size(); FilesList_Source_Pos++)
    {
        ZtringList Temp=Dir::GetAllFileNames(FilesList_Source[FilesList_Source_Pos](0));
        for (size_t Temp_Pos=0; Temp_Pos<Temp.size(); Temp_Pos++)
        {
            UserHandle_struct ToAdd;
            ToAdd.Name=Temp[Temp_Pos];
            ToAdd.DataBaseDirectory=DataBaseDirectory+_T("\\Events");
            if (FilesList_Source[FilesList_Source_Pos](1).To_int32u())
                ToAdd.ParseSpeed=true;
            if (FilesList_Source[FilesList_Source_Pos](2).To_int32u())
                ToAdd.NextPacket=true;

            FilesList.push_back(ToAdd);
        }
    }

    
    for (size_t FilesList_Pos=0; FilesList_Pos<FilesList.size(); FilesList_Pos++)
    {
        cout<<" "<<FilesList_Pos<<"/"<<FilesList.size()<<endl;

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
        Event_CallBackFunction_Text<<_T("CallBack=memory://")<<(MediaInfo_int64u)Event_CallBackFunction<<_T(";UserHandler=memory://")<<(MediaInfo_int64u)&FilesList[FilesList_Pos];
        MI_Result=MI.Option(_T("File_Event_CallBackFunction"), Event_CallBackFunction_Text.str());
        if (!MI_Result.empty())
        {
            wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
            return;
        }

        // We want to use the "NextPacket" interface
        if (FilesList[FilesList_Pos].NextPacket)
        {
            MI.Open(FilesList[FilesList_Pos].Name);
            Ztring Delay_10s=Ztring().Duration_From_Milliseconds(Ztring(MI.Get(Stream_Video, 0, _T("Delay"))).To_int64u()+10000);
            
            
            MI_Result=MI.Option(_T("ParseSpeed"), _T("1.0"));
            if (!MI_Result.empty())
            {
                wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }

            MI_Result=MI.Option(_T("Demux"), _T("container"));
            if (!MI_Result.empty())
            {
                wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }

            MI_Result=MI.Option(_T("File_Demux_Unpacketize"), _T("1"));
            if (!MI_Result.empty())
            {
                wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }

            MI_Result=MI.Option(_T("File_Demux_PCM_20bitTo16bit"), _T("1"));
            if (!MI_Result.empty())
            {
                wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }

            MI_Result=MI.Option(_T("File_NextPacket"), _T("1"));
            if (!MI_Result.empty())
            {
                wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
                return;
            }
            int Counter=0;
            MI.Open(FilesList[FilesList_Pos].Name);
            while (MI.Open_NextPacket()&0x100)
            {
                Counter++;
                if (Counter==0)
                    MI.Option(_T("File_Seek"), _T("0"));
                if (Counter==100)
                    MI.Option(_T("File_Seek"), Delay_10s);
                if (Counter==200)
                    MI.Option(_T("File_Seek"), _T("Frame=100"));
                if (Counter==300)
                    MI.Option(_T("File_Seek"), _T("95%"));
            }
        }
        else
        {
            // We want all the stream, not only the beginning
            if (FilesList[FilesList_Pos].ParseSpeed)
            {
                MI_Result=MI.Option(_T("ParseSpeed"), _T("1.0"));
                if (!MI_Result.empty())
                {
                    wcout<<_T("MediaInfo error: ")<<MI_Result<<endl;
                    return;
                }
            }

            MI.Open(FilesList[FilesList_Pos].Name);
        }

        FilesList[FilesList_Pos].Clear();
    }


    ZtringList Ref=Dir::GetAllFileNames(DataBaseDirectory+_T("\\Events\\Ref\\*.*"));
    ZtringList New=Dir::GetAllFileNames(DataBaseDirectory+_T("\\Events\\New\\*.*"));
    for (size_t Ref_Pos=0; Ref_Pos<Ref.size(); Ref_Pos++)
    {
        Ztring Ref_ToFind=Ref[Ref_Pos];
        Ref_ToFind.FindAndReplace(_T("\\Events\\Ref\\"), _T("\\Events\\New\\"));
        size_t New_RefPos=New.Find(Ref_ToFind);
        bool IsDiff=false;
        if (New_RefPos!=(size_t)-1)
        {
            File F_Ref; F_Ref.Open(Ref[Ref_Pos]);
            File F_New; F_New.Open(New[New_RefPos]);
            if (F_Ref.Size_Get()==F_New.Size_Get())
            {
                int8u* F_Ref_Buffer=new int8u[(size_t)F_Ref.Size_Get()];
                F_Ref.Read(F_Ref_Buffer, (size_t)F_Ref.Size_Get());
                int8u* F_New_Buffer=new int8u[(size_t)F_New.Size_Get()];
                F_New.Read(F_New_Buffer, (size_t)F_New.Size_Get());

                if (memcmp(F_Ref_Buffer, F_New_Buffer, (size_t)F_Ref.Size_Get()))
                    IsDiff=true;
            }
            else
                IsDiff=true;
        }
        if (New_RefPos==(size_t)-1 || IsDiff)
        {
            //Not in new or is different
            Ztring Diff_Name=Ref[Ref_Pos];
            Diff_Name.FindAndReplace(_T("\\Events\\Ref\\"), _T("\\Events\\Diff\\"));
            if (!Dir::Exists(DataBaseDirectory+_T("\\Events\\Diff")))
                Dir::Create(DataBaseDirectory+_T("\\Events\\Diff"));
            if (!Dir::Exists(FileName(Diff_Name).Path_Get()))
                Dir::Create(FileName(Diff_Name).Path_Get());
            if (!IsDiff)
                File::Copy(Ref[Ref_Pos], Diff_Name+_T(".RefAlone.txt")); //Not in new
            else
            {
                File::Copy(Ref[Ref_Pos], Diff_Name+_T(".Ref.txt")); //Diff
                File::Copy(New[New_RefPos], Diff_Name+_T(".New.txt"));
            }
        }
        if (New_RefPos!=(size_t)-1)
            New.erase(New.begin()+New_RefPos);
    }
    for (size_t New_Pos=0; New_Pos<New.size(); New_Pos++)
    {
        //Not in ref
        Ztring Diff_Name=New[New_Pos];
        Diff_Name.FindAndReplace(_T("\\Events\\New\\"), _T("\\Events\\Diff\\"));
        if (!Dir::Exists(DataBaseDirectory+_T("\\Events\\Diff")))
            Dir::Create(DataBaseDirectory+_T("\\Events\\Diff"));
        if (!Dir::Exists(FileName(Diff_Name).Path_Get()))
            Dir::Create(FileName(Diff_Name).Path_Get());
         File::Copy(New[New_Pos], Diff_Name+_T(".NewAlone.txt")); //Not in new
    }
}
