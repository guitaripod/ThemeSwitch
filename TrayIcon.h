#ifndef TRAYICON_H
#define TRAYICON_H

#include <windows.h>
#include <shellapi.h>

class TrayIcon {
private:
    NOTIFYICONDATA nid;
    HICON hIconLight;
    HICON hIconDark;
    
public:
    TrayIcon(HWND hwnd, UINT uCallbackMessage, bool isLightMode);
    ~TrayIcon();
    
    void UpdateIcon(bool isLightMode);
};

#endif