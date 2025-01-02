@echo off

:: Set up directories
set SRC_DIR=src
set BIN_DIR=bin
set OUTPUT_EXE=main.exe
set COMPILER_FLAGS=-Wall -Wextra -pedantic -g -std=c11 -Wno-unused-parameter -Wno-unused-variable

:: Create the BIN directory if it doesn't exist
if not exist %BIN_DIR% mkdir %BIN_DIR%

:: Clear the BIN directory of old object files
del /q %BIN_DIR%\*.o > nul 2>&1

:: Compile all .c files in the src directory into .o in BIN_DIR
for %%f in (%SRC_DIR%\*.c) do (
    echo gcc -c %%f %COMPILER_FLAGS% -o %BIN_DIR%\%%~nf.o
    gcc -c %%f %COMPILER_FLAGS% -o %BIN_DIR%\%%~nf.o
    if errorlevel 1 goto :error
)

:: Link all .o files in BIN_DIR to create the executable
cd %BIN_DIR%
echo Linking...
gcc *.o -o %OUTPUT_EXE%
if errorlevel 1 goto :error
cd ..

echo Compilation and linking successful! Executable: %BIN_DIR%\%OUTPUT_EXE%
goto :eof

:error
echo Error occurred during compilation or linking.
exit /b 1
