#include "WindowsThemeController.h"

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED 0x031A
#endif

WindowsThemeController::WindowsThemeController() {
}

DWORD WindowsThemeController::readThemeValue(const wchar_t* valueName) const {
    HKEY hKey;
    DWORD value = 0;
    DWORD dataSize = sizeof(DWORD);
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, PERSONALIZE_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueEx(hKey, valueName, NULL, NULL, (LPBYTE)&value, &dataSize);
        RegCloseKey(hKey);
    }
    
    return value;
}

void WindowsThemeController::writeThemeValue(const wchar_t* valueName, DWORD value) {
    HKEY hKey;
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, PERSONALIZE_KEY, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, valueName, 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

void WindowsThemeController::broadcastThemeChange() {
    SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"ImmersiveColorSet");
    Sleep(50);
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"ImmersiveColorSet", 
                      SMTO_ABORTIFHUNG, 5000, NULL);
    SendMessageTimeout(HWND_BROADCAST, WM_THEMECHANGED, 0, 0, 
                      SMTO_ABORTIFHUNG, 5000, NULL);
    SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"WindowsThemeElement");
    SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"WindowsThemeElement");
}

bool WindowsThemeController::isLightMode() const {
    return readThemeValue(SYSTEM_THEME_VALUE) == 1;
}

void WindowsThemeController::setTheme(bool lightMode) {
    DWORD value = lightMode ? 1 : 0;
    writeThemeValue(SYSTEM_THEME_VALUE, value);
    writeThemeValue(APPS_THEME_VALUE, value);
    broadcastThemeChange();
}

void WindowsThemeController::toggle() {
    setTheme(!isLightMode());
}