#ifndef LINUXSETTINGS_H
#define LINUXSETTINGS_H

#include "../../common/ISettings.h"
#include <string>

class LinuxSettings : public ISettings {
private:
    bool startWithSystem;
    bool changeIconWithTheme;
    HotkeyConfig hotkeyConfig;
    
    std::string getConfigPath() const;
    std::string getAutostartPath() const;
    
public:
    LinuxSettings();
    ~LinuxSettings() override = default;
    
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