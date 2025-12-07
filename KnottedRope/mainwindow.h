#pragma once

#include <qmainwindow.h>
#include <qtimer.h>
#include <qevent.h>

#include "knotted_rope_ds.h"
#include "ui_mainwindow.h"

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    void loadKnottedRope(const KnottedRope& knottedRope);
    void loadKnottedRope(const QString& filename);
    void saveKnottedRope(const QString& filename) const;
    void saveKnottedRope();

    const QString& knottedRopeName() const;
    const QString& knottedRopeFilename() const;
    uint64_t elapsed() const;

    void toggle();
    // 如果成功关闭返回true，否则返回false（当用户中止close操作时）。
    bool closeKnottedRope();
    // 尝试退出程序。
    void tryExitApp();

signals:
    // 当计时状态切换时发出。
    void toggled(bool isPlaying);
    // 当KnottedRope对象不变，其数据发生变化时发出。
    void knottedRopeUpdated();
    // 当KnottedRope对象变更时发出。
    void knottedRopeLoaded();
    // 当KnottedRope对象变更或数据发生变化时均发出。
    void knottedRopeChaged();
    // 当计时时长改变时发出。
    void elapsedChanged(uint64_t secs);
    // 但语言发生改变时发出。
    void languageChanged();

protected:
    virtual void updateText();
    void changeEvent(QEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void focusEditName();
    void unfocusEditName();

protected slots:
    void onTimerTimeOut();
    void onKnottedRopeChanged();
    void onRecentFilesChanged();

    // UI
    void onCopyTotalElapsedBtnPressed();

    // File Menu Action
    void onNewActived();
    void onOpenActived();
    void onSaveActived();
    void onSaveAsActived();
    void onExportCSVActived();

    // Settings Menu Action
    void onSettingActived();
    void onAboutActived();

    // Menu Menu Action
    void onViewRopesActived();

    void onEdit();

private:
    void setKnottedRopeFilename_(const QString& filename);
    // 弹窗询问用户操作是否中止并保存当前计时。
    // 如果确认中止并保存返回true，否则返回false。
    bool dialogACK_();

    Ui::MainWindow ui;
    QTimer timer_;
    KnottedRope kr_;
    QString krFileName_;
    QDateTime startTime_;
    bool isPlaying_ = false;
};
