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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/MediaInfo_Config.h"
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
    Config_Details=0;

    //Configuration
    DataMustAlwaysBeComplete=true;
    MustUseAlternativeParser=false;

    //Buffer
    Buffer=NULL;
    Buffer_Temp=NULL;
    Buffer_Size=0;
    Buffer_Size_Max=0;
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
    if (Config.Details_Get()!=0)
    {
        //ToShow part
        Element[0].ToShow.Name.clear();
        Element[0].ToShow.Info.clear();
        Element[0].ToShow.Details.clear();
        Element[0].ToShow.NoShow=false;
    }
    Element_Level_Base=0;
    Element_Level=0;

    //BitStream
    BS=new BitStream;
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
int File__Analyze::Open_File (const Ztring &File_Name_)
{
    //Preparing
    Clear();
    File_Name=File_Name_;

    //Analyzing
    Read_File();

    //Finalizing
    if (Count_Get(Stream_General)>0)
    {
        Finalize();
        return 1;
    }
    else
        return 0;
}

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Init (int64u File_Size_, int64u File_Offset_)
{
    //Preparing
    File_Size=File_Size_;
    File_Offset=File_Offset_;
    Element[0].Next=File_Size;

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

    //Format
    if (!Buffer_Init_Done)
    {
        Read_Buffer_Init();
        Buffer_Init_Done=true;
    }
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
    Sub->Open_Buffer_Init(File_Size_, File_Offset_);
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

    if (Buffer_Size>0) //There is buffered data from before
    {
        //Allocating new buffer if needed
        if (Buffer_Size+ToAdd_Size>Buffer_Size_Max)
        {
            int8u* Old=Buffer_Temp;
            size_t Buffer_Size_Max_ToAdd=ToAdd_Size>32768?ToAdd_Size:32768;
            if (Buffer_Size_Max_ToAdd<Buffer_Size_Max) Buffer_Size_Max_ToAdd=Buffer_Size_Max;
            Buffer_Size_Max+=Buffer_Size_Max_ToAdd;
            Buffer_Temp=new int8u[Buffer_Size_Max];
            std::memcpy(Buffer_Temp, Old, Buffer_Size);
            delete[] Old; //Old=NULL;
        }

        //Copying buffer
        std::memcpy(Buffer_Temp+Buffer_Size, ToAdd, ToAdd_Size);
        Buffer_Size+=ToAdd_Size;

        //Buffer
        Buffer=Buffer_Temp;
    }
    else
    {
        Buffer=ToAdd;
        Buffer_Size=ToAdd_Size;
    }

    //Preparing
    Buffer_Offset=0;
    Trusted=Buffer_Size>2*8*1024?Buffer_Size/8/1024:2; //Never less than 2 acceptable errors

    //Parsing
    if (Buffer_Size>=Buffer_MinimumSize || File_Offset+Buffer_Size==File_Size) //Parsing only if we have enough buffer
        Open_Buffer_Continue_Loop();

    //Finnished?
    if (File_GoTo==File_Size || File_Offset+Buffer_Offset>=File_Size)
    {
        if (!BookMark_Code.empty())
            BookMark_Get();

        if (File_GoTo==File_Size || File_Offset+Buffer_Offset>=File_Size)
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
    if (General.empty() && !File_Name.empty() && File_Offset>File_MaximumOffset)
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
        if (File_GoTo>=File_Size)
            File_GoTo=File_Size;
        Buffer_Clear();
        return;
    }
    if (Buffer_Offset>=Buffer_Size)
    {
        if (Buffer_Offset>Buffer_Size)
            File_GoTo=File_Offset+Buffer_Offset;
        File_Offset+=Buffer_Offset;
        Buffer_Clear();
        return;
    }

    //Buffer handling
    if (Buffer_Offset!=Buffer_Size) //all is not used
    {
        if (Buffer_Temp==NULL) //If there was no copy
        {
            size_t Buffer_Size_Max_ToAdd=ToAdd_Size-Buffer_Offset>32768?ToAdd_Size-Buffer_Offset:32768;
            if (Buffer_Size_Max_ToAdd<Buffer_Size_Max) Buffer_Size_Max_ToAdd=Buffer_Size_Max;
            Buffer_Size_Max=Buffer_Size_Max_ToAdd;
            Buffer_Temp=new int8u[Buffer_Size_Max];
            std::memcpy(Buffer_Temp, ToAdd+Buffer_Offset, ToAdd_Size-Buffer_Offset);
        }
        else
            std::memmove(Buffer_Temp, Buffer_Temp+Buffer_Offset, Buffer_Size-Buffer_Offset);
    }
    else if (Buffer_Temp!=NULL)
    {
        //Buffer_Temp is no more needed
        delete[] Buffer_Temp; Buffer_Temp=NULL;
    }
    
    //Reserving unused data
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
    Sub->Element_Level_Base=Element_Level_Base+Element_Level;

    //{File F; F.Open(Ztring(_T("d:\\direct"))+Ztring::ToZtring((size_t)this, 16), File::Access_Write_Append); F.Write(ToAdd, ToAdd_Size);}

    //Adaptating File_Offset
    if (Sub!=this && Sub->Buffer_Size<=Sub->File_Offset)
        Sub->File_Offset-=Sub->Buffer_Size;

    //Parsing
    Sub->Open_Buffer_Continue(ToAdd, ToAdd_Size);

    //Details handling
    if (!Sub->Element[0].ToShow.Details.empty())
    {
        //Line separator
        if (!Element[Element_Level].ToShow.Details.empty())
            Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

        //From Sub
        Element[Element_Level].ToShow.Details+=Sub->Element[0].ToShow.Details;
        Sub->Element[0].ToShow.Details.clear();
    }
    else
        Element[Element_Level].ToShow.NoShow=true; //We don't want to show this item because there is no info in it
}

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Continue_Loop ()
{
    //Save for speed improvement
    Config_Details=Config.Details_Get();

    //Parsing specific
    Read_Buffer_Continue();
    if (File_GoTo!=(int64u)-1)
        return; //Finnished

    //Header
    if (MustParseTheHeaderFile && !FileHeader_Manage())
        return; //Wait for more data
    if (File_GoTo!=(int64u)-1)
        return; //Finnished

    //Parsing;
    while (Buffer_Parse());

    //Jumping to the end of the file if needed
    if (!EOF_AlreadyDetected)
    {
        Element[Element_Level].WaitForMoreData=false;
        Detect_EOF();
        if (File_GoTo!=(int64u)-1 || File_Offset==(int64u)-1)
        {
            EOF_AlreadyDetected=true;
            return;
        }
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Open_Buffer_Finalize ()
{
    //File with unknown size (stream...), finnishing
    if (File_Size==(int64u)-1)
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
    Finnished();
    Details=Element[0].ToShow.Details;

    //Buffer
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
void File__Analyze::Read_Buffer_Init ()
{
}

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Continue ()
{
}

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Unsynched()
{
    Synched=false;
    File_GoTo=(int64u)-1;
    Buffer_Clear();
}

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Finalize ()
{
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
    delete[] Buffer_Temp; Buffer_Temp=NULL;
    Buffer_Size=0;
    Buffer_Size_Max=0;
    Buffer_Offset=0;
    Buffer_Offset_Temp=0;
    Buffer_MinimumSize=0;
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
        Element[Element_Level].ToShow.Details.clear();
        return false;
    }

    //Positionning
    Buffer_Offset+=Element_Offset;

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
    if (Buffer_Offset>Buffer_Size)
        return false;

    //From the parser
    Element[Element_Level].IsComplete=true;
    if (!Header_Begin())
        return false; //Wait for more data
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
    if (Element[Element_Level-1].ToShow.Name.empty())
        Element[Element_Level-1].ToShow.Name=_T("Unknown");
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].UnTrusted=false;
    Element[Element_Level].IsComplete=true;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element_Offset;
        Element[Element_Level].ToShow.Header_Size=0;
        Element[Element_Level-1].ToShow.Header_Size=Header_Size;
    }

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
        Element_Size=Buffer_Size-Buffer_Offset;
        Element[Element_Level-1].IsComplete=false;
    }

    Element_End(); //Header
    return true;
}

//---------------------------------------------------------------------------
void File__Analyze::Header_Fill_Code(int64u Code, const Ztring &Name)
{
    //Filling
    Element[Element_Level-1].Code=Code;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level-1].ToShow.Name=Name;
    }
}

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
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level-1].ToShow.Pos=File_Offset+Buffer_Offset;
        Element[Element_Level-1].ToShow.Size=Element[Element_Level-1].Next-(File_Offset+Buffer_Offset);
    }
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
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next element
    if (!Element_WantNextLevel)
    {
        if (Element[Element_Level].Next<=File_Offset+Buffer_Size)
            Buffer_Offset=(size_t)(Element[Element_Level].Next-File_Offset);
        else
        {
            File_GoTo=Element[Element_Level].Next;
            return false;
        }
    }
    else
        Buffer_Offset+=Element_Offset; //This is a sub-level
    Header_Size=0;
    Element_Offset=0;
    Element_Size=0;
    if (Buffer_Offset>Buffer_Size && File_Offset!=File_Size)
        File_GoTo=File_Offset+Buffer_Offset; //Preparing to go far

    if (Element_Level>0)
        Element[Element_Level-1].ToShow.NoShow=Element[Element_Level].ToShow.NoShow; //If data must not be shown, we hide the header too
    else
        Element[0].ToShow.NoShow=false; //This should never happen, but in case of
    Element_End(); //Element
    if (Element_WantNextLevel)
        Element_Level++;
    Element[Element_Level].UnTrusted=false;

    //Jumping to the end of the file if needed
    if (!EOF_AlreadyDetected && File_GoTo==(int64u)-1)
    {
        Element[Element_Level].WaitForMoreData=false;
        Detect_EOF();
        if (File_GoTo!=(int64u)-1 || File_Offset==(int64u)-1)
        {
            EOF_AlreadyDetected=true;
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
void File__Analyze::Data_Info (const Ztring &Parameter)
{
    size_t Element_Level_Temp=Element_Level;
    Element_Level=Data_Level;
    Element_Info(Parameter);
    Element_Level=Element_Level_Temp;
}

//---------------------------------------------------------------------------
void File__Analyze::Data_GoTo (int64u GoTo, const char* ParserName)
{
    Element_Show();
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
    Element[Element_Level].ToShow.Pos=File_Offset+Buffer_Offset+Element_Offset+BS->OffsetBeforeLastCall_Get(); //TODO: change this, used in Element_End()
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Offset+BS->OffsetBeforeLastCall_Get());
        Element[Element_Level].ToShow.Header_Size=0;
        Element[Element_Level].ToShow.Name.clear();
        Element[Element_Level].ToShow.Info.clear();
        Element[Element_Level].ToShow.Details.clear();
        Element[Element_Level].ToShow.NoShow=false;
    }
}

//---------------------------------------------------------------------------
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
    if (Config.Details_Get()!=0)
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

//---------------------------------------------------------------------------
void File__Analyze::Element_Name(const Ztring &Name)
{
    //ToShow
    if (Config.Details_Get()!=0)
    {
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
        else
            Element[Element_Level].ToShow.Name=_T("(Empty)");
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Info(const Ztring &Parameter)
{
    //Coherancy
    if (Config.Details_Get()==0 || Element[Element_Level].ToShow.Details.size()>64*1024*1024)
        return;

    //Needed?
    if (Config.Details_Get()<=0.7)
        return;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Ztring Parameter2(Parameter);
        Parameter2.FindAndReplace(_T("\r\n"), _T(" / "));
        Parameter2.FindAndReplace(_T("\r"), _T(" / "));
        Parameter2.FindAndReplace(_T("\n"), _T(" / "));
        Element[Element_Level].ToShow.Info+=Ztring(_T(" - "))+Parameter2;
    }
}

//---------------------------------------------------------------------------
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
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-Element[Element_Level].ToShow.Pos;
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
    }

    Element_End_Common_Flush();
}

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
    //Size if not filled
    if (File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get()<Element[Element_Level].Next)
        Element[Element_Level].ToShow.Size=File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get()-Element[Element_Level].ToShow.Pos;

    //Level
    if (Element_Level==0)
        return;

    //Element level
    Element_Level--;

    //Element
    Element[Element_Level].UnTrusted=Element[Element_Level+1].UnTrusted;
    Element[Element_Level].WaitForMoreData=Element[Element_Level+1].WaitForMoreData;
    Element[Element_Level].ToShow.NoShow=Element[Element_Level+1].ToShow.NoShow;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        if (!Element[Element_Level+1].WaitForMoreData && (Element[Element_Level+1].IsComplete || !Element[Element_Level+1].UnTrusted) && !Element[Element_Level+1].ToShow.NoShow)// && Config.Details_Get()!=0 && Element[Element_Level].ToShow.Details.size()<=64*1024*1024)
        {
            //Element
            if (!Element[Element_Level+1].ToShow.Name.empty())
            {
                if (!Element[Element_Level].ToShow.Details.empty())
                    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
                Element[Element_Level].ToShow.Details+=Element_End_Common_Flush_Build();
                Element[Element_Level+1].ToShow.Name.clear();
            }

            //Info
            if (!Element[Element_Level+1].ToShow.Details.empty())
            {
                if (!Element[Element_Level].ToShow.Details.empty())
                    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
                Element[Element_Level].ToShow.Details+=Element[Element_Level+1].ToShow.Details;
                Element[Element_Level+1].ToShow.Details.clear();
            }
        }
    }
}

//---------------------------------------------------------------------------
Ztring File__Analyze::Element_End_Common_Flush_Build()
{
    Ztring ToReturn;

    //Show Offset
    if (Config.Details_Get()>0.7)
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
    if (Config.Details_Get()>0.3)
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

//---------------------------------------------------------------------------
void File__Analyze::Element_Prepare (int64u Size)
{
    Element_Offset=0;
    Element_Size=Size;
    Element[Element_Level].ToShow.Size=Size;
}
//***************************************************************************
// Param
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Param(const Ztring& Parameter, const Ztring& Value)
{
    if (Config.Details_Get()==0)
        return;

    //Position
    int64u Pos=Element_Offset+BS->OffsetBeforeLastCall_Get();

    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;

    const size_t Padding_Value=40;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

    //Show Offset
    if (Config.Details_Get()>0.7)
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

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Info(const Ztring& Value)
{
    if (Config.Details_Get()==0)
        return;

    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

    //Preparing
    Ztring ToShow; ToShow.resize(Element_Level, _T(' '));
    ToShow+=_T("---   ");
    ToShow+=Value;
    ToShow+=_T("   ---");
    Ztring Separator; Separator.resize(Element_Level, _T(' '));
    Separator.resize(ToShow.size(), _T('-'));

    //Show Offset
    Ztring Offset;
    if (Config.Details_Get()>0.7)
        Offset=Log_Offset(File_Offset+Buffer_Offset+Element_Offset+BS->Offset_Get());
    Offset.resize(Offset.size()+Element_Level_Base, _T(' '));

    //Show Value
    Element[Element_Level].ToShow.Details+=Offset;
    Element[Element_Level].ToShow.Details+=Separator;
    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
    Element[Element_Level].ToShow.Details+=Offset;
    Element[Element_Level].ToShow.Details+=ToShow;
    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
    Element[Element_Level].ToShow.Details+=Offset;
    Element[Element_Level].ToShow.Details+=Separator;
}

//---------------------------------------------------------------------------
void File__Analyze::Param_Info (const Ztring &Text)
{
    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;
    if (Config.Details_Get()==0 || Element[Element_Level].ToShow.Details.size()>64*1024*1024)
        return;

    //Needed?
    if (Config.Details_Get()<=0.7)
        return;

    //Filling
    Element[Element_Level].ToShow.Details+=_T(" - ")+Text;
}

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
            Element_Offset=(size_t)Element_Size;
            BS->Attach(NULL, 0);
            Element[Element_Level].WaitForMoreData=true;
            return;
        }

        Element[Element_Level].UnTrusted=true;
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
void File__Analyze::Element_DoNotShow ()
{
    Element[Element_Level].ToShow.NoShow=true;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Show ()
{
    Element[Element_Level].ToShow.NoShow=false;
}

//---------------------------------------------------------------------------
bool File__Analyze::Element_Show_Get ()
{
    return !Element[Element_Level].ToShow.NoShow;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Show_Add (const Ztring &ToShow)
{
    if (ToShow.empty())
        return;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

    //From Sub
    Element[Element_Level].ToShow.Details+=ToShow;
}

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
    if (Element_Level_ToSet==(size_t)-1 && Element_Level>0)
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
void File__Analyze::BookMark_Get (size_t Element_Level_ToGet)
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

