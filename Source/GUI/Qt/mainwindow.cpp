#include "mainwindow.h"
#include "_Automated/ui_mainwindow.h"

#include <iostream>
#include <iomanip>
#include <QtGui/QDropEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>
#include "easyviewwidget.h"

#include <ZenLib/Ztring.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setText("Bienvenue dans MediaInfo");
    C = new Core();
    view = VIEW_EASY;

    // View menu:
    QActionGroup* menuItemGroup = new QActionGroup(this); // Maybe it can be done directly in mainwindow.ui
    menuItemGroup->addAction(ui->actionEasy);
    menuItemGroup->addAction(ui->actionText);
    menuItemGroup->addAction(ui->actionHTML);
    menuItemGroup->setParent(ui->menuView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::openFiles(QStringList fileNames) {
    //Configuring
    C->Menu_File_Open_Files_Begin();
    for (int Pos=0; Pos<fileNames.size(); Pos++)
        C->Menu_File_Open_Files_Continue(QString2wstring(fileNames[(size_t)Pos]));

    refreshDisplay();
}

void MainWindow::openDir(QString dirName) {

    //Configuring
    C->Menu_File_Open_Files_Begin();
    C->Menu_File_Open_Files_Continue(QString2wstring(dirName));

    refreshDisplay();
}

void MainWindow::refreshDisplay() {
    QWidget* viewWidget;
    switch(view) {
    default:
    case VIEW_TEXT:
        viewWidget = new QTextBrowser();
        C->Menu_View_Text();
        ((QTextBrowser*)viewWidget)->setText(wstring2QString(C->Inform_Get()));
        break;
    case VIEW_EASY:
        viewWidget = new EasyViewWidget(C);
        break;
    case VIEW_HTML:
        viewWidget = new QTextBrowser();
        C->Menu_View_HTML();
        ((QTextBrowser*)viewWidget)->setHtml(wstring2QString(C->Inform_Get()));
        break;
    }
    setCentralWidget(viewWidget);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList files;
    for(int i=0;i<event->mimeData()->urls().size();i++) {
        files.push_back(event->mimeData()->urls().at(i).toLocalFile());
        qDebug("adding ");
        qDebug(files.last().toStdString().c_str());
        qDebug("to the list");
    }
    openFiles(files);
    event->acceptProposedAction();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,tr("Open File(s)"), QDir::home().absolutePath(), tr("All Files (*.*)"));
    openFiles(fileNames);
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this,tr("Open File(s)"), QDir::home().absolutePath());
    openDir(dirName);
}

void MainWindow::on_actionAbout_triggered()
{
    QString Version=wstring2QString(C->Menu_Option_Preferences_Option(_T("Info_Version"), String()));
    QMessageBox::about(this, "About MediaInfo",
            "This is the About dialog of the minimal MediaInfo sample.\n" \
            "MediaInfoLib - v0.7.34\n" \
            +Version+ \
            "a video or audio file.\n" \
            "To get more info's visit \n" \
            "http://mediainfo.sourceforge.net");
}

void MainWindow::on_actionKnown_formats_triggered()
{
    C->Menu_Help_Info_Formats();
    QTextBrowser* textBrowser = new QTextBrowser();
    textBrowser->setText(wstring2QString(C->Text_Get()));
    setCentralWidget(textBrowser);
}

void MainWindow::on_actionKnown_codecs_triggered()
{
    C->Menu_Help_Info_Codecs();
    QTextBrowser* textBrowser = new QTextBrowser();
    textBrowser->setText(wstring2QString(C->Text_Get()));
    setCentralWidget(textBrowser);
}

void MainWindow::on_actionKnown_parameters_triggered()
{
    C->Menu_Help_Info_Parameters();
    QTextBrowser* textBrowser = new QTextBrowser();
    textBrowser->setText(wstring2QString(C->Text_Get()));
    setCentralWidget(textBrowser);
}

void MainWindow::on_actionText_toggled(bool checked)
{
    if(checked)
        view=VIEW_TEXT;
    refreshDisplay();
}

void MainWindow::on_actionHTML_toggled(bool checked)
{
    if(checked)
        view=VIEW_HTML;
    refreshDisplay();
}

void MainWindow::on_actionEasy_toggled(bool checked)
{
    if(checked)
        view=VIEW_EASY;
    refreshDisplay();
}

void MainWindow::on_actionHTML_triggered()
{

}
