#!/bin/bash

echo "Building simplified Linux version..."

cd src

# Compile with minimal dependencies
g++ -O2 -std=c++17 \
    main.cpp \
    Application.cpp \
    platform/linux/LinuxThemeController.cpp \
    platform/linux/LinuxTrayIcon.cpp \
    platform/linux/LinuxSettings.cpp \
    platform/linux/SettingsDialog.cpp \
    -I. \
    -I/usr/include/qt6 \
    -I/usr/include/qt6/QtCore \
    -I/usr/include/qt6/QtWidgets \
    -I/usr/include/qt6/QtGui \
    -lQt6Core \
    -lQt6Widgets \
    -lQt6Gui \
    -fPIC \
    -o ../ThemeSwitch

if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./ThemeSwitch"
else
    echo "Build failed."
fi