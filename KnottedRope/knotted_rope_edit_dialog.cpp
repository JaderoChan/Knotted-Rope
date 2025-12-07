#include "knotted_rope_edit_dialog.h"

#include <easy_translate.hpp>

#include "def.h"

KnottedRopeEditDialog::KnottedRopeEditDialog(KnottedRope& kr, QWidget* parent) :
    QDialog(parent), kr_(kr)
{
    ui.setupUi(this);
    ui.nameEdit->setText(kr.name);
    ui.descEdit->setText(kr.desc);
    ui.inputContentAfterRopeFinishedCb->setChecked(kr.inputContentAfterRopeFinished);

    connect(ui.confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui.cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    updateText();
}

template <typename T>
void assign(T& dest, const T& src, bool& changed)
{
    if (dest != src)
    {
        changed = true;
        dest = src;
    }
}

bool KnottedRopeEditDialog::execForEdit()
{
    int ret = exec();
    bool edited = false;
    if (ret == QDialog::Accepted)
    {
        assign(kr_.name, ui.nameEdit->text(), edited);
        assign(kr_.desc, ui.descEdit->toPlainText(), edited);
        assign(kr_.inputContentAfterRopeFinished, ui.inputContentAfterRopeFinishedCb->isChecked(), edited);
    }
    return edited;
}

void KnottedRopeEditDialog::updateText()
{
    setWindowTitle(EASYTR(APP_TITLE));
    ui.nameTextLbl->setText(EASYTR("Name"));
    ui.nameEdit->setPlaceholderText(EASYTR("No Knotted Rope Name"));
    ui.descTextLbl->setText(EASYTR("Description"));
    ui.descEdit->setPlaceholderText(EASYTR("No Description"));
    ui.inputContentAfterRopeFinishedCb->setText(EASYTR("Input the content after Rope finished"));
    ui.confirmBtn->setText(EASYTR("Confirm"));
    ui.cancelBtn->setText(EASYTR("Cancel"));
}

void KnottedRopeEditDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        updateText();
    QDialog::changeEvent(event);
}
