#include "config.h"

#include <fstream>
#include <stdexcept>

#include <qdir.h>
#include <qfileinfo.h>

#include <nlohmann/json.hpp>

#include "def.h"

using namespace nlohmann;

QMutex Config::mtx;

Config::Config()
{
    setDefaultStorePath(DEFAULT_STORE_PATH);
    setLastOpenedPath("");
}

Config& Config::globalInstance()
{
    static Config instance;
    return instance;
}

template <typename T>
static QList<T> numsFromJson(const json& j, const char* key)
{
    QList<T> nums;
    if (j.contains(key) && j[key].is_array())
    {
        const auto& arr = j[key];
        for (const auto& num : arr)
            nums.append((T) num);
    }
    return nums;
}

static QList<QString> stringsFromJson(const json& j, const char* key)
{
    QList<QString> strs;
    if (j.contains(key) && j[key].is_array())
    {
        const auto& arr = j[key];
        for (const auto& str : arr)
            if (!std::string(str).empty())
                strs += QString::fromStdString(str);
    }
    return strs;
}

static QPoint pointFromJson(const json& j, const char* key)
{
    auto arr = numsFromJson<int>(j, key);
    if (arr.size() != 2)
        return INVALID_POS;
    return QPoint(arr[0], arr[1]);
}

Config Config::fromFile(const QString& filename)
{
    Config config;
    std::ifstream ifs(filename.toStdWString().c_str());
    if (!ifs.is_open())
        throw std::runtime_error(QString("[Warning] Failed to open the file: %1").arg(filename).toStdString());

    json j = json::parse(ifs, nullptr, false, true);
    if (j.is_discarded())
    {
        ifs.close();
        throw std::runtime_error("[Warning] Failed to parse the Json");
    }

    config.openLastFile_ = j.value("openLastFile", true);
    config.rememberPos_ = j.value("rememberPos", true);
    config.keepSystemTray_ = j.value("keepSystemTray", false);
    config.language_ = static_cast<LanguageID>(j.value("language", LANG_EN));
    config.pos_ = pointFromJson(j, "pos");
    config.setLastFilePath(QString::fromStdString(j.value("lastFilePath", "")));
    config.setDefaultStorePath(QString::fromStdString(j.value("defaultStorePath", DEFAULT_STORE_PATH)));
    config.setLastOpenedPath(QString::fromStdString(j.value("lastOpenedPath", "")));
    config.recentFiles_ = stringsFromJson(j, "recentFiles");

    ifs.close();
    return config;
}

template <typename T>
static json numsToJson(const QList<T>& nums)
{
    auto arr = json::array();
    for (int num : nums)
        arr += num;
    return arr;
}

static json stringsToJson(const QList<QString>& strs)
{
    auto arr = json::array();
    for (const auto& str : strs)
        arr += str.toStdString();
    return arr;
}

static json pointToJson(const QPoint& pos)
{
    return numsToJson<int>({pos.x(), pos.y()});
}

void Config::toFile(const QString& filename) const
{
    QMutexLocker<QMutex> locker(&mtx);

    json j;

    j["openLastFile"] = openLastFile_;
    j["rememberPos"] = rememberPos_;
    j["keepSystemTray"] = keepSystemTray_;
    j["language"] = static_cast<int>(language_);
    j["pos"] = pointToJson(pos_);
    j["lastFilePath"] = lastFilePath_.toStdString();
    j["defaultStorePath"] = defaultStorePath_.toStdString();
    j["lastOpenedPath"] = lastOpenedPath_.toStdString();
    j["recentFiles"] = stringsToJson(recentFiles_);

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

void Config::setLastFilePath(const QString& path)
{
    QMutexLocker<QMutex> locker(&mtx);

    QFileInfo file(path);
    if (!file.exists() || !file.isFile())
        lastFilePath_ = "";
    else
        lastFilePath_ = QDir(path).absolutePath();
}

void Config::setDefaultStorePath(const QString& path)
{
    QMutexLocker<QMutex> locker(&mtx);

    QFileInfo file(path);
    if (!file.exists() || !file.isDir())
        defaultStorePath_ = QDir(DEFAULT_STORE_PATH).absolutePath();
    else
        defaultStorePath_ = QDir(path).absolutePath();
}

void Config::setLastOpenedPath(const QString& path)
{
    QMutexLocker<QMutex> locker(&mtx);

    QFileInfo file(path);
    if (!file.exists() || !file.isDir())
        lastOpenedPath_ = QDir::currentPath();
    else
        lastOpenedPath_ = QDir(path).absolutePath();
}

void Config::addRecentFile(const QString& filename)
{
    QMutexLocker<QMutex> locker(&mtx);

    auto absFilename = QDir(filename).absolutePath();
    for (int i = 0; i < recentFiles_.size(); ++i)
        if (recentFiles_[i] == filename)
            recentFiles_.remove(i);

    recentFiles_.push_front(filename);
    if (recentFiles_.size() > RECENT_FILE_MAXNUM)
        recentFiles_.pop_back();
}

void Config::removeRecentFile(const QString& filename)
{
    QMutexLocker<QMutex> locker(&mtx);

    auto absFilename = QDir(filename).absolutePath();
    for (int i = 0; i < recentFiles_.size(); ++i)
        if (recentFiles_[i] == filename)
            recentFiles_.remove(i);
}

void Config::removeRecentFile(int i)
{
    QMutexLocker<QMutex> locker(&mtx);
    recentFiles_.remove(i);
}
