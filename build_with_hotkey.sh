#!/bin/bash

echo "Building with KDE hotkey support..."

cd src

# Compile with KGlobalAccel support
g++ -O2 -std=c++17 \
    main.cpp \
    Application.cpp \
    platform/linux/LinuxThemeController.cpp \
    platform/linux/LinuxTrayIcon.cpp \
    platform/linux/LinuxSettings.cpp \
    platform/linux/LinuxHotkeyManager.cpp \
    platform/linux/SettingsDialog.cpp \
    -I. \
    -I/usr/include/qt6 \
    -I/usr/include/qt6/QtCore \
    -I/usr/include/qt6/QtWidgets \
    -I/usr/include/qt6/QtGui \
    -I/usr/include/qt6/QtDBus \
    -I/usr/include/KF6 \
    -I/usr/include/KF6/KGlobalAccel \
    -lQt6Core \
    -lQt6Widgets \
    -lQt6Gui \
    -lQt6DBus \
    -lKF6GlobalAccel \
    -fPIC \
    -o ../ThemeSwitch

if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./ThemeSwitch"
else
    echo "Build failed."
fi