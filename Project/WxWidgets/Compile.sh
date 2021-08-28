#!/bin/sh

# Initialization
test -d ../../Source || mkdir -p ../../Source
WX_source=../../Source/WxWidgets

##########################################################################
# Fetch if necessary
if test -e "$WX_source/configure"; then
    echo
    echo "The source of WxWidgets are presents"
    echo
else
    echo
    echo "Downloading WxWidgets..."
    echo
    rm -fr "$WX_source"
    git clone -b "v3.0.2" https://github.com/wxWidgets/wxWidgets "$WX_source"
    if test -e "$WX_source/configure"; then
        echo
        echo "WxWidgets downloaded, compiling it"
        echo
    else
        echo
        echo "Error while downloading WxWidgets"
        echo
        exit 1
    fi
fi

##########################################################################
# Already compiled
if test -e "$WX_source"/lib/libwx_*; then
    echo
    echo "WxWidgets is already compiled, recompiling it"
    echo
fi

##########################################################################
# Compile
cd "$WX_source"
test -e Makefile && make clean
echo
echo "Compiling WxWidgets..."
echo
./configure --disable-shared --disable-gui --enable-unicode --enable-monolithic $*
make clean
make

unset -v WX_source
