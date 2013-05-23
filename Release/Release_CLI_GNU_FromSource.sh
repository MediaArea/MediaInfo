##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 #  Use of this source code is governed by a BSD-style license that can
 #  be found in the License.html file in the root of the source tree.
 ##

#! /bin/sh

#-----------------------------------------------------------------------
# Clean up
test -e MediaInfo_CLI_GNU_FromSource.tar     && rm    MediaInfo_CLI_GNU_FromSource.tar
test -e MediaInfo_CLI_GNU_FromSource.tar.bz2 && rm    MediaInfo_CLI_GNU_FromSource.tar.bz2
test -d MediaInfo_CLI_GNU_FromSource         && rm -r MediaInfo_CLI_GNU_FromSource
mkdir MediaInfo_CLI_GNU_FromSource


#-----------------------------------------------------------------------
# Preparing : Project
cd ../Project/GNU/CLI
chmod u+x autogen
./autogen
cd ../../../Release


#-----------------------------------------------------------------------
# Copying : Project
mkdir -p MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI
cp ../Project/GNU/CLI/aclocal.m4 MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/autogen MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/config.guess MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/config.sub MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/configure MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/configure.ac MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/depcomp MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/install-sh MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/ltmain.sh MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/Makefile.am MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/Makefile.in MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/
cp ../Project/GNU/CLI/missing MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/

#-----------------------------------------------------------------------
# Copying : Source
mkdir -p MediaInfo_CLI_GNU_FromSource/Source/Common
cp -r ../Source/Common/*.h MediaInfo_CLI_GNU_FromSource/Source/Common/
cp -r ../Source/Common/*.cpp MediaInfo_CLI_GNU_FromSource/Source/Common/
mkdir -p MediaInfo_CLI_GNU_FromSource/Source/CLI
cp -r ../Source/CLI/*.h MediaInfo_CLI_GNU_FromSource/Source/CLI/
cp -r ../Source/CLI/*.cpp MediaInfo_CLI_GNU_FromSource/Source/CLI/

#-----------------------------------------------------------------------
# Copying : Release
mkdir -p MediaInfo_CLI_GNU_FromSource/Release
cp Release_CLI_GNU.sub MediaInfo_CLI_GNU_FromSource/Release/
cp *.sh MediaInfo_CLI_GNU_FromSource/Release/
chmod u+x MediaInfo_CLI_GNU_FromSource/Release/*.sh

#-----------------------------------------------------------------------
# Copying : Information
cp ../License.html MediaInfo_CLI_GNU_FromSource/
cp ../History_CLI.txt MediaInfo_CLI_GNU_FromSource/
dos2unix MediaInfo_CLI_GNU_FromSource/History_CLI.txt
cp ReadMe_CLI_Linux.txt MediaInfo_CLI_GNU_FromSource/Release/
cp ReadMe_CLI_Mac.txt MediaInfo_CLI_GNU_FromSource/Release/

#-----------------------------------------------------------------------
# Copying : Contrib
mkdir -p MediaInfo_CLI_GNU_FromSource/Contrib
cp ../Contrib/CLI_Help.doc MediaInfo_CLI_GNU_FromSource/Contrib/


#-----------------------------------------------------------------------
# Preparing Archive : MediaInfo
mv MediaInfo_CLI_GNU_FromSource MediaInfo
mkdir MediaInfo_CLI_GNU_FromSource
mv MediaInfo MediaInfo_CLI_GNU_FromSource/

#-----------------------------------------------------------------------
# Preparing Archive : From MediaInfoLib
cd ../../MediaInfoLib/Release
. ./Release_Lib_GNU_FromSource.sh
tar xf MediaInfo_Lib_GNU_FromSource.tar.bz2
rm MediaInfo_Lib_GNU_FromSource.tar.bz2
cd ../../MediaInfo/Release
mv ../../MediaInfoLib/Release/MediaInfo_Lib_GNU_FromSource MediaInfo_CLI_GNU_FromSource/MediaInfoLib

#-----------------------------------------------------------------------
# Preparing Archive : From ZenLib
cd ../../ZenLib/Release
. ./Release_GNU_FromSource.sh
tar xf ZenLib_GNU_FromSource.tar.bz2
rm ZenLib_GNU_FromSource.tar.bz2
cd ../../MediaInfo/Release
mv ../../ZenLib/Release/ZenLib_GNU_FromSource MediaInfo_CLI_GNU_FromSource/ZenLib

#-----------------------------------------------------------------------
# Preparing Archive : From Shared
mkdir -p MediaInfo_CLI_GNU_FromSource/Shared/Project/_Common
cp -r ../../Shared/Project/_Common/*.sh MediaInfo_CLI_GNU_FromSource/Shared/Project/_Common
mkdir -p MediaInfo_CLI_GNU_FromSource/Shared/Project/curl
cp -r ../../Shared/Project/curl/*.sh MediaInfo_CLI_GNU_FromSource/Shared/Project/curl
mkdir -p MediaInfo_CLI_GNU_FromSource/Shared/Project/ZLib
cp -r ../../Shared/Project/ZLib/*.sh MediaInfo_CLI_GNU_FromSource/Shared/Project/ZLib
mkdir -p MediaInfo_CLI_GNU_FromSource/Shared/Project/ZLib/Template/projects/GNU
cp -r ../../Shared/Project/ZLib/Template/projects/GNU/* MediaInfo_CLI_GNU_FromSource/Shared/Project/ZLib/Template/projects/GNU
mkdir -p MediaInfo_CLI_GNU_FromSource/Shared/Project/WxWidgets
cp -r ../../Shared/Project/WxWidgets/*.sh MediaInfo_CLI_GNU_FromSource/Shared/Project/WxWidgets

#-----------------------------------------------------------------------
# Preparing Archive : Automation
cp ../Project/GNU/CLI/AddThisToRoot_CLI_compile.sh MediaInfo_CLI_GNU_FromSource/CLI_Compile.sh
chmod u+x MediaInfo_CLI_GNU_FromSource/CLI_Compile.sh


#-----------------------------------------------------------------------
# Compressing Archive
tar jchf MediaInfo_CLI_GNU_FromSource.tar.bz2 MediaInfo_CLI_GNU_FromSource/*

#-----------------------------------------------------------------------
# Clean up
rm -r MediaInfo_CLI_GNU_FromSource
