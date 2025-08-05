#ifndef ITHEMECONTROLLER_H
#define ITHEMECONTROLLER_H

class IThemeController {
public:
    virtual ~IThemeController() = default;
    
    virtual bool isLightMode() const = 0;
    virtual void setTheme(bool lightMode) = 0;
    virtual void toggle() = 0;
};

#endif