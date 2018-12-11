#!/bin/bash

#  Copyright (c) MediaArea.net SARL. All Rights Reserved.
#
#  Use of this source code is governed by a GPL v3+ and MPL v2+ license that can
#  be found in the License.html file in the root of the source tree.
#

# This script build mediainfo and mediainfo-gui AppImages
# This script is supposed to be run on a CentOS 6 container or chroot"

function Make_image() {
    local APP=$1 BIN=$2 DESKTOP=$3 ICON=$4 LOWERAPP=${1,,}

    mkdir -p $LOWERAPP/$LOWERAPP.AppDir
    pushd $LOWERAPP
    pushd $LOWERAPP.AppDir

    mkdir -p usr/bin

    cp $BIN usr/bin/
    cp $DESKTOP $ICON .

    if [ "$LOWERAPP" == "mediainfo-gui" ] ; then
        get_desktopintegration $LOWERAPP
    fi

    get_apprun
    # Multiple runs to ensure we catch indirect ones
    copy_deps; copy_deps; copy_deps; copy_deps
    move_lib
    delete_blacklisted
    if [ "$ARCH" == "x86_64" ] ; then
        cp -f /usr/lib64/libnss3.so usr/lib64
    else
        cp -f /usr/lib/libnss3.so usr/lib
    fi
    popd
    generate_appimage
    popd
}

# Detect host
if ! grep "CentOS release 6\..*" /etc/centos-release ; then
    echo "This script is supposed to be run on a CentOS 6 container or chroot"
    exit 1
fi

# Setup evironment
VERSION=18.12

if [ "$(arch)" == "i386" ] ; then
    ARCH="i686"
else
    ARCH="$(arch)"
fi

PREFIX=$PWD/prefix

# Get AppImage utils
curl -L -O https://github.com/probonopd/AppImages/raw/master/functions.sh

# Fix functions.sh
sed -i "s/-x86_64/-$ARCH/g" functions.sh

source ./functions.sh

# Add EPEL repository
curl -L -O https://dl.fedoraproject.org/pub/epel/epel-release-latest-6.noarch.rpm
rpm -i --nodeps epel-release-*.rpm

# Install build and AppImage dependencies
yum install -y file wget tar fuse-libs fuse gcc-c++ pkgconfig libtool automake autoconf \
               git zlib-devel libcurl-devel cairo-devel wxGTK-devel

# Compile ZenLib
if test -e ZenLib/Project/GNU/Library; then
    pushd ZenLib/Project/GNU/Library
    autoreconf -i

    if [ "$ARCH" == "x86_64" ] ; then
        ./configure --enable-shared --disable-static --prefix=/usr --libdir=/usr/lib64
    else
        ./configure --enable-shared --disable-static --prefix=/usr
    fi

    if test ! -e Makefile; then
        echo Problem while configuring ZenLib
        exit 1
    fi

    make -j$(nproc)

    if test ! -e libzen.la; then
        echo Problem while compiling ZenLib
        exit 1
    fi

    make install
    popd
else
    echo ZenLib directory is not found
    exit 1
fi

# Compile MediaInfoLib
if test -e MediaInfoLib/Project/GNU/Library; then
    pushd MediaInfoLib/Project/GNU/Library
    autoreconf -i

    if [ "$ARCH" == "x86_64" ] ; then
        ./configure --enable-shared --disable-static --prefix=/usr --libdir=/usr/lib64 --with-libcurl
    else
        ./configure --enable-shared --disable-static --prefix=/usr --with-libcurl
    fi

    if test ! -e Makefile; then
        echo Problem while configuring MediaInfoLib
        exit 1
    fi

    make -j$(nproc)

    if test ! -e libmediainfo.la; then
        echo Problem while compiling MediaInfoLib
        exit 1
    fi

    make install
    popd
else
    echo MediaInfoLib directory is not found
    exit 1
fi


# Compile MediaInfo
if test -e MediaInfo; then
    # CLI
    pushd MediaInfo/Project/GNU/CLI
    autoreconf -i
    ./configure --prefix=$PREFIX

    if test ! -e Makefile; then
        echo Problem while configuring MediaInfo
        exit 1
    fi

    make -j$(nproc)

    if test ! -e mediainfo; then
        echo Problem while compiling MediaInfo
        exit 1
    fi

    make install
    popd

    # GUI
    pushd MediaInfo/Project/GNU/GUI
    autoreconf -i
    ./configure --prefix=$PREFIX

    if test ! -e Makefile; then
        echo Problem while configuring MediaInfo GUI
        exit 1
    fi

    make -j$(nproc)

    if test ! -e mediainfo-gui; then
        echo Problem while compiling MediaInfo GUI
        exit 1
    fi

    make install
    popd
else
    echo MediaInfo directory is not found
    exit 1
fi

# Make appImages
cp ${PWD}/MediaInfo/Source/Resource/Image/MediaInfo.png mediainfo.png

cat > mediainfo.desktop <<EOF
[Desktop Entry]
Comment=Command-line utility for reading information from audio/video files
Name=mediainfo
Exec=mediainfo
Icon=mediainfo
Terminal=true
Categories=Multimedia;
EOF

Make_image mediainfo $PREFIX/bin/mediainfo \
                      ${PWD}/mediainfo.desktop \
                      ${PWD}/mediainfo.png

Make_image mediainfo-gui $PREFIX/bin/mediainfo-gui \
                          ${PWD}/MediaInfo/Project/GNU/GUI/mediainfo-gui.desktop \
                          ${PWD}/mediainfo.png
