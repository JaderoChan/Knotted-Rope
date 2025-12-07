#include "ok_cancel_dialog.h"

#include <easy_translate.hpp>

#include "def.h"

OkCancelDialog::OkCancelDialog(const QString& text, QWidget* parent)
    : QDialog(parent), text_(text)
{
    ui.setupUi(this);

    connect(ui.okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui.cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    updateText();
}

QString OkCancelDialog::text() const
{
    return text_;
}

void OkCancelDialog::setText(const QString& text)
{
    text_ = text;
    ui.textLbl->setText(EASYTR(text_.toStdString()));
}

void OkCancelDialog::updateText()
{
    setWindowTitle(EASYTR(APP_TITLE));
    ui.titleLbl->setText(EASYTR("Warning"));
    ui.okBtn->setText(EASYTR("OK"));
    ui.cancelBtn->setText(EASYTR("Cancel"));
    ui.textLbl->setText(EASYTR(text_.toStdString()));
}

void OkCancelDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        updateText();
    QDialog::changeEvent(event);
}
