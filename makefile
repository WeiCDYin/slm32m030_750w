# makefile: build (Keil uVision / UV4) and flash (SEGGER JLink Commander) via
# helper batch files
#
#   make all         incremental build + JLink flash
#   make build       incremental build (UV4 -b), Keil decides what to rebuild
#   make build-all   full rebuild   (UV4 -r), rebuild everything
#   make flash       incremental build (if needed) + JLink flash
#   make flash-keil  flash via Keil UV4 -f (Flash Download, builds if needed)
#   make clean       remove Objects/Listings temp files
#
# Build output (Keil) and the live JLink flash progress ("Programming Flash
# ... xx%") are printed directly to the terminal / VSCode output panel.
#
# Override tool paths if the defaults do not match your install:
#   make flash UV4_DIR=D:/Keil_v5/UV4
#   make flash JLINK_EXE="D:/SEGGER/JLink.exe"

SHELL := cmd.exe

export UV4_DIR  ?= C:/Keil_v5/UV4
export JLINK_EXE ?= C:/Program Files/SEGGER/JLink_V952/JLink.exe

.PHONY: all build build-all flash flash-keil clean push

all: flash

build:
	@call MDK\build.bat b

build-all:
	@call MDK\build.bat r

flash:
	@call MDK\flash.bat

flash-keil:
	@call MDK\flash-keil.bat

clean:
	@call MDK\clean.bat

push:
	git push origin main
	git push new-origin main
