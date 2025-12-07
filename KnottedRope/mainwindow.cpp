#include "mainwindow.h"

#include <stdexcept>

#include <qclipboard.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include <easy_translate.hpp>

#include "config.h"
#include "def.h"
#include "export_to_csv.h"
#include "utility.h"

#include "ok_cancel_dialog.h"
#include "rope_content_dialog.h"
#include "knotted_rope_edit_dialog.h"
#include "ropes_view_dialog.h"
#include "setting.h"
#include "about.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 为name编辑框增加当ESC按下时完成编辑的功能。
    ui.nameLbl->installEventFilter(this);
    ui.editBtn->setIcon(QIcon(":/resource/image/edit.png"));
    ui.playPauseBtn->setIcon(QIcon(":/resource/image/play.png"));
    ui.copyTotalElapsedBtn->setIcon(QIcon(":/resource/image/copy_text.png"));

    timer_.setInterval(100);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::onTimerTimeOut);

    connect(ui.playPauseBtn, &QPushButton::clicked, this, &MainWindow::toggle);
    connect(ui.editBtn, &QPushButton::clicked, this, &MainWindow::onEdit);
    connect(ui.nameLbl, &QLineEdit::editingFinished, this, &MainWindow::unfocusEditName);
    connect(ui.copyTotalElapsedBtn, &QPushButton::clicked, this, &MainWindow::onCopyTotalElapsedBtnPressed);

    connect(ui.actionNew, &QAction::triggered, this, &MainWindow::onNewActived);
    connect(ui.actionOpen, &QAction::triggered, this, &MainWindow::onOpenActived);
    connect(ui.actionSave, &QAction::triggered, this, &MainWindow::onSaveActived);
    connect(ui.actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsActived);
    connect(ui.actionExportCSV, &QAction::triggered, this, &MainWindow::onExportCSVActived);
    connect(ui.actionSetting, &QAction::triggered, this, &MainWindow::onSettingActived);
    connect(ui.actionAbout, &QAction::triggered, this, &MainWindow::onAboutActived);

    connect(ui.actionEdit, &QAction::triggered, this, &MainWindow::onEdit);
    connect(ui.actionEditName, &QAction::triggered, this, &MainWindow::focusEditName);

    connect(ui.actionViewRopes, &QAction::triggered, this, &MainWindow::onViewRopesActived);

    connect(this, &MainWindow::knottedRopeLoaded, this, [=]()
    {
        emit knottedRopeChaged();
    });
    connect(this, &MainWindow::knottedRopeUpdated, this, [=]()
    {
        emit knottedRopeChaged();
        saveKnottedRope();
    });
    connect(this, &MainWindow::knottedRopeChaged, this, &MainWindow::onKnottedRopeChanged);

    onRecentFilesChanged();
    updateText();
}

void MainWindow::loadKnottedRope(const KnottedRope& kr)
{
    kr_ = kr;
    emit knottedRopeLoaded();
}

void MainWindow::loadKnottedRope(const QString& filename)
{
    try
    {
        KnottedRope kr = KnottedRope::fromFile(filename);
        loadKnottedRope(kr);
        setKnottedRopeFilename_(filename);
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}

void MainWindow::saveKnottedRope(const QString& filename) const
{
    try
    {
        kr_.toFile(filename);
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}

void MainWindow::saveKnottedRope()
{
    QString filename = krFileName_;
    if (filename.isEmpty())
    {
        if (kr_.name.isEmpty())
            filename = DEFAULT_FILENAME_PREFIX +
                dateTime2StringForFilename(QDateTime::currentDateTime());
        else
            filename = kr_.name;
        filename += APP_ASSOCIATED_FILEEXT;
    }

    filename = QDir(Config::globalInstance().defaultStorePath()).filePath(filename);
    saveKnottedRope(filename);
    setKnottedRopeFilename_(filename);
}

const QString& MainWindow::knottedRopeName() const
{
    return kr_.name;
}

const QString& MainWindow::knottedRopeFilename() const
{
    return krFileName_;
}

uint64_t MainWindow::elapsed() const
{
    return startTime_.secsTo(QDateTime::currentDateTime());
}

void MainWindow::toggle()
{
    if (isPlaying_)
    {
        timer_.stop();
        QDateTime endTime = QDateTime::currentDateTime();
        ui.playPauseBtn->setIcon(QIcon(":/resource/image/play"));

        if (startTime_.secsTo(endTime) > 0)
        {
            Rope rope;
            rope.startTime = startTime_;
            rope.endTime = endTime;

            QString content;
            if (kr_.inputContentAfterRopeFinished)
            {
                RopeContentDialog dlg(rope.elapsed());
                content = dlg.execForText();
            }
            rope.content = content;

            kr_.addRope(rope);
            emit knottedRopeUpdated();
        }
    }
    else
    {
        timer_.start();
        startTime_ = QDateTime::currentDateTime();
        ui.playPauseBtn->setIcon(QIcon(":/resource/image/pause"));
    }

    isPlaying_ = !isPlaying_;
    emit toggled(isPlaying_);
}

bool MainWindow::closeKnottedRope()
{
    // 如果计时器处于活动状态则弹窗询问用户。
    if (isPlaying_)
    {
        if (dialogACK_()) toggle();
        else return false;
    }

    // 尝试保存关闭的Knotted Rope。
    if (!kr_.isEmpty() || !krFileName_.isEmpty())
        saveKnottedRope();
    krFileName_.clear();

    return true;
}

void MainWindow::tryExitApp()
{
    if (closeKnottedRope())
    {
        Config::globalInstance().setPos(pos());
        qApp->quit();
    }
}

void MainWindow::updateText()
{
    // Window
    setWindowTitle(EASYTR(APP_TITLE));

    // Menu
    ui.menuFile->setTitle(EASYTR("File"));
    ui.menuEdit->setTitle(EASYTR("Edit"));
    ui.menuMenu->setTitle(EASYTR("Menu"));
    ui.menuSettings->setTitle(EASYTR("Settings"));

    // File Menu Action
    ui.actionNew->setText(EASYTR("New"));
    ui.actionOpen->setText(EASYTR("Open"));
    ui.menuOpenRecentFile->setTitle(EASYTR("Open Recent File"));
    ui.actionSave->setText(EASYTR("Save"));
    ui.actionSaveAs->setText(EASYTR("Save As"));
    ui.actionExportCSV->setText(EASYTR("Export To CSV"));
    ui.actionSetting->setText(EASYTR("Setting"));
    ui.actionAbout->setText(EASYTR("About"));

    // Edit Menu Action
    ui.actionEdit->setText(EASYTR("Edit"));
    ui.actionEditName->setText(EASYTR("Edit Knotted Rope Name"));

    // Menu Menu Action
    ui.actionViewRopes->setText(EASYTR("View Ropes"));

    // UI
    ui.nameLbl->setPlaceholderText(EASYTR("No Knotted Rope Name"));
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        updateText();
        emit languageChanged();
    }
    QMainWindow::changeEvent(event);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseMove ||
        event->type() == QEvent::ContextMenu)
        return true;
    if (event->type() == QEvent::KeyPress)
    {
        auto e = dynamic_cast<QKeyEvent*>(event);
        if (e != nullptr)
        {
            if (e->key() == Qt::Key_Escape && e->modifiers() == Qt::KeyboardModifier::NoModifier)
                unfocusEditName();
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (Config::globalInstance().keepSystemTray())
        hide();
    else
        tryExitApp();
    event->ignore();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space && event->modifiers() == Qt::KeyboardModifier::NoModifier)
    {
        toggle();
        event->accept();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::focusEditName()
{
    ui.nameLbl->setReadOnly(false);
    ui.nameLbl->setFocus();
    ui.nameLbl->end(false);
}

void MainWindow::unfocusEditName()
{
    ui.nameLbl->setReadOnly(true);
    ui.nameLbl->clearFocus();
    kr_.name = ui.nameLbl->text();
    emit knottedRopeUpdated();
}

void MainWindow::onTimerTimeOut()
{
    auto secs = elapsed();
    emit elapsedChanged(secs);
    ui.currentElapsedLbl->setText(getTimeStringFromSecs(secs));
}

void MainWindow::onKnottedRopeChanged()
{
    ui.nameLbl->setText(kr_.name);
    ui.nameLbl->setToolTip(kr_.desc);
    ui.totalElapsedLbl->setText(getTimeStringFromSecs(kr_.elapsed));
    ui.currentElapsedLbl->setText(getTimeStringFromSecs(0));
}

void MainWindow::onRecentFilesChanged()
{
    ui.menuOpenRecentFile->clear();
    for (const auto& filename : Config::globalInstance().recentFiles())
    {
        if (!filename.isEmpty())
        {
            QAction* action = new QAction(filename);
            ui.menuOpenRecentFile->addAction(action);
            connect(action, &QAction::triggered, [=](){
                QFileInfo file(filename);
                if (!file.exists() || !file.isFile())
                {
                    QMessageBox msgbox(
                        QMessageBox::Icon::Warning,
                        EASYTR("Warning"),
                        EASYTR("The file is not exists.")
                    );
                    msgbox.exec();
                    Config::globalInstance().removeRecentFile(filename);
                    ui.menuOpenRecentFile->removeAction(action);
                    action->deleteLater();
                }
                else
                {
                    loadKnottedRope(filename);
                }
            });
        }
    }
}

void MainWindow::onNewActived()
{
    if (closeKnottedRope())
    {
        Config::globalInstance().setLastFilePath("");
        loadKnottedRope(KnottedRope());
    }
}

void MainWindow::onOpenActived()
{
    if (closeKnottedRope())
    {
        QString filename = QFileDialog::getOpenFileName(
            this,
            EASYTR("Open a Knotted Rope file"),
            Config::globalInstance().lastOpenedPath(),
            QString("Knotted Rope File (*%1)").arg(APP_ASSOCIATED_FILEEXT)
        );
        if (filename.isEmpty())
            return;

        auto dir = QFileInfo(filename).absoluteDir();
        Config::globalInstance().setLastOpenedPath(dir.path());
        loadKnottedRope(filename);
    }
}

void MainWindow::onSaveActived()
{
    if (isPlaying_)
    {
        if (dialogACK_()) toggle();
        else return;
    }
    saveKnottedRope();
}

void MainWindow::onSaveAsActived()
{
    if (isPlaying_)
    {
        if (dialogACK_()) toggle();
        else return;
    }

    QString defaultName = DEFAULT_FILENAME_PREFIX;
    if (!kr_.name.isEmpty())
        defaultName = defaultName;
    defaultName = QDir(Config::globalInstance().defaultStorePath()).filePath(DEFAULT_FILENAME_PREFIX);
    defaultName += APP_ASSOCIATED_FILEEXT;
    QString filename = QFileDialog::getSaveFileName(
        this,
        EASYTR("Save the Knotted Rope file"),
        defaultName,
        QString("Knotted Rope File (*%1)").arg(APP_ASSOCIATED_FILEEXT)
    );
    if (filename.isEmpty())
        return;

    saveKnottedRope(filename);
}

void MainWindow::onExportCSVActived()
{
    if (isPlaying_)
    {
        if (dialogACK_()) toggle();
        else return;
    }

    QString defaultName = DEFAULT_FILENAME_PREFIX;
    if (!kr_.name.isEmpty())
        defaultName = defaultName;
    defaultName = QDir(Config::globalInstance().defaultStorePath()).filePath(DEFAULT_FILENAME_PREFIX);
    defaultName += ".csv";
    QString filename = QFileDialog::getSaveFileName(
        this,
        EASYTR("Export the Knotted Rope to CSV file"),
        defaultName,
        QString("CSV File (*%1)").arg(".csv")
    );
    if (filename.isEmpty())
        return;

    if (kr_.shouldSort(kr_.sortOrder))
        kr_.sort(kr_.sortOrder);
    try
    {
        exportKnottedRopeToCsv(kr_, filename);
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}

void MainWindow::onSettingActived()
{
    Setting setting(Config::globalInstance());
    setting.exec();
}

void MainWindow::onAboutActived()
{
    About about;
    about.exec();
}

void MainWindow::onViewRopesActived()
{
    RopesViewDialog dlg(kr_);
    connect(&dlg, &RopesViewDialog::ropesChanged, this, [=]() { emit knottedRopeUpdated(); });
    dlg.exec();
}

void MainWindow::onEdit()
{
    KnottedRopeEditDialog dlg(kr_);
    bool edited = dlg.execForEdit();
    if (edited)
        emit knottedRopeUpdated();
}

void MainWindow::setKnottedRopeFilename_(const QString& filename)
{
    krFileName_ = filename;
    Config::globalInstance().setLastFilePath(krFileName_);
    Config::globalInstance().addRecentFile(krFileName_);
    onRecentFilesChanged();
}

void MainWindow::onCopyTotalElapsedBtnPressed()
{
    qApp->clipboard()->setText(ui.totalElapsedLbl->text());
}

bool MainWindow::dialogACK_()
{
    OkCancelDialog dlg("Whether pause and save current Rope?");
    return dlg.exec() == QDialog::Accepted;
}
