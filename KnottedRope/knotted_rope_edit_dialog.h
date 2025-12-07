#pragma once

#include <qdialog.h>
#include <qevent.h>

#include "ui_knotted_rope_edit_dialog.h"
#include "knotted_rope_ds.h"

class KnottedRopeEditDialog : public QDialog
{
public:
    explicit KnottedRopeEditDialog(KnottedRope& kr, QWidget* parent = nullptr);

    // Return true if the knotted Rope be edited, else return false.
    bool execForEdit();

protected:
    virtual void updateText();
    void changeEvent(QEvent* event) override;

private:
    Ui::KnottedRopeEditDialog ui;
    KnottedRope& kr_;
};
