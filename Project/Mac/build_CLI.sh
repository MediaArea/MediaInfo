#! /bin/sh

# Because of the autotools bug
cd ZenLib/Project/GNU/Library
./autogen.sh
cd ../../../../MediaInfoLib/Project/GNU/Library
./autogen.sh
cd ../../../../MediaInfo/Project/GNU/CLI
./autogen.sh
cd ../../../..

./CLI_Compile.sh --enable-arch-x86_64 --enable-arch-i386

strip -u -r MediaInfo/Project/GNU/CLI/mediainfo
