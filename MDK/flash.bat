@echo off
setlocal
rem flash.bat - program HEX to target via JLink
set "JLINK=C:\Program Files\SEGGER\JLink_V952\JLink.exe"
if defined JLINK_EXE set "JLINK=%JLINK_EXE%"
set "ROOT=%~dp0"
set "SCRIPT=%ROOT%MDK\flash.jlink"
set "HEX=%ROOT%MDK\Objects\slm32m030_750w.hex"
set "LOG=%ROOT%JlinkLog.txt"

if not exist "%HEX%" (
    echo [FLASH] ERROR: %HEX% not found, build first
    exit /b 1
)

echo [FLASH] download %HEX% via JLink
"%JLINK%" -CommandFile "%SCRIPT%" -ExitOnError 1 -Log "%LOG%"
set "RC=%ERRORLEVEL%"
if %RC% NEQ 0 (
    echo [FLASH] FAILED ^(JLink exit %RC%^)
    exit /b 1
)
echo [FLASH] done
exit /b 0
