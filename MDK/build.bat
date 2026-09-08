@echo off
setlocal
rem MDK\build.bat <b|r>
rem   b = incremental build (UV4 -b)
rem   r = full rebuild    (UV4 -r)
rem Prints the Keil Build Output to stdout and returns 0 on success, 1 on error.

set "UV4=C:\Keil_v5\UV4\UV4.exe"
if defined UV4_DIR set "UV4=%UV4_DIR%\UV4.exe"
set "PROJ=%~dp0slm32m030_750w.uvprojx"
set "LOG=%~dp0build.log"
set "MODE=%~1"

if exist "%LOG%" del /q "%LOG%"

if /i "%MODE%"=="r" (
    echo [BUILD] full rebuild via UV4 -r
    "%UV4%" -r "%PROJ%" -j0 -o "%LOG%"
) else (
    echo [BUILD] incremental build via UV4 -b
    "%UV4%" -b "%PROJ%" -j0 -o "%LOG%"
)
set "RC=%ERRORLEVEL%"

if %RC% GEQ 2 goto :failed
if %RC% EQU 0 goto :uptodate
goto :built

:built
    echo.
    type "%LOG%"
    del /q "%LOG%"
    echo.
    echo [BUILD] completed with warnings
    exit /b 0

:uptodate
    if exist "%LOG%" (
        echo.
        type "%LOG%"
        del /q "%LOG%"
        echo.
        echo [BUILD] ok
    ) else (
        echo [BUILD] up to date
    )
    exit /b 0

:failed
    echo.
    if exist "%LOG%" (
        type "%LOG%"
        del /q "%LOG%"
    ) else (
        echo [BUILD] UV4 failed with code %RC% and produced no log
    )
    echo.
    echo [BUILD] FAILED
    exit /b 1
