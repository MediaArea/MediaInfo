#!/bin/sh

if [ $# != 2 ]; then
    echo
    echo "Usage: Make_MI_dmg.sh kind version"
    echo
    exit 1
fi

KIND="$1"
VERSION="$2"

if [ "$KIND" = "CLI" ] || [ "$KIND" = "cli" ]; then
    KIND="CLI"
elif [ "$KIND" = "GUI" ] || [ "$KIND" = "gui" ]; then
    KIND="GUI"
else
    echo
    echo "KIND must be either [CLI | cli] or [GUI | gui]"
    echo
    exit 1
fi

APPNAME="MediaInfo"
APPNAME_lower=$(echo "${APPNAME}" | awk '{print tolower($0)}')
KIND_lower=$(echo "${KIND}" | awk '{print tolower($0)}')
SIGNATURE="MediaArea.net"
FILES="tmp-${APPNAME}_${KIND}"
TEMPDMG="tmp-${APPNAME}_${KIND}.dmg"
FINALDMG="${APPNAME}_${KIND}_${VERSION}_Mac.dmg"

# Clean up
rm -fr "${FILES}-Root"
rm -fr "${FILES}"
rm -f "${APPNAME}.pkg"
rm -f "${TEMPDMG}"
rm -f "${FINALDMG}"

echo
echo ========== Create the package ==========
echo

mkdir -p "${FILES}/.background"
cp ../../License.html "${FILES}"
cp "../../Release/ReadMe_${KIND}_Mac.txt" "${FILES}/ReadMe.txt"
cp "../../History_${KIND}.txt" "${FILES}/History.txt"
cp Logo_White.icns "${FILES}/.background"

if [ "$KIND" = "CLI" ]; then

    cd ../GNU/CLI
    if test -e ".libs/${APPNAME_lower}"; then
        mv -f ".libs/${APPNAME_lower}" .
    fi
    if ! test -x "${APPNAME_lower}"; then
        echo
        echo "${APPNAME_lower} can’t be found, or this file isn’t a executable."
        echo
        exit 1
    fi
    strip -u -r "${APPNAME_lower}"
    cd ../../Mac

    mkdir -p "${FILES}-Root/usr/local/bin"
    cp "../GNU/CLI/${APPNAME_lower}" "${FILES}-Root/usr/local/bin"
    codesign -f --deep --options=runtime -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}-Root/usr/local/bin/${APPNAME_lower}"

    pkgbuild --root "${FILES}-Root" --identifier "net.mediaarea.${APPNAME_lower}.mac-${KIND_lower}" --sign "Developer ID Installer: ${SIGNATURE}" --version "${VERSION}" "${FILES}/${APPNAME_lower}.pkg"
    codesign -f --deep --options=runtime -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME_lower}.pkg"

fi

if [ "$KIND" = "GUI" ]; then

    cd ../GNU/GUI
    if test -e ".libs/${APPNAME_lower}-${KIND_lower}"; then
        mv -f ".libs/${APPNAME_lower}-${KIND_lower}" .
    fi
    if ! test -x "${APPNAME_lower}-${KIND_lower}"; then
        echo
        echo "${APPNAME_lower}-${KIND_lower} can’t be found, or this file isn’t a executable."
        echo
        exit 1
    fi
    strip -u -r "${APPNAME_lower}-${KIND_lower}"
    cd ../../Mac

    mkdir -p "${FILES}/${APPNAME}.app/Contents/MacOS"
    mkdir "${FILES}/${APPNAME}.app/Contents/Resources"
    cp "../GNU/GUI/${APPNAME_lower}-${KIND_lower}" "${FILES}/${APPNAME}.app/Contents/MacOS/${APPNAME}"
    cp Info.plist "${FILES}/${APPNAME}.app/Contents"
    sed -i '' -e "s/VERSION/${VERSION}/g" "${FILES}/${APPNAME}.app/Contents/Info.plist"
    printf '%s' 'APPL????' > "${FILES}/${APPNAME}.app/Contents/PkgInfo"
    cp ${APPNAME}.icns "${FILES}/${APPNAME}.app/Contents/Resources"
    
    codesign -f --deep --options=runtime -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/MacOS/${APPNAME}"
    codesign -f --deep --options=runtime -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app"

fi

echo
echo ========== Create the disk image ==========
echo

# Check if an old image isn't already attached
DEVICE=$(hdiutil info | grep -B 1 "/Volumes/${APPNAME}" | grep -E '^/dev/' | sed 1q | awk '{print $1}')
test -e "$DEVICE" && hdiutil detach -force "${DEVICE}"

hdiutil create "${TEMPDMG}" -ov -fs HFS+ -format UDRW -volname "${APPNAME}" -srcfolder "${FILES}"
DEVICE=$(hdiutil attach -readwrite -noverify "${TEMPDMG}" | grep -E '^/dev/' | sed 1q | awk '{print $1}')
sleep 2

cd "/Volumes/${APPNAME}"
if [ "$KIND" = "GUI" ]; then
    ln -s /Applications
fi
test -e .DS_Store && rm -fr .DS_Store
cd - >/dev/null

. "Osascript_${KIND}.sh"
osascript_Function

hdiutil detach "${DEVICE}"
sleep 2

echo
echo ========== Convert to compressed image ==========
echo
hdiutil convert "${TEMPDMG}" -format UDBZ -o "${FINALDMG}"

codesign -f --deep --options=runtime -s "Developer ID Application: ${SIGNATURE}" --verbose "${FINALDMG}"

unset -v APPNAME APPNAME_lower KIND KIND_lower VERSION SIGNATURE
unset -v TEMPDMG FINALDMG FILES DEVICE
