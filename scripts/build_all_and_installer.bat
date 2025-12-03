@echo off
echo ========================================
echo   AURIC HALO - BUILD ALL + INSTALLER
echo   (VST3 + AAX 64-bit + Standalone)
echo ========================================
echo.

REM Check if Visual Studio solution exists
if not exist "..\Builds\VisualStudio2022\AuricHalo.sln" (
    echo ERROR: Visual Studio solution tidak ditemukan!
    echo.
    echo Silakan generate dulu dengan Projucer:
    echo 1. Buka AuricHalo.jucer di Projucer
    echo 2. Pastikan AAX enabled
    echo 3. Pastikan platform x64
    echo 4. Save Project (Ctrl+Shift+S)
    echo.
    pause
    exit /b 1
)

echo [1/4] Building Release x64...
echo.

REM Try to find MSBuild
set MSBUILD=""
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
    set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
)

if %MSBUILD%=="" (
    echo WARNING: MSBuild tidak ditemukan otomatis.
    echo Silakan build manual di Visual Studio:
    echo 1. Buka Builds\VisualStudio2022\AuricHalo.sln
    echo 2. Configuration: Release
    echo 3. Platform: x64
    echo 4. Build → Build Solution
    echo.
    echo Setelah build selesai, tekan tombol apapun untuk lanjut ke installer...
    pause
    goto :create_installer
)

echo Building dengan MSBuild...
%MSBUILD% "..\Builds\VisualStudio2022\AuricHalo.sln" /p:Configuration=Release /p:Platform=x64 /m

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build gagal!
    echo Coba build manual di Visual Studio.
    pause
    exit /b 1
)

echo.
echo ✓ Build selesai!
echo.

:create_installer
echo [2/4] Checking build results...
echo.

if exist "..\Builds\VisualStudio2022\x64\Release\VST3\AuricHalo.vst3" (
    echo   ✓ VST3 (64-bit)
) else (
    echo   ✗ VST3 MISSING!
)

if exist "..\Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin" (
    echo   ✓ AAX (64-bit)
) else (
    echo   ⚠ AAX not built
)

if exist "..\Builds\VisualStudio2022\x64\Release\Standalone Plugin\AuricHalo.exe" (
    echo   ✓ Standalone (64-bit)
) else (
    echo   ⚠ Standalone not built
)

echo.
echo [3/4] Creating installer...
echo.

call build_installer_windows.bat

echo.
echo [4/4] Done!
echo.
echo ========================================
echo   SELESAI!
echo ========================================
echo.
echo Installer siap di folder: Release\
echo File: AuricHalo-1.0.0-Win64-Setup.exe
echo.
echo Installer ini sudah:
echo ✓ 64-bit only
echo ✓ Otomatis install VST3
echo ✓ Otomatis install AAX ke Pro Tools
echo ✓ Otomatis install Standalone
echo ✓ Cek Windows 64-bit
echo ✓ Pesan dalam Bahasa Indonesia
echo.
echo Kirim file installer ini ke client!
echo.
pause
