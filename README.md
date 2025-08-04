# ThemeSwitch

A lightweight Windows 11 system tray application for toggling between dark and light themes.

## Features
- Single-click toggle between dark and light mode (system + apps)
- Customizable hotkey (default: Win+Shift+D)
- Start with Windows option
- Dynamic tray icon that changes with theme
- Native Win32 implementation (~50KB executable)

## Building

### Prerequisites
- Visual Studio 2022 Build Tools or Visual Studio 2022
- Windows SDK

### Build Instructions
1. Open Developer Command Prompt for VS 2022
2. Navigate to project directory
3. Run: `build.bat`

### Manual Build
```cmd
rc.exe /fo resource.res resource.rc
cl.exe /EHsc /W3 /O2 main.cpp TrayIcon.cpp ThemeToggle.cpp Settings.cpp resource.res user32.lib shell32.lib advapi32.lib comctl32.lib /Fe:ThemeSwitch.exe
```

## Usage
- **Left-click tray icon**: Toggle theme
- **Right-click tray icon**: Open menu
- **Win+Shift+D**: Toggle theme (customizable)

## Note
You'll need to provide `light.ico` and `dark.ico` icon files (16x16 or 32x32) for the tray icons.