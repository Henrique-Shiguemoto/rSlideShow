:: This script is just so I don't need to type bin\rslideshow.exe all the time, now I just type run and it works.

@echo off

set BIN_DIR=bin
set OUTPUT_EXE=rslideshow.exe

%BIN_DIR%\%OUTPUT_EXE%
if errorlevel 1 goto :error

goto :eof

:error
echo Run Script: Could not find the target executable.