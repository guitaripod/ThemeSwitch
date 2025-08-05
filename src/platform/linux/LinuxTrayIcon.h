#ifndef LINUXTRAYICON_H
#define LINUXTRAYICON_H

#include "../../common/ITrayIcon.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <memory>

class LinuxSettings;

class LinuxTrayIcon : public ITrayIcon {
private:
    std::unique_ptr<QSystemTrayIcon> trayIcon;
    std::unique_ptr<QMenu> contextMenu;
    std::unique_ptr<QAction> settingsAction;
    std::unique_ptr<QAction> exitAction;
    
    std::function<void()> onLeftClickCallback;
    std::function<void()> onRightClickCallback;
    std::function<void()> onSettingsClickCallback;
    
    void setupMenu();
    
public:
    LinuxTrayIcon();
    ~LinuxTrayIcon() override = default;
    
    void show() override;
    void hide() override;
    void updateIcon(bool isLightMode) override;
    void setTooltip(const std::string& tooltip) override;
    
    void setOnLeftClick(std::function<void()> callback) override;
    void setOnRightClick(std::function<void()> callback) override;
    void setOnSettingsClick(std::function<void()> callback);
};

#endif