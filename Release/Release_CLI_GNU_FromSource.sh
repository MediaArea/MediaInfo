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
cp ../Project/GNU/CLI/wxwin.m4 MediaInfo_CLI_GNU_FromSource/Project/GNU/CLI/

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
cp Release_CLI_Linux_i386.sh MediaInfo_CLI_GNU_FromSource/Release/
cp Release_CLI_Linux_x64.sh MediaInfo_CLI_GNU_FromSource/Release/
cp Release_CLI_Mac_Intel.sh MediaInfo_CLI_GNU_FromSource/Release/
chmod u+x MediaInfo_CLI_GNU_FromSource/Release/Release_CLI_Linux_i386.sh
chmod u+x MediaInfo_CLI_GNU_FromSource/Release/Release_CLI_Linux_x64.sh
chmod u+x MediaInfo_CLI_GNU_FromSource/Release/Release_CLI_Mac_Intel.sh

#-----------------------------------------------------------------------
# Copying : Information
cp ../Licence.html MediaInfo_CLI_GNU_FromSource/
cp ../History.CLI.txt MediaInfo_CLI_GNU_FromSource/
cp ReadMe.Linux.txt MediaInfo_CLI_GNU_FromSource/Release/
cp ReadMe.Mac.txt MediaInfo_CLI_GNU_FromSource/Release/


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
mkdir -p MediaInfo_CLI_GNU_FromSource/Shared/Project/ZLib
cp -r ../../Shared/Project/ZLib/*.sh MediaInfo_CLI_GNU_FromSource/Shared/Project/ZLib

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
