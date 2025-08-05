#ifndef WINDOWSHOTKEYMANAGER_H
#define WINDOWSHOTKEYMANAGER_H

#include "../../common/IHotkeyManager.h"
#include <windows.h>

class WindowsHotkeyManager : public IHotkeyManager {
private:
    static constexpr int HOTKEY_ID = 1;
    HWND hwnd;
    std::function<void()> callback;
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
public:
    WindowsHotkeyManager();
    ~WindowsHotkeyManager() override;
    
    bool registerHotkey(const HotkeyConfig& config, std::function<void()> callback) override;
    void unregisterHotkey() override;
};

#endif