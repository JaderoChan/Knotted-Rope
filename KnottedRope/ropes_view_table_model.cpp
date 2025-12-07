#include "ropes_view_table_model.h"

#include <algorithm>

#include <qcolor.h>

#include <easy_translate.hpp>

#include "utility.h"

RopesViewTableModel::RopesViewTableModel(KnottedRope& kr, QObject* parent)
    : QAbstractTableModel(parent), kr_(kr)
{
    trySort(kr.sortOrder);
}

int RopesViewTableModel::rowCount(const QModelIndex& parent) const
{
    return (int) kr_.ropes.size();
}

int RopesViewTableModel::columnCount(const QModelIndex& parent) const
{
    return 5;
}

QVariant RopesViewTableModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    int col = index.column();
    if (!index.isValid() || row >= kr_.ropes.size() || col >= columnCount())
        return QVariant();

    auto& rope = kr_.ropes[row];
    if (role == Qt::DisplayRole)
    {
        if (col == 3 && rope.content.isEmpty())
            return EASYTR("No Content");

        switch (col)
        {
            case 0:     return row + 1;
            case 1:     return dateTime2String(rope.startTime);
            case 2:     return dateTime2String(rope.endTime);
            case 3:     return rope.content;
            case 4:     return getTimeStringFromSecs(rope.elapsed());
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch (col)
        {
            case 0: // Fallthrough
            case 1:
            case 2:
            case 4:
                return Qt::AlignCenter;
        }
    }
    else if (role == Qt::EditRole)
    {
        if (col == 3)
            return rope.content;
    }
    else if (role == Qt::ForegroundRole)
    {
        if (col == 3 && rope.content.isEmpty())
            return QColor("#969696");
    }

    return QVariant();
}

QVariant RopesViewTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || section >= columnCount())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0:     return EASYTR("No.");
            case 1:     return EASYTR("Start Time");
            case 2:     return EASYTR("End Time");
            case 3:     return EASYTR("Content");
            case 4:     return EASYTR("Elapsed");
        }
    }

    return QVariant();
}

Qt::ItemFlags RopesViewTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flag;
    int row = index.row();
    int col = index.column();
    if (!index.isValid() || row >= kr_.ropes.size() || col >= columnCount())
        return flag;

    flag = QAbstractTableModel::flags(index);
    if (col != 0)
        flag |= Qt::ItemIsSelectable;
    if (col == 3)
        flag |= Qt::ItemIsEditable;
    return flag;
}

bool RopesViewTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    int row = index.row();
    int col = index.column();
    if (!index.isValid() || row >= kr_.ropes.size() || col >= columnCount())
        return false;
    if (role != Qt::DisplayRole)
        return false;

    auto& rope = kr_.ropes[row];
    if (col == 3)
    {
        rope.content = value.toString();
        emit dataChanged(index, index, {Qt::DisplayRole});
        return true;
    }

    return false;
}

void RopesViewTableModel::trySort(SortOrder order)
{
    if (kr_.shouldSort(order))
    {
        emit layoutAboutToBeChanged();
        kr_.sort(order);
        emit layoutChanged();

        if (!kr_.ropes.empty())
        {
            QModelIndex topLeft = createIndex(0, 0);
            QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
            emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
        }
    }
}
