// File_SequenceInfo - Info for XML files starting with SEQUENCEINFO
// Copyright (C) 2012-2012 MediaArea.net SARL, Info@MediaArea.net
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
#if defined(MEDIAINFO_SEQUENCEINFO_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_SequenceInfo.h"
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfo_Internal.h"
#include "MediaInfo/Multiple/File__ReferenceFilesHelper.h"
#include "ZenLib/File.h"
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
File_SequenceInfo::File_SequenceInfo()
:File__Analyze()
{
    //Temp
    ReferenceFiles=NULL;
}

//---------------------------------------------------------------------------
File_SequenceInfo::~File_SequenceInfo()
{
    delete ReferenceFiles; //ReferenceFiles=NULL;
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_SequenceInfo::Streams_Finish()
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
size_t File_SequenceInfo::Read_Buffer_Seek (size_t Method, int64u Value, int64u ID)
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
bool File_SequenceInfo::FileHeader_Begin()
{
    //Element_Size
    if (File_Size>1024*1024)
    {
        Reject("SequenceInfo");
        return false; //SequenceInfo files are not big
    }

    TiXmlDocument document(File_Name.To_Local());
    if (document.LoadFile())
    {
        TiXmlElement* Root=document.FirstChildElement("SEQUENCEINFO");
        if (Root)
        {
            Accept("SequenceInfo");
            Fill(Stream_General, 0, General_Format, "SequenceInfo");

            ReferenceFiles=new File__ReferenceFilesHelper(this, Config);

            File__ReferenceFilesHelper::reference ReferenceFile;
            ReferenceFile.StreamKind=Stream_Video;

            FileName FN(File_Name);
            Ztring Base=FN.Path_Get();
            size_t Pos=Base.rfind(PathSeparator);
            if (Pos!=string::npos)
            {
                Ztring ToAdd=Base.substr(Pos, string::npos);
                Pos=ToAdd.rfind(_T('_'));
                if (Pos!=string::npos)
                {
                    Ztring Extension=ToAdd.substr(Pos+1, string::npos);
                    Ztring DirectoryBase=Base;
                    DirectoryBase+=ToAdd;
                    DirectoryBase+=_T('_');

                    size_t DirNumberCount=1;
                    Ztring Directory=DirectoryBase;
                    for (; DirNumberCount<9; DirNumberCount++)
                    {
                        Directory+=_T('0');
                        if (Dir::Exists(Directory))
                            break;
                    }

                    if (DirNumberCount<9)
                    {
                        int32u DirNumber=0;
                        do
                        {
                            Ztring Number=Ztring::ToZtring(DirNumber);
                            if (Number.size()<DirNumberCount)
                                Number.insert(0, DirNumberCount-Number.size(), _T('0'));
                            
                            Directory=DirectoryBase;
                            Directory+=Number;
                            if (!Dir::Exists(Directory))
                                break;

                            Ztring FileBase=Directory;
                            FileBase+=ToAdd;
                            FileBase+=_T('_');
                            FileBase+=_T('.');
                            FileBase+=Extension;

                            size_t FileNumberCount=1;
                            Ztring FullFile=FileBase;
                            for (; FileNumberCount<10; FileNumberCount++)
                            {
                                FullFile.insert(FullFile.begin()+FullFile.size()-Extension.size()-1, _T('0'));
                                if (File::Exists(FullFile))
                                    break;
                            }
                            bool FromZero=true;
                            if (FileNumberCount>=9)
                            {
                                //Trying with consecutive file numbers betweens dirs
                                Number=Ztring::ToZtring(ReferenceFile.FileNames.size());
                                FullFile=FileBase;
                                FullFile.insert(FullFile.size()-Extension.size()-1, Number);
                                FileNumberCount=Number.size();
                                if (!File::Exists(FullFile))
                                {
                                    FileNumberCount++;
                                    for (; FileNumberCount<10; FileNumberCount++)
                                    {
                                        FullFile.insert(FullFile.begin()+FullFile.size()-Extension.size()-Number.size()-1, _T('0'));
                                        if (File::Exists(FullFile))
                                        {
                                            FromZero=false;
                                            break;
                                        }
                                    }
                                }
                                else
                                    FromZero=false;
                            }

                            if (FileNumberCount<9)
                            {
                                int32u FileNumber=FromZero?0:ReferenceFile.FileNames.size();
                                do
                                {
                                    Number=Ztring::ToZtring(FileNumber);
                                    if (Number.size()<FileNumberCount)
                                        Number.insert(0, FileNumberCount-Number.size(), _T('0'));
                            
                                    FullFile=FileBase;
                                    FullFile.insert(FullFile.size()-Extension.size()-1, Number);
                                    if (!File::Exists(FullFile))
                                        break;

                                    ReferenceFile.FileNames.push_back(FullFile);

                                    FileNumber++;
                                }
                                while (FileNumber<1000000000);
                            }

                            DirNumber++;
                        }
                        while (DirNumber<1000000000);

                        ReferenceFiles->References.push_back(ReferenceFile);
                    }
                }
            }
        }
        else
        {
            Reject("SequenceInfo");
            return false;
        }
    }
    else
    {
        Reject("SequenceInfo");
        return false;
    }

    //All should be OK...
    return true;
}

} //NameSpace

#endif //MEDIAINFO_SEQUENCEINFO_YES
