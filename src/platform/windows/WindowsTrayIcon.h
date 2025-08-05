#ifndef WINDOWSTRAYICON_H
#define WINDOWSTRAYICON_H

#include "../../common/ITrayIcon.h"
#include <windows.h>
#include <shellapi.h>
#include <functional>
#include <string>

class WindowsTrayIcon : public ITrayIcon {
private:
    HWND hwnd;
    NOTIFYICONDATA nid;
    HICON hIconLight;
    HICON hIconDark;
    UINT callbackMessage;
    
    std::function<void()> onLeftClickCallback;
    std::function<void()> onRightClickCallback;
    
public:
    WindowsTrayIcon(HWND window, UINT message);
    ~WindowsTrayIcon() override;
    
    void show() override;
    void hide() override;
    void updateIcon(bool isLightMode) override;
    void setTooltip(const std::string& tooltip) override;
    
    void setOnLeftClick(std::function<void()> callback) override;
    void setOnRightClick(std::function<void()> callback) override;
    
    void handleMessage(LPARAM lParam);
};

#endif