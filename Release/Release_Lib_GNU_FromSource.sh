##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 #  Use of this source code is governed by a BSD-style license that can
 #  be found in the License.html file in the root of the source tree.
 ##

#! /bin/sh

#-----------------------------------------------------------------------
# Clean up
test -e MediaInfo_Lib_GNU_FromSource.tar     && rm    MediaInfo_Lib_GNU_FromSource.tar
test -e MediaInfo_Lib_GNU_FromSource.tar.bz2 && rm    MediaInfo_Lib_GNU_FromSource.tar.bz2
test -d MediaInfo_Lib_GNU_FromSource         && rm -r MediaInfo_Lib_GNU_FromSource
mkdir MediaInfo_Lib_GNU_FromSource


#-----------------------------------------------------------------------
# Preparing : Project
cd ../Project/GNU/Library
chmod u+x autogen
./autogen
cd ../../../Release


#-----------------------------------------------------------------------
# Copying : Project
mkdir -p MediaInfo_Lib_GNU_FromSource/Project/GNU/Library
cp ../Project/GNU/Library/aclocal.m4 MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/autogen MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/config.guess MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/config.sub MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/configure MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/configure.ac MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/depcomp MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/install-sh MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/ltmain.sh MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/Makefile.am MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/Makefile.in MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/missing MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/libmediainfo.pc.in MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/
cp ../Project/GNU/Library/libmediainfo-config.in MediaInfo_Lib_GNU_FromSource/Project/GNU/Library/

#-----------------------------------------------------------------------
# Copying : Source
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/aes-gladman
cp -r ../Source/ThirdParty/aes-gladman/*.asm MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/aes-gladman/
cp -r ../Source/ThirdParty/aes-gladman/*.h MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/aes-gladman/
cp -r ../Source/ThirdParty/aes-gladman/*.c MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/aes-gladman/
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/base64
cp -r ../Source/ThirdParty/base64/*.h MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/base64/
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/md5
cp -r ../Source/ThirdParty/md5/*.h MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/md5/
cp -r ../Source/ThirdParty/md5/*.c MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/md5/
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/tinyxml2
cp -r ../Source/ThirdParty/tinyxml2/*.h MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/tinyxml2/
cp -r ../Source/ThirdParty/tinyxml2/*.cpp MediaInfo_Lib_GNU_FromSource/Source/ThirdParty/tinyxml2/
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/MediaInfo
cp -r ../Source/MediaInfo/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/
cp -r ../Source/MediaInfo/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Archive
cp -r ../Source/MediaInfo/Archive/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Archive/
cp -r ../Source/MediaInfo/Archive/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Archive/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Audio
cp -r ../Source/MediaInfo/Audio/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Audio/
cp -r ../Source/MediaInfo/Audio/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Audio/
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Duplicate
cp -r ../Source/MediaInfo/Duplicate/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Duplicate/
cp -r ../Source/MediaInfo/Duplicate/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Duplicate/
mkdir -p MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Export
cp -r ../Source/MediaInfo/Export/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Export/
cp -r ../Source/MediaInfo/Export/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Export/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Image
cp -r ../Source/MediaInfo/Image/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Image/
cp -r ../Source/MediaInfo/Image/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Image/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Reader
cp -r ../Source/MediaInfo/Reader/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Reader/
cp -r ../Source/MediaInfo/Reader/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Reader/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Multiple
cp -r ../Source/MediaInfo/Multiple/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Multiple/
cp -r ../Source/MediaInfo/Multiple/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Multiple/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Tag
cp -r ../Source/MediaInfo/Tag/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Tag/
cp -r ../Source/MediaInfo/Tag/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Tag/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Text
cp -r ../Source/MediaInfo/Text/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Text/
cp -r ../Source/MediaInfo/Text/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Text/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Video
cp -r ../Source/MediaInfo/Video/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Video/
cp -r ../Source/MediaInfo/Video/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfo/Video/
mkdir MediaInfo_Lib_GNU_FromSource/Source/MediaInfoDLL
cp -r ../Source/MediaInfoDLL/*.h MediaInfo_Lib_GNU_FromSource/Source/MediaInfoDLL/
cp -r ../Source/MediaInfoDLL/*.cpp MediaInfo_Lib_GNU_FromSource/Source/MediaInfoDLL/

#-----------------------------------------------------------------------
# Copying : Information
cp ../License.html MediaInfo_Lib_GNU_FromSource/
cp ../History_DLL.txt MediaInfo_Lib_GNU_FromSource/
cp ../Changes.txt MediaInfo_Lib_GNU_FromSource/
test -d MediaInfo_Lib_GNU_FromSource/Release || mkdir MediaInfo_Lib_GNU_FromSource/Release
cp ReadMe_DLL_Linux.txt MediaInfo_Lib_GNU_FromSource/Release/
cp ReadMe_DLL_Mac.txt MediaInfo_Lib_GNU_FromSource/Release/


#-----------------------------------------------------------------------
# Compressing Archive
tar jchf MediaInfo_Lib_GNU_FromSource.tar.bz2 MediaInfo_Lib_GNU_FromSource/*

#-----------------------------------------------------------------------
# Clean up
rm -r MediaInfo_Lib_GNU_FromSource
