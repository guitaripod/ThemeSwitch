#include "LinuxHotkeyManager.h"
#include <QAction>
#include <QKeySequence>
#include <KGlobalAccel>

LinuxHotkeyManager::LinuxHotkeyManager() : QObject(nullptr), hotkeyAction(nullptr) {
}

LinuxHotkeyManager::~LinuxHotkeyManager() {
    unregisterHotkey();
}

bool LinuxHotkeyManager::registerHotkey(const HotkeyConfig& config, std::function<void()> cb) {
    unregisterHotkey();
    
    callback = cb;
    
    hotkeyAction = new QAction("Toggle Theme", this);
    hotkeyAction->setObjectName("ThemeSwitch_ToggleTheme");
    hotkeyAction->setProperty("componentName", "ThemeSwitch");
    
    QString keySequence;
    if (config.useSuper) keySequence += "Meta+";
    if (config.useCtrl) keySequence += "Ctrl+";
    if (config.useAlt) keySequence += "Alt+";
    if (config.useShift) keySequence += "Shift+";
    keySequence += config.key;
    
    KGlobalAccel *globalAccel = KGlobalAccel::self();
    globalAccel->setShortcut(hotkeyAction, QList<QKeySequence>{QKeySequence(keySequence)});
    globalAccel->setDefaultShortcut(hotkeyAction, QList<QKeySequence>{QKeySequence(keySequence)});
    
    connect(hotkeyAction, &QAction::triggered, this, &LinuxHotkeyManager::onHotkeyTriggered);
    
    return true;
}

void LinuxHotkeyManager::unregisterHotkey() {
    if (hotkeyAction) {
        KGlobalAccel *globalAccel = KGlobalAccel::self();
        globalAccel->removeAllShortcuts(hotkeyAction);
        delete hotkeyAction;
        hotkeyAction = nullptr;
    }
}

void LinuxHotkeyManager::onHotkeyTriggered() {
    if (callback) {
        callback();
    }
}