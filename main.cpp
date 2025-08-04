#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include "resource.h"
#include "TrayIcon.h"
#include "ThemeToggle.h"
#include "Settings.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "comctl32.lib")

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_SETTINGS 1002
#define ID_HOTKEY_TOGGLE 1003

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

TrayIcon* g_trayIcon = nullptr;
ThemeToggle* g_themeToggle = nullptr;
Settings* g_settings = nullptr;
HWND g_hwnd = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);
    
    const wchar_t* className = L"ThemeSwitchWindow";
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    g_hwnd = CreateWindowEx(
        0,
        className,
        L"ThemeSwitch",
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,
        NULL,
        hInstance,
        NULL
    );
    
    if (!g_hwnd) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    g_themeToggle = new ThemeToggle();
    g_settings = new Settings(hInstance);
    g_trayIcon = new TrayIcon(g_hwnd, WM_TRAYICON, g_themeToggle->IsLightMode());
    
    if (g_settings->GetStartWithWindows()) {
        g_settings->SetAutoStart(true);
    }
    
    UINT modifiers = MOD_WIN | MOD_SHIFT;
    UINT vk = 'D';
    g_settings->GetHotkey(modifiers, vk);
    RegisterHotKey(g_hwnd, ID_HOTKEY_TOGGLE, modifiers, vk);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    delete g_trayIcon;
    delete g_themeToggle;
    delete g_settings;
    
    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            return 0;
            
        case WM_TRAYICON:
            if (lParam == WM_LBUTTONUP) {
                g_themeToggle->Toggle();
                if (g_settings->GetChangeIconWithTheme()) {
                    g_trayIcon->UpdateIcon(g_themeToggle->IsLightMode());
                }
            } else if (lParam == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt);
                
                HMENU hMenu = CreatePopupMenu();
                AppendMenu(hMenu, MF_STRING, ID_TRAY_SETTINGS, L"Settings");
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");
                
                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
                DestroyMenu(hMenu);
            }
            return 0;
            
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_TRAY_EXIT:
                    PostQuitMessage(0);
                    break;
                case ID_TRAY_SETTINGS:
                    g_settings->ShowDialog(hwnd);
                    break;
            }
            return 0;
            
        case WM_HOTKEY:
            if (wParam == ID_HOTKEY_TOGGLE) {
                g_themeToggle->Toggle();
                if (g_settings->GetChangeIconWithTheme()) {
                    g_trayIcon->UpdateIcon(g_themeToggle->IsLightMode());
                }
            }
            return 0;
            
        case WM_DESTROY:
            UnregisterHotKey(hwnd, ID_HOTKEY_TOGGLE);
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}