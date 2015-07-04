#!/bin/sh

# codesign and osascript requires an GUI session

if [ $# != 3 ]; then
	echo
    echo "Usage: mkdmg appname kind version"
	echo
    echo "appname: [MediaInfo | mi] or [MediaConch | mc]"
    echo "kind: [CLI | cli] or [GUI | gui]"
	echo
    exit 1
fi

APPNAME="$1"
KIND="$2"
VERSION="$3"

echo
echo
echo

if [ "$KIND" = "CLI" ] || [ "$KIND" = "cli" ]; then
	echo "CLI "
	KIND="CLI"
elif [ "$KIND" = "GUI" ] || [ "$KIND" = "gui" ]; then
	KIND="GUI"
	echo "GUI "
else
	echo
	echo "KIND must be either [MediaInfo | mi] or [MediaConch | mc]"
	echo
	exit 1
fi
if [ "$APPNAME" = "MediaInfo" ] || [ "$APPNAME" = "mediainfo" ] || [ "$APPNAME" = "mi" ]; then
	echo "MediaInfo"
	APPNAME="MediaInfo"
elif [ "$APPNAME" = "MediaConch" ] || [ "$APPNAME" = "mediaconch" ] || [ "$APPNAME" = "mc" ]; then
	echo "MediaConch"
	APPNAME="MediaConch"
else
	echo
	echo "APPNAME must be either [CLI | cli] or [GUI | gui]"
	echo
	exit 1
fi

echo
echo
echo

APPNAME_lower=`echo ${APPNAME} |awk '{print tolower($0)}'`
KIND_lower=`echo ${KIND} |awk '{print tolower($0)}'`
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

	mkdir -p "${FILES}-Root/usr/local/bin"
	cp "../GNU/CLI/${APPNAME_lower}" "${FILES}-Root/usr/local/bin"
	codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}-Root/usr/local/bin/${APPNAME_lower}"

	pkgbuild --root "${FILES}-Root" --identifier "net.mediaarea.${APPNAME_lower}.mac-${KIND_lower}" --sign "Developer ID Installer: ${SIGNATURE}" --version "${VERSION}" "${FILES}/${APPNAME_lower}.pkg"
	codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME_lower}.pkg"

fi

if [ "$KIND" = "GUI" ]; then

	if [ "$APPNAME" = "MediaInfo" ]; then

		mkdir -p "${FILES}/${APPNAME}.app/Contents/MacOS"
		mkdir "${FILES}/${APPNAME}.app/Contents/Resources"
		cp "../GNU/GUI/${APPNAME_lower}-${KIND_lower}" "${FILES}/${APPNAME}.app/Contents/MacOS/${APPNAME}"
		cp Info.plist "${FILES}/${APPNAME}.app/Contents"
		sed -i '' -e "s/VERSION/${VERSION}/g" "${FILES}/${APPNAME}.app/Contents/Info.plist"
		echo -n 'APPL????' > "${FILES}/${APPNAME}.app/Contents/PkgInfo"
		cp ${APPNAME}.icns "${FILES}/${APPNAME}.app/Contents/Resources"
		
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/MacOS/${APPNAME}"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app"

	fi
	
	if [ "$APPNAME" = "MediaConch" ]; then

		mkdir "${FILES}/${APPNAME}.app"
		cp -r "../Qt/${APPNAME}.app" "${FILES}/"
		sed -i '' -e "s/VERSION/${VERSION}/g" "${FILES}/${APPNAME}.app/Contents/Info.plist"
		macdeployqt "${FILES}/${APPNAME}.app"
		
		#Temporary, due to Qt 5.3 not copying Info.plist
		cp "${HOME}/Qt/5.3/clang_64/lib/QtCore.framework/Contents/Info.plist" "${FILES}/${APPNAME}.app/Contents/Frameworks/QtCore.framework/Resources/"
		cp "${HOME}/Qt/5.3/clang_64/lib/QtGui.framework/Contents/Info.plist" "${FILES}/${APPNAME}.app/Contents/Frameworks/QtGui.framework/Resources/"
		cp "${HOME}/Qt/5.3/clang_64/lib/QtPrintSupport.framework/Contents/Info.plist" "${FILES}/${APPNAME}.app/Contents/Frameworks/QtPrintSupport.framework/Resources/"
		cp "${HOME}/Qt/5.3/clang_64/lib/QtWidgets.framework/Contents/Info.plist" "${FILES}/${APPNAME}.app/Contents/Frameworks/QtWidgets.framework/Resources/"
		
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtCore.framework/Versions/5/QtCore"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets"
		
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtCore.framework"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtGui.framework"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtPrintSupport.framework"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/Frameworks/QtWidgets.framework"
		
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/accessible/libqtaccessiblewidgets.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqdds.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqgif.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqicns.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqico.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqjp2.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqjpeg.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqmng.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqtga.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqtiff.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqwbmp.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/imageformats/libqwebp.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/platforms/libqcocoa.dylib"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/PlugIns/printsupport/libcocoaprintersupport.dylib"
		
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app/Contents/MacOS/${APPNAME}"
		codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FILES}/${APPNAME}.app"
		
		# pkg creation
		productbuild --component "${FILES}/${APPNAME}.app" /Applications --sign "3rd Party Mac Developer Installer: ${SIGNATURE}" --product "${FILES}/${APPNAME}.app/Contents/Info.plist" "${APPNAME}.pkg"
	
	fi
fi

echo
echo ========== Create the disk image ==========
echo

# Check if an old image isn't already attached
DEVICE=$(hdiutil info |grep -B 1 "/Volumes/${APPNAME}" |egrep '^/dev/' | sed 1q | awk '{print $1}')
test -e "$DEVICE" && hdiutil detach "${DEVICE}"

hdiutil create "${TEMPDMG}" -ov -format UDRW -volname "${APPNAME}" -srcfolder "${FILES}"
DEVICE=$(hdiutil attach -readwrite -noverify "${TEMPDMG}" | egrep '^/dev/' | sed 1q | awk '{print $1}')
sleep 2

cd "/Volumes/${APPNAME}"
if [ "$KIND" = "GUI" ]; then
	ln -s /Applications
fi
test -e .DS_Store && rm -fr .DS_Store
cd - >/dev/null

. osascript_${APPNAME}_${KIND}.sh
osascript_Function

hdiutil detach "${DEVICE}"
sleep 2

echo
echo ========== Convert to compressed image ==========
echo
hdiutil convert "${TEMPDMG}" -format UDBZ -o "${FINALDMG}"
codesign -f -s "Developer ID Application: ${SIGNATURE}" --verbose "${FINALDMG}"

# Clean up
rm -fr "${FILES}-Root"
rm -fr "${FILES}"
rm -f "${TEMPDMG}"

unset -v APPNAME APPNAME_lower KIND KIND_lower VERSION SIGNATURE
unset -v TEMPDMG FINALDMG FILES DEVICE
