#include "WindowsHotkeyManager.h"

static WindowsHotkeyManager* g_instance = nullptr;

WindowsHotkeyManager::WindowsHotkeyManager() {
    g_instance = this;
    
    const wchar_t* className = L"ThemeSwitchHotkeyWindow";
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;
    
    RegisterClassEx(&wc);
    
    hwnd = CreateWindowEx(0, className, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
}

WindowsHotkeyManager::~WindowsHotkeyManager() {
    unregisterHotkey();
    if (hwnd) {
        DestroyWindow(hwnd);
    }
    g_instance = nullptr;
}

bool WindowsHotkeyManager::registerHotkey(const HotkeyConfig& config, std::function<void()> cb) {
    unregisterHotkey();
    
    callback = cb;
    
    UINT modifiers = 0;
    if (config.useSuper) modifiers |= MOD_WIN;
    if (config.useShift) modifiers |= MOD_SHIFT;
    if (config.useCtrl) modifiers |= MOD_CONTROL;
    if (config.useAlt) modifiers |= MOD_ALT;
    
    return RegisterHotKey(hwnd, HOTKEY_ID, modifiers, config.key) != 0;
}

void WindowsHotkeyManager::unregisterHotkey() {
    UnregisterHotKey(hwnd, HOTKEY_ID);
}

LRESULT CALLBACK WindowsHotkeyManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_HOTKEY && wParam == HOTKEY_ID && g_instance && g_instance->callback) {
        g_instance->callback();
        return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}