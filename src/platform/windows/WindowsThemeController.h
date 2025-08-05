#ifndef WINDOWSTHEMECONTROLLER_H
#define WINDOWSTHEMECONTROLLER_H

#include "../../common/IThemeController.h"
#include <windows.h>

class WindowsThemeController : public IThemeController {
private:
    const wchar_t* PERSONALIZE_KEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
    const wchar_t* SYSTEM_THEME_VALUE = L"SystemUsesLightTheme";
    const wchar_t* APPS_THEME_VALUE = L"AppsUseLightTheme";
    
    DWORD readThemeValue(const wchar_t* valueName) const;
    void writeThemeValue(const wchar_t* valueName, DWORD value);
    void broadcastThemeChange();
    
public:
    WindowsThemeController();
    ~WindowsThemeController() override = default;
    
    bool isLightMode() const override;
    void setTheme(bool lightMode) override;
    void toggle() override;
};

#endif