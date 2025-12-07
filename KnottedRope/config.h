#pragma once

#include <set>

#include <qlist.h>
#include <qpoint.h>
#include <qstring.h>
#include <qdatetime.h>

#include "language.h"

constexpr int INVALID_INT INT_MIN;
constexpr QPoint INVALID_POS = QPoint(INVALID_INT, INVALID_INT);

class Config
{
public:
    Config();

    // 获取全局Config实例（空Config）
    static Config& globalInstance();

    // Maybe throw
    static Config fromFile(const QString& filename);
    // Maybe throw
    void toFile(const QString& filename) const;

    // Getter
    bool openLastFile() const { return openLastFile_; }
    bool rememberPos() const { return rememberPos_; }
    bool keepSystemTray() const { return keepSystemTray_; }
    LanguageID language() const { return language_; }
    const QPoint& pos() const { return pos_; }
    const QString& lastFilePath() const { return lastFilePath_; }
    const QString& defaultStorePath() const { return defaultStorePath_; }
    const QString& lastOpenedPath() const { return lastOpenedPath_; }
    const QList<QString>& recentFiles() const { return recentFiles_; }

    // Setter
    void setOpenLastFile(bool openLastFile) { openLastFile_ = openLastFile; }
    void setRememberPos(bool rememberPos) { rememberPos_ = rememberPos; }
    void setKeepSystemTray(bool keepSystemTray) { keepSystemTray_ = keepSystemTray; }
    void setLanguage(LanguageID language) { language_ = language; }
    void setPos(const QPoint& pos) { pos_ = pos; }

    // 下面的函数都会对路径进行合法性验证，如果不合法将回退为默认值。
    // 此外，他们还会将相对路径转换为绝对路径。
    void setLastFilePath(const QString& path);
    void setDefaultStorePath(const QString& path);
    void setLastOpenedPath(const QString& path);

    // Recent Files Operator
    void addRecentFile(const QString& filename);
    void removeRecentFile(const QString& filename);
    void removeRecentFile(int i);

    bool hasValidPos() const { return pos_ != INVALID_POS; }
    bool hasValidLastFilePath() const { return !lastFilePath_.isEmpty(); }

private:
    bool openLastFile_ = true;
    bool rememberPos_ = true;
    bool keepSystemTray_ = false;
    LanguageID language_ = LANG_EN;
    QPoint pos_ = INVALID_POS;
    QString lastFilePath_;
    QString defaultStorePath_;
    QString lastOpenedPath_;
    QList<QString> recentFiles_;
};
