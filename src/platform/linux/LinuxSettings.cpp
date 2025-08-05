#include "LinuxSettings.h"
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QString>

namespace fs = std::filesystem;

LinuxSettings::LinuxSettings() : startWithSystem(false), changeIconWithTheme(true) {
    load();
}

std::string LinuxSettings::getConfigPath() const {
    const char* home = std::getenv("HOME");
    if (!home) return "";
    
    std::string configDir = std::string(home) + "/.config/ThemeSwitch";
    fs::create_directories(configDir);
    
    return configDir + "/settings.json";
}

std::string LinuxSettings::getAutostartPath() const {
    const char* home = std::getenv("HOME");
    if (!home) return "";
    
    std::string autostartDir = std::string(home) + "/.config/autostart";
    fs::create_directories(autostartDir);
    
    return autostartDir + "/themeswitch.desktop";
}

bool LinuxSettings::getStartWithSystem() const {
    return startWithSystem;
}

void LinuxSettings::setStartWithSystem(bool enable) {
    startWithSystem = enable;
    
    std::string desktopFile = getAutostartPath();
    
    if (enable) {
        std::ofstream file(desktopFile);
        if (file.is_open()) {
            file << "[Desktop Entry]\n"
                 << "Type=Application\n"
                 << "Name=ThemeSwitch\n"
                 << "Comment=Toggle dark/light theme\n"
                 << "Exec=" << fs::current_path() / "ThemeSwitch" << "\n"
                 << "Hidden=false\n"
                 << "NoDisplay=false\n"
                 << "X-GNOME-Autostart-enabled=true\n";
            file.close();
        }
    } else {
        fs::remove(desktopFile);
    }
}

bool LinuxSettings::getChangeIconWithTheme() const {
    return changeIconWithTheme;
}

void LinuxSettings::setChangeIconWithTheme(bool enable) {
    changeIconWithTheme = enable;
}

HotkeyConfig LinuxSettings::getHotkey() const {
    return hotkeyConfig;
}

void LinuxSettings::setHotkey(const HotkeyConfig& config) {
    hotkeyConfig = config;
}

void LinuxSettings::save() {
    QJsonObject json;
    json["startWithSystem"] = startWithSystem;
    json["changeIconWithTheme"] = changeIconWithTheme;
    
    QJsonObject hotkey;
    hotkey["useSuper"] = hotkeyConfig.useSuper;
    hotkey["useShift"] = hotkeyConfig.useShift;
    hotkey["useCtrl"] = hotkeyConfig.useCtrl;
    hotkey["useAlt"] = hotkeyConfig.useAlt;
    hotkey["key"] = QString(QChar(hotkeyConfig.key));
    json["hotkey"] = hotkey;
    
    QJsonDocument doc(json);
    
    QFile file(QString::fromStdString(getConfigPath()));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void LinuxSettings::load() {
    QFile file(QString::fromStdString(getConfigPath()));
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;
    
    QJsonObject json = doc.object();
    startWithSystem = json["startWithSystem"].toBool(false);
    changeIconWithTheme = json["changeIconWithTheme"].toBool(true);
    
    if (json.contains("hotkey")) {
        QJsonObject hotkey = json["hotkey"].toObject();
        hotkeyConfig.useSuper = hotkey["useSuper"].toBool(true);
        hotkeyConfig.useShift = hotkey["useShift"].toBool(true);
        hotkeyConfig.useCtrl = hotkey["useCtrl"].toBool(false);
        hotkeyConfig.useAlt = hotkey["useAlt"].toBool(false);
        
        QString keyStr = hotkey["key"].toString("D");
        if (!keyStr.isEmpty()) {
            hotkeyConfig.key = keyStr[0].toLatin1();
        }
    }
}