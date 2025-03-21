#NSIS: encoding=UTF-8
RequestExecutionLevel admin
ManifestDPIAware true

; Some defines
!define PRODUCT_NAME "MediaInfo"
!define PRODUCT_PUBLISHER "MediaArea.net"
!define PRODUCT_VERSION "25.03"
!define PRODUCT_VERSION4 "${PRODUCT_VERSION}.0.0"

; Compression
SetCompressor /FINAL /SOLID lzma

; x64 stuff
!include x64.nsh

; Logic stuff
!include LogicLib.nsh

; Windows version stuff
!include WinVer.nsh

; String functions
!include StrFunc.nsh
${Using:StrFunc} StrTrimNewLines

; Word functions
!include WordFunc.nsh

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
  Pop $0
  ${If} $0 != "OK"
    MessageBox MB_OK "Download Status: $0"
    goto Exit
  ${EndIf}

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
    MessageBox MB_OK "Please contact us at info@mediaarea.net for old versions."
    goto Exit
  ${EndIf}

  inetc::get /CAPTION "MediaInfo GUI Online Installer" /BANNER "Downloading MediaInfo GUI $VERSION_SELECTED $ARCH_SELECTED..." \
    "https://mediaarea.net/download/binary/mediainfo-gui/$VERSION_SELECTED/$FILENAME" "$PLUGINSDIR\MediaInfoInstaller.exe"
  Pop $0
  ${If} $0 != "OK"
    MessageBox MB_OK "Download Status: $0"
    goto Exit
  ${EndIf}

  ; Verify digital signature of the downloaded installer
  ${VersionCompare} "$VERSION_SELECTED" "22.11" $R8
  ${If} $R8 == "1" 
    CertCheck::CheckPETrustAndInfoAsync "$PLUGINSDIR\MediaInfoInstaller.exe" "MEDIAAREA.NET" "SSL.com Code Signing Intermediate CA RSA R1"
  ${Else}
    CertCheck::CheckPETrustAndInfoAsync "$PLUGINSDIR\MediaInfoInstaller.exe" "MediaArea.net" "Sectigo RSA Code Signing CA"
  ${EndIf}
  CertCheckGetStatus:
    CertCheck::GetStatus
    Pop $0
    ${If} $0 == 0
      sleep 10
      goto CertCheckGetStatus
    ${EndIf}
  Pop $0
  Pop $1
  ${If} $0 == 0
    ${AndIf} $1 == 0
      MessageBox mb_iconStop "Installer signed with an unexpected cert that is untrusted!"
      goto Exit
  ${ElseIf} $0 == 0
    MessageBox mb_iconStop "Installer signed with an untrusted cert!"
    goto Exit
  ${ElseIf} $1 == 0
    MessageBox mb_iconStop "Installer signed with an unexpected cert!"
    goto Exit
  ${EndIf}

  ; Execute the downloaded installer
  ${IfNot} ${Silent}
    ExecWait '"$PLUGINSDIR\MediaInfoInstaller.exe"'
  ${Else}
    ExecWait '"$PLUGINSDIR\MediaInfoInstaller.exe" /S'
  ${EndIf}

  Exit:
  ; Make sure PLUGINSDIR is not locked and can be deleted
  SetOutPath "$TEMP"

  Quit
SectionEnd
