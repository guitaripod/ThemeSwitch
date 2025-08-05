#ifndef APPLICATION_H
#define APPLICATION_H

#include "common/IThemeController.h"
#include "common/ITrayIcon.h"
#include "common/ISettings.h"
#include "common/IHotkeyManager.h"
#include <memory>

class Application {
private:
    std::unique_ptr<IThemeController> themeController;
    std::unique_ptr<ITrayIcon> trayIcon;
    std::unique_ptr<ISettings> settings;
    std::unique_ptr<IHotkeyManager> hotkeyManager;
    
    void setupTrayIcon();
    void setupHotkeys();
    void toggleTheme();
    
public:
    Application();
    ~Application();
    
    int run();
    void showSettings();
};

#endif