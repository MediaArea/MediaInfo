#NSIS: encoding=UTF-8
; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Some defines
!define PRODUCT_NAME "MediaInfo"
!define PRODUCT_PUBLISHER "MediaArea.net"
!define PRODUCT_VERSION "18.12"
!define PRODUCT_VERSION4 "${PRODUCT_VERSION}.0.0"
!define PRODUCT_WEB_SITE "http://MediaArea.net/MediaInfo"
!define COMPANY_REGISTRY_OLD "Software\MediaArea.net"
!define PRODUCT_REGISTRY_OLD "Software\MediaArea.net\MediaInfo"
!define COMPANY_REGISTRY "Software\MediaArea"
!define PRODUCT_REGISTRY "Software\MediaArea\MediaInfo"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\MediaInfo.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; Compression
SetCompressor /FINAL /SOLID lzma

; x64 stuff
!include "x64.nsh"

; MediaInfo stuff
!include "MediaInfo_Extensions.nsh"

; File size
!include FileFunc.nsh
!include WinVer.nsh

; Modern UI
!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_ICON "..\..\Source\Resource\Image\MediaInfo.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Installer pages
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
; !define MUI_FINISHPAGE_RUN "$INSTDIR\MediaInfo.exe" //Removing it because it is run in admin privileges
!define MUI_WELCOMEFINISHPAGE_BITMAP "..\..\Source\Resource\Image\Windows_Finish.bmp"
!insertmacro MUI_PAGE_FINISH
; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_RESERVEFILE_LANGDLL

; Info
VIProductVersion "${PRODUCT_VERSION4}"
VIAddVersionKey "CompanyName"      "${PRODUCT_PUBLISHER}"
VIAddVersionKey "ProductName"      "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion"   "${PRODUCT_VERSION4}"
VIAddVersionKey "FileDescription"  "All about your audio and video files"
VIAddVersionKey "FileVersion"      "${PRODUCT_VERSION4}"
VIAddVersionKey "LegalCopyright"   "${PRODUCT_PUBLISHER}"
VIAddVersionKey "OriginalFilename" "${PRODUCT_NAME}_GUI_${PRODUCT_VERSION}_Windows.exe"
BrandingText " "

; Modern UI end

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\Release\${PRODUCT_NAME}_GUI_${PRODUCT_VERSION}_Windows.exe"
InstallDir "$PROGRAMFILES64\${PRODUCT_NAME}"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails nevershow
ShowUnInstDetails nevershow

Function .onInit
  ${If} ${RunningX64}
    SetRegView 64
  ${EndIf}
  !insertmacro MUI_LANGDLL_DISPLAY
  
  ; Increment install count
  ReadRegDWORD $0 HKCU "${PRODUCT_REGISTRY}" "InstallCount"
  IntOp $0 $0 + 1
  WriteRegDWORD HKCU "${PRODUCT_REGISTRY}" "InstallCount" $0
FunctionEnd

Section "SectionPrincipale" SEC01
  SetOverwrite on
  SetOutPath "$SMPROGRAMS"
  CreateShortCut "$SMPROGRAMS\MediaInfo.lnk" "$INSTDIR\MediaInfo.exe" "" "" "" "" "" "Convenient unified display of the most relevant technical and tag data for video and audio files"
  SetOutPath "$INSTDIR"
  File "/oname=MediaInfo.exe" "..\..\Project\BCB\GUI\Win32\Release\MediaInfo_GUI.exe"
  File "/oname=MediaInfo_i386.dll" "..\..\..\MediaInfoLib\Project\MSVC2017\Win32\Release\MediaInfo.dll"
  ${If} ${RunningX64}
    File "..\..\..\MediaInfoLib\Project\MSVC2017\x64\Release\MediaInfo_InfoTip.dll"
    File "..\..\..\MediaInfoLib\Project\MSVC2017\x64\Release\MediaInfo.dll"
  ${Else}
    File "..\..\..\MediaInfoLib\Project\MSVC2017\Win32\Release\MediaInfo_InfoTip.dll"
    File "..\..\..\MediaInfoLib\Project\MSVC2017\Win32\Release\MediaInfo.dll"
  ${EndIf}
  File "$%BPATH%\Windows\libcurl\Win32\Release\LIBCURL.DLL"
  File "$%BPATH%\Windows\libcurl\curl-ca-bundle.crt"
  File "/oname=History.txt" "..\..\History_GUI.txt"
  File "..\..\License.html"
  File "/oname=ReadMe.txt" "..\..\Release\ReadMe_GUI_Windows.txt"
  SetOverwrite try
  SetOutPath "$INSTDIR\Plugin\Custom"
  File "..\Resource\Plugin\Custom\*.csv"
  SetOutPath "$INSTDIR\Plugin\Language"
  File "..\Resource\Plugin\Language\*.csv"
  SetOutPath "$INSTDIR\Plugin\Sheet"
  File "..\Resource\Plugin\Sheet\*.csv"
  SetOutPath "$INSTDIR\Plugin\Tree"
  File "..\Resource\Plugin\Tree\*.csv"

  # Create files
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"

  # Delete files that might be present from older installation
  Delete "$INSTDIR\History_GUI.txt"
  Delete "$INSTDIR\Licence.txt"
  Delete "$INSTDIR\Licence.html"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\ReadMe_Windows.txt"
  Delete "$SMPROGRAMS\MediaInfo\Website.lnk"
  Delete "$SMPROGRAMS\MediaInfo\Uninstall.lnk"
  Delete "$SMPROGRAMS\MediaInfo\History.lnk"
  Delete "$SMPROGRAMS\MediaInfo\MediaInfo.lnk"
  RMDir  "$SMPROGRAMS\MediaInfo"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\MediaInfo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName"     "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher"       "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon"     "$INSTDIR\MediaInfo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion"  "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout"    "${PRODUCT_WEB_SITE}"
  Exec 'regsvr32 "$INSTDIR\MediaInfo_InfoTip.dll" /s'
  !insertmacro MediaInfo_Extensions_Install

  ${If} ${AtLeastWin7}
    ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
    IntFmt $0 "0x%08X" $0 ; Convert the decimal KB value in $0 to DWORD, put it right back into $0
    WriteRegDWORD ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "EstimatedSize" "$0" ; Create/Write the reg key with the dword value
  ${EndIf}
SectionEnd


Section Uninstall
  SetRegView 64
  !insertmacro MediaInfo_Extensions_Uninstall
  Exec 'regsvr32 "$INSTDIR\MediaInfo_InfoTip.dll" /u /s'
  Sleep 3000

  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\MediaInfo.exe"
  Delete "$INSTDIR\MediaInfo_InfoTip.dll"
  Delete "$INSTDIR\MediaInfo.dll"
  Delete "$INSTDIR\MediaInfo_i386.dll"
  Delete "$INSTDIR\History.txt"
  Delete "$INSTDIR\License.html"
  Delete "$INSTDIR\License.NoModifications.html"
  Delete "$INSTDIR\ReadMe.txt"
  Delete "$INSTDIR\Plugin\MediaInfo.cfg"
  Delete "$INSTDIR\Plugin\Custom\*.csv"
  Delete "$INSTDIR\Plugin\Language\*.csv"
  Delete "$INSTDIR\Plugin\Sheet\*.csv"
  Delete "$INSTDIR\Plugin\Tree\*.csv"
  Delete "$SMPROGRAMS\MediaInfo\Uninstall.lnk"
  Delete "$SMPROGRAMS\MediaInfo\Website.lnk"
  Delete "$SMPROGRAMS\MediaInfo\MediaInfo.lnk"
  Delete "$SMPROGRAMS\MediaInfo\History.lnk"
  Delete "$SMPROGRAMS\MediaInfo.lnk"

  RMDir "$SMPROGRAMS\MediaInfo"
  RMDir "$INSTDIR\Plugin\Custom"
  RMDir "$INSTDIR\Plugin\Language"
  RMDir "$INSTDIR\Plugin\Sheet"
  RMDir "$INSTDIR\Plugin\Tree"
  RMDir "$INSTDIR\Plugin"
  RMDir "$INSTDIR"

  SetRegView 64
  DeleteRegKey HKLM "${PRODUCT_REGISTRY}"
  DeleteRegKey /ifempty HKLM "${COMPANY_REGISTRY}"
  DeleteRegKey HKCU "${PRODUCT_REGISTRY}"
  DeleteRegKey /ifempty HKCU "${COMPANY_REGISTRY}"
  DeleteRegKey HKLM "${PRODUCT_REGISTRY_OLD}"
  DeleteRegKey /ifempty HKLM "${COMPANY_REGISTRY_OLD}"
  DeleteRegKey HKCU "${PRODUCT_REGISTRY_OLD}"
  DeleteRegKey /ifempty HKCU "${COMPANY_REGISTRY_OLD}"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
