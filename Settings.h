#ifndef SETTINGS_H
#define SETTINGS_H

#include <windows.h>

class Settings {
private:
    HINSTANCE hInstance;
    const wchar_t* SETTINGS_KEY = L"SOFTWARE\\ThemeSwitch";
    const wchar_t* AUTOSTART_KEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t* APP_NAME = L"ThemeSwitch";
    
    bool startWithWindows;
    bool changeIconWithTheme;
    UINT hotkeyModifiers;
    UINT hotkeyVirtualKey;
    
    void LoadSettings();
    void SaveSettings();
    
public:
    Settings(HINSTANCE hInst);
    ~Settings();
    
    bool GetStartWithWindows() { return startWithWindows; }
    bool GetChangeIconWithTheme() { return changeIconWithTheme; }
    void GetHotkey(UINT& modifiers, UINT& vk) { modifiers = hotkeyModifiers; vk = hotkeyVirtualKey; }
    
    void SetStartWithWindows(bool value);
    void SetChangeIconWithTheme(bool value) { changeIconWithTheme = value; SaveSettings(); }
    void SetHotkey(UINT modifiers, UINT vk);
    
    void SetAutoStart(bool enable);
    void ShowDialog(HWND hwndParent);
    
    static LRESULT CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif