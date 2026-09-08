# makefile: build (Keil uVision / UV4) and flash (JLink) via helper batch files
#
#   make all       incremental build + JLink flash
#   make axf       incremental build (UV4 -b), Keil decides what to rebuild
#   make axf-all   full rebuild   (UV4 -r), rebuild everything
#   make flash     incremental build (if needed) + JLink flash
#   make clean     remove Objects/Listings temp files
#
# All compiler/linker output (Keil Build Output) and JLink output are printed
# directly to the terminal / VSCode output panel.
#
# Override tool paths if the defaults do not match your install:
#   make flash UV4_DIR=D:/Keil_v5/UV4
#   make flash JLINK_EXE="D:/SEGGER/JLink.exe"

SHELL := cmd.exe

export UV4_DIR  ?= C:/Keil_v5/UV4
export JLINK_EXE ?= C:/Program Files/SEGGER/JLink_V952/JLink.exe

.PHONY: all axf axf-all flash clean

all: flash

axf:
	@call MDK\build.bat b

axf-all:
	@call MDK\build.bat r

flash: axf
	@call MDK\flash.bat

clean:
	@call MDK\clean.bat
