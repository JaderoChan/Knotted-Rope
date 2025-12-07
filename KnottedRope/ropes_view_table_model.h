#pragma once

#include <qabstractitemmodel.h>

#include "knotted_rope_ds.h"

class RopesViewTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RopesViewTableModel(KnottedRope& kr, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole) override;
    // 尝试对Knotted Rope进行排序，如果给定的排序方法等同于Knotted Rope当前排序方法且Knotted Rope是已排序状态则不做任何操作。
    void trySort(SortOrder order);

private:
    KnottedRope& kr_;
    QString noContentText_;
};
