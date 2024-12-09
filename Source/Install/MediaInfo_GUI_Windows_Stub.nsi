#NSIS: encoding=UTF-8
RequestExecutionLevel admin

; Some defines
!define PRODUCT_NAME "MediaInfo"
!define PRODUCT_PUBLISHER "MediaArea.net"
!define PRODUCT_VERSION "24.12"
!define PRODUCT_VERSION4 "${PRODUCT_VERSION}.0.0"

!define BASEURL "https://mediaarea.net/download/binary/mediainfo-gui/${PRODUCT_VERSION}"
!ifdef SNAPSHOT
  !define /redef BASEURL "https://mediaarea.net/download/snapshots/binary/mediainfo-gui/${SNAPSHOT}"
!endif

; Compression
SetCompressor /FINAL /SOLID lzma

; x64 stuff
!include "x64.nsh"

; Logic stuff
!include LogicLib.nsh

; Windows version stuff
!include WinVer.nsh

; Installer icon
Icon "..\..\Source\Resource\Image\MediaInfo.ico"

; Enable Visual Styles
XPStyle on

; Info
VIProductVersion "${PRODUCT_VERSION4}"
VIAddVersionKey /LANG=0 "CompanyName"      "${PRODUCT_PUBLISHER}"
VIAddVersionKey /LANG=0 "ProductName"      "${PRODUCT_NAME}"
VIAddVersionKey /LANG=0 "ProductVersion"   "${PRODUCT_VERSION4}"
VIAddVersionKey /LANG=0 "FileDescription"  "All about your audio and video files"
VIAddVersionKey /LANG=0 "FileVersion"      "${PRODUCT_VERSION4}"
VIAddVersionKey /LANG=0 "LegalCopyright"   "${PRODUCT_PUBLISHER}"
VIAddVersionKey /LANG=0 "OriginalFilename" "${PRODUCT_NAME}_GUI_${PRODUCT_VERSION}_Windows_Online.exe"
BrandingText " "

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\Release\${PRODUCT_NAME}_GUI_${PRODUCT_VERSION}_Windows_Online.exe"
ShowInstDetails nevershow

Section
  HideWindow
  InitPluginsDir
  SetOutPath "$PLUGINSDIR"
  Delete "$PLUGINSDIR\MediaInfoInstaller.exe"

  ${If} ${IsNativeARM64}
    ${AndIf} ${AtLeastWin11}
      inetc::get /CAPTION "MediaInfo Online Installer" /BANNER "Downloading MediaInfo ${PRODUCT_VERSION} ARM64..." \
      "${BASEURL}/MediaInfo_GUI_${PRODUCT_VERSION}_Windows_ARM64.exe" "$PLUGINSDIR\MediaInfoInstaller.exe"
  ${Else}
    ${If} ${IsNativeAMD64}
      inetc::get /CAPTION "MediaInfo Online Installer" /BANNER "Downloading MediaInfo ${PRODUCT_VERSION} x64..." \
      "${BASEURL}/MediaInfo_GUI_${PRODUCT_VERSION}_Windows_x64.exe" "$PLUGINSDIR\MediaInfoInstaller.exe"
    ${Else}
      inetc::get /CAPTION "MediaInfo Online Installer" /BANNER "Downloading MediaInfo ${PRODUCT_VERSION} i386..." \
      "${BASEURL}/MediaInfo_GUI_${PRODUCT_VERSION}_Windows_i386.exe" "$PLUGINSDIR\MediaInfoInstaller.exe"
    ${EndIf}
  ${EndIf}

  Pop $0
  ${If} $0 == "OK"
    ${IfNot} ${Silent}
      ExecWait '"$PLUGINSDIR\MediaInfoInstaller.exe"'
    ${Else}
      ExecWait '"$PLUGINSDIR\MediaInfoInstaller.exe" /S'
    ${EndIf}
  ${Else}
    MessageBox MB_OK "Download Status: $0"
  ${EndIf}

  ; Make sure PLUGINSDIR is not locked and can be deleted
  SetOutPath "$TEMP"

  Quit
SectionEnd
