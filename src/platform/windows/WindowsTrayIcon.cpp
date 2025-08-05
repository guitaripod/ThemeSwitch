#include "WindowsTrayIcon.h"
#include "../../../resources/windows/resource.h"

WindowsTrayIcon::WindowsTrayIcon(HWND window, UINT message) 
    : hwnd(window), callbackMessage(message) {
    
    hIconLight = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LIGHT));
    hIconDark = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DARK));
    
    if (!hIconLight) hIconLight = LoadIcon(NULL, IDI_APPLICATION);
    if (!hIconDark) hIconDark = LoadIcon(NULL, IDI_APPLICATION);
    
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = callbackMessage;
    nid.hIcon = hIconDark;
    wcscpy_s(nid.szTip, L"ThemeSwitch - Click to toggle");
}

WindowsTrayIcon::~WindowsTrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    if (hIconLight) DestroyIcon(hIconLight);
    if (hIconDark) DestroyIcon(hIconDark);
}

void WindowsTrayIcon::show() {
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void WindowsTrayIcon::hide() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void WindowsTrayIcon::updateIcon(bool isLightMode) {
    nid.hIcon = isLightMode ? hIconLight : hIconDark;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void WindowsTrayIcon::setTooltip(const std::string& tooltip) {
    std::wstring wtooltip(tooltip.begin(), tooltip.end());
    wcscpy_s(nid.szTip, wtooltip.c_str());
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void WindowsTrayIcon::setOnLeftClick(std::function<void()> callback) {
    onLeftClickCallback = callback;
}

void WindowsTrayIcon::setOnRightClick(std::function<void()> callback) {
    onRightClickCallback = callback;
}

void WindowsTrayIcon::handleMessage(LPARAM lParam) {
    if (lParam == WM_LBUTTONUP && onLeftClickCallback) {
        onLeftClickCallback();
    } else if (lParam == WM_RBUTTONUP && onRightClickCallback) {
        onRightClickCallback();
    }
}