#ifndef LINUXTHEMECONTROLLER_H
#define LINUXTHEMECONTROLLER_H

#include "../../common/IThemeController.h"
#include <string>

class LinuxThemeController : public IThemeController {
private:
    mutable bool cachedLightMode;
    mutable bool cacheValid;
    
    bool detectCurrentTheme() const;
    
public:
    LinuxThemeController();
    ~LinuxThemeController() override = default;
    
    bool isLightMode() const override;
    void setTheme(bool lightMode) override;
    void toggle() override;
};

#endif