#include "knotted_rope_ds.h"

#include <fstream>
#include <stdexcept>

#include <qdir.h>
#include <qfileinfo.h>

#include <nlohmann/json.hpp>

#include "utility.h"

using json = nlohmann::json;

uint64_t Rope::elapsed() const
{
    return startTime.secsTo(endTime);
}

static Rope ropeFromJson(const json& j)
{
    Rope rope;
    if (j.contains("StartTime") && j["StartTime"].is_string() &&
        j.contains("EndTime") && j["EndTime"].is_string())
    {
        rope.startTime = string2DateTime(QString::fromStdString(j["StartTime"]));
        rope.endTime = string2DateTime(QString::fromStdString(j["EndTime"]));
        if (j.contains("Content") && j["Content"].is_string())
            rope.content = QString::fromStdString(j["Content"]);
    }
    else
    {
        throw std::runtime_error("[Warning] Obtained illegal data when try get the Rope from Json");
    }
    return rope;
}

KnottedRope KnottedRope::fromFile(const QString& filename)
{
    KnottedRope kr;
    std::ifstream ifs(filename.toStdWString().c_str());
    if (!ifs.is_open())
        throw std::runtime_error(QString("[Warning] Failed to open the file: %1").arg(filename).toStdString());

    json j = json::parse(ifs, nullptr, false, true);
    if (j.is_discarded())
    {
        ifs.close();
        throw std::runtime_error("[Warning] Failed to parse the Json");
    }

    kr.name = QString::fromStdString(j.value("KnottedRopeName", ""));
    kr.desc = QString::fromStdString(j.value("Description", ""));
    kr.sortOrder = (SortOrder)(j.value("SortOrder", SortByTimeAscending));
    kr.inputContentAfterRopeFinished = j.value("IsInputContentAfterRopeFinished", true);
    kr.isSorted_ = j.value("IsSorted", false);
    if (j.contains("Ropes") && j["Ropes"].is_array())
    {
        const auto& list = j["Ropes"];
        for (const auto& obj : list)
        {
            try
            {
                kr.addRope(ropeFromJson(obj));
            }
            catch (std::exception& e)
            {
                qDebug() << e.what();
            }
        }
    }

    ifs.close();
    return kr;
}

static json ropeToJson(const Rope& rope)
{
    json j;
    j["StartTime"] = dateTime2String(rope.startTime).toStdString();
    j["EndTime"] = dateTime2String(rope.endTime).toStdString();
    if (!rope.content.isEmpty())
        j["Content"] = rope.content.toStdString();
    return j;
}

void KnottedRope::toFile(const QString& filename) const
{
    json j;

    if (!name.isEmpty())
        j["KnottedRopeName"] = name.toStdString();
    if (!desc.isEmpty())
        j["Description"] = desc.toStdString();
    j["SortOrder"] = sortOrder;
    j["IsInputContentAfterRopeFinished"] = inputContentAfterRopeFinished;
    j["IsSorted"] = isSorted_;

    auto arr = json::array();
    for (const auto& rope : ropes)
        arr += ropeToJson(rope);
    j["Ropes"] = arr;

    QFileInfo fileInfo(filename);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists())
    {
        qDebug() << "[Info] The file parent path is not exists, will make path: " << dir.path();
        if (!dir.mkpath("."))
            throw std::runtime_error(QString("[Warning] Failed to make path: %1").arg(dir.path()).toStdString());
    }

    std::ofstream ofs(filename.toStdWString().c_str());
    if (!ofs.is_open())
        throw std::runtime_error(QString("[Warning] Failed to open the file: %1").arg(filename).toStdString());

    ofs << j.dump(4);
    ofs.close();
}

bool KnottedRope::isEmpty() const
{
    return name.isEmpty() && desc.isEmpty() && ropes.empty();
}

bool KnottedRope::shouldSort(SortOrder sortOrder) const
{
    return this->sortOrder != sortOrder || !isSorted_;
}

void KnottedRope::addRope(const Rope& rope)
{
    ropes.append(rope);
    elapsed += rope.elapsed();
    isSorted_ = false;
}

void KnottedRope::sort(SortOrder sortOrder)
{
    std::sort(ropes.begin(), ropes.end(), [=](const Rope& lhs, const Rope& rhs)
    {
        switch (sortOrder)
        {
            case SortByTimeAscending:       return lhs.startTime < rhs.startTime;
            case SortByTimeDescending:      return lhs.startTime > rhs.startTime;
            case SortByElapsedAscending:    return lhs.elapsed() < rhs.elapsed();
            case SortByElapsedDescending:   return lhs.elapsed() > rhs.elapsed();
            default:                        return true;
        }
    });
    isSorted_ = true;
}
