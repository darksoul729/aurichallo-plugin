@echo off
echo ========================================
echo   AURIC HALO - WINDOWS RELEASE BUILD
echo ========================================
echo.

REM Check if Visual Studio is available
where msbuild >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: MSBuild not found!
    echo Please run this from Visual Studio Developer Command Prompt
    echo Or: Open Visual Studio and build manually
    pause
    exit /b 1
)

echo Step 1: Cleaning previous build...
msbuild "..\Builds\VisualStudio2022\AuricHalo.sln" /t:Clean /p:Configuration=Release /p:Platform=x64

echo.
echo Step 2: Building Release configuration...
msbuild "..\Builds\VisualStudio2022\AuricHalo.sln" /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo   BUILD SUCCESSFUL!
echo ========================================
echo.
echo Output locations:
echo VST3: Builds\VisualStudio2022\x64\Release\VST3\AuricHalo.vst3
echo Standalone: Builds\VisualStudio2022\x64\Release\Standalone Plugin\AuricHalo.exe
echo.
echo Next steps:
echo 1. Test the Release build
echo 2. Create installer (run build_installer_windows.bat)
echo 3. Distribute!
echo.
pause
