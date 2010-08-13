#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "Common/Core.h"
#include <QtCore/QSettings>
#include <QtGui/QTreeWidget>
#include "views.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

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


    Ui::MainWindow *ui;
    //Non-GUI Elements
    Core* C;
    ViewMode view;
    QSettings* settings;

private slots:
    void on_actionAdvanced_Mode_toggled(bool );
    void on_actionExport_triggered();
    void on_actionPreferences_triggered();
    void on_actionKnown_parameters_triggered();
    void on_actionView_toggled(QAction* view);
    void on_actionKnown_codecs_triggered();
    void on_actionKnown_formats_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_Folder_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
