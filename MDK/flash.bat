@echo off
setlocal
rem flash.bat - program HEX to target via SEGGER JLink Commander.
rem JLink prints the live "Programming Flash ([..] xx%)" progress to the
rem console; the command script is MDK\flash.jlink.
rem
rem Override the executable from the environment / make:
rem   set JLINK_EXE=D:\path\JLink.exe

set "JLINK=C:\Program Files\SEGGER\JLink_V952\JLink.exe"
if defined JLINK_EXE set "JLINK=%JLINK_EXE%"
set "ROOT=%~dp0"
set "PROJROOT=%ROOT%.."
set "SCRIPT=%ROOT%flash.jlink"
set "HEX=%ROOT%Objects\slm32m030_750w.hex"
set "LOG=%ROOT%JlinkLog.txt"

if not exist "%HEX%" (
    echo [FLASH] ERROR: %HEX% not found, build first
    exit /b 1
    )

echo [FLASH] download %HEX% via JLink
rem JLink command-script paths are root-relative (MDK/Objects/...), run at root.
pushd "%PROJROOT%"
"%JLINK%" -CommandFile "%SCRIPT%" -ExitOnError 1 -Log "%LOG%"
set "RC=%ERRORLEVEL%"
popd

if %RC% NEQ 0 (
    echo !!!!!!!!!!!!!!!!!!! [FLASH] FAILED ^(JLink exit %RC%^) !!!!!!!!!!!!!!!!!!!!!!
    exit /b 1
)
echo ********************* [FLASH] done OKOKOKOKOKOKOK ********************
exit /b 0
