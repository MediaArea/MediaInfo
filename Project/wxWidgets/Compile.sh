cd ../../Source/WxWidgets
./configure --disable-shared --disable-gui --enable-unicode --enable-monolithic $*
make clean
make
