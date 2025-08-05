#include "Application.h"

#ifdef _WIN32
    #include "platform/windows/WindowsThemeController.h"
    #include "platform/windows/WindowsTrayIcon.h"
    #include "platform/windows/WindowsSettings.h"
    #include "platform/windows/WindowsHotkeyManager.h"
#else
    #include "platform/linux/LinuxThemeController.h"
    #include "platform/linux/LinuxTrayIcon.h"
    #include "platform/linux/LinuxSettings.h"
    #include "platform/linux/SettingsDialog.h"
    #include "platform/linux/LinuxHotkeyManager.h"
#endif

Application::Application() {
#ifdef _WIN32
    themeController = std::make_unique<WindowsThemeController>();
    settings = std::make_unique<WindowsSettings>();
    hotkeyManager = std::make_unique<WindowsHotkeyManager>();
#else
    themeController = std::make_unique<LinuxThemeController>();
    trayIcon = std::make_unique<LinuxTrayIcon>();
    settings = std::make_unique<LinuxSettings>();
    hotkeyManager = std::make_unique<LinuxHotkeyManager>();
#endif

    setupTrayIcon();
    setupHotkeys();
    
    if (settings->getStartWithSystem()) {
        settings->setStartWithSystem(true);
    }
}

Application::~Application() = default;

void Application::setupTrayIcon() {
    if (!trayIcon) return;
    
    trayIcon->setTooltip("ThemeSwitch - Click to toggle");
    trayIcon->updateIcon(themeController->isLightMode());
    
    trayIcon->setOnLeftClick([this]() {
        toggleTheme();
    });
    
    trayIcon->setOnRightClick([this]() {
    });
    
#ifndef _WIN32
    if (auto* linuxTray = dynamic_cast<LinuxTrayIcon*>(trayIcon.get())) {
        linuxTray->setOnSettingsClick([this]() {
            showSettings();
        });
    }
#endif
    
    trayIcon->show();
}

void Application::setupHotkeys() {
    if (!hotkeyManager) return;
    
    HotkeyConfig config = settings->getHotkey();
    hotkeyManager->registerHotkey(config, [this]() {
        toggleTheme();
    });
}

void Application::toggleTheme() {
    themeController->toggle();
    
    if (settings->getChangeIconWithTheme() && trayIcon) {
        trayIcon->updateIcon(themeController->isLightMode());
    }
}

void Application::showSettings() {
#ifdef _WIN32
#else
    auto* linuxSettings = static_cast<LinuxSettings*>(settings.get());
    SettingsDialog dialog(linuxSettings);
    dialog.exec();
#endif
}

int Application::run() {
#ifdef _WIN32
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
#else
    return 0;
#endif
}