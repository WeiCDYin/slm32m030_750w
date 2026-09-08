@echo off
setlocal
rem clean.bat - remove Keil build output (Objects/Listings) and temp logs
set "MDK=%~dp0"
set "ROOT=%~dp0.."

echo [CLEAN] removing Objects/Listings
if exist "%MDK%Objects"  rmdir /s /q "%MDK%Objects"
if exist "%MDK%Listings" rmdir /s /q "%MDK%Listings"
if exist "%MDK%build.log" del /q "%MDK%build.log"
if exist "%ROOT%\JlinkLog.txt" del /q "%ROOT%\JlinkLog.txt"
echo [CLEAN] done
exit /b 0
