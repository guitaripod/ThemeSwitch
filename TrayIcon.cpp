#include "TrayIcon.h"
#include "resource.h"

TrayIcon::TrayIcon(HWND hwnd, UINT uCallbackMessage, bool isLightMode) {
    hIconLight = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LIGHT));
    hIconDark = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DARK));
    
    if (!hIconLight) hIconLight = LoadIcon(NULL, IDI_APPLICATION);
    if (!hIconDark) hIconDark = LoadIcon(NULL, IDI_APPLICATION);
    
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = uCallbackMessage;
    nid.hIcon = isLightMode ? hIconLight : hIconDark;
    wcscpy_s(nid.szTip, L"ThemeSwitch - Click to toggle");
    
    Shell_NotifyIcon(NIM_ADD, &nid);
}

TrayIcon::~TrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    if (hIconLight) DestroyIcon(hIconLight);
    if (hIconDark) DestroyIcon(hIconDark);
}

void TrayIcon::UpdateIcon(bool isLightMode) {
    nid.hIcon = isLightMode ? hIconLight : hIconDark;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

