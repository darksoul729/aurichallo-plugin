@echo off
echo =====================================
echo AURIC HALO - PLUGIN TESTER
echo =====================================
echo.

REM Check VST3
if exist "Builds\VisualStudio2022\x64\Debug\VST3\AuricHalo.vst3" (
    echo [OK] VST3 Plugin ditemukan!
    echo      Location: Builds\VisualStudio2022\x64\Debug\VST3\AuricHalo.vst3
) else (
    echo [X] VST3 Plugin tidak ditemukan
)

echo.

REM Check Standalone
if exist "Builds\VisualStudio2022\x64\Debug\Standalone Plugin\AuricHalo.exe" (
    echo [OK] Standalone Plugin ditemukan!
    echo      Location: Builds\VisualStudio2022\x64\Debug\Standalone Plugin\AuricHalo.exe
    echo.
    echo Mau test Standalone sekarang? (Y/N)
    choice /C YN /N
    if errorlevel 2 goto :skip
    if errorlevel 1 goto :run
) else (
    echo [X] Standalone Plugin tidak ditemukan
    goto :end
)

:run
echo.
echo Menjalankan Auric Halo Standalone...
echo.
start "" "Builds\VisualStudio2022\x64\Debug\Standalone Plugin\AuricHalo.exe"
echo Plugin dijalankan!
echo.
echo TEST CHECKLIST:
echo [ ] UI muncul dengan background hitam
echo [ ] Title "AURIC HALO" terlihat (gold)
echo [ ] 4 knobs terlihat (Tone, Drive, Mix, Output)
echo [ ] Halo Engine button terlihat (kiri bawah)
echo [ ] Toggle Halo Engine ON - glow effect muncul
echo [ ] Putar knob Drive - glow intensity berubah
echo [ ] Semua knob bisa diputar
echo.
goto :end

:skip
echo.
echo Test dibatalkan.
echo.

:end
echo =====================================
echo.
echo COPY VST3 KE DAW:
echo copy "Builds\VisualStudio2022\x64\Debug\VST3\AuricHalo.vst3" "C:\Program Files\Common Files\VST3\"
echo.
pause
