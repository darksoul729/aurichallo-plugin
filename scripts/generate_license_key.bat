@echo off
echo ========================================
echo   AURIC HALO - LICENSE KEY GENERATOR
echo ========================================
echo.

setlocal enabledelayedexpansion

REM Generate random license key
set "chars=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
set "key=AURIC-"

REM Generate 16 random characters
for /L %%i in (1,1,16) do (
    set /a "rand=!random! %% 36"
    for %%j in (!rand!) do set "key=!key!!chars:~%%j,1!"
    
    REM Add dash every 4 characters for readability
    if %%i==4 set "key=!key!-"
    if %%i==8 set "key=!key!-"
    if %%i==12 set "key=!key!-"
)

echo Generated License Key:
echo.
echo !key!
echo.
echo ========================================
echo.
echo Copy this key and give it to your client.
echo Client can activate plugin with this key.
echo.
echo Key format: AURIC-XXXX-XXXX-XXXX-XXXX
echo.
pause
