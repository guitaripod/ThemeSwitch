#include "Settings.h"
#include "resource.h"
#include "ThemeToggle.h"
#include <commctrl.h>
#include <sstream>

Settings* g_settingsInstance = nullptr;

Settings::Settings(HINSTANCE hInst) : hInstance(hInst) {
    hotkeyModifiers = MOD_WIN | MOD_SHIFT;
    hotkeyVirtualKey = 'D';
    startWithWindows = false;
    changeIconWithTheme = true;
    g_settingsInstance = this;
    LoadSettings();
}

Settings::~Settings() {
    g_settingsInstance = nullptr;
}

void Settings::LoadSettings() {
    HKEY hKey;
    DWORD dataSize = sizeof(DWORD);
    DWORD value;
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, SETTINGS_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, L"StartWithWindows", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            startWithWindows = (value != 0);
        }
        
        dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"ChangeIconWithTheme", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            changeIconWithTheme = (value != 0);
        }
        
        dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"HotkeyModifiers", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            hotkeyModifiers = value;
        }
        
        dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"HotkeyVirtualKey", NULL, NULL, (LPBYTE)&value, &dataSize) == ERROR_SUCCESS) {
            hotkeyVirtualKey = value;
        }
        
        RegCloseKey(hKey);
    }
}

void Settings::SaveSettings() {
    HKEY hKey;
    DWORD value;
    
    if (RegCreateKeyEx(HKEY_CURRENT_USER, SETTINGS_KEY, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        value = startWithWindows ? 1 : 0;
        RegSetValueEx(hKey, L"StartWithWindows", 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        
        value = changeIconWithTheme ? 1 : 0;
        RegSetValueEx(hKey, L"ChangeIconWithTheme", 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
        
        RegSetValueEx(hKey, L"HotkeyModifiers", 0, REG_DWORD, (LPBYTE)&hotkeyModifiers, sizeof(DWORD));
        RegSetValueEx(hKey, L"HotkeyVirtualKey", 0, REG_DWORD, (LPBYTE)&hotkeyVirtualKey, sizeof(DWORD));
        
        RegCloseKey(hKey);
    }
}

void Settings::SetStartWithWindows(bool value) {
    startWithWindows = value;
    SetAutoStart(value);
    SaveSettings();
}

void Settings::SetHotkey(UINT modifiers, UINT vk) {
    hotkeyModifiers = modifiers;
    hotkeyVirtualKey = vk;
    SaveSettings();
}

void Settings::SetAutoStart(bool enable) {
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

void Settings::ShowDialog(HWND hwndParent) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = DialogProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"ThemeSwitchSettings";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);
    
    HWND hwndDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_WINDOWEDGE,
        L"ThemeSwitchSettings",
        L"ThemeSwitch Settings",
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION | DS_MODALFRAME | DS_CENTER | WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, 280,
        hwndParent, NULL, hInstance, NULL
    );
    
    if (hwndDlg) {
        EnableWindow(hwndParent, FALSE);
        
        MSG msg;
        while (IsWindow(hwndDlg) && GetMessage(&msg, NULL, 0, 0)) {
            if (!IsDialogMessage(hwndDlg, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        
        EnableWindow(hwndParent, TRUE);
        SetForegroundWindow(hwndParent);
    }
}

LRESULT CALLBACK Settings::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndCheckStartup;
    static HWND hwndCheckIcon;
    static HWND hwndButtonOK;
    static HWND hwndButtonCancel;
    static HWND hwndThemeStatus;
    static HFONT hFontBold;
    static HFONT hFontNormal;
    static HBRUSH hbrBackground = NULL;
    static HBRUSH hbrAccent = NULL;
    static HPEN hpenBorder = NULL;
    static ThemeToggle themeCheck;
    
    const COLORREF clrBackground = RGB(252, 243, 243);
    const COLORREF clrAccent = RGB(180, 83, 100);
    const COLORREF clrText = RGB(64, 45, 48);
    const COLORREF clrSecondary = RGB(234, 196, 203);
    
    switch (uMsg) {
        case WM_CREATE: {
            hbrBackground = CreateSolidBrush(clrBackground);
            hbrAccent = CreateSolidBrush(clrAccent);
            hpenBorder = CreatePen(PS_SOLID, 2, clrAccent);
            LOGFONT lf = {0};
            lf.lfHeight = -14;
            wcscpy_s(lf.lfFaceName, L"Segoe UI");
            lf.lfQuality = CLEARTYPE_QUALITY;
            hFontNormal = CreateFontIndirect(&lf);
            
            lf.lfHeight = -16;
            lf.lfWeight = FW_SEMIBOLD;
            hFontBold = CreateFontIndirect(&lf);
            
            HWND hwndTitle = CreateWindow(L"STATIC", L"ThemeSwitch Settings",
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                10, 10, 290, 25,
                hwndDlg, NULL, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndTitle, WM_SETFONT, (WPARAM)hFontBold, TRUE);
            
            CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ,
                10, 40, 290, 1, hwndDlg, NULL, g_settingsInstance->hInstance, NULL);
            
            hwndCheckStartup = CreateWindow(L"BUTTON", L"Start with Windows",
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_FLAT,
                20, 55, 200, 20,
                hwndDlg, (HMENU)IDC_CHECK_STARTUP, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndCheckStartup, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            if (g_settingsInstance->startWithWindows) {
                SendMessage(hwndCheckStartup, BM_SETCHECK, BST_CHECKED, 0);
            }
            
            hwndCheckIcon = CreateWindow(L"BUTTON", L"Change tray icon with theme",
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_FLAT,
                20, 85, 200, 20,
                hwndDlg, (HMENU)IDC_CHECK_ICON, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndCheckIcon, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            if (g_settingsInstance->changeIconWithTheme) {
                SendMessage(hwndCheckIcon, BM_SETCHECK, BST_CHECKED, 0);
            }
            
            HWND hwndHotkeyLabel = CreateWindow(L"STATIC", L"Hotkey:",
                WS_CHILD | WS_VISIBLE,
                20, 120, 60, 20,
                hwndDlg, NULL, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndHotkeyLabel, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            
            std::wstring hotkeyText = L"Win+Shift+";
            hotkeyText += (wchar_t)g_settingsInstance->hotkeyVirtualKey;
            HWND hwndHotkeyValue = CreateWindow(L"STATIC", hotkeyText.c_str(),
                WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER,
                85, 118, 100, 22,
                hwndDlg, NULL, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndHotkeyValue, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            
            CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ,
                10, 155, 290, 1, hwndDlg, NULL, g_settingsInstance->hInstance, NULL);
            
            std::wstring themeText = L"Current theme: ";
            themeText += themeCheck.IsLightMode() ? L"Light Mode" : L"Dark Mode";
            hwndThemeStatus = CreateWindow(L"STATIC", themeText.c_str(),
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                10, 165, 290, 20,
                hwndDlg, (HMENU)IDC_THEME_STATUS, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndThemeStatus, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            
            HWND hwndVersion = CreateWindow(L"STATIC", L"Version 1.0.0",
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                10, 185, 290, 20,
                hwndDlg, NULL, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndVersion, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            
            hwndButtonOK = CreateWindow(L"BUTTON", L"OK",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                155, 215, 70, 25,
                hwndDlg, (HMENU)IDOK, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndButtonOK, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            
            hwndButtonCancel = CreateWindow(L"BUTTON", L"Cancel",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                230, 215, 70, 25,
                hwndDlg, (HMENU)IDCANCEL, g_settingsInstance->hInstance, NULL);
            SendMessage(hwndButtonCancel, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
            
            RECT rcWindow;
            GetWindowRect(hwndDlg, &rcWindow);
            int xPos = (GetSystemMetrics(SM_CXSCREEN) - (rcWindow.right - rcWindow.left)) / 2;
            int yPos = (GetSystemMetrics(SM_CYSCREEN) - (rcWindow.bottom - rcWindow.top)) / 2;
            SetWindowPos(hwndDlg, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            
            return 0;
        }
        
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    g_settingsInstance->SetStartWithWindows(
                        SendMessage(hwndCheckStartup, BM_GETCHECK, 0, 0) == BST_CHECKED);
                    g_settingsInstance->SetChangeIconWithTheme(
                        SendMessage(hwndCheckIcon, BM_GETCHECK, 0, 0) == BST_CHECKED);
                    DestroyWindow(hwndDlg);
                    return 0;
                }
                
                case IDCANCEL:
                    DestroyWindow(hwndDlg);
                    return 0;
            }
            break;
            
        case WM_CLOSE:
            DestroyWindow(hwndDlg);
            return 0;
            
        case WM_DESTROY:
            if (hFontBold) DeleteObject(hFontBold);
            if (hFontNormal) DeleteObject(hFontNormal);
            if (hbrBackground) DeleteObject(hbrBackground);
            if (hbrAccent) DeleteObject(hbrAccent);
            if (hpenBorder) DeleteObject(hpenBorder);
            return 0;
            
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND hwndCtl = (HWND)lParam;
            
            if (hwndCtl == hwndCheckStartup || hwndCtl == hwndCheckIcon) {
                SetTextColor(hdc, clrText);
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)hbrBackground;
            }
            
            SetTextColor(hdc, clrText);
            SetBkColor(hdc, clrBackground);
            return (LRESULT)hbrBackground;
        }
            
        case WM_CTLCOLORDLG: {
            return (LRESULT)hbrBackground;
        }
            
        case WM_SETTINGCHANGE: {
            if (lParam && wcscmp((LPCWSTR)lParam, L"ImmersiveColorSet") == 0) {
                std::wstring themeText = L"Current theme: ";
                themeText += themeCheck.IsLightMode() ? L"Light Mode" : L"Dark Mode";
                SetWindowText(hwndThemeStatus, themeText.c_str());
            }
            break;
        }
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwndDlg, &ps);
            RECT rcClient;
            GetClientRect(hwndDlg, &rcClient);
            RECT rcAccent = {0, 0, rcClient.right, 3};
            FillRect(hdc, &rcAccent, hbrAccent);
            
            EndPaint(hwndDlg, &ps);
            return 0;
        }
    }
    
    return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}