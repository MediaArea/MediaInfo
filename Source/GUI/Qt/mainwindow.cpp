#include "mainwindow.h"
#include "_Automated/ui_mainwindow.h"

#include <iostream>
#include <iomanip>
#include <QtGui/QDropEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>
#include <QtGui/QActionGroup>
#include <QtGui/QTextBrowser>
#include <QtCore/QSettings>
#include <QtGui/QTreeWidget>
#include "easyviewwidget.h"
#include "preferences.h"
#include "about.h"
#include "export.h"
#include "sheetview.h"
#include "sheet.h"

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

    settings = new QSettings("MediaInfo.net","MediaInfo");
    defaultSettings();
    applySettings();

    view = (ViewMode)settings->value("defaultView",VIEW_EASY).toInt();
    // View menu:
    QActionGroup* menuItemGroup = new QActionGroup(this);
    for(int v=VIEW_EASY;v<NB_VIEW;v++) {
        QAction* action = new QAction(nameView((ViewMode)v),ui->menuView);
        action->setCheckable(true);
        if(view==v)
            action->setChecked(true);
        action->setProperty("view",v);
        ui->menuView->addAction(action);
        menuItemGroup->addAction(action);
    }
    connect(menuItemGroup,SIGNAL(selected(QAction*)),SLOT(on_actionView_toggled(QAction*)));
    menuItemGroup->setParent(ui->menuView);

    refreshDisplay();
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

void MainWindow::closeEvent(QCloseEvent *e)
{
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    QMainWindow::closeEvent(e);
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
    if(C->Count_Get()>0)
        ui->actionExport->setEnabled(true);
    else
        ui->actionExport->setEnabled(false);
    switch(view) {
    default:
    case VIEW_TEXT:
        C->Menu_View_Text();
        viewWidget = new QTextBrowser();
        ((QTextBrowser*)viewWidget)->setFontFamily("mono");
        ((QTextBrowser*)viewWidget)->setText(wstring2QString(C->Inform_Get()));
        break;
    case VIEW_EASY:
        C->Menu_View_Easy();
        viewWidget = new EasyViewWidget(C);
        break;
    case VIEW_HTML:
        C->Menu_View_HTML();
        viewWidget = new QTextBrowser();
        ((QTextBrowser*)viewWidget)->setHtml(wstring2QString(C->Inform_Get()));
        break;
    case VIEW_TREE:
        C->Menu_View_Tree();
        viewWidget = showTreeView(ui->actionAdvanced_Mode->isChecked());
        break;
    case VIEW_SHEET:
        C->Menu_View_Sheet();
        viewWidget = new SheetView(C,this);
        break;
    }
    setCentralWidget(viewWidget);
}

QTreeWidget* MainWindow::showTreeView(bool completeDisplay) {
    QTreeWidget* treeWidget = new QTreeWidget();
    treeWidget->setHeaderHidden(true);
    for (size_t FilePos=0; FilePos<C->Count_Get(); FilePos++) {
        //Pour chaque fichier
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(treeWidget,QStringList(wstring2QString(C->Get(FilePos, Stream_General, 0, _T("CompleteName")))));
        treeWidget->addTopLevelItem(treeItem);

        for (int StreamKind=(int)Stream_General; StreamKind<(int)Stream_Max; StreamKind++)
        {
            //Pour chaque type de flux
            QString StreamKindText=wstring2QString(C->Get(FilePos, (stream_t)StreamKind, 0, _T("StreamKind/String"), Info_Text));
            unsigned StreamsCount=C->Count_Get(FilePos, (stream_t)StreamKind);
            for (size_t StreamPos=Stream_General; StreamPos<StreamsCount; StreamPos++)
            {
                //Pour chaque stream
                QString A=StreamKindText;
                QString B=wstring2QString(C->Get(FilePos, (stream_t)StreamKind, StreamPos, _T("StreamKindPos"), Info_Text));
                if (!B.isEmpty())
                {
                    A+=" #";
                    A+=B;
                }
                QTreeWidgetItem* node = new QTreeWidgetItem(treeItem,QStringList(A));
                treeItem->addChild(node);
                unsigned ChampsCount=C->Count_Get(FilePos, (stream_t)StreamKind, StreamPos);

                for (size_t Champ_Pos=0; Champ_Pos<ChampsCount; Champ_Pos++)
                {
                    if ((completeDisplay || C->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]==_T('Y')) && C->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text)!=_T(""))
                    {
                        //Pour chaque champ
                        QString A=wstring2QString(C->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text));
                        A+=wstring2QString(C->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Measure_Text));
                        //Quoi Refresh?
                        QString D=wstring2QString(C->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Name_Text));
                        if (D.isEmpty())
                            D=wstring2QString(C->Get(FilePos, (stream_t)StreamKind, StreamPos, Champ_Pos, Info_Name)); //Texte n'existe pas
                        //Affichage
                        node->addChild(new QTreeWidgetItem(node,QStringList((D + ": " + A))));
                    }
                }
            }
        }
    }
    return treeWidget;
}

void MainWindow::defaultSettings() {
    if(!settings->contains("showMenu"))
        settings->setValue("showMenu",true);
    if(!settings->contains("showToolBar"))
        settings->setValue("showToolBar",true);
    if(!settings->contains("closeBeforeOpen"))
        settings->setValue("closeBeforeOpen",true);
    if(!settings->contains("defaultView"))
        settings->setValue("defaultView",VIEW_EASY);
    if(!settings->contains("checkForNewVersion"))
        settings->setValue("checkForNewVersion",true);
    if(!settings->contains("rememberToolBarPosition"))
        settings->setValue("rememberToolBarPosition",true);
    if(!settings->contains("sheets")) {
        Sheet::add("example");
        Sheet::setDefault(0);
        Sheet::getSheet()->addColumn("File Name",100,Stream_General,"CompleteName");
        Sheet::save(settings);
    } else {
        Sheet::load(settings);
    }

}

void MainWindow::applySettings() {
    ui->menuBar->setVisible(settings->value("showMenu",true).toBool());
    ui->toolBar->setVisible(settings->value("showToolbar",true).toBool());
    if(settings->value("rememberGeometry",false).toBool())
        restoreGeometry(settings->value("geometry").toByteArray());
    if(settings->value("rememberToolBarPosition",true).toBool())
        restoreState(settings->value("windowState").toByteArray());

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
    About a(this);
    a.exec();
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

void MainWindow::on_actionView_toggled(QAction* view)
{
    this->view = (ViewMode)view->property("view").toInt();
    refreshDisplay();
}

void MainWindow::on_actionPreferences_triggered()
{
    Preferences p(settings,this);
    if(p.exec() == QDialog::Accepted) {
        p.saveSettings();
        settings->setValue("geometry", saveGeometry()); // we save positions and geometry as they might be restored in the applySettings function
        settings->setValue("windowState", saveState());
        applySettings();
    } else
        qDebug("annulation");
}

void MainWindow::on_actionExport_triggered()
{
    Export e(this);
    if(e.exec() == QDialog::Accepted) {
        QFile file(e.getFileName());
        if(!file.open(e.getOpenMode()))
            QMessageBox::warning(this,"Error","The file cannot be open");
        C->Menu_Debug_Complete(e.isAdvancedChecked());
        switch(e.getExportMode()) {
        case Export::TEXT_MODE:
            C->Menu_View_Text();
            file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            break;
        case Export::HTML_MODE:
            C->Menu_View_HTML();
            file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            break;
        default:
            QMessageBox::warning(this,"Error","Please signal this error to the MediaInfo project team : Unkown export mode");
            break;
        }
    } else
        qDebug("annulation");
}

void MainWindow::on_actionAdvanced_Mode_toggled(bool checked)
{
    C->Menu_Debug_Complete(checked);
    refreshDisplay();
}
