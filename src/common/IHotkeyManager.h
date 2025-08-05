#ifndef IHOTKEYMANAGER_H
#define IHOTKEYMANAGER_H

#include <functional>
#include "ISettings.h"

class IHotkeyManager {
public:
    virtual ~IHotkeyManager() = default;
    
    virtual bool registerHotkey(const HotkeyConfig& config, std::function<void()> callback) = 0;
    virtual void unregisterHotkey() = 0;
};

#endif