#include "SettingsDialog.h"
#include "LinuxSettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

SettingsDialog::SettingsDialog(LinuxSettings* s, QWidget* parent) 
    : QDialog(parent), settings(s) {
    
    setWindowTitle("ThemeSwitch Settings");
    setFixedSize(350, 250);
    
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    
    auto* settingsGroup = new QGroupBox("Settings", this);
    auto* settingsLayout = new QVBoxLayout(settingsGroup);
    
    startWithSystemCheck = new QCheckBox("Start with system", this);
    changeIconCheck = new QCheckBox("Change tray icon with theme", this);
    
    settingsLayout->addWidget(startWithSystemCheck);
    settingsLayout->addWidget(changeIconCheck);
    
    auto* hotkeyGroup = new QGroupBox("Hotkey", this);
    auto* hotkeyLayout = new QVBoxLayout(hotkeyGroup);
    
    HotkeyConfig config = settings->getHotkey();
    QString hotkeyText = "Current: ";
    if (config.useSuper) hotkeyText += "Super+";
    if (config.useShift) hotkeyText += "Shift+";
    if (config.useCtrl) hotkeyText += "Ctrl+";
    if (config.useAlt) hotkeyText += "Alt+";
    hotkeyText += config.key;
    
    hotkeyLabel = new QLabel(hotkeyText, this);
    hotkeyLayout->addWidget(hotkeyLabel);
    
    auto* infoLabel = new QLabel("Click the tray icon to toggle theme", this);
    infoLabel->setStyleSheet("QLabel { color: gray; }");
    
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addWidget(settingsGroup);
    mainLayout->addWidget(hotkeyGroup);
    mainLayout->addWidget(infoLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    connect(okButton, &QPushButton::clicked, [this]() { onOkClicked(); });
    connect(cancelButton, &QPushButton::clicked, [this]() { onCancelClicked(); });
}

void SettingsDialog::loadSettings() {
    startWithSystemCheck->setChecked(settings->getStartWithSystem());
    changeIconCheck->setChecked(settings->getChangeIconWithTheme());
}

void SettingsDialog::onOkClicked() {
    settings->setStartWithSystem(startWithSystemCheck->isChecked());
    settings->setChangeIconWithTheme(changeIconCheck->isChecked());
    settings->save();
    
    accept();
}

void SettingsDialog::onCancelClicked() {
    reject();
}