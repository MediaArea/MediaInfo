// File__Tags - Info for all kind of framed tags tagged files
// Copyright (C) 2005-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_APETAG_YES)
    #include "MediaInfo/Tag/File_ApeTag.h"
#endif //MEDIAINFO_APETAG_YES
#if defined(MEDIAINFO_ID3_YES)
    #include "MediaInfo/Tag/File_Id3.h"
#endif //MEDIAINFO_ID3_YES
#if defined(MEDIAINFO_ID3V2_YES)
    #include "MediaInfo/Tag/File_Id3v2.h"
#endif //MEDIAINFO_ID3V2_YES
#if defined(MEDIAINFO_LYRICS3_YES)
    #include "MediaInfo/Tag/File_Lyrics3.h"
#endif //MEDIAINFO_LYRICS3_YES
#if defined(MEDIAINFO_LYRICS3V2_YES)
    #include "MediaInfo/Tag/File_Lyrics3v2.h"
#endif //MEDIAINFO_LYRICS3V2_YES
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

File__Tags_Helper::File__Tags_Helper()
{
    //In
    Base=NULL;

    //Out
    TagsSize=0;
    File_BeginTagSize=0;
    File_EndTagSize=0;

    //Temp
    Parser=NULL;
    Id3v1_Offset=(int64u)-1;
    Lyrics3_Offset=(int64u)-1;
    Lyrics3v2_Offset=(int64u)-1;
    ApeTag_Offset=(int64u)-1;
    JumpTo_WantedByParser=(int64u)-1;
    Id3v1_Size=0;
    Lyrics3_Size=0;
    Lyrics3v2_Size=0;
    ApeTag_Size=0;
    TagSizeIsFinal=false;
    SearchingForEndTags=false;
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Tags_Helper::Read_Buffer_Continue()
{
    //Tags
    bool Tag_Found;
    if (File__Tags_Helper::Synchronize(Tag_Found) &&  Tag_Found)
        File__Tags_Helper::Synched_Test();
    return true;
}

//---------------------------------------------------------------------------
bool File__Tags_Helper::Synchronize(bool &Tag_Found, size_t Synchro_Offset)
{
    //Buffer size
    if (Base->Buffer_Offset+Synchro_Offset+3>Base->Buffer_Size)
        return false;

    if (Parser)
    {
        Synched_Test();
        if (Parser)
            return false;
    }

    if (SearchingForEndTags)
    {
        Data_GoTo(0, "Tags detected");
        return false;
    }

    //ID
    switch (CC3(Base->Buffer+Base->Buffer_Offset+Synchro_Offset))
    {
        case 0x494433 : //"ID3"
                        if (!Synched_Test()) //Handling begin/intermediate Id3v2
                            return false;
                        return Synchronize(Tag_Found, Synchro_Offset);
        case 0x544147 : //"TAG"
        case 0x4C5952 : //"LYR"
        case 0x415045 : //"APE"
                        if (TagSizeIsFinal && Base->File_Offset+Base->Buffer_Offset==Base->File_Size-File_EndTagSize)
                        {
                            Tag_Found=true;
                            return Synched_Test();
                        }
                        else if (Base->File_Offset+Base->Buffer_Size==Base->File_Size)
                        {
                             while (!TagSizeIsFinal && DetectBeginOfEndTags_Test());
                             Tag_Found=true;
                             return true;
                        }
                        else
                            return false; //Need more data
        default       : if (Base->File_Offset+Base->Buffer_Offset==ApeTag_Offset)
                        {
                            Tag_Found=true;
                            return true;
                        };
    }

    Tag_Found=false;
    return Base->File_GoTo==(int64u)-1;
}

//---------------------------------------------------------------------------
bool File__Tags_Helper::Synched_Test()
{
    while (1)
    {
        if (!Parser)
        {
            //Must have enough buffer for having header
            if (Base->Buffer_Offset+3>Base->Buffer_Size)
                return false;

            //Quick test of synchro
            int32u ID=CC3(Base->Buffer+Base->Buffer_Offset);
                 if (ID==0x494433) //"ID3"
            {
                if (Base->Buffer_Offset+10>Base->Buffer_Size)
                    return false;
                Parser=new File_Id3v2;
                int32u Size=BigEndian2int32u(Base->Buffer+Base->Buffer_Offset+6);
                Parser_Buffer_Size=(((Size>>0)&0x7F)
                                  | ((Size>>1)&0x3F80)
                                  | ((Size>>2)&0x1FC000)
                                  | ((Size>>3)&0x0FE00000))
                                  +10;
                File_BeginTagSize+=Parser_Buffer_Size;
                if (Base->File_Offset_FirstSynched==(int64u)-1)
                    Base->Buffer_TotalBytes_FirstSynched_Max+=Parser_Buffer_Size;
                Base->Element_Begin("Id3v2");
            }
            else if (ID==0x544147) //"TAG"
            {
                Parser=new File_Id3;
                Parser_Buffer_Size=128;
                Base->Element_Begin("Id3");
            }
            else if (Base->File_Offset+Base->Buffer_Offset==Lyrics3_Offset)
            {
                Parser=new File_Lyrics3;
                ((File_Lyrics3*)Parser)->TotalSize=Lyrics3_Size;
                Parser_Buffer_Size=(size_t)Lyrics3_Size;
                Base->Element_Begin("Lyrics3");
            }
            else if (Base->File_Offset+Base->Buffer_Offset==Lyrics3v2_Offset)
            {
                Parser=new File_Lyrics3v2;
                ((File_Lyrics3v2*)Parser)->TotalSize=Lyrics3v2_Size;
                Parser_Buffer_Size=(size_t)Lyrics3v2_Size;
                Base->Element_Begin("Lyrics3v2");
            }
            else if (Base->File_Offset+Base->Buffer_Offset==ApeTag_Offset)
            {
                Parser=new File_ApeTag;
                Parser_Buffer_Size=(size_t)ApeTag_Size;
                Base->Element_Begin("ApeTag");
            }
            else
                break;
            Base->Open_Buffer_Init(Parser);
        }

        //We continue
        if (Parser)
        {
            int64u Size_ToParse=Parser_Buffer_Size<Base->Buffer_Size?Parser_Buffer_Size:Base->Buffer_Size;
            Base->Open_Buffer_Continue(Parser, Base->Buffer+Base->Buffer_Offset, (size_t)Size_ToParse);
            Base->Buffer_Offset+=(size_t)Size_ToParse;
            TagsSize+=Size_ToParse;
            Parser_Buffer_Size-=(size_t)Size_ToParse;
            if (Parser->IsFinished || Parser_Buffer_Size==0)
            {
                if (Parser->Count_Get(Stream_General)>0)
                {
                    Parser->Read_Buffer_Finalize();
                    Base->Merge(*Parser, Stream_General, 0, 0, false);
                    Base->Merge(*Parser, Stream_Audio  , 0, 0, false);
                }
                delete Parser; Parser=NULL;
                if (Parser_Buffer_Size)
                    Base->Skip_XX(Parser_Buffer_Size,           "Data continued");
                Base->Element_Show();
                Base->Element_End();
            }
            else
                break;
        }
    }

    return true;
}

//***************************************************************************
// Streams
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__Tags_Helper::Stream_Prepare(stream_t StreamKind)
{
    if (Base->Count_Get(StreamKind))
        return 1; //Only one is permited
    return Base->Stream_Prepare(StreamKind);
}

//***************************************************************************
// End
//***************************************************************************

//---------------------------------------------------------------------------
void File__Tags_Helper::Data_GoTo (int64u GoTo, const char* Message)
{
    if (Base->IsSub)
    {
        Base->Data_GoTo(GoTo, Message);
        return;
    }

    //Normal Data_GoTo;

    //Configuring
    if (!SearchingForEndTags)
    {
        if (GoTo==(int64u)-1)
            JumpTo_WantedByParser=Base->File_Offset+Base->Buffer_Offset;
        else
            JumpTo_WantedByParser=GoTo;
        SearchingForEndTags=true;
    }

    //Trying to parse tags
    while (!TagSizeIsFinal && DetectBeginOfEndTags_Test());

    //If a jump is requested
    if (Base->File_GoTo!=(int64u)-1)
        return;

    //Positionning (if finnished)
    if (TagSizeIsFinal)
    {
        if (JumpTo_WantedByParser!=(int64u)-1)
        {
            if (JumpTo_WantedByParser<Id3v1_Size+Lyrics3_Size+Lyrics3v2_Size+ApeTag_Size)
                Base->Data_GoTo(JumpTo_WantedByParser, Message);
            else
                Base->Data_GoTo(JumpTo_WantedByParser-Id3v1_Size-Lyrics3_Size-Lyrics3v2_Size-ApeTag_Size, Message);
        }
        SearchingForEndTags=false;
    }
    else
        Base->Data_GoTo(GoTo, Message);
}

//---------------------------------------------------------------------------
void File__Tags_Helper::Detected (int64u BeforeEnd, const char* Message)
{
    if (Base->IsSub)
    {
        Base->Detected(BeforeEnd, Message);
        return;
    }

    if (BeforeEnd<Base->File_Size-Base->File_Offset)
        Data_GoTo(Base->File_Size-BeforeEnd, Message);
    else
        Data_GoTo((int64u)-1, Message); //Don't move
    Base->IsDetected=true;
}

//---------------------------------------------------------------------------
void File__Tags_Helper::Finished (const char* Message)
{
    if (Base->IsDetected)
        Data_GoTo(Base->File_Size, Message);
    else
        Base->Finished(Message);
}

//---------------------------------------------------------------------------
bool File__Tags_Helper::DetectBeginOfEndTags_Test()
{
        TagSizeIsFinal=true;

        //Id3v1
        if (Id3v1_Size==0 && File_EndTagSize==0 && Base->File_Size>=128) //Only one, at the end, larger than 128 bytes
        {
            if (Base->File_Offset>Base->File_Size-128) //Must be at least at the end less 128 bytes
            {
                Base->Data_GoTo(Base->File_Size-128-32, "Tags detection"); //32 to be able to quickly see another tag system
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-125) //Must be at least at the end less 128 bytes plus 3 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-128)
                    Base->Data_GoTo(Base->File_Size-128-32, "Tags detection"); //32 to be able to quickly see another tag system
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

        //Finished
        return true;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

