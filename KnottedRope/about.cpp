#include "about.h"

#include <easy_translate.hpp>

#include "def.h"

About::About(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.iconLbl->setPixmap(QPixmap(":/resource/icon/icon.ico"));
    ui.versionLbl->setText(KNOTTED_ROPE_VERSION);

    updateText();
}

void About::updateText()
{
    setWindowTitle(EASYTR(APP_TITLE));
    ui.titleLbl->setText(EASYTR(APP_TITLE));
    ui.authorLbl->setText(EASYTR("Author"));
    ui.repositoryUrlLbl->setText(EASYTR("Repository URL"));
}

void About::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        updateText();
    QDialog::changeEvent(event);
}
