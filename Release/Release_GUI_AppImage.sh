#!/bin/bash

#  Copyright (c) MediaArea.net SARL. All Rights Reserved.
#
#  Use of this source code is governed by a BSD-style license that can
#  be found in the License.html file in the root of the source tree.
#

# dependencies (ubuntu 24.04):
# - build-essential
# - ninja-build
# - patchelf
# - pkgconf
# - cmake
# - bison
# - flex
# - curl
# - git
# - qt6-base-dev
# - libqt6svg6-dev
# - qt6-webengine-dev
# - qt6-tools-dev-tools
# - qt6-l10n-tools
# - qmake6
# - libcurl4-openssl-dev
# - libmms-dev
# - zlib1g-dev
# - libltdl-dev
# - libexpat1-dev
# - libpango1.0-dev

set -e # Fail on any error

#-----------------------------------------------------------------------
# Setup
release_directory="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
version="$(<"${release_directory}/../Project/version.txt")"
arch="${1:-"x86_64"}"

export APPIMAGE_EXTRACT_AND_RUN=1

export PKG_CONFIG_PATH="${release_directory}/AppDir/usr/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
export PATH="${release_directory}:${PATH}"
export ARCH="${arch}"

#-----------------------------------------------------------------------
# Cleanup
rm -fr "${release_directory}/MediaInfo_GUI_${version}_${arch}.appImage"
rm -fr "${release_directory}/linuxdeploy-plugin-qt.AppImage"
rm -fr "${release_directory}/appimagetool.AppImage"
rm -fr "${release_directory}/linuxdeploy.AppImage"
rm -fr "${release_directory}/graphviz"
rm -fr "${release_directory}/AppDir"
rm -fr "${release_directory}/AppRun"

#-----------------------------------------------------------------------
# Download AppImage tools
curl -L -o "${release_directory}/linuxdeploy.AppImage" "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${arch}.AppImage"
chmod +x "${release_directory}/linuxdeploy.AppImage"

curl -L -o "${release_directory}/linuxdeploy-plugin-qt.AppImage" "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-${arch}.AppImage"
chmod +x "${release_directory}/linuxdeploy-plugin-qt.AppImage"

curl -L -o "${release_directory}/appimagetool.AppImage" "https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-${arch}.AppImage"
chmod +x "${release_directory}/appimagetool.AppImage"

#-----------------------------------------------------------------------
# Build
pushd "${release_directory}"
    git clone --depth 1 https://gitlab.com/graphviz/graphviz.git --branch 14.1.5
    mkdir graphviz/build
    pushd graphviz/build
        cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DGRAPHVIZ_CLI=ON -DBUILD_SHARED_LIBS=ON -DENABLE_LTDL=ON -DWITH_GDK=OFF -DWITH_GHOSTSCRIPT=OFF -DWITH_GTK=OFF -DWITH_POPPLER=OFF -DWITH_RSVG=OFF -DWITH_WEBP=OFF -DWITH_X=OFF -DWITH_EXPAT=ON -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_INSTALL_PREFIX="${release_directory}/AppDir/usr" ..
        ninja install
    popd
    rm -fr ${release_directory}/AppDir/usr/bin
    rm -fr ${release_directory}/AppDir/usr/share
popd

pushd "${release_directory}/../../ZenLib/Project/GNU/Library"
    ./configure --prefix="${release_directory}/AppDir/usr" --enable-shared --disable-static
    make -j$(nproc)
    make install
    patchelf --set-rpath '$ORIGIN' "${release_directory}/AppDir/usr/lib/libzen.so"
popd

pushd "${release_directory}/../../MediaInfoLib/Project/GNU/Library"
    ./configure --prefix="${release_directory}/AppDir/usr" --with-libcurl --with-graphviz --with-libmms --enable-shared --disable-static
    make -j$(nproc)
    make install
    patchelf --set-rpath '$ORIGIN' "${release_directory}/AppDir/usr/lib/libmediainfo.so"
popd

pushd "${release_directory}/../Project/QMake/GUI"
    qmake6 MediaInfoQt.pro
    make -j$(nproc)
    install -D -m 0755 "mediainfo-gui" "${release_directory}/AppDir/usr/bin/mediainfo-gui"
    patchelf --set-rpath '$ORIGIN/../lib' "${release_directory}/AppDir/usr/bin/mediainfo-gui"
popd

pushd "${release_directory}"
    install -d "AppDir/usr/lib/graphviz"
    install -d "AppDir/usr/share/applications"
    install -d "AppDir/usr/share/metainfo"
    install -d "AppDir/usr/share/icons/hicolor/scalable/apps"
    install -d "AppDir/usr/share/icons/hicolor/256x256/apps"
    install -m 0644 "${release_directory}/../Project/GNU/GUI/mediainfo-gui.desktop" "AppDir/usr/share/applications/mediainfo-gui.desktop"
    install -m 0644 "${release_directory}/../Project/GNU/GUI/mediainfo-gui.metainfo.xml" "AppDir/usr/share/metainfo/mediainfo-gui.metainfo.xml"
    install -m 0644 "${release_directory}/../Source/Resource/Image/MediaInfo.svg" "AppDir/usr/share/icons/hicolor/scalable/apps/mediainfo.svg"
    install -m 0644 "${release_directory}/../Source/Resource/Image/MediaInfo.png" "AppDir/usr/share/icons/hicolor/256x256/apps/mediainfo.png"

    rm -fr AppDir/usr/include
    rm -fr AppDir/usr/lib/pkgconfig
    rm -fr AppDir/usr/lib/cmake
    rm -fr AppDir/usr/lib/*.la
popd

#-----------------------------------------------------------------------
# Package
pushd "${release_directory}"
    library_args=()
    while IFS= read -r lib; do
        library_args+=("--deploy-deps-only" "$lib")
    done < <(find AppDir/usr/lib -maxdepth 1 -type f -name "*.so*")

    echo -e '#!/bin/bash\n\nset -e\n\nthis_dir="$(dirname -- "$(readlink -f -- "$0")")"\n\nexport GVBINDIR="$this_dir/usr/lib/graphviz"\n\nexec "$this_dir/usr/bin/mediainfo-gui" "$@"' > "AppRun"
    chmod +x AppRun

    ./linuxdeploy.AppImage --appdir AppDir \
                           --deploy-deps-only AppDir/usr/bin/mediainfo-gui \
                           --desktop-file AppDir/usr/share/applications/mediainfo-gui.desktop \
                           --icon-file AppDir/usr/share/icons/hicolor/scalable/apps/mediainfo.svg \
                           --custom-apprun AppRun \
                           "${library_args[@]}" \
                           --plugin qt

    ./appimagetool.AppImage AppDir "MediaInfo_GUI_${version}_${arch}.AppImage"
popd
