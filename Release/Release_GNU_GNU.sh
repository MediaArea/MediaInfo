# MediaInfo
# Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http:#www.gnu.org/licenses/>.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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