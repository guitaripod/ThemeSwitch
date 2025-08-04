# ThemeSwitch

Windows system tray app for toggling dark/light mode with a single click.

## Features
- Single-click dark/light mode toggle
- Win+Shift+D hotkey
- Start with Windows option
- Pine rose themed settings
- ~90KB standalone executable

## Build

### MinGW
```bash
g++ -DUNICODE -D_UNICODE -O2 -mwindows main.cpp TrayIcon.cpp ThemeToggle.cpp Settings.cpp -luser32 -lshell32 -ladvapi32 -lcomctl32 -lgdi32 -o ThemeSwitch.exe
```

### Visual Studio
```cmd
build.bat
```

## Usage
- **Left-click**: Toggle theme
- **Right-click**: Settings/Exit
- **Win+Shift+D**: Toggle theme

## Notes
- Add `dark.ico` and `light.ico` (16x16) for custom tray icons
- Works on Windows 10/11