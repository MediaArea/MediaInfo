# MediaInfo README

MediaInfo is a convenient unified display of the most relevant technical and tag data for video and audio files.

[![Build Status](https://travis-ci.org/MediaArea/MediaInfo.svg?branch=master)](https://travis-ci.org/MediaArea/MediaInfo)
[![Build status](https://ci.appveyor.com/api/projects/status/msq8b4fuqfy9jmvr/branch/master?svg=true)](https://ci.appveyor.com/project/MediaArea/mediainfo/branch/master)

# How to build MediaInfo

## Build under macOS and Linux

First, you must create a directory which will receive the MediaInfo directory, and ZenLib and MediaInfoLib if you decide to compile them yourself.

In this document, this directory will be referred as $BUILD_DIR.

### Dependencies under macOS

#### MacPorts

Some dependencies are available with MacPorts. To install MacPorts:
https://guide.macports.org/#installing

```sh
port install autoconf automake libtool pkgconfig zlib wxWidgets-3.0
```

*MediaArea tools*
* [libzen](#zenlib)
* [libmediainfo](#mediainfolib)

### Dependencies under Linux

#### Listing

*Build tools*

* git
* automake
* autoconf
* libtool
* pkgconfig
* make
* g++

*MediaArea tools*

* libzen0
* libmediainfo0

*CLI and GUI dependencies*

* zlib

*GUI only dependencies*

* wxwidgets

#### Ubuntu

*Build tools and CLI/GUI dependencies*

```sh
apt-get install git automake autoconf libtool pkg-config make g++ zlib1g-dev
```

*MediaArea tools*

Go to https://mediaarea.net/fr/MediaInfo/Download/Ubuntu and download the libmediainfo0, libmediainfo-dev, libzen0 and libzen-dev packages corresponding to your Ubuntu version. Then install them with :

```sh
dpkg -i libmediainfo* libzen*
```

*GUI only dependencies*

```sh
apt-get install libwxgtk3.2-dev
```

#### Fedora

*Build tools and CLI/GUI dependencies*

```sh
sudo yum install git automake autoconf libtool pkgconfig make gcc-c++ zlib-devel
```

*MediaArea tools*

Go to https://mediaarea.net/fr/MediaInfo/Download/Fedora and download the libmediainfo0, libmediainfo-devel, libzen0 and libzen-devel packages corresponding to your Fedora version and CPU architecture. Then install them with :

```sh
sudo yum install libmediainfo*.rpm libzen*.rpm
```

*GUI only dependencies*

```sh
sudo yum install wxGTK-devel desktop-file-utils
```

#### Debian

*Build tools and CLI/GUI dependencies*

```sh
apt-get install git automake autoconf libtool pkg-config make g++ zlib1g-dev
```

*MediaArea tools*

Go to https://mediaarea.net/fr/MediaInfo/Download/Debian and download the libmediainfo0, libmediainfo-dev, libzen0 and libzen-dev packages corresponding to your Debian version. Then install them with :

```sh
dpkg -i libmediainfo* libzen*
```

*GUI only dependencies*

```sh
apt-get install libwxgtk3.0-dev
```

#### OpenSUSE

*Build tools and CLI/GUI dependencies*

```sh
zypper install git automake autoconf libtool pkgconfig make gcc-c++ zlib-devel
```

*MediaArea tools*

Go to https://mediaarea.net/fr/MediaInfo/Download/openSUSE and download the libmediainfo0, libmediainfo-devel, libzen0 and libzen-devel packages corresponding to your OpenSuse version. Then install them with :

```sh
rpm -i libmediainfo* libzen*
```

*GUI only dependencies*

```sh
zypper install libqt4-devel libQtWebKit-devel update-desktop-files
```

### Build MediaInfo CLI

When you have done all the prerequisites for your configuration, then build MediaInfo. We start with the CLI.

```sh
cd $BUILD_DIR
git clone https://github.com/MediaArea/MediaInfo.git
cd MediaInfo/Project/GNU/CLI
./autogen.sh
```

##### Then, under macOS:

```sh
./configure --enable-staticlibs
make
```

##### Under Linux:

```sh
./configure --enable-shared
make
```

Or for statically linked executable, if you also build ZenLib and MediaInfoLib:
```sh
./configure --enable-staticlibs
make
```

#### Launch the CLI

```sh
./mediainfo
```

### Build MediaInfo GUI

If you have already built the CLI, no need to run git twice. In fact, if you re-run git with an existing MediaInfo directory, git will complain and exit.

To compile MediaInfo GUI under macOS and Linux:

```sh
cd $BUILD_DIR
git clone https://github.com/MediaArea/MediaInfo.git
cd MediaInfo/Project/GNU/GUI
./autogen.sh
```

##### Then, under macOS:

```sh
export PATH=$PATH:/opt/local/Library/Frameworks/wxWidgets.framework/Versions/wxWidgets/3/0/bin
./configure --enable-staticlibs
make
```

##### Under Linux:

```sh
./configure --enable-shared
make
```

Or for statically linked executable, if you also build ZenLib and MediaInfoLib:
```sh
./configure --enable-staticlibs
make
```

#### Launch the GUI

```sh
./mediainfo-gui
```

### Optional : build ZenLib and MediaInfoLib

#### ZenLib

To compile ZenLib under macOS and Linux:

```sh
cd $BUILD_DIR
git clone https://github.com/MediaArea/ZenLib.git
cd ZenLib/Project/GNU/Library
./autogen.sh
./configure --enable-static
make
```

#### MediaInfoLib

To compile MediaInfoLib under macOS and Linux:

```sh
cd $BUILD_DIR
git clone https://github.com/MediaArea/MediaInfoLib.git
cd MediaInfoLib/Project/GNU/Library
./autogen.sh
./configure --enable-static
make
```

MediaInfo - https://github.com/MediaArea/MediaInfo
Copyright (c) MediaArea.net SARL. All Rights Reserved.

This program is freeware under BSD-2-Clause license conditions.
See the [License](https://mediaarea.net/en/MediaInfo/License) for more information
