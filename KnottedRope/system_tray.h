#pragma once

#include <qsystemtrayicon.h>
#include <qmenu.h>
#include <qaction.h>

#include "mainwindow.h"

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit SystemTrayIcon(MainWindow* mainWindow, QObject* parent = nullptr);

    void setToolTipByMainWindow();

protected:
    virtual void updateText();

protected slots:
    void onActivated(ActivationReason reason);
    void onToggled(bool isPlaying);

private:

    QMenu* menu_;
    QAction* showMainWindow_;
    QAction* toggle_;
    QAction* exitApp_;
    MainWindow* mainWindow_;
    QIcon playIcon_;
    QIcon pauseIcon_;
    bool isPlaying_ = false;
};
