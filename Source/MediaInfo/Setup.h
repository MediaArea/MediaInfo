// Config - Config file for MediaInfo
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
// All compilation definitions
// Helpers for compilers (precompilation)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//***************************************************************************
// Precompilation
//***************************************************************************

//---------------------------------------------------------------------------
//Useful for precompiled headers
#ifdef ZENLIB_USEWX
    #ifndef __BORLANDC__ //Borland C++ does NOT support large files
        #ifndef _FILE_OFFSET_BITS
            #define _FILE_OFFSET_BITS 64
        #endif //_FILE_OFFSET_BITS
        #ifndef _LARGE_FILES
            #define _LARGE_FILES
        #endif //_LARGE_FILES
        #ifndef _LARGEFILE_SOURCE
            #define _LARGEFILE_SOURCE 1
        #endif //_LARGEFILE_SOURCE
    #endif //__BORLANDC__
    #ifdef __BORLANDC__
        #include <mem.h> //memcpy
    #endif //__BORLANDC__
    #include <wx/wxprec.h>
#else //ZENLIB_USEWX
    #if defined(__VISUALC__) || defined(__BORLANDC__)
        #if defined WINDOWS && !defined ZENLIB_STANDARD
            #undef __TEXT
            #include <windows.h>
        #endif //WINDOWS
        #include <iomanip>
        #include <algorithm>
        #include <cmath>
        #include <cstdio>
        #include <cstdlib>
        #include <ctime>
        #include <map>
        #include <string>
        #include <sstream>
        #include <vector>
    #endif //defined(__VISUALC__) || defined(__BORLANDC__)
#endif //ZENLIB_USEWX

//---------------------------------------------------------------------------
//Needed in the whole library
#include "ZenLib/Conf.h"

//***************************************************************************
// Compilation conditions
//***************************************************************************

//---------------------------------------------------------------------------
// All in one for no parsers
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_MULTI_NO)
    #define MEDIAINFO_MULTI_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_VIDEO_NO)
    #define MEDIAINFO_VIDEO_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_AUDIO_NO)
    #define MEDIAINFO_AUDIO_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_TEXT_NO)
    #define MEDIAINFO_TEXT_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_IMAGE_NO)
    #define MEDIAINFO_IMAGE_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_ARCHIVE_NO)
    #define MEDIAINFO_ARCHIVE_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_TAG_NO)
    #define MEDIAINFO_TAG_NO
#endif

//---------------------------------------------------------------------------
// Multiple
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_BDAV_NO) && !defined(MEDIAINFO_BDAV_YES)
    #define MEDIAINFO_BDAV_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_CDXA_NO) && !defined(MEDIAINFO_CDXA_YES)
    #define MEDIAINFO_CDXA_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_DVDIF_NO) && !defined(MEDIAINFO_DVDIF_YES)
    #define MEDIAINFO_DVDIF_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_DVDV_NO) && !defined(MEDIAINFO_DVDV_YES)
    #define MEDIAINFO_DVDV_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_FLV_NO) && !defined(MEDIAINFO_FLV_YES)
    #define MEDIAINFO_FLV_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_MK_NO) && !defined(MEDIAINFO_MK_YES)
    #define MEDIAINFO_MK_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_MPEG4_NO) && !defined(MEDIAINFO_MPEG4_YES)
    #define MEDIAINFO_MPEG4_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_MPEGPS_NO) && !defined(MEDIAINFO_MPEGPS_YES)
    #define MEDIAINFO_MPEGPS_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_MPEGTS_NO) && !defined(MEDIAINFO_MPEGTS_YES)
    #define MEDIAINFO_MPEGTS_YES
#endif
#if defined(MEDIAINFO_MPEGTS_YES) && !defined(MEDIAINFO_MPEGTS_PCR_NO) && !defined(MEDIAINFO_MPEGTS_PCR_YES)
    #define MEDIAINFO_MPEGTS_PCR_YES
#endif
#if defined(MEDIAINFO_MPEGTS_YES) && !defined(MEDIAINFO_MPEGTS_PESTIMESTAMP_NO) && !defined(MEDIAINFO_MPEGTS_PESTIMESTAMP_YES)
    #define MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_MXF_NO) && !defined(MEDIAINFO_MXF_YES)
    #define MEDIAINFO_MXF_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_NUT_NO) && !defined(MEDIAINFO_NUT_YES)
    #define MEDIAINFO_NUT_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_OGG_NO) && !defined(MEDIAINFO_OGG_YES)
    #define MEDIAINFO_OGG_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_RIFF_NO) && !defined(MEDIAINFO_RIFF_YES)
    #define MEDIAINFO_RIFF_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_RM_NO) && !defined(MEDIAINFO_RM_YES)
    #define MEDIAINFO_RM_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_SKM_NO) && !defined(MEDIAINFO_SKM_YES)
    #define MEDIAINFO_SKM_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_SWF_NO) && !defined(MEDIAINFO_SWF_YES)
    #define MEDIAINFO_SWF_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_TSP_NO) && !defined(MEDIAINFO_TSP_YES)
    #define MEDIAINFO_TSP_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_WM_NO) && !defined(MEDIAINFO_WM_YES)
    #define MEDIAINFO_WM_YES
#endif
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_DPG_NO) && !defined(MEDIAINFO_DPG_YES)
    #define MEDIAINFO_DPG_YES
#endif

//---------------------------------------------------------------------------
// Video
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_AVC_NO) && !defined(MEDIAINFO_AVC_YES)
    #define MEDIAINFO_AVC_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_DIRAC_NO) && !defined(MEDIAINFO_DIRAC_YES)
    #define MEDIAINFO_DIRAC_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_FLIC_NO) && !defined(MEDIAINFO_FLIC_YES)
    #define MEDIAINFO_FLIC_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_MPEG4V_NO) && !defined(MEDIAINFO_MPEG4V_YES)
    #define MEDIAINFO_MPEG4V_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_MPEGV_NO) && !defined(MEDIAINFO_MPEGV_YES)
    #define MEDIAINFO_MPEGV_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_VC1_NO) && !defined(MEDIAINFO_VC1_YES)
    #define MEDIAINFO_VC1_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_AVSV_NO) && !defined(MEDIAINFO_AVSV_YES)
    #define MEDIAINFO_AVSV_YES
#endif
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_THORA_NO) && !defined(MEDIAINFO_THEORA_YES)
    #define MEDIAINFO_THEORA_YES
#endif

//---------------------------------------------------------------------------
// Audio
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_AAC_NO) && !defined(MEDIAINFO_AAC_YES)
    #define MEDIAINFO_AAC_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_AC3_NO) && !defined(MEDIAINFO_AC3_YES)
    #define MEDIAINFO_AC3_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_ADIF_NO) && !defined(MEDIAINFO_ADIF_YES)
    #define MEDIAINFO_ADIF_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_ADPCM_NO) && !defined(MEDIAINFO_ADPCM_YES)
    #define MEDIAINFO_ADPCM_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_ADTS_NO) && !defined(MEDIAINFO_ADTS_YES)
    #define MEDIAINFO_ADTS_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_LATM_NO) && !defined(MEDIAINFO_LATM_YES)
    #define MEDIAINFO_LATM_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_AMR_NO) && !defined(MEDIAINFO_AMR_YES)
    #define MEDIAINFO_AMR_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_AMV_NO) && !defined(MEDIAINFO_AMV_YES)
    #define MEDIAINFO_AMV_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_APE_NO) && !defined(MEDIAINFO_APE_YES)
    #define MEDIAINFO_APE_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_AU_NO) && !defined(MEDIAINFO_AU_YES)
    #define MEDIAINFO_AU_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_DTS_NO) && !defined(MEDIAINFO_DTS_YES)
    #define MEDIAINFO_DTS_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_FLAC_NO) && !defined(MEDIAINFO_FLAC_YES)
    #define MEDIAINFO_FLAC_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_IT_NO) && !defined(MEDIAINFO_IT_YES)
    #define MEDIAINFO_IT_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_MIDO_NO) && !defined(MEDIAINFO_MIDO_YES)
    #define MEDIAINFO_MIDI_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_MOD_NO) && !defined(MEDIAINFO_MOD_YES)
    #define MEDIAINFO_MOD_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_MPC_NO) && !defined(MEDIAINFO_MPC_YES)
    #define MEDIAINFO_MPC_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_MPCSV8_NO) && !defined(MEDIAINFO_MPCSV8_YES)
    #define MEDIAINFO_MPCSV8_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_MPEGA_NO) && !defined(MEDIAINFO_MPEGA_YES)
    #define MEDIAINFO_MPEGA_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_PCM_NO) && !defined(MEDIAINFO_PCM_YES)
    #define MEDIAINFO_PCM_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_S3M_NO) && !defined(MEDIAINFO_S3M_YES)
    #define MEDIAINFO_S3M_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_TTA_NO) && !defined(MEDIAINFO_TTA_YES)
    #define MEDIAINFO_TTA_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_TWINVQ_NO) && !defined(MEDIAINFO_TWINVQ_YES)
    #define MEDIAINFO_TWINVQ_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_VORBIS_NO) && !defined(MEDIAINFO_VORBIS_YES)
    #define MEDIAINFO_VORBIS_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_WVPK_NO) && !defined(MEDIAINFO_WVPK_YES)
    #define MEDIAINFO_WVPK_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_XM_NO) && !defined(MEDIAINFO_XM_YES)
    #define MEDIAINFO_XM_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_AES3_NO) && !defined(MEDIAINFO_AES3_YES)
    #define MEDIAINFO_AES3_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_SPEEX_NO) && !defined(MEDIAINFO_SPEEX_YES)
    #define MEDIAINFO_SPEEX_YES
#endif
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_PS2A_NO) && !defined(MEDIAINFO_PS2A_YES)
    #define MEDIAINFO_PS2A_YES
#endif

//---------------------------------------------------------------------------
// Text
#if !defined(MEDIAINFO_TEXT_NO) && !defined(MEDIAINFO_CMML_NO) && !defined(MEDIAINFO_CMML_YES)
    #define MEDIAINFO_CMML_YES
#endif
#if !defined(MEDIAINFO_TEXT_NO) && !defined(MEDIAINFO_KATE_NO) && !defined(MEDIAINFO_KATE_YES)
    #define MEDIAINFO_KATE_YES
#endif
#if !defined(MEDIAINFO_TEXT_NO) && !defined(MEDIAINFO_PGS_NO) && !defined(MEDIAINFO_PGS_YES)
    #define MEDIAINFO_PGS_YES
#endif
#if !defined(MEDIAINFO_TEXT_NO) && !defined(MEDIAINFO_OTHERTEXT_NO) && !defined(MEDIAINFO_OTHERTEXT_YES)
    #define MEDIAINFO_OTHERTEXT_YES
#endif

//---------------------------------------------------------------------------
// Image
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_BMP_NO) && !defined(MEDIAINFO_BMP_YES)
    #define MEDIAINFO_BMP_YES
#endif
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_GIF_NO) && !defined(MEDIAINFO_GIF_YES)
    #define MEDIAINFO_GIF_YES
#endif
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_ICO_NO) && !defined(MEDIAINFO_ICO_YES)
    #define MEDIAINFO_ICO_YES
#endif
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_JPEG_NO) && !defined(MEDIAINFO_JPEG_YES)
    #define MEDIAINFO_JPEG_YES
#endif
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_PNG_NO) && !defined(MEDIAINFO_PNG_YES)
    #define MEDIAINFO_PNG_YES
#endif
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_RLE_NO) && !defined(MEDIAINFO_RLE_YES)
    #define MEDIAINFO_RLE_YES
#endif
#if !defined(MEDIAINFO_IMAGE_NO) && !defined(MEDIAINFO_TIFF_NO) && !defined(MEDIAINFO_TIFF_YES)
    #define MEDIAINFO_TIFF_YES
#endif

//---------------------------------------------------------------------------
// Archive
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_7Z_NO) && !defined(MEDIAINFO_7Z_YES)
    #define MEDIAINFO_7Z_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_ACE_NO) && !defined(MEDIAINFO_ACE_YES)
    #define MEDIAINFO_ACE_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_BZIP2_NO) && !defined(MEDIAINFO_BZIP2_YES)
    #define MEDIAINFO_BZIP2_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_ELF_NO) && !defined(MEDIAINFO_ELF_YES)
    #define MEDIAINFO_ELF_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_GZIP_NO) && !defined(MEDIAINFO_GZIP_YES)
    #define MEDIAINFO_GZIP_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_MZ_NO) && !defined(MEDIAINFO_MZ_YES)
    #define MEDIAINFO_MZ_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_RAR_NO) && !defined(MEDIAINFO_RAR_YES)
    #define MEDIAINFO_RAR_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_TAR_NO) && !defined(MEDIAINFO_TAR_YES)
    #define MEDIAINFO_TAR_YES
#endif
#if !defined(MEDIAINFO_ARCHIVE_NO) && !defined(MEDIAINFO_ZIP_NO) && !defined(MEDIAINFO_ZIP_YES)
    #define MEDIAINFO_ZIP_YES
#endif

//---------------------------------------------------------------------------
// Tag
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_TAG_YES)
    #define MEDIAINFO_TAG_YES
#endif
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_APETAG_NO) && !defined(MEDIAINFO_APETAG_YES)
    #define MEDIAINFO_APETAG_YES
#endif
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_ID3_NO) && !defined(MEDIAINFO_ID3_YES)
    #define MEDIAINFO_ID3_YES
#endif
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_ID3V2_NO) && !defined(MEDIAINFO_ID3V2_YES)
    #define MEDIAINFO_ID3V2_YES
#endif
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_LYRICS3_NO) && !defined(MEDIAINFO_LYRICS3_YES)
    #define MEDIAINFO_LYRICS3_YES
#endif
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_LYRICS3V2_NO) && !defined(MEDIAINFO_LYRICS3V2_YES)
    #define MEDIAINFO_LYRICS3V2_YES
#endif
#if !defined(MEDIAINFO_TAG_NO) && !defined(MEDIAINFO_VORBISCOM_NO) && !defined(MEDIAINFO_VORBISCOM_YES)
    #define MEDIAINFO_VORBISCOM_YES
#endif

//---------------------------------------------------------------------------
// Other
#if !defined(MEDIAINFO_DUMMY_NO)
    //#define MEDIAINFO_DUMMY_YES
#endif
#if !defined(MEDIAINFO_OTHER_NO)
    #define MEDIAINFO_OTHER_YES
#endif
#if !defined(MEDIAINFO_UNKNOWN_NO)
    #define MEDIAINFO_UNKNOWN_YES
#endif
//---------------------------------------------------------------------------

