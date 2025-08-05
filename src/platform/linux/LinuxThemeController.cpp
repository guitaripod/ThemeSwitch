#include "LinuxThemeController.h"
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <array>

LinuxThemeController::LinuxThemeController() : cachedLightMode(false), cacheValid(false) {
    detectCurrentTheme();
}

bool LinuxThemeController::detectCurrentTheme() const {
    std::array<char, 128> buffer;
    std::string result;
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen("plasma-apply-colorscheme --list-schemes | grep '(current' | grep -q 'BreezeLight' && echo 'light' || echo 'dark'", "r"),
        pclose
    );
    
    if (!pipe) {
        return false;
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    cachedLightMode = (result.find("light") != std::string::npos);
    cacheValid = true;
    
    return cachedLightMode;
}

bool LinuxThemeController::isLightMode() const {
    if (!cacheValid) {
        detectCurrentTheme();
    }
    return cachedLightMode;
}

void LinuxThemeController::setTheme(bool lightMode) {
    const char* command = lightMode 
        ? "plasma-apply-colorscheme BreezeLight >/dev/null 2>&1"
        : "plasma-apply-colorscheme BreezeDark >/dev/null 2>&1";
    
    system(command);
    
    cachedLightMode = lightMode;
    cacheValid = true;
}

void LinuxThemeController::toggle() {
    setTheme(!isLightMode());
}