#pragma once

#include <qstyleditemdelegate.h>
#include <qevent.h>

class RopesViewContentEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit RopesViewContentEditDelegate(QObject* parent = nullptr);

    QWidget* createEditor(
        QWidget* parent,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(
        QWidget* editor,
        QAbstractItemModel* model,
        const QModelIndex& index) const override;
    void updateEditorGeometry(
        QWidget* editor,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};
