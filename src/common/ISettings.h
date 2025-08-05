#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <string>

struct HotkeyConfig {
    bool useSuper = true;
    bool useShift = true;
    bool useCtrl = false;
    bool useAlt = false;
    char key = 'D';
};

class ISettings {
public:
    virtual ~ISettings() = default;
    
    virtual bool getStartWithSystem() const = 0;
    virtual void setStartWithSystem(bool enable) = 0;
    
    virtual bool getChangeIconWithTheme() const = 0;
    virtual void setChangeIconWithTheme(bool enable) = 0;
    
    virtual HotkeyConfig getHotkey() const = 0;
    virtual void setHotkey(const HotkeyConfig& config) = 0;
    
    virtual void save() = 0;
    virtual void load() = 0;
};

#endif