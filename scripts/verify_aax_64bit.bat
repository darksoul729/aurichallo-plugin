@echo off
echo ========================================
echo   VERIFY AAX 64-BIT
echo ========================================
echo.

set AAX_PATH="..\Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin\Contents\x64\AuricHalo.aaxplugin"

if not exist %AAX_PATH% (
    echo ERROR: AAX plugin not found!
    echo Expected: %AAX_PATH%
    echo.
    echo Please build Release x64 first.
    pause
    exit /b 1
)

echo Checking AAX plugin architecture...
echo.

REM Check if file is 64-bit using dumpbin (if available)
where dumpbin >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Using dumpbin to verify...
    dumpbin /headers %AAX_PATH% | findstr "machine"
    echo.
) else (
    echo dumpbin not found. Install Visual Studio to use this tool.
    echo.
)

echo File location: %AAX_PATH%
echo.
echo If the output shows "x64" or "8664", the plugin is 64-bit.
echo If it shows "x86" or "14C", the plugin is 32-bit.
echo.

REM Check file size (64-bit plugins are typically larger)
for %%F in (%AAX_PATH%) do echo File size: %%~zF bytes
echo.

echo ========================================
echo   INSTALLATION PATH
echo ========================================
echo.
echo Pro Tools AAX folder (64-bit):
echo C:\Program Files\Common Files\Avid\Audio\Plug-Ins\
echo.
echo To install:
echo 1. Copy entire AuricHalo.aaxplugin folder
echo 2. Paste to Pro Tools AAX folder
echo 3. Rescan plugins in Pro Tools
echo.
pause
