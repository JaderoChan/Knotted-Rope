#include "ropes_view_content_edit_delegate.h"

#include <qtextedit.h>

#include <easy_translate.hpp>

RopesViewContentEditDelegate::RopesViewContentEditDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{}

QWidget* RopesViewContentEditDelegate::createEditor(
    QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QTextEdit* editor = new QTextEdit(parent);
    editor->setFrameStyle(QFrame::NoFrame);
    editor->setPlaceholderText(EASYTR("No Content"));
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->installEventFilter(const_cast<RopesViewContentEditDelegate*>(this));
    return editor;
}

void RopesViewContentEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    auto obj = qobject_cast<QTextEdit*>(editor);
    if (obj)
        obj->setPlainText(index.data(Qt::EditRole).toString());
    obj->moveCursor(QTextCursor::End);
}

void RopesViewContentEditDelegate::setModelData(
    QWidget* editor,
    QAbstractItemModel* model,
    const QModelIndex& index) const
{
    auto obj = qobject_cast<QTextEdit*>(editor);
    if (obj)
        model->setData(index, obj->document()->toPlainText(), Qt::DisplayRole);
}

void RopesViewContentEditDelegate::updateEditorGeometry(
    QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

bool RopesViewContentEditDelegate::eventFilter(QObject* obj, QEvent* event)
{
    auto editor = qobject_cast<QTextEdit*>(obj);
    if (!editor)
        return QStyledItemDelegate::eventFilter(obj, event);

    switch (event->type())
    {
        case QEvent::Wheel:
        {
            auto e = static_cast<QWheelEvent*>(event);
            if (editor->rect().contains(e->position().toPoint()))
            {
                e->accept();
                return true;
            }
            break;
        }
        // 按下 Ctrl+Enter 时提交数据。
        case QEvent::KeyPress:
        {
            auto e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
            {
                if (e->modifiers() & Qt::ControlModifier)
                {
                    emit commitData(editor);
                    emit closeEditor(editor);
                    return true;
                }
            }
            break;
        }
        case QEvent::FocusOut:
        {
            emit commitData(editor);
            emit closeEditor(editor);
            return true;
            break;
        }
    }

    return QStyledItemDelegate::eventFilter(obj, event);
}
