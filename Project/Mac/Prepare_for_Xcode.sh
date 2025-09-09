#!/bin/sh

cp ../dylib_for_xcode/MediaInfoLib/libmediainfo.dylib MediaInfo/Source/GUI/Cocoa/libmediainfo.dylib

cd MediaInfo/Source/GUI/Cocoa
install_name_tool -id @executable_path/../Resources/libmediainfo.dylib libmediainfo.dylib

./copy_locales.sh
