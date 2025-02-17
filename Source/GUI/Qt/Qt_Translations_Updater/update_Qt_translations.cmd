@echo off
setlocal

REM Enable ANSI escape sequences
setlocal enableextensions
setlocal enabledelayedexpansion

REM ANSI escape codes for colors
set GREEN=[92m
set YELLOW=[33m
set RED=[91m
set RESET=[0m

REM Set paths
set PROJECT_FILE=%~dp0\..\..\..\..\Project\QMake\GUI\MediaInfoQt.pro
set TS_FILES_FOLDER=%~dp0\..\..\..\Resource\Translations
set CSV_FILE=%~dp0\..\..\..\Resource\Language.csv
set PYTHON_SCRIPT=%~dp0\update_Qt_translations.py

REM Step 1: Run lupdate to update .ts files
echo.
echo !YELLOW!Running lupdate...!RESET!
lupdate -noobsolete %PROJECT_FILE%
if %errorlevel% neq 0 (
    echo.
    echo !RED!lupdate failed!%RESET%
    exit /b %errorlevel%
)
echo.
echo !GREEN!lupdate completed successfully!%RESET%
echo.

REM Step 2: Run the Python script to update .ts files
echo !YELLOW!Updating .ts files with translations...!RESET!
python %PYTHON_SCRIPT% %TS_FILES_FOLDER% %CSV_FILE%
if %errorlevel% neq 0 (
    echo.
    echo !RED!Python script failed!%RESET%
    exit /b %errorlevel%
)
echo.
echo !GREEN!.ts files updated successfully!%RESET%
echo.

REM Step 3: Run lrelease to generate .qm files
echo !YELLOW!Running lrelease...!RESET!
lrelease %PROJECT_FILE%
if %errorlevel% neq 0 (
    echo.
    echo !RED!lrelease failed!%RESET%
    exit /b %errorlevel%
)
echo.
echo !GREEN!lrelease completed successfully!%RESET%
echo.

echo !GREEN!All steps completed successfully!%RESET!
endlocal
