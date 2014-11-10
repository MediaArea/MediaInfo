/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Easy_Box_Core.h"
#include "GUI/Common/GUI_Main_Easy_Core.h"
#include "ZenLib/Conf.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const size_t Title_Pos=94; //TODO: Position of Title in General.csv, should shange this...
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Easy_Box_Core::GUI_Main_Easy_Box_Core(Core* Core_, GUI_Main_Easy_Core* Parent_Core_, stream_t StreamKind, size_t StreamPos)
{
    //Internal
    C=Core_;
    Parent_Core=Parent_Core_;

    //Configuration
    GUI_Main_Easy_Box_Core::StreamKind=StreamKind;
    GUI_Main_Easy_Box_Core::StreamPos=StreamPos;
}

//***************************************************************************
// Actions - Per StreamKind
//***************************************************************************

//---------------------------------------------------------------------------
size_t GUI_Main_Easy_Box_Core::Boxes_Count_Get()
{
    return Parent_Core->Boxes_Count_Get(StreamKind);
}

//---------------------------------------------------------------------------
size_t GUI_Main_Easy_Box_Core::Boxes_Show_Get()
{
    size_t StreamCount=Parent_Core->StreamsCount_Get(StreamKind);
    if (Parent_Core->StreamsCount_Get(Stream_General)==0)
        StreamCount=Boxes_Count_Get();
    if (StreamCount>Boxes_Count_Get())
        StreamCount=Boxes_Count_Get();
    return StreamCount;
}

//---------------------------------------------------------------------------
size_t GUI_Main_Easy_Box_Core::Lines_Count_Get()
{
    switch (StreamKind)
    {
        case 0 : return 5;
        case 1 : return 2;
        case 2 : return 2;
        case 3 : return 2;
        default: return 0;
    }
}

//***************************************************************************
// Actions - Per Stream
//***************************************************************************

//---------------------------------------------------------------------------
bool GUI_Main_Easy_Box_Core::MustHide()
{
    size_t StreamCount=C->MI->Count_Get(Parent_Core->FilesPos_Get(), StreamKind);
    if (Parent_Core->StreamsCount_Get(Stream_General)==0)
        StreamCount=Boxes_Count_Get();
    if (StreamCount<=StreamPos)
        return true;
    else
        return false;
}

//---------------------------------------------------------------------------
bool GUI_Main_Easy_Box_Core::IsLastBox()
{
    if (StreamPos+1==Boxes_Show_Get())
        return true;
    else
        return false;
}

//---------------------------------------------------------------------------
String GUI_Main_Easy_Box_Core::Box_Get()
{
    String Temp;
    switch (StreamKind)
    {
        case Stream_General : Temp=__T("General"); break;
        case Stream_Video   : Temp=__T("Video");   break;
        case Stream_Audio   : Temp=__T("Audio");   break;
        case Stream_Text    : Temp=__T("Text");    break;
        default             : Temp=__T("");
    }

    return Temp;
}

//---------------------------------------------------------------------------
String GUI_Main_Easy_Box_Core::Text_Get()
{
    //First line
    if (Lines_Count_Get()==0)
        return __T("");
    String Temp=Parent_Core->Summary_Get(StreamKind, StreamPos);

    //Depend of StreamKind
    if (Parent_Core->StreamsCount_Get(Stream_General)==0)
    {
        //Empty box
        Temp+=EOL;
        Temp+=EOL;
    }
    else if (StreamKind==Stream_General)
    {
        //Stream_General
        size_t Lines=1;
        for (int KindOfStream=1; KindOfStream<Stream_Max; KindOfStream++)
        {
            if (C->MI->Count_Get(Parent_Core->FilesPos_Get(), (stream_t)KindOfStream)>0)
            {
                String Z1=String(C->MI->Get(Parent_Core->FilesPos_Get(), (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Measure))+__T("StreamCount");
                String Z2=String(__T(" "))+C->MI->Get(Parent_Core->FilesPos_Get(), (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text);
                if (C->MI->Count_Get(Parent_Core->FilesPos_Get(), (stream_t)KindOfStream)==1)
                    Z2+=__T(" stream");//__T(" stream1");
                else
                    Z2+=__T(" streams");//__T(" stream2");
                String Z3=String(C->MI->Get(Parent_Core->FilesPos_Get(), (stream_t)KindOfStream, 0, __T("StreamKind"), Info_Text)+__T("_Format_WithHint_List"));
                Temp+=EOL;
                Temp+=(C->MI->Get(Parent_Core->FilesPos_Get(), (stream_t)KindOfStream, 0, Z1)+/*Prefs->Translate(*/Z2/*)*/+/*Prefs->Translate(*/__T(": ")/*)*/+C->MI->Get(Parent_Core->FilesPos_Get(), Stream_General, 0, Z3)).c_str();
                Lines++;
            }

            if (Lines>=Lines_Count_Get())
                KindOfStream=Stream_Max;
        }
    }
    else if (Lines_Count_Get()>1)
    {
        //Other than Stream_General
        String Title=C->MI->Get(Parent_Core->FilesPos_Get(), StreamKind, StreamPos, __T("Title"));
        if (!Title.empty())
        {
            Temp+=EOL;
            Temp+=Title;
        }
    }

    return Temp.c_str();
}

//---------------------------------------------------------------------------
String GUI_Main_Easy_Box_Core::ToolTip_Get()
{
    return Parent_Core->Inform_Get(StreamKind, StreamPos);
}

//---------------------------------------------------------------------------
String GUI_Main_Easy_Box_Core::Tags_Get()
{
    //First line
    if (StreamKind!=Stream_General || Lines_Count_Get()==0)
        return __T("");

    size_t MI_Pos=Title_Pos; //Should begin with "Title"
    size_t MI_Max=C->MI->Count_Get(Parent_Core->FilesPos_Get(), Stream_General, 0);
    String Temp;
    size_t Lines=0;
    while(MI_Pos<MI_Max)
    {
        String Z1=C->MI->Get(Parent_Core->FilesPos_Get(), Stream_General, 0, MI_Pos);
        String Options=C->MI->Get(Parent_Core->FilesPos_Get(), Stream_General, 0, MI_Pos, Info_Options);
        if (Options.size()>InfoOption_ShowInInform && Options[InfoOption_ShowInInform]==__T('Y') && Z1.size()>0)
        {
            String Z2=C->MI->Get(Parent_Core->FilesPos_Get(), Stream_General, 0, MI_Pos, Info_Name_Text);
            if (Z2.size()==0)
                Z2=C->MI->Get(Parent_Core->FilesPos_Get(), Stream_General, 0, MI_Pos, Info_Name);
            Z2+=/*Prefs->Translate(*/__T(": ")/*)*/;
            Z2+=Z1;
            if (Lines>0)
                Temp+=EOL;
            Temp+=Z2;

            Lines++;
            if (Lines>=Lines_Count_Get())
                MI_Pos=MI_Max;
        }
        MI_Pos++;
    }

    return Temp;
}

//---------------------------------------------------------------------------
String GUI_Main_Easy_Box_Core::Button_Get()
{
    if (StreamKind==Stream_General)
        return __T("Go to the website of a player for this file");
    else
        return __T("Go to the website of this codec");
}

//---------------------------------------------------------------------------
bool GUI_Main_Easy_Box_Core::Button_Show()
{
    if (Parent_Core->CodecID_Url_Get(StreamKind, StreamPos).empty())
        return false;
    else
        return true;
}

//---------------------------------------------------------------------------
String GUI_Main_Easy_Box_Core::Button_Click()
{
    return Parent_Core->CodecID_Url_Get(StreamKind, StreamPos);
}


