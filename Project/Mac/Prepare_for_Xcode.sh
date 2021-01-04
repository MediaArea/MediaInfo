#! /bin/sh

cp ../dylib_for_xcode/MediaInfoLib/libmediainfo.dylib MediaInfo/Source/GUI/Cocoa/libmediainfo.dylib

cd MediaInfo/Source/GUI/Cocoa
install_name_tool -id @executable_path/../Resources/libmediainfo.dylib libmediainfo.dylib

cd _i18n
chmod +x *pl *sh
./update_csv.sh all
./update_strings.sh all
./update_xibs.sh all
