#include "mainwindow.h"
#include "translate.h"
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
#include <QtXml/QDomDocument>
#include <QtGui/QTreeWidget>
#include <QtGui/QToolButton>
#include "easyviewwidget.h"
#include "preferences.h"
#include "about.h"
#include "export.h"
#include "sheetview.h"
#include "sheet.h"
#include "configtreetext.h"

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
    connect(menuItemGroup,SIGNAL(selected(QAction*)),SLOT(actionView_toggled(QAction*)));
    menuItemGroup->setParent(ui->menuView);

    QToolButton* tb = new QToolButton(ui->toolBar);
    tb->setMenu(ui->menuView);
    tb->setText("view");
    tb->setPopupMode(QToolButton::InstantPopup);
    tb->setIcon(QIcon(":/icon/view.svg"));
    ui->toolBar->addWidget(tb);

    QIcon::setThemeName("gnome-dust");
    ui->actionQuit->setIcon(QIcon::fromTheme("application-exit"));
    ui->actionOpen->setIcon(QIcon::fromTheme("document-open",QIcon(":/icon/openfile.svg")));
    ui->actionExport->setIcon(QIcon::fromTheme("document-save",QIcon(":/icon/export.svg")));
    ui->actionAbout->setIcon(QIcon::fromTheme("help-about",QIcon(":/icon/about.svg")));

    timer=NULL;
    progressDialog=NULL;

    refreshDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::shortName(Core*C, QString name) {
    //Elminating unuseful info from filenames
    qDebug(("File path : "+name).toStdString().c_str());
    qDebug(("Common dir : "+getCommonDir(C).absolutePath()).toStdString().c_str());
    qDebug(("Short name : "+getCommonDir(C).relativeFilePath(name)).toStdString().c_str());
    return getCommonDir(C).relativeFilePath(name);
}

QDir MainWindow::getCommonDir(Core*C) {
    QList<QStringList> list;
    QStringList dirName;
    for(unsigned int filePos=0;filePos<C->Count_Get();filePos++)
        list.append(QDir::toNativeSeparators(wstring2QString(C->Get(filePos,Stream_General, 0, _T("CompleteName")))).split(QDir::separator ()));
    for(int i=0;i<list[0].size()-1;++i) {
        int j;
        for(j=1;(j<list.size())&&(list[j].size()>i);++j) {
            if(list[j][i]!=list[0][i])
                break;
        }
        if((j<list.size())&&(list[j].size()>=i))
            break;
        else
            dirName.append(list[0][i]);
    }
    QDir dir(dirName.join(QDir::separator ()));
    return dir;
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
    for(int i=0;i<fileNames.size();i++) {
        fileNames[i] = QDir::toNativeSeparators(fileNames[i]);
    }
    C->Menu_File_Open_Files_Begin(settings->value("closeBeforeOpen",true).toBool());
    for (int Pos=0; Pos<fileNames.size(); Pos++)
        C->Menu_File_Open_Files_Continue(QString2wstring(fileNames[Pos]));
    openTimerInit();

    refreshDisplay();
}

void MainWindow::openTimerInit ()
{
    progressDialog=new QProgressDialog(Tr("Opening files..."), Tr("Abort Opening"), 0, 10000, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(0);
    progressDialog->setWindowTitle("MediaInfo");

    if (timer==NULL)
    {
        timer=new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
        timer->start(100);
    }
}

void MainWindow::updateProgressBar ()
{
    if (progressDialog==NULL)
        return;

    progressDialog->setValue(C->State_Get());

    if (C->State_Get()==10000 || progressDialog->wasCanceled())
    {
        progressDialog->hide();
        timer->stop();

        delete progressDialog; progressDialog=NULL;
        delete timer; timer=NULL;

        //Showing
        refreshDisplay();
    }
}

void MainWindow::openDir(QString dirName) {

    //Configuring
    dirName = QDir::toNativeSeparators(dirName);
    C->Menu_File_Open_Files_Begin(settings->value("closeBeforeOpen",true).toBool());
    C->Menu_File_Open_Directory(QString2wstring(dirName));
    openTimerInit();

    refreshDisplay();
}

void MainWindow::refreshDisplay() {
    ui->actionAdapt_columns_to_content->setVisible(false);
    ui->actionReset_field_sizes->setVisible(false);
    QWidget* viewWidget;
    if(C->Count_Get()>0)
        ui->actionExport->setEnabled(true);
    else
        ui->actionExport->setEnabled(false);
    QDomDocument* xis;
    switch(view) {
        default:
        case VIEW_TEXT:
            C->Menu_View_Text();
            viewWidget = new QTextBrowser();
            ((QTextBrowser*)viewWidget)->setFontFamily("mono");
            if(ConfigTreeText::getIndex()==0)
                ((QTextBrowser*)viewWidget)->setText(wstring2QString(C->Inform_Get()));
            else {
                for (size_t FilePos=0; FilePos<C->Count_Get(); FilePos++) {
                    for (int streamKind=0;streamKind<4;streamKind++) {
                        if(!ConfigTreeText::getConfigTreeText()->getFields(streamKind).isEmpty())
                            ((QTextBrowser*)viewWidget)->append("\n"+wstring2QString(C->Get(FilePos, (stream_t)streamKind, 0, _T("StreamKind/String"), Info_Text)));
                        for (size_t streamPos=Stream_General; streamPos<C->Count_Get(FilePos, (stream_t)streamKind); streamPos++)
                        {
                            foreach(QString field, ConfigTreeText::getConfigTreeText()->getFields(streamKind)) {
                                QString A=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field)));
                                QString B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name_Text));
                                if (B.isEmpty())
                                    B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name));
                                ((QTextBrowser*)viewWidget)->append(B+" : "+A);
                            }
                        }
                    }
                }
            }
            break;
        case VIEW_PBCORE:
            C->Menu_View_PBCore();
            viewWidget = new QTextBrowser();
            xis = new QDomDocument();
            xis->setContent(wstring2QString(C->Inform_Get()));
            ((QTextBrowser*)viewWidget)->setText(xis->toString(4));
            break;
        case VIEW_MPEG7:
            C->Menu_View_MPEG7();
            viewWidget = new QTextBrowser();
            xis = new QDomDocument();
            xis->setContent(wstring2QString(C->Inform_Get()));
            ((QTextBrowser*)viewWidget)->setText(xis->toString(4));
            break;
        case VIEW_XML:
            C->Menu_View_XML();
            viewWidget = new QTextBrowser();
            xis = new QDomDocument();
            xis->setContent(wstring2QString(C->Inform_Get()));
            ((QTextBrowser*)viewWidget)->setText(xis->toString(4));
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
            ui->actionReset_field_sizes->setVisible(true);
            if(!Sheet::getSheet()->getAdaptColumns())
                ui->actionAdapt_columns_to_content->setVisible(true);
            connect(ui->actionReset_field_sizes,SIGNAL(triggered()),(SheetView*)viewWidget,SLOT(resetColumnsSizes()));
            connect(ui->actionAdapt_columns_to_content,SIGNAL(triggered()),(SheetView*)viewWidget,SLOT(adaptColumnsToContent()));
            break;
    }
    setCentralWidget(viewWidget);
    if(C->Count_Get()>0)
        if(C->Count_Get()==1)
            this->setWindowTitle("MediaInfo - "+shortName(C,wstring2QString(C->Get(0, Stream_General, 0, _T("CompleteName")))));
        else
            this->setWindowTitle(Tr("MediaInfo - %n files","window title",C->Count_Get()));
    else
        this->setWindowTitle(Tr("MediaInfo"));
}

QTreeWidget* MainWindow::showTreeView(bool completeDisplay) {
    QTreeWidget* treeWidget = new QTreeWidget();
    //treeWidget->setHeaderHidden(true);
    treeWidget->setColumnCount(2);
    QStringList headers = QStringList(Tr("key"));
    headers.append(Tr("value"));
    treeWidget->setHeaderLabels(headers);
    for (size_t FilePos=0; FilePos<C->Count_Get(); FilePos++) {
        //Pour chaque fichier
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(treeWidget,QStringList(shortName(C,wstring2QString(C->Get(FilePos, Stream_General, 0, _T("CompleteName"))))));
        treeWidget->addTopLevelItem(treeItem);

        for (int streamKind=(int)Stream_General; streamKind<(int)Stream_Max; streamKind++)
        {
            //Pour chaque type de flux
            QString StreamKindText=wstring2QString(C->Get(FilePos, (stream_t)streamKind, 0, _T("StreamKind/String"), Info_Text));
            unsigned StreamsCount=C->Count_Get(FilePos, (stream_t)streamKind);
            for (size_t streamPos=Stream_General; streamPos<StreamsCount; streamPos++)
            {
                //Pour chaque stream
                QString A=StreamKindText;
                QString B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, _T("StreamKindPos"), Info_Text));
                if (!B.isEmpty())
                {
                    A+=" #";
                    A+=B;
                }
                QTreeWidgetItem* node = new QTreeWidgetItem(treeItem,QStringList(A));
                treeItem->addChild(node);
                unsigned ChampsCount=C->Count_Get(FilePos, (stream_t)streamKind, streamPos);

                if(ConfigTreeText::getIndex()==0) {
                    for (size_t Champ_Pos=0; Champ_Pos<ChampsCount; Champ_Pos++)
                    {
                        if ((completeDisplay || C->Get(FilePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]==_T('Y')) && C->Get(FilePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Text)!=_T(""))
                        {
                            //Pour chaque champ
                            QString A=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Text));
                            A+=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Measure_Text));
                            //Quoi Refresh?
                            QString D=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Name_Text));
                            if (D.isEmpty())
                                D=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Name)); //Texte n'existe pas
                            //Affichage
                            QStringList sl = QStringList(D);
                            sl.append(A);
                            node->addChild(new QTreeWidgetItem(node,sl));
                        }
                    }
                } else {
                    foreach(QString field, ConfigTreeText::getConfigTreeText()->getFields(streamKind)) {
                        QString A=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field)));
                        QString B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name_Text));
                        if (B.isEmpty())
                            B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name));
                        QStringList sl = QStringList(B);
                        sl.append(A);
                        node->addChild(new QTreeWidgetItem(node,sl));
                    }
                }
            }
        }
    }
    if(C->Count_Get()<=1)
        treeWidget->expandAll();
    treeWidget->resizeColumnToContents(0);
    return treeWidget;
}

void MainWindow::defaultSettings() {
    /*if(!settings->contains("showMenu"))
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
        settings->setValue("rememberToolBarPosition",true);*/
    Sheet::load(settings);
    if(Sheet::getNbSheets()==0) {
        Sheet::add("example");
        Sheet::setDefault(0);
        Sheet::getSheet()->addColumn(Tr("File Name").toStdString().c_str(),300,Stream_General,"CompleteName");
        Sheet::getSheet()->addColumn(Tr("Format").toStdString().c_str(),100,Stream_General,"Format");
        Sheet::getSheet()->addColumn(Tr("Video Codec List").toStdString().c_str(),100,Stream_General,"Video_Codec_List");
        Sheet::getSheet()->addColumn(Tr("Audio Codec List").toStdString().c_str(),100,Stream_General,"Audio_Codec_List");
        Sheet::getSheet()->addColumn(Tr("Text Codec List").toStdString().c_str(),100,Stream_General,"Text_Codec_List");
        Sheet::getSheet()->addColumn(Tr("Video Format").toStdString().c_str(),100,Stream_Video,"Format");
        Sheet::getSheet()->addColumn(Tr("Audio Duration").toStdString().c_str(),100,Stream_Audio,"Duration");
        Sheet::getSheet()->addColumn(Tr("Text Width").toStdString().c_str(),100,Stream_Text,"Width");
    }
    ConfigTreeText::load(settings);
    if(ConfigTreeText::getNbConfigTreeTexts()==0) {
        ConfigTreeText::add("Default");
        ConfigTreeText::setDefault(0);
    }

}

void MainWindow::applySettings() {
    if(settings->value("rememberGeometry",false).toBool())
        restoreGeometry(settings->value("geometry").toByteArray());
    if(settings->value("showToolbar",true).toBool() && settings->value("rememberToolBarPosition",true).toBool())
        restoreState(settings->value("windowState").toByteArray());
    ui->menuBar->setVisible(settings->value("showMenu",true).toBool());
    ui->toolBar->setVisible(settings->value("showToolbar",true).toBool());
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
    QStringList fileNames = QFileDialog::getOpenFileNames(this,Tr("Open File(s)"), QDir::home().absolutePath(), Tr("All Files (*.*)"));
    openFiles(fileNames);
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this,Tr("Open File(s)"), QDir::home().absolutePath());
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

void MainWindow::actionView_toggled(QAction* view)
{
    this->view = (ViewMode)view->property("view").toInt();
    refreshDisplay();
}

void MainWindow::on_actionPreferences_triggered()
{
    ViewMode oldView = (ViewMode)settings->value("defaultView",VIEW_EASY).toInt();
    Preferences p(settings,C,this);
    if(p.exec() == QDialog::Accepted) {
        p.saveSettings();
        settings->setValue("geometry", saveGeometry()); // we save positions and geometry as they might be restored in the applySettings function
        settings->setValue("windowState", saveState());
        applySettings();
        if(settings->value("defaultView",VIEW_EASY)!=oldView) {
            this->view = (ViewMode)settings->value("defaultView",VIEW_EASY).toInt();
        }
        refreshDisplay();
    } else
        qDebug("preferences cancelled");
}

void MainWindow::on_actionExport_triggered()
{
    QString name;
    if(C->Count_Get()==1)
        name = wstring2QString(C->Get(0, Stream_General, 0, _T("CompleteName")))+".txt";
    else
        name = getCommonDir(C).absoluteFilePath("MediaInfo.txt");
    Export e(name,settings->value("exportMode",Export::TEXT).toInt(),this);
    if(e.exec() == QDialog::Accepted) {
        QFile file(e.getFileName());
        if(!file.open(e.getOpenMode()))
            QMessageBox::warning(this,"Error","The file cannot be open");
        C->Menu_Debug_Complete(e.isAdvancedChecked());
        switch(e.getExportMode()) {
        case Export::TEXT:
            C->Menu_View_Text();
            if(e.getExportConfig()==0)
                file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            else {
                for (size_t FilePos=0; FilePos<C->Count_Get(); FilePos++) {
                    for (int streamKind=0;streamKind<4;streamKind++) {
                        if(!ConfigTreeText::getConfigTreeText()->getFields(streamKind).isEmpty())
                            file.write(("\n"+wstring2QString(C->Get(FilePos, (stream_t)streamKind, 0, _T("StreamKind/String"), Info_Text))+"\n").toStdString().c_str());
                        for (size_t streamPos=Stream_General; streamPos<C->Count_Get(FilePos, (stream_t)streamKind); streamPos++)
                        {
                            foreach(QString field, ConfigTreeText::getConfigTreeText()->getFields(streamKind)) {
                                QString A=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field)));
                                file.write((field+" : "+A+"\n").toStdString().c_str());
                            }
                        }
                    }
                }
            }
            break;
        case Export::HTML:
            C->Menu_View_HTML();
            file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            break;
        case Export::XML:
            C->Menu_View_XML();
            file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            break;
        case Export::PBCORE:
            C->Menu_View_PBCore();
            file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            break;
        case Export::MPEG7:
            C->Menu_View_MPEG7();
            file.write(wstring2QString(C->Inform_Get()).toStdString().c_str());
            break;
        default:
            QMessageBox::warning(this,"Error","Please signal this error to the MediaInfo project team : Unkown export mode");
            break;
        }

        settings->setValue("exportMode",e.getExportMode());
    } else
        qDebug("export cancelled");
}

void MainWindow::on_actionAdvanced_Mode_toggled(bool checked)
{
    C->Menu_Debug_Complete(checked);
    refreshDisplay();
}

void MainWindow::on_actionClose_All_triggered()
{
    C->Menu_File_Open_Files_Begin(true);
    refreshDisplay();
}
