##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
##
##  Use of this source code is governed by a BSD-style license that can
##  be found in the License.html file in the root of the source tree.
##

Param(
    [parameter(Mandatory=$true)][String]$arch,
    [String]$msvc = "MSVC2022"
)

$ErrorActionPreference = "Stop"

#-----------------------------------------------------------------------
# Setup
$release_directory = Split-Path -Parent $MyInvocation.MyCommand.Path
$version = (Get-Content "${release_directory}\..\Project\version.txt" -Raw).Trim()
$arch_alt="${arch}"
if ("${arch}" -eq "Win32" ) {
    $arch_alt="i386"
}

switch ($msvc) {
    "MSVC2022" { break }
    "MSVC2026" { break }
    Default { throw "Invalid parameter: '$msvc'." }
}

#-----------------------------------------------------------------------
# Cleanup
$artifact = "${release_directory}\MediaInfo_CLI_${version}_Windows_${arch_alt}"
if (Test-Path "${artifact}") {
    Remove-Item -Force -Recurse "${artifact}"
}

$artifact = "${release_directory}\MediaInfo_CLI_${version}_Windows_${arch_alt}.zip"
if (Test-Path "${artifact}") {
    Remove-Item -Force "${artifact}"
}

#-----------------------------------------------------------------------
# Package CLI
Push-Location "${release_directory}"
    New-Item -Force -ItemType Directory -Path "MediaInfo_CLI_${version}_Windows_${arch_alt}"
    Push-Location "MediaInfo_CLI_${version}_Windows_${arch_alt}"
        ### Copying: Exe ###
        Copy-Item -Force "..\..\Project\$msvc\${arch}\Release\MediaInfo.exe" .
        ### Copying: Plugins ###
        New-Item -Force -ItemType Directory "Plugin\Custom"
        Copy-Item -Force "..\..\Source\Resource\Plugin\Custom\*" "Plugin\Custom"
        ### Copying: libCURL ###
        Copy-Item -Force "..\..\..\libcurl\${arch}\Release\LIBCURL.DLL" .
        ### Copying: Information files ###
        Copy-Item -Force "..\..\License.html" .
        Copy-Item -Force "..\..\LICENSE" .
        Copy-Item -Force "..\..\History_CLI.txt" "History.txt"
        Copy-Item -Force "..\Readme_CLI_Windows.txt" "ReadMe.txt"
        ### Copying: Contrib files ###
        New-Item -Force -ItemType Directory "Contrib"
        Copy-Item -Force "..\..\Contrib\CLI_Help.doc" "Contrib"
        ### Archive
        7za.exe a -r -tzip -mx9 "..\MediaInfo_CLI_${version}_Windows_${arch_alt}.zip" *
    Pop-Location
Pop-Location
