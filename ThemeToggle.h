#ifndef THEMETOGGLE_H
#define THEMETOGGLE_H

#include <windows.h>

#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED 0x031A
#endif

class ThemeToggle {
private:
    const wchar_t* PERSONALIZE_KEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    const wchar_t* SYSTEM_THEME_VALUE = L"SystemUsesLightTheme";
    const wchar_t* APPS_THEME_VALUE = L"AppsUseLightTheme";
    
    DWORD ReadThemeValue(const wchar_t* valueName);
    void WriteThemeValue(const wchar_t* valueName, DWORD value);
    void BroadcastThemeChange();
    
public:
    ThemeToggle();
    ~ThemeToggle();
    
    bool IsLightMode();
    void Toggle();
    void SetTheme(bool lightMode);
};

#endif