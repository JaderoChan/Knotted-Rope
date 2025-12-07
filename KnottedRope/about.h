#pragma once

#include <qdialog.h>
#include <qevent.h>

#include "ui_about.h"

class About : public QDialog
{
public:
    explicit About(QWidget* parent = nullptr);

protected:
    virtual void updateText();
    void changeEvent(QEvent* event) override;

private:
    Ui::About ui;
};
