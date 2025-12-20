#include "system_tray.h"

#include <easy_translate.hpp>

SystemTrayIcon::SystemTrayIcon(MainWindow* mainWindow, QObject* parent)
    : QSystemTrayIcon(parent), mainWindow_(mainWindow),
    playIcon_(QIcon(":/resource/image/play")), pauseIcon_(QIcon(":/resource/image/pause"))
{
    setIcon(QIcon(":/resource/icon/icon.ico"));

    showMainWindow_ = new QAction();
    toggle_ = new QAction();
    exitApp_ = new QAction();
    toggle_->setIcon(isPlaying_ ? pauseIcon_ : playIcon_);

    menu_ = new QMenu();
    menu_->addActions({showMainWindow_, toggle_, exitApp_});
    setContextMenu(menu_);

    connect(this, &SystemTrayIcon::activated, this, &SystemTrayIcon::onActivated);

    connect(mainWindow_, &MainWindow::toggled, this, &SystemTrayIcon::onToggled);
    connect(mainWindow_, &MainWindow::knottedRopeChaged, this, &SystemTrayIcon::setToolTipByMainWindow);
    connect(mainWindow_, &MainWindow::languageChanged, this, &SystemTrayIcon::updateText);

    connect(showMainWindow_, &QAction::triggered, mainWindow_, &MainWindow::showAndActivate);
    connect(toggle_, &QAction::triggered, mainWindow_, &MainWindow::toggle);
    connect(exitApp_, &QAction::triggered, mainWindow_, &MainWindow::tryExitApp);

    updateText();
}

void SystemTrayIcon::updateText()
{
    showMainWindow_->setText(EASYTR("Show Main Window"));
    toggle_->setText(isPlaying_ ? EASYTR("Pause") : EASYTR("Play"));
    exitApp_->setText(EASYTR("Exit"));
}

void SystemTrayIcon::onActivated(ActivationReason reason)
{
    switch (reason)
    {
        case Trigger:   // Fallthrough
        case Context:
            contextMenu()->popup(QCursor::pos());
            break;
        case DoubleClick:
            mainWindow_->showAndActivate();
            break;
        default:
            break;
    }
}

void SystemTrayIcon::onToggled(bool isPlaying)
{
    isPlaying_ = isPlaying;
    toggle_->setIcon(isPlaying_ ? pauseIcon_ : playIcon_);
    toggle_->setText(isPlaying_ ? EASYTR("Pause") : EASYTR("Play"));
}

void SystemTrayIcon::setToolTipByMainWindow()
{
    QString krName = mainWindow_->knottedRopeName();
    if (krName.isEmpty())
        krName = EASYTR("No Knotted Rope Name");
    setToolTip(krName);
}
