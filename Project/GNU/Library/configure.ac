dnl #########################################################################
dnl ### Init
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Test if we are at the good place
dnl
AC_INIT(../../../Source/MediaInfo/MediaInfo.cpp)

dnl -------------------------------------------------------------------------
dnl sets build, host, target variables and the same with _alias
dnl
AC_CANONICAL_BUILD

dnl -------------------------------------------------------------------------
dnl Name and version
dnl
AM_INIT_AUTOMAKE(libmediainfo,0)

dnl -------------------------------------------------------------------------
dnl Autotools init
dnl
CFLAGS="$CFLAGS"
CXXFLAGS="$CXXFLAGS"
AC_PROG_CC
AC_PROG_CXX
AC_DISABLE_SHARED
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_PROG_INSTALL

dnl #########################################################################
dnl Configure
dnl #########################################################################

Home=`pwd`

dnl #########################################################################
dnl ### Options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Enable
dnl
AC_ARG_ENABLE(debug,   AC_HELP_STRING([--enable-debug],    [Enable full debug]),                                          ,                            enable_debug=no)
AC_ARG_ENABLE(gprof,   AC_HELP_STRING([--enable-gprof],    [Enable gprof log]),                                           ,                            enable_gprof=no)
AC_ARG_ENABLE(minimize-size,   AC_HELP_STRING([--enable-minimize-size], [Enable size optimisations]),                     MediaInfoMinimizeSize=$enableval, MediaInfoMinimizeSize=depend)
AC_ARG_ENABLE(all,     AC_HELP_STRING([--disable-all],     [Disable all parsers]),                                        MediaInfoAll=$enableval,     MediaInfoAll=depend)
AC_ARG_ENABLE(archive, AC_HELP_STRING([--disable-archive], [Disable all archive parsers]),                                MediaInfoArchive=$enableval, MediaInfoArchive=depend)
AC_ARG_ENABLE(audio,   AC_HELP_STRING([--disable-audio],   [Disable all audio parsers]),                                  MediaInfoAudio=$enableval,   MediaInfoAudio=depend)
AC_ARG_ENABLE(image,   AC_HELP_STRING([--disable-image],   [Disable all image parsers]),                                  MediaInfoImage=$enableval,   MediaInfoImage=depend)
AC_ARG_ENABLE(multi,   AC_HELP_STRING([--disable-multi],   [Disable all multiple parsers]),                               MediaInfoMulti=$enableval,   MediaInfoMulti=depend)
AC_ARG_ENABLE(text,    AC_HELP_STRING([--disable-text],    [Disable all text parsers]),                                   MediaInfoText=$enableval,    MediaInfoText=depend)
AC_ARG_ENABLE(video,   AC_HELP_STRING([--disable-video],   [Disable all video parsers]),                                  MediaInfoVideo=$enableval,   MediaInfoVideo=depend)
AC_ARG_ENABLE(7z,      AC_HELP_STRING([--disable-7z],      [Disable Archive - 7z]),                                       MediaInfo7z=$enableval,      MediaInfo7z=depend)
AC_ARG_ENABLE(ace,     AC_HELP_STRING([--disable-ace],     [Disable Archive - Ace]),                                      MediaInfoAce=$enableval,     MediaInfoAce=depend)
AC_ARG_ENABLE(bzip2,   AC_HELP_STRING([--disable-bzip2],   [Disable Archive - Bzip2]),                                    MediaInfoBzip2=$enableval,   MediaInfoBzip2=depend)
AC_ARG_ENABLE(gzip,    AC_HELP_STRING([--disable-gzip],    [Disable Archive - GZip]),                                     MediaInfoGzip=$enableval,    MediaInfoGzip=depend)
AC_ARG_ENABLE(rar,     AC_HELP_STRING([--disable-rar],     [Disable Archive - Rar]),                                      MediaInfoRar=$enableval,     MediaInfoRar=depend)
AC_ARG_ENABLE(tar,     AC_HELP_STRING([--disable-tar],     [Disable Archive - Tar]),                                      MediaInfoTar=$enableval,     MediaInfoTar=depend)
AC_ARG_ENABLE(zip,     AC_HELP_STRING([--disable-zip],     [Disable Archive - Zip]),                                      MediaInfoZip=$enableval,     MediaInfoZip=depend)
AC_ARG_ENABLE(aac,     AC_HELP_STRING([--disable-aac],     [Disable Audio - AAC (raw)]),                                  MediaInfoAac=$enableval,     MediaInfoAac=depend)
AC_ARG_ENABLE(ac3,     AC_HELP_STRING([--disable-ac3],     [Disable Audio - AC-3]),                                       MediaInfoAc3=$enableval,     MediaInfoAc3=depend)
AC_ARG_ENABLE(adif,    AC_HELP_STRING([--disable-adif],    [Disable Audio - ADIF]),                                       MediaInfoAdif=$enableval,    MediaInfoAdif=depend)
AC_ARG_ENABLE(adpcm,   AC_HELP_STRING([--disable-adpcm],   [Disable Audio - ADPCM]),                                      MediaInfoAdpcm=$enableval,   MediaInfoAdpcm=depend)
AC_ARG_ENABLE(adts,    AC_HELP_STRING([--disable-adts],    [Disable Audio - ADTS]),                                       MediaInfoAdts=$enableval,    MediaInfoAdts=depend)
AC_ARG_ENABLE(aes3,    AC_HELP_STRING([--disable-aes3],    [Disable Audio - AES3]),                                       MediaInfoAdts=$enableval,    MediaInfoAes3=depend)
AC_ARG_ENABLE(latm,    AC_HELP_STRING([--disable-latm],    [Disable Audio - LATM]),                                       MediaInfoAdts=$enableval,    MediaInfoAdts=depend)
AC_ARG_ENABLE(amr,     AC_HELP_STRING([--disable-amr],     [Disable Audio - AMR]),                                        MediaInfoAmr=$enableval,     MediaInfoAmr=depend)
AC_ARG_ENABLE(amv,     AC_HELP_STRING([--disable-amv],     [Disable Audio - AMV]),                                        MediaInfoAmv=$enableval,     MediaInfoAmv=depend)
AC_ARG_ENABLE(ape,     AC_HELP_STRING([--disable-ape],     [Disable Audio - Monkey Audio (.ape, .mac)]),                  MediaInfoApe=$enableval,     MediaInfoApe=depend)
AC_ARG_ENABLE(au,      AC_HELP_STRING([--disable-au],      [Disable Audio - Au]),                                         MediaInfoAu=$enableval,      MediaInfoAu=depend)
AC_ARG_ENABLE(dts,     AC_HELP_STRING([--disable-dts],     [Disable Audio - DTS]),                                        MediaInfoDts=$enableval,     MediaInfoDts=depend)
AC_ARG_ENABLE(flac,    AC_HELP_STRING([--disable-flac],    [Disable Audio - FLAC format]),                                MediaInfoFlac=$enableval,    MediaInfoFlac=depend)
AC_ARG_ENABLE(midi,    AC_HELP_STRING([--disable-midi],    [Disable Audio - MIDI]),                                       MediaInfoMidi=$enableval,    MediaInfoMidi=depend)
AC_ARG_ENABLE(mpc,     AC_HELP_STRING([--disable-mpc],     [Disable Audio - Musepack format (.mpc)]),                     MediaInfoMpc=$enableval,     MediaInfoMpc=depend)
AC_ARG_ENABLE(mpcsv8,  AC_HELP_STRING([--disable-mpc],     [Disable Audio - Musepack format SV8 (.mpc)]),                 MediaInfoMpcSv8=$enableval,  MediaInfoMpcSv8=depend)
AC_ARG_ENABLE(mpega,   AC_HELP_STRING([--disable-mpega],   [Disable Audio - MPEG-Audio]),                                 MediaInfoMpega=$enableval,   MediaInfoMpega=depend)
AC_ARG_ENABLE(pcm,     AC_HELP_STRING([--disable-pcm],     [Disable Audio - PCM]),                                        MediaInfoPcm=$enableval,     MediaInfoPcm=depend)
AC_ARG_ENABLE(tta,     AC_HELP_STRING([--disable-tta],     [Disable Audio - True Audio]),                                 MediaInfoTta=$enableval,     MediaInfoTta=depend)
AC_ARG_ENABLE(twinvq,  AC_HELP_STRING([--disable-twinvq],  [Disable Audio - TwinVQ]),                                     MediaInfoTwinVQ=$enableval,  MediaInfoTwinVQ=depend)
AC_ARG_ENABLE(vorbis,  AC_HELP_STRING([--disable-vorbis],  [Disable Audio - Vorbis]),                                     MediaInfovorbis=$enableval,  MediaInfovorbis=depend)
AC_ARG_ENABLE(wvpk,    AC_HELP_STRING([--disable-wvpk],    [Disable Audio - Wavepack]),                                   MediaInfoWvpk=$enableval,    MediaInfoWvpk=depend)
AC_ARG_ENABLE(bmp,     AC_HELP_STRING([--disable-bmp],     [Disable Image - Bitmap]),                                     MediaInfoBmp=$enableval,     MediaInfoBmp=depend)
AC_ARG_ENABLE(gif,     AC_HELP_STRING([--disable-gif],     [Disable Image - GIF]),                                        MediaInfoGif=$enableval,     MediaInfoGif=depend)
AC_ARG_ENABLE(ico,     AC_HELP_STRING([--disable-ico],     [Disable Image - ICO]),                                        MediaInfoIco=$enableval,     MediaInfoIco=depend)
AC_ARG_ENABLE(jpeg,    AC_HELP_STRING([--disable-jpeg],    [Disable Image - JPEG]),                                       MediaInfoJpeg=$enableval,    MediaInfoJpeg=depend)
AC_ARG_ENABLE(png,     AC_HELP_STRING([--disable-png],     [Disable Image - PNG]),                                        MediaInfoPng=$enableval,     MediaInfoPng=depend)
AC_ARG_ENABLE(rle,     AC_HELP_STRING([--disable-rle],     [Disable Image - Run-length encoding]),                        MediaInfoRle=$enableval,     MediaInfoRle=depend)
AC_ARG_ENABLE(tiff,    AC_HELP_STRING([--disable-tiff],    [Disable Image - TIFF]),                                       MediaInfoTiff=$enableval,    MediaInfoTiff=depend)
AC_ARG_ENABLE(bdav,    AC_HELP_STRING([--disable-bdav],    [Disable Multiple - BluRay Disc]),                             MediaInfoBdav=$enableval,    MediaInfoBdav=depend)
AC_ARG_ENABLE(cdxa,    AC_HELP_STRING([--disable-cdxa],    [Disable Multiple - CDXA (CD mode 2)]),                        MediaInfoCdxa=$enableval,    MediaInfoCdxa=depend)
AC_ARG_ENABLE(dvdv,    AC_HELP_STRING([--disable-dvdv],    [Disable Multiple - DVD-Video]),                               MediaInfoDvdv=$enableval,    MediaInfoDvdv=depend)
AC_ARG_ENABLE(flv,     AC_HELP_STRING([--disable-flv],     [Disable Multiple - FLV (Flash video)]),                       MediaInfoFlv=$enableval,     MediaInfoFlv=depend)
AC_ARG_ENABLE(mk,      AC_HELP_STRING([--disable-mk],      [Disable Multiple - Matroska]),                                MediaInfoMk=$enableval,      MediaInfoMk=depend)
AC_ARG_ENABLE(mpeg4,   AC_HELP_STRING([--disable-mpeg4],   [Disable Multiple - MPEG-4 based formats (MOV, AAC...)]),      MediaInfoMpeg4=$enableval,   MediaInfoMpeg4=depend)
AC_ARG_ENABLE(mpegps,  AC_HELP_STRING([--disable-mpegps],  [Disable Multiple - MPEG-PS]),                                 MediaInfoMpegPs=$enableval,  MediaInfoMpegPs=depend)
AC_ARG_ENABLE(mpegts,  AC_HELP_STRING([--disable-mpegts],  [Disable Multiple - MPEG-TS]),                                 MediaInfoMpegTs=$enableval,  MediaInfoMpegTs=depend)
AC_ARG_ENABLE(mxf,     AC_HELP_STRING([--disable-mxf],     [Disable Multiple - MXF]),                                     MediaInfoMxf=$enableval,     MediaInfoMxf=depend)
AC_ARG_ENABLE(nut,     AC_HELP_STRING([--disable-nut],     [Disable Multiple - Nut]),                                     MediaInfoNut=$enableval,     MediaInfoNut=depend)
AC_ARG_ENABLE(ogg,     AC_HELP_STRING([--disable-ogg],     [Disable Multiple - OGG (ogg, ogm)]),                          MediaInfoOgg=$enableval,     MediaInfoOgg=depend)
AC_ARG_ENABLE(riff,    AC_HELP_STRING([--disable-riff],    [Disable Multiple - RIFF based formats (AVI...)]),             MediaInfoRiff=$enableval,    MediaInfoRiff=depend)
AC_ARG_ENABLE(rm,      AC_HELP_STRING([--disable-rm],      [Disable Multiple - Real Media]),                              MediaInfoRm=$enableval,      MediaInfoRm=depend)
AC_ARG_ENABLE(skm,     AC_HELP_STRING([--disable-skm],     [Disable Multiple - SKM]),                                     MediaInfoSkm=$enableval,     MediaInfoSkm=depend)
AC_ARG_ENABLE(swf,     AC_HELP_STRING([--disable-swf],     [Disable Multiple - Shockwave]),                               MediaInfoSwf=$enableval,     MediaInfoSwf=depend)
AC_ARG_ENABLE(wm,      AC_HELP_STRING([--disable-wm],      [Disable Multiple - Windows Media (ASF, WMV, WMA)]),           MediaInfoWm=$enableval,      MediaInfoWm=depend)
AC_ARG_ENABLE(avc,     AC_HELP_STRING([--disable-avc],     [Disable Video - AVC/H264]),                                   MediaInfoAvc=$enableval,     MediaInfoAvc=depend)
AC_ARG_ENABLE(avsV,    AC_HELP_STRING([--disable-avsv],    [Disable Video - AVS (Chinese)]),                              MediaInfoAvsV=$enableval,    MediaInfoAvsV=depend)
AC_ARG_ENABLE(flic,    AC_HELP_STRING([--disable-flic],    [Disable Video - FLIC (fli, flc)]),                            MediaInfoFlic=$enableval,    MediaInfoFlic=depend)
AC_ARG_ENABLE(mpeg4v,  AC_HELP_STRING([--disable-mpeg4v],  [Disable Video - MPEG-4 Video based formats (DivX, XviD...)]), MediaInfoMpeg4v=$enableval,  MediaInfoMpeg4v=depend)
AC_ARG_ENABLE(mpegv,   AC_HELP_STRING([--disable-mpegv],   [Disable Video - MPEG-Video]),                                 MediaInfoMpegv=$enableval,   MediaInfoMpegv=depend)
AC_ARG_ENABLE(vc1,     AC_HELP_STRING([--disable-vc1],     [Disable Video - VC-1]),                                       MediaInfoVc1=$enableval,     MediaInfoVc1=depend)

dnl -------------------------------------------------------------------------
dnl With
dnl
AC_ARG_WITH(libz_static,      AC_HELP_STRING([--with-libz-static],      [Use static version of Zlib]),                           , with_libz_static=no)

dnl -------------------------------------------------------------------------
dnl MediaInfo *
dnl
if test "$MediaInfoMinimizeSize" = "yes";  then AC_DEFINE(MEDIAINFO_MINIMIZESIZE) fi
if test "$MediaInfoAll"     = "no";  then AC_DEFINE(MEDIAINFO_ALL_NO)     fi
if test "$MediaInfoArchive" = "no";  then AC_DEFINE(MEDIAINFO_ARCHIVE_NO) fi
if test "$MediaInfoAudio"   = "no";  then AC_DEFINE(MEDIAINFO_AUDIO_NO)   fi
if test "$MediaInfoImage"   = "no";  then AC_DEFINE(MEDIAINFO_IMAGE_NO)   fi
if test "$MediaInfoMulti"   = "no";  then AC_DEFINE(MEDIAINFO_MULTI_NO)   fi
if test "$MediaInfoText"    = "no";  then AC_DEFINE(MEDIAINFO_TEXT_NO)    fi
if test "$MediaInfoVideo"   = "no";  then AC_DEFINE(MEDIAINFO_VIDEO_NO)   fi
if test "$MediaInfo7z"      = "no";  then AC_DEFINE(MEDIAINFO_7Z_NO)      fi; if test "$MediaInfo7z"      = "yes"; then AC_DEFINE(MEDIAINFO_7Z_YES)      fi
if test "$MediaInfoAce"     = "no";  then AC_DEFINE(MEDIAINFO_ACE_NO)     fi; if test "$MediaInfoAce"     = "yes"; then AC_DEFINE(MEDIAINFO_ACE_YES)     fi
if test "$MediaInfoRar"     = "no";  then AC_DEFINE(MEDIAINFO_RAR_NO)     fi; if test "$MediaInfoRar"     = "yes"; then AC_DEFINE(MEDIAINFO_RAR_YES)     fi
if test "$MediaInfoZip"     = "no";  then AC_DEFINE(MEDIAINFO_ZIP_NO)     fi; if test "$MediaInfoZip"     = "yes"; then AC_DEFINE(MEDIAINFO_ZIP_YES)     fi
if test "$MediaInfoAc3"     = "no";  then AC_DEFINE(MEDIAINFO_AC3_NO)     fi; if test "$MediaInfoAc3"     = "yes"; then AC_DEFINE(MEDIAINFO_AC3_YES)     fi
if test "$MediaInfoAdif"    = "no";  then AC_DEFINE(MEDIAINFO_ADIF_NO)    fi; if test "$MediaInfoAdif"    = "yes"; then AC_DEFINE(MEDIAINFO_ADIF_YES)    fi
if test "$MediaInfoAdts"    = "no";  then AC_DEFINE(MEDIAINFO_ADTS_NO)    fi; if test "$MediaInfoAdts"    = "yes"; then AC_DEFINE(MEDIAINFO_ADTS_YES)    fi
if test "$MediaInfoAes3"    = "no";  then AC_DEFINE(MEDIAINFO_AES3_NO)    fi; if test "$MediaInfoAes3"    = "yes"; then AC_DEFINE(MEDIAINFO_AES3_YES)    fi
if test "$MediaInfoAmr"     = "no";  then AC_DEFINE(MEDIAINFO_AMR_NO)     fi; if test "$MediaInfoAmr"     = "yes"; then AC_DEFINE(MEDIAINFO_AMR_YES)     fi
if test "$MediaInfoApe"     = "no";  then AC_DEFINE(MEDIAINFO_APE_NO)     fi; if test "$MediaInfoApe"     = "yes"; then AC_DEFINE(MEDIAINFO_APE_YES)     fi
if test "$MediaInfoDts"     = "no";  then AC_DEFINE(MEDIAINFO_DTS_NO)     fi; if test "$MediaInfoDts"     = "yes"; then AC_DEFINE(MEDIAINFO_DTS_YES)     fi
if test "$MediaInfoFlac"    = "no";  then AC_DEFINE(MEDIAINFO_FLAC_NO)    fi; if test "$MediaInfoFlac"    = "yes"; then AC_DEFINE(MEDIAINFO_FLAC_YES)    fi
if test "$MediaInfoId3"     = "no";  then AC_DEFINE(MEDIAINFO_ID3_NO)     fi; if test "$MediaInfoId3"     = "yes"; then AC_DEFINE(MEDIAINFO_ID3_YES)     fi
if test "$MediaInfoId3v2"   = "no";  then AC_DEFINE(MEDIAINFO_ID3V2_NO)   fi; if test "$MediaInfoId3v2"   = "yes"; then AC_DEFINE(MEDIAINFO_ID3V2_YES)   fi
if test "$MediaInfoMpc"     = "no";  then AC_DEFINE(MEDIAINFO_MPC_NO)     fi; if test "$MediaInfoMpc"     = "yes"; then AC_DEFINE(MEDIAINFO_MPC_YES)     fi
if test "$MediaInfoMpcSv8"  = "no";  then AC_DEFINE(MEDIAINFO_MPCSV8_NO)  fi; if test "$MediaInfoMpcSv8"  = "yes"; then AC_DEFINE(MEDIAINFO_MPCSV8_YES)  fi
if test "$MediaInfoMpega"   = "no";  then AC_DEFINE(MEDIAINFO_MPEGA_NO)   fi; if test "$MediaInfoMpega"   = "yes"; then AC_DEFINE(MEDIAINFO_MPEGA_YES)   fi
if test "$MediaInfoTwinVQ"  = "no";  then AC_DEFINE(MEDIAINFO_TWINVQ_NO)  fi; if test "$MediaInfoTwinVQ"  = "yes"; then AC_DEFINE(MEDIAINFO_TWINVQ_YES)  fi
if test "$MediaInfoBmp"     = "no";  then AC_DEFINE(MEDIAINFO_BMP_NO)     fi; if test "$MediaInfoBmp"     = "yes"; then AC_DEFINE(MEDIAINFO_BMP_YES)     fi
if test "$MediaInfoGif"     = "no";  then AC_DEFINE(MEDIAINFO_GIF_NO)     fi; if test "$MediaInfoGif"     = "yes"; then AC_DEFINE(MEDIAINFO_GIF_YES)     fi
if test "$MediaInfoIco"     = "no";  then AC_DEFINE(MEDIAINFO_ICO_NO)     fi; if test "$MediaInfoIco"     = "yes"; then AC_DEFINE(MEDIAINFO_ICO_YES)     fi
if test "$MediaInfoJpeg"    = "no";  then AC_DEFINE(MEDIAINFO_JPEG_NO)    fi; if test "$MediaInfoJpeg"    = "yes"; then AC_DEFINE(MEDIAINFO_JPEG_YES)    fi
if test "$MediaInfoPng"     = "no";  then AC_DEFINE(MEDIAINFO_PNG_NO)     fi; if test "$MediaInfoPng"     = "yes"; then AC_DEFINE(MEDIAINFO_PNG_YES)     fi
if test "$MediaInfoTiff"    = "no";  then AC_DEFINE(MEDIAINFO_TIFF_NO)    fi; if test "$MediaInfoTiff"    = "yes"; then AC_DEFINE(MEDIAINFO_TIFF_YES)    fi
if test "$MediaInfoBdav"    = "no";  then AC_DEFINE(MEDIAINFO_BDAV_NO)    fi; if test "$MediaInfoBdav"    = "yes"; then AC_DEFINE(MEDIAINFO_BDAV_YES)    fi
if test "$MediaInfoCdxa"    = "no";  then AC_DEFINE(MEDIAINFO_CDXA_NO)    fi; if test "$MediaInfoCdxa"    = "yes"; then AC_DEFINE(MEDIAINFO_CDXA_YES)    fi
if test "$MediaInfoDvdv"    = "no";  then AC_DEFINE(MEDIAINFO_DVDV_NO)    fi; if test "$MediaInfoDvdv"    = "yes"; then AC_DEFINE(MEDIAINFO_DVDV_YES)    fi
if test "$MediaInfoFlv"     = "no";  then AC_DEFINE(MEDIAINFO_FLV_NO)     fi; if test "$MediaInfoFlv"     = "yes"; then AC_DEFINE(MEDIAINFO_FLV_YES)     fi
if test "$MediaInfoMk"      = "no";  then AC_DEFINE(MEDIAINFO_MK_NO)      fi; if test "$MediaInfoMk"      = "yes"; then AC_DEFINE(MEDIAINFO_MK_YES)      fi
if test "$MediaInfoMpeg4"   = "no";  then AC_DEFINE(MEDIAINFO_MPEG4_NO)   fi; if test "$MediaInfoMpeg4"   = "yes"; then AC_DEFINE(MEDIAINFO_MPEG4_YES)   fi
if test "$MediaInfoMpegPs"  = "no";  then AC_DEFINE(MEDIAINFO_MPEGPS_NO)  fi; if test "$MediaInfoMpegPs"  = "yes"; then AC_DEFINE(MEDIAINFO_MPEGPS_YES)  fi
if test "$MediaInfoMpegTs"  = "no";  then AC_DEFINE(MEDIAINFO_MPEGTS_NO)  fi; if test "$MediaInfoMpegTs"  = "yes"; then AC_DEFINE(MEDIAINFO_MPEGTS_YES)  fi
if test "$MediaInfoMxf"     = "no";  then AC_DEFINE(MEDIAINFO_MXF_NO)     fi; if test "$MediaInfoMxf"     = "yes"; then AC_DEFINE(MEDIAINFO_MXF_YES)     fi
if test "$MediaInfoNut"     = "no";  then AC_DEFINE(MEDIAINFO_NUT_NO)     fi; if test "$MediaInfoNut"     = "yes"; then AC_DEFINE(MEDIAINFO_NUT_YES)     fi
if test "$MediaInfoOgg"     = "no";  then AC_DEFINE(MEDIAINFO_OGG_NO)     fi; if test "$MediaInfoOgg"     = "yes"; then AC_DEFINE(MEDIAINFO_OGG_YES)     fi
if test "$MediaInfoRiff"    = "no";  then AC_DEFINE(MEDIAINFO_RIFF_NO)    fi; if test "$MediaInfoRiff"    = "yes"; then AC_DEFINE(MEDIAINFO_RIFF_YES)    fi
if test "$MediaInfoRm"      = "no";  then AC_DEFINE(MEDIAINFO_RM_NO)      fi; if test "$MediaInfoRm"      = "yes"; then AC_DEFINE(MEDIAINFO_RM_YES)      fi
if test "$MediaInfoSkm"     = "no";  then AC_DEFINE(MEDIAINFO_SKM_NO)     fi; if test "$MediaInfoSkm"     = "yes"; then AC_DEFINE(MEDIAINFO_SKM_YES)     fi
if test "$MediaInfoSwf"     = "no";  then AC_DEFINE(MEDIAINFO_SWF_NO)     fi; if test "$MediaInfoSwf"     = "yes"; then AC_DEFINE(MEDIAINFO_SWF_YES)     fi
if test "$MediaInfoWm"      = "no";  then AC_DEFINE(MEDIAINFO_WM_NO)      fi; if test "$MediaInfoWm"      = "yes"; then AC_DEFINE(MEDIAINFO_WM_YES)      fi
if test "$MediaInfoAvc"     = "no";  then AC_DEFINE(MEDIAINFO_AVC_NO)     fi; if test "$MediaInfoAvc"     = "yes"; then AC_DEFINE(MEDIAINFO_AVC_YES)     fi
if test "$MediaInfoAvsV"    = "no";  then AC_DEFINE(MEDIAINFO_AVSV_NO)    fi; if test "$MediaInfoAvsV"    = "yes"; then AC_DEFINE(MEDIAINFO_AVSV_YES)    fi
if test "$MediaInfoFlic"    = "no";  then AC_DEFINE(MEDIAINFO_FLIC_NO)    fi; if test "$MediaInfoflic"    = "yes"; then AC_DEFINE(MEDIAINFO_FLIC_YES)    fi
if test "$MediaInfoMpeg4v"  = "no";  then AC_DEFINE(MEDIAINFO_MPEG4V_NO)  fi; if test "$MediaInfoMpeg4v"  = "yes"; then AC_DEFINE(MEDIAINFO_MPEG4V_YES)  fi
if test "$MediaInfoMpegv"   = "no";  then AC_DEFINE(MEDIAINFO_MPEGV_NO)   fi; if test "$MediaInfoMpegv"   = "yes"; then AC_DEFINE(MEDIAINFO_MPEGV_YES)   fi

dnl #########################################################################
dnl ### Options from elsewhere
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/libzen-config; then
	enable_unicode="$(../../../../ZenLib/Project/GNU/Library/libzen-config Unicode)"
else
	if pkg-config --exists libzen; then
		enable_unicode="$(pkg-config --variable=Unicode libzen)"
	else
		AC_MSG_ERROR([libzen configuration is not found])
	fi
fi

dnl #########################################################################
dnl ### Needed libs
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl ZLib
dnl

if test "$with_libz_static" != "yes"; then
	AC_CHECK_LIB(z, uncompress, [have_libz="yes"], [have_libz="no"])
	AC_CHECK_HEADERS(zlib.h, [], [have_libz="no"])
else
	have_libz="no"
fi
if test "$have_libz" != "yes"; then
	if test -e ../../../../Shared/Source/ZLib/libz.a; then
		with_zlib="builtin"
	else
		AC_MSG_NOTICE([zlib is not found, trying to download and compile it])
		if test -e ../../../../Shared/Project/ZLib/Compile.sh; then
			cd ../../../../Shared/Project/ZLib
			chmod u+x Compile.sh
			./Compile.sh
			cd $Home
			if test -e ../../../../Shared/Source/ZLib/libz.a; then
				with_zlib="builtin"
			else
				with_zlib="no"
				AC_MSG_ERROR([Problem while compiling builtin ZLib])
			fi
		else
			with_zlib="no"
			AC_MSG_ERROR([ZLib is not found, please install development package of zlib])
		fi
	fi
	CXXFLAGS="$CXXFLAGS -I../../../../Shared/Source/ZLib"
	if test "$enable_shared" = "yes"; then
		LIBS="$LIBS -L../../../../Shared/Source/ZLib"
	fi
	MediaInfoLib_LIBS="$MediaInfoLib_LIBS -L../../../../Shared/Source/ZLib"
else
    with_zlib="system"
fi
if test "$enable_shared" = "yes"; then
    LIBS="$LIBS -lz"
fi
MediaInfoLib_LIBS="$MediaInfoLib_LIBS -lz"

dnl -------------------------------------------------------------------------
dnl ZenLib
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/libzen.la; then
	with_zenlib="builtin"
	CXXFLAGS="$CXXFLAGS $(../../../../ZenLib/Project/GNU/Library/libzen-config CXXFLAGS)"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $(../../../../ZenLib/Project/GNU/Library/libzen-config CXXFLAGS)"
	if test "$enable_shared" = "yes"; then
		LIBS="$LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
	fi
	MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
else
	if pkg-config --exists libzen; then
		with_zenlib="system"
		CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libzen)"
		MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $(pkg-config --cflags libzen)"
		if test "$enable_shared" = "yes"; then
			LIBS="$LIBS $(pkg-config --libs libzen)"
		fi
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs libzen)"
	else
		AC_MSG_ERROR([libzen configuration is not found])
	fi
fi

dnl #########################################################################
dnl ### C/C++ compiler options used to compile
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test "$enable_unicode" = "yes"; then
	AC_DEFINE(UNICODE)
	MediaInfoLib_Unicode="yes"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS -DUNICODE"
else
	MediaInfoLib_Unicode="no"
fi

dnl -------------------------------------------------------------------------
dnl GNU compiler
dnl
if test "$GXX" = yes ; then
	CFLAGS="$CFLAGS -fPIC"
	CXXFLAGS="$CXXFLAGS -fPIC"
	if test "$enable_debug" = "yes"; then
		CFLAGS="$CFLAGS -O0 -g -Wall -Wundef"
		CXXFLAGS="$CXXFLAGS -O0 -g -Wall -W -Wundef -Wcast-qual -Wno-ctor-dtor-privacy"
	else
		CFLAGS="$CFLAGS -O2"
		CXXFLAGS="$CXXFLAGS -O2"
	fi
	if test "$enable_gprof" = "yes"; then
		CFLAGS="$CFLAGS -pg -g"
		CXXFLAGS="$CXXFLAGS -pg -g"
	fi
fi

dnl #########################################################################
dnl ### MediaInfoLib flags
dnl #########################################################################

MediaInfoLib_LIBS=" -L$(pwd) -lmediainfo $MediaInfoLib_LIBS"

dnl #########################################################################
dnl ### Output
dnl #########################################################################

AC_SUBST(MediaInfoLib_CXXFLAGS)
AC_SUBST(MediaInfoLib_LIBS)
AC_SUBST(MediaInfoLib_Unicode)
AC_CONFIG_FILES(libmediainfo-config, [chmod u+x libmediainfo-config])
AC_CONFIG_FILES(libmediainfo.pc)

AC_OUTPUT(Makefile)

dnl #########################################################################
dnl ### Report how we have been configured
dnl #########################################################################

Mcho ()
{
if test "$2" = "no" || test "$2" = "yes"; then
echo "  $1 forced to                                        $2"
fi
}

echo ""
echo "Configured MediaInfoLib for '$build'"
echo ""
echo "  Unicode?                                                $enable_unicode"
echo "  Using ZenLib?                                           $with_zenlib"
echo "  Using ZLib?                                             $with_zlib"
echo ""
echo "  Create static lib?                                      $enable_static"
echo "  Create shared lib?                                      $enable_shared"
echo ""
Mcho "Minimimize size" "$MediaInfoMinimizeSize"
Mcho "All    " "$MediaInfoAll"
Mcho "Archive" "$MediaInfoArchive"
Mcho "Audio  " "$MediaInfoAudio"
Mcho "Image  " "$MediaInfoImage"
Mcho "Multi  " "$MediaInfoMulti"
Mcho "Text   " "$MediaInfoText"
Mcho "Video  " "$MediaInfoVideo"
Mcho "7z     " "$MediaInfo7z"
Mcho "Ace    " "$MediaInfoAce"
Mcho "Rar    " "$MediaInfoRar"
Mcho "Zip    " "$MediaInfoZip"
Mcho "Ac3    " "$MediaInfoAc3"
Mcho "Adif   " "$MediaInfoAdif"
Mcho "Adts   " "$MediaInfoAdts"
Mcho "Aes3   " "$MediaInfoAes3"
Mcho "Latm   " "$MediaInfoLatm"
Mcho "Amr    " "$MediaInfoAmr"
Mcho "Ape    " "$MediaInfoApe"
Mcho "Dts    " "$MediaInfoDts"
Mcho "Flac   " "$MediaInfoFlac"
Mcho "Id3    " "$MediaInfoId3"
Mcho "Id3v2  " "$MediaInfoId3v2"
Mcho "Mpc    " "$MediaInfoMpc"
Mcho "MpcSv8 " "$MediaInfoMpcSv8"
Mcho "Mpega  " "$MediaInfoMpega"
Mcho "TwinVQ " "$MediaInfoTwinVQ"
Mcho "Bmp    " "$MediaInfoBmp"
Mcho "Gif    " "$MediaInfoGif"
Mcho "Ico    " "$MediaInfoIco"
Mcho "Jpeg   " "$MediaInfoJpeg"
Mcho "Png    " "$MediaInfoPng"
Mcho "Tiff   " "$MediaInfoTiff"
Mcho "Bdav   " "$MediaInfoBdav"
Mcho "Cdxa   " "$MediaInfoCdxa"
Mcho "Dvdv   " "$MediaInfoDvdv"
Mcho "Flv    " "$MediaInfoFlv"
Mcho "Mk     " "$MediaInfoMk"
Mcho "Mpeg4  " "$MediaInfoMpeg4"
Mcho "MpegPs " "$MediaInfoMpegPs"
Mcho "MpegTs " "$MediaInfoMpegTs"
Mcho "Mxf    " "$MediaInfoMxf"
Mcho "Nut    " "$MediaInfoNut"
Mcho "Ogg    " "$MediaInfoOgg"
Mcho "Riff   " "$MediaInfoRiff"
Mcho "Rm     " "$MediaInfoRm"
Mcho "Skm    " "$MediaInfoSkm"
Mcho "Swf    " "$MediaInfoSwf"
Mcho "Wm     " "$MediaInfoWm"
Mcho "Avc    " "$MediaInfoAvc"
Mcho "AvsV   " "$MediaInfoAvsV"
Mcho "Flic   " "$MediaInfoFlic"
Mcho "Mpeg4v " "$MediaInfoMpeg4v"
Mcho "Mpegv  " "$MediaInfoMpegv"
echo ""
echo "  CXXFLAGS: $CXXFLAGS"
echo "  LIBS: $LIBS"
echo "  MediaInfoLib_CXXFLAGS: $CXXFLAGS"
echo "  MediaInfoLib_LIBS: $MediaInfoLib_LIBS"
