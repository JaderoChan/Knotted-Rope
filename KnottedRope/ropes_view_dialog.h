#pragma once

#include <qdialog.h>
#include <qevent.h>

#include "ui_ropes_view_dialog.h"
#include "knotted_rope_ds.h"
#include "ropes_view_table_model.h"

class RopesViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RopesViewDialog(KnottedRope& kr, QWidget* parent = nullptr);

signals:
    void ropesChanged();

protected:
    virtual void updateText();

    void changeEvent(QEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

    void onCustomContextMenu(const QPoint& pos);

private:
    static SortOrder sortMethodIndexToSortOrder_(int index);

    Ui::RopesViewDialog ui;
    KnottedRope& kr_;
};
