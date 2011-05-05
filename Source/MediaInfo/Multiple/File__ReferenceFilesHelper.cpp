// File__ReferenceFilesHelper - class for analyzing/demuxing reference files
// Copyright (C) 2011-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
#if defined(MEDIAINFO_REFERENCES_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File__ReferenceFilesHelper.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "ZenLib/Dir.h"
#include "ZenLib/FileName.h"
#include "ZenLib/File.h"
#include "ZenLib/Format/Http/Http_Utils.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File__ReferenceFilesHelper::File__ReferenceFilesHelper(File__Analyze* MI_, MediaInfo_Config_MediaInfo* Config_)
{
    //Temp
    MI=MI_;
    Config=Config_;
    Reference=References.end();
    File_Size_Total=MI->File_Size;
    Init_Done=false;
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File__ReferenceFilesHelper::ParseReferences()
{
    if (!Init_Done)
    {
        Reference=References.begin();
        Init_Done=true;

        #if MEDIAINFO_DEMUX
            if (Config->NextPacket_Get())
            {
                Demux_Interleave=Config->File_Demux_Interleave_Get();
                if (Demux_Interleave)
                    CountOfReferencesToParse=References.size();
            }
        #endif //MEDIAINFO_DEMUX
    }

    while (Reference!=References.end())
    {
        ParseReference();

        #if MEDIAINFO_DEMUX
            if (Demux_Interleave)
            {
                references::iterator Reference_Next=Reference; Reference_Next++;
                if (Reference_Next==References.end() && Config->NextPacket_Get() && CountOfReferencesToParse)
                    Reference=References.begin();
                else
                    Reference=Reference_Next;

                if (Config->Demux_EventWasSent)
                    return;
            }
            else
            {
                if (Config->Demux_EventWasSent)
                    return;

                Reference++;
            }
        #else //MEDIAINFO_DEMUX
            Reference++;
        #endif //MEDIAINFO_DEMUX
    }

    //File size handling
    if (File_Size_Total!=MI->File_Size)
    {
        MI->Fill(Stream_General, 0, General_FileSize, File_Size_Total, 10, true);
        MI->Fill(Stream_General, 0, General_StreamSize, MI->File_Size, 10, true);
    }
}

//---------------------------------------------------------------------------
void File__ReferenceFilesHelper::ParseReference()
{
    if (Reference->MI==NULL && !Reference->FileNames.empty())
    {
        //Configuration
        Reference->MI=new MediaInfo_Internal();
        Reference->MI->Option(_T("File_FileNameFormat"), _T("CSV"));
        if (MediaInfoLib::Config.ParseSpeed_Get()<1.0)
            Reference->MI->Option(_T("File_StopAfterFilled"), _T("1"));
        Reference->MI->Option(_T("File_KeepInfo"), _T("1"));
        #if MEDIAINFO_NEXTPACKET
            if (Config->NextPacket_Get())
                Reference->MI->Option(_T("File_NextPacket"), _T("1"));
        #endif //MEDIAINFO_NEXTPACKET
        #if MEDIAINFO_EVENTS
            if (Config->Event_CallBackFunction_IsSet())
                Reference->MI->Option(_T("File_Event_CallBackFunction"), Config->Event_CallBackFunction_Get());
            Reference->MI->Option(_T("File_SubFile_StreamID_Set"), Reference->StreamID);
        #endif //MEDIAINFO_EVENTS
        #if MEDIAINFO_DEMUX
            if (Config->Demux_Unpacketize_Get())
                Reference->MI->Option(_T("File_Demux_Unpacketize"), _T("1"));
            if (Reference->FrameRate)
                Reference->MI->Option(_T("File_Demux_Rate"), Ztring::ToZtring(Reference->FrameRate, 15));
            else
            {
                //Using the frame rate from the first stream having a frame rate
                for (references::iterator ReferenceFrameRate=References.begin(); ReferenceFrameRate!=References.end(); ReferenceFrameRate++)
                {
                    Reference->MI->Option(_T("File_Demux_Rate"), Ztring::ToZtring(ReferenceFrameRate->FrameRate, 15));
                    break;
                }
            }
        #endif //MEDIAINFO_DEMUX

        //Configuring file name
        ZtringList Names=Reference->FileNames;
        ZtringList AbsoluteNames; AbsoluteNames.Separator_Set(0, ",");
        for (size_t Pos=0; Pos<Names.size(); Pos++)
        {
            if (Names[Pos].find(_T("file:"))==0)
            {
                Names[Pos].erase(0, 5); //Removing "file:", this is the default behaviour and this makes comparison easier
                Names[Pos]=ZenLib::Format::Http::URL_Encoded_Decode(Names[Pos]);
            }
            Ztring AbsoluteName;
            if (Names[Pos].find(_T(':'))!=1 && Names[Pos].find(_T("/"))!=0 && Names[Pos].find(_T("\\\\"))!=0) //If absolute patch
            {
                AbsoluteName=ZenLib::FileName::Path_Get(MI->File_Name);
                if (!AbsoluteName.empty())
                    AbsoluteName+=ZenLib::PathSeparator;
            }
            AbsoluteName+=Names[Pos];
            #ifdef __WINDOWS__
                AbsoluteName.FindAndReplace(_T("/"), _T("\\"), 0, Ztring_Recursive); //Names[Pos] normalization
            #endif //__WINDOWS__
            AbsoluteNames.push_back(AbsoluteName);
        }
        if (!File::Exists(AbsoluteNames[0]))
        {
            //Configuring file name (this time, we try to force URL decode in all cases)
            for (size_t Pos=0; Pos<Names.size(); Pos++)
            {
                Names[Pos]=ZenLib::Format::Http::URL_Encoded_Decode(Names[Pos]);
                Ztring AbsoluteName;
                if (Names[Pos].find(_T(':'))!=1 && Names[Pos].find(_T("/"))!=0 && Names[Pos].find(_T("\\\\"))!=0) //If absolute patch
                {
                    AbsoluteName=ZenLib::FileName::Path_Get(MI->File_Name);
                    if (!AbsoluteName.empty())
                        AbsoluteName+=ZenLib::PathSeparator;
                }
                AbsoluteName+=Names[Pos];
                #ifdef __WINDOWS__
                    AbsoluteName.FindAndReplace(_T("/"), _T("\\"), 0, Ztring_Recursive); //Names[Pos] normalization
                #endif //__WINDOWS__
                AbsoluteNames.push_back(AbsoluteName);
            }
        }

        if (AbsoluteNames[0]==MI->File_Name)
        {
            MI->Fill(Reference->StreamKind, Reference->StreamPos, "Source_Info", "Circular");
            if (!Config->File_KeepInfo_Get())
            {
                #if MEDIAINFO_DEMUX
                    if (CountOfReferencesToParse)
                        CountOfReferencesToParse--;
                #endif //MEDIAINFO_DEMUX
                Reference->StreamKind=Stream_Max;
                Reference->StreamPos=(size_t)-1;
                delete Reference->MI; Reference->MI=NULL;
            }
            Reference->FileNames.clear();
        }
        else
        {
            //Run
            if (!Reference->MI->Open(AbsoluteNames.Read()))
            {
                MI->Fill(Reference->StreamKind, Reference->StreamPos, "Source_Info", "Missing");
                if (!Config->File_KeepInfo_Get())
                {
                    #if MEDIAINFO_DEMUX
                        if (CountOfReferencesToParse)
                            CountOfReferencesToParse--;
                    #endif //MEDIAINFO_DEMUX
                    Reference->StreamKind=Stream_Max;
                    Reference->StreamPos=(size_t)-1;
                    delete Reference->MI; Reference->MI=NULL;
                }
                Reference->FileNames.clear();
            }
            MI->Fill(Reference->StreamKind, Reference->StreamPos, "Source", Reference->FileNames.Read(0));
            Reference->FileNames=AbsoluteNames;
        }
    }

    if (Reference->MI)
    {
        #if MEDIAINFO_NEXTPACKET
            if (Config->Event_CallBackFunction_IsSet() && !Reference->Status[File__Analyze::IsFinished])
            {
                #if MEDIAINFO_DEMUX
					while ((Reference->Status=Reference->MI->Open_NextPacket())[8])
					{
							if (Config->Event_CallBackFunction_IsSet())
							{
								Config->Demux_EventWasSent=true;
								return;
							}
					}
                if (CountOfReferencesToParse)
                    CountOfReferencesToParse--;
				#endif //MEDIAINFO_DEMUX
            }
        #endif //MEDIAINFO_NEXTPACKET
        ParseReference_Finalize();
        if (!Config->File_KeepInfo_Get())
        {
            Reference->StreamKind=Stream_Max;
            Reference->StreamPos=(size_t)-1;
            delete Reference->MI; Reference->MI=NULL;
        }
    }
}

//---------------------------------------------------------------------------
void File__ReferenceFilesHelper::ParseReference_Finalize ()
{
    //Creating stream
    if (Reference->StreamPos==(size_t)-1)
    {
        if (Reference->StreamKind==Stream_Max)
        {
            for (size_t StreamKind=Stream_General+1; StreamKind<Stream_Max; StreamKind++)
                if (Reference->MI->Count_Get((stream_t)StreamKind))
                {
                    Reference->StreamKind=(stream_t)StreamKind;
                    break;
                }
                
                if (Reference->StreamKind==(size_t)-1)
                    return; //Tehre is a problem
        }

        Reference->StreamPos=MI->Stream_Prepare(Reference->StreamKind);
    }

    //Preparing
    stream_t StreamKind_Last=Reference->StreamKind;
    size_t StreamPos_Last=Reference->StreamPos;

    //Image as video
    if (Reference->FileNames.size()>1 && Reference->MI->Count_Get(Stream_Image))
    {
        Reference->MI->Info->Stream_Prepare(Stream_Video);
        for (size_t Pos=0; Pos<Reference->MI->Count_Get(Stream_Image, 0); Pos++)
            Reference->MI->Info->Fill(Stream_Video, 0, Reference->MI->Get(Stream_Image, 0, Pos, Info_Name).To_UTF8().c_str(), Reference->MI->Get(Stream_Image, 0, Pos), true);
        Reference->MI->Info->Stream_Erase(Stream_Image, 0);

        Reference->MI->Info->Fill(Stream_Video, 0, Video_FrameRate, Reference->FrameRate);
        Reference->MI->Info->Fill(Stream_Video, 0, Video_FrameCount, Reference->FileNames.size());

        //Stream size (only for files not of FTP/HTTP)
        int64u StreamSize=Reference->MI->Get(Stream_General, 0, General_FileSize).To_int64u();
        for (size_t Pos=1; Pos<Reference->FileNames.size(); Pos++)
            StreamSize+=File::Size_Get(Reference->FileNames[Pos]);
        Reference->MI->Info->Fill(Stream_General, 0, General_FileSize, StreamSize, 10, true);
        Reference->MI->Info->Fill(Stream_Video, 0, Video_StreamSize, StreamSize, 10, true);
    }

    //Hacks - Before
    Ztring CodecID=MI->Retrieve(StreamKind_Last, StreamPos_Last, MI->Fill_Parameter(StreamKind_Last, Generic_CodecID));
    Ztring ID;
    if (Reference->StreamID.empty())
        ID=MI->Retrieve(StreamKind_Last, StreamPos_Last, General_ID);
    else
        ID=Reference->StreamID;
    MI->Clear(StreamKind_Last, StreamPos_Last, General_ID);

    MI->Merge(*Reference->MI->Info, StreamKind_Last, 0, StreamPos_Last);
    File_Size_Total+=Ztring(Reference->MI->Get(Stream_General, 0, General_FileSize)).To_int64u();

    //Hacks - After
    if (CodecID!=MI->Retrieve(StreamKind_Last, StreamPos_Last, MI->Fill_Parameter(StreamKind_Last, Generic_CodecID)))
    {
        if (!CodecID.empty())
            CodecID+=_T(" / ");
        CodecID+=MI->Retrieve(StreamKind_Last, StreamPos_Last, MI->Fill_Parameter(StreamKind_Last, Generic_CodecID));
        MI->Fill(StreamKind_Last, StreamPos_Last, MI->Fill_Parameter(StreamKind_Last, Generic_CodecID), CodecID, true);
    }
    if (!MI->Retrieve(StreamKind_Last, StreamPos_Last, General_ID).empty() &&  !Config->File_ID_OnlyRoot_Get())
        ID+=_T('-')+MI->Retrieve(StreamKind_Last, StreamPos_Last, General_ID);
    MI->Fill(StreamKind_Last, StreamPos_Last, General_ID, ID, true);
    MI->Fill(StreamKind_Last, StreamPos_Last, General_ID_String, ID, true);

    //Special case: MXF in MXF
    if (Reference->MI->Info && Reference->MI->Info->Get(Stream_General, 0, General_Format)==_T("MXF"))
    {
        MI->Fill(StreamKind_Last, StreamPos_Last, "MuxingMode", "MXF");

        if (Reference->MI->Info->Count_Get(Stream_Video)>0)
        {
            size_t Parser_Audio_Count=Reference->MI->Info->Count_Get(Stream_Audio);
            for (size_t Parser_Audio_Pos=0; Parser_Audio_Pos<Parser_Audio_Count; Parser_Audio_Pos++)
            {
                MI->Fill_Flush();
                size_t Pos=0;
                while (Pos<MI->Count_Get(Stream_Audio))
                {
                    if (MI->Retrieve(Stream_Audio, Pos, General_ID).To_int32u()>ID.To_int32u())
                        break;
                    Pos++;
                }
                MI->Stream_Prepare(Stream_Audio, Pos);
                MI->Merge(*Reference->MI->Info, Stream_Audio, Parser_Audio_Pos, StreamPos_Last);
                if (MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_MuxingMode).empty())
                    MI->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, "MXF");
                else
                    MI->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, _T("MXF / ")+MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_MuxingMode), true);
                MI->Fill(Stream_Audio, StreamPos_Last, "Source", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source"));
                MI->Fill(Stream_Audio, StreamPos_Last, "Source_Info", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source_Info"));
                Ztring ID_Audio=MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_ID);
                MI->Fill(Stream_Audio, StreamPos_Last, Audio_ID, ID.SubString(Ztring(), _T("-"))+_T("-")+ID_Audio, true);
                MI->Fill(Stream_Audio, StreamPos_Last, Audio_ID_String, ID.SubString(Ztring(), _T("-"))+_T("-")+ID_Audio, true);
            }
            size_t Parser_Text_Count=Reference->MI->Info->Count_Get(Stream_Text);
            for (size_t Parser_Text_Pos=0; Parser_Text_Pos<Parser_Text_Count; Parser_Text_Pos++)
            {
                MI->Fill_Flush();
                size_t Pos=0;
                while (Pos<MI->Count_Get(Stream_Text))
                {
                    if (MI->Retrieve(Stream_Text, Pos, General_ID).To_int32u()>ID.To_int32u())
                        break;
                    Pos++;
                }
                MI->Stream_Prepare(Stream_Text, Pos);
                MI->Merge(*Reference->MI->Info, Stream_Text, Parser_Text_Pos, StreamPos_Last);
                if (MI->Retrieve(Stream_Text, StreamPos_Last, Text_MuxingMode).empty())
                    MI->Fill(Stream_Text, StreamPos_Last, Text_MuxingMode, "MXF");
                else
                    MI->Fill(Stream_Text, StreamPos_Last, Text_MuxingMode, _T("MXF / ")+MI->Retrieve(Stream_Text, StreamPos_Last, Text_MuxingMode), true);
                MI->Fill(Stream_Text, StreamPos_Last, "Source", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source"));
                MI->Fill(Stream_Text, StreamPos_Last, "Source_Info", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source_Info"));
                Ztring ID_Text=MI->Retrieve(Stream_Text, StreamPos_Last, Text_ID);
                MI->Fill(Stream_Text, StreamPos_Last, Text_ID, ID.SubString(Ztring(), _T("-"))+_T("-")+ID_Text, true);
                MI->Fill(Stream_Text, StreamPos_Last, Text_ID_String, ID.SubString(Ztring(), _T("-"))+_T("-")+ID_Text, true);
            }
        }
    }
    //Special case: DV with Audio or/and Text in the video stream
    else if (Reference->MI->Info && Reference->MI->Info->Get(Stream_General, 0, General_Format)==_T("DV") && (Reference->MI->Info->Count_Get(Stream_Audio) || Reference->MI->Info->Count_Get(Stream_Text)))
    {
        //Video and Audio are together
        size_t Parser_Audio_Count=Reference->MI->Info->Count_Get(Stream_Audio);
        for (size_t Parser_Audio_Pos=0; Parser_Audio_Pos<Parser_Audio_Count; Parser_Audio_Pos++)
        {
            MI->Fill_Flush();
            size_t Pos=0;
            while (Pos<MI->Count_Get(Stream_Audio))
            {
                if (MI->Retrieve(Stream_Audio, Pos, General_ID).To_int32u()>ID.To_int32u())
                    break;
                Pos++;
            }
            MI->Stream_Prepare(Stream_Audio, Pos);
            MI->Merge(*Reference->MI->Info, Stream_Audio, Parser_Audio_Pos, StreamPos_Last);
            if (MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_MuxingMode).empty())
                MI->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_Format), true);
            else
                MI->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_Format)+_T(" / ")+MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_MuxingMode), true);
            MI->Fill(Stream_Audio, StreamPos_Last, Audio_Duration, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_Duration), true);
            MI->Fill(Stream_Audio, StreamPos_Last, "Source", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source"));
            MI->Fill(Stream_Audio, StreamPos_Last, "Source_Info", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source_Info"));
            Ztring ID=MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_ID);
            MI->Fill(Stream_Audio, StreamPos_Last, Audio_ID, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_ID)+_T("-")+ID, true);
            MI->Fill(Stream_Audio, StreamPos_Last, Audio_ID_String, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_ID_String)+_T("-")+ID, true);
        }

        //Video and Text are together
        size_t Parser_Text_Count=Reference->MI->Info->Count_Get(Stream_Text);
        for (size_t Parser_Text_Pos=0; Parser_Text_Pos<Parser_Text_Count; Parser_Text_Pos++)
        {
            MI->Fill_Flush();
            size_t Pos=0;
            while (Pos<MI->Count_Get(Stream_Text))
            {
                if (MI->Retrieve(Stream_Text, Pos, General_ID).To_int32u()>ID.To_int32u())
                    break;
                Pos++;
            }
            MI->Stream_Prepare(Stream_Text, Pos);
            MI->Merge(*Reference->MI->Info, Stream_Text, Parser_Text_Pos, StreamPos_Last);
            if (MI->Retrieve(Stream_Text, StreamPos_Last, Text_MuxingMode).empty())
                MI->Fill(Stream_Text, StreamPos_Last, Text_MuxingMode, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_Format), true);
            else
                MI->Fill(Stream_Text, StreamPos_Last, Text_MuxingMode, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_Format)+_T(" / ")+MI->Retrieve(Stream_Text, StreamPos_Last, Text_MuxingMode), true);
            MI->Fill(Stream_Text, StreamPos_Last, Text_Duration, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_Duration), true);
            MI->Fill(Stream_Text, StreamPos_Last, "Source", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source"));
            MI->Fill(Stream_Text, StreamPos_Last, "Source_Info", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source_Info"));
            Ztring ID=MI->Retrieve(Stream_Text, StreamPos_Last, Text_ID);
            MI->Fill(Stream_Text, StreamPos_Last, Text_ID, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_ID)+_T("-")+ID, true);
            MI->Fill(Stream_Text, StreamPos_Last, Text_ID_String, MI->Retrieve(Stream_Video, Reference->StreamPos, Video_ID_String)+_T("-")+ID, true);
        }
    }
    //Others
    else if (Reference->MI->Info && MI->Retrieve(StreamKind_Last, StreamPos_Last, Reference->MI->Info->Fill_Parameter(StreamKind_Last, Generic_Format))!=Reference->MI->Info->Get(Stream_General, 0, General_Format))
        MI->Fill(StreamKind_Last, StreamPos_Last, "MuxingMode", Reference->MI->Info->Get(Stream_General, 0, General_Format));
}

} //NameSpace

#endif //MEDIAINFO_REFERENCES_YES

