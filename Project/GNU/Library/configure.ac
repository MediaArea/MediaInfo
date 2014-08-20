dnl #########################################################################
dnl ### Init
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Name and version
dnl
AC_INIT([libmediainfo], [0.7.70])

dnl -------------------------------------------------------------------------
dnl Test if we are at the good place
dnl
AC_CONFIG_SRCDIR([../../../Source/MediaInfo/MediaInfo.cpp])

dnl -------------------------------------------------------------------------
dnl sets build, host, target variables and the same with _alias
dnl
AC_CANONICAL_BUILD
AM_INIT_AUTOMAKE([foreign -Wall])

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
AC_ARG_ENABLE(debug,            AC_HELP_STRING([--enable-debug],            [Enable full debug]),                         , enable_debug=no)
AC_ARG_ENABLE(gprof,            AC_HELP_STRING([--enable-gprof],            [Enable gprof log]),                          , enable_gprof=no)
AC_ARG_ENABLE(visibility,       AC_HELP_STRING([--enable-visibility],       [Enable hidden visibility]),                  , enable_visibility=no)
AC_ARG_ENABLE(universal_binary, AC_HELP_STRING([--enable-universal_binary], [Create Mac PPC/i386 Universal binary]),      , enable_universal_binary=no)
AC_ARG_ENABLE(arch-ppc,         AC_HELP_STRING([--enable-arch-ppc],         [Create Mac PPC architecture]),               , enable_arch_ppc=no)
AC_ARG_ENABLE(arch-ppc64,       AC_HELP_STRING([--enable-arch-ppc64],       [Create Mac PPC64 architecture]),             , enable_arch_ppc64=no)
AC_ARG_ENABLE(arch-i386,        AC_HELP_STRING([--enable-arch-i386],        [Create Mac i386 architecture]),              , enable_arch_i386=no)
AC_ARG_ENABLE(arch-x86_64,      AC_HELP_STRING([--enable-arch-x86_64],      [Create Mac x86_64 architecture]),            , enable_arch_x86_64=no)
AC_ARG_ENABLE(staticlibs,       AC_HELP_STRING([--enable-staticlibs],       [Use static libmediainfo and libzen]),        , enable_staticlibs=no)
AC_ARG_ENABLE(minimize-size,    AC_HELP_STRING([--enable-minimize-size],    [Enable size optimisations]),                 MediaInfoMinimizeSize=$enableval, MediaInfoMinimizeSize=depend)
AC_ARG_ENABLE(minimal,    		AC_HELP_STRING([--enable-minimal],    		[Enable minimal management (except parsers)]),MediaInfoMinimal=$enableval, MediaInfoMinimal=depend)
AC_ARG_ENABLE(trace,    		AC_HELP_STRING([--enable-trace],    		[Enable trace management]),                   MediaInfoTrace=$enableval,   MediaInfoTrace=depend)
AC_ARG_ENABLE(events,    		AC_HELP_STRING([--enable-events],    		[Enable events management]),                  MediaInfoEvents=$enableval,  MediaInfoEvents=depend)
AC_ARG_ENABLE(demux,    		AC_HELP_STRING([--enable-demux],    		[Enable demux management]),                   MediaInfoDemux=$enableval,   MediaInfoDemux=depend)
AC_ARG_ENABLE(libmms-describe-support, AC_HELP_STRING([--enable-libmms-describe-support], [Enable customized libmms describe support]), MediaInfoLibmmsDescribeSupport=$enableval, MediaInfoLibmmsDescribeSupport=depend)
AC_ARG_ENABLE(all,     AC_HELP_STRING([--disable-all],     [Disable all parsers]),                                        MediaInfoAll=$enableval,     MediaInfoAll=depend)
AC_ARG_ENABLE(archive, AC_HELP_STRING([--disable-archive], [Disable all archive parsers]),                                MediaInfoArchive=$enableval, MediaInfoArchive=depend)
AC_ARG_ENABLE(audio,   AC_HELP_STRING([--disable-audio],   [Disable all audio parsers]),                                  MediaInfoAudio=$enableval,   MediaInfoAudio=depend)
AC_ARG_ENABLE(image,   AC_HELP_STRING([--disable-image],   [Disable all image parsers]),                                  MediaInfoImage=$enableval,   MediaInfoImage=depend)
AC_ARG_ENABLE(multi,   AC_HELP_STRING([--disable-multi],   [Disable all multiple parsers]),                               MediaInfoMulti=$enableval,   MediaInfoMulti=depend)
AC_ARG_ENABLE(tag,     AC_HELP_STRING([--disable-tag],     [Disable all tag parsers]),                                    MediaInfoTag=$enableval,     MediaInfoTag=depend)
AC_ARG_ENABLE(text,    AC_HELP_STRING([--disable-text],    [Disable all text parsers]),                                   MediaInfoText=$enableval,    MediaInfoText=depend)
AC_ARG_ENABLE(video,   AC_HELP_STRING([--disable-video],   [Disable all video parsers]),                                  MediaInfoVideo=$enableval,   MediaInfoVideo=depend)
AC_ARG_ENABLE(7z,      AC_HELP_STRING([--disable-7z],      [Disable Archive - 7z]),                                       MediaInfo7z=$enableval,      MediaInfo7z=depend)
AC_ARG_ENABLE(ace,     AC_HELP_STRING([--disable-ace],     [Disable Archive - Ace]),                                      MediaInfoAce=$enableval,     MediaInfoAce=depend)
AC_ARG_ENABLE(bzip2,   AC_HELP_STRING([--disable-bzip2],   [Disable Archive - Bzip2]),                                    MediaInfoBzip2=$enableval,   MediaInfoBzip2=depend)
AC_ARG_ENABLE(elf,     AC_HELP_STRING([--disable-elf],     [Disable Archive - ELF]),                                      MediaInfoElf=$enableval,     MediaInfoElf=depend)
AC_ARG_ENABLE(gzip,    AC_HELP_STRING([--disable-gzip],    [Disable Archive - GZip]),                                     MediaInfoGzip=$enableval,    MediaInfoGzip=depend)
AC_ARG_ENABLE(iso9660, AC_HELP_STRING([--disable-iso9660], [Disable Archive - ISO 9660]),                                 MediaInfoIso9660=$enableval, MediaInfoIso9660=depend)
AC_ARG_ENABLE(mz,      AC_HELP_STRING([--disable-mz],      [Disable Archive - MZ (DOS) and PE (Windows)]),                MediaInfoMz=$enableval,      MediaInfoMz=depend)
AC_ARG_ENABLE(rar,     AC_HELP_STRING([--disable-rar],     [Disable Archive - Rar]),                                      MediaInfoRar=$enableval,     MediaInfoRar=depend)
AC_ARG_ENABLE(tar,     AC_HELP_STRING([--disable-tar],     [Disable Archive - Tar]),                                      MediaInfoTar=$enableval,     MediaInfoTar=depend)
AC_ARG_ENABLE(zip,     AC_HELP_STRING([--disable-zip],     [Disable Archive - Zip]),                                      MediaInfoZip=$enableval,     MediaInfoZip=depend)
AC_ARG_ENABLE(aac,     AC_HELP_STRING([--disable-aac],     [Disable Audio - AAC (raw)]),                                  MediaInfoAac=$enableval,     MediaInfoAac=depend)
AC_ARG_ENABLE(ac3,     AC_HELP_STRING([--disable-ac3],     [Disable Audio - AC-3]),                                       MediaInfoAc3=$enableval,     MediaInfoAc3=depend)
AC_ARG_ENABLE(adif,    AC_HELP_STRING([--disable-adif],    [Disable Audio - ADIF]),                                       MediaInfoAdif=$enableval,    MediaInfoAdif=depend)
AC_ARG_ENABLE(adpcm,   AC_HELP_STRING([--disable-adpcm],   [Disable Audio - ADPCM]),                                      MediaInfoAdpcm=$enableval,   MediaInfoAdpcm=depend)
AC_ARG_ENABLE(adts,    AC_HELP_STRING([--disable-adts],    [Disable Audio - ADTS]),                                       MediaInfoAdts=$enableval,    MediaInfoAdts=depend)
AC_ARG_ENABLE(aes3,    AC_HELP_STRING([--disable-aes3],    [Disable Audio - AES3]),                                       MediaInfoAdts=$enableval,    MediaInfoAes3=depend)
AC_ARG_ENABLE(als,     AC_HELP_STRING([--disable-als],     [Disable Audio - ALS]),                                        MediaInfoAls=$enableval,     MediaInfoAls=depend)
AC_ARG_ENABLE(amr,     AC_HELP_STRING([--disable-amr],     [Disable Audio - AMR]),                                        MediaInfoAmr=$enableval,     MediaInfoAmr=depend)
AC_ARG_ENABLE(amv,     AC_HELP_STRING([--disable-amv],     [Disable Audio - AMV]),                                        MediaInfoAmv=$enableval,     MediaInfoAmv=depend)
AC_ARG_ENABLE(ape,     AC_HELP_STRING([--disable-ape],     [Disable Audio - Monkey Audio (.ape, .mac)]),                  MediaInfoApe=$enableval,     MediaInfoApe=depend)
AC_ARG_ENABLE(au,      AC_HELP_STRING([--disable-au],      [Disable Audio - Au]),                                         MediaInfoAu=$enableval,      MediaInfoAu=depend)
AC_ARG_ENABLE(celt,    AC_HELP_STRING([--disable-celt],    [Disable Audio - CELT]),                                       MediaInfoCelt=$enableval,    MediaInfoCelt=depend)
AC_ARG_ENABLE(dts,     AC_HELP_STRING([--disable-dts],     [Disable Audio - DTS]),                                        MediaInfoDts=$enableval,     MediaInfoDts=depend)
AC_ARG_ENABLE(flac,    AC_HELP_STRING([--disable-flac],    [Disable Audio - FLAC format]),                                MediaInfoFlac=$enableval,    MediaInfoFlac=depend)
AC_ARG_ENABLE(la,      AC_HELP_STRING([--disable-la],      [Disable Audio - LA (Lossless Audio)]),                        MediaInfoLa=$enableval,      MediaInfoLa=depend)
AC_ARG_ENABLE(latm,    AC_HELP_STRING([--disable-latm],    [Disable Audio - LATM]),                                       MediaInfoLatm=$enableval,    MediaInfoLatm=depend)
AC_ARG_ENABLE(midi,    AC_HELP_STRING([--disable-midi],    [Disable Audio - MIDI]),                                       MediaInfoMidi=$enableval,    MediaInfoMidi=depend)
AC_ARG_ENABLE(mpc,     AC_HELP_STRING([--disable-mpc],     [Disable Audio - Musepack format (.mpc)]),                     MediaInfoMpc=$enableval,     MediaInfoMpc=depend)
AC_ARG_ENABLE(mpcsv8,  AC_HELP_STRING([--disable-mpc],     [Disable Audio - Musepack format SV8 (.mpc)]),                 MediaInfoMpcSv8=$enableval,  MediaInfoMpcSv8=depend)
AC_ARG_ENABLE(mpega,   AC_HELP_STRING([--disable-mpega],   [Disable Audio - MPEG-Audio]),                                 MediaInfoMpega=$enableval,   MediaInfoMpega=depend)
AC_ARG_ENABLE(openmf,  AC_HELP_STRING([--disable-openmg],  [Disable Audio - OpenMG (.oma)]),                              MediaInfoOpenMG=$enableval,  MediaInfoOpenMG=depend)
AC_ARG_ENABLE(pcm,     AC_HELP_STRING([--disable-pcm],     [Disable Audio - PCM]),                                        MediaInfoPcm=$enableval,     MediaInfoPcm=depend)
AC_ARG_ENABLE(ps2a,    AC_HELP_STRING([--disable-ps2a],    [Disable Audio - PS2 Audio]),                                  MediaInfoPs2a=$enableval,    MediaInfoPs2a=depend)
AC_ARG_ENABLE(rkau,    AC_HELP_STRING([--disable-rkau],    [Disable Audio - RK Audio]),                                   MediaInfoRkau=$enableval,    MediaInfoRkau=depend)
AC_ARG_ENABLE(speex,   AC_HELP_STRING([--disable-speex],   [Disable Audio - Speex]),                                      MediaInfoSpeex=$enableval,   MediaInfoSpeex=depend)
AC_ARG_ENABLE(tak,     AC_HELP_STRING([--disable-tak],     [Disable Audio - TAK]),                                        MediaInfoTak=$enableval,     MediaInfoTak=depend)
AC_ARG_ENABLE(tta,     AC_HELP_STRING([--disable-tta],     [Disable Audio - True Audio]),                                 MediaInfoTta=$enableval,     MediaInfoTta=depend)
AC_ARG_ENABLE(twinvq,  AC_HELP_STRING([--disable-twinvq],  [Disable Audio - TwinVQ]),                                     MediaInfoTwinVQ=$enableval,  MediaInfoTwinVQ=depend)
AC_ARG_ENABLE(vorbis,  AC_HELP_STRING([--disable-vorbis],  [Disable Audio - Vorbis]),                                     MediaInfovorbis=$enableval,  MediaInfovorbis=depend)
AC_ARG_ENABLE(wvpk,    AC_HELP_STRING([--disable-wvpk],    [Disable Audio - Wavepack]),                                   MediaInfoWvpk=$enableval,    MediaInfoWvpk=depend)
AC_ARG_ENABLE(arriraw, AC_HELP_STRING([--disable-arriraw], [Disable Image - Arri Raw]),                                   MediaInfoArriRaw=$enableval, MediaInfoArriRaw=depend)
AC_ARG_ENABLE(bmp,     AC_HELP_STRING([--disable-bmp],     [Disable Image - Bitmap]),                                     MediaInfoBmp=$enableval,     MediaInfoBmp=depend)
AC_ARG_ENABLE(dds,     AC_HELP_STRING([--disable-dds],     [Disable Image - DirectDraw Surface]),                         MediaInfoDds=$enableval,     MediaInfoDds=depend)
AC_ARG_ENABLE(dpx,     AC_HELP_STRING([--disable-dpx],     [Disable Image - DPX]),                                        MediaInfoDpx=$enableval,     MediaInfoDpx=depend)
AC_ARG_ENABLE(exr,     AC_HELP_STRING([--disable-exr],     [Disable Image - EXR]),                                        MediaInfoExr=$enableval,     MediaInfoExr=depend)
AC_ARG_ENABLE(gif,     AC_HELP_STRING([--disable-gif],     [Disable Image - GIF]),                                        MediaInfoGif=$enableval,     MediaInfoGif=depend)
AC_ARG_ENABLE(ico,     AC_HELP_STRING([--disable-ico],     [Disable Image - ICO]),                                        MediaInfoIco=$enableval,     MediaInfoIco=depend)
AC_ARG_ENABLE(jpeg,    AC_HELP_STRING([--disable-jpeg],    [Disable Image - JPEG]),                                       MediaInfoJpeg=$enableval,    MediaInfoJpeg=depend)
AC_ARG_ENABLE(pcx,     AC_HELP_STRING([--disable-pcx],     [Disable Image - PCX]),                                        MediaInfoPcx=$enableval,     MediaInfoPcx=depend)
AC_ARG_ENABLE(png,     AC_HELP_STRING([--disable-png],     [Disable Image - PNG]),                                        MediaInfoPng=$enableval,     MediaInfoPng=depend)
AC_ARG_ENABLE(psd,     AC_HELP_STRING([--disable-psd],     [Disable Image - PSD]),                                        MediaInfoPsd=$enableval,     MediaInfoPsd=depend)
AC_ARG_ENABLE(rle,     AC_HELP_STRING([--disable-rle],     [Disable Image - Run-length encoding]),                        MediaInfoRle=$enableval,     MediaInfoRle=depend)
AC_ARG_ENABLE(tiff,    AC_HELP_STRING([--disable-tiff],    [Disable Image - TIFF]),                                       MediaInfoTiff=$enableval,    MediaInfoTiff=depend)
AC_ARG_ENABLE(aaf,     AC_HELP_STRING([--disable-aaf],     [Disable Multiple - AAF]),                                     MediaInfoAaf=$enableval,     MediaInfoAaf=depend)
AC_ARG_ENABLE(bdav,    AC_HELP_STRING([--disable-bdav],    [Disable Multiple - Blu-ray audio-video (BDAV)]),              MediaInfoBdav=$enableval,    MediaInfoBdav=depend)
AC_ARG_ENABLE(bdmv,    AC_HELP_STRING([--disable-bdmv],    [Disable Multiple - Blu-ray Movie]),                           MediaInfoBdmv=$enableval,    MediaInfoBdmv=depend)
AC_ARG_ENABLE(cdxa,    AC_HELP_STRING([--disable-cdxa],    [Disable Multiple - CDXA (CD mode 2)]),                        MediaInfoCdxa=$enableval,    MediaInfoCdxa=depend)
AC_ARG_ENABLE(dashmpd, AC_HELP_STRING([--disable-dashmpd], [Disable Multiple - DASH MPD]),                                MediaInfoDashMpd=$enableval, MediaInfoDashMpd=depend)
AC_ARG_ENABLE(dcp,     AC_HELP_STRING([--disable-dcp],     [Disable Multiple - DCP]),                                     MediaInfoDcp=$enableval,     MediaInfoDcp=depend)
AC_ARG_ENABLE(dpg,     AC_HELP_STRING([--disable-dpg],     [Disable Multiple - DPG (Nintendo DS)]),                       MediaInfoDpg=$enableval,     MediaInfoDpg=depend)
AC_ARG_ENABLE(references, AC_HELP_STRING([--disable-references], [Disable Multiple - Reference files support]),           MediaInfoReferences=$enableval, MediaInfoReferences=depend)
AC_ARG_ENABLE(dvdif,   AC_HELP_STRING([--disable-dvdif],   [Disable Multiple - DV DIF (Digital Video)]),                  MediaInfoDvDif=$enableval,   MediaInfoDvDif=depend)
AC_ARG_ENABLE(dvdif-analyze, AC_HELP_STRING([--enable-dvdif-analyze],   [Enable Multiple - DV DIF (Digital Video) Analyze part]), MediaInfoDvDifAnalyze=$enableval,   MediaInfoDvDifAnalyze=depend)
AC_ARG_ENABLE(dvdv,    AC_HELP_STRING([--disable-dvdv],    [Disable Multiple - DVD-Video]),                               MediaInfoDvdv=$enableval,    MediaInfoDvdv=depend)
AC_ARG_ENABLE(dxw,     AC_HELP_STRING([--disable-dxw],     [Disable Multiple - DXW]),                                     MediaInfoDxw=$enableval,     MediaInfoDxw=depend)
AC_ARG_ENABLE(flv,     AC_HELP_STRING([--disable-flv],     [Disable Multiple - FLV (Flash video)]),                       MediaInfoFlv=$enableval,     MediaInfoFlv=depend)
AC_ARG_ENABLE(gxf,     AC_HELP_STRING([--disable-gxf],     [Disable Multiple - GXF]),                                     MediaInfoGxf=$enableval,     MediaInfoGxf=depend)
AC_ARG_ENABLE(hdsf4m,  AC_HELP_STRING([--disable-hdsf4m],  [Disable Multiple - HDS F4M (Flash Media Manifest)]),          MediaInfoHdsF4m=$enableval,  MediaInfoHdsf4m=depend)
AC_ARG_ENABLE(hls,     AC_HELP_STRING([--disable-hls],     [Disable Multiple - HLS]),                                     MediaInfoHls=$enableval,     MediaInfoHls=depend)
AC_ARG_ENABLE(mk,      AC_HELP_STRING([--disable-mk],      [Disable Multiple - Matroska]),                                MediaInfoMk=$enableval,      MediaInfoMk=depend)
AC_ARG_ENABLE(mpeg4,   AC_HELP_STRING([--disable-mpeg4],   [Disable Multiple - MPEG-4 based formats (MOV, AAC...)]),      MediaInfoMpeg4=$enableval,   MediaInfoMpeg4=depend)
AC_ARG_ENABLE(mpegps,  AC_HELP_STRING([--disable-mpegps],  [Disable Multiple - MPEG-PS]),                                 MediaInfoMpegPs=$enableval,  MediaInfoMpegPs=depend)
AC_ARG_ENABLE(mpegts,  AC_HELP_STRING([--disable-mpegts],  [Disable Multiple - MPEG-TS]),                                 MediaInfoMpegTs=$enableval,  MediaInfoMpegTs=depend)
AC_ARG_ENABLE(mpegts-pcr, AC_HELP_STRING([--disable-mpegts-pcr],  [Disable Multiple - MPEG-TS (PCR)]), MediaInfoMpegTsPcr=$enableval, MediaInfoMpegTsPcr=depend)
AC_ARG_ENABLE(mpegts-pestimestamp, AC_HELP_STRING([--disable-mpegts-pestimestamp],  [Disable Multiple - MPEG-TS (parser time stamp)]), MediaInfoMpegTsPesTimeStamp=$enableval, MediaInfoMpegTsPesTimeStamp=depend)
AC_ARG_ENABLE(mxf,     AC_HELP_STRING([--disable-mxf],     [Disable Multiple - MXF]),                                     MediaInfoMxf=$enableval,     MediaInfoMxf=depend)
AC_ARG_ENABLE(nut,     AC_HELP_STRING([--disable-nut],     [Disable Multiple - Nut]),                                     MediaInfoNut=$enableval,     MediaInfoNut=depend)
AC_ARG_ENABLE(ogg,     AC_HELP_STRING([--disable-ogg],     [Disable Multiple - OGG (ogg, ogm)]),                          MediaInfoOgg=$enableval,     MediaInfoOgg=depend)
AC_ARG_ENABLE(pmp,     AC_HELP_STRING([--disable-pmp],     [Disable Multiple - Playstation Portable (PMP)]),              MediaInfoPmp=$enableval,     MediaInfoPmp=depend)
AC_ARG_ENABLE(riff,    AC_HELP_STRING([--disable-riff],    [Disable Multiple - RIFF based formats (AVI...)]),             MediaInfoRiff=$enableval,    MediaInfoRiff=depend)
AC_ARG_ENABLE(rm,      AC_HELP_STRING([--disable-rm],      [Disable Multiple - Real Media]),                              MediaInfoRm=$enableval,      MediaInfoRm=depend)
AC_ARG_ENABLE(skm,     AC_HELP_STRING([--disable-skm],     [Disable Multiple - SKM]),                                     MediaInfoSkm=$enableval,     MediaInfoSkm=depend)
AC_ARG_ENABLE(swf,     AC_HELP_STRING([--disable-swf],     [Disable Multiple - Shockwave]),                               MediaInfoSwf=$enableval,     MediaInfoSwf=depend)
AC_ARG_ENABLE(tsp,     AC_HELP_STRING([--disable-tsp],     [Disable Multiple - TSP]),                                     MediaInfoTsp=$enableval,     MediaInfoTsp=depend)
AC_ARG_ENABLE(wm,      AC_HELP_STRING([--disable-wm],      [Disable Multiple - Windows Media (ASF, WMV, WMA)]),           MediaInfoWm=$enableval,      MediaInfoWm=depend)
AC_ARG_ENABLE(apetag,  AC_HELP_STRING([--disable-apetag],  [Disable Tag - ApeTag]),                                       MediaInfoApeTag=$enableval,  MediaInfoApeTag=depend)
AC_ARG_ENABLE(Id3,     AC_HELP_STRING([--disable-Id3],     [Disable Tag - Id3]),                                          MediaInfoId3=$enableval,     MediaInfoId3=depend)
AC_ARG_ENABLE(Id3v2,   AC_HELP_STRING([--disable-Id3v2],   [Disable Tag - Id3v2]),                                        MediaInfoId3v2=$enableval,   MediaInfoId3v2=depend)
AC_ARG_ENABLE(Lyrics3, AC_HELP_STRING([--disable-Lyrics3], [Disable Tag - Lyrics3]),                                      MediaInfoLyrics3=$enableval, MediaInfoLyrics3=depend)
AC_ARG_ENABLE(Lyrics3v2, AC_HELP_STRING([--disable-Lyrics3v2], [Disable Tag - Lyrics3v2]),                                MediaInfoLyrics3v2=$enableval, MediaInfoLyrics3v2=depend)
AC_ARG_ENABLE(VorbisCom, AC_HELP_STRING([--disable-VorbisCom], [Disable Tag - VorbisCom]),                                MediaInfoVorbisCom=$enableval, MediaInfoVorbisCom=depend)
AC_ARG_ENABLE(cmml,    AC_HELP_STRING([--disable-cmml],    [Disable Text - CMML]),                                        MediaInfoCmml=$enableval,    MediaInfoCmml=depend)
AC_ARG_ENABLE(dvbsubtitle,  AC_HELP_STRING([--disable-dvbSsubtitle],  [Disable Text - DVB Subtitle]),                     MediaInfoDvbSubtitle=$enableval, MediaInfoDvbSubtitle=depend)
AC_ARG_ENABLE(aribstdb24b37, AC_HELP_STRING([--disable-aribstdb24],  [Disable Text - Closed captions (ARIB STD B24/B37)]), MediaInfoaribstdb24b37=$enableval, MediaInfoaribstdb24b37=depend)
AC_ARG_ENABLE(eia608,  AC_HELP_STRING([--disable-eia608],  [Disable Text - Closed captions (EIA-608)]),                   MediaInfoEia608=$enableval,  MediaInfoEia608=depend)
AC_ARG_ENABLE(eia708,  AC_HELP_STRING([--disable-eia708],  [Disable Text - Closed captions (EIA-708)]),                   MediaInfoEia708=$enableval,  MediaInfoEia708=depend)
AC_ARG_ENABLE(subrip,  AC_HELP_STRING([--disable-subrip],  [Disable Text - SubRip]),                                      MediaInfoSubRip=$enableval,  MediaInfoSubRip=depend)
AC_ARG_ENABLE(teletext,  AC_HELP_STRING([--disable-teletext],  [Disable Text - Teletext]),                                MediaInfoTeletext=$enableval, MediaInfoTeletext=depend)
AC_ARG_ENABLE(timedtext, AC_HELP_STRING([--disable-timedtext], [Disable Text - Timed Text (MPEG-4 Part 17)]),             MediaInfoTimedText=$enableval, MediaInfoTimedText=depend)
AC_ARG_ENABLE(ttml,    AC_HELP_STRING([--disable-ttml],    [Disable Text - TTML]),                                        MediaInfoTtml=$enableval,    MediaInfoTtml=depend)
AC_ARG_ENABLE(kate,    AC_HELP_STRING([--disable-kate],    [Disable Text - Kate]),                                        MediaInfoKate=$enableval,    MediaInfoKate=depend)
AC_ARG_ENABLE(pdf,     AC_HELP_STRING([--disable-pdf] ,    [Disable Text - PDF]),                                         MediaInfoPdf=$enableval,     MediaInfoPdf=depend)
AC_ARG_ENABLE(pgs,     AC_HELP_STRING([--disable-pgs] ,    [Disable Text - PGS]),                                         MediaInfoPgs=$enableval,     MediaInfoPgs=depend)
AC_ARG_ENABLE(scc,     AC_HELP_STRING([--disable-scc] ,    [Disable Text - SCC]),                                         MediaInfoScc=$enableval,     MediaInfoScc=depend)
AC_ARG_ENABLE(aic,     AC_HELP_STRING([--disable-aic],     [Disable Video - AIC (Apple Intermediate Codec)]),             MediaInfoAic=$enableval,     MediaInfoAic=depend)
AC_ARG_ENABLE(avc,     AC_HELP_STRING([--disable-avc],     [Disable Video - AVC/H264]),                                   MediaInfoAvc=$enableval,     MediaInfoAvc=depend)
AC_ARG_ENABLE(avsV,    AC_HELP_STRING([--disable-avsv],    [Disable Video - AVS (Chinese)]),                              MediaInfoAvsV=$enableval,    MediaInfoAvsV=depend)
AC_ARG_ENABLE(canopus, AC_HELP_STRING([--disable-canopus], [Disable Video - Canopus]),                                    MediaInfoCanopus=$enableval, MediaInfoCanopus=depend)
AC_ARG_ENABLE(ffv1,    AC_HELP_STRING([--disable-ffv1],    [Disable Video - FFV1]),                            			  MediaInfoFfv1=$enableval,    MediaInfoFfv1=depend)
AC_ARG_ENABLE(flic,    AC_HELP_STRING([--disable-flic],    [Disable Video - FLIC (fli, flc)]),                            MediaInfoFlic=$enableval,    MediaInfoFlic=depend)
AC_ARG_ENABLE(h263,    AC_HELP_STRING([--disable-h263],    [Disable Video - H.263]),                                      MediaInfoH263=$enableval,    MediaInfoH263=depend)
AC_ARG_ENABLE(hevc,    AC_HELP_STRING([--disable-hevc],    [Disable Video - HEVC/H265]),                                  MediaInfoHevc=$enableval,    MediaInfoHevc=depend)
AC_ARG_ENABLE(huffyuv, AC_HELP_STRING([--disable-huffyuv], [Disable Video - HuffYUV]),                                    MediaInfoHuffYUV=$enableval, MediaInfoHuffYUV=depend)
AC_ARG_ENABLE(mpeg4v,  AC_HELP_STRING([--disable-mpeg4v],  [Disable Video - MPEG-4 Video based formats (DivX, XviD...)]), MediaInfoMpeg4v=$enableval,  MediaInfoMpeg4v=depend)
AC_ARG_ENABLE(mpegv,   AC_HELP_STRING([--disable-mpegv],   [Disable Video - MPEG-Video]),                                 MediaInfoMpegv=$enableval,   MediaInfoMpegv=depend)
AC_ARG_ENABLE(prores,  AC_HELP_STRING([--disable-prores],  [Disable Video - ProRes]),                                     MediaInfoProRes=$enableval,  MediaInfoProRes=depend)
AC_ARG_ENABLE(theora,  AC_HELP_STRING([--disable-theora],  [Disable Video - Theora]),                                     MediaInfoTheora=$enableval,  MediaInfoTheora=depend)
AC_ARG_ENABLE(vc1,     AC_HELP_STRING([--disable-vc1],     [Disable Video - VC-1]),                                       MediaInfoVc1=$enableval,     MediaInfoVc1=depend)
AC_ARG_ENABLE(vc3,     AC_HELP_STRING([--disable-vc3],     [Disable Video - VC-3 (DNxHD)]),                               MediaInfoVc3=$enableval,     MediaInfoVc3=depend)
AC_ARG_ENABLE(vp8,     AC_HELP_STRING([--disable-vp8],     [Disable Video - VP8]),                                        MediaInfovp8=$enableval,     MediaInfovp8=depend)
AC_ARG_ENABLE(y4m,     AC_HELP_STRING([--disable-y4m],     [Disable Video - YUV4MPEG2]),                                  MediaInfoy4m=$enableval,     MediaInfoy4m=depend)

dnl -------------------------------------------------------------------------
dnl With
dnl
AC_ARG_WITH(macosx-sdk,         AC_HELP_STRING([--with-macosx-sdk],         [Force the Mac SDK]),                         , with_macosx_sdk=no)
AC_ARG_WITH(macosx-version-min, AC_HELP_STRING([--with-macosx-version-min], [Force the Mac Version]),                     , with_macosx_version_min=no)
AC_ARG_WITH(libz_static,        AC_HELP_STRING([--with-libz-static],        [Use static version of Zlib]),                , with_libz_static=no)
AC_ARG_WITH(libcurl,            AC_HELP_STRING([--with-libcurl],            [Use libcurl (specific directory)]),          , with_libcurl=no)
AC_ARG_WITH(libmms,             AC_HELP_STRING([--with-libmms],             [Use libmms (specific directory)]),           , with_libmms=no)
AC_ARG_WITH(libaes,             AC_HELP_STRING([--with-libaes],             [Use libaes (system)]),                       , with_libaes=depend)
AC_ARG_WITH(libmd5,             AC_HELP_STRING([--with-libmd5],             [Use libmd5 (system)]),                       , with_libmd5=depend)
AC_ARG_WITH(libtinyxml2,        AC_HELP_STRING([--with-libtinyxml2],        [Use libtinyxml2 (system)]),                  , with_libtinyxml2=depend)
AC_ARG_WITH(libmediainfo-name,  AC_HELP_STRING([--with-libmediainfo-name],  [Force libmediainfo filename]),               , with_libmediainfo_name=mediainfo)

dnl -------------------------------------------------------------------------
dnl MediaInfo *
dnl
if test "$MediaInfoMinimizeSize" = "yes";  then AC_DEFINE(MEDIAINFO_MINIMIZESIZE) fi
if test "$MediaInfoMinimal" = "no";  then AC_DEFINE(MEDIAINFO_MINIMAL_NO) fi; if test "$MediaInfoMinimal" = "yes"; then AC_DEFINE(MEDIAINFO_MINIMAL_YES) fi
if test "$MediaInfoTrace"   = "no";  then AC_DEFINE(MEDIAINFO_TRACE_NO)   fi; if test "$MediaInfoTrace"   = "yes"; then AC_DEFINE(MEDIAINFO_TRACE_YES)   fi
if test "$MediaInfoEvents"  = "no";  then AC_DEFINE(MEDIAINFO_EVENTS_NO)  fi; if test "$MediaInfoEvents"  = "yes"; then AC_DEFINE(MEDIAINFO_EVENTS_YES)  fi
if test "$MediaInfoDemux"   = "no";  then AC_DEFINE(MEDIAINFO_DEMUX_NO)   fi; if test "$MediaInfoDemux"   = "yes"; then AC_DEFINE(MEDIAINFO_DEMUX_YES)   fi
if test "$MediaInfoLibmmsDescribeSupport" = "yes"; then AC_DEFINE(MEDIAINFO_LIBMMS_DESCRIBE_SUPPORT)   fi
if test "$MediaInfoAll"     = "no";  then AC_DEFINE(MEDIAINFO_ALL_NO)     fi
if test "$MediaInfoArchive" = "no";  then AC_DEFINE(MEDIAINFO_ARCHIVE_NO) fi
if test "$MediaInfoAudio"   = "no";  then AC_DEFINE(MEDIAINFO_AUDIO_NO)   fi
if test "$MediaInfoImage"   = "no";  then AC_DEFINE(MEDIAINFO_IMAGE_NO)   fi
if test "$MediaInfoMulti"   = "no";  then AC_DEFINE(MEDIAINFO_MULTI_NO)   fi
if test "$MediaInfoText"    = "no";  then AC_DEFINE(MEDIAINFO_TAG_NO)     fi
if test "$MediaInfoText"    = "no";  then AC_DEFINE(MEDIAINFO_TEXT_NO)    fi
if test "$MediaInfoVideo"   = "no";  then AC_DEFINE(MEDIAINFO_VIDEO_NO)   fi
if test "$MediaInfo7z"      = "no";  then AC_DEFINE(MEDIAINFO_7Z_NO)      fi; if test "$MediaInfo7z"      = "yes"; then AC_DEFINE(MEDIAINFO_7Z_YES)      fi
if test "$MediaInfoAce"     = "no";  then AC_DEFINE(MEDIAINFO_ACE_NO)     fi; if test "$MediaInfoAce"     = "yes"; then AC_DEFINE(MEDIAINFO_ACE_YES)     fi
if test "$MediaInfoBzip2"   = "no";  then AC_DEFINE(MEDIAINFO_BZIP2_NO)   fi; if test "$MediaInfoBzip2"   = "yes"; then AC_DEFINE(MEDIAINFO_BZIP2_YES)   fi
if test "$MediaInfoElf"     = "no";  then AC_DEFINE(MEDIAINFO_ELF_NO)     fi; if test "$MediaInfoElf"     = "yes"; then AC_DEFINE(MEDIAINFO_ELF_YES)     fi
if test "$MediaInfoGzip"    = "no";  then AC_DEFINE(MEDIAINFO_GZIP_NO)    fi; if test "$MediaInfoGzip"    = "yes"; then AC_DEFINE(MEDIAINFO_GZIP_YES)    fi
if test "$MediaInfoIso9660" = "no";  then AC_DEFINE(MEDIAINFO_ISO9660_NO) fi; if test "$MediaInfoIso9660" = "yes"; then AC_DEFINE(MEDIAINFO_ISO9660_YES) fi
if test "$MediaInfoMz"      = "no";  then AC_DEFINE(MEDIAINFO_MZ_NO)      fi; if test "$MediaInfoMz"      = "yes"; then AC_DEFINE(MEDIAINFO_MZ_YES)      fi
if test "$MediaInfoRar"     = "no";  then AC_DEFINE(MEDIAINFO_RAR_NO)     fi; if test "$MediaInfoRar"     = "yes"; then AC_DEFINE(MEDIAINFO_RAR_YES)     fi
if test "$MediaInfoTar"     = "no";  then AC_DEFINE(MEDIAINFO_TAR_NO)     fi; if test "$MediaInfoTar"     = "yes"; then AC_DEFINE(MEDIAINFO_TAR_YES)     fi
if test "$MediaInfoZip"     = "no";  then AC_DEFINE(MEDIAINFO_ZIP_NO)     fi; if test "$MediaInfoZip"     = "yes"; then AC_DEFINE(MEDIAINFO_ZIP_YES)     fi
if test "$MediaInfoAac"     = "no";  then AC_DEFINE(MEDIAINFO_AAC_NO)     fi; if test "$MediaInfoAac"     = "yes"; then AC_DEFINE(MEDIAINFO_AAC_YES)     fi
if test "$MediaInfoAc3"     = "no";  then AC_DEFINE(MEDIAINFO_AC3_NO)     fi; if test "$MediaInfoAc3"     = "yes"; then AC_DEFINE(MEDIAINFO_AC3_YES)     fi
if test "$MediaInfoAdif"    = "no";  then AC_DEFINE(MEDIAINFO_ADIF_NO)    fi; if test "$MediaInfoAdif"    = "yes"; then AC_DEFINE(MEDIAINFO_ADIF_YES)    fi
if test "$MediaInfoAdpcm"   = "no";  then AC_DEFINE(MEDIAINFO_ADPCM_NO)   fi; if test "$MediaInfoAdpcm"   = "yes"; then AC_DEFINE(MEDIAINFO_ADPCM_YES)   fi
if test "$MediaInfoAdts"    = "no";  then AC_DEFINE(MEDIAINFO_ADTS_NO)    fi; if test "$MediaInfoAdts"    = "yes"; then AC_DEFINE(MEDIAINFO_ADTS_YES)    fi
if test "$MediaInfoAes3"    = "no";  then AC_DEFINE(MEDIAINFO_AES3_NO)    fi; if test "$MediaInfoAes3"    = "yes"; then AC_DEFINE(MEDIAINFO_AES3_YES)    fi
if test "$MediaInfoAls"     = "no";  then AC_DEFINE(MEDIAINFO_ALS_NO)     fi; if test "$MediaInfoAls"     = "yes"; then AC_DEFINE(MEDIAINFO_ALS_YES)     fi
if test "$MediaInfoAmr"     = "no";  then AC_DEFINE(MEDIAINFO_AMR_NO)     fi; if test "$MediaInfoAmr"     = "yes"; then AC_DEFINE(MEDIAINFO_AMR_YES)     fi
if test "$MediaInfoAmv"     = "no";  then AC_DEFINE(MEDIAINFO_AMV_NO)     fi; if test "$MediaInfoAmv"     = "yes"; then AC_DEFINE(MEDIAINFO_AMV_YES)     fi
if test "$MediaInfoApe"     = "no";  then AC_DEFINE(MEDIAINFO_APE_NO)     fi; if test "$MediaInfoApe"     = "yes"; then AC_DEFINE(MEDIAINFO_APE_YES)     fi
if test "$MediaInfoAu"      = "no";  then AC_DEFINE(MEDIAINFO_AU_NO)      fi; if test "$MediaInfoAu"      = "yes"; then AC_DEFINE(MEDIAINFO_AU_YES)      fi
if test "$MediaInfoCelt"    = "no";  then AC_DEFINE(MEDIAINFO_CELT_NO)    fi; if test "$MediaInfoCelt"    = "yes"; then AC_DEFINE(MEDIAINFO_CELT_YES)    fi
if test "$MediaInfoDts"     = "no";  then AC_DEFINE(MEDIAINFO_DTS_NO)     fi; if test "$MediaInfoDts"     = "yes"; then AC_DEFINE(MEDIAINFO_DTS_YES)     fi
if test "$MediaInfoFlac"    = "no";  then AC_DEFINE(MEDIAINFO_FLAC_NO)    fi; if test "$MediaInfoFlac"    = "yes"; then AC_DEFINE(MEDIAINFO_FLAC_YES)    fi
if test "$MediaInfoLa"      = "no";  then AC_DEFINE(MEDIAINFO_LA_NO)      fi; if test "$MediaInfoLa"      = "yes"; then AC_DEFINE(MEDIAINFO_LA_YES)      fi
if test "$MediaInfoLatm"    = "no";  then AC_DEFINE(MEDIAINFO_LATM_NO)    fi; if test "$MediaInfoLatm"    = "yes"; then AC_DEFINE(MEDIAINFO_LATM_YES)    fi
if test "$MediaInfoMidi"    = "no";  then AC_DEFINE(MEDIAINFO_MIDO_NO)    fi; if test "$MediaInfoMidi"    = "yes"; then AC_DEFINE(MEDIAINFO_MIDI_YES)    fi
if test "$MediaInfoMpc"     = "no";  then AC_DEFINE(MEDIAINFO_MPC_NO)     fi; if test "$MediaInfoMpc"     = "yes"; then AC_DEFINE(MEDIAINFO_MPC_YES)     fi
if test "$MediaInfoMpcSv8"  = "no";  then AC_DEFINE(MEDIAINFO_MPCSV8_NO)  fi; if test "$MediaInfoMpcSv8"  = "yes"; then AC_DEFINE(MEDIAINFO_MPCSV8_YES)  fi
if test "$MediaInfoMpega"   = "no";  then AC_DEFINE(MEDIAINFO_MPEGA_NO)   fi; if test "$MediaInfoMpega"   = "yes"; then AC_DEFINE(MEDIAINFO_MPEGA_YES)   fi
if test "$MediaInfoOpenMG"  = "no";  then AC_DEFINE(MEDIAINFO_OPENMG_NO)  fi; if test "$MediaInfoOpenMG"  = "yes"; then AC_DEFINE(MEDIAINFO_OPENMG_YES)  fi
if test "$MediaInfoPcm"     = "no";  then AC_DEFINE(MEDIAINFO_PCM_NO)     fi; if test "$MediaInfoPcm"     = "yes"; then AC_DEFINE(MEDIAINFO_PCM_YES)     fi
if test "$MediaInfoPs2a"    = "no";  then AC_DEFINE(MEDIAINFO_PS2A_NO)    fi; if test "$MediaInfoPs2a"    = "yes"; then AC_DEFINE(MEDIAINFO_PS2A_YES)    fi
if test "$MediaInfoRkau"    = "no";  then AC_DEFINE(MEDIAINFO_RKAU_NO)    fi; if test "$MediaInfoRkau"    = "yes"; then AC_DEFINE(MEDIAINFO_RKAU_YES)    fi
if test "$MediaInfoSpeex"   = "no";  then AC_DEFINE(MEDIAINFO_SPEEX_NO)   fi; if test "$MediaInfoSpeex"   = "yes"; then AC_DEFINE(MEDIAINFO_SPEEX_YES)   fi
if test "$MediaInfoTak"     = "no";  then AC_DEFINE(MEDIAINFO_TAK_NO)     fi; if test "$MediaInfoTak"     = "yes"; then AC_DEFINE(MEDIAINFO_TAK_YES)     fi
if test "$MediaInfoTta"     = "no";  then AC_DEFINE(MEDIAINFO_TTA_NO)     fi; if test "$MediaInfoTta"     = "yes"; then AC_DEFINE(MEDIAINFO_TTA_YES)     fi
if test "$MediaInfoTwinVQ"  = "no";  then AC_DEFINE(MEDIAINFO_TWINVQ_NO)  fi; if test "$MediaInfoTwinVQ"  = "yes"; then AC_DEFINE(MEDIAINFO_TWINVQ_YES)  fi
if test "$MediaInfoVorbis"  = "no";  then AC_DEFINE(MEDIAINFO_VORBIS_NO)  fi; if test "$MediaInfoVorbis"  = "yes"; then AC_DEFINE(MEDIAINFO_VORBIS_YES)  fi
if test "$MediaInfoWvpk"    = "no";  then AC_DEFINE(MEDIAINFO_WVPK_NO)    fi; if test "$MediaInfoWvpk"    = "yes"; then AC_DEFINE(MEDIAINFO_WVPK_YES)    fi
if test "$MediaInfoArriRaw" = "no";  then AC_DEFINE(MEDIAINFO_ARRIRAW_NO) fi; if test "$MediaInfoArriRaw" = "yes"; then AC_DEFINE(MEDIAINFO_ARRIRAW_YES) fi
if test "$MediaInfoBmp"     = "no";  then AC_DEFINE(MEDIAINFO_BMP_NO)     fi; if test "$MediaInfoBmp"     = "yes"; then AC_DEFINE(MEDIAINFO_BMP_YES)     fi
if test "$MediaInfoDds"     = "no";  then AC_DEFINE(MEDIAINFO_DDS_NO)     fi; if test "$MediaInfoDds"     = "yes"; then AC_DEFINE(MEDIAINFO_DDS_YES)     fi
if test "$MediaInfoDpx"     = "no";  then AC_DEFINE(MEDIAINFO_DPX_NO)     fi; if test "$MediaInfoDpx"     = "yes"; then AC_DEFINE(MEDIAINFO_DPX_YES)     fi
if test "$MediaInfoExr"     = "no";  then AC_DEFINE(MEDIAINFO_EXR_NO)     fi; if test "$MediaInfoExr"     = "yes"; then AC_DEFINE(MEDIAINFO_EXR_YES)     fi
if test "$MediaInfoGif"     = "no";  then AC_DEFINE(MEDIAINFO_GIF_NO)     fi; if test "$MediaInfoGif"     = "yes"; then AC_DEFINE(MEDIAINFO_GIF_YES)     fi
if test "$MediaInfoIco"     = "no";  then AC_DEFINE(MEDIAINFO_ICO_NO)     fi; if test "$MediaInfoIco"     = "yes"; then AC_DEFINE(MEDIAINFO_ICO_YES)     fi
if test "$MediaInfoJpeg"    = "no";  then AC_DEFINE(MEDIAINFO_JPEG_NO)    fi; if test "$MediaInfoJpeg"    = "yes"; then AC_DEFINE(MEDIAINFO_JPEG_YES)    fi
if test "$MediaInfoPcx"     = "no";  then AC_DEFINE(MEDIAINFO_PCX_NO)     fi; if test "$MediaInfoPcx"     = "yes"; then AC_DEFINE(MEDIAINFO_PCX_YES)     fi
if test "$MediaInfoPng"     = "no";  then AC_DEFINE(MEDIAINFO_PNG_NO)     fi; if test "$MediaInfoPng"     = "yes"; then AC_DEFINE(MEDIAINFO_PNG_YES)     fi
if test "$MediaInfoPsd"     = "no";  then AC_DEFINE(MEDIAINFO_PSD_NO)     fi; if test "$MediaInfoPsd"     = "yes"; then AC_DEFINE(MEDIAINFO_PSD_YES)     fi
if test "$MediaInfoTiff"    = "no";  then AC_DEFINE(MEDIAINFO_TIFF_NO)    fi; if test "$MediaInfoTiff"    = "yes"; then AC_DEFINE(MEDIAINFO_TIFF_YES)    fi
if test "$MediaInfoAaf"     = "no";  then AC_DEFINE(MEDIAINFO_AAF_NO)     fi; if test "$MediaInfoAaf"     = "yes"; then AC_DEFINE(MEDIAINFO_AAF_YES)     fi
if test "$MediaInfoBdav"    = "no";  then AC_DEFINE(MEDIAINFO_BDAV_NO)    fi; if test "$MediaInfoBdav"    = "yes"; then AC_DEFINE(MEDIAINFO_BDAV_YES)    fi
if test "$MediaInfoBdmv"    = "no";  then AC_DEFINE(MEDIAINFO_BDMV_NO)    fi; if test "$MediaInfoBdmv"    = "yes"; then AC_DEFINE(MEDIAINFO_BDMV_YES)    fi
if test "$MediaInfoCdxa"    = "no";  then AC_DEFINE(MEDIAINFO_CDXA_NO)    fi; if test "$MediaInfoCdxa"    = "yes"; then AC_DEFINE(MEDIAINFO_CDXA_YES)    fi
if test "$MediaInfoDashMpd" = "no";  then AC_DEFINE(MEDIAINFO_DASHMPD_NO) fi; if test "$MediaInfoDashMpd" = "yes"; then AC_DEFINE(MEDIAINFO_DASHMPD_YES) fi
if test "$MediaInfoDcp"     = "no";  then AC_DEFINE(MEDIAINFO_DCP_NO)     fi; if test "$MediaInfoDcp"     = "yes"; then AC_DEFINE(MEDIAINFO_DCP_YES)     fi
if test "$MediaInfoDpg"     = "no";  then AC_DEFINE(MEDIAINFO_DPG_NO)     fi; if test "$MediaInfoDpg"     = "yes"; then AC_DEFINE(MEDIAINFO_DPG_YES)     fi
if test "$MediaInfoReferences" = "no";  then AC_DEFINE(MEDIAINFO_REFERENCES_NO)   fi; if test "$MediaInfoReferences" = "yes"; then AC_DEFINE(MEDIAINFO_REFERENCES_YES)   fi
if test "$MediaInfoDvDif"   = "no";  then AC_DEFINE(MEDIAINFO_DVDIF_NO)   fi; if test "$MediaInfoDvDif"   = "yes"; then AC_DEFINE(MEDIAINFO_DVDIF_YES)   fi
if test "$MediaInfoDvDifAnalyze" = "no";  then AC_DEFINE(MEDIAINFO_DVDIF_ANALYZE_NO)   fi; if test "$MediaInfoDvDifAnalyze" = "yes"; then AC_DEFINE(MEDIAINFO_DVDIF_ANALYZE_YES) fi
if test "$MediaInfoDvdv"    = "no";  then AC_DEFINE(MEDIAINFO_DVDV_NO)    fi; if test "$MediaInfoDvdv"    = "yes"; then AC_DEFINE(MEDIAINFO_DVDV_YES)    fi
if test "$MediaInfoDxw"     = "no";  then AC_DEFINE(MEDIAINFO_DXW_NO)     fi; if test "$MediaInfoDxw"     = "yes"; then AC_DEFINE(MEDIAINFO_DXW_YES)     fi
if test "$MediaInfoFlv"     = "no";  then AC_DEFINE(MEDIAINFO_FLV_NO)     fi; if test "$MediaInfoFlv"     = "yes"; then AC_DEFINE(MEDIAINFO_FLV_YES)     fi
if test "$MediaInfoGxf"     = "no";  then AC_DEFINE(MEDIAINFO_GXF_NO)     fi; if test "$MediaInfoGxf"     = "yes"; then AC_DEFINE(MEDIAINFO_GXF_YES)     fi
if test "$MediaInfohdsF4m"  = "no";  then AC_DEFINE(MEDIAINFO_HDSF4M_NO)  fi; if test "$MediaInfoHdsF4m"  = "yes"; then AC_DEFINE(MEDIAINFO_HDSF4M_YES)  fi
if test "$MediaInfoHls"     = "no";  then AC_DEFINE(MEDIAINFO_HLS_NO)     fi; if test "$MediaInfoHls"     = "yes"; then AC_DEFINE(MEDIAINFO_HLS_YES)     fi
if test "$MediaInfoMk"      = "no";  then AC_DEFINE(MEDIAINFO_MK_NO)      fi; if test "$MediaInfoMk"      = "yes"; then AC_DEFINE(MEDIAINFO_MK_YES)      fi
if test "$MediaInfoMpeg4"   = "no";  then AC_DEFINE(MEDIAINFO_MPEG4_NO)   fi; if test "$MediaInfoMpeg4"   = "yes"; then AC_DEFINE(MEDIAINFO_MPEG4_YES)   fi
if test "$MediaInfoMpegPs"  = "no";  then AC_DEFINE(MEDIAINFO_MPEGPS_NO)  fi; if test "$MediaInfoMpegPs"  = "yes"; then AC_DEFINE(MEDIAINFO_MPEGPS_YES)  fi
if test "$MediaInfoMpegTs"  = "no";  then AC_DEFINE(MEDIAINFO_MPEGTS_NO)  fi; if test "$MediaInfoMpegTs"  = "yes"; then AC_DEFINE(MEDIAINFO_MPEGTS_YES)  fi
if test "$MediaInfoMpegTsPcr" = "no"; then AC_DEFINE(MEDIAINFO_MPEGTS_PCR_NO)  fi; if test "$MediaInfoMpegTs" = "yes"; then AC_DEFINE(MEDIAINFO_MPEGTS_PCR_YES) fi
if test "$MediaInfoMpegTsPesTimeStamp" = "no"; then AC_DEFINE(MEDIAINFO_MPEGTS_PESTIMESTAMP_NO)  fi; if test "$MediaInfoMpegTs" = "yes"; then AC_DEFINE(MEDIAINFO_MPEGTS_PESTIMESTAMP_YES) fi
if test "$MediaInfoMxf"     = "no";  then AC_DEFINE(MEDIAINFO_MXF_NO)     fi; if test "$MediaInfoMxf"     = "yes"; then AC_DEFINE(MEDIAINFO_MXF_YES)     fi
if test "$MediaInfoNut"     = "no";  then AC_DEFINE(MEDIAINFO_NUT_NO)     fi; if test "$MediaInfoNut"     = "yes"; then AC_DEFINE(MEDIAINFO_NUT_YES)     fi
if test "$MediaInfoOgg"     = "no";  then AC_DEFINE(MEDIAINFO_OGG_NO)     fi; if test "$MediaInfoOgg"     = "yes"; then AC_DEFINE(MEDIAINFO_OGG_YES)     fi
if test "$MediaInfoPmp"     = "no";  then AC_DEFINE(MEDIAINFO_PMP_NO)     fi; if test "$MediaInfoPmp"     = "yes"; then AC_DEFINE(MEDIAINFO_PMP_YES)     fi
if test "$MediaInfoRiff"    = "no";  then AC_DEFINE(MEDIAINFO_RIFF_NO)    fi; if test "$MediaInfoRiff"    = "yes"; then AC_DEFINE(MEDIAINFO_RIFF_YES)    fi
if test "$MediaInfoRm"      = "no";  then AC_DEFINE(MEDIAINFO_RM_NO)      fi; if test "$MediaInfoRm"      = "yes"; then AC_DEFINE(MEDIAINFO_RM_YES)      fi
if test "$MediaInfoSkm"     = "no";  then AC_DEFINE(MEDIAINFO_SKM_NO)     fi; if test "$MediaInfoSkm"     = "yes"; then AC_DEFINE(MEDIAINFO_SKM_YES)     fi
if test "$MediaInfoSwf"     = "no";  then AC_DEFINE(MEDIAINFO_SWF_NO)     fi; if test "$MediaInfoSwf"     = "yes"; then AC_DEFINE(MEDIAINFO_SWF_YES)     fi
if test "$MediaInfoTsp"     = "no";  then AC_DEFINE(MEDIAINFO_TSP_NO)     fi; if test "$MediaInfoTsp"     = "yes"; then AC_DEFINE(MEDIAINFO_TSP_YES)     fi
if test "$MediaInfoWm"      = "no";  then AC_DEFINE(MEDIAINFO_WM_NO)      fi; if test "$MediaInfoWm"      = "yes"; then AC_DEFINE(MEDIAINFO_WM_YES)      fi
if test "$MediaInfoApeTag"  = "no";  then AC_DEFINE(MEDIAINFO_APETAG_NO)  fi; if test "$MediaInfoApeTag"  = "yes"; then AC_DEFINE(MEDIAINFO_APETAG_YES)  fi
if test "$MediaInfoId3"     = "no";  then AC_DEFINE(MEDIAINFO_ID3_NO)     fi; if test "$MediaInfoId3"     = "yes"; then AC_DEFINE(MEDIAINFO_ID3_YES)     fi
if test "$MediaInfoId3v2"   = "no";  then AC_DEFINE(MEDIAINFO_ID3V2_NO)   fi; if test "$MediaInfoId3v2"   = "yes"; then AC_DEFINE(MEDIAINFO_ID3V2_YES)   fi
if test "$MediaInfoLyrics3" = "no";  then AC_DEFINE(MEDIAINFO_LYRICS3_NO) fi; if test "$MediaInfoLyrics3" = "yes"; then AC_DEFINE(MEDIAINFO_LYRICS3_YES) fi
if test "$MediaInfoLyrics3v2" = "no";  then AC_DEFINE(MEDIAINFO_LYRICS3V2_NO) fi; if test "$MediaInfoLyrics3v2" = "yes"; then AC_DEFINE(MEDIAINFO_LYRICS3V2_YES) fi
if test "$MediaInfoVorbisCom" = "no";  then AC_DEFINE(MEDIAINFO_VORBISCOM_NO) fi; if test "$MediaInfoVorbisCom" = "yes"; then AC_DEFINE(MEDIAINFO_VORBISCOM_YES) fi
if test "$MediaInfoCmml"    = "no";  then AC_DEFINE(MEDIAINFO_CMML_NO)    fi; if test "$MediaInfoCmml"    = "yes"; then AC_DEFINE(MEDIAINFO_CMML_YES)    fi
if test "$MediaInfoDvbSubtitle" = "no"; then AC_DEFINE(MEDIAINFO_DVBSUBTITLE_NO) fi; if test "$MediaInfoDvbSubtitle" = "yes"; then AC_DEFINE(MEDIAINFO_DVBSUBTITLE_YES)  fi
if test "$MediaInfoaribstdb24b37" = "no"; then AC_DEFINE(MEDIAINFO_ARIBSTDB24B37_NO) fi; if test "$MediaInfoaribstdb24b37" = "yes"; then AC_DEFINE(MEDIAINFO_ARIBSTDB24B37_YES) fi
if test "$MediaInfoEia608"  = "no";  then AC_DEFINE(MEDIAINFO_EIA608_NO)  fi; if test "$MediaInfoEia608"  = "yes"; then AC_DEFINE(MEDIAINFO_EIA608_YES)  fi
if test "$MediaInfoEia708"  = "no";  then AC_DEFINE(MEDIAINFO_EIA708_NO)  fi; if test "$MediaInfoEia708"  = "yes"; then AC_DEFINE(MEDIAINFO_EIA708_YES)  fi
if test "$MediaInfoSubRip"  = "no";  then AC_DEFINE(MEDIAINFO_SUBRIP_NO)  fi; if test "$MediaInfoSubRip"  = "yes"; then AC_DEFINE(MEDIAINFO_SUBRIP_YES) fi
if test "$MediaInfoTeletext" = "no";  then AC_DEFINE(MEDIAINFO_TELETEXT_NO) fi; if test "$MediaInfoTeletext" = "yes"; then AC_DEFINE(MEDIAINFO_TELETEXT_YES) fi
if test "$MediaInfoTimedText" = "no";  then AC_DEFINE(MEDIAINFO_TIMEDTEXT_NO)    fi; if test "$MediaInfoTimedText"  = "yes"; then AC_DEFINE(MEDIAINFO_TIMEDTEXT_YES) fi
if test "$MediaInfoTtml"    = "no";  then AC_DEFINE(MEDIAINFO_TTML_NO)    fi; if test "$MediaInfoTtml"    = "yes"; then AC_DEFINE(MEDIAINFO_TTML_YES) fi
if test "$MediaInfoKate"    = "no";  then AC_DEFINE(MEDIAINFO_KATE_NO)    fi; if test "$MediaInfoKate"    = "yes"; then AC_DEFINE(MEDIAINFO_KATE_YES)    fi
if test "$MediaInfoPdf"     = "no";  then AC_DEFINE(MEDIAINFO_PDF_NO)     fi; if test "$MediaInfoPdf"     = "yes"; then AC_DEFINE(MEDIAINFO_PDF_YES)     fi
if test "$MediaInfoPgs"     = "no";  then AC_DEFINE(MEDIAINFO_PGS_NO)     fi; if test "$MediaInfoPgs"     = "yes"; then AC_DEFINE(MEDIAINFO_PGS_YES)     fi
if test "$MediaInfoScc"     = "no";  then AC_DEFINE(MEDIAINFO_SCC_NO)     fi; if test "$MediaInfoScc"     = "yes"; then AC_DEFINE(MEDIAINFO_SCC_YES)     fi
if test "$MediaInfoAic"     = "no";  then AC_DEFINE(MEDIAINFO_AIC_NO)     fi; if test "$MediaInfoAic"     = "yes"; then AC_DEFINE(MEDIAINFO_AIC_YES)     fi
if test "$MediaInfoAvc"     = "no";  then AC_DEFINE(MEDIAINFO_AVC_NO)     fi; if test "$MediaInfoAvc"     = "yes"; then AC_DEFINE(MEDIAINFO_AVC_YES)     fi
if test "$MediaInfoAvsV"    = "no";  then AC_DEFINE(MEDIAINFO_AVSV_NO)    fi; if test "$MediaInfoAvsV"    = "yes"; then AC_DEFINE(MEDIAINFO_AVSV_YES)    fi
if test "$MediaInfoCanopus" = "no";  then AC_DEFINE(MEDIAINFO_CANOPUS_NO) fi; if test "$MediaInfoCanopus" = "yes"; then AC_DEFINE(MEDIAINFO_CANOPUS_YES) fi
if test "$MediaInfoFfv1"    = "no";  then AC_DEFINE(MEDIAINFO_FFV1_NO)    fi; if test "$MediaInfoFfv1"    = "yes"; then AC_DEFINE(MEDIAINFO_FFV1_YES)    fi
if test "$MediaInfoFlic"    = "no";  then AC_DEFINE(MEDIAINFO_FLIC_NO)    fi; if test "$MediaInfoFlic"    = "yes"; then AC_DEFINE(MEDIAINFO_FLIC_YES)    fi
if test "$MediaInfoH263"    = "no";  then AC_DEFINE(MEDIAINFO_H263_NO)    fi; if test "$MediaInfoH263"    = "yes"; then AC_DEFINE(MEDIAINFO_H263_YES)    fi
if test "$MediaInfoHevc"    = "no";  then AC_DEFINE(MEDIAINFO_HEVC_NO)    fi; if test "$MediaInfoHevc"    = "yes"; then AC_DEFINE(MEDIAINFO_HEVC_YES)    fi
if test "$MediaInfoHuffYUV" = "no";  then AC_DEFINE(MEDIAINFO_HUFFYUV_NO) fi; if test "$MediaInfoHuffYUV" = "yes"; then AC_DEFINE(MEDIAINFO_HUFFYUV_YES) fi
if test "$MediaInfoMpeg4v"  = "no";  then AC_DEFINE(MEDIAINFO_MPEG4V_NO)  fi; if test "$MediaInfoMpeg4v"  = "yes"; then AC_DEFINE(MEDIAINFO_MPEG4V_YES)  fi
if test "$MediaInfoMpegv"   = "no";  then AC_DEFINE(MEDIAINFO_MPEGV_NO)   fi; if test "$MediaInfoMpegv"   = "yes"; then AC_DEFINE(MEDIAINFO_MPEGV_YES)   fi
if test "$MediaInfoProRes"  = "no";  then AC_DEFINE(MEDIAINFO_PRORES_NO)  fi; if test "$MediaInfoProRes"  = "yes"; then AC_DEFINE(MEDIAINFO_PRORES_YES)  fi
if test "$MediaInfoTheora"  = "no";  then AC_DEFINE(MEDIAINFO_THEORA_NO)  fi; if test "$MediaInfoTheora"  = "yes"; then AC_DEFINE(MEDIAINFO_THEORA_YES)  fi
if test "$MediaInfoVc1"     = "no";  then AC_DEFINE(MEDIAINFO_VC1_NO)     fi; if test "$MediaInfoVc1"     = "yes"; then AC_DEFINE(MEDIAINFO_VC1_YES)     fi
if test "$MediaInfoVc3"     = "no";  then AC_DEFINE(MEDIAINFO_VC3_NO)     fi; if test "$MediaInfoVc3"     = "yes"; then AC_DEFINE(MEDIAINFO_VC3_YES)     fi
if test "$MediaInfoVp8"     = "no";  then AC_DEFINE(MEDIAINFO_VP8_NO)     fi; if test "$MediaInfoVp8"     = "yes"; then AC_DEFINE(MEDIAINFO_VP8_YES)     fi
if test "$MediaInfoY4m"     = "no";  then AC_DEFINE(MEDIAINFO_Y4M_NO)     fi; if test "$MediaInfoY4m"     = "yes"; then AC_DEFINE(MEDIAINFO_Y4M_YES)     fi

dnl #########################################################################
dnl ### Options from elsewhere
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/libzen-config; then
	enable_unicode="$(../../../../ZenLib/Project/GNU/Library/libzen-config Unicode)"
elif test "$(command -v libzen-config)" ; then
	enable_unicode="$(libzen-config Unicode)"
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
dnl zlib
dnl

if test "$with_libz_static" != "yes"; then
	AC_CHECK_LIB(z, uncompress, [have_libz="yes"], [have_libz="no"])
	AC_CHECK_HEADERS(zlib.h, [], [have_libz="no"])
else
	have_libz="no"
fi
if test "$have_libz" != "yes"; then
	if test -e ../../../../Shared/Source/zlib/libz.a || test -e ../../../../Shared/Source/zlib/libz.la; then
		with_zlib="builtin"
	else
		AC_MSG_NOTICE([zlib is not found, trying to download and compile it])
		if test -e ../../../../Shared/Project/zlib/Compile.sh; then
			cd ../../../../Shared/Project/zlib
			chmod u+x Compile.sh
			./Compile.sh --build=$build_alias
			cd $Home
			if test -e ../../../../Shared/Source/zlib/libz.a || test -e ../../../../Shared/Source/zlib/libz.la; then
				with_zlib="builtin"
			else
				with_zlib="no"
				AC_MSG_ERROR([Problem while compiling builtin zlib])
			fi
		else
			with_zlib="no"
			AC_MSG_ERROR([zlib is not found, please install development package of zlib])
		fi
	fi
	CXXFLAGS="$CXXFLAGS -I../../../../Shared/Source/zlib"
	LIBS="$LIBS -L../../../../Shared/Source/zlib"
	MediaInfoLib_LIBS="$MediaInfoLib_LIBS -L../../../../Shared/Source/zlib -lz"
	MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static -L../../../../Shared/Source/zlib -lz"
else
    with_zlib="system"
    LIBS="$LIBS -lz"
    MediaInfoLib_LIBS="$MediaInfoLib_LIBS -lz"
    MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static -lz"
fi

dnl -------------------------------------------------------------------------
dnl ZenLib
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/libzen.la; then
	CXXFLAGS="$CXXFLAGS $(../../../../ZenLib/Project/GNU/Library/libzen-config CXXFLAGS)"
	if test "$enable_staticlibs" = "yes"; then
		with_zenlib="builtin (static)"
		LIBS="$LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS_Static)"
	else
		with_zenlib="builtin"
		LIBS="$LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
	fi
elif test "$(command -v libzen-config)" ; then
	CXXFLAGS="$CXXFLAGS $(libzen-config CXXFLAGS)"
	if test "$enable_staticlibs" = "yes"; then
		with_zenlib="system (static)"
		LIBS="$LIBS $(libzen-config LIBS_Static)"
	else
		with_zenlib="system"
		LIBS="$LIBS $(libzen-config LIBS)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(libzen-config LIBS)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(libzen-config LIBS)"
	fi
else
	if pkg-config --exists libzen; then
		CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libzen)"
		if test "$enable_staticlibs" = "yes"; then
			with_zenlib="system (static)"
			LIBS="$LIBS $(pkg-config --variable=LIBS_Static libzen)"
		else
			with_zenlib="system"
			LIBS="$LIBS $(pkg-config --libs libzen)"
			MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs libzen)"
			MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(pkg-config --variable=LIBS libzen)"
		fi
	else
		AC_MSG_ERROR([libzen configuration is not found])
	fi
fi

dnl -------------------------------------------------------------------------
dnl libcurl
dnl
if test -d $with_libcurl; then
	if test -f $with_libcurl/libcurl.pc; then
		libcurlpcfile="$with_libcurl/libcurl.pc"
	else
		if test -f "$with_libcurl/lib/pkgconfig/libcurl.pc"; then
			libcurlpcfile="$with_libcurl/lib/pkgconfig/libcurl.pc"
		else
			AC_MSG_ERROR([Problem while configuring builtin curl (libcurl.pc not found)])
		fi
	fi
	CXXFLAGS="$CXXFLAGS $(pkg-config --cflags $libcurlpcfile)"
	if test "$enable_staticlibs" = "yes"; then
		using_libcurl="custom (static)"
		LIBS="$LIBS $(pkg-config --libs --static $libcurlpcfile)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(pkg-config --libs --static $libcurlpcfile)"
	else
		using_libcurl="custom"
		LIBS="$LIBS $with_libcurl/lib $(pkg-config --libs $libcurlpcfile)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs $libcurlpcfile)"
	fi
elif test "$with_libcurl" = "no"; then
	CXXFLAGS="$CXXFLAGS -DMEDIAINFO_LIBCURL_NO"
	using_libcurl="no"
elif test -e ../../../../curl/libcurl.pc; then
	CXXFLAGS="$CXXFLAGS -I../../../../curl/include"
	if test "$enable_staticlibs" = "yes"; then
		using_libcurl="builtin (static)"
		LIBS="$LIBS $(pkg-config --libs --static ../../../../curl/libcurl.pc)"
	else
		using_libcurl="builtin"
		LIBS="$LIBS $(pkg-config --libs ../../../../curl/libcurl.pc)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs ../../../../curl/libcurl.pc)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(pkg-config --libs ../../../../curl/libcurl.pc)"
	fi
elif pkg-config --exists libcurl; then
	CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libcurl)"
	if test "$enable_staticlibs" = "yes"; then
		using_libcurl="system (static)"
		LIBS="$LIBS $(pkg-config --libs --static libcurl)"
	else
		using_libcurl="system"
		LIBS="$LIBS $(pkg-config --libs libcurl)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs libcurl)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(pkg-config --libs libcurl)"
	fi
elif test -e /usr/bin/curl-config; then
	CXXFLAGS="$CXXFLAGS $(/usr/bin/curl-config --cflags)"
	if test "$enable_staticlibs" = "yes"; then
		using_libcurl="system (static)"
		LIBS="$LIBS $(/usr/bin/curl-config --libs)"
	else
		using_libcurl="system"
		LIBS="$LIBS $(pkg-config --libs libcurl)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(/usr/bin/curl-config --libs)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(/usr/bin/curl-config --libs)"
	fi
elif test -e ../../../../Shared/Project/curl/Compile.sh; then
	cd ../../../../Shared/Project/curl
	chmod u+x Compile.sh
	./Compile.sh --build=$build_alias
	cd $Home
	if test -e ../../../../Shared/Source/curl/lib/libcurl.a || test -e ../../../../Shared/Source/curl/lib/libcurl.la; then
		CXXFLAGS="$CXXFLAGS -I../../../../Shared/Source/curl/include"
		if test "$enable_staticlibs" = "yes"; then
			using_libcurl="builtin (static)"
			LIBS="$LIBS -L../../../../Shared/Source/curl/lib $(pkg-config --libs --static ../../../../Shared/Source/curl/libcurl.pc)"
			MediaInfoLib_LIBS="$MediaInfoLib_LIBS -L../../../../Shared/Source/curl/lib $(pkg-config --libs --static ../../../../Shared/Source/curl/libcurl.pc)"
			MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static -L../../../../Shared/Source/curl/lib $(pkg-config --libs --static ../../../../Shared/Source/curl/libcurl.pc)"
		else
			using_libcurl="builtin"
			LIBS="$LIBS -L../../../../Shared/Source/curl/lib $(pkg-config --libs ../../../../Shared/Source/curl/libcurl.pc)"
			MediaInfoLib_LIBS="$MediaInfoLib_LIBS -L../../../../Shared/Source/curl/lib $(pkg-config --libs ../../../../Shared/Source/curl/libcurl.pc)"
			MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static -L../../../../Shared/Source/curl/lib $(pkg-config --libs ../../../../Shared/Source/curl/libcurl.pc)"
		fi
	else
		AC_MSG_ERROR([Problem while compiling builtin curl])
	fi
else
	AC_MSG_ERROR([libcurl development files are not found. Install the corresponding libcurl development package (e.g. "apt-get install libcurl-dev" or "yum install libcurl-devel"])
fi

dnl -------------------------------------------------------------------------
dnl libmms
dnl
if test -d $with_libmms; then
	CXXFLAGS="$CXXFLAGS -DMEDIAINFO_LIBMMS_FROMSOURCE -I$with_libmms/src $(pkg-config --cflags $with_libmms/pkgconfig/libmms.pc)"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS -I$with_libmms $(pkg-config --cflags $with_libmms/pkgconfig/libmms.pc)"
	if test "$enable_staticlibs" = "yes"; then
		using_libmms="custom (static)"
		LIBS="$LIBS -L$with_libmms/src/.libs $(pkg-config --libs --static $with_libmms/pkgconfig/libmms.pc)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static -L$with_libmms/src/.libs $(pkg-config --libs --static $with_libmms/pkgconfig/libmms.pc)"
	else
		using_libmms="custom"
		LIBS="$LIBS -L$with_libmms/src/.libs $(pkg-config --libs $with_libmms/pkgconfig/libmms.pc)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS -L$with_libmms/src/.libs $(pkg-config --libs $with_libmms/pkgconfig/libmms.pc)"
	fi
elif test "$with_libmms" = "no"; then
	CXXFLAGS="$CXXFLAGS -DMEDIAINFO_LIBMMS_NO"
	using_libmms="no"
elif test -e ../../../../libmms/libmms.pc; then
	CXXFLAGS="$CXXFLAGS $(pkg-config --cflags ../../../../libmms/libmms.pc)"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $(pkg-config --cflags ../../../../libmms/libmms.pc)"
	if test "$enable_staticlibs" = "yes"; then
		using_libmms="builtin (static)"
		LIBS="$LIBS $(pkg-config --libs --static ../../../../libmms/libmms.pc)"
	else
		using_libmms="builtin"
		LIBS="$LIBS $(pkg-config --libs ../../../../libmms/libmms.pc)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs ../../../../libmms/libmms.pc)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(pkg-config --libs ../../../../libmms/libmms.pc)"
	fi
elif pkg-config --exists libmms; then
	CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libmms)"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $(pkg-config --cflags libmms)"
	if test "$enable_staticlibs" = "yes"; then
		using_libmms="system (static)"
		LIBS="$LIBS $(pkg-config --libs --static libmms)"
	else
		using_libmms="system"
		LIBS="$LIBS $(pkg-config --libs libmms)"
		MediaInfoLib_LIBS="$MediaInfoLib_LIBS $(pkg-config --libs libmms)"
		MediaInfoLib_LIBS_Static="$MediaInfoLib_LIBS_Static $(pkg-config --libs libmms)"
	fi
else
	CXXFLAGS="$CXXFLAGS -DMEDIAINFO_LIBMMS_NO"
	using_libmms="not found"
fi

dnl -------------------------------------------------------------------------
dnl libaes
dnl
if test "$with_libaes" = "yes"; then
	PKG_CHECK_MODULES([AES_GLADMAN], [aes_gladman])
	CXXFLAGS="$CXXFLAGS $AES_CXXFLAGS"
	CFLAGS="$CFLAGS $AES_CFLAGS"
	MediaInfoLib_CFLAGS="$MediaInfoLib_CFLAGS $AES_CFLAGS"
	LIBS="$LIBS $AES_LIBS"
	MediaInfoLib_LIBS="$MediaInfoLib_LIBS $AES_LIBS"
	AM_CONDITIONAL(COMPILE_AES, false)
	using_libaes="system"
else
	CXXFLAGS="$CXXFLAGS -I../../../Source/ThirdParty/aes-gladman"
	CFLAGS="$CFLAGS -I../../../Source/ThirdParty/aes-gladman"
	using_libaes="internal"
	AM_CONDITIONAL(COMPILE_AES, true)
fi

dnl -------------------------------------------------------------------------
dnl libmd5
dnl
if test "$with_libmd5" = "yes"; then
	PKG_CHECK_MODULES([MD5], [md5])
	CXXFLAGS="$CXXFLAGS $MD5_CXXFLAGS"
	CFLAGS="$CFLAGS $MD5_CFLAGS"
	MediaInfoLib_CFLAGS="$MediaInfoLib_CFLAGS $MD5_CFLAGS"
	LIBS="$LIBS $MD5_LIBS"
	MediaInfoLib_LIBS="$MediaInfoLib_LIBS $MD5_LIBS"
	AM_CONDITIONAL(COMPILE_MD5, false)
	using_libmd5="system"
else
	CXXFLAGS="$CXXFLAGS -I../../../Source/ThirdParty/md5"
	CFLAGS="$CFLAGS -I../../../Source/ThirdParty/md5"
	using_libmd5="internal"
	AM_CONDITIONAL(COMPILE_MD5, true)
fi

dnl -------------------------------------------------------------------------
dnl libtinyxml2
dnl
if test "$with_libtinyxml2" = "yes"; then
	PKG_CHECK_MODULES([TINYXML2], [tinyxml2])
	CXXFLAGS="$CXXFLAGS $TINYXML2_CXXFLAGS"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $TINYXML2_CXXFLAGS"
	LIBS="$LIBS $TINYXML2_LIBS"
	MediaInfoLib_LIBS="$MediaInfoLib_LIBS $TINYXML2_LIBS"
	AM_CONDITIONAL(COMPILE_TINYXML2, false)
	using_libtinyxml2="system"
else
	CXXFLAGS="$CXXFLAGS -I../../../Source/ThirdParty/tinyxml2"
	using_libtinyxml2="internal"
	AM_CONDITIONAL(COMPILE_TINYXML2, true)
fi

dnl -------------------------------------------------------------------------
dnl libmediainfo-name
dnl
MediaInfoLib_LibName="$with_libmediainfo_name"

dnl -------------------------------------------------------------------------
dnl base64
dnl
CXXFLAGS="$CXXFLAGS -I../../../Source/ThirdParty/base64"

dnl #########################################################################
dnl ### Compiler specific
dnl #########################################################################

dnl -------------------------------------------------------------------------
case "${build}" in
  powerpc-montavista* )
    CFLAGS="$CFLAGS -D__MONTAVISTA__"
    CXXFLAGS="$CXXFLAGS -D__MONTAVISTA__"
  ;;
  powerpc64-* )
    enable_visibility="no"
  ;;
esac

dnl -------------------------------------------------------------------------
dnl Mac Architecture and SDK
dnl
if test "$enable_universal_binary" = "yes"; then
	enable_arch_ppc=yes
	enable_arch_i386=yes
	if test "$with_macosx_sdk" = no ; then
		with_macosx_sdk="/Developer/SDKs/MacOSX10.4u.sdk"
	fi
	if test "$with_macosx_version_min" = no ; then
		with_macosx_version_min="10.4"
	fi
fi
if test "$enable_arch_ppc" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch ppc"
	CFLAGS="$CFLAGS -arch ppc"
	LDFLAGS="$LDFLAGS -arch ppc"
fi
if test "$enable_arch_ppc64" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch ppc64"
	CFLAGS="$CFLAGS -arch ppc64"
	LDFLAGS="$LDFLAGS -arch ppc64"
fi
if test "$enable_arch_i386" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch i386"
	CFLAGS="$CFLAGS -arch i386"
	LDFLAGS="$LDFLAGS -arch i386"
fi
if test "$enable_arch_x86_64" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch x86_64"
	CFLAGS="$CFLAGS -arch x86_64"
	LDFLAGS="$LDFLAGS -arch x86_64"
fi
if test "$with_macosx_sdk" = "yes" ; then
	CXXFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk $CXXFLAGS"
	CFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk $CFLAGS"
	LDFLAGS="-isysroot /Developer/SDKs/MacOSX10.4u.sdk $LDFLAGS"
elif test "x$with_macosx_sdk" != xno ; then
	CXXFLAGS="-isysroot $with_macosx_sdk $CXXFLAGS"
	CFLAGS="-isysroot $with_macosx_sdk $CFLAGS"
	LDFLAGS="-isysroot $with_macosx_sdk $LDFLAGS"
fi
if test "x$with_macosx_version_min" != xno ; then
	CXXFLAGS="-mmacosx-version-min=$with_macosx_version_min $CXXFLAGS"
	CFLAGS="-mmacosx-version-min=$with_macosx_version_min $CFLAGS"
	LDFLAGS="-mmacosx-version-min=$with_macosx_version_min $LDFLAGS"
fi

dnl #########################################################################
dnl ### C/C++ compiler options used to compile
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test "$enable_unicode" = "yes"; then
	CXXFLAGS="$CXXFLAGS -DUNICODE"
	MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS -DUNICODE"
	MediaInfoLib_Unicode="yes"
else
	MediaInfoLib_Unicode="no"
fi

dnl -------------------------------------------------------------------------
dnl GNU compiler
dnl
if test "$GXX" = yes ; then
    case "${build}" in
      *mingw* | *cygwin* ) ;;
      *)
        CFLAGS="$CFLAGS -fPIC"
        CXXFLAGS="$CXXFLAGS -fPIC"
        LDFLAGS="$LDFLAGS -fPIC"
      ;;
    esac
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

if test "$enable_visibility" = yes ; then
	if test -e "/etc/debian_version" ; then
		Debian_Version="$(more /etc/debian_version)"
		case $Debian_Version in
		*4.*)
			enable_visibility="no" ;;
		esac
	fi
fi
if test "$enable_visibility" = yes ; then
	CFLAGS="$CFLAGS -fvisibility=hidden -fvisibility-inlines-hidden"
	CXXFLAGS="$CXXFLAGS -fvisibility=hidden -fvisibility-inlines-hidden"
	LDFLAGS="$LDFLAGS -fvisibility=hidden -fvisibility-inlines-hidden"
fi

dnl #########################################################################
dnl ### MediaInfoLib flags
dnl #########################################################################

MediaInfoLib_LIBS=" -L$(pwd) -l$with_libmediainfo_name $MediaInfoLib_LIBS"
MediaInfoLib_LIBS_Static=" $(pwd)/.libs/lib$with_libmediainfo_name.a $MediaInfoLib_LIBS_Static"

dnl #########################################################################
dnl ### Output
dnl #########################################################################

AC_SUBST(MediaInfoLib_CXXFLAGS)
AC_SUBST(MediaInfoLib_LIBS)
AC_SUBST(MediaInfoLib_LIBS_Static)
AC_SUBST(MediaInfoLib_Unicode)
AC_SUBST(MediaInfoLib_LibName)
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
echo "  Using libcurl?                                          $using_libcurl"
echo "  Using libmms?                                           $using_libmms"
echo "  Using zlib?                                             $with_zlib"
echo "  Using aes?                                              $using_libaes"
echo "  Using md5?                                              $using_libmd5"
echo "  Using tinyxml2?                                         $using_libtinyxml2"
echo ""
echo "  Create static lib?                                      $enable_static"
echo "  Create shared lib?                                      $enable_shared"
echo ""
Mcho "Minimize size" "$MediaInfoMinimizeSize"
Mcho "libmms describe support" "$MediaInfoLibmmsDescribeSupport"
Mcho "All    " "$MediaInfoAll"
Mcho "Archive" "$MediaInfoArchive"
Mcho "Audio  " "$MediaInfoAudio"
Mcho "Image  " "$MediaInfoImage"
Mcho "Multi  " "$MediaInfoMulti"
Mcho "Multi  " "$MediaInfoTag"
Mcho "Text   " "$MediaInfoText"
Mcho "Video  " "$MediaInfoVideo"
Mcho "7z     " "$MediaInfo7z"
Mcho "Ace    " "$MediaInfoAce"
Mcho "Bzip2  " "$MediaInfoBzip2"
Mcho "Elf    " "$MediaInfoElf"
Mcho "Gzip   " "$MediaInfoGzip"
Mcho "ISO9660" "$MediaInfoIso9660"
Mcho "Mz     " "$MediaInfoMz"
Mcho "Rar    " "$MediaInfoRar"
Mcho "Tar    " "$MediaInfoTar"
Mcho "Zip    " "$MediaInfoZip"
Mcho "Aac    " "$MediaInfoAac"
Mcho "Ac3    " "$MediaInfoAc3"
Mcho "Adif   " "$MediaInfoAdif"
Mcho "Adpcm  " "$MediaInfoAdpcm"
Mcho "Adts   " "$MediaInfoAdts"
Mcho "Aes3   " "$MediaInfoAes3"
Mcho "Als    " "$MediaInfoAls"
Mcho "Amr    " "$MediaInfoAmr"
Mcho "Amv    " "$MediaInfoAmv"
Mcho "Ape    " "$MediaInfoApe"
Mcho "Au     " "$MediaInfoAu"
Mcho "Celt   " "$MediaInfoCelt"
Mcho "Dts    " "$MediaInfoDts"
Mcho "Flac   " "$MediaInfoFlac"
Mcho "La     " "$MediaInfoLa"
Mcho "Latm   " "$MediaInfoLatm"
Mcho "Midi   " "$MediaInfoMidi"
Mcho "Mpc    " "$MediaInfoMpc"
Mcho "MpcSv8 " "$MediaInfoMpcSv8"
Mcho "Mpega  " "$MediaInfoMpega"
Mcho "OpenMG " "$MediaInfoOpenMG"
Mcho "Pcm    " "$MediaInfoPcm"
Mcho "Ps2a   " "$MediaInfoPs2a"
Mcho "Rkau   " "$MediaInfoRkau"
Mcho "Speex  " "$MediaInfoSpeex"
Mcho "Tak    " "$MediaInfoTak"
Mcho "Tta    " "$MediaInfoTta"
Mcho "TwinVQ " "$MediaInfoTwinVQ"
Mcho "Vorbis " "$MediaInfoVorbis"
Mcho "Wvpk   " "$MediaInfoWvpk"
Mcho "ArriRaw" "$MediaInfoArriRaw"
Mcho "Bmp    " "$MediaInfoBmp"
Mcho "Dds    " "$MediaInfoDds"
Mcho "Dpx    " "$MediaInfoDpx"
Mcho "Exr    " "$MediaInfoDpx"
Mcho "Gif    " "$MediaInfoGif"
Mcho "Ico    " "$MediaInfoIco"
Mcho "Jpeg   " "$MediaInfoJpeg"
Mcho "Pcx    " "$MediaInfoPcx"
Mcho "Png    " "$MediaInfoPng"
Mcho "Psd    " "$MediaInfoPsd"
Mcho "Tiff   " "$MediaInfoTiff"
Mcho "Aaf    " "$MediaInfoAaf"
Mcho "Bdav   " "$MediaInfoBdav"
Mcho "Bdmv   " "$MediaInfoBdmv"
Mcho "Cdxa   " "$MediaInfoCdxa"
Mcho "DASH   " "$MediaInfoDashMpd"
Mcho "Dcp    " "$MediaInfoDcp"
Mcho "Dpg    " "$MediaInfoDpg"
Mcho "References" "$References"
Mcho "DvDif  " "$MediaInfoDvDif"
Mcho "DvDifAnalyze" "$MediaInfoDvDifAnalyze"
Mcho "Dvdv   " "$MediaInfoDvdv"
Mcho "Dxw    " "$MediaInfoFlv"
Mcho "Gxf "  "$MediaInfoHdsGxf"
Mcho "HdsF4m " "$MediaInfoHdsF4m"
Mcho "Hls    " "$MediaInfoHls"
Mcho "Flv    " "$MediaInfoFlv"
Mcho "Mk     " "$MediaInfoMk"
Mcho "Mpeg4  " "$MediaInfoMpeg4"
Mcho "MpegPs " "$MediaInfoMpegPs"
Mcho "MpegTs " "$MediaInfoMpegTs"
Mcho "MpegTs (PCR)" "$MediaInfoMpegTsPcr"
Mcho "MpegTs (Parser time stamp)" "$MediaInfoMpegTsPesTimeStamp"
Mcho "Mxf    " "$MediaInfoMxf"
Mcho "Nut    " "$MediaInfoNut"
Mcho "Ogg    " "$MediaInfoOgg"
Mcho "Pmp    " "$MediaInfoPmp"
Mcho "Riff   " "$MediaInfoRiff"
Mcho "Rm     " "$MediaInfoRm"
Mcho "Skm    " "$MediaInfoSkm"
Mcho "Swf    " "$MediaInfoSwf"
Mcho "Tsp    " "$MediaInfoTsp"
Mcho "Wm     " "$MediaInfoWm"
Mcho "ApeTag " "$MediaInfoApeTag"
Mcho "Id3    " "$MediaInfoId3"
Mcho "Id3v2  " "$MediaInfoId3v2"
Mcho "Lyrics3" "$MediaInfoLyrics3"
Mcho "Lyrics3v2" "$MediaInfoLyrics3v2"
Mcho "VorbisCom" "$MediaInfoVorbisCom"
Mcho "Cmml   " "$MediaInfoCmml"
Mcho "DvbSubtitle " "$MediaInfoDvbSubtitle"
Mcho "Eia608 " "$MediaInfoEia608"
Mcho "Eia708 " "$MediaInfoEia708"
Mcho "SubRip " "$MediaInfoSubRip"
Mcho "Teletext " "$MediaInfoTeletext"
Mcho "TimedText " "$MediaInfoTimedText"
Mcho "Ttml   " "$MediaInfoTtml"
Mcho "Kate   " "$MediaInfoKate"
Mcho "Pdf    " "$MediaInfoPdf"
Mcho "Pgs    " "$MediaInfoPgs"
Mcho "Scc    " "$MediaInfoScc"
Mcho "Aic    " "$MediaInfoAic"
Mcho "Avc    " "$MediaInfoAvc"
Mcho "AvsV   " "$MediaInfoAvsV"
Mcho "Canopus" "$MediaInfoCanopus"
Mcho "Ffv1   " "$MediaInfoFfv1"
Mcho "Flic   " "$MediaInfoFlic"
Mcho "H263   " "$MediaInfoH263"
Mcho "Hevc   " "$MediaInfoHevc"
Mcho "HuffYUV" "$MediaInfoHuffYUV"
Mcho "Mpeg4v " "$MediaInfoMpeg4v"
Mcho "Mpegv  " "$MediaInfoMpegv"
Mcho "ProRes " "$MediaInfoProRes"
Mcho "Theora " "$MediaInfoTheora"
Mcho "Vc1    " "$MediaInfoVc1"
Mcho "Vc3    " "$MediaInfoVc3"
Mcho "VP8    " "$MediaInfoVp8"
Mcho "Y4m    " "$MediaInfoY4m"
echo ""
echo "  CXXFLAGS: $CXXFLAGS"
echo "  LDFLAGS: $LDFLAGS"
echo "  LIBS: $LIBS"
