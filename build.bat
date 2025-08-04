@echo off
echo Building ThemeSwitch...

REM Compile resource file
rc.exe /fo resource.res resource.rc
if errorlevel 1 (
    echo Failed to compile resources
    exit /b 1
)

REM Compile and link all source files
cl.exe /EHsc /W3 /O2 main.cpp TrayIcon.cpp ThemeToggle.cpp Settings.cpp resource.res user32.lib shell32.lib advapi32.lib comctl32.lib /Fe:ThemeSwitch.exe
if errorlevel 1 (
    echo Build failed
    exit /b 1
)

echo Build successful!
echo Output: ThemeSwitch.exe