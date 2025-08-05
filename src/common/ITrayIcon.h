#ifndef ITRAYICON_H
#define ITRAYICON_H

#include <functional>
#include <string>

class ITrayIcon {
public:
    virtual ~ITrayIcon() = default;
    
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void updateIcon(bool isLightMode) = 0;
    virtual void setTooltip(const std::string& tooltip) = 0;
    
    virtual void setOnLeftClick(std::function<void()> callback) = 0;
    virtual void setOnRightClick(std::function<void()> callback) = 0;
};

#endif