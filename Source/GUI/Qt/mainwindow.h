#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "Common/Core.h"

namespace Ui {
    class MainWindow;
}

typedef enum {
    VIEW_TEXT,
    VIEW_EASY,
    VIEW_HTML
} ViewMode;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

protected:
    void changeEvent(QEvent *e);

private:
    QString  mediaInfoTest(QString filename);
    void refreshDisplay();
    void openFiles(QStringList fileNames);
    void openDir(QString dirName);

    Ui::MainWindow *ui;
    //Non-GUI Elements
    Core* C;
    ViewMode view;

private slots:
    void on_actionKnown_parameters_triggered();
    void on_actionEasy_toggled(bool );
    void on_actionHTML_toggled(bool );
    void on_actionText_toggled(bool );
    void on_actionHTML_triggered();
    void on_actionKnown_codecs_triggered();
    void on_actionKnown_formats_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_Folder_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
