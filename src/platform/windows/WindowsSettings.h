#ifndef WINDOWSSETTINGS_H
#define WINDOWSSETTINGS_H

#include "../../common/ISettings.h"
#include <windows.h>

class WindowsSettings : public ISettings {
private:
    const wchar_t* SETTINGS_KEY = L"SOFTWARE\\ThemeSwitch";
    const wchar_t* AUTOSTART_KEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t* APP_NAME = L"ThemeSwitch";
    
    bool startWithSystem;
    bool changeIconWithTheme;
    HotkeyConfig hotkeyConfig;
    
    void setAutoStart(bool enable);
    
public:
    WindowsSettings();
    ~WindowsSettings() override = default;
    
    bool getStartWithSystem() const override;
    void setStartWithSystem(bool enable) override;
    
    bool getChangeIconWithTheme() const override;
    void setChangeIconWithTheme(bool enable) override;
    
    HotkeyConfig getHotkey() const override;
    void setHotkey(const HotkeyConfig& config) override;
    
    void save() override;
    void load() override;
};

#endif