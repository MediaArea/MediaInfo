##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
##
##  Use of this source code is governed by a BSD-style license that can
##  be found in the License.html file in the root of the source tree.
##

Param([parameter(Mandatory=$true)][String]$arch)

$ErrorActionPreference = "Stop"

#-----------------------------------------------------------------------
# Setup
$release_directory = Split-Path -Parent $MyInvocation.MyCommand.Path
$version = (Get-Content "${release_directory}\..\Project\version.txt" -Raw).Trim()
$arch_alt="${arch}"
if ("${arch}" -eq "Win32" ) {
    $arch_alt="i386"
}
$arch_bcb="${arch}"
if ("${arch}" -eq "x64" -Or "${arch}" -eq "ARM64") {
    $arch_bcb="Win64"
}

#-----------------------------------------------------------------------
# Cleanup
$artifact = "${release_directory}\MediaInfo_GUI_${version}_Windows_${arch_alt}"
if (Test-Path "${artifact}") {
    Remove-Item -Force -Recurse "${artifact}"
}

$artifact = "${release_directory}\MediaInfo_GUI_${version}_Windows_${arch_alt}_WithoutInstaller.7z"
if (Test-Path "${artifact}") {
    Remove-Item -Force "${artifact}"
}

$artifact = "${release_directory}\MediaInfo_GUI_${version}_Windows_${arch_alt}.exe"
if (Test-Path "${artifact}") {
    Remove-Item -Force "${artifact}"
}

#-----------------------------------------------------------------------
# Package GUI
Push-Location "${release_directory}"
    New-Item -Force -ItemType Directory -Path "MediaInfo_GUI_${version}_Windows_${arch_alt}"
    Push-Location "MediaInfo_CLI_${version}_Windows_${arch_alt}"
        ### Copying: Exe ###
        Copy-Item -Force "..\..\Project\BCB\GUI\${arch_bcb}\Release\MediaInfo_GUI.exe" MediaInfo.exe
        ### Copying: Dll ###
        if ("${arch}" -eq "ARM64") {
            Copy-Item -Force "..\..\..\MediaInfoLib\Project\MSVC2022\${arch}EC\Release\MediaInfo.dll" .
        }
        else {
            Copy-Item -Force "..\..\..\MediaInfoLib\Project\MSVC2022\${arch}\Release\MediaInfo.dll" .
        }
        Copy-Item -Force "..\..\..\MediaInfoLib\Project\MSVC2022\${arch}\Release\MediaInfo_InfoTip.dll" .
        Copy-Item -Force "$Env:BDS\Redist\${arch_bcb}\WebView2Loader.dll" .
        ### Copying: Plugins ###
        New-Item -Force -ItemType Directory "Plugin"
        Copy-Item -Force -Recurse "..\..\Source\Resource\Plugin\*" "Plugin"
        ### Copying: libCURL ###
        Copy-Item -Force "..\..\..\libcurl\${arch}\Release\LIBCURL.DLL" .
        ### Copying: Information files ###
        Copy-Item -Force "..\..\License.html" .
        Copy-Item -Force "..\..\History_GUI.txt" "History.txt"
        Copy-Item -Force "..\Readme_GUI_Windows.txt" "ReadMe.txt"
        ### Archive
        7za.exe a -r -t7z -mx9 "..\MediaInfo_GUI_${version}_Windows_${arch_alt}_WithoutInstaller.7z" *
    Pop-Location
Pop-Location

#-----------------------------------------------------------------------
# Package installer
Push-Location "${release_directory}"
    makensis.exe "..\Source\Install\MediaInfo_GUI_Windows_${arch_alt}.nsi"
Pop-Location
