#!/bin/sh

osascript_Function () {
	echo '
		tell application "Finder"
			tell disk "'${APPNAME}'"
				open
				set current view of container window to icon view
				set toolbar visible of container window to false
				set the bounds of container window to {400, 100, 950, 600}
				set viewOptions to the icon view options of container window
				set arrangement of viewOptions to not arranged
				set icon size of viewOptions to 72
				set background picture of viewOptions to file ".background:Logo_White.icns"
				set position of item "'${APPNAME_lower}'.pkg" of container window to {125, 175}
				set position of item "ReadMe.txt" of container window to {450, 125}
				set position of item "License.html" of container window to {450, 225}
				set position of item "History.txt" of container window to {450, 325}
				close
			end tell
		end tell
	' | osascript
}
