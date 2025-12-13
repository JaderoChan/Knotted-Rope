#include "setting.h"

#include <qfiledialog.h>

#include <easy_translate.hpp>

Setting::Setting(Config& config, QWidget* parent)
    : QDialog(parent), config_(config)
{
    ui.setupUi(this);
    ui.languageCombox->setCurrentIndex(config_.language());
    ui.defaultStorePathEdit->setText(config_.defaultStorePath());
    ui.openLastFileCb->setChecked(config_.openLastFile());
    ui.rememberLastPosCb->setChecked(config_.rememberPos());
    ui.keepSystemTrayCb->setChecked(config_.keepSystemTray());

    connect(ui.languageCombox, &QComboBox::currentIndexChanged, this, &Setting::onLanguageComboxChanged);
    connect(ui.defaultStorePathBtn, &QToolButton::clicked, this, &Setting::onDefaultStorePathBtnClicked);
    connect(ui.openLastFileCb, &QCheckBox::clicked, this, &Setting::onOpenLastFileCbChanged);
    connect(ui.rememberLastPosCb, &QCheckBox::clicked, this, &Setting::onRememberPosCbChanged);
    connect(ui.keepSystemTrayCb, &QCheckBox::clicked, this, &Setting::onKeepSystemTrayCbChanged);

    updateText();
}

void Setting::updateText()
{
    setWindowTitle(EASYTR(APP_TITLE));
    ui.languageTextLbl->setText(EASYTR("Language"));
    ui.languageCombox->setItemText(LANG_EN, EASYTR("English"));
    ui.languageCombox->setItemText(LANG_ZH, EASYTR("Chinese"));
    ui.defaultStorePathTextLbl->setText(EASYTR("Default store path"));
    ui.openLastFileCb->setText(EASYTR("Open last closed file"));
    ui.rememberLastPosCb->setText(EASYTR("Remember the window position"));
    ui.keepSystemTrayCb->setText(EASYTR("Minimize to the system tray when the window is closed"));
}

void Setting::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        updateText();
    QDialog::changeEvent(event);
}

void Setting::onLanguageComboxChanged()
{
    LanguageID langId = (LanguageID) ui.languageCombox->currentIndex();
    config_.setLanguage(langId);
    setLanguage(langId);
}

void Setting::onDefaultStorePathBtnClicked()
{
    QString path = QFileDialog::getExistingDirectory(
        this,
        EASYTR("Select a directory"),
        config_.defaultStorePath()
    );
    if (path.isEmpty())
        return;
    config_.setDefaultStorePath(path);
    ui.defaultStorePathEdit->setText(config_.defaultStorePath());
}

void Setting::onOpenLastFileCbChanged(bool checked)
{
    config_.setOpenLastFile(checked);
}

void Setting::onRememberPosCbChanged(bool checked)
{
    config_.setRememberPos(checked);
}

void Setting::onKeepSystemTrayCbChanged(bool checked)
{
    config_.setKeepSystemTray(checked);
}
