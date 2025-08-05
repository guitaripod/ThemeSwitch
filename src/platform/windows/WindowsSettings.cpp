#include "WindowsSettings.h"

WindowsSettings::WindowsSettings() : startWithSystem(false), changeIconWithTheme(true) {
    hotkeyConfig.useSuper = true;
    hotkeyConfig.useShift = true;
    hotkeyConfig.useCtrl = false;
    hotkeyConfig.useAlt = false;
    hotkeyConfig.key = 'D';
    
    load();
}

bool WindowsSettings::getStartWithSystem() const {
    return startWithSystem;
}

void WindowsSettings::setStartWithSystem(bool enable) {
    startWithSystem = enable;
    setAutoStart(enable);
    save();
}

bool WindowsSettings::getChangeIconWithTheme() const {
    return changeIconWithTheme;
}

void WindowsSettings::setChangeIconWithTheme(bool enable) {
    changeIconWithTheme = enable;
    save();
}

HotkeyConfig WindowsSettings::getHotkey() const {
    return hotkeyConfig;
}

void WindowsSettings::setHotkey(const HotkeyConfig& config) {
    hotkeyConfig = config;
    save();
}

void WindowsSettings::save() {
    HKEY hKey;
    DWORD value;
    
    if (RegCreateKeyEx(HKEY_CURRENT_USER, SETTINGS_KEY, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        value = startWithSystem ? 1 : 0;
        RegSetValueEx(hKey, L"StartWithWindows", 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        
        value = changeIconWithTheme ? 1 : 0;
        RegSetValueEx(hKey, L"ChangeIconWithTheme", 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        
        value = 0;
        if (hotkeyConfig.useSuper) value |= MOD_WIN;
        if (hotkeyConfig.useShift) value |= MOD_SHIFT;
        if (hotkeyConfig.useCtrl) value |= MOD_CONTROL;
        if (hotkeyConfig.useAlt) value |= MOD_ALT;
        RegSetValueEx(hKey, L"HotkeyModifiers", 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        
        value = hotkeyConfig.key;
        RegSetValueEx(hKey, L"HotkeyVirtualKey", 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        
        RegCloseKey(hKey);
    }
}

void WindowsSettings::load() {
    HKEY hKey;
    DWORD dataSize = sizeof(DWORD);
    DWORD value;
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, SETTINGS_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, L"StartWithWindows", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            startWithSystem = (value != 0);
        }
        
        dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"ChangeIconWithTheme", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            changeIconWithTheme = (value != 0);
        }
        
        dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"HotkeyModifiers", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            hotkeyConfig.useSuper = (value & MOD_WIN) != 0;
            hotkeyConfig.useShift = (value & MOD_SHIFT) != 0;
            hotkeyConfig.useCtrl = (value & MOD_CONTROL) != 0;
            hotkeyConfig.useAlt = (value & MOD_ALT) != 0;
        }
        
        dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"HotkeyVirtualKey", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            hotkeyConfig.key = (char)value;
        }
        
        RegCloseKey(hKey);
    }
}

void WindowsSettings::setAutoStart(bool enable) {
    HKEY hKey;
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, AUTOSTART_KEY, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            wchar_t szPath[MAX_PATH];
            GetModuleFileName(NULL, szPath, MAX_PATH);
            RegSetValueEx(hKey, APP_NAME, 0, REG_SZ, (LPBYTE)szPath, (wcslen(szPath) + 1) * sizeof(wchar_t));
        } else {
            RegDeleteValue(hKey, APP_NAME);
        }
        RegCloseKey(hKey);
    }
}