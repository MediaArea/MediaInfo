#NSIS: encoding=UTF-8
RequestExecutionLevel admin

; Some defines
!define PRODUCT_NAME "MediaInfo"
!define PRODUCT_PUBLISHER "MediaArea.net"
!define PRODUCT_VERSION "24.12"
!define PRODUCT_VERSION4 "${PRODUCT_VERSION}.0.0"

; Compression
SetCompressor /FINAL /SOLID lzma

; x64 stuff
!include "x64.nsh"

; Logic stuff
!include LogicLib.nsh

; String stuff
!include StrFunc.nsh
${Using:StrFunc} StrTrimNewLines

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
VIAddVersionKey /LANG=0 "OriginalFilename" "${PRODUCT_NAME}_GUI_Windows_Online.exe"
BrandingText " "

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\Release\${PRODUCT_NAME}_GUI_Windows_Online.exe"
ShowInstDetails nevershow

; Variables
Var LATEST_VERSION
Var ARCH_SELECTED
Var VERSION_SELECTED
Var FILENAME

Section
  HideWindow
  InitPluginsDir
  SetOutPath "$PLUGINSDIR"
  Delete "$PLUGINSDIR\MediaInfoInstaller.exe"
  Delete "$PLUGINSDIR\version.txt"

  inetc::get /CAPTION "MediaInfo GUI Online Installer" /BANNER "Downloading version information..." \
    "https://mediaarea.net/mediainfo_check/version.txt" "$PLUGINSDIR\version.txt"

  FileOpen $4 "$PLUGINSDIR\version.txt" r
  FileRead $4 $LATEST_VERSION
  FileClose $4

  ${If} ${AtLeastWinVista}
    ${If} ${IsNativeARM64}
      ${AndIf} ${AtLeastWin11}
        StrCpy $ARCH_SELECTED "ARM64"
    ${ElseIf} ${IsNativeAMD64}
      StrCpy $ARCH_SELECTED "x64"
    ${Else}
      StrCpy $ARCH_SELECTED "i386"
    ${EndIf}
    ${StrTrimNewLines} $VERSION_SELECTED $LATEST_VERSION
    StrCpy $FILENAME "MediaInfo_GUI_$VERSION_SELECTED_Windows_$ARCH_SELECTED.exe"
  ${ElseIf} ${AtLeastWinXP}
    StrCpy $VERSION_SELECTED "21.03"
    StrCpy $ARCH_SELECTED "Universal"
    StrCpy $FILENAME "MediaInfo_GUI_$VERSION_SELECTED_Windows.exe"
  ${Else}
    StrCpy $VERSION_SELECTED "0.7.60"
    StrCpy $ARCH_SELECTED "i386"
    StrCpy $FILENAME "MediaInfo_GUI_$VERSION_SELECTED_Windows_$ARCH_SELECTED.exe"
  ${EndIf}

  inetc::get /CAPTION "MediaInfo GUI Online Installer" /BANNER "Downloading MediaInfo GUI $VERSION_SELECTED $ARCH_SELECTED..." \
    "https://mediaarea.net/download/binary/mediainfo-gui/$VERSION_SELECTED/$FILENAME" "$PLUGINSDIR\MediaInfoInstaller.exe"

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
