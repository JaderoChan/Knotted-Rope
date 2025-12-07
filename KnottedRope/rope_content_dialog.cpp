#include "rope_content_dialog.h"

#include <easy_translate.hpp>

#include "def.h"
#include "utility.h"

RopeContentDialog::RopeContentDialog(uint64_t elapsed, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.elapsedLbl->setText(getTimeStringFromSecs(elapsed));
    updateText();
}

QString RopeContentDialog::execForText()
{
    int ret = exec();
    // 忽略结果，始终返回文本内容。
    return ui.textEdit->document()->toPlainText();
}

void RopeContentDialog::updateText()
{
    setWindowTitle(EASYTR(APP_TITLE));
    ui.titleLbl->setText(EASYTR("Input the work content of this time"));
}

void RopeContentDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        updateText();
    QDialog::changeEvent(event);
}

void RopeContentDialog::keyPressEvent(QKeyEvent* event)
{
    // 按下 Ctrl+Enter 时视为确认操作。
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            accept();
        }
    }
    QDialog::keyPressEvent(event);
}
