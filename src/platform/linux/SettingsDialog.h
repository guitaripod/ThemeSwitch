#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <memory>

class QCheckBox;
class QPushButton;
class QLabel;
class LinuxSettings;

class SettingsDialog : public QDialog {
private:
    LinuxSettings* settings;
    
    QCheckBox* startWithSystemCheck;
    QCheckBox* changeIconCheck;
    QLabel* hotkeyLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;
    
    void setupUI();
    void loadSettings();
    void onOkClicked();
    void onCancelClicked();
    
public:
    explicit SettingsDialog(LinuxSettings* settings, QWidget* parent = nullptr);
    ~SettingsDialog() override = default;
};

#endif