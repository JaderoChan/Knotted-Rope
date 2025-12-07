#include "ropes_view_dialog.h"

#include <algorithm>

#include <qapplication.h>
#include <qmenu.h>
#include <qclipboard.h>

#include <easy_translate.hpp>

#include "utility.h"
#include "def.h"
#include "ropes_view_content_edit_delegate.h"

RopesViewDialog::RopesViewDialog(KnottedRope& kr, QWidget* parent)
    : QDialog(parent), kr_(kr)
{
    ui.setupUi(this);

    ui.knottedRopeNameLbl->setText(kr_.name);
    ui.totalElapsedTimeLbl->setText(getTimeStringFromSecs(kr_.elapsed));
    ui.ropeCountLbl->setText(QString::number(kr_.ropes.size()));
    ui.sortMethodComBox->setCurrentIndex(kr_.sortOrder);
    ui.knottedRopeNameLbl->installEventFilter(this);

    auto model = new RopesViewTableModel(kr_, this);
    ui.tableView->setModel(model);
    ui.tableView->setItemDelegate(new RopesViewContentEditDelegate(this));
    ui.tableView->verticalHeader()->setMinimumSectionSize(35);
    ui.tableView->verticalHeader()->setSectionsClickable(false);
    ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableView->horizontalHeader()->setSectionsClickable(false);
    ui.tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui.tableView->setColumnWidth(0, 40);
    ui.tableView->setColumnWidth(1, 140);
    ui.tableView->setColumnWidth(2, 140);
    ui.tableView->setColumnWidth(4, 80);

    connect(ui.sortMethodComBox, &QComboBox::currentIndexChanged, this, [=](int index)
    {
        auto sortOrder = sortMethodIndexToSortOrder_(index);
        (qobject_cast<RopesViewTableModel*>(ui.tableView->model()))->trySort(sortOrder);
        kr_.sortOrder = sortOrder;
    });

    connect(model, &RopesViewTableModel::dataChanged, this, [=]() { emit ropesChanged(); });
    connect(ui.tableView, &QTableView::customContextMenuRequested, this, &RopesViewDialog::onCustomContextMenu);

    updateText();
}

void RopesViewDialog::updateText()
{
    setWindowTitle(EASYTR(APP_TITLE));
    ui.knottedRopeNameLbl->setPlaceholderText(EASYTR("No Knotted Rope Name"));
    ui.ropeTextLbl->setText(EASYTR("Rope"));
    ui.sortMethodComBox->setToolTip(EASYTR("Sort Method"));
    ui.sortMethodComBox->setItemText(0, EASYTR("Ascending By Time"));
    ui.sortMethodComBox->setItemText(1, EASYTR("Descending By Time"));
    ui.sortMethodComBox->setItemText(2, EASYTR("Ascending By Elapsed"));
    ui.sortMethodComBox->setItemText(3, EASYTR("Descending By Elapsed"));
}

void RopesViewDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        updateText();
    QDialog::changeEvent(event);
}

bool RopesViewDialog::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseMove ||
        event->type() == QEvent::ContextMenu)
        return true;
    return QDialog::eventFilter(obj, event);
}

void RopesViewDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    ui.tableView->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void RopesViewDialog::onCustomContextMenu(const QPoint& pos)
{
    auto* tableView = qobject_cast<QTableView*>(sender());
    if (!tableView)
        return;

    QModelIndex index = tableView->indexAt(pos);
    if (!index.isValid() || index.column() == 0)
        return;

    auto* menu = new QMenu(this);
    menu->addAction(EASYTR("Copy"), [=]()
    {
        QString text = index.data().toString();
        qApp->clipboard()->setText(text);
    });

    menu->exec(tableView->viewport()->mapToGlobal(pos));
    menu->deleteLater();
}

SortOrder RopesViewDialog::sortMethodIndexToSortOrder_(int index)
{
    switch (index)
    {
        case 0:     return SortByTimeAscending;
        case 1:     return SortByTimeDescending;
        case 2:     return SortByElapsedAscending;
        case 3:     return SortByElapsedDescending;
        default:    return SortByTimeAscending;
    }
}
