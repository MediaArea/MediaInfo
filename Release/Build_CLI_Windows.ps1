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

switch ($msvc) {
    "MSVC2022" {
        $vc = "vc17"
        $sln = "sln"
        break
    }
    "MSVC2026" {
        $vc = "vc18"
        $sln = "slnx"
        break
    }
    Default { throw "Invalid parameter: '$msvc'." }
}

#-----------------------------------------------------------------------
# Prepare
Push-Location -Path "${release_directory}\..\..\zlib\contrib\vstudio\$vc"
    ((Get-Content -Path zlibstat.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path zlibstat.vcxproj
Pop-Location

Push-Location -Path "${release_directory}\..\..\ZenLib\Project\$msvc\Library"
    ((Get-Content -Path ZenLib.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path ZenLib.vcxproj
Pop-Location

Push-Location -Path "${release_directory}\..\..\MediaInfoLib\Project\$msvc"
    ((Get-Content -Path Library\MediaInfoLib.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path Library\MediaInfoLib.vcxproj
    ((Get-Content -Path Dll\MediaInfoDll.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path Dll\MediaInfoDll.vcxproj
    ((Get-Content -Path Example\HowToUse_Dll.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path Example\HowToUse_Dll.vcxproj
    ((Get-Content -Path ShellExtension\MediaInfoShellExt.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path ShellExtension\MediaInfoShellExt.vcxproj
    ((Get-Content -Path FieldsDescription\FieldsDescription.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path FieldsDescription\FieldsDescription.vcxproj
    ((Get-Content -Path RegressionTest\RegressionTest.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path RegressionTest\RegressionTest.vcxproj
    ((Get-Content -Path PreRelease\PreRelease.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path PreRelease\PreRelease.vcxproj
Pop-Location

Push-Location -Path "${release_directory}\..\Project\$msvc"
    ((Get-Content -Path CLI\MediaInfo-CLI.vcxproj) -Replace 'MultiThreadedDLL','MultiThreaded') | Set-Content -Path CLI\MediaInfo-CLI.vcxproj
Pop-Location

#-----------------------------------------------------------------------
# Build
Push-Location -Path "${release_directory}\..\Project\$msvc"
    MSBuild "/p:Configuration=Release;Platform=${arch}" "/t:MediaInfo-CLI" MediaInfo.$sln
Pop-Location
