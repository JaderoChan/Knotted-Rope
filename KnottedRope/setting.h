#pragma once

#include <qdialog.h>
#include <qevent.h>

#include "ui_setting.h"
#include "config.h"

class Setting : public QDialog
{
public:
    explicit Setting(Config& config, QWidget* parent = nullptr);

protected:
    virtual void updateText();
    void changeEvent(QEvent* event) override;

protected slots:
    void onLanguageComboxChanged();
    void onDefaultStorePathBtnClicked();
    void onOpenLastFileCbChanged(bool checked);
    void onRememberPosCbChanged(bool checked);
    void onKeepSystemTrayCbChanged(bool checked);

private:
    Ui::Setting ui;
    Config& config_;
};
