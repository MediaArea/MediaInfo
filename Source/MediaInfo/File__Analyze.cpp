// File__Analyze - Base for analyze files
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#include "MediaInfo/MediaInfo_Config.h"
#include <cstring>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File__Analyze::File__Analyze ()
:File__Base()
{
    //Save for speed improvement
    #ifndef MEDIAINFO_MINIMIZESIZE
        Config_Details=0;
    #endif //MEDIAINFO_MINIMIZESIZE
    IsSub=false;

    //Configuration
    DataMustAlwaysBeComplete=true;
    MustUseAlternativeParser=false;

    //Buffer
    Buffer=NULL;
    Buffer_Temp=NULL;
    Buffer_Size=0;
    Buffer_Temp_Size=0;
    Buffer_Temp_Size_Max=0;
    Buffer_Offset=0;
    Buffer_Offset_Temp=0;
    Buffer_MinimumSize=0;
    Buffer_MaximumSize=1024*1024;
    Buffer_Init_Done=false;

    //EOF
    EOF_AlreadyDetected=false;

    //Synchro
    MustParseTheHeaderFile=true;
    Synched=false;
    Trusted=Error;
    Trusted_Multiplier=1;

    //Header
    Header_Size=0;

    //Element
    Element_Offset=0;
    Element_Size=0;

    //Elements
    Element.resize(64);
    Element[0].Code=0;
    Element[0].Next=File_Size;
    Element[0].WaitForMoreData=false;
    Element[0].UnTrusted=false;
    Element[0].IsComplete=false;
    Element[0].InLoop=false;
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        //ToShow part
        Element[0].ToShow.Name.clear();
        Element[0].ToShow.Info.clear();
        Element[0].ToShow.Details.clear();
        Element[0].ToShow.NoShow=false;
    }
    #endif //MEDIAINFO_MINIMIZESIZE
    Element_Level_Base=0;
    Element_Level=0;

    //BitStream
    BS=new BitStream;

    //Temp
    NewFinnishMethod=false;
    IsFinnished=false;
    ShouldContinueParsing=false;
}

//---------------------------------------------------------------------------
File__Analyze::~File__Analyze ()
{
    //Buffer
    delete[] Buffer_Temp; //Buffer_Temp=NULL;

    //BitStream
    delete BS; //BS=NULL;
}

//***************************************************************************
// Open
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Init (int64u File_Size_, int64u File_Offset_)
{
    if (!Stream)
    {
        Stream=new std::vector<std::vector<ZtringList> >;
        Stream->resize(Stream_Max);
        Stream_More=new std::vector<std::vector<ZtringListList> >;
        Stream_More->resize(Stream_Max);
        Stream_MustBeDeleted=true;
    }

    //Preparing
    File_Size=File_Size_;
    File_Offset=File_Offset_;
    Element[0].Next=File_Size;

    //Options
    if (!IsSub)
        IsSub=Config->File_IsSub_Get();

    //Format
    if (!Buffer_Init_Done)
    {
        Read_Buffer_Init();
        Buffer_Init_Done=true;
    }

    //Integrity
    if (File_Offset>=File_Size)
    {
        Buffer_Clear();
        Finnished();
        return; //There is a problem
    }

    //Jump handling
    if (!Buffer_Init_Done || File_GoTo!=(int64u)-1)
        Read_Buffer_Unsynched();
}

void File__Analyze::Open_Buffer_Init (File__Analyze* Sub)
{
    Open_Buffer_Init(Sub, File_Size, File_Offset+Buffer_Offset+Element_Offset);
}

void File__Analyze::Open_Buffer_Init (File__Analyze* Sub, int64u File_Size_, int64u File_Offset_)
{
    //Integrity
    if (Sub==NULL)
        Sub=this;

    //Parsing
    Sub->Init(Config, Details);
    Sub->IsSub=true;
    Sub->Open_Buffer_Init(File_Size_, File_Offset_);

    //Configuring
    File_MaximumOffset=MediaInfoLib::Config.FormatDetection_MaximumOffset_Get();
}

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Continue (const int8u* ToAdd, size_t ToAdd_Size)
{
    //Integrity
    if (ToAdd==NULL || File_Offset==File_Size)
        return;

    //{File F; F.Open(Ztring(_T("d:\\direct"))+Ztring::ToZtring((size_t)this, 16), File::Access_Write_Append); F.Write(ToAdd, ToAdd_Size);}

    //Demand to go elsewhere
    if (File_GoTo!=(int64u)-1)
    {
        if (File_GoTo>File_Offset+ToAdd_Size)
            return; //No need of this piece of data

        //The needed offset is in the new buffer
        ToAdd+=(size_t)(File_GoTo-File_Offset);
        ToAdd_Size-=(size_t)(File_GoTo-File_Offset);
    }

    if (Buffer_Temp_Size) //There is buffered data from before
    {
        //Allocating new buffer if needed
        if (Buffer_Temp_Size+ToAdd_Size>Buffer_Temp_Size_Max)
        {
            int8u* Old=Buffer_Temp;
            size_t Buffer_Temp_Size_Max_ToAdd=ToAdd_Size>32768?ToAdd_Size:32768;
            if (Buffer_Temp_Size_Max_ToAdd<Buffer_Temp_Size_Max) Buffer_Temp_Size_Max_ToAdd=Buffer_Temp_Size_Max;
            Buffer_Temp_Size_Max+=Buffer_Temp_Size_Max_ToAdd;
            Buffer_Temp=new int8u[Buffer_Temp_Size_Max];
            std::memcpy(Buffer_Temp, Old, Buffer_Temp_Size);
            delete[] Old; //Old=NULL;
        }

        //Copying buffer
        if (ToAdd_Size>0)
        {
            std::memcpy(Buffer_Temp+Buffer_Size, ToAdd, ToAdd_Size);
            Buffer_Temp_Size+=ToAdd_Size;
        }
        
        //Buffer
        Buffer=Buffer_Temp;
        Buffer_Size=Buffer_Temp_Size;
    }
    else
    {
        Buffer=ToAdd;
        Buffer_Size=ToAdd_Size;
    }

    //Preparing
    Buffer_Offset=0;
    Trusted=(Buffer_Size>2*8*1024?Buffer_Size/8/1024:2)*Trusted_Multiplier; //Never less than 2 acceptable errors

    //Parsing
    if (Buffer_Size>=Buffer_MinimumSize || File_Offset+Buffer_Size==File_Size) //Parsing only if we have enough buffer
        Open_Buffer_Continue_Loop();

    //Registering the first synch offset
    if (File_Offset_FirstSynched==(int64u)-1 && Synched)
        File_Offset_FirstSynched=File_Offset; //This is ~this offset, we don't know precisely the offset synched in the buffer

    //Finnished?
    if ((File_GoTo==File_Size && File_Size!=(int64u)-1)|| File_Offset+Buffer_Offset>=File_Size)
    {
        if (!BookMark_Code.empty())
            BookMark_Get();

        if (File_GoTo>=File_Size)
        {
            File_Offset=File_Size;
            File_GoTo=File_Size;
            Buffer_Clear();
            Element_Show(); //If Element_Level is >0, we must show what is in the details buffer
            while (Element_Level>0)
                Element_End(); //This is finnished, must flush
            return;
        }
    }

    //Detection is parsing too much
    if (!File_Name.empty() && Count_Get(Stream_General)==0 && File_Offset>File_MaximumOffset)
    {
        //Starter not found, we abandon
        Buffer_Clear();
        File_Offset=File_Size;
        File_GoTo=File_Size;
        return;
    }

    //Demand to go elsewhere
    if (File_GoTo!=(int64u)-1)
    {
        if (Config->File_IsSeekable_Get())
        {
            if (File_GoTo>=File_Size)
                File_GoTo=File_Size;
            Buffer_Clear();
        }
        return;
    }
    if (Buffer_Offset>=Buffer_Size)
    {
        if (Buffer_Offset>Buffer_Size)
            File_GoTo=File_Offset+Buffer_Offset;
        if (File_Offset!=(int64u)-1)
            File_Offset+=Buffer_Offset;
        Buffer_Clear();
        return;
    }

    //Buffer handling
    if (Buffer_Offset!=Buffer_Size) //all is not used
    {
        if (Buffer_Temp_Size==0) //If there was no copy
        {
            if (Buffer_Temp!=NULL && Buffer_Temp_Size_Max<ToAdd_Size-Buffer_Offset)
            {
                delete[] Buffer_Temp; Buffer_Temp=NULL; Buffer_Temp_Size=0; Buffer_Temp_Size_Max=0;
            }
            if (Buffer_Temp==NULL)
            {
                size_t Buffer_Temp_Size_Max_ToAdd=ToAdd_Size-Buffer_Offset>32768?ToAdd_Size-Buffer_Offset:32768;
                if (Buffer_Temp_Size_Max_ToAdd<Buffer_Temp_Size_Max) Buffer_Temp_Size_Max_ToAdd=Buffer_Temp_Size_Max;
                Buffer_Temp_Size_Max=Buffer_Temp_Size_Max_ToAdd;
                Buffer_Temp=new int8u[Buffer_Temp_Size_Max];
            }
            Buffer_Temp_Size=ToAdd_Size-Buffer_Offset;
            std::memcpy(Buffer_Temp, ToAdd+Buffer_Offset, Buffer_Temp_Size);
        }
        else //Already a copy, just moving it
        {
            std::memmove(Buffer_Temp, Buffer_Temp+Buffer_Offset, Buffer_Size-Buffer_Offset);
            Buffer_Temp_Size=Buffer_Size-Buffer_Offset;
        }
    }
    else if (Buffer_Temp_Size)
        Buffer_Temp_Size=0;

    //Reserving unused data
    if ((int64u)-1-Buffer_Offset<File_Offset) //In case of unknown filesize, File_Offset may be (int64u)-1
        Buffer_Offset=(size_t)((int64u)-1-File_Offset);
    Buffer_Size-=Buffer_Offset;
    File_Offset+=Buffer_Offset;
    if (Buffer_Offset_Temp>=Buffer_Offset)
        Buffer_Offset_Temp-=Buffer_Offset;
    Buffer_Offset=0;

    //Is it OK?
    if (Buffer_Size>Buffer_MaximumSize)
    {
        //Buffer too big, we abandon
        Buffer_Clear();
        File_Offset=File_Size;
        File_GoTo=File_Size;
        return;
    }
}

void File__Analyze::Open_Buffer_Continue (File__Analyze* Sub, const int8u* ToAdd, size_t ToAdd_Size)
{
    if (Sub==NULL)
        Sub=this;

    //Sub
    #ifndef MEDIAINFO_MINIMIZESIZE
        Sub->Element_Level_Base=Element_Level_Base+Element_Level;
    #endif

    //{File F; F.Open(Ztring(_T("d:\\direct"))+Ztring::ToZtring((size_t)this, 16), File::Access_Write_Append); F.Write(ToAdd, ToAdd_Size);}

    //Adaptating File_Offset
    if (Sub!=this && Sub->Buffer_Size<=Sub->File_Offset)
        Sub->File_Offset-=Sub->Buffer_Size;

    //Parsing
    Sub->Open_Buffer_Continue(ToAdd, ToAdd_Size);

    #ifndef MEDIAINFO_MINIMIZESIZE
        //Details handling
        if (!Sub->Element[0].ToShow.Details.empty())
        {
            //Line separator
            if (!Element[Element_Level].ToShow.Details.empty())
                Element[Element_Level].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();

            //From Sub
            Element[Element_Level].ToShow.Details+=Sub->Element[0].ToShow.Details;
            Sub->Element[0].ToShow.Details.clear();
        }
        else
            Element[Element_Level].ToShow.NoShow=true; //We don't want to show this item because there is no info in it
    #endif
}

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Continue_Loop ()
{
    //Save for speed improvement
    #ifndef MEDIAINFO_MINIMIZESIZE
        Config_Details=MediaInfoLib::Config.Details_Get();
    #endif
    
    //Parsing specific
    Frame_Count_InThisBlock=0; //Out
    Read_Buffer_Continue();
    if (File_GoTo!=(int64u)-1)
        return; //Finnished

    //Header
    if (MustParseTheHeaderFile)
    {
        if (!FileHeader_Manage())
            return; //Wait for more data
        if (File_GoTo!=(int64u)-1)
            return; //Finnished
    }

    //Parsing;
    while (Buffer_Parse());

    //Jumping to the end of the file if needed
    if (!EOF_AlreadyDetected && Count_Get(Stream_General))
    {
        Element[Element_Level].WaitForMoreData=false;
        Detect_EOF();
        if ((File_GoTo!=(int64u)-1 && File_GoTo>File_Offset+Buffer_Offset) || File_Offset==File_Size || File_Offset==(int64u)-1 || (IsFinnished && !ShouldContinueParsing))
        {
            EOF_AlreadyDetected=true;
            return;
        }
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Finalize (bool NoBufferModification)
{
    //File with unknown size (stream...), finnishing
    if (!NoBufferModification && File_Size==(int64u)-1)
    {
        File_Size=File_Offset+Buffer_Size;
        Open_Buffer_Continue(NULL, 0);
    }

    //Format
    Read_Buffer_Finalize();

    //Element must be finnished
    while (Element_Level>0)
        Element_End();

    //Parsing
    if (!NoBufferModification)
        Finnished();
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (Details)
        Details->assign(Element[0].ToShow.Details);
    #endif //MEDIAINFO_MINIMIZESIZE

    //Buffer
    if (!NoBufferModification)
        Buffer_Clear();
}

void File__Analyze::Open_Buffer_Finalize (File__Analyze* Sub)
{
    if (Sub==NULL)
        Sub=this;

    //Finalize
    Sub->Open_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Unsynched()
{
    Synched=false;
    File_GoTo=(int64u)-1;
    Buffer_Clear();
}

//---------------------------------------------------------------------------
bool File__Analyze::Buffer_Parse()
{
    //End of this level?
    if (File_Offset+Buffer_Offset>=Element[Element_Level].Next)
    {
        if (Element[Element_Level].InLoop)
            return false; //To next element
        else
        {
            //There is no loop handler, so we make the level down here
            while (Element_Level>0 && File_Offset+Buffer_Offset>=Element[Element_Level].Next)
                Element_End(); //This is a buffer restart, must sync to Element level
            if (File_Offset+Buffer_Offset==File_Size)
                return false; //End of file
        }
        MustUseAlternativeParser=false; //Reset it if we go out of an element
    }

    //Header
    if (!Header_Manage())
        return false; //Wait for more data

    //Data
    if (!Data_Manage())
        return false; //Wait for more data

    return true;
}

//---------------------------------------------------------------------------
void File__Analyze::Buffer_Clear()
{
    //Buffer
    BS->Attach(NULL, 0);
    Buffer_Size=0;
    Buffer_Temp_Size=0;
    Buffer_Offset=0;
    Buffer_Offset_Temp=0;
    Buffer_MinimumSize=0;

    //Details
    #ifndef MEDIAINFO_MINIMIZESIZE
        Element[Element_Level].WaitForMoreData=false; //We must finalize the details
        Element[Element_Level].IsComplete=true; //We must finalize the details
    #endif //MEDIAINFO_MINIMIZESIZE

}

//***************************************************************************
// File Header
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::FileHeader_Manage()
{
    //From the parser
    if (!FileHeader_Begin())
        return false; //Wait for more data

    //From the parser
    Element_Size=Buffer_Size;
    Element_Begin("File Header");
    FileHeader_Parse();
    Element_End();

    //Testing the parser result
    if (Element_IsWaitingForMoreData() || Element[Element_Level].UnTrusted) //Wait or problem
    {
        //The header is not complete, need more data
        #ifndef MEDIAINFO_MINIMIZESIZE
        Element[Element_Level].ToShow.Details.clear();
        #endif //MEDIAINFO_MINIMIZESIZE
        return false;
    }

    //Positionning
    Buffer_Offset+=(size_t)Element_Offset;

    MustParseTheHeaderFile=false;
    return true;
}

//***************************************************************************
// Header
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::Header_Manage()
{
    //Test
    if (Buffer_Offset>=Buffer_Size)
        return false;

    //From the parser
    Element[Element_Level].IsComplete=true;
    if (!Header_Begin())
    {
        //Jumping to the end of the file if needed
        if (!EOF_AlreadyDetected && File_GoTo==(int64u)-1)
        {
            Element[Element_Level].WaitForMoreData=false;
            Detect_EOF();
            if ((File_GoTo!=(int64u)-1 && File_GoTo>File_Offset+Buffer_Offset) || File_Offset==File_Size || File_Offset==(int64u)-1 || (IsFinnished && !ShouldContinueParsing))
                EOF_AlreadyDetected=true;
        }
        return false; //Wait for more data
    }
    Element[Element_Level].UnTrusted=false;

    //Going in a lower level
    Element_Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get());
    if (Buffer_Offset+Element_Size>Buffer_Size)
    {
        Element_Size=Buffer_Size-Buffer_Offset;
        Element[Element_Level].IsComplete=false;
    }
    if (Element_Size==0)
        return false;
    Element_Offset=0;
    Element_Begin(); //Element
    Data_Level=Element_Level;
    Element_Begin("Header"); //Header

    //From the parser
    Header_Parse();

    //Testing the parser result
    if (Element[Element_Level].UnTrusted) //Problem
    {
        Header_Fill_Code(0, "Problem");
        Header_Fill_Size(Element_Size);
    }
    if (Element_IsWaitingForMoreData() || (DataMustAlwaysBeComplete && Element[Element_Level-1].Next>File_Offset+Buffer_Size)) //Wait or want to have a comple data chunk
    {
        //The header is not complete, need more data
        Element[Element_Level].WaitForMoreData=true;
        Element_End(); //Header
        Element_End(); //Element
        return false;
    }

    //Filling
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (Element[Element_Level-1].ToShow.Name.empty())
        Element[Element_Level-1].ToShow.Name=_T("Unknown");
    #endif //MEDIAINFO_MINIMIZESIZE
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].UnTrusted=false;
    Element[Element_Level].IsComplete=true;

    //ToShow
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element_Offset;
        Element[Element_Level].ToShow.Header_Size=0;
        Element[Element_Level-1].ToShow.Header_Size=Header_Size;
    }
    #endif //MEDIAINFO_MINIMIZESIZE

    //Integrity
    if (Element[Element_Level-1].Next<(File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get()))
        //Size is not good
        Element[Element_Level-1].Next=File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get();

    //Positionning
    Element_Size=Element[Element_Level-1].Next-(File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get());
    Header_Size=Element_Offset;
    Buffer_Offset+=(size_t)Header_Size;
    Element_Offset=0;
    if (Buffer_Offset+Element_Size>Buffer_Size)
    {
        if (Buffer_Size>Buffer_Offset)
            Element_Size=Buffer_Size-Buffer_Offset;
        else
            Element_Size=0; //There is an error in the parsing
        Element[Element_Level-1].IsComplete=false;
    }

    Element_End(); //Header
    return true;
}

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Header_Fill_Code(int64u Code, const Ztring &Name)
{
    //Filling
    Element[Element_Level-1].Code=Code;

    //ToShow
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Element[Element_Level-1].ToShow.Name=Name;
    }
}

#else //MEDIAINFO_MINIMIZESIZE
void File__Analyze::Header_Fill_Code(int64u Code)
{
    //Filling
    Element[Element_Level-1].Code=Code;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File__Analyze::Header_Fill_Size(int64u Size)
{
    if (Element[Element_Level].UnTrusted)
        return;

    //Integrity
    if (Size<Element_Offset)
        Size=Element_Offset; //At least what we read before!!!
        
    //Filling
    Element[Element_Level-1].Next=File_Offset+Buffer_Offset+Size;
    if (Element[Element_Level-1].Next>Element[Element_Level-2].Next)
    {
        Element[Element_Level-1].Next=Element[Element_Level-2].Next;
        Element[Element_Level-1].IsComplete=false;
    }
    else
        Element[Element_Level-1].IsComplete=true;

    //ToShow
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Element[Element_Level-1].ToShow.Pos=File_Offset+Buffer_Offset;
        Element[Element_Level-1].ToShow.Size=Element[Element_Level-1].Next-(File_Offset+Buffer_Offset);
    }
    #endif //MEDIAINFO_MINIMIZESIZE
}

//***************************************************************************
// Data
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::Data_Manage()
{
    Element_Code=Element[Element_Level].Code;
    //size_t Element_Level_Save=Element_Level;
    Element_WantNextLevel=false;
    Data_Parse();
    BS->Attach(NULL, 0); //Clear it
    //Element_Level=Element_Level_Save;

    //Testing the parser result
    if (Element_IsWaitingForMoreData())
    {
        //The data is not complete, need more data
        Element_End(); //Element
        Buffer_Offset-=(size_t)Header_Size;
        return false;
    }

    Element[Element_Level].IsComplete=true;

    //If no need of more
    if ((File_GoTo!=(int64u)-1 && File_GoTo>File_Offset+Buffer_Offset) || File_Offset==File_Size || File_Offset==(int64u)-1 || (IsFinnished && !ShouldContinueParsing))
    {
        Element_Offset=0;
        return false;
    }
    
    //Next element
    if (!Element_WantNextLevel)
    {
        if (Element[Element_Level].Next<=File_Offset+Buffer_Size)
            Element_Offset=(size_t)(Element[Element_Level].Next-File_Offset-Buffer_Offset);
        else
        {
            File_GoTo=Element[Element_Level].Next;
            return false;
        }
    }
    if (Buffer_Offset+Element_Offset>Buffer_Size && File_Offset!=File_Size)
        File_GoTo=File_Offset+Buffer_Offset+Element_Offset; //Preparing to go far
    Buffer_Offset+=(size_t)Element_Offset;
    Header_Size=0;
    Element_Size=0;
    Element_Offset=0;

    #ifndef MEDIAINFO_MINIMIZESIZE
    if (Element_Level>0)
        Element[Element_Level-1].ToShow.NoShow=Element[Element_Level].ToShow.NoShow; //If data must not be shown, we hide the header too
    else
        Element[0].ToShow.NoShow=false; //This should never happen, but in case of
    #endif //MEDIAINFO_MINIMIZESIZE
    Element_End(); //Element
    if (Element_WantNextLevel)
        Element_Level++;
    Element[Element_Level].UnTrusted=false;

    //Jumping to the end of the file if needed
    if (!EOF_AlreadyDetected && File_GoTo==(int64u)-1)
    {
        Element[Element_Level].WaitForMoreData=false;
        Detect_EOF();
        if ((File_GoTo!=(int64u)-1 && File_GoTo>File_Offset+Buffer_Offset) || File_Offset==File_Size || File_Offset==(int64u)-1 || (IsFinnished && !ShouldContinueParsing))
        {
            EOF_AlreadyDetected=true;
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Data_Info (const Ztring &Parameter)
{
    size_t Element_Level_Temp=Element_Level;
    Element_Level=Data_Level;
    Element_Info(Parameter);
    Element_Level=Element_Level_Temp;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Data_GoTo (int64u GoTo, const char* ParserName)
{
    Element_Show();

    if (IsSub)
    {
        Info(Ztring(ParserName)+(ShouldContinueParsing?_T(" detected"):_T(", parsing finnished")), 1);
        Finnished();
        return;
    }

    if (Element_Level>0)
        Element_End(); //Element

    if (GoTo==File_Size)
    {
        Info(Ztring(ParserName)+_T(", parsing finnished"));
        Finnished();
    }
    else
    {
        Info(Ztring(ParserName)+_T(", jumping to offset ")+Ztring::ToZtring(GoTo, 16));
        File_GoTo=GoTo;
    }
}
#else //MEDIAINFO_MINIMIZESIZE
void File__Analyze::Data_GoTo (int64u GoTo)
{
    if (IsSub)
    {
        Finnished();
        return;
    }

    if (Element_Level>0)
        Element_End(); //Element

    if (GoTo==File_Size)
        Finnished();
    else
        File_GoTo=GoTo;
}
#endif //MEDIAINFO_MINIMIZESIZE

//***************************************************************************
// Element
//***************************************************************************

//---------------------------------------------------------------------------
Ztring Log_Offset (int64u OffsetToShow)
{
    if (OffsetToShow==(int64u)-1)
        return _T("         ");
    int64u Offset=OffsetToShow%0x100000000ULL; //Only 32 bits
    Ztring Pos1; Pos1.From_Number(Offset, 16);
    Ztring Pos2;
    Pos2.resize(8-Pos1.size(), _T('0'));
    Pos2+=Pos1;
    Pos2.MakeUpperCase();
    Pos2+=_T(' ');
    return Pos2;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Begin()
{
    //Level
    Element_Level++;
    Header_Size=0;

    //Element
    Element[Element_Level].Code=0;
    Element[Element_Level].Next=Element[Element_Level-1].Next;
    Element[Element_Level].WaitForMoreData=Element[Element_Level-1].WaitForMoreData;
    Element[Element_Level].UnTrusted=Element[Element_Level-1].UnTrusted;
    Element[Element_Level].IsComplete=Element[Element_Level-1].IsComplete;
    Element[Element_Level].InLoop=Element[Element_Level-1].InLoop;

    //ToShow
    #ifndef MEDIAINFO_MINIMIZESIZE
    Element[Element_Level].ToShow.Pos=File_Offset+Buffer_Offset+Element_Offset+BS->OffsetBeforeLastCall_Get(); //TODO: change this, used in Element_End()
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Offset+BS->OffsetBeforeLastCall_Get());
        Element[Element_Level].ToShow.Header_Size=0;
        Element[Element_Level].ToShow.Name.clear();
        Element[Element_Level].ToShow.Info.clear();
        Element[Element_Level].ToShow.Details.clear();
        Element[Element_Level].ToShow.NoShow=false;
    }
    #endif //MEDIAINFO_MINIMIZESIZE
}

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_Begin(const Ztring &Name, int64u Size)
{
    //Level
    Element_Level++;
    Header_Size=0;

    //Element
    Element[Element_Level].Code=0;
    if (Size==(int64u)-1)
        Element[Element_Level].Next=Element[Element_Level-1].Next;
    else
    {
        Element[Element_Level].Next=File_Offset+Buffer_Offset+Element_Offset+Size;
        if (Element[Element_Level].Next>Element[Element_Level-1].Next)
            Element[Element_Level].Next=Element[Element_Level-1].Next;
    }
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].UnTrusted=Element[Element_Level-1].UnTrusted;
    Element[Element_Level].IsComplete=Element[Element_Level-1].IsComplete;
    Element[Element_Level].InLoop=false;

    //ToShow
    Element[Element_Level].ToShow.Pos=File_Offset+Buffer_Offset+Element_Offset+BS->OffsetBeforeLastCall_Get(); //TODO: change this, used in Element_End()
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Offset+BS->OffsetBeforeLastCall_Get());
        Element[Element_Level].ToShow.Header_Size=0;
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
        Element[Element_Level].ToShow.Info.clear();
        Element[Element_Level].ToShow.Details.clear();
        Element[Element_Level].ToShow.NoShow=false;
    }
}
#else //MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_Begin(int64u Size)
{
    //Level
    Element_Level++;
    Header_Size=0;

    //Element
    Element[Element_Level].Code=0;
    if (Size==(int64u)-1)
        Element[Element_Level].Next=Element[Element_Level-1].Next;
    else
    {
        Element[Element_Level].Next=File_Offset+Buffer_Offset+Element_Offset+Size;
        if (Element[Element_Level].Next>Element[Element_Level-1].Next)
            Element[Element_Level].Next=Element[Element_Level-1].Next;
    }
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].UnTrusted=Element[Element_Level-1].UnTrusted;
    Element[Element_Level].IsComplete=Element[Element_Level-1].IsComplete;
    Element[Element_Level].InLoop=false;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_Name(const Ztring &Name)
{
    //ToShow
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
        else
            Element[Element_Level].ToShow.Name=_T("(Empty)");
    }
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_Info(const Ztring &Parameter)
{
    //Coherancy
    if (MediaInfoLib::Config.Details_Get()==0 || Element[Element_Level].ToShow.Details.size()>64*1024*1024)
        return;

    //Needed?
    if (MediaInfoLib::Config.Details_Get()<=0.7)
        return;

    //ToShow
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Ztring Parameter2(Parameter);
        Parameter2.FindAndReplace(_T("\r\n"), _T(" / "));
        Parameter2.FindAndReplace(_T("\r"), _T(" / "));
        Parameter2.FindAndReplace(_T("\n"), _T(" / "));
        Element[Element_Level].ToShow.Info+=Ztring(_T(" - "))+Parameter2;
    }
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_End(const Ztring &Name, int64u Size)
{
    //Element
    if (Size!=(int64u)-1)
    {
        Element[Element_Level].Next=Element[Element_Level].ToShow.Pos+Size; //TODO: change this
        if (Element[Element_Level].Next>Element[Element_Level-1].Next)
            Element[Element_Level].Next=Element[Element_Level-1].Next;
    }

    //ToShow
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-Element[Element_Level].ToShow.Pos;
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
    }

    Element_End_Common_Flush();
}
#else //MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_End(int64u Size)
{
    //Element
    if (Size!=(int64u)-1)
    {
        if (Element[Element_Level].Next>Element[Element_Level-1].Next)
            Element[Element_Level].Next=Element[Element_Level-1].Next;
    }

    Element_End_Common_Flush();
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File__Analyze::Element_End()
{
    Element_End_Common_Flush();
}


//***************************************************************************
// Element - Common
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Element_End_Common_Flush()
{
    #ifndef MEDIAINFO_MINIMIZESIZE
    //Size if not filled
    if (File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get()<Element[Element_Level].Next)
        Element[Element_Level].ToShow.Size=File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get()-Element[Element_Level].ToShow.Pos;
    #endif //MEDIAINFO_MINIMIZESIZE

    //Level
    if (Element_Level==0)
        return;

    //Element level
    Element_Level--;

    //Element
    Element[Element_Level].UnTrusted=Element[Element_Level+1].UnTrusted;
    Element[Element_Level].WaitForMoreData=Element[Element_Level+1].WaitForMoreData;

    #ifndef MEDIAINFO_MINIMIZESIZE
        Element_End_Common_Flush_Details();
    #endif //MEDIAINFO_MINIMIZESIZE
}

#ifndef MEDIAINFO_MINIMIZESIZE
//---------------------------------------------------------------------------
void File__Analyze::Element_End_Common_Flush_Details()
{
    Element[Element_Level].ToShow.NoShow=Element[Element_Level+1].ToShow.NoShow;
    if (MediaInfoLib::Config.Details_Get()!=0)
    {
        if (!Element[Element_Level+1].WaitForMoreData && (Element[Element_Level+1].IsComplete || !Element[Element_Level+1].UnTrusted) && !Element[Element_Level+1].ToShow.NoShow)// && MediaInfoLib::Config.Details_Get()!=0 && Element[Element_Level].ToShow.Details.size()<=64*1024*1024)
        {
            //Element
            if (!Element[Element_Level+1].ToShow.Name.empty())
            {
                if (!Element[Element_Level].ToShow.Details.empty())
                    Element[Element_Level].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();
                Element[Element_Level].ToShow.Details+=Element_End_Common_Flush_Build();
                Element[Element_Level+1].ToShow.Name.clear();
            }

            //Info
            if (!Element[Element_Level+1].ToShow.Details.empty())
            {
                if (!Element[Element_Level].ToShow.Details.empty())
                    Element[Element_Level].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();
                Element[Element_Level].ToShow.Details+=Element[Element_Level+1].ToShow.Details;
                Element[Element_Level+1].ToShow.Details.clear();
            }
        }
    }
}
#endif //MEDIAINFO_MINIMIZESIZE

#ifndef MEDIAINFO_MINIMIZESIZE
//---------------------------------------------------------------------------
Ztring File__Analyze::Element_End_Common_Flush_Build()
{
    Ztring ToReturn;

    //Show Offset
    if (MediaInfoLib::Config.Details_Get()>0.7)
    {
        ToReturn+=Log_Offset(Element[Element_Level+1].ToShow.Pos);
    }

    //Name
    ToReturn.resize(ToReturn.size()+Element_Level_Base+Element_Level, _T(' '));
    ToReturn+=Element[Element_Level+1].ToShow.Name;

    //Info
    ToReturn+=Element[Element_Level+1].ToShow.Info;
    Element[Element_Level+1].ToShow.Info.clear();

    //Size
    if (MediaInfoLib::Config.Details_Get()>0.3)
    {
        ToReturn+=_T(" (");
        ToReturn+=Ztring::ToZtring(Element[Element_Level+1].ToShow.Size);
        if (Element[Element_Level+1].ToShow.Header_Size>0)
        {
            ToReturn+=_T("/");
            ToReturn+=Ztring::ToZtring(Element[Element_Level+1].ToShow.Size-Element[Element_Level+1].ToShow.Header_Size);
        }
        ToReturn+=_T(" bytes)");
    }

    return ToReturn;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File__Analyze::Element_Prepare (int64u Size)
{
    Element_Offset=0;
    Element_Size=Size;
    #ifndef MEDIAINFO_MINIMIZESIZE
    Element[Element_Level].ToShow.Size=Size;
    #endif //MEDIAINFO_MINIMIZESIZE
}
//***************************************************************************
// Param
//***************************************************************************

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Param(const Ztring& Parameter, const Ztring& Value)
{
    if (MediaInfoLib::Config.Details_Get()==0)
        return;

    //Position
    int64u Pos=Element_Offset+BS->OffsetBeforeLastCall_Get();

    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;

    const size_t Padding_Value=40;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();

    //Show Offset
    if (MediaInfoLib::Config.Details_Get()>0.7)
    {
        Element[Element_Level].ToShow.Details+=Log_Offset(Pos==(int64u)-1?Pos:(File_Offset+Buffer_Offset+Pos));
    }

    //Show Parameter
    Ztring Param; Param=Parameter;
    if (Param.size()>Padding_Value) Param.resize(Padding_Value);
    Element[Element_Level].ToShow.Details.resize(Element[Element_Level].ToShow.Details.size()+Element_Level_Base+Element_Level, _T(' '));
    Element[Element_Level].ToShow.Details+=Param;

    //Show Value
    if (!Value.empty())
    {
        Element[Element_Level].ToShow.Details+=_T(": ");
        Element[Element_Level].ToShow.Details.resize(Element[Element_Level].ToShow.Details.size()+Padding_Value-Param.size()-Element_Level+1, _T(' '));
        Ztring Value2(Value);
        Value2.FindAndReplace(_T("\r\n"), _T(" / "));
        Value2.FindAndReplace(_T("\r"), _T(" / "));
        Value2.FindAndReplace(_T("\n"), _T(" / "));
        Element[Element_Level].ToShow.Details+=Value2;
    }
}
#endif //MEDIAINFO_MINIMIZESIZE

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Info(const Ztring& Value, size_t Element_Level_Minus)
{
    //Handling a different level (only Element_Level_Minus to 1 is currently well supported)
    size_t Element_Level_Final=Element_Level;
    if (Element_Level_Minus<=Element_Level)
    {
        if (Element_Level_Minus==1)
        {
            Element_Level--;
            Element_End_Common_Flush_Details();
            Element_Level++;
        }
        Element_Level_Final-=Element_Level_Minus;
    }

    if (MediaInfoLib::Config.Details_Get()==0)
        return;

    //Coherancy
    if (Element[Element_Level_Final].UnTrusted)
        return;

    //Line separator
    if (!Element[Element_Level_Final].ToShow.Details.empty())
        Element[Element_Level_Final].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();

    //Preparing
    Ztring ToShow; ToShow.resize(Element_Level_Final, _T(' '));
    ToShow+=_T("---   ");
    ToShow+=Value;
    ToShow+=_T("   ---");
    Ztring Separator; Separator.resize(Element_Level_Final, _T(' '));
    Separator.resize(ToShow.size(), _T('-'));

    //Show Offset
    Ztring Offset;
    if (MediaInfoLib::Config.Details_Get()>0.7)
        Offset=Log_Offset(File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get());
    Offset.resize(Offset.size()+Element_Level_Base, _T(' '));

    //Show Value
    Element[Element_Level_Final].ToShow.Details+=Offset;
    Element[Element_Level_Final].ToShow.Details+=Separator;
    Element[Element_Level_Final].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();
    Element[Element_Level_Final].ToShow.Details+=Offset;
    Element[Element_Level_Final].ToShow.Details+=ToShow;
    Element[Element_Level_Final].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();
    Element[Element_Level_Final].ToShow.Details+=Offset;
    Element[Element_Level_Final].ToShow.Details+=Separator;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Param_Info (const Ztring &Text)
{
    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;
    if (MediaInfoLib::Config.Details_Get()==0 || Element[Element_Level].ToShow.Details.size()>64*1024*1024)
        return;

    //Needed?
    if (MediaInfoLib::Config.Details_Get()<=0.7)
        return;

    //Filling
    Element[Element_Level].ToShow.Details+=_T(" - ")+Text;
}
#endif //MEDIAINFO_MINIMIZESIZE

//***************************************************************************
// Next code planning
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::NextCode_Add (int64u Code)
{
    NextCode[Code]=true;
}

//---------------------------------------------------------------------------
void File__Analyze::NextCode_Clear ()
{
    NextCode.clear();
}

//---------------------------------------------------------------------------
void File__Analyze::NextCode_Test ()
{
    if (NextCode.find(Element_Code)==NextCode.end())
        Trusted_IsNot("Frames are not in the right order");
}

//***************************************************************************
// Element trusting
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Trusted_IsNot (const char* Reason)
{
    if (!Element[Element_Level].UnTrusted)
    {
        Param(Reason, 0);

        //Enough data?
        if (!Element[Element_Level].IsComplete)
        {
            Element_Offset=Element_Size;
            BS->Attach(NULL, 0);
            Element[Element_Level].WaitForMoreData=true;
            return;
        }

        Element[Element_Level].UnTrusted=true;
        Synched=false;
        if (Trusted>0)
            Trusted--;
    }

    if (Trusted==0)
    {
        Clear();
        Finnished();
    }
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Finnished ()
{
    if (NewFinnishMethod)
    {
        IsFinnished=true;
        return;
    }

    File_Offset=File_Size;
    File_GoTo=File_Size;
}

//---------------------------------------------------------------------------
int64u File__Analyze::Element_Code_Get (size_t Level)
{
    return Element[Level].Code;
}

//---------------------------------------------------------------------------
int64u File__Analyze::Element_TotalSize_Get (size_t LevelLess)
{
    return Element[Element_Level-LevelLess].Next-(File_Offset+Buffer_Offset);
}

//---------------------------------------------------------------------------
bool File__Analyze::Element_IsComplete_Get ()
{
    return Element[Element_Level].IsComplete;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_ThisIsAList ()
{
    Element_WantNextLevel=true;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_WaitForMoreData ()
{
    if (File_Offset+Buffer_Size<File_Size)
        Element[Element_Level].WaitForMoreData=true;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_DoNotTrust (const char* Reason)
{
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].IsComplete=true;
    Trusted_IsNot(Reason);
}

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_DoNotShow ()
{
    Element[Element_Level].ToShow.NoShow=true;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_Show ()
{
    Element[Element_Level].ToShow.NoShow=false;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
bool File__Analyze::Element_Show_Get ()
{
    return !Element[Element_Level].ToShow.NoShow;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
#ifndef MEDIAINFO_MINIMIZESIZE
void File__Analyze::Element_Show_Add (const Ztring &ToShow)
{
    if (ToShow.empty())
        return;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=MediaInfoLib::Config.LineSeparator_Get();

    //From Sub
    Element[Element_Level].ToShow.Details+=ToShow;
}
#endif //MEDIAINFO_MINIMIZESIZE

//***************************************************************************
// Status
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::Element_IsOK ()
{
    return !Element[Element_Level].WaitForMoreData && !Element[Element_Level].UnTrusted;
}

//---------------------------------------------------------------------------
bool File__Analyze::Element_IsNotFinnished ()
{
    if (BS->Remain()>0 || Element_Offset+BS->Offset_Get()<Element_Size)
        return true;
    else
        return false;
}

//---------------------------------------------------------------------------
bool File__Analyze::Element_IsWaitingForMoreData ()
{
    return Element[Element_Level].WaitForMoreData;
}

//***************************************************************************
// BookMarks
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::BookMark_Set (size_t Element_Level_ToSet)
{
    Element_Level_ToSet=Element_Level;
    BookMark_Element_Level=Element_Level_ToSet;
    BookMark_Code.resize(BookMark_Element_Level+1);
    BookMark_Next.resize(BookMark_Element_Level+1);
    for (size_t Pos=0; Pos<=BookMark_Element_Level; Pos++)
    {
        BookMark_Code[Pos]=Element[Pos].Code;
        BookMark_Next[Pos]=Element[Pos].Next;
    }
    BookMark_GoTo=File_Offset+Buffer_Offset+Element_Offset;
}

//---------------------------------------------------------------------------
void File__Analyze::BookMark_Get ()
{
    File_GoTo=(int64u)-1;
    if (!BookMark_Needed())
    {
        Finnished();
        return;
    }

    Element_Show();
    while (Element_Level>0)
        Element_End();
    File_Offset=0;
    Buffer_Offset=0;
    while (Element_Level<BookMark_Element_Level)
        Element_Begin("Restarting parsing...", File_Size);

    for (size_t Pos=0; Pos<=Element_Level; Pos++)
    {
        Element[Pos].Code=BookMark_Code[Pos];
        Element[Pos].Next=BookMark_Next[Pos];
    }
    BookMark_Code.clear();
    BookMark_Next.clear();
    if (File_GoTo==(int64u)-1)
        File_GoTo=BookMark_GoTo;
    File_Offset=0; //In case of end of file
}

} //NameSpace

