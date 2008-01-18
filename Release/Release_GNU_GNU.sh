#! /bin/sh

#-----------------------------------------------------------------------
# Clean up
rm    MediaInfo_GUI_GNU.tar
rm    MediaInfo_GUI_GNU.tar.bz2
rm -r MediaInfo_GUI_GNU
mkdir MediaInfo_GUI_GNU

#-----------------------------------------------------------------------
# Copying : Exe
cp ../Project/GNU/GUI/MediaInfo MediaInfo_GUI_GNU/MediaInfo

#-----------------------------------------------------------------------
# Copying : Licence files
cp ../Licence.html MediaInfo_GUI_GNU/

#-----------------------------------------------------------------------
# Copying : History
cp ../History.GUI.txt MediaInfo_GUI_GNU/

#-----------------------------------------------------------------------
# Copying : Information file
cp Readme.Linux.txt MediaInfo_GUI_GNU/

#-----------------------------------------------------------------------
# Compressing Archive
tar jcf MediaInfo_GUI_GNU.tar.bz2 MediaInfo_GUI_GNU
rm MediaInfo_GUI_GNU.tar

#-----------------------------------------------------------------------
# Clean up
#if "%1"=="SkipCleanUp" goto SkipCleanUp
rm -r MediaInfo_GUI_GNU
#:SkipCleanUp