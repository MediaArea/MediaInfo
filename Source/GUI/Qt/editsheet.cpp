/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "translate.h"
#include "editsheet.h"
#include "ui_editsheet.h"
#include "sheet.h"
#include "columneditsheet.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QToolButton>
#include <QSizePolicy>
#include <QComboBox>

EditSheet::EditSheet(Sheet* s, Core* C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditSheet)
{
    ui->setupUi(this);
    this->sheet = s;
    this->C = C;

    initDisplay();
    refreshDisplay();
    connect(ui->pushButton,SIGNAL(clicked()),SLOT(addColumn()));
    ui->checkBoxAdapt->setChecked(s->getAdaptColumns());
}

EditSheet::~EditSheet()
{
    delete ui;
}

void EditSheet::initDisplay() {
    ui->lineEdit->setText(sheet->getName());
    for(int i=0;i<sheet->getNbColumns();i++) {
        ui->vboxLayout->addLayout(createColumn(sheet->getColumn(i)));
        emit newPos(ui->vboxLayout->count());
    }
    ui->tableWidget->setRowCount(1);
}

void EditSheet::refreshDisplay() {
    qDebug() << ui->vboxLayout->count() << " columns :";
    ui->tableWidget->setColumnCount(ui->vboxLayout->count());
    for(int i=0;i<ui->vboxLayout->count();++i) {
        //column c = sheet->getColumn(i);
        qDebug() << i << " : " << ((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text();
        if(!ui->checkBoxAdapt)
            ui->tableWidget->setColumnWidth(i,((QSpinBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(1)->widget())->value());
        if(ui->tableWidget->horizontalHeaderItem(i))
            ui->tableWidget->horizontalHeaderItem(i)->setText(((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text());
        else {
            QTableWidgetItem* header = new QTableWidgetItem(((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text());
            ui->tableWidget->setHorizontalHeaderItem(i,header);
        }
    }
    if(ui->checkBoxAdapt)
        ui->tableWidget->resizeColumnsToContents();
}

QLayout* EditSheet::createColumn(column c) {
    ColumnEditSheet* col = new ColumnEditSheet(c,ui->vboxLayout->count(),ui->vboxLayout->count()+1,C);

    connect(col,SIGNAL(somethingChanged()),SLOT(refreshDisplay()));
    connect(col,SIGNAL(moveMeUp(int)),SLOT(upCol(int)));
    connect(col,SIGNAL(moveMeDown(int)),SLOT(downCol(int)));
    connect(col,SIGNAL(removeMe(int)),SLOT(delCol(int)));
    connect(this,SIGNAL(switchPos(int,int,int)),col,SLOT(posSwitched(int,int,int)));
    connect(this,SIGNAL(deletePos(int,int)),col,SLOT(posRemoved(int,int)));
    connect(this,SIGNAL(newPos(int)),col,SLOT(posChanged(int)));
    connect(ui->checkBoxAdapt,SIGNAL(toggled(bool)),col->widthBox(),SLOT(setDisabled(bool)));

    col->widthBox()->setDisabled(ui->checkBoxAdapt->isChecked());

#if QT_VERSION >= 0x040300
    col->setContentsMargins(0,0,0,0);
#endif
    col->setSpacing(0);

    return col;
}

void EditSheet::upCol(int i) {
    if(i>0) {
        ui->vboxLayout->insertLayout(i-1,((QLayout*)ui->vboxLayout->takeAt(i)));
        emit switchPos(i,i-1,ui->vboxLayout->count());
    }
}

void EditSheet::downCol(int i) {
    if(i<ui->vboxLayout->count()-1) {
        ui->vboxLayout->insertLayout(i+1,((QLayout*)ui->vboxLayout->takeAt(i)));
        emit switchPos(i,i+1,ui->vboxLayout->count());
    }
}

void EditSheet::delCol(int i) {
    ColumnEditSheet* c = (ColumnEditSheet*)ui->vboxLayout->takeAt(i);
    delete c;
    emit deletePos(i,ui->vboxLayout->count());
}

void EditSheet::addColumn() {
    column c;
    c.name = Tr("CompleteName");
    c.width = 50;
    c.stream = Stream_General;
    c.key = "CompleteName";
    ui->vboxLayout->addLayout(createColumn(c));
    emit newPos(ui->vboxLayout->count());
    refreshDisplay();
}

void EditSheet::changeEvent(QEvent *e)
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

void EditSheet::apply() {
    sheet->resetColumns();
    for(int i=0;i<ui->vboxLayout->count();++i) {
        column c;
        c.name = ((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text();
        c.width = ((QSpinBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(1)->widget())->value();
        c.stream = (stream_t)((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(2)->widget())->itemData(((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(2)->widget())->currentIndex()).toInt();
        c.key = ((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(3)->widget())->itemData(((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(3)->widget())->currentIndex()).toString();
        sheet->addColumn(c);
    }
    sheet->setName(ui->lineEdit->text().toStdString().c_str());
    sheet->setAdaptColumns(ui->checkBoxAdapt->isChecked());
}
