##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 #  Use of this source code is governed by a BSD-style license that can
 #  be found in the License.html file in the root of the source tree.
 ##

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
# Copying : Information file
cp ../License.html MediaInfo_GUI_GNU/
cp ../History_GUI.txt MediaInfo_GUI_GNU/History.txt
cp ReadMe_GUI_Linux.txt MediaInfo_GUI_GNU/ReadMe.txt

#-----------------------------------------------------------------------
# Compressing Archive
tar jcf MediaInfo_GUI_GNU.tar.bz2 MediaInfo_GUI_GNU
rm MediaInfo_GUI_GNU.tar

#-----------------------------------------------------------------------
# Clean up
#if "%1"=="SkipCleanUp" goto SkipCleanUp
rm -r MediaInfo_GUI_GNU
#:SkipCleanUp