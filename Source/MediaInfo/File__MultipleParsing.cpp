// File__MultipleParsing - Info for MultipleParsing files
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
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__MultipleParsing.h"
//---------------------------------------------------------------------------
// Multiple
#if defined(MEDIAINFO_BDAV_YES)
    #include "MediaInfo/Multiple/File_Bdav.h"
#endif
#if defined(MEDIAINFO_CDXA_YES)
    #include "MediaInfo/Multiple/File_Cdxa.h"
#endif
#if defined(MEDIAINFO_DVDIF_YES)
    #include "MediaInfo/Multiple/File_DvDif.h"
#endif
#if defined(MEDIAINFO_DVDV_YES)
    #include "MediaInfo/Multiple/File_Dvdv.h"
#endif
#if defined(MEDIAINFO_FLV_YES)
    #include "MediaInfo/Multiple/File_Flv.h"
#endif
#if defined(MEDIAINFO_MK_YES)
    #include "MediaInfo/Multiple/File_Mk.h"
#endif
#if defined(MEDIAINFO_MPEG4_YES)
    #include "MediaInfo/Multiple/File_Mpeg4.h"
#endif
#if defined(MEDIAINFO_MPEGPS_YES)
    #include "MediaInfo/Multiple/File_MpegPs.h"
#endif
#if defined(MEDIAINFO_MPEGTS_YES)
    #include "MediaInfo/Multiple/File_MpegTs.h"
#endif
#if defined(MEDIAINFO_MXF_YES)
    #include "MediaInfo/Multiple/File_Mxf.h"
#endif
#if defined(MEDIAINFO_NUT_YES)
    #include "MediaInfo/Multiple/File_Nut.h"
#endif
#if defined(MEDIAINFO_OGG_YES)
    #include "MediaInfo/Multiple/File_Ogg.h"
#endif
#if defined(MEDIAINFO_RIFF_YES)
    #include "MediaInfo/Multiple/File_Riff.h"
#endif
#if defined(MEDIAINFO_RM_YES)
    #include "MediaInfo/Multiple/File_Rm.h"
#endif
#if defined(MEDIAINFO_SKM_YES)
    #include "MediaInfo/Multiple/File_Skm.h"
#endif
#if defined(MEDIAINFO_SWF_YES)
    #include "MediaInfo/Multiple/File_Swf.h"
#endif
#if defined(MEDIAINFO_WM_YES)
    #include "MediaInfo/Multiple/File_Wm.h"
#endif

//---------------------------------------------------------------------------
// Video
#if defined(MEDIAINFO_AVC_YES)
    #include "MediaInfo/Video/File_Avc.h"
#endif
#if defined(MEDIAINFO_DIRAC_YES)
    #include "MediaInfo/Video/File_Dirac.h"
#endif
#if defined(MEDIAINFO_FLIC_YES)
    #include "MediaInfo/Video/File_Flic.h"
#endif
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEGV_YES)
    #include "MediaInfo/Video/File_Mpegv.h"
#endif
#if defined(MEDIAINFO_VC1_YES)
    #include "MediaInfo/Video/File_Vc1.h"
#endif

//---------------------------------------------------------------------------
// Audio
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_ADIF_YES)
    #include "MediaInfo/Audio/File_Adif.h"
#endif
#if defined(MEDIAINFO_ADTS_YES)
    #include "MediaInfo/Audio/File_Adts.h"
#endif
#if defined(MEDIAINFO_AMR_YES)
    #include "MediaInfo/Audio/File_Amr.h"
#endif
#if defined(MEDIAINFO_AMV_YES)
    #include "MediaInfo/Audio/File_Amv.h"
#endif
#if defined(MEDIAINFO_APE_YES)
    #include "MediaInfo/Audio/File_Ape.h"
#endif
#if defined(MEDIAINFO_AU_YES)
    #include "MediaInfo/Audio/File_Au.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_FLAC_YES)
    #include "MediaInfo/Audio/File_Flac.h"
#endif
#if defined(MEDIAINFO_MIDI_YES)
    #include "MediaInfo/Audio/File_Midi.h"
#endif
#if defined(MEDIAINFO_MPC_YES)
    #include "MediaInfo/Audio/File_Mpc.h"
#endif
#if defined(MEDIAINFO_MPCSV8_YES)
    #include "MediaInfo/Audio/File_MpcSv8.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
#if defined(MEDIAINFO_PCM_YES)
    #include "MediaInfo/Audio/File_Pcm.h"
#endif
#if defined(MEDIAINFO_TTA_YES)
    #include "MediaInfo/Audio/File_Tta.h"
#endif
#if defined(MEDIAINFO_TWINVQ_YES)
    #include "MediaInfo/Audio/File_TwinVQ.h"
#endif
#if defined(MEDIAINFO_WVPK_YES)
    #include "MediaInfo/Audio/File_Wvpk.h"
#endif

//---------------------------------------------------------------------------
// Text
#if defined(MEDIAINFO_OTHERTEXT_YES)
    #include "MediaInfo/Text/File_OtherText.h"
#endif

//---------------------------------------------------------------------------
// Image
#if defined(MEDIAINFO_BMP_YES)
    #include "MediaInfo/Image/File_Bmp.h"
#endif
#if defined(MEDIAINFO_GIF_YES)
    #include "MediaInfo/Image/File_Gif.h"
#endif
#if defined(MEDIAINFO_ICO_YES)
    #include "MediaInfo/Image/File_Ico.h"
#endif
#if defined(MEDIAINFO_JPEG_YES)
    #include "MediaInfo/Image/File_Jpeg.h"
#endif
#if defined(MEDIAINFO_PNG_YES)
    #include "MediaInfo/Image/File_Png.h"
#endif
#if defined(MEDIAINFO_RLE_YES)
    #include "MediaInfo/Image/File_Rle.h"
#endif
#if defined(MEDIAINFO_TIFF_YES)
    #include "MediaInfo/Image/File_Tiff.h"
#endif

//---------------------------------------------------------------------------
// Archive
#if defined(MEDIAINFO_7Z_YES)
    #include "MediaInfo/Archive/File_7z.h"
#endif
#if defined(MEDIAINFO_ACE_YES)
    #include "MediaInfo/Archive/File_Ace.h"
#endif
#if defined(MEDIAINFO_BZIP2_YES)
    #include "MediaInfo/Archive/File_Bzip2.h"
#endif
#if defined(MEDIAINFO_GZIP_YES)
    #include "MediaInfo/Archive/File_Gzip.h"
#endif
#if defined(MEDIAINFO_RAR_YES)
    #include "MediaInfo/Archive/File_Rar.h"
#endif
#if defined(MEDIAINFO_TAR_YES)
    #include "MediaInfo/Archive/File_Tar.h"
#endif
#if defined(MEDIAINFO_ZIP_YES)
    #include "MediaInfo/Archive/File_Zip.h"
#endif

//---------------------------------------------------------------------------
// Other
#if !defined(MEDIAINFO_OTHER_NO)
    #include "MediaInfo/File_Other.h"
#endif
#if !defined(MEDIAINFO_UNKNOWN_NO)
    #include "MediaInfo/File_Unknown.h"
#endif
#if !defined(MEDIAINFO_DUMMY_NO)
    #include "MediaInfo/File_Dummy.h"
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Parser
//***************************************************************************

//---------------------------------------------------------------------------
File__Analyze* File__MultipleParsing::Parser_Get()
{
    if (Parser.empty())
        return NULL;

    File__Analyze* ToReturn=Parser[0]; //The first parser
    Parser.erase(Parser.begin());
    return ToReturn;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File__MultipleParsing::File__MultipleParsing()
:File__Analyze()
{
}

//---------------------------------------------------------------------------
File__MultipleParsing::~File__MultipleParsing()
{
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
        delete Parser[Pos]; //Parser[Pos]=NULL
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File__MultipleParsing::Read_Buffer_Continue()
{
    if (Parser.empty())
    {
        File__Analyze* Temp;
        // Multiple
        #if defined(MEDIAINFO_BDAV_YES)
            Temp=new File_Bdav(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_CDXA_YES)
            Temp=new File_Cdxa(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_DVDIF_YES)
            Temp=new File_DvDif(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_DVDV_YES)
            Temp=new File_Dvdv(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_FLV_YES)
          Temp=new File_Flv(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MK_YES)
          Temp=new File_Mk(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPEG4_YES)
          Temp=new File_Mpeg4(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPEGPS_YES)
            Temp=new File_MpegPs(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPEGTS_YES)
            Temp=new File_MpegTs(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MXF_YES)
          Temp=new File_Mxf(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_NUT_YES)
          Temp=new File_Nut(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_OGG_YES)
            Temp=new File_Ogg(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_RIFF_YES)
          Temp=new File_Riff(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_RM_YES)
          Temp=new File_Rm(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_SKM_YES)
          Temp=new File_Skm(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_SWF_YES)
          Temp=new File_Swf(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_WM_YES)
          Temp=new File_Wm(); Parser.push_back(Temp);
        #endif

        // Video
        #if defined(MEDIAINFO_AVC_YES)
            Temp=new File_Avc(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_DIRAC_YES)
          Temp=new File_Dirac(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_FLIC_YES)
          Temp=new File_Flic(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPEG4V_YES)
            Temp=new File_Mpeg4v(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPEGV_YES)
            Temp=new File_Mpegv(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_VC1_YES)
            Temp=new File_Vc1(); Parser.push_back(Temp);
        #endif

        // Audio
        #if defined(MEDIAINFO_AC3_YES)
            Temp=new File_Ac3(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_ADIF_YES)
            Temp=new File_Adif(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_ADTS_YES)
            Temp=new File_Adts(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_AMR_YES)
          Temp=new File_Amr(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_AMV_YES)
          Temp=new File_Amv(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_APE_YES)
          Temp=new File_Ape(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_AU_YES)
          Temp=new File_Au(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_DTS_YES)
            Temp=new File_Dts(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_FLAC_YES)
            Temp=new File_Flac(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MIDI_YES)
          Temp=new File_Midi(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPC_YES)
          Temp=new File_Mpc(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPCSV8_YES)
          Temp=new File_MpcSv8(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_MPEGA_YES)
          Temp=new File_Mpega(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_PCM_YES)
          //Temp=new File_Pcm(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_TTA_YES)
            Temp=new File_Tta(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_TWINVQ_YES)
          Temp=new File_TwinVQ(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_WVPK_YES)
            Temp=new File_Wvpk(); Parser.push_back(Temp);
        #endif

        // Text
        #if defined(MEDIAINFO_OTHERTEXT_YES)
          Temp=new File_OtherText(); Parser.push_back(Temp);
        #endif

        // Image
        #if defined(MEDIAINFO_BMP_YES)
          Temp=new File_Bmp(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_GIF_YES)
          Temp=new File_Gif(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_ICO_YES)
          Temp=new File_Ico(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_JPEG_YES)
            Temp=new File_Jpeg(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_PNG_YES)
          Temp=new File_Png(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_RLE_YES)
          //Temp=new File_Rle(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_TIFF_YES)
          Temp=new File_Tiff(); Parser.push_back(Temp);
        #endif

        // Archive
        #if defined(MEDIAINFO_ACE_YES)
          Temp=new File_Ace(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_7Z_YES)
          Temp=new File_7z(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_BZIP2_YES)
          Temp=new File_Bzip2(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_GZIP_YES)
          Temp=new File_Gzip(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_RAR_YES)
          Temp=new File_Rar(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_TAR_YES)
          Temp=new File_Tar(); Parser.push_back(Temp);
        #endif
        #if defined(MEDIAINFO_ZIP_YES)
          Temp=new File_Zip(); Parser.push_back(Temp);
        #endif
    }
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File__MultipleParsing::Header_Parse()
{
    //Filling
    Header_Fill_Size(Buffer_Size);
    Header_Fill_Code(0, "Block");
}

//---------------------------------------------------------------------------
void File__MultipleParsing::Data_Parse()
{
    //Parsing
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
    {
        //Parsing
        if (IsSub)
            Parser[Pos]->Init(Config, Details);
        else
            Parser[Pos]->Init(Config, Details, Stream, Stream_More);
        Open_Buffer_Init(Parser[Pos], File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Parser[Pos], Buffer+Buffer_Offset, (size_t)Element_Size);

        //Testing if the parser failed
        if (Parser[Pos]->Count_Get(Stream_General)==0 && (Parser[Pos]->File_GoTo==Parser[Pos]->File_Size || Parser[Pos]->IsFinnished))
        {
            delete Parser[Pos];
            Parser.erase(Parser.begin()+Pos);
            Pos--; //for the next position
        }
        else
        {
            //If Parser is found, erasing all the other parsers
            if (Parser.size()>1 && Parser[Pos]->Count_Get(Stream_General)>0)
            {
                if (IsSub)
                    Stream_Prepare(Stream_General);

                File__Analyze* Temp=Parser[Pos];
                for (size_t To_Delete_Pos=0; To_Delete_Pos<Parser.size(); To_Delete_Pos++)
                    if (To_Delete_Pos!=Pos)
                        delete Parser[To_Delete_Pos]; //Parser[Pos]=NULL
                Parser.clear();
                Parser.push_back(Temp);
                Pos=0;
            }

            //Positionning if requested
            if (Parser.size()==1 && Parser[0]->File_GoTo!=(int64u)-1)
               File_GoTo=Parser[0]->File_GoTo;

            //Ending if requested
            if (Parser.size()==1 && (Parser[Pos]->File_GoTo==Parser[Pos]->File_Size || Parser[Pos]->IsFinnished))
               Finnished();
        }
    }
}

} //NameSpace

/*
//---------------------------------------------------------------------------
void File__MultipleParsing::Data_Parse()
{
    //Parsing
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
    {
        //Parsing
        if (IsSub)
            Parser[Pos]->Init(Config, Details);
        else
            Parser[Pos]->Init(Config, Details, Stream, Stream_More);
        Open_Buffer_Init(Parser[Pos], File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Parser[Pos], Buffer+Buffer_Offset, (size_t)Element_Size);

        //Testing if the parser failed
        if (Parser[Pos]->Count_Get(Stream_General)==0 && (Parser[Pos]->File_GoTo==Parser[Pos]->File_Size || Parser[Pos]->IsFinnished))
        {
            delete Parser[Pos];
            Parser.erase(Parser.begin()+Pos);
            Pos--; //for the next position
        }
        else
        {
            //If Parser is found, erasing all the other parsers
            if (Parser.size()>1 && Parser[Pos]->Count_Get(Stream_General)>0)
            {
                File__Analyze* Temp=Parser[Pos];
                for (size_t To_Delete_Pos=0; To_Delete_Pos<Parser.size(); To_Delete_Pos++)
                    if (To_Delete_Pos!=Pos)
                        delete Parser[To_Delete_Pos]; //Parser[Pos]=NULL
                Parser.clear();
                Parser.push_back(Temp);
                Pos=0;
            }

            //Positionning if requested
            if (Parser.size()==1 && Parser[0]->File_GoTo!=(int64u)-1)
               File_GoTo=Parser[0]->File_GoTo;

            //Ending if requested
            if (Parser.size()==1 && (Parser[Pos]->File_GoTo==Parser[Pos]->File_Size || Parser[Pos]->IsFinnished))
               Finnished();
        }
    }
}

*/
