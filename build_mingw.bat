@echo off
echo Building ThemeSwitch with MinGW...

REM Compile resource file
windres resource.rc -O coff -o resource.o
if errorlevel 1 (
    echo Failed to compile resources
    exit /b 1
)

REM Compile and link all source files
g++ -O2 -mwindows main.cpp TrayIcon.cpp ThemeToggle.cpp Settings.cpp resource.o -luser32 -lshell32 -ladvapi32 -lcomctl32 -o ThemeSwitch.exe
if errorlevel 1 (
    echo Build failed
    exit /b 1
)

echo Build successful!
echo Output: ThemeSwitch.exe