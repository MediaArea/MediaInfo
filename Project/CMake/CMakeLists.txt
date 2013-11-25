project(mediainfo)

set(MEDIAINFO_MAJOR_VERSION 0)
set(MEDIAINFO_MINOR_VERSION 7)
set(MEDIAINFO_PATCH_VERSION 53)
set(MEDIAINFO_VERSION ${MEDIAINFO_MAJOR_VERSION}.${MEDIAINFO_MINOR_VERSION}.${MEDIAINFO_PATCH_VERSION})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(${ZLIB_INCLUDE_DIR})

add_definitions(-DUNICODE)

set(mediainfo_SRCS
	ThirdParty/tinyxml2/tinyxml2.cpp
	ThirdParty/md5/md5.c
	MediaInfo/File__Analyze.cpp
	MediaInfo/File__Analyze_Buffer.cpp
	MediaInfo/File__Analyze_Buffer_MinimizeSize.cpp
	MediaInfo/File__Analyze_Streams.cpp
	MediaInfo/File__Analyze_Streams_Finish.cpp
	MediaInfo/File__Base.cpp
	MediaInfo/File__MultipleParsing.cpp
	MediaInfo/File__Duplicate.cpp
	MediaInfo/File_Dummy.cpp
	MediaInfo/File_Other.cpp
	MediaInfo/File_Unknown.cpp
	MediaInfo/MediaInfo.cpp
	MediaInfo/MediaInfo_Config.cpp
	MediaInfo/MediaInfo_Config_Automatic.cpp
	MediaInfo/MediaInfo_Config_MediaInfo.cpp
	MediaInfo/MediaInfo_File.cpp
	MediaInfo/MediaInfo_Inform.cpp
	MediaInfo/MediaInfo_Internal.cpp
	MediaInfo/MediaInfoList.cpp
	MediaInfo/MediaInfoList_Internal.cpp
	MediaInfo/TimeCode.cpp
	MediaInfo/Archive/File_7z.cpp
	MediaInfo/Archive/File_Ace.cpp
	MediaInfo/Archive/File_Bzip2.cpp
	MediaInfo/Archive/File_Elf.cpp
	MediaInfo/Archive/File_Gzip.cpp
	MediaInfo/Archive/File_Iso9660.cpp
	MediaInfo/Archive/File_Mz.cpp
	MediaInfo/Archive/File_Rar.cpp
	MediaInfo/Archive/File_Tar.cpp
	MediaInfo/Archive/File_Zip.cpp
	MediaInfo/Audio/File_Aac.cpp
	MediaInfo/Audio/File_Aac_GeneralAudio.cpp
	MediaInfo/Audio/File_Aac_GeneralAudio_Sbr.cpp
	MediaInfo/Audio/File_Aac_GeneralAudio_Sbr_Ps.cpp
	MediaInfo/Audio/File_Aac_Main.cpp
	MediaInfo/Audio/File_Aac_Others.cpp
	MediaInfo/Audio/File_Ac3.cpp
	MediaInfo/Audio/File_Adpcm.cpp
	MediaInfo/Audio/File_Aes3.cpp
	MediaInfo/Audio/File_Als.cpp
	MediaInfo/Audio/File_Amr.cpp
	MediaInfo/Audio/File_Amv.cpp
	MediaInfo/Audio/File_Ape.cpp
	MediaInfo/Audio/File_Au.cpp
	MediaInfo/Audio/File_Celt.cpp
	MediaInfo/Audio/File_ChannelGrouping.cpp
	MediaInfo/Audio/File_Dts.cpp
	MediaInfo/Audio/File_DolbyE.cpp
	MediaInfo/Audio/File_ExtendedModule.cpp
	MediaInfo/Audio/File_Flac.cpp
	MediaInfo/Audio/File_ImpulseTracker.cpp
	MediaInfo/Audio/File_La.cpp
	MediaInfo/Audio/File_Midi.cpp
	MediaInfo/Audio/File_Module.cpp
	MediaInfo/Audio/File_Mpc.cpp
	MediaInfo/Audio/File_MpcSv8.cpp
	MediaInfo/Audio/File_Mpega.cpp
	MediaInfo/Audio/File_Opus.cpp
	MediaInfo/Audio/File_Pcm.cpp
	MediaInfo/Audio/File_Ps2Audio.cpp
	MediaInfo/Audio/File_Rkau.cpp
	MediaInfo/Audio/File_ScreamTracker3.cpp
	MediaInfo/Audio/File_Speex.cpp
	MediaInfo/Audio/File_Tak.cpp
	MediaInfo/Audio/File_Tta.cpp
	MediaInfo/Audio/File_TwinVQ.cpp
	MediaInfo/Audio/File_Vorbis.cpp
	MediaInfo/Audio/File_Wvpk.cpp
	MediaInfo/Duplicate/File__Duplicate__Base.cpp
	MediaInfo/Duplicate/File__Duplicate__Writer.cpp
	MediaInfo/Duplicate/File__Duplicate_MpegTs.cpp
	MediaInfo/Export/Export_EbuCore.cpp
	MediaInfo/Export/Export_Mpeg7.cpp
	MediaInfo/Export/Export_PBCore.cpp
	MediaInfo/Export/Export_reVTMD.cpp
	MediaInfo/Image/File_Bmp.cpp
	MediaInfo/Image/File_Dpx.cpp
	MediaInfo/Image/File_Exr.cpp
	MediaInfo/Image/File_Gif.cpp
	MediaInfo/Image/File_Ico.cpp
	MediaInfo/Image/File_Jpeg.cpp
	MediaInfo/Image/File_Pcx.cpp
	MediaInfo/Image/File_Png.cpp
	MediaInfo/Image/File_Psd.cpp
	MediaInfo/Image/File_Rle.cpp
	MediaInfo/Image/File_Tiff.cpp
	MediaInfo/Image/File_Tga.cpp
	MediaInfo/Multiple/File__ReferenceFilesHelper.cpp
	MediaInfo/Multiple/File_Ancillary.cpp
	MediaInfo/Multiple/File_Bdav.cpp
	MediaInfo/Multiple/File_Cdxa.cpp
	MediaInfo/Multiple/File_DashMpd.cpp
	MediaInfo/Multiple/File_DcpAm.cpp
	MediaInfo/Multiple/File_DcpCpl.cpp
	MediaInfo/Multiple/File_DcpPkl.cpp
	MediaInfo/Multiple/File_Dpg.cpp
	MediaInfo/Multiple/File_DvDif.cpp
	MediaInfo/Multiple/File_DvDif_Analysis.cpp
	MediaInfo/Multiple/File_Dvdv.cpp
	MediaInfo/Multiple/File_Dxw.cpp
	MediaInfo/Multiple/File_Flv.cpp
	MediaInfo/Multiple/File_Gxf.cpp
	MediaInfo/Multiple/File_HdsF4m.cpp
	MediaInfo/Multiple/File_Hls.cpp
	MediaInfo/Multiple/File_Gxf.cpp
	MediaInfo/Multiple/File_Gxf_TimeCode.cpp
	MediaInfo/Multiple/File_Ibi.cpp
	MediaInfo/Multiple/File_Ibi_Create.cpp
	MediaInfo/Multiple/File_Ism.cpp
	MediaInfo/Multiple/File_Ivf.cpp
	MediaInfo/Multiple/File_Lxf.cpp
	MediaInfo/Multiple/File_Mk.cpp
	MediaInfo/Multiple/File_Mpeg4.cpp
	MediaInfo/Multiple/File_Mpeg4_Descriptors.cpp
	MediaInfo/Multiple/File_Mpeg4_Elements.cpp
	MediaInfo/Multiple/File_Mpeg4_TimeCode.cpp
	MediaInfo/Multiple/File_Mpeg_Descriptors.cpp
	MediaInfo/Multiple/File_Mpeg_Psi.cpp
	MediaInfo/Multiple/File_MpegPs.cpp
	MediaInfo/Multiple/File_MpegTs.cpp
	MediaInfo/Multiple/File_MpegTs_Duplicate.cpp
	MediaInfo/Multiple/File_Mxf.cpp
	MediaInfo/Multiple/File_Nut.cpp
	MediaInfo/Multiple/File_Ogg.cpp
	MediaInfo/Multiple/File_Ogg_SubElement.cpp
	MediaInfo/Multiple/File_P2_Clip.cpp
	MediaInfo/Multiple/File_Riff.cpp
	MediaInfo/Multiple/File_Riff_Elements.cpp
	MediaInfo/Multiple/File_Rm.cpp
	MediaInfo/Multiple/File_SequenceInfo.cpp
	MediaInfo/Multiple/File_Skm.cpp
	MediaInfo/Multiple/File_Swf.cpp
	MediaInfo/Multiple/File_Umf.cpp
	MediaInfo/Multiple/File_Wm.cpp
	MediaInfo/Multiple/File_Wm_Elements.cpp
	MediaInfo/Multiple/File_Xdcam_Clip.cpp
	MediaInfo/Reader/Reader_Directory.cpp \
	MediaInfo/Reader/Reader_File.cpp \
	MediaInfo/Reader/Reader_libcurl.cpp \
	MediaInfo/Reader/Reader_libmms.cpp \
	MediaInfo/Tag/File__Tags.cpp
	MediaInfo/Tag/File_ApeTag.cpp
	MediaInfo/Tag/File_Id3.cpp
	MediaInfo/Tag/File_Id3v2.cpp
	MediaInfo/Tag/File_Lyrics3.cpp
	MediaInfo/Tag/File_Lyrics3v2.cpp
	MediaInfo/Tag/File_VorbisCom.cpp
	MediaInfo/Text/File_Cdp.cpp
	MediaInfo/Text/File_Cmml.cpp
	MediaInfo/Text/File_DvbSubtitle.cpp
	MediaInfo/Text/File_DtvccTransport.cpp
	MediaInfo/Text/File_Eia608.cpp
	MediaInfo/Text/File_Eia708.cpp
	MediaInfo/Text/File_Kate.cpp
	MediaInfo/Text/File_N19.cpp
	MediaInfo/Text/File_OtherText.cpp
	MediaInfo/Text/File_Pgs.cpp
	MediaInfo/Text/File_Scc.cpp
	MediaInfo/Text/File_Scte20.cpp
	MediaInfo/Text/File_SubRip.cpp
	MediaInfo/Text/File_Teletext.cpp
	MediaInfo/Text/File_TimedText.cpp
	MediaInfo/Text/File_Ttml.cpp
	MediaInfo/Video/File_Aic.cpp
	MediaInfo/Video/File_AfdBarData.cpp
	MediaInfo/Video/File_Avc.cpp
	MediaInfo/Video/File_Avc_Duplicate.cpp
	MediaInfo/Video/File_AvsV.cpp
	MediaInfo/Video/File_Canopus.cpp
	MediaInfo/Video/File_Dirac.cpp
	MediaInfo/Video/File_Flic.cpp
	MediaInfo/Video/File_Fraps.cpp
	MediaInfo/Video/File_H263.cpp
	MediaInfo/Video/File_Lagarith.cpp
	MediaInfo/Video/File_Mpeg4v.cpp
	MediaInfo/Video/File_Mpegv.cpp
	MediaInfo/Video/File_ProRes.cpp
	MediaInfo/Video/File_Theora.cpp
	MediaInfo/Video/File_Vc1.cpp
	MediaInfo/Video/File_Vc3.cpp
	MediaInfo/Video/File_Vp8.cpp
	MediaInfo/Video/File_Y4m.cpp
	MediaInfoDLL/MediaInfoDLL.cpp
	MediaInfoDLL/MediaInfoDLL.def
)

if (WIN32)
	#FIXME MediaInfo.rc should use ${MEDIAINFO_MAJOR_VERSION} and friends
	list(APPEND mediainfo_SRCS
		MediaInfo.rc
	)
endif (WIN32)

add_library(mediainfo SHARED ${mediainfo_SRCS})

target_link_libraries(mediainfo
	zenlib
	${ZLIB_LIBRARY}
)

set_target_properties(mediainfo PROPERTIES
	VERSION ${MEDIAINFO_VERSION}
	SOVERSION ${MEDIAINFO_MAJOR_VERSION}
)

install(TARGETS mediainfo DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})