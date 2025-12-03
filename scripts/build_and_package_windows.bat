@echo off
echo ========================================
echo   AURIC HALO - COMPLETE WINDOWS BUILD
echo ========================================
echo.
echo This will:
echo 1. Build Release version
echo 2. Create installer
echo 3. Package everything
echo.
pause

REM Step 1: Build Release
echo.
echo ========================================
echo   STEP 1: BUILDING RELEASE
echo ========================================
call build_release_windows.bat
if %ERRORLEVEL% NEQ 0 (
    echo Build failed, stopping...
    exit /b 1
)

REM Step 2: Create Installer
echo.
echo ========================================
echo   STEP 2: CREATING INSTALLER
echo ========================================
call build_installer_windows.bat
if %ERRORLEVEL% NEQ 0 (
    echo Installer creation failed, stopping...
    exit /b 1
)

REM Step 3: Package
echo.
echo ========================================
echo   STEP 3: PACKAGING
echo ========================================

REM Create Release folder if not exists
if not exist "..\Release" mkdir "..\Release"

REM Copy files
echo Copying files...
copy "..\README.txt" "..\Release\" >nul
copy "..\LICENSE.txt" "..\Release\" >nul
copy "..\CHANGELOG.txt" "..\Release\" >nul

REM Create portable version (optional)
echo Creating portable version...
if not exist "..\Release\Portable" mkdir "..\Release\Portable"
xcopy "..\Builds\VisualStudio2022\x64\Release\Standalone Plugin\*" "..\Release\Portable\" /E /I /Y >nul

echo.
echo ========================================
echo   COMPLETE! READY FOR DISTRIBUTION
echo ========================================
echo.
echo Release folder contents:
dir /b "..\Release"
echo.
echo Files ready for distribution:
echo - AuricHalo-1.0.0-Setup.exe (Installer)
echo - Portable folder (Portable version)
echo - Documentation files
echo.
echo Next steps:
echo 1. Test everything thoroughly
echo 2. Upload to your website
echo 3. Announce release!
echo.
pause
