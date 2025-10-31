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
$arch_bcb="${arch}"
if ("${arch}" -eq "x64" ) {
    $arch_bcb="Win64"
}

#-----------------------------------------------------------------------
# Prepare
Push-Location -Path "${release_directory}\..\..\brotli\Project\$msvc"
    ((Get-Content -Path brotlicommon.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path brotlicommon.vcxproj
    ((Get-Content -Path brotlidec.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path brotlidec.vcxproj
Pop-Location

Push-Location -Path "${release_directory}\..\..\zlib\contrib\vstudio\vc17"
    ((Get-Content -Path zlibstat.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path zlibstat.vcxproj
Pop-Location

Push-Location -Path "${release_directory}\..\..\ZenLib\Project\MSVC2022\Library"
    ((Get-Content -Path ZenLib.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path ZenLib.vcxproj
Pop-Location

Push-Location -Path "${release_directory}\..\..\MediaInfoLib\Project\MSVC2022"
    ((Get-Content -Path Library\MediaInfoLib.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path Library\MediaInfoLib.vcxproj
    ((Get-Content -Path Dll\MediaInfoDll.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path Dll\MediaInfoDll.vcxproj
    ((Get-Content -Path Example\HowToUse_Dll.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path Example\HowToUse_Dll.vcxproj
    ((Get-Content -Path ShellExtension\MediaInfoShellExt.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path ShellExtension\MediaInfoShellExt.vcxproj
    ((Get-Content -Path FieldsDescription\FieldsDescription.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path FieldsDescription\FieldsDescription.vcxproj
    ((Get-Content -Path RegressionTest\RegressionTest.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path RegressionTest\RegressionTest.vcxproj
    ((Get-Content -Path PreRelease\PreRelease.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path PreRelease\PreRelease.vcxproj
Pop-Location


#-----------------------------------------------------------------------
# Build
### Build: MediaInfo DLL ###
Push-Location -Path "${release_directory}\..\..\MediaInfoLib\Project\MSVC2022"
    MSBuild "/p:Configuration=Release;Platform=${arch}" MediaInfoLib.sln
    if ("${arch}" -eq "x64") { # Also build native ARM MediaInfo.dll
        MSBuild "/p:Configuration=Release;Platform=ARM64" MediaInfoLib.sln
        MSBuild "/p:Configuration=Release;Platform=ARM64EC" MediaInfoLib.sln
    }
Pop-Location

### Build: Windows 11 menu helper ###
Push-Location -Path "${release_directory}\..\Project\MSVC2022"
    makeappx pack /d "..\..\Source\WindowsSparsePackage\MSIX" /p "${arch}\Release\MediaInfo_SparsePackage.msix" /nv
Pop-Location

Push-Location -Path "${release_directory}\..\Project\MSVC2022"
    MSBuild /restore "/p:RestorePackagesConfig=true;Configuration=Release;Platform=${arch}" "/t:MediaInfo_WindowsShellExtension" MediaInfo.sln
    if ("${arch}" -eq "x64") { # Also build native ARM MediaInfo_WindowsShellExtension.dll
        MSBuild "/p:RestorePackagesConfig=true;Configuration=Release;Platform=ARM64" "/t:MediaInfo_WindowsShellExtension" MediaInfo.sln
    }
    MSBuild /restore "/p:RestorePackagesConfig=true;Configuration=Release;Platform=Win32" "/t:MediaInfo_PackageHelper" MediaInfo.sln
Pop-Location

### Build: zlib BCB library ###
Push-Location -Path "${release_directory}\..\..\zlib\contrib\BCB"
    MSBuild "/p:Configuration=Release;Platform=${arch_bcb}" zlib.cbproj
Pop-Location

### Build: Zenlib BCB library ###
Push-Location -Path "${release_directory}\..\..\ZenLib\Project\BCB\Library"
    MSBuild "/p:Configuration=Release;Platform=${arch_bcb}" ZenLib.cbproj
Pop-Location

### Build: GUI ###
Push-Location -Path "${release_directory}\..\Project\BCB\GUI"
    MSBuild "/p:Configuration=Release;Platform=${arch_bcb}" MediaInfo_GUI.cbproj
Pop-Location
