#pragma once

#include <qdialog.h>
#include <qevent.h>

#include "ui_ok_cancel_dialog.h"

class OkCancelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OkCancelDialog(const QString& text, QWidget* parent = nullptr);

    QString text() const;
    void setText(const QString& text);

protected:
    virtual void updateText();
    void changeEvent(QEvent* event) override;

private:
    Ui::OkCancelDialog ui;
    QString text_;
};
