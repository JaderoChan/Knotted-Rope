#pragma once

#include <qdialog.h>
#include <qevent.h>

#include "ui_rope_content_dialog.h"

class RopeContentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RopeContentDialog(uint64_t elapsed, QWidget* parent = nullptr);

    QString execForText();

protected:
    virtual void updateText();
    void changeEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::RopeContentDialog ui;
};
