@echo off
echo ========================================
echo   AURIC HALO - BUILD INSTALLER 64-BIT
echo ========================================
echo.

REM Get current directory and go to project root
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%.."

REM Check if Inno Setup is installed
set INNO_PATH="C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
if not exist %INNO_PATH% (
    echo ERROR: Inno Setup tidak ditemukan!
    echo.
    echo Download dan install Inno Setup dari:
    echo https://jrsoftware.org/isdl.php
    echo.
    echo Setelah install, jalankan script ini lagi.
    pause
    exit /b 1
)

echo [1/3] Checking build files...
echo.

REM Check VST3 (wajib) - cek folder .vst3
if exist "Builds\VisualStudio2022\x64\Release\VST3\AuricHalo.vst3" (
    echo   ✓ VST3 (64-bit) found
) else (
    echo ERROR: VST3 build tidak ditemukan!
    echo.
    echo Path yang dicari:
    echo %CD%\Builds\VisualStudio2022\x64\Release\VST3\AuricHalo.vst3
    echo.
    echo Silakan build dulu dengan:
    echo 1. Buka Visual Studio
    echo 2. Pilih Configuration: Release
    echo 3. Pilih Platform: x64
    echo 4. Build Solution
    echo.
    pause
    exit /b 1
)

REM Check AAX (optional tapi penting)
if exist "Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin" (
    echo   ✓ AAX (64-bit) found
) else (
    echo   ⚠ AAX not found - installer akan skip AAX
)

REM Check Standalone (optional)
if exist "Builds\VisualStudio2022\x64\Release\Standalone Plugin\AuricHalo.exe" (
    echo   ✓ Standalone (64-bit) found
) else (
    echo   ⚠ Standalone not found - installer akan skip Standalone
)

echo.
echo [2/3] Creating installer...
echo.

REM Compile Inno Setup script
%INNO_PATH% "installers\windows\AuricHalo_Setup.iss"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Gagal membuat installer!
    pause
    exit /b 1
)

echo.
echo [3/3] Verifying installer...
echo.

REM Find installer file
for %%F in ("Release\AuricHalo-*-Win64-Setup.exe") do (
    echo ========================================
    echo   INSTALLER BERHASIL DIBUAT!
    echo ========================================
    echo.
    echo File: %%~nxF
    echo Size: %%~zF bytes
    echo Path: %%~fF
    echo.
    echo ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    echo INSTALLER INI SUDAH 64-BIT!
    echo ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    echo.
    echo Fitur installer:
    echo • Otomatis install VST3 ke folder standard
    echo • Otomatis install AAX ke Pro Tools
    echo • Otomatis install Standalone
    echo • Cek Windows 64-bit
    echo • Pesan dalam Bahasa Indonesia
    echo.
    echo Langkah selanjutnya:
    echo 1. Test installer di komputer kamu
    echo 2. Test di komputer client
    echo 3. Kirim ke client!
    echo.
)

pause
