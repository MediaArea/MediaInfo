// File_P2_Clip - Info for P2 Clip (XML) files
// Copyright (C) 2010-2011 MediaArea.net SARL, Info@MediaArea.net
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
#if defined(MEDIAINFO_DXW_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Dxw.h"
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "ZenLib/Dir.h"
#include "ZenLib/FileName.h"
#include "ZenLib/TinyXml/tinyxml.h"
#include "ZenLib/Format/Http/Http_Utils.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dxw::Streams_Finish()
{
    while (Reference!=References.end())
    {
        Streams_Finish_ParseReference();
		#if MEDIAINFO_DEMUX
			if (Config->Demux_EventWasSent)
				return;
		#endif //MEDIAINFO_DEMUX
        Reference++;
    }

    if (File_Size_Total!=File_Size)
        Fill(Stream_General, 0, General_FileSize, File_Size_Total, 10, true);
}

//---------------------------------------------------------------------------
void File_Dxw::Streams_Finish_ParseReference()
{
    if (MI==NULL)
    {
        //Configuring file name
        Ztring Name=Reference->FileName;
        if (Name.find(_T("file:"))==0)
        {
            Name.erase(0, 5); //Removing "file:", this is the default behaviour and this makes comparison easier
            Name=ZenLib::Format::Http::URL_Encoded_Decode(Name);
        }
        Ztring AbsoluteName;
        if (Name.find(_T(':'))!=1 && Name.find(_T("/"))!=0 && Name.find(_T("\\\\"))!=0) //If absolute patch
        {
            AbsoluteName=ZenLib::FileName::Path_Get(File_Name);
            if (!AbsoluteName.empty())
                AbsoluteName+=ZenLib::PathSeparator;
        }
        AbsoluteName+=Name;
        #ifdef __WINDOWS__
            AbsoluteName.FindAndReplace(_T("/"), _T("\\"), 0, Ztring_Recursive); //Name normalization
        #endif //__WINDOWS__

        if (AbsoluteName==File_Name)
        {
            if (StreamKind_Last!=Stream_Max)
            {
                Fill(StreamKind_Last, StreamPos_Last, "Source_Info", "Circular");
                StreamKind_Last=Stream_Max;
                StreamPos_Last=(size_t)-1;
            }
            return;
        }

        //Configuration
        MI=new MediaInfo_Internal();
        MI->Option(_T("File_KeepInfo"), _T("1"));
        if (Config->NextPacket_Get())
            MI->Option(_T("File_NextPacket"), _T("1"));
        if (Config->Event_CallBackFunction_IsSet())
            MI->Option(_T("File_Event_CallBackFunction"), Config->Event_CallBackFunction_Get());
        MI->Option(_T("File_SubFile_StreamID_Set"), Ztring::ToZtring(Reference-References.begin()+1));
        #if MEDIAINFO_DEMUX
            if (Config->Demux_Unpacketize_Get())
                MI->Option(_T("File_Demux_Unpacketize"), _T("1"));
        #endif //MEDIAINFO_DEMUX

        //Run
        if (MI->Open(AbsoluteName))
        {
            if (!Config->NextPacket_Get()) //Only if NextPacket interface is not requested, else this is done later
            {
                Streams_Finish_ParseReference_Finalize();
                delete MI; MI=NULL;
            }
        }
        else
        {
            //Filling
            if (StreamKind_Last!=Stream_Max)
            {
                Fill(StreamKind_Last, StreamPos_Last, General_ID, Ztring::ToZtring(Reference-References.begin()+1));
                Fill(StreamKind_Last, StreamPos_Last, "Source", (*Reference).FileName);
                Fill(StreamKind_Last, StreamPos_Last, "Source_Info", "Missing");
            }
            delete MI; MI=NULL;
        }
    }

    if (MI)
    {
        while (MI->Open_NextPacket()[8])
        {
            #if MEDIAINFO_DEMUX
                if (Config->Event_CallBackFunction_IsSet())
                {
                    Config->Demux_EventWasSent=true;
                    return;
                }
            #endif //MEDIAINFO_DEMUX
        }
        Streams_Finish_ParseReference_Finalize();
        delete MI; MI=NULL;
    }
}

//---------------------------------------------------------------------------
void File_Dxw::Streams_Finish_ParseReference_Finalize ()
{
    File_Size_Total+=Ztring(MI->Get(Stream_General, 0, General_FileSize)).To_int64u();

    for (size_t StreamKind=Stream_General+1; StreamKind<Stream_Max; StreamKind++)
        for (size_t StreamPos=0; StreamPos<MI->Count_Get((stream_t)StreamKind); StreamPos++)
        {
            Stream_Prepare((stream_t)StreamKind);
            Merge(*MI, StreamKind_Last, StreamPos, StreamPos_Last);
            Fill(StreamKind_Last, StreamPos_Last, "MuxingMode", MI->Get(Stream_General, 0, General_Format));
            Fill(StreamKind_Last, StreamPos_Last, "Source", (*Reference).FileName);

            //ID
            Ztring ID=Ztring::ToZtring(Reference-References.begin()+1);
            if (!Retrieve(StreamKind_Last, StreamPos_Last, General_ID).empty())
                ID+=_T('-')+Retrieve(StreamKind_Last, StreamPos_Last, General_ID);
            Fill(StreamKind_Last, StreamPos_Last, General_ID, ID, true);
        }
}

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Dxw::FileHeader_Begin()
{
    //Element_Size
    if (File_Size>64*1024)
    {
        Reject("DXW");
        return false; //DXW files are not big
    }

    //Element_Size
    if (Buffer_Size<5)
        return false; //Must wait for more data

    //XML header
    if (Buffer[0]!='<'
     || Buffer[1]!='?'
     || Buffer[2]!='x'
     || Buffer[3]!='m'
     || Buffer[4]!='l')
    {
        Reject("DXW");
        return false;
    }

    TiXmlDocument document(File_Name.To_Local());
    if (document.LoadFile())
    {
        TiXmlElement* Root=document.FirstChildElement("indexFile");
        if (Root)
        {
            const char* Attribute=Root->Attribute("xmlns");
            if (Attribute==NULL || Ztring().From_UTF8(Attribute)!=_T("urn:digimetrics-xml-wrapper"))
            {
                Reject("DXW");
                Reject();
            }

            Accept("DXW");
            Fill(Stream_General, 0, General_Format, "DXW");

            File_Size_Total=File_Size;

            TiXmlElement* Track=Root->FirstChildElement();
            while (Track)
            {
                if (Track->ValueStr()=="clip")
                {
                    Attribute=Track->Attribute("file");
                    if (Attribute)
                    {
                        reference Reference_Temp;
                        Reference_Temp.FileName.From_UTF8(Attribute);

                        Attribute=Track->Attribute("type");
                        if (Attribute)
                        {
                            Ztring StreamKind; StreamKind.From_UTF8(Attribute);
                            if (StreamKind==_T("video"))
                                 Reference_Temp.StreamKind=Stream_Video;
                            if (StreamKind==_T("audio"))
                                 Reference_Temp.StreamKind=Stream_Audio;
                            if (StreamKind==_T("data"))
                                 Reference_Temp.StreamKind=Stream_Text; //Not sure this is a right mapping, but this is only used when file is missing
                        }

                        References.push_back(Reference_Temp);
                        File_Dxw::Reference=References.begin();
                        MI=NULL;
                    }
                }

                Track=Track->NextSiblingElement();
            }
        }
        else
        {
            Reject("DXW");
            return false;
        }
    }
    else
    {
        Reject("DXW");
        return false;
    }

    //All should be OK...
    return true;
}

} //NameSpace

#endif //MEDIAINFO_DXW_YES

