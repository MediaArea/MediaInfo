/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "sheetview.h"
#include "translate.h"
#include "ui_sheetview.h"
#include "sheet.h"
#include "mainwindow.h"
#include <QDesktopServices>
#include <QUrl>

#include <ZenLib/Ztring.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

SheetView::SheetView(Core *C, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SheetView)
{
    this->C=C;

    ui->setupUi(this);

#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) // Workaround render bug
    QString style = "QComboBox QAbstractItemView { border: 1px solid gray }";
    ui->comboBox->setStyleSheet(style);
#endif

    refreshDisplay();
    ui->tableWidget->selectRow(0);

    QFont font = ui->label->font();
    font.setStyleHint(QFont::TypeWriter);
    ui->label->setFont(font);
}

SheetView::~SheetView()
{
    delete ui;
}

void SheetView::refreshDisplay() {
    ui->tableWidget->setRowCount((int)C->Count_Get());
    ui->tableWidget->setColumnCount(Sheet::getSheet()->getNbColumns());
    for(int i=0;i<Sheet::getSheet()->getNbColumns();++i) {
        column c = Sheet::getSheet()->getColumn(i);
        if(ui->tableWidget->horizontalHeaderItem(i))
            ui->tableWidget->horizontalHeaderItem(i)->setText(c.name);
        else
            ui->tableWidget->setHorizontalHeaderItem(i,new QTableWidgetItem(c.name));
        ui->tableWidget->setColumnWidth(i,c.width);
    }
    for (unsigned FilePos=0;FilePos<C->Count_Get();FilePos++) {
        //ui->tableWidget->setItem(FilePos,0,new QTableWidgetItem(wstring2QString(C->Get(FilePos, Stream_General, 0, __T("CompleteName")))));
        for(int i=0;i<Sheet::getSheet()->getNbColumns();++i) {
            column c = Sheet::getSheet()->getColumn(i);
            QString itemText = wstring2QString(C->Get(FilePos,c.stream,0,QString2wstring(c.key)));
            if(c.key=="CompleteName")
                itemText=MainWindow::shortName(C,itemText);
            QTableWidgetItem* twi = new QTableWidgetItem(itemText);
            twi->setData(Qt::UserRole,FilePos);
            ui->tableWidget->setItem(FilePos,i,twi);
        }
    }
    if(Sheet::getSheet()->getAdaptColumns())
        ui->tableWidget->resizeColumnsToContents();
}

void SheetView::adaptColumnsToContent() {
    ui->tableWidget->resizeColumnsToContents();
}

void SheetView::resetColumnsSizes() {
    refreshDisplay();
}

void SheetView::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SheetView::on_tableWidget_itemSelectionChanged()
{
    ui->comboBox->clear();
    ui->comboBox->addItem(Tr("Summary"),QPoint(-1,-1));
    if(ui->tableWidget->selectedItems().isEmpty())
        return;
    int filePos = ui->tableWidget->selectedItems().at(0)->data(Qt::UserRole).toInt();
    for(int streamKind=0;streamKind<Stream_Max;++streamKind) {
        if(C->Count_Get(filePos,(stream_t)streamKind) && ui->comboBox->count())
            ui->comboBox->insertSeparator(ui->comboBox->count());
        for(unsigned int streamPos=0;streamPos<C->Count_Get(filePos,(stream_t)streamKind);++streamPos) {
            ui->comboBox->addItem(wstring2QString(C->Summary_Get(filePos,(stream_t)streamKind, streamPos)),QPoint(streamKind,streamPos));
        }
    }
}

void SheetView::on_comboBox_currentIndexChanged(int index)
{
    if(ui->tableWidget->selectedItems().isEmpty()) {
        ui->label->setText("no selection");
        ui->toolButton->setEnabled(false);
        return;
    }
    int filePos = ui->tableWidget->selectedItems().at(0)->data(Qt::UserRole).toInt();
    stream_t kind = (stream_t)ui->comboBox->itemData(index).toPoint().x();
    if(kind==-1) {
        QString str="";
        for(int streamKind=0;streamKind<Stream_Max;++streamKind) {
            for(unsigned int streamPos=0;streamPos<C->Count_Get(filePos,(stream_t)streamKind);++streamPos) {
                str.append(wstring2QString(C->Summary_Get(filePos,(stream_t)streamKind, streamPos))+"\n");
            }
        }
        ui->label->setText(str);
        url = "";
    } else {
        int pos = ui->comboBox->itemData(index).toPoint().y();
        ui->label->setText(wstring2QString(C->Inform_Get(filePos,kind,pos)));
        url = wstring2QString(C->Get(filePos, (stream_t)kind, pos, __T("CodecID/Url")));
        if(url.isEmpty())
            url = wstring2QString(C->Get(filePos, (stream_t)kind, pos, __T("Format/Url")));
    }
    ui->toolButton->setEnabled(!url.isEmpty());
}

void SheetView::on_toolButton_clicked()
{
    QDesktopServices::openUrl(QUrl(url));
}
