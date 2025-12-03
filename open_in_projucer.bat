@echo off
echo Membuka Auric Halo di Projucer...
echo.

REM Coba beberapa lokasi umum Projucer
set PROJUCER_PATHS=^
"C:\JUCE\Projucer.exe" ^
"C:\Program Files\JUCE\Projucer.exe" ^
"%USERPROFILE%\JUCE\Projucer.exe" ^
"..\..\..\JUCE\Projucer.exe"

for %%P in (%PROJUCER_PATHS%) do (
    if exist %%P (
        echo Menemukan Projucer di: %%P
        start "" %%P "%~dp0AuricHalo.jucer"
        echo.
        echo Project dibuka di Projucer!
        echo Langkah selanjutnya:
        echo 1. Klik "Save Project and Open in IDE"
        echo 2. Build di Visual Studio
        pause
        exit /b 0
    )
)

echo.
echo ERROR: Projucer tidak ditemukan!
echo.
echo Silakan buka Projucer secara manual, lalu:
echo 1. File -^> Open
echo 2. Pilih: AuricHalo.jucer
echo 3. Klik "Save Project and Open in IDE"
echo.
pause
