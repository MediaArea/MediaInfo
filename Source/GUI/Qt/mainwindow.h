/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Common/Core.h"
#include <QSettings>
#include <QDir>
#include <QTreeWidget>
#include <QTextBrowser>
#include <QProgressDialog>
#include <QTimer>
#include "views.h"
#include <QDebug>
#include <QUrl>
#ifdef NEW_VERSION
    #include <QNetworkAccessManager>
    #include <QNetworkReply>
    #include <QTemporaryFile>
#endif //NEW_VERSION

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum {IONLY,TONLY,TUICONS,TBICONS,NBNAMES} ActionName;
    enum {SMALL,MEDIUM,BIG,HUGE,NBSIZES} Size;
    MainWindow(QStringList filesnames, int viewasked=-1, QWidget *parent = 0);
    ~MainWindow();

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    static QString shortName(QDir d, QString name);
    static QString shortName(Core*C, QString name);
    static QDir getCommonDir(Core*C);
#ifdef NEW_VERSION
    static bool isNewer(QString distant, QString local);
    void checkForNewVersion();
#endif //NEW_VERSION



protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    void defaultSettings();
    void applySettings();
    QString  mediaInfoTest(QString filename);
    void refreshDisplay();
    void openFiles(QStringList fileNames);
    void openDir(QString dirName);
    QTreeWidget* showTreeView(bool completeDisplay);
    QTextBrowser* showCustomView(bool forcePlainText=false);

    Ui::MainWindow *ui;
    //Non-GUI Elements
#ifdef NEW_VERSION
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QString file;
#endif //NEW_VERSION
    Core* C;
    ViewMode view;
    QSettings* settings;
    QProgressDialog* progressDialog;
    QTimer* timer;
    void openTimerInit ();

private slots:
    void updateProgressBar ();
    void on_actionClose_All_triggered();
    void on_actionAdvanced_Mode_toggled(bool );
    void on_actionExport_triggered();
    void on_actionPreferences_triggered();
    void on_actionKnown_parameters_triggered();
    void actionView_toggled(QAction* view);
    void on_actionKnown_codecs_triggered();
    void on_actionKnown_formats_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_Folder_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void toolBarOptions(QPoint);
#ifdef NEW_VERSION
    void httpFinished();
    void httpReadyRead();
    void updateToNewVersion();
#endif //NEW_VERSION
};

#endif // MAINWINDOW_H
