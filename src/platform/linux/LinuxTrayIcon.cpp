#include "LinuxTrayIcon.h"
#include <QApplication>
#include <QIcon>

LinuxTrayIcon::LinuxTrayIcon() {
    trayIcon = std::make_unique<QSystemTrayIcon>();
    contextMenu = std::make_unique<QMenu>();
    
    setupMenu();
    trayIcon->setContextMenu(contextMenu.get());
    
    QObject::connect(trayIcon.get(), &QSystemTrayIcon::activated,
        [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger && onLeftClickCallback) {
                onLeftClickCallback();
            }
        });
}

void LinuxTrayIcon::setupMenu() {
    settingsAction = std::make_unique<QAction>("Settings", nullptr);
    exitAction = std::make_unique<QAction>("Exit", nullptr);
    
    contextMenu->addAction(settingsAction.get());
    contextMenu->addSeparator();
    contextMenu->addAction(exitAction.get());
    
    QObject::connect(settingsAction.get(), &QAction::triggered, [this]() {
        if (onSettingsClickCallback) {
            onSettingsClickCallback();
        }
    });
    
    QObject::connect(exitAction.get(), &QAction::triggered, []() {
        QApplication::quit();
    });
}

void LinuxTrayIcon::show() {
    trayIcon->show();
}

void LinuxTrayIcon::hide() {
    trayIcon->hide();
}

void LinuxTrayIcon::updateIcon(bool isLightMode) {
    QString iconName = isLightMode ? ":/icons/light.svg" : ":/icons/dark.svg";
    QIcon icon(iconName);
    
    if (icon.isNull()) {
        icon = QIcon::fromTheme(isLightMode ? "weather-clear" : "weather-clear-night");
    }
    
    trayIcon->setIcon(icon);
}

void LinuxTrayIcon::setTooltip(const std::string& tooltip) {
    trayIcon->setToolTip(QString::fromStdString(tooltip));
}

void LinuxTrayIcon::setOnLeftClick(std::function<void()> callback) {
    onLeftClickCallback = callback;
}

void LinuxTrayIcon::setOnRightClick(std::function<void()> callback) {
    onRightClickCallback = callback;
}

void LinuxTrayIcon::setOnSettingsClick(std::function<void()> callback) {
    onSettingsClickCallback = callback;
}