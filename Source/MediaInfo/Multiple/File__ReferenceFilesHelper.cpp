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
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_REFERENCES_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File__ReferenceFilesHelper.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "ZenLib/Dir.h"
#include "ZenLib/File.h"
#include "ZenLib/FileName.h"
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
    FrameRate=0;
    Duration=0;
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

            //Using the frame rate from the first stream having a frame rate
            if (!FrameRate)
                for (references::iterator ReferenceFrameRate=References.begin(); ReferenceFrameRate!=References.end(); ReferenceFrameRate++)
                    if (ReferenceFrameRate->FrameRate)
                    {
                        FrameRate=ReferenceFrameRate->FrameRate;
                        break;
                    }

            if (Config->NextPacket_Get() && MI->Demux_EventWasSent_Accept_Specific)
            {
                while (Reference!=References.end())
                {
                    ParseReference(); //Init
                    Reference++;
                }

                //Cleanup
                for (size_t Pos=0; Pos<References.size(); Pos++)
                    if (References[Pos].Status[File__Analyze::IsFinished])
                    {
                        References.erase(References.begin()+Pos);
                        Pos--;
                    }
                if (References.empty())
                    return;

                //File size handling
                if (File_Size_Total!=MI->File_Size)
                {
                    MI->Fill(Stream_General, 0, General_FileSize, File_Size_Total, 10, true);
                    MI->Fill(Stream_General, 0, General_StreamSize, MI->File_Size, 10, true);
                }

                Reference=References.begin();
                Config->Demux_EventWasSent=true;
                return;
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
        Reference->MI->Option(_T("File_IsReferenced"), _T("1"));
        Reference->MI->Option(_T("File_FileNameFormat"), _T("CSV"));
        if (MediaInfoLib::Config.ParseSpeed_Get()<1.0)
            Reference->MI->Option(_T("File_StopAfterFilled"), _T("1"));
        Reference->MI->Option(_T("File_KeepInfo"), _T("1"));
        Reference->MI->Option(_T("File_ID_OnlyRoot"), Config->File_ID_OnlyRoot_Get()?_T("1"):_T("0"));
        Reference->MI->Option(_T("File_DvDif_DisableAudioIfIsInContainer"), Config->File_DvDif_DisableAudioIfIsInContainer_Get()?_T("1"):_T("0"));
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
            if (FrameRate)
                Reference->MI->Option(_T("File_Demux_Rate"), Ztring::ToZtring(FrameRate, 15));
            switch (Config->Demux_InitData_Get())
            {
                case 0 : Reference->MI->Option(_T("File_Demux_InitData"), _T("Event")); break;
                case 1 : Reference->MI->Option(_T("File_Demux_InitData"), _T("Field")); break;
                default: ;
            }
        #endif //MEDIAINFO_DEMUX
        #if MEDIAINFO_IBI
            if (!Reference->IbiStream.Infos.empty())
            {
                ibi Ibi;
                Ibi.Streams[(int64u)-1]=new ibi::stream(Reference->IbiStream);

                //IBI Creation
                File_Ibi_Creation IbiCreation(Ibi);
                Ztring IbiText=IbiCreation.Finish();
                if (!IbiText.empty())
                    Reference->MI->Option(_T("File_Ibi"), IbiText);
            }
        #endif //MEDIAINFO_IBI
 
        //Configuring file name
        ZtringList Names=Reference->FileNames;
        ZtringList AbsoluteNames; AbsoluteNames.Separator_Set(0, ",");
        for (size_t Pos=0; Pos<Names.size(); Pos++)
        {
            if (Names[Pos].find(_T("file:///"))==0)
            {
                Names[Pos].erase(0, 8); //Removing "file:///", this is the default behaviour and this makes comparison easier
                Names[Pos]=ZenLib::Format::Http::URL_Encoded_Decode(Names[Pos]);
            }
            if (Names[Pos].find(_T("file://"))==0)
            {
                Names[Pos].erase(0, 7); //Removing "file://", this is the default behaviour and this makes comparison easier
                Names[Pos]=ZenLib::Format::Http::URL_Encoded_Decode(Names[Pos]);
            }
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
            AbsoluteNames.clear();
            
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

            if (!File::Exists(AbsoluteNames[0]))
            {
                AbsoluteNames.clear();
                Names=Reference->FileNames;
            
                //Configuring file name (this time, we try to test local files)
                size_t PathSeparator_Pos=Names[0].find_last_of(_T("\\/"));
                if (PathSeparator_Pos!=string::npos && PathSeparator_Pos)
                {
                    Ztring PathToRemove=Names[0].substr(0, PathSeparator_Pos);
                    bool IsOk=true;
                    for (size_t Pos=0; Pos<Names.size(); Pos++)
                        if (Names[Pos].find(PathToRemove))
                        {
                            IsOk=false;
                            break;
                        }
                    if (IsOk)
                    {
                        for (size_t Pos=0; Pos<Names.size(); Pos++)
                        {
                            Names[Pos].erase(0, PathSeparator_Pos+1);
                            Ztring AbsoluteName=ZenLib::FileName::Path_Get(MI->File_Name);
                            if (!AbsoluteName.empty())
                                AbsoluteName+=ZenLib::PathSeparator;
                            AbsoluteName+=Names[Pos];
                            #ifdef __WINDOWS__
                                AbsoluteName.FindAndReplace(_T("/"), _T("\\"), 0, Ztring_Recursive); //Names[Pos] normalization
                            #endif //__WINDOWS__
                            AbsoluteNames.push_back(AbsoluteName);
                        }

                        if (!File::Exists(AbsoluteNames[0]))
                        {
                            AbsoluteNames.clear();
                            Names=Reference->FileNames;
            
                            //Configuring file name (this time, we try to test local files)
                            size_t PathSeparator_Pos=Names[0].find_last_of(_T("\\/"));
                            if (PathSeparator_Pos!=string::npos && PathSeparator_Pos)
                                PathSeparator_Pos=Names[0].find_last_of(_T("\\/"), PathSeparator_Pos-1);
                            if (PathSeparator_Pos!=string::npos && PathSeparator_Pos)
                            {
                                Ztring PathToRemove=Names[0].substr(0, PathSeparator_Pos);
                                bool IsOk=true;
                                for (size_t Pos=0; Pos<Names.size(); Pos++)
                                    if (Names[Pos].find(PathToRemove))
                                    {
                                        IsOk=false;
                                        break;
                                    }
                                if (IsOk)
                                    for (size_t Pos=0; Pos<Names.size(); Pos++)
                                    {
                                        Names[Pos].erase(0, PathSeparator_Pos+1);
                                        Ztring AbsoluteName=ZenLib::FileName::Path_Get(MI->File_Name);
                                        if (!AbsoluteName.empty())
                                            AbsoluteName+=ZenLib::PathSeparator;
                                        AbsoluteName+=Names[Pos];
                                        #ifdef __WINDOWS__
                                            AbsoluteName.FindAndReplace(_T("/"), _T("\\"), 0, Ztring_Recursive); //Names[Pos] normalization
                                        #endif //__WINDOWS__
                                        AbsoluteNames.push_back(AbsoluteName);
                                    }
                            }
                        }
                    }
                }
            }
        }
        MI->Fill(Reference->StreamKind, Reference->StreamPos, "Source", Reference->FileNames.Read(0));
        if (!AbsoluteNames.empty())
            Reference->FileNames=AbsoluteNames;

        if (!AbsoluteNames.empty() && AbsoluteNames[0]==MI->File_Name)
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
            else
                File_Size_Total+=Ztring(Reference->MI->Get(Stream_General, 0, General_FileSize)).To_int64u();

            #if MEDIAINFO_NEXTPACKET && MEDIAINFO_DEMUX
                if (Config->NextPacket_Get() && MI->Demux_EventWasSent_Accept_Specific)
                    return;
            #endif //MEDIAINFO_NEXTPACKET
        }
    }

    if (Reference->MI)
    {
        #if MEDIAINFO_EVENTS && MEDIAINFO_NEXTPACKET
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
        #endif //MEDIAINFO_EVENTS && MEDIAINFO_NEXTPACKET
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
                
                if (Reference->StreamKind==Stream_Max)
                    return; //There is a problem
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
        for (size_t Pos=General_ID; Pos<Reference->MI->Count_Get(Stream_Image, 0); Pos++)
            Reference->MI->Info->Fill(Stream_Video, 0, Reference->MI->Get(Stream_Image, 0, Pos, Info_Name).To_UTF8().c_str(), Reference->MI->Get(Stream_Image, 0, Pos), true);
        Reference->MI->Info->Stream_Erase(Stream_Image, 0);

        if (Reference->FrameRate)
            Reference->MI->Info->Fill(Stream_Video, 0, Video_FrameRate, Reference->FrameRate);
        Reference->MI->Info->Fill(Stream_Video, 0, Video_FrameCount, Reference->FileNames.size());

        //Stream size (only for files not of FTP/HTTP)
        int64u StreamSize=Reference->MI->Get(Stream_General, 0, General_FileSize).To_int64u();
        for (size_t Pos=1; Pos<Reference->FileNames.size(); Pos++)
        {
            int64u OneFileSize=File::Size_Get(Reference->FileNames[Pos]);
            StreamSize+=OneFileSize;
            File_Size_Total+=OneFileSize;
        }
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

    //Frame rate if not present
    if (StreamKind_Last==Stream_Video && MI->Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate).empty() && Reference->FrameRate)
        MI->Fill(Stream_Video, StreamPos_Last, Video_FrameRate, Reference->FrameRate);

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
                StreamPos_Last=Stream_Prepare(Stream_Audio, Pos);
                MI->Merge(*Reference->MI->Info, Stream_Audio, Parser_Audio_Pos, StreamPos_Last);
                if (MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_MuxingMode).empty())
                    MI->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, "MXF");
                else
                    MI->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, _T("MXF / ")+MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_MuxingMode), true);
                MI->Fill(Stream_Audio, StreamPos_Last, "Source", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source"));
                MI->Fill(Stream_Audio, StreamPos_Last, "Source_Info", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source_Info"));
                Ztring ID_Audio=MI->Retrieve(Stream_Audio, StreamPos_Last, Audio_ID);
                if (ID_Audio.find(_T('-'))!=string::npos)
                    ID_Audio.erase(0, ID_Audio.find(_T('-')));
                MI->Fill(Stream_Audio, StreamPos_Last, Audio_ID, ID+ID_Audio, true);
                MI->Fill(Stream_Audio, StreamPos_Last, Audio_ID_String, ID+ID_Audio, true);
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
                StreamPos_Last=Stream_Prepare(Stream_Text, Pos);
                MI->Merge(*Reference->MI->Info, Stream_Text, Parser_Text_Pos, StreamPos_Last);
                if (MI->Retrieve(Stream_Text, StreamPos_Last, Text_MuxingMode).empty())
                    MI->Fill(Stream_Text, StreamPos_Last, Text_MuxingMode, "MXF");
                else
                    MI->Fill(Stream_Text, StreamPos_Last, Text_MuxingMode, _T("MXF / ")+MI->Retrieve(Stream_Text, StreamPos_Last, Text_MuxingMode), true);
                MI->Fill(Stream_Text, StreamPos_Last, "Source", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source"));
                MI->Fill(Stream_Text, StreamPos_Last, "Source_Info", MI->Retrieve(Stream_Video, Reference->StreamPos, "Source_Info"));
                Ztring ID_Text=MI->Retrieve(Stream_Text, StreamPos_Last, Text_ID);
                if (ID_Text.find(_T('-'))!=string::npos)
                    ID_Text.erase(0, ID_Text.find(_T('-')));
                MI->Fill(Stream_Text, StreamPos_Last, Text_ID, ID+ID_Text, true);
                MI->Fill(Stream_Text, StreamPos_Last, Text_ID_String, ID+ID_Text, true);
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
            StreamPos_Last=Stream_Prepare(Stream_Audio, Pos);
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
            StreamPos_Last=Stream_Prepare(Stream_Text, Pos);
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

//---------------------------------------------------------------------------
void File__ReferenceFilesHelper::Read_Buffer_Unsynched()
{
    MI->Open_Buffer_Unsynch();
    for (references::iterator Reference=References.begin(); Reference!=References.end(); Reference++)
        if (Reference->MI)
            Reference->MI->Open_Buffer_Unsynch();

    #if MEDIAINFO_DEMUX
        Config->Demux_EventWasSent=true; //We want not try to read new data from the file
    #endif //MEDIAINFO_DEMUX
}

//---------------------------------------------------------------------------
#if MEDIAINFO_SEEK
size_t File__ReferenceFilesHelper::Read_Buffer_Seek (size_t Method, int64u Value, int64u ID)
{
    //Parsing
    switch (Method)
    {
        case 0  :   
                    #if MEDIAINFO_DEMUX
                        {
                        if (Value)
                        {
                            if (Value>File_Size_Total)
                                return 2; //Invalid value

                            //Init
                            if (!Duration)
                            {
                                MediaInfo_Internal MI2;
                                MI2.Option(_T("File_KeepInfo"), _T("1"));
                                Ztring ParseSpeed_Save=MI2.Option(_T("ParseSpeed_Get"), _T(""));
                                Ztring Demux_Save=MI2.Option(_T("Demux_Get"), _T(""));
                                MI2.Option(_T("ParseSpeed"), _T("0"));
                                MI2.Option(_T("Demux"), Ztring());
                                size_t MiOpenResult=MI2.Open(MI->File_Name);
                                MI2.Option(_T("ParseSpeed"), ParseSpeed_Save); //This is a global value, need to reset it. TODO: local value
                                MI2.Option(_T("Demux"), Demux_Save); //This is a global value, need to reset it. TODO: local value
                                if (!MiOpenResult)
                                    return -1;
                                Duration=MI2.Get(Stream_General, 0, General_Duration).To_float64()/1000;
                            }

                            //Time percentage
                            float64 DurationF=Duration;
                            DurationF*=Value;
                            DurationF/=File_Size_Total;
                            size_t DurationM=(size_t)(DurationF*1000);
                            Ztring DurationS;
                            DurationS+=L'0'+DurationM/(10*60*60*1000); DurationM%=10*60*60*1000;
                            DurationS+=L'0'+DurationM/(   60*60*1000); DurationM%=   60*60*1000;
                            DurationS+=L':';
                            DurationS+=L'0'+DurationM/(   10*60*1000); DurationM%=   10*60*1000;
                            DurationS+=L'0'+DurationM/(      60*1000); DurationM%=      60*1000;
                            DurationS+=L':';
                            DurationS+=L'0'+DurationM/(      10*1000); DurationM%=      10*1000;
                            DurationS+=L'0'+DurationM/(         1000); DurationM%=         1000;
                            DurationS+=L'.';
                            DurationS+=L'0'+DurationM/(          100); DurationM%=          100;
                            DurationS+=L'0'+DurationM/(           10); DurationM%=           10;
                            DurationS+=L'0'+DurationM;

                            CountOfReferencesToParse=References.size();
                            bool HasProblem=false;
                            for (Reference=References.begin(); Reference!=References.end(); Reference++)
                            {
                                if (Reference->MI)
                                {
                                    Ztring Result=Reference->MI->Option(_T("File_Seek"), DurationS);
                                    if (!Result.empty())
                                        HasProblem=true;
                                }
                                Reference->Status.reset();
                            }
                            Reference=References.begin();
                            Open_Buffer_Unsynch();
                            return HasProblem?2:1; //Invalid value if there is a problem (TODO: better info)
                        }

                        CountOfReferencesToParse=References.size();
                        bool HasProblem=false;
                        for (Reference=References.begin(); Reference!=References.end(); Reference++)
                        {
                            if (Reference->MI)
                            {
                                Ztring Result=Reference->MI->Option(_T("File_Seek"), Ztring::ToZtring(Value));
                                if (!Result.empty())
                                    HasProblem=true;
                            }
                            Reference->Status.reset();
                        }
                        Reference=References.begin();
                        Open_Buffer_Unsynch();
                        return HasProblem?2:1; //Invalid value if there is a problem (TODO: better info)
                        }
                    #else //MEDIAINFO_DEMUX
                        return (size_t)-1; //Not supported
                    #endif //MEDIAINFO_DEMUX
        case 1  :   
                    {
                        //Time percentage
                        int64u Duration=MI->Get(Stream_General, 0, General_Duration).To_int64u();
                        Duration*=Value;
                        Duration/=10000;
                        Ztring DurationS;
                        DurationS+=L'0'+Duration/(10*60*60*1000); Duration%=10*60*60*1000;
                        DurationS+=L'0'+Duration/(   60*60*1000); Duration%=   60*60*1000;
                        DurationS+=L':';
                        DurationS+=L'0'+Duration/(   10*60*1000); Duration%=   10*60*1000;
                        DurationS+=L'0'+Duration/(      60*1000); Duration%=      60*1000;
                        DurationS+=L':';
                        DurationS+=L'0'+Duration/(      10*1000); Duration%=      10*1000;
                        DurationS+=L'0'+Duration/(         1000); Duration%=         1000;
                        DurationS+=L'.';
                        DurationS+=L'0'+Duration/(          100); Duration%=          100;
                        DurationS+=L'0'+Duration/(           10); Duration%=           10;
                        DurationS+=L'0'+Duration;

                        CountOfReferencesToParse=References.size();
                        bool HasProblem=false;
                        for (Reference=References.begin(); Reference!=References.end(); Reference++)
                        {
                            if (Reference->MI)
                            {
                                Ztring Result=Reference->MI->Option(_T("File_Seek"), DurationS);
                                if (!Result.empty())
                                    HasProblem=true;
                            }
                            Reference->Status.reset();
                        }
                        Reference=References.begin();
                        Open_Buffer_Unsynch();
                        return HasProblem?2:1; //Invalid value if there is a problem (TODO: better info)
                    }
        case 2  :   //Timestamp
                    {
                        //We transform TimeStamp to a frame number
                        if (!FrameRate)
                            return (size_t)-1; //Not supported

                        if (References.begin()!=References.end())
                        {
                            int64u Delay=References.begin()->Delay;
                            if (Value<Delay)
                                return 2; //Invalid value 
                            Value-=Delay;
                        }
                        Value=(int64u)(((float64)Value)/1000000000*FrameRate);
                        }
                    //No break;
        case 3  :   //FrameNumber
                    #if MEDIAINFO_DEMUX
                        CountOfReferencesToParse=References.size();
                        for (Reference=References.begin(); Reference!=References.end(); Reference++)
                        {
                            if (Reference->MI)
                            {
                                Ztring Result=Reference->MI->Option(_T("File_Seek"), _T("Frame=")+Ztring::ToZtring(Value));
                                if (!Result.empty())
                                    return 2; //Invalid value
                            }
                            else
                            {
                                //There was a problem, removing Reference
                                References.clear();
                                return Read_Buffer_Seek(Method, Value, ID);
                            }
                            Reference->Status.reset();
                        }
                        Reference=References.begin();
                        Open_Buffer_Unsynch();
                        return 1;
                    #else //MEDIAINFO_DEMUX
                        return (size_t)-1; //Not supported
                    #endif //MEDIAINFO_DEMUX
         default :   return 0;
    }
}
#endif //MEDIAINFO_SEEK

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
size_t File__ReferenceFilesHelper::Stream_Prepare (stream_t StreamKind, size_t StreamPos)
{
    size_t StreamPos_Last=MI->Stream_Prepare(StreamKind, StreamPos);

    for (references::iterator ReferencePos=References.begin(); ReferencePos!=References.end(); ReferencePos++)
        if (ReferencePos->StreamKind==StreamKind && ReferencePos->StreamPos>=StreamPos_Last)
            if (ReferencePos->StreamPos!=(size_t)-1)
                ReferencePos->StreamPos++;

    return StreamPos_Last;
}

} //NameSpace

#endif //MEDIAINFO_REFERENCES_YES
