// File__Tags - Info for all kind of framed tags tagged files
// Copyright (C) 2005-2008 Jerome Martinez, Zen@MediaArea.net
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
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File__Tags.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// External Helper
//***************************************************************************

File__Tags_Helper::File__Tags_Helper()
{
    Base=NULL;

    //Out
    File_BeginTagSize=0;
    File_EndTagSize=0;

    //Offset
    Id3v1_Offset=(int64u)-1;
    Lyrics3_Offset=(int64u)-1;
    Lyrics3v2_Offset=(int64u)-1;
    ApeTag_Offset=(int64u)-1;

    //Size
    Id3v1_Size=0;
    Lyrics3_Size=0;
    Lyrics3v2_Size=0;
    ApeTag_Size=0;
    JumpTo_WantedByParser=(int64u)-1;
    TagSizeIsFinal=false;
    SearchingForEndTags=false;

    //Handlers
    Id3=NULL;
    Id3v2=NULL;
    Lyrics3=NULL;
    Lyrics3v2=NULL;
    ApeTag=NULL;
}

File__Tags_Helper::~File__Tags_Helper()
{
    //Handlers
    delete Id3; //Id3=NULL;
    delete Id3v2; //Id3v2=NULL;
    delete Lyrics3; //Lyrics3=NULL;
    delete Lyrics3v2; //Lyrics3v2=NULL;
    delete ApeTag; //ApeTag=NULL;
}

bool File__Tags_Helper::Read_Buffer_Continue()
{
    if (Base->IsSub)
        return true;

    if (!TagSizeIsFinal && Base->File_Offset+Base->Buffer_Size==Base->File_Size && JumpTo_WantedByParser==(int64u)-1)
        SearchingForEndTags=true; //For small files

    if (!SearchingForEndTags || TagSizeIsFinal)
        return true;

    //Trying to parse tags
    while (!TagSizeIsFinal && DetectBeginOfEndTags_Test());

    //Positionning (if finnished)
    if (TagSizeIsFinal)
    {
        SearchingForEndTags=false;
        if (JumpTo_WantedByParser!=(int64u)-1)
        {
            Base->Data_GoTo(JumpTo_WantedByParser-Id3v1_Size-Lyrics3_Size-Lyrics3v2_Size-ApeTag_Size, "Tags, searching sync");
            JumpTo_WantedByParser=(int64u)-1;
            return false;
        }
    }

    return true;
}

void File__Tags_Helper::Data_GoTo (int64u GoTo, const char* Message)
{
    if (Base->IsSub)
    {
        Base->Data_GoTo(GoTo, Message);
        return;
    }

    //Normal Data_GoTo;

    //Configuring
    JumpTo_WantedByParser=GoTo;
    SearchingForEndTags=true;

    //Trying to parse tags
    while (!TagSizeIsFinal && DetectBeginOfEndTags_Test());

    //If a jump is requested
    if (Base->File_GoTo!=(int64u)-1)
        return;

    //Positionning (if finnished)
    if (TagSizeIsFinal)
    {
        Base->Data_GoTo(JumpTo_WantedByParser-Id3v1_Size-Lyrics3_Size-Lyrics3v2_Size-ApeTag_Size, Message);
        SearchingForEndTags=false;
    }
    else
        Base->Data_GoTo(GoTo, Message);
}

bool File__Tags_Helper::DetectBeginOfEndTags_Test()
{
        TagSizeIsFinal=true;

        //Id3v1
        if (Id3v1_Size==0 && File_EndTagSize==0 && Base->File_Size>=128) //Only one, at the end, larger than 128 bytes
        {
            if (Base->File_Offset>Base->File_Size-128) //Must be at least at the end less 128 bytes
            {
                Base->Data_GoTo(Base->File_Size-128-32, "Tags"); //32 to be able to quickly see another tag system
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-125) //Must be at least at the end less 128 bytes plus 3 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-128)
                    Base->Data_GoTo(Base->File_Size-128-32, "Tags"); //32 to be able to quickly see another tag system
                TagSizeIsFinal=false;
                return false;
            }

            if (CC3(Base->Buffer+((size_t)((Base->File_Size-128)-Base->File_Offset)))==CC3("TAG"))
            {
                Id3v1_Size=128;
                File_EndTagSize+=128;
                Id3v1_Offset=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
            }
        }

        //Lyrics3
        if (Lyrics3_Size==0 && Base->File_Size>=File_EndTagSize+9) //Only one, larger than File_EndTagSize+15 bytes
        {
            if (Base->File_Offset>Base->File_Size-File_EndTagSize-9) //Must be at the end less 15 bytes
            {
                Base->File_GoTo=Base->File_Size-File_EndTagSize-9;
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize+9 bytes plus 9 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-File_EndTagSize)
                    Base->File_GoTo=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
                return false;
            }

            if (CC8(Base->Buffer+((size_t)((Base->File_Size-File_EndTagSize-9)-Base->File_Offset)))==CC8("LYRICSEN"))
            {
                //Must find the beginning, 5100 bytes before
                if (Base->File_Offset>Base->File_Size-File_EndTagSize-5100) //Must be at the end less 15 bytes
                {
                    Base->File_GoTo=Base->File_Size-File_EndTagSize-5100;
                    TagSizeIsFinal=false;
                    return false;
                }

                if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize
                {
                    if (Base->File_Offset!=Base->File_Size-File_EndTagSize)
                        Base->File_GoTo=Base->File_Size-File_EndTagSize;
                    TagSizeIsFinal=false;
                    return false;
                }

                //Searching for "LYRICSBEGIN"
                std::string Buf((const char*)(Base->Buffer+Base->Buffer_Offset), Base->Buffer_Size-Base->Buffer_Offset);
                size_t Pos=Buf.find("LYRICSBEGIN");
                if (Pos!=std::string::npos)
                {
                    Lyrics3_Size=Base->File_Size-File_EndTagSize-(Base->File_Offset+Base->Buffer_Offset+Pos);
                    File_EndTagSize+=Lyrics3_Size;
                    Lyrics3_Offset=Base->File_Size-File_EndTagSize;
                    TagSizeIsFinal=false;
                }
            }
        }

        //Lyrics3v2
        if (Lyrics3v2_Size==0 && Base->File_Size>=File_EndTagSize+15) //Only one, larger than File_EndTagSize+15 bytes
        {
            if (Base->File_Offset>Base->File_Size-File_EndTagSize-15) //Must be at the end less 15 bytes
            {
                Base->File_GoTo=Base->File_Size-File_EndTagSize-15;
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize
            {
                if (Base->File_Offset!=Base->File_Size-File_EndTagSize)
                    Base->File_GoTo=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
                return false;
            }

            if (CC8(Base->Buffer+((size_t)((Base->File_Size-File_EndTagSize-15+6)-Base->File_Offset)))==CC8("LYRICS20"))
            {
                Lyrics3v2_Size=Ztring((const char*)(Base->Buffer+((size_t)(Base->Buffer_Size-File_EndTagSize-15))), 6).To_int32u()+15;
                File_EndTagSize+=Lyrics3v2_Size;
                Lyrics3v2_Offset=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
            }
        }

        //ApeTag
        if (ApeTag_Size==0 && Base->File_Size>=File_EndTagSize+32) //Only one, larger than File_EndTagSize+32 bytes
        {
            if (Base->File_Offset>Base->File_Size-File_EndTagSize-32) //Must be at the end less 32 bytes
            {
                Base->File_GoTo=Base->File_Size-File_EndTagSize-32;
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize+15 bytes plus 15 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-File_EndTagSize-32)
                    Base->File_GoTo=Base->File_Size-File_EndTagSize-32;
                TagSizeIsFinal=false;
                return false;
            }

            if (CC8(Base->Buffer+((size_t)((Base->File_Size-File_EndTagSize-32)-Base->File_Offset)))==CC8("APETAGEX"))
            {
                ApeTag_Size=LittleEndian2int32u(Base->Buffer+((size_t)(Base->Buffer_Size-File_EndTagSize-32+12)));
                if (LittleEndian2int32u(Base->Buffer+((size_t)(Base->Buffer_Size-File_EndTagSize-32+8)))==2000)
                    ApeTag_Size+=32;
                File_EndTagSize+=ApeTag_Size;
                TagSizeIsFinal=false;
                ApeTag_Offset=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
            }
        }

        //Finnished
        return true;
}

bool File__Tags_Helper::Header_Begin()
{
    //Id3v2
    if (Base->Buffer_Offset+3>Base->Buffer_Size)
        return false; //Not enough buffer
    if (CC3(Base->Buffer+Base->Buffer_Offset)==CC3("ID3"))
    {
        if (Base->Buffer_Offset+10>Base->Buffer_Size)
            return false; //Not enough buffer

        //Retrieving size
        int32u Id3v2_Size=SynchSafeInt(Base->Buffer+Base->Buffer_Offset+6);
        if (Base->Count_Get(Stream_General)==0 && Base->Buffer_Offset+Id3v2_Size>Base->Buffer_MaximumSize && Id3v2_Size<16*1024*1024) //Fixing a limit of the Id3v2 size
            Base->Buffer_MaximumSize=Base->Buffer_Offset+Id3v2_Size;
        if (Base->Buffer_Offset+Id3v2_Size>Base->Buffer_Size)
            return false; //Not enough buffer

        Base->Element_Begin("Id3v2");
        #if defined(MEDIAINFO_ID3V2_YES)
                Id3v2=new File_Id3v2();
                Base->Open_Buffer_Init(Id3v2, Base->File_Size, Base->File_Offset+Base->Buffer_Offset);
                Base->Open_Buffer_Continue(Id3v2, Base->Buffer+Base->Buffer_Offset, 10+Id3v2_Size);
        #else //MEDIAINFO_ID3V2_YES
                Base->Skip_C3(                                  "Identifier");
                Base->Skip_XX(10+Id3v2_Size-3,                  "Data");
        #endif //MEDIAINFO_ID3V2_YES
        Base->Buffer_Offset+=10+Id3v2_Size; //Skiping Id3v2
        File_BeginTagSize+=10+Id3v2_Size;
        Base->Element_End();
    }

    //All other tags are at the end
    if (Base->File_Offset+Base->Buffer_Offset<Base->File_Size-File_EndTagSize)
        return true;

    //ApeTag
    if (Base->File_Offset+Base->Buffer_Offset==ApeTag_Offset)
    {
        if (Base->Buffer_Offset+ApeTag_Size>Base->Buffer_Size)
            return false; //Not enough buffer
        Base->Element_Begin("ApeTag");
            Base->Element_Prepare(ApeTag_Size);
            #if defined(MEDIAINFO_APETAG_YES)
                ApeTag=new File_ApeTag();
                Base->Open_Buffer_Init(ApeTag, Base->File_Size, Base->File_Offset+Base->Buffer_Offset);
                Base->Open_Buffer_Continue(ApeTag, Base->Buffer+Base->Buffer_Offset, (size_t)ApeTag_Size);
            #else //MEDIAINFO_APETAG_YES
                Base->Skip_C8(                                  "Identifier");
                Base->Skip_XX(ApeTag_Size-8,                    "Data");
            #endif //MEDIAINFO_APETAG_YES
            Base->Buffer_Offset+=(size_t)ApeTag_Size; //Skiping ApeTag
        Base->Element_End();
    }

    //Lyrics3
    if (Base->File_Offset+Base->Buffer_Offset==Lyrics3_Offset)
    {
        if (Base->Buffer_Offset+Lyrics3_Size>Base->Buffer_Size)
            return false; //Not enough buffer
        Base->Element_Begin("Lyrics3");
            Base->Element_Prepare(Lyrics3_Size);
            #if defined(MEDIAINFO_LYRICS3_YES)
                Lyrics3=new File_Lyrics3();
                Base->Open_Buffer_Init(Lyrics3, Base->File_Size, Base->File_Offset+Base->Buffer_Offset);
                Base->Open_Buffer_Continue(Lyrics3, Base->Buffer+Base->Buffer_Offset, (size_t)Lyrics3_Size);
            #else //MEDIAINFO_LYRICS3_YES
                Base->Skip_XX(11,                                "Identifier");
                Base->Skip_XX(Lyrics3_Size-11,                   "Data");
            #endif //MEDIAINFO_LYRICS3_YES
            Base->Buffer_Offset+=(size_t)Lyrics3_Size; //Skiping Lyrics
        Base->Element_End();
    }

    //Lyrics3v2
    if (Base->File_Offset+Base->Buffer_Offset==Lyrics3v2_Offset)
    {
        if (Base->Buffer_Offset+Lyrics3_Size>Base->Buffer_Size)
            return false; //Not enough buffer
        Base->Element_Begin("Lyrics3v2");
            Base->Element_Prepare(Lyrics3v2_Size);
            #if defined(MEDIAINFO_LYRICS3V2_YES)
                Lyrics3v2=new File_Lyrics3v2();
                Base->Open_Buffer_Init(Lyrics3v2, Base->File_Size, Base->File_Offset+Base->Buffer_Offset);
                Base->Open_Buffer_Continue(Lyrics3v2, Base->Buffer+Base->Buffer_Offset, (size_t)Lyrics3v2_Size);
            #else //MEDIAINFO_LYRICS3V2_YES
                Base->Skip_XX(11,                               "Identifier");
                Base->Skip_XX(Lyrics3v2_Size-11,                "Data");
            #endif //MEDIAINFO_LYRICS3V2_YES
            Base->Buffer_Offset+=(size_t)Lyrics3v2_Size; //Skiping Lyrics3v2
        Base->Element_End();
    }

    //ApeTag again (order)
    if (Base->File_Offset+Base->Buffer_Offset==ApeTag_Offset)
    {
        if (Base->Buffer_Offset+ApeTag_Size>Base->Buffer_Size)
            return false; //Not enough buffer
        Base->Element_Begin("ApeTag");
            Base->Element_Prepare(ApeTag_Size);
            #if defined(MEDIAINFO_APETAG_YES)
                ApeTag=new File_ApeTag();
                Base->Open_Buffer_Init(ApeTag, Base->File_Size, Base->File_Offset+Base->Buffer_Offset);
                Base->Open_Buffer_Continue(ApeTag, Base->Buffer+Base->Buffer_Offset, (size_t)ApeTag_Size);
            #else //MEDIAINFO_APETAG_YES
                Base->Skip_C8(                                  "Identifier");
                Base->Skip_XX(ApeTag_Size-8,                    "Data");
            #endif //MEDIAINFO_APETAG_YES
            Base->Buffer_Offset+=(size_t)ApeTag_Size; //Skiping ApeTag
        Base->Element_End();
    }

    //Id3v1
    if (Base->File_Offset+Base->Buffer_Offset==Id3v1_Offset)
    {
        if (Base->Buffer_Offset+Id3v1_Size>Base->Buffer_Size)
            return false; //Not enough buffer
        Base->Element_Begin("Id3v1");
            Base->Element_Prepare(128);
            #if defined(MEDIAINFO_ID3_YES)
                Id3=new File_Id3();
                Base->Open_Buffer_Init(Id3, Base->File_Size, Base->File_Offset+Base->Buffer_Offset);
                Base->Open_Buffer_Continue(Id3, Base->Buffer+Base->Buffer_Offset, 128);
            #else //MEDIAINFO_ID3_YES
                Base->Skip_C3(                                  "Identifier");
                Base->Skip_XX(128-3,                            "Data");
            #endif //MEDIAINFO_ID3_YES
            Base->Buffer_Offset+=128; //Skiping Id3
        Base->Element_End();
    }

    return true; //Finnished! Should be end of file
}

void File__Tags_Helper::Read_Buffer_Finalize()
{
    #if defined(MEDIAINFO_APETAG_YES)
    if (ApeTag!=NULL)
    {
        ApeTag->Read_Buffer_Finalize();
        if (ApeTag->Count_Get(Stream_General)>0)
        {
            Base->Merge(*ApeTag, Stream_General, 0, 0, false);
            Base->Merge(*ApeTag, Stream_Audio  , 0, 0, false);
        }
        delete ApeTag; ApeTag=NULL;
    }
    #endif //MEDIAINFO_APETAG_YES
    #if defined(MEDIAINFO_LYRICS3V2_YES)
    if (Lyrics3v2!=NULL)
    {
        Lyrics3v2->Read_Buffer_Finalize();
        if (Lyrics3v2->Count_Get(Stream_General)>0)
        {
            Base->Merge(*Lyrics3v2, Stream_General, 0, 0, false);
            Base->Merge(*Lyrics3v2, Stream_Audio  , 0, 0, false);
        }
        delete Lyrics3v2; Lyrics3v2=NULL;
    }
    #endif //MEDIAINFO_LYRICS3V2_YES
    #if defined(MEDIAINFO_LYRICS3_YES)
    if (Lyrics3!=NULL)
    {
        Lyrics3->Read_Buffer_Finalize();
        if (Lyrics3->Count_Get(Stream_General)>0)
        {
            Base->Merge(*Lyrics3, Stream_General, 0, 0, false);
            Base->Merge(*Lyrics3, Stream_Audio  , 0, 0, false);
        }
        delete Lyrics3; Lyrics3=NULL;
    }
    #endif //MEDIAINFO_LYRICS3_YES
    #if defined(MEDIAINFO_ID3V2_YES)
    if (Id3v2!=NULL)
    {
        Id3v2->Read_Buffer_Finalize();
        if (Id3v2->Count_Get(Stream_General)>0)
        {
            Base->Merge(*Id3v2, Stream_General, 0, 0, false);
            Base->Merge(*Id3v2, Stream_Audio  , 0, 0, false);
        }
        delete Id3v2; Id3v2=NULL;
    }
    #endif //MEDIAINFO_ID3V2_YES
    #if defined(MEDIAINFO_ID3_YES)
    if (Id3!=NULL)
    {
        Id3->Read_Buffer_Finalize();
        if (Id3->Count_Get(Stream_General)>0)
        {
            Base->Merge(*Id3, Stream_General, 0, 0, false);
            Base->Merge(*Id3, Stream_Audio  , 0, 0, false);
        }
        delete Id3; Id3=NULL;
    }
    #endif //MEDIAINFO_ID3_YES
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

