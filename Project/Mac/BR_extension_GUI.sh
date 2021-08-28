#!/bin/sh

# Because of the autotools bug
cd ZenLib/Project/GNU/Library
./autogen.sh
cd ../../../../MediaInfoLib/Project/GNU/Library
./autogen.sh
cd ../../../../MediaInfo/Project/GNU/GUI
./autogen.sh
cd ../../../..

./GUI_Compile.sh --with-wx-static --enable-arch-x86_64 --enable-arch-arm64
