/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "translate.h"
#include "editcustom.h"
#include "ui_editcustom.h"

#include <QTextEdit>

#include <ZenLib/Ztring.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

EditCustom::EditCustom(Custom* c, Core* C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCustom)
{
    ui->setupUi(this);

#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) // Workaround render bug
    QString style = "QComboBox QAbstractItemView { border: 1px solid gray }";
    ui->comboBox->setStyleSheet(style);
#endif

    this->C = C;
    this->c = c;
    ui->lineEdit->setText(c->getName());
    ui->treeWidget->setColumnHidden(1,true);
    QStringList sl("Page");
    sl.append("0");
    ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(sl));
    sl.clear();
    sl.append("File");
    sl.append("1");
    ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(sl));
    for(int i=0;i<Stream_Max;i++) {
        sl.clear();
        sl.append(wstring2QString(C->StreamName((stream_t)i)));
        sl.append(QString::number(i+2));
        ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(sl));
    }

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->setMinimumWidth(ui->treeWidget->columnWidth(0));
    ui->treeWidget->setMaximumWidth(ui->treeWidget->columnWidth(0));

    refreshDisplay();
}

EditCustom::~EditCustom()
{
    delete ui;
}

void EditCustom::refreshDisplay() {
    int index;
    if(ui->treeWidget->selectedItems().isEmpty())
        index=0;
    else if(ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).isNull())
        index=0;
    else
        index=ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).toInt();

    //ui->treeWidget->itemAt(index,index)->setSelected(true);

    switch(index) {
    case 0:
        ui->textEdit->setPlainText(c->getPage());
        ui->comboBox->clear();
        ui->comboBox->addItem("Files","%Files%");
        break;
    case 1:
        ui->textEdit->setPlainText(c->getFile());
        ui->comboBox->clear();
        for(int i=0;i<Stream_Max;i++) {
            ui->comboBox->addItem(wstring2QString(C->StreamName((stream_t)i)),"%"+wstring2QString(C->StreamName((stream_t)i))+"%");
        }
        break;
    default:
        ui->textEdit->setPlainText(c->getStream(index-2));
        QString s = wstring2QString(C->Parameters());
        s.replace("\r\n","\n").replace("\r","\n");
        s.remove(0,s.indexOf(wstring2QString(C->StreamName((stream_t)(index-2)))+"\n"));
        s.truncate((s.indexOf("\n\n")==-1?s.size():s.indexOf("\n\n")));
        QStringList sl = s.split("\n");
        sl.removeAt(0);
        sl.replaceInStrings(QRegularExpression(";(.*)"),"");
        ui->comboBox->clear();
        for (int i=0; i<sl.size(); ++i) {
            ui->comboBox->addItem(sl.at(i),"%"+sl.at(i)+"%");
        }
        break;
    }
}

void EditCustom::saveText() {
    int index;
    if(ui->treeWidget->selectedItems().isEmpty())
        index=0;
    else if(ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).isNull())
        index=0;
    else
        index=ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).toInt();

    switch(index) {
    case 0:
        c->setPage(ui->textEdit->toPlainText());
        break;
    case 1:
        c->setFile(ui->textEdit->toPlainText());
        break;
    default:
        c->setStream(index-2,ui->textEdit->toPlainText());
        break;
    }
}

void EditCustom::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EditCustom::apply() {
    c->setName(ui->lineEdit->text());
}

void EditCustom::on_treeWidget_itemSelectionChanged()
{
    refreshDisplay();
}

void EditCustom::on_textEdit_textChanged()
{
    saveText();
}

void EditCustom::on_pushButton_clicked()
{
    ui->textEdit->insertPlainText(ui->comboBox->itemData(ui->comboBox->currentIndex(),Qt::UserRole).toString());
}
