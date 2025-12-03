@echo off
echo ========================================
echo   INSTALL AAX TO PRO TOOLS (LOCAL)
echo ========================================
echo.

set SOURCE="..\Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin"
set DEST="C:\Program Files\Common Files\Avid\Audio\Plug-Ins\AuricHalo.aaxplugin"

if not exist %SOURCE% (
    echo ERROR: AAX plugin not found!
    echo Expected: %SOURCE%
    echo.
    echo Please build Release x64 first.
    pause
    exit /b 1
)

echo Source: %SOURCE%
echo Destination: %DEST%
echo.

REM Check if Pro Tools plugin folder exists
if not exist "C:\Program Files\Common Files\Avid\Audio\Plug-Ins\" (
    echo ERROR: Pro Tools plugin folder not found!
    echo Is Pro Tools installed?
    pause
    exit /b 1
)

echo Copying AAX plugin...
xcopy /E /I /Y %SOURCE% %DEST%

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Failed to copy plugin!
    echo Make sure you run this as Administrator.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   INSTALLATION SUCCESSFUL!
echo ========================================
echo.
echo Plugin installed to:
echo %DEST%
echo.
echo Next steps:
echo 1. Launch Pro Tools
echo 2. Go to: Setup → Plug-ins
echo 3. Click "Rescan"
echo 4. Find "Auric Halo" in your plugin list
echo.
pause
