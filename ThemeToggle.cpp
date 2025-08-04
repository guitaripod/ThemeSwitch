#include "ThemeToggle.h"

ThemeToggle::ThemeToggle() {
}

ThemeToggle::~ThemeToggle() {
}

DWORD ThemeToggle::ReadThemeValue(const wchar_t* valueName) {
    HKEY hKey;
    DWORD value = 0;
    DWORD dataSize = sizeof(DWORD);
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, PERSONALIZE_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueEx(hKey, valueName, NULL, NULL, (LPBYTE)&value, &dataSize);
        RegCloseKey(hKey);
    }
    
    return value;
}

void ThemeToggle::WriteThemeValue(const wchar_t* valueName, DWORD value) {
    HKEY hKey;
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, PERSONALIZE_KEY, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, valueName, 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

void ThemeToggle::BroadcastThemeChange() {
    SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"ImmersiveColorSet");
    Sleep(50);
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"ImmersiveColorSet", 
                      SMTO_ABORTIFHUNG, 5000, NULL);
    SendMessageTimeout(HWND_BROADCAST, WM_THEMECHANGED, 0, 0, 
                      SMTO_ABORTIFHUNG, 5000, NULL);
    SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"WindowsThemeElement");
    SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"WindowsThemeElement");
}

bool ThemeToggle::IsLightMode() {
    return ReadThemeValue(SYSTEM_THEME_VALUE) == 1;
}

void ThemeToggle::Toggle() {
    bool currentlyLight = IsLightMode();
    SetTheme(!currentlyLight);
}

void ThemeToggle::SetTheme(bool lightMode) {
    DWORD value = lightMode ? 1 : 0;
    WriteThemeValue(SYSTEM_THEME_VALUE, value);
    WriteThemeValue(APPS_THEME_VALUE, value);
    BroadcastThemeChange();
}