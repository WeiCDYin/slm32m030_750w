@echo off
setlocal
rem MDK\flash-keil.bat - program the target via Keil UV4 -f (Flash Download).
rem UV4 -f builds if needed then downloads to flash; build/flash output is
rem printed to the console via the log file.
rem
rem Override the UV4 directory from the environment / make:
rem   set UV4_DIR=D:\Keil_v5\UV4

set "UV4=C:\Keil_v5\UV4\UV4.exe"
if defined UV4_DIR set "UV4=%UV4_DIR%\UV4.exe"
set "PROJ=%~dp0slm32m030_750w.uvprojx"
set "LOG=%~dp0flash-keil.log"

if exist "%LOG%" del /q "%LOG%"

echo [FLASH-KEIL] UV4 -f download
"%UV4%" -f "%PROJ%" -j0 -o "%LOG%"
set "RC=%ERRORLEVEL%"

if exist "%LOG%" (
    echo.
    type "%LOG%"
    del /q "%LOG%"
)

if %RC% GEQ 2 (
    echo.
    echo !!!!!!!!!!!!!!!!!!! [FLASH-KEIL] FAILED ^(UV4 exit %RC%^) !!!!!!!!!!!!!!!!!!!
    exit /b 1
)
echo ********************* [FLASH] done OKOKOKOKOKOKOK ********************
exit /b 0
