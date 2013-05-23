##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 #  Use of this source code is governed by a BSD-style license that can
 #  be found in the License.html file in the root of the source tree.
 ##

#! /bin/sh

#-----------------------------------------------------------------------
# Clean up
test -e MediaInfo_DLL_GNU_FromSource.tar     && rm    MediaInfo_DLL_GNU_FromSource.tar
test -e MediaInfo_DLL_GNU_FromSource.tar.bz2 && rm    MediaInfo_DLL_GNU_FromSource.tar.bz2
test -d MediaInfo_DLL_GNU_FromSource         && rm -r MediaInfo_DLL_GNU_FromSource


#-----------------------------------------------------------------------
# Preparing : From MediaInfo library
. ./Release_Lib_GNU_FromSource.sh
tar xf MediaInfo_Lib_GNU_FromSource.tar.bz2
rm MediaInfo_Lib_GNU_FromSource.tar.bz2
mv MediaInfo_Lib_GNU_FromSource MediaInfo_DLL_GNU_FromSource


#-----------------------------------------------------------------------
# Copying : Source
mkdir -p MediaInfo_DLL_GNU_FromSource/Source/Doc
cp ../Source/Doc/Doxyfile MediaInfo_DLL_GNU_FromSource/Source/Doc/
cp ../Source/Doc/Documentation.html MediaInfo_DLL_GNU_FromSource/Source/Doc/
mkdir -p MediaInfo_DLL_GNU_FromSource/Source/Example
cp ../Source/Example/HowToUse* MediaInfo_DLL_GNU_FromSource/Source/Example/
mkdir -p MediaInfo_DLL_$1_$2/Developpers/Include/MediaInfo
cp ../Source/MediaInfo/MediaInfo.h MediaInfo_DLL_$1_$2/Developpers/Include/MediaInfo/
cp ../Source/MediaInfo/MediaInfo_Events.h MediaInfo_DLL_$1_$2/Developpers/Include/MediaInfo/
cp ../Source/MediaInfo/MediaInfoList.h MediaInfo_DLL_$1_$2/Developpers/Include/MediaInfo/
mkdir -p MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL
cp ../Source/MediaInfoDLL/MediaInfoDLL.h MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/
cp ../Source/MediaInfoDLL/MediaInfoDLL_Static.h MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/
cp ../Source/MediaInfoDLL/MediaInfoDLL.cs MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/
cp ../Source/MediaInfoDLL/MediaInfoDLL.JNA.java MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/
cp ../Source/MediaInfoDLL/MediaInfoDLL.JNative.java MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/
cp ../Source/MediaInfoDLL/MediaInfoDLL.py MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/
cp ../Source/MediaInfoDLL/MediaInfoDLL3.py MediaInfo_DLL_GNU_FromSource/Source/MediaInfoDLL/

#-----------------------------------------------------------------------
# Copying : Release
mkdir -p MediaInfo_DLL_GNU_FromSource/Release
cp Release_DLL_GNU.sub MediaInfo_DLL_GNU_FromSource/Release/
cp *.sh MediaInfo_DLL_GNU_FromSource/Release/
chmod u+x MediaInfo_DLL_GNU_FromSource/Release/*.sh

#-----------------------------------------------------------------------
# Copying : Information
cp ../License.html MediaInfo_DLL_GNU_FromSource/
cp ReadMe_DLL_Linux.txt MediaInfo_DLL_GNU_FromSource/Release/
dos2unix MediaInfo_DLL_GNU_FromSource/Release/ReadMe_DLL_Linux.txt
cp ReadMe_DLL_Mac.txt MediaInfo_DLL_GNU_FromSource/Release/


#-----------------------------------------------------------------------
# Preparing Archive : MediaInfoLib
mv MediaInfo_DLL_GNU_FromSource MediaInfoLib
mkdir MediaInfo_DLL_GNU_FromSource
mv MediaInfoLib MediaInfo_DLL_GNU_FromSource/

#-----------------------------------------------------------------------
# Preparing Archive : From ZenLib
cd ../../ZenLib/Release
. ./Release_GNU_FromSource.sh
tar xf ZenLib_GNU_FromSource.tar.bz2
rm ZenLib_GNU_FromSource.tar.bz2
cd ../../MediaInfoLib/Release
mv ../../ZenLib/Release/ZenLib_GNU_FromSource MediaInfo_DLL_GNU_FromSource/ZenLib

#-----------------------------------------------------------------------
# Preparing Archive : From Shared
mkdir -p MediaInfo_DLL_GNU_FromSource/Shared/Project/_Common
cp -r ../../Shared/Project/_Common/*.sh MediaInfo_DLL_GNU_FromSource/Shared/Project/_Common
mkdir -p MediaInfo_DLL_GNU_FromSource/Shared/Project/curl
cp -r ../../Shared/Project/curl/*.sh MediaInfo_DLL_GNU_FromSource/Shared/Project/curl
mkdir -p MediaInfo_DLL_GNU_FromSource/Shared/Project/zlib
cp -r ../../Shared/Project/zlib/*.sh MediaInfo_DLL_GNU_FromSource/Shared/Project/zlib

#-----------------------------------------------------------------------
# Preparing Archive : Automation
cp ../Project/GNU/Library/AddThisToRoot_DLL_compile.sh MediaInfo_DLL_GNU_FromSource/SO_Compile.sh
chmod u+x MediaInfo_DLL_GNU_FromSource/SO_Compile.sh


#-----------------------------------------------------------------------
# Compressing Archive
tar jchf MediaInfo_DLL_GNU_FromSource.tar.bz2 MediaInfo_DLL_GNU_FromSource/*

#-----------------------------------------------------------------------
# Clean up
rm -r MediaInfo_DLL_GNU_FromSource
