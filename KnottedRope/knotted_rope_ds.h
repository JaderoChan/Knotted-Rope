#pragma once

#include <qdatetime.h>
#include <qlist.h>
#include <qstring.h>

enum SortOrder
{
    SortByTimeAscending,
    SortByTimeDescending,
    SortByElapsedAscending,
    SortByElapsedDescending
};

class Rope
{
public:
    QDateTime startTime;
    QDateTime endTime;
    QString content;

    uint64_t elapsed() const;
};

class KnottedRope
{
public:
    KnottedRope() = default;

    // Maybe throw exception
    static KnottedRope fromFile(const QString& filename);
    // Maybe throw exception
    void toFile(const QString& filename) const;

    bool isEmpty() const;
    // 如果给定的排序方法与对象当前的排序方法不同或者对象是未排序的，则返回true，否则返回false。
    bool shouldSort(SortOrder sortOrder) const;
    void addRope(const Rope& rope);
    void sort(SortOrder sortOrder);

    QString name;
    QString desc;
    QList<Rope> ropes;
    uint64_t elapsed = 0;
    SortOrder sortOrder = SortByTimeAscending;
    bool inputContentAfterRopeFinished = true;

private:
    bool isSorted_ = false;
};
