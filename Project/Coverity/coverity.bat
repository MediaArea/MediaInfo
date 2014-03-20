@ECHO OFF
REM Script taken from MPC-HC, https://github.com/mpc-hc/mpc-hc

SETLOCAL

PUSHD %~dp0

SET "COVDIR=H:\progs\thirdparty\cov-analysis-win64-7.0.2"
IF NOT EXIST "%COVDIR%\bin\cov-build.exe" (ECHO. & ECHO ERROR: Coverity not found & GOTO End)

CALL "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x86

SET MSBUILD_SWITCHES=/nologo /consoleloggerparameters:Verbosity=minimal /maxcpucount^
 /nodeReuse:true /target:MediaInfoLib:Rebuild /property:Configuration="Release";Platform=Win32

"%COVDIR%\bin\cov-build.exe" --dir cov-int MSBuild "..\MSVC2013\MediaInfoLib.sln" %MSBUILD_SWITCHES%

IF EXIST "MediaInfoLib.tar" DEL "MediaInfoLib.tar"
IF EXIST "MediaInfoLib.tgz" DEL "MediaInfoLib.tgz"


:tar
tar --version 1>&2 2>NUL || (ECHO. & ECHO ERROR: tar not found & GOTO SevenZip)
tar czvf "MediaInfoLib.tgz" "cov-int"
GOTO End


:SevenZip
IF NOT EXIST "%PROGRAMFILES%\7-Zip\7z.exe" (
  ECHO.
  ECHO ERROR: "%PROGRAMFILES%\7-Zip\7z.exe" not found
  GOTO End
)
"%PROGRAMFILES%\7-Zip\7z.exe" a -ttar "MediaInfoLib.tar" "cov-int"
"%PROGRAMFILES%\7-Zip\7z.exe" a -tgzip "MediaInfoLib.tgz" "MediaInfoLib.tar"
IF EXIST "MediaInfoLib.tar" DEL "MediaInfoLib.tar"


:End
POPD
ECHO. & ECHO Press any key to close this window...
PAUSE >NUL
ENDLOCAL
EXIT /B
