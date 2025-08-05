#ifndef LINUXHOTKEYMANAGER_H
#define LINUXHOTKEYMANAGER_H

#include "../../common/IHotkeyManager.h"
#include <QObject>
#include <memory>

class QAction;

class LinuxHotkeyManager : public QObject, public IHotkeyManager {
private:
    QAction* hotkeyAction;
    std::function<void()> callback;
    
public:
    LinuxHotkeyManager();
    ~LinuxHotkeyManager() override;
    
    bool registerHotkey(const HotkeyConfig& config, std::function<void()> callback) override;
    void unregisterHotkey() override;
    
    void onHotkeyTriggered();
};

#endif