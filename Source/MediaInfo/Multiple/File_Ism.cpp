// File_Ism - Info for ISM files
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
#if defined(MEDIAINFO_ISM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Ism.h"
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
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ism::File_Ism()
:File__Analyze()
{
    //Temp
    ReferenceFiles=NULL;
}

//---------------------------------------------------------------------------
File_Ism::~File_Ism()
{
    delete ReferenceFiles; //ReferenceFiles=NULL;
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ism::Streams_Finish()
{
    if (ReferenceFiles==NULL)
        return;

    ReferenceFiles->ParseReferences();
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
#if MEDIAINFO_SEEK
size_t File_Ism::Read_Buffer_Seek (size_t Method, int64u Value, int64u ID)
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
bool File_Ism::FileHeader_Begin()
{
    //Element_Size
    if (File_Size>1024*1024)
    {
        Reject("ISM");
        return false; //ISM files are not big
    }

    TiXmlDocument document(File_Name.To_Local());
    if (document.LoadFile())
    {
        TiXmlElement* Root=document.FirstChildElement("smil");
        if (Root)
        {
            Accept("ISM");
            Fill(Stream_General, 0, General_Format, "ISM");

            ReferenceFiles=new File__ReferenceFilesHelper(this, Config);

            TiXmlElement* Body=Root->FirstChildElement();
            while (Body)
            {
                if (Body->ValueStr()=="body")
                {
                    TiXmlElement* Switch=Body->FirstChildElement();
                    while (Switch)
                    {
                        if (Switch->ValueStr()=="switch")
                        {
                            TiXmlElement* Stream=Switch->FirstChildElement();
                            while (Stream)
                            {
                                if (Stream->ValueStr()=="video" || Stream->ValueStr()=="audio")
                                {
                                    File__ReferenceFilesHelper::reference ReferenceFile;

                                    if (Stream->ValueStr()=="video")
                                        ReferenceFile.StreamKind=Stream_Video;
                                    if (Stream->ValueStr()=="audio")
                                        ReferenceFile.StreamKind=Stream_Audio;

                                    const char* Attribute=Stream->Attribute("src");
                                    if (Attribute)
                                        ReferenceFile.FileNames.push_back(Ztring().From_UTF8(Attribute));

                                    TiXmlElement* Param=Stream->FirstChildElement();
                                    while (Param)
                                    {
                                        if (Param->ValueStr()=="param")
                                        {
                                            Attribute=Param->Attribute("name");
                                            if (Attribute && Ztring().From_UTF8(Attribute)==_T("trackID"))
                                            {
                                                 Attribute=Param->Attribute("value");
                                                 if (Attribute)
                                                     ReferenceFile.StreamID.From_UTF8(Attribute);
                                            }
                                        }
                                        Param=Param->NextSiblingElement();
                                    }

                                    ReferenceFiles->References.push_back(ReferenceFile);
                                }

                                Stream=Stream->NextSiblingElement();
                            }
                        }

                        Switch=Switch->NextSiblingElement();
                    }
                }

                Body=Body->NextSiblingElement();
            }
        }
        else
        {
            Reject("ISM");
            return false;
        }
    }
    else
    {
        Reject("ISM");
        return false;
    }

    //All should be OK...
    return true;
}

} //NameSpace

#endif //MEDIAINFO_ISM_YES

