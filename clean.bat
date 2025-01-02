@echo off

:: Set the bin directory
set BIN_DIR=bin

:: Check if the bin directory exists
if exist %BIN_DIR% (
    echo Deleting *.o and *.exe files in %BIN_DIR%...
    del /q %BIN_DIR%\*.o > nul 2>&1
    del /q %BIN_DIR%\*.exe > nul 2>&1

    echo Removing %BIN_DIR% directory...
    rmdir /s /q %BIN_DIR%
    echo Clean-up completed.
) else (
    echo %BIN_DIR% does not exist. Nothing to clean.
)

exit /b
