#NSIS: encoding=UTF-8
RequestExecutionLevel admin
ManifestDPIAware true

; Some defines
!define PRODUCT_NAME "MediaInfo"
!define PRODUCT_PUBLISHER "MediaArea.net"
!define PRODUCT_VERSION "25.07"
!define PRODUCT_VERSION4 "${PRODUCT_VERSION}.0.0"

!define BASEURL "https://mediaarea.net/download/binary/mediainfo-gui/${PRODUCT_VERSION}"
!ifdef SNAPSHOT
  !define /redef BASEURL "https://mediaarea.net/download/snapshots/binary/mediainfo-gui/${SNAPSHOT}"
!endif

; Compression
SetCompressor /FINAL /SOLID lzma

; x64 stuff
!include x64.nsh

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

; Variables
Var ARCH_SELECTED

Section
  HideWindow
  InitPluginsDir
  SetOutPath "$PLUGINSDIR"
  Delete "$PLUGINSDIR\MediaInfoInstaller.exe"

  ${If} ${IsNativeARM64}
    ${AndIf} ${AtLeastWin11}
      StrCpy $ARCH_SELECTED "ARM64"
  ${ElseIf} ${IsNativeAMD64}
    StrCpy $ARCH_SELECTED "x64"
  ${Else}
    StrCpy $ARCH_SELECTED "i386"
  ${EndIf}

  inetc::get /CAPTION "MediaInfo GUI Online Installer" /BANNER "Downloading MediaInfo GUI ${PRODUCT_VERSION} $ARCH_SELECTED..." \
    "${BASEURL}/MediaInfo_GUI_${PRODUCT_VERSION}_Windows_$ARCH_SELECTED.exe" "$PLUGINSDIR\MediaInfoInstaller.exe"
  Pop $0
  ${If} $0 != "OK"
    MessageBox MB_OK "Download Status: $0"
    goto Exit
  ${EndIf}

  ; Verify digital signature of the downloaded installer
  CertCheck::CheckPETrustAndInfoAsync "$PLUGINSDIR\MediaInfoInstaller.exe" "MEDIAAREA.NET" "SSL.com Code Signing Intermediate CA RSA R1"
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
