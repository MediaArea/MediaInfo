#include "translate.h"
#include "editsheet.h"
#include "_Automated/ui_editsheet.h"
#include "sheet.h"
#include "columneditsheet.h"
#include "QtGui/QHBoxLayout"
#include "QtGui/QLabel"
#include "QtGui/QLineEdit"
#include "QtGui/QSpinBox"
#include "QtGui/QPushButton"
#include "QtGui/QToolButton"
#include "QtGui/QSizePolicy"
#include "QtGui/QComboBox"

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
        ui->verticalLayout->addLayout(createColumn(sheet->getColumn(i)));
        emit newPos(ui->verticalLayout->count());
    }
    ui->tableWidget->setRowCount(1);
}

void EditSheet::refreshDisplay() {
    qDebug((QString::number(ui->verticalLayout->count())+" columns :").toStdString().c_str());
    ui->tableWidget->setColumnCount(ui->verticalLayout->count());
    for(int i=0;i<ui->verticalLayout->count();++i) {
        //column c = sheet->getColumn(i);
        qDebug((QString::number(i)+" : "+((QLineEdit*)ui->verticalLayout->itemAt(i)->layout()->itemAt(0)->widget())->text()).toStdString().c_str());
        if(!ui->checkBoxAdapt)
            ui->tableWidget->setColumnWidth(i,((QSpinBox*)ui->verticalLayout->itemAt(i)->layout()->itemAt(1)->widget())->value());
        if(ui->tableWidget->horizontalHeaderItem(i))
            ui->tableWidget->horizontalHeaderItem(i)->setText(((QLineEdit*)ui->verticalLayout->itemAt(i)->layout()->itemAt(0)->widget())->text());
        else {
            QTableWidgetItem* header = new QTableWidgetItem(((QLineEdit*)ui->verticalLayout->itemAt(i)->layout()->itemAt(0)->widget())->text());
            ui->tableWidget->setHorizontalHeaderItem(i,header);
        }
    }
    if(ui->checkBoxAdapt)
        ui->tableWidget->resizeColumnsToContents();
}

QLayout* EditSheet::createColumn(column c) {
    ColumnEditSheet* col = new ColumnEditSheet(c,ui->verticalLayout->count(),ui->verticalLayout->count()+1,C);

    connect(col,SIGNAL(somethingChanged()),SLOT(refreshDisplay()));
    connect(col,SIGNAL(moveMeUp(int)),SLOT(upCol(int)));
    connect(col,SIGNAL(moveMeDown(int)),SLOT(downCol(int)));
    connect(col,SIGNAL(removeMe(int)),SLOT(delCol(int)));
    connect(this,SIGNAL(switchPos(int,int,int)),col,SLOT(posSwitched(int,int,int)));
    connect(this,SIGNAL(deletePos(int,int)),col,SLOT(posRemoved(int,int)));
    connect(this,SIGNAL(newPos(int)),col,SLOT(posChanged(int)));
    connect(ui->checkBoxAdapt,SIGNAL(toggled(bool)),col->widthBox(),SLOT(setDisabled(bool)));

    col->widthBox()->setDisabled(ui->checkBoxAdapt->isChecked());

    col->setContentsMargins(0,0,0,0);
    col->setSpacing(0);
    col->setMargin(0);

    return col;
}

void EditSheet::upCol(int i) {
    if(i>0) {
        ui->verticalLayout->insertLayout(i-1,((QLayout*)ui->verticalLayout->takeAt(i)));
        emit switchPos(i,i-1,ui->verticalLayout->count());
    }
}

void EditSheet::downCol(int i) {
    if(i<ui->verticalLayout->count()-1) {
        ui->verticalLayout->insertLayout(i+1,((QLayout*)ui->verticalLayout->takeAt(i)));
        emit switchPos(i,i+1,ui->verticalLayout->count());
    }
}

void EditSheet::delCol(int i) {
    ColumnEditSheet* c = (ColumnEditSheet*)ui->verticalLayout->takeAt(i);
    delete c;
    emit deletePos(i,ui->verticalLayout->count());
}

void EditSheet::addColumn() {
    column c;
    c.name = Tr("CompleteName");
    c.width = 50;
    c.stream = Stream_General;
    c.key = "CompleteName";
    ui->verticalLayout->addLayout(createColumn(c));
    emit newPos(ui->verticalLayout->count());
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
    for(int i=0;i<ui->verticalLayout->count();++i) {
        column c;
        c.name = ((QLineEdit*)ui->verticalLayout->itemAt(i)->layout()->itemAt(0)->widget())->text();
        c.width = ((QSpinBox*)ui->verticalLayout->itemAt(i)->layout()->itemAt(1)->widget())->value();
        c.stream = (stream_t)((QComboBox*)ui->verticalLayout->itemAt(i)->layout()->itemAt(2)->widget())->itemData(((QComboBox*)ui->verticalLayout->itemAt(i)->layout()->itemAt(2)->widget())->currentIndex()).toInt();
        c.key = ((QComboBox*)ui->verticalLayout->itemAt(i)->layout()->itemAt(3)->widget())->itemData(((QComboBox*)ui->verticalLayout->itemAt(i)->layout()->itemAt(3)->widget())->currentIndex()).toString();
        sheet->addColumn(c);
    }
    sheet->setName(ui->lineEdit->text().toStdString().c_str());
    sheet->setAdaptColumns(ui->checkBoxAdapt->isChecked());
}
