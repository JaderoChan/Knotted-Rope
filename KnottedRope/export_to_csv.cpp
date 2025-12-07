#include "export_to_csv.h"

#include <stdexcept>

#include <qdir.h>
#include <qfileinfo.h>

#include <rapidcsv.h>
#include <easy_translate.hpp>

#include "utility.h"

static std::string utf8ToLocal(const char* str)
{
    return QString::fromUtf8(str).toLocal8Bit().toStdString();
}

// 表头顺序：起始时间-终止时间-工作内容-时长
void exportKnottedRopeToCsv(const KnottedRope& kr, const QString& filename)
{
    QFileInfo fileInfo(filename);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists())
    {
        qDebug() << "[Info] The file parent path is not exists, will make path: " << dir.path();
        if (!dir.mkpath("."))
            throw std::runtime_error(QString("[Warning] Failed to make path: %1").arg(dir.path()).toStdString());
    }

    std::ofstream ofs(filename.toStdWString().c_str(), std::ios_base::binary);
    if (!ofs.is_open())
        throw std::runtime_error(QString("[Warning] Failed to open the file: %1").arg(filename).toStdString());

    rapidcsv::Document doc;
    doc.SetColumnName(0, utf8ToLocal(EASYTR("Start Time")));
    doc.SetColumnName(1, utf8ToLocal(EASYTR("End Time")));
    doc.SetColumnName(2, utf8ToLocal(EASYTR("Content")));
    doc.SetColumnName(3, utf8ToLocal(EASYTR("Elapsed")));

    int index = 0;
    for (const auto& rope : kr.ropes)
    {
        auto startTimeStr = dateTime2String(rope.startTime).toStdString();
        auto endTimeStr = dateTime2String(rope.endTime).toStdString();
        auto elapsedStr = getTimeStringFromSecs(rope.startTime.secsTo(rope.endTime)).toStdString();
        auto content = rope.content.toLocal8Bit().toStdString();
        std::vector<std::string> row = {startTimeStr, endTimeStr, content, elapsedStr};
        doc.SetRow(index++, row);
    }

    // 在最后一行的时长列加上总时长信息
    doc.SetCell(3, index, getTimeStringFromSecs(kr.elapsed).toStdString());

    doc.Save(ofs);
    ofs.close();
}
