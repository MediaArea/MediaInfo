#!/bin/bash

PLATFORMPATH="/Applications/Xcode.app/Contents/Developer/Platforms"

export IPHONEOS_DEPLOYMENT_TARGET="8.0"

build_lib()
{
    project=$1
    target=$2
    platform=$3
    prefix=$4

    host=$target
    if [[ $host == "x86_64" ]]; then
        host="i386"
    elif [[ $host == "arm64" ]]; then
        host="arm"
    fi

    export CC="$(xcrun -sdk iphoneos -find clang)"
    export CXX="$(xcrun -sdk iphoneos -find clang++)"
    export AR="$(xcrun -sdk iphoneos -find ar)"
    export RANLIB="$(xcrun -sdk iphoneos -find ranlib)"
    export CFLAGS="-stdlib=libc++ -fembed-bitcode -arch ${target} -isysroot $PLATFORMPATH/$platform.platform/Developer/SDKs/$platform.sdk -miphoneos-version-min=$IPHONEOS_DEPLOYMENT_TARGET"
    export CXXFLAGS="-stdlib=libc++ -fembed-bitcode -arch ${target}  -isysroot $PLATFORMPATH/$platform.platform/Developer/SDKs/$platform.sdk -miphoneos-version-min=$IPHONEOS_DEPLOYMENT_TARGET"
    export LDFLAGS="-stdlib=libc++ -fembed-bitcode -arch ${target} -isysroot $PLATFORMPATH/$platform.platform/Developer/SDKs/$platform.sdk"

    mkdir -p $PWD/native/$target

    pushd ../../../../../$project/Project/GNU/Library
        ./configure --prefix="$prefix" --disable-shared --enable-static --host=$host-apple-darwin
        make clean
        make
        make install
    popd
}

build_lib ZenLib armv7 iPhoneOS "$PWD/native/armv7"
build_lib MediaInfoLib armv7 iPhoneOS "$PWD/native/armv7"
build_lib ZenLib armv7s iPhoneOS "$PWD/native/armv7s"
build_lib MediaInfoLib armv7s iPhoneOS "$PWD/native/armv7s"
build_lib ZenLib arm64 iPhoneOS "$PWD/native/arm64"
build_lib MediaInfoLib arm64 iPhoneOS "$PWD/native/arm64"
build_lib ZenLib i386 iPhoneSimulator "$PWD/native/i386"
build_lib MediaInfoLib i386 iPhoneSimulator "$PWD/native/i386"
build_lib ZenLib x86_64 iPhoneSimulator "$PWD/native/x86_64"
build_lib MediaInfoLib x86_64 iPhoneSimulator "$PWD/native/x86_64"

LIPO=$(xcrun -sdk iphoneos -find lipo)

$LIPO -create $PWD/native/armv7/lib/libzen.a $PWD/native/armv7s/lib/libzen.a $PWD/native/arm64/lib/libzen.a $PWD/native/x86_64/lib/libzen.a $PWD/native/i386/lib/libzen.a -output $PWD/libzen.a
$LIPO -create $PWD/native/armv7/lib/libmediainfo.a $PWD/native/armv7s/lib/libmediainfo.a $PWD/native/arm64/lib/libmediainfo.a $PWD/native/x86_64/lib/libmediainfo.a $PWD/native/i386/lib/libmediainfo.a -output $PWD/libmediainfo.a
