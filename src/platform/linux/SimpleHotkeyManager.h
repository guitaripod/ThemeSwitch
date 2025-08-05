#ifndef SIMPLEHOTKEYMANAGER_H
#define SIMPLEHOTKEYMANAGER_H

#include "../../common/IHotkeyManager.h"
#include <QObject>
#include <memory>

class QShortcut;

class SimpleHotkeyManager : public QObject, public IHotkeyManager {
    Q_OBJECT
    
private:
    std::unique_ptr<QShortcut> shortcut;
    std::function<void()> callback;
    
public:
    SimpleHotkeyManager();
    ~SimpleHotkeyManager() override;
    
    bool registerHotkey(const HotkeyConfig& config, std::function<void()> callback) override;
    void unregisterHotkey() override;
    
private slots:
    void onActivated();
};

#endif