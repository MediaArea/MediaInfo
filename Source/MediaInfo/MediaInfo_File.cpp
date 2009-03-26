// MediaInfo_Internal - All info about media files, different parser listing part
// Copyright (C) 2006-2009 Jerome Martinez, Zen@MediaArea.net
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
//
// How to:
// To add a new format,
// Fill includes, SelectFromExtension, ListFormats and LibraryIsModified
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Internal.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Multiple
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
#if defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_BDAV_YES) || defined(MEDIAINFO_TSP_YES)
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
#if defined(MEDIAINFO_DPG_YES)
    #include "MediaInfo/Multiple/File_Dpg.h"
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
#if defined(MEDIAINFO_AVSV_YES)
    #include "MediaInfo/Video/File_AvsV.h"
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
#if defined(MEDIAINFO_IT_YES)
    #include "MediaInfo/Audio/File_ImpulseTracker.h"
#endif
#if defined(MEDIAINFO_MIDI_YES)
    #include "MediaInfo/Audio/File_Midi.h"
#endif
#if defined(MEDIAINFO_MOD_YES)
    #include "MediaInfo/Audio/File_Module.h"
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
#if defined(MEDIAINFO_S3M_YES)
    #include "MediaInfo/Audio/File_ScreamTracker3.h"
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
#if defined(MEDIAINFO_XM_YES)
    #include "MediaInfo/Audio/File_ExtendedModule.h"
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
#if defined(MEDIAINFO_ELF_YES)
    #include "MediaInfo/Archive/File_Elf.h"
#endif
#if defined(MEDIAINFO_GZIP_YES)
    #include "MediaInfo/Archive/File_Gzip.h"
#endif
#if defined(MEDIAINFO_MZ_YES)
    #include "MediaInfo/Archive/File_Mz.h"
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
#if defined(MEDIAINFO_OTHER_YES)
    #include "MediaInfo/File_Other.h"
#endif
#if defined(MEDIAINFO_UNKNOWN_YES)
    #include "MediaInfo/File_Unknown.h"
#endif
#if defined(MEDIAINFO_DUMMY_YES)
    #include "MediaInfo/File_Dummy.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MediaInfo_Internal::SelectFromExtension (const String &Parser)
{
    //Clear last value
    delete Info; Info=NULL;

    //Searching the right File_*
             if (0) {} //For #defines

    // Multiple
    #if defined(MEDIAINFO_BDAV_YES)
        else if (Parser==_T("Bdav"))       {Info=new File_MpegTs(); ((File_MpegTs*)Info)->BDAV_Size=4;}
    #endif
    #if defined(MEDIAINFO_CDXA_YES)
        else if (Parser==_T("Cdxa"))        Info=new File_Cdxa();
    #endif
    #if defined(MEDIAINFO_DVDIF_YES)
        else if (Parser==_T("DvDif"))        Info=new File_DvDif();
    #endif
    #if defined(MEDIAINFO_DVDV_YES)
        else if (Parser==_T("Dvdv"))        Info=new File_Dvdv();
    #endif
    #if defined(MEDIAINFO_FLV_YES)
        else if (Parser==_T("Flv"))         Info=new File_Flv();
    #endif
    #if defined(MEDIAINFO_MK_YES)
        else if (Parser==_T("Mk"))          Info=new File_Mk();
    #endif
    #if defined(MEDIAINFO_MPEG4_YES)
        else if (Parser==_T("Mpeg4"))       Info=new File_Mpeg4();
    #endif
    #if defined(MEDIAINFO_MPEGPS_YES)
        else if (Parser==_T("MpegPs"))      Info=new File_MpegPs();
    #endif
    #if defined(MEDIAINFO_MPEGTS_YES)
        else if (Parser==_T("MpegTs"))      Info=new File_MpegTs();
    #endif
    #if defined(MEDIAINFO_MXF_YES)
        else if (Parser==_T("Mxf"))         Info=new File_Mxf();
    #endif
    #if defined(MEDIAINFO_NUT_YES)
        else if (Parser==_T("Nut"))         Info=new File_Nut();
    #endif
    #if defined(MEDIAINFO_OGG_YES)
        else if (Parser==_T("Ogg"))         Info=new File_Ogg();
    #endif
    #if defined(MEDIAINFO_RIFF_YES)
        else if (Parser==_T("Riff"))        Info=new File_Riff();
    #endif
    #if defined(MEDIAINFO_RM_YES)
        else if (Parser==_T("Rm"))          Info=new File_Rm();
    #endif
    #if defined(MEDIAINFO_SKM_YES)
        else if (Parser==_T("Skm"))         Info=new File_Skm();
    #endif
    #if defined(MEDIAINFO_SWF_YES)
        else if (Parser==_T("Swf"))         Info=new File_Swf();
    #endif
    #if defined(MEDIAINFO_WM_YES)
        else if (Parser==_T("Wm"))          Info=new File_Wm();
    #endif
    #if defined(MEDIAINFO_DPG_YES)
        else if (Parser==_T("Dpg"))         Info=new File_Dpg();
    #endif

    // Video
    #if defined(MEDIAINFO_AVC_YES)
        else if (Parser==_T("Avc"))         Info=new File_Avc();
    #endif
    #if defined(MEDIAINFO_DIRAC_YES)
        else if (Parser==_T("Dirac"))       Info=new File_Dirac();
    #endif
    #if defined(MEDIAINFO_FLIC_YES)
        else if (Parser==_T("Flic"))        Info=new File_Flic();
    #endif
    #if defined(MEDIAINFO_MPEG4V_YES)
        else if (Parser==_T("Mpeg4v"))      Info=new File_Mpeg4v();
    #endif
    #if defined(MEDIAINFO_MPEGV_YES)
        else if (Parser==_T("Mpegv"))       Info=new File_Mpegv();
    #endif
    #if defined(MEDIAINFO_VC1_YES)
        else if (Parser==_T("Vc1"))         Info=new File_Vc1();
    #endif
    #if defined(MEDIAINFO_AVSV_YES)
        else if (Parser==_T("AvsV"))        Info=new File_AvsV();
    #endif

    // Audio
    #if defined(MEDIAINFO_AC3_YES)
        else if (Parser==_T("Ac3"))         Info=new File_Ac3();
    #endif
    #if defined(MEDIAINFO_ADTS_YES)
        else if (Parser==_T("Aac"))         Info=new File_Adts();
    #endif
    #if defined(MEDIAINFO_AMR_YES)
        else if (Parser==_T("Amr"))         Info=new File_Amr();
    #endif
    #if defined(MEDIAINFO_AMV_YES)
        else if (Parser==_T("Amv"))         Info=new File_Amv();
    #endif
    #if defined(MEDIAINFO_APE_YES)
        else if (Parser==_T("Ape"))         Info=new File_Ape();
    #endif
    #if defined(MEDIAINFO_AU_YES)
        else if (Parser==_T("Au"))          Info=new File_Au();
    #endif
    #if defined(MEDIAINFO_DTS_YES)
        else if (Parser==_T("Dts"))         Info=new File_Dts();
    #endif
    #if defined(MEDIAINFO_FLAC_YES)
        else if (Parser==_T("Flac"))        Info=new File_Flac();
    #endif
    #if defined(MEDIAINFO_MIDI_YES)
        else if (Parser==_T("Midi"))        Info=new File_Midi();
    #endif
    #if defined(MEDIAINFO_MPC_YES)
        else if (Parser==_T("Mpc"))         Info=new File_Mpc();
    #endif
    #if defined(MEDIAINFO_MPCSV8_YES)
        else if (Parser==_T("Mpc"))         Info=new File_MpcSv8();
    #endif
    #if defined(MEDIAINFO_MPEGA_YES)
        else if (Parser==_T("Mpega"))       Info=new File_Mpega();
    #endif
    #if defined(MEDIAINFO_PCM_YES)
        else if (Parser==_T("Pcm"))         Info=new File_Pcm();
    #endif
    #if defined(MEDIAINFO_AU_YES)
        else if (Parser==_T("Au"))          Info=new File_Au();
    #endif
    #if defined(MEDIAINFO_TTA_YES)
        else if (Parser==_T("Tta"))         Info=new File_Tta();
    #endif
    #if defined(MEDIAINFO_TWINVQ_YES)
        else if (Parser==_T("TwinVQ"))      Info=new File_TwinVQ();
    #endif
    #if defined(MEDIAINFO_WVPK_YES)
        else if (Parser==_T("Wvpk"))        Info=new File_Wvpk();
    #endif
    #if defined(MEDIAINFO_XM_YES)
        else if (Parser==_T("Xm"))          Info=new File_ExtendedModule();
    #endif
    #if defined(MEDIAINFO_MOD_YES)
        else if (Parser==_T("Mod"))         Info=new File_Module();
    #endif
    #if defined(MEDIAINFO_S3M_YES)
        else if (Parser==_T("S3m"))         Info=new File_ScreamTracker3();
    #endif
    #if defined(MEDIAINFO_IT_YES)
        else if (Parser==_T("It"))          Info=new File_ImpulseTracker();
    #endif

    // Text
    #if defined(MEDIAINFO_OTHERTEXT_YES)
        else if (Parser==_T("OtherText"))   Info=new File_OtherText();
    #endif

    // Image
    #if defined(MEDIAINFO_GIF_YES)
        else if (Parser==_T("Gif"))         Info=new File_Gif();
    #endif
    #if defined(MEDIAINFO_BMP_YES)
        else if (Parser==_T("Bmp"))         Info=new File_Bmp();
    #endif
    #if defined(MEDIAINFO_ICO_YES)
        else if (Parser==_T("Ico"))         Info=new File_Ico();
    #endif
    #if defined(MEDIAINFO_JPEG_YES)
        else if (Parser==_T("Jpeg"))        Info=new File_Jpeg();
    #endif
    #if defined(MEDIAINFO_PNG_YES)
        else if (Parser==_T("Png"))         Info=new File_Png();
    #endif
    #if defined(MEDIAINFO_TIFF_YES)
        else if (Parser==_T("Tiff"))        Info=new File_Tiff();
    #endif

    // Archive
    #if defined(MEDIAINFO_7Z_YES)
        else if (Parser==_T("7z"))          Info=new File_7z();
    #endif
    #if defined(MEDIAINFO_ACE_YES)
        else if (Parser==_T("Ace"))         Info=new File_Ace();
    #endif
    #if defined(MEDIAINFO_BZIP2_YES)
        else if (Parser==_T("Bzip2"))       Info=new File_Bzip2();
    #endif
    #if defined(MEDIAINFO_ELF_YES)
        else if (Parser==_T("Elf"))         Info=new File_Elf();
    #endif
    #if defined(MEDIAINFO_GZIP_YES)
        else if (Parser==_T("Gzip"))        Info=new File_Gzip();
    #endif
    #if defined(MEDIAINFO_MZ_YES)
        else if (Parser==_T("Mz"))          Info=new File_Mz();
    #endif
    #if defined(MEDIAINFO_RAR_YES)
        else if (Parser==_T("Rar"))         Info=new File_Rar();
    #endif
    #if defined(MEDIAINFO_TAR_YES)
        else if (Parser==_T("Tar"))         Info=new File_Tar();
    #endif
    #if defined(MEDIAINFO_ZIP_YES)
        else if (Parser==_T("Zip"))         Info=new File_Zip();
    #endif

    // Other
    #if !defined(MEDIAINFO_OTHER_NO)
        else if (Parser==_T("Other"))       Info=new File_Other();
    #endif
}

//---------------------------------------------------------------------------
int MediaInfo_Internal::ListFormats()
{
    delete Info; Info=NULL;

    // Multiple
    #if defined(MEDIAINFO_BDAV_YES)
        delete Info; Info=new File_MpegTs(); ((File_MpegTs*)Info)->BDAV_Size=4; if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_CDXA_YES)
        delete Info; Info=new File_Cdxa();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_DVDIF_YES)
        delete Info; Info=new File_DvDif();              if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_DVDV_YES)
        delete Info; Info=new File_Dvdv();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_FLV_YES)
        delete Info; Info=new File_Flv();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MK_YES)
        delete Info; Info=new File_Mk();                 if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPEG4_YES)
        delete Info; Info=new File_Mpeg4();              if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPEGPS_YES)
        delete Info; Info=new File_MpegPs();             if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPEGTS_YES)
        delete Info; Info=new File_MpegTs();             if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MXF_YES)
        delete Info; Info=new File_Mxf();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_NUT_YES)
        delete Info; Info=new File_Nut();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_OGG_YES)
        delete Info; Info=new File_Ogg();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_RIFF_YES)
        delete Info; Info=new File_Riff();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_RM_YES)
        delete Info; Info=new File_Rm();                 if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_SKM_YES)
        delete Info; Info=new File_Skm();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_SWF_YES)
        delete Info; Info=new File_Swf();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_TSP_YES)
        delete Info; Info=new File_MpegTs(); ((File_MpegTs*)Info)->TSP_Size=16; if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_WM_YES)
        delete Info; Info=new File_Wm();                 if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_DPG_YES)
        delete Info; Info=new File_Dpg();                if (ApplyMethod()>0) return 1;
    #endif

    // Video
    #if defined(MEDIAINFO_AVC_YES)
        delete Info; Info=new File_Avc();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_DIRAC_YES)
        delete Info; Info=new File_Dirac();              if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_FLIC_YES)
        delete Info; Info=new File_Flic();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPEG4V_YES)
        delete Info; Info=new File_Mpeg4v();             if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPEGV_YES)
        delete Info; Info=new File_Mpegv();              if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_VC1_YES)
        delete Info; Info=new File_Vc1();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_AVSV_YES)
        delete Info; Info=new File_AvsV();               if (ApplyMethod()>0) return 1;
    #endif

    // Audio
    #if defined(MEDIAINFO_AC3_YES)
        delete Info; Info=new File_Ac3();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_ADIF_YES)
        delete Info; Info=new File_Adif();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_ADTS_YES)
        delete Info; Info=new File_Adts();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_AMR_YES)
        delete Info; Info=new File_Amr();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_AMV_YES)
        delete Info; Info=new File_Amv();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_APE_YES)
        delete Info; Info=new File_Ape();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_AU_YES)
        delete Info; Info=new File_Au();                 if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_DTS_YES)
        delete Info; Info=new File_Dts();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_FLAC_YES)
        delete Info; Info=new File_Flac();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MIDI_YES)
        delete Info; Info=new File_Midi();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPC_YES)
        delete Info; Info=new File_Mpc();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPCSV8_YES)
        delete Info; Info=new File_MpcSv8();             if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MPEGA_YES)
        delete Info; Info=new File_Mpega();              if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_PCM_YES)
      //delete Info; Info=new File_Pcm();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_TTA_YES)
        delete Info; Info=new File_Tta();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_TWINVQ_YES)
        delete Info; Info=new File_TwinVQ();             if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_WVPK_YES)
        delete Info; Info=new File_Wvpk();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_XM_YES)
        delete Info; Info=new File_ExtendedModule();     if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MOD_YES)
        delete Info; Info=new File_Module();             if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_S3M_YES)
        delete Info; Info=new File_ScreamTracker3();      if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_IT_YES)
        delete Info; Info=new File_ImpulseTracker();     if (ApplyMethod()>0) return 1;
    #endif

    // Text
    #if defined(MEDIAINFO_OTHERTEXT_YES)
        delete Info; Info=new File_OtherText();          if (ApplyMethod()>0) return 1;
    #endif

    // Image
    #if defined(MEDIAINFO_BMP_YES)
        delete Info; Info=new File_Bmp();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_GIF_YES)
        delete Info; Info=new File_Gif();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_ICO_YES)
        delete Info; Info=new File_Ico();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_JPEG_YES)
        delete Info; Info=new File_Jpeg();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_PNG_YES)
        delete Info; Info=new File_Png();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_TIFF_YES)
        delete Info; Info=new File_Tiff();               if (ApplyMethod()>0) return 1;
    #endif

    // Archive
    #if defined(MEDIAINFO_ACE_YES)
        delete Info; Info=new File_Ace();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_7Z_YES)
        delete Info; Info=new File_7z();                 if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_BZIP2_YES)
        delete Info; Info=new File_Bzip2();              if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_ELF_YES)
        delete Info; Info=new File_Elf();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_GZIP_YES)
        delete Info; Info=new File_Gzip();               if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_MZ_YES)
        delete Info; Info=new File_Mz();                 if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_RAR_YES)
        delete Info; Info=new File_Rar();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_TAR_YES)
        delete Info; Info=new File_Tar();                if (ApplyMethod()>0) return 1;
    #endif
    #if defined(MEDIAINFO_ZIP_YES)
        delete Info; Info=new File_Zip();                if (ApplyMethod()>0) return 1;
    #endif

    // Other
    #if !defined(MEDIAINFO_OTHER_NO)
    if (InternalMethod==1 || InternalMethod==2)
    {
        delete Info; Info=new File_Other();              if (ApplyMethod()>0) return 1;
    }
    #endif
    #if !defined(MEDIAINFO_UNKNOWN_NO)
    if (InternalMethod==1)
    {
        delete Info; Info=new File_Unknown();            if (ApplyMethod()>0) return 1;
    }
    #endif
    return 0;
}

//---------------------------------------------------------------------------
bool MediaInfo_Internal::LibraryIsModified ()
{
    #if defined(MEDIAINFO_MULTI_NO) || defined(MEDIAINFO_VIDEO_NO) || defined(MEDIAINFO_AUDIO_NO) || defined(MEDIAINFO_TEXT_NO) || defined(MEDIAINFO_IMAGE_NO) || defined(MEDIAINFO_ARCHIVE_NO) \
     || defined(MEDIAINFO_BDAV_NO) || defined(MEDIAINFO_MK_NO) || defined(MEDIAINFO_OGG_NO) || defined(MEDIAINFO_RIFF_NO) || defined(MEDIAINFO_MPEG4_NO) || defined(MEDIAINFO_MPEGPS_NO) || defined(MEDIAINFO_MPEGTS_NO) || defined(MEDIAINFO_FLV_NO) || defined(MEDIAINFO_SWF_NO) || defined(MEDIAINFO_MXF_NO) || defined(MEDIAINFO_NUT_NO) || defined(MEDIAINFO_WM_NO) || defined(MEDIAINFO_QT_NO) || defined(MEDIAINFO_RM_NO) || defined(MEDIAINFO_DVDIF_NO) || defined(MEDIAINFO_DVDV_NO) || defined(MEDIAINFO_CDXA_NO) || defined(MEDIAINFO_DPG_NO) || defined(MEDIAINFO_TSP_NO) \
     || defined(MEDIAINFO_AVC_NO) || defined(MEDIAINFO_MPEG4V_NO) || defined(MEDIAINFO_MPEGV_NO) || defined(MEDIAINFO_FLIC_NO) || defined(MEDIAINFO_THEORA_NO) \
     || defined(MEDIAINFO_AC3_NO) || defined(MEDIAINFO_ADIF_NO) || defined(MEDIAINFO_ADTS_NO) || defined(MEDIAINFO_AMR_NO) || defined(MEDIAINFO_DTS_NO) || defined(MEDIAINFO_FLAC_NO) || defined(MEDIAINFO_APE_NO) || defined(MEDIAINFO_MPC_NO) || defined(MEDIAINFO_MPCSV8_NO) || defined(MEDIAINFO_MPEGA_NO) || defined(MEDIAINFO_TWINVQ_NO) || defined(MEDIAINFO_XM_NO) || defined(MEDIAINFO_MOD_NO) || defined(MEDIAINFO_S3M_NO) || defined(MEDIAINFO_IT_NO) || defined(MEDIAINFO_AES3_NO) || defined(MEDIAINFO_SPEEX_NO) || defined(MEDIAINFO_PS2A_NO) \
     || defined(MEDIAINFO_CMML_NO)  || defined(MEDIAINFO_KATE_NO)  || defined(MEDIAINFO_PGS_NO) || defined(MEDIAINFO_OTHERTEXT_NO) \
     || defined(MEDIAINFO_PNG_NO) || defined(MEDIAINFO_JPEG_NO) || defined(MEDIAINFO_BMP_NO) || defined(MEDIAINFO_ICO_NO) || defined(MEDIAINFO_GIF_NO) || defined(MEDIAINFO_TIFF_NO) \
     || defined(MEDIAINFO_7Z_NO) || defined(MEDIAINFO_ZIP_NO) || defined(MEDIAINFO_RAR_NO) || defined(MEDIAINFO_ACE_NO) || defined(MEDIAINFO_ELF_NO) || defined(MEDIAINFO_MZ_NO) \
     || defined(MEDIAINFO_OTHER_NO) || defined(MEDIAINFO_DUMMY_NO)
        return true;
    #else
        return false;
    #endif
}

//---------------------------------------------------------------------------
void MediaInfo_Internal::CreateDummy (const String& Value)
{
    #if defined(MEDIAINFO_DUMMY_YES)
        Info=new File_Dummy();
        ((File_Dummy*)Info)->KindOfDummy=Value;
    #endif
}

} //NameSpace


