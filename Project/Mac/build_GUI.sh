#! /bin/sh

# Because of the autotools bug
cd ZenLib/Project/GNU/Library
./autogen.sh
cd ../../../../MediaInfoLib/Project/GNU/Library
./autogen.sh
cd ../../../../MediaInfo/Project/GNU/GUI
./autogen.sh
cd ../../../..

# Dependencies
mkdir -p Shared/Source
cp -r ~/Documents/almin/WxWidgets Shared/Source

# Because wx doesn't compile in 32 bits
#./GUI_Compile.sh --with-wx-static --enable-arch-x86_64 --enable-arch-i386
./GUI_Compile.sh --with-wx-static

strip -u -r MediaInfo/Project/GNU/GUI/mediainfo-gui
