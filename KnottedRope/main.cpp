#include <stdexcept>

#include <qapplication.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qlockfile.h>

#include <easy_translate.hpp>

#include "config.h"
#include "def.h"
#include "language.h"
#include "mainwindow.h"
#include "system_tray.h"

int main(int argc, char* argv[])
{
    QLockFile lockfile(QDir::temp().absoluteFilePath(APP_LOCK_FILENAME));
    if (lockfile.isLocked() || !lockfile.tryLock(500))
        return 0;

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setWindowIcon(QIcon(":/resource/icon/icon.ico"));

    auto& cfg = Config::globalInstance();
    try
    {
        cfg = Config::fromFile(APP_CONFIG_FILENAME);
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

    if (!setLanguage(cfg.language()))
        qDebug() << "[Warning] Failed to load the language";

    MainWindow w;
    if (cfg.openLastFile() && cfg.hasValidLastFilePath())
        w.loadKnottedRope(cfg.lastFilePath());

    w.show();
    if (cfg.rememberPos() && cfg.hasValidPos())
        w.move(cfg.pos());

    SystemTrayIcon systemTray(&w);
    systemTray.show();
    systemTray.setToolTipByMainWindow();

    int ret = a.exec();

    easytr::updateTranslationsFiles();
    cfg.toFile(APP_CONFIG_FILENAME);

    return ret;
}
