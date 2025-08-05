# ThemeSwitch

Cross-platform system tray application for toggling dark/light mode with a single click.

## Features
- Single-click dark/light mode toggle
- Global hotkey (Win+Shift+D by default)
- Start with system option
- Minimal resource usage (~90KB on Windows, ~2MB on Linux)
- Native platform integration

## Supported Platforms
- **Windows**: Windows 10/11
- **Linux**: KDE Plasma 6 (Arch Linux tested)

## Project Structure
```
ThemeSwitch/
├── src/
│   ├── common/           # Platform-agnostic interfaces
│   ├── platform/          
│   │   ├── windows/      # Windows-specific implementation
│   │   └── linux/        # Linux/KDE implementation
│   ├── Application.cpp   # Main application logic
│   └── main.cpp          # Entry point
├── resources/
│   ├── windows/          # Windows resources (icons, rc files)
│   └── linux/            # Linux resources (SVG icons, desktop file)
└── CMakeLists.txt        # Cross-platform build configuration
```

## Build Instructions

### Prerequisites

#### Windows
- Visual Studio 2019+ or MinGW-w64
- CMake 3.16+

#### Linux (KDE Plasma)
- CMake 3.16+
- Qt6 (6.2+)
- KDE Frameworks 6
- GCC/Clang with C++17 support

Install dependencies on Arch Linux:
```bash
sudo pacman -S cmake qt6-base kstatusnotifieritem kglobalaccel
```

### Building

#### Using CMake (Recommended)
```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build . --config Release

# The executable will be in build/bin/
```

#### Windows (Direct Compilation)
```bash
cd src
g++ -DUNICODE -D_UNICODE -O2 -mwindows main.cpp Application.cpp ^
    platform/windows/*.cpp ^
    ../resources/windows/resource.rc ^
    -I. -luser32 -lshell32 -ladvapi32 -lcomctl32 -lgdi32 ^
    -o ../ThemeSwitch.exe
```

#### Linux (Direct Compilation)
```bash
cd src
g++ -O2 -std=c++17 main.cpp Application.cpp platform/linux/*.cpp \
    -I. -I/usr/include/qt6 -I/usr/include/qt6/QtCore \
    -I/usr/include/qt6/QtWidgets -I/usr/include/qt6/QtGui \
    -I/usr/include/qt6/QtDBus -I/usr/include/KF6/KGlobalAccel \
    -lQt6Core -lQt6Widgets -lQt6Gui -lQt6DBus -lKF6GlobalAccel \
    -fPIC -o ../ThemeSwitch
```

## Installation

### Windows
1. Build the executable
2. Place `dark.ico` and `light.ico` (16x16) in the same directory as the executable
3. Run `ThemeSwitch.exe`

### Linux
```bash
# Build the project
mkdir build && cd build
cmake .. && make

# Install system-wide (optional)
sudo make install

# Or run directly
./bin/ThemeSwitch
```

## Usage
- **Left-click tray icon**: Toggle theme
- **Right-click tray icon**: Settings/Exit menu
- **Win+Shift+D** (default): Toggle theme via hotkey

## Configuration

### Windows
Settings are stored in the Windows Registry:
`HKEY_CURRENT_USER\SOFTWARE\ThemeSwitch`

### Linux
Settings are stored in JSON format:
`~/.config/ThemeSwitch/settings.json`

## Architecture

The application uses a platform abstraction layer with interfaces for:
- **IThemeController**: Theme detection and switching
- **ITrayIcon**: System tray integration
- **ISettings**: Configuration management
- **IHotkeyManager**: Global hotkey handling

Each platform implements these interfaces using native APIs:
- Windows: Win32 API, Registry
- Linux/KDE: Qt6, KDE Frameworks, plasma-apply-colorscheme

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
MIT