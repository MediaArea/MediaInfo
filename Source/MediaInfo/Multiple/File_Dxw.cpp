// File_Dxw - Info for DXW files
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
#include "MediaInfo/Multiple/File__ReferenceFilesHelper.h"
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
    if (ReferenceFiles==NULL)
    {
        ReferenceFiles=new File__ReferenceFilesHelper(this, Config);

        for (references::iterator Reference=References.begin(); Reference!=References.end(); Reference++)
        {
            File__ReferenceFilesHelper::reference ReferenceFile;
            ReferenceFile.FileNames=Reference->FileNames;
            ReferenceFile.StreamKind=Reference->StreamKind;
            ReferenceFile.FrameRate=Reference->FrameRate;
            ReferenceFile.StreamID=Ztring::ToZtring((size_t)(Reference-References.begin())+1);
            ReferenceFiles->References.push_back(ReferenceFile);
        }
    }
    
    ReferenceFiles->ParseReferences();
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#if MEDIAINFO_SEEK
size_t File_Dxw::Read_Buffer_Seek (size_t Method, int64u Value, int64u ID)
{
    if (ReferenceFiles==NULL)
        return 0;
        
    return ReferenceFiles->Read_Buffer_Seek(Method, Value, ID);    
}
#endif //MEDIAINFO_SEEK

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
                return false;
            }

            Accept("DXW");
            Fill(Stream_General, 0, General_Format, "DXW");

            TiXmlElement* Track=Root->FirstChildElement();
            while (Track)
            {
                if (Track->ValueStr()=="clip")
                {
                    Attribute=Track->Attribute("file");
                    if (Attribute)
                    {
                        reference Reference_Temp;
                        Reference_Temp.FileNames.push_back(Ztring().From_UTF8(Attribute));

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
                        ReferenceFiles=NULL;
                    }

                    Attribute=Track->Attribute("framerate");
                    if (Attribute)
                    {
                        reference Reference_Temp;
                        Reference_Temp.FrameRate=Ztring().From_UTF8(Attribute).To_float64();

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

                        TiXmlElement* Frame=Track->FirstChildElement();
                        while (Frame)
                        {
                            if (Frame->ValueStr()=="frame")
                            {
                                Attribute=Frame->Attribute("file");
                                if (Attribute)
                                    Reference_Temp.FileNames.push_back(Ztring().From_UTF8(Attribute));
                            }

                            Frame=Frame->NextSiblingElement();
                        }

                        References.push_back(Reference_Temp);
                        ReferenceFiles=NULL;
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
    ReferenceFiles=NULL;
    return true;
}

} //NameSpace

#endif //MEDIAINFO_DXW_YES

