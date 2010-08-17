#include "sheetview.h"
#include "_Automated/ui_sheetview.h"
#include "sheet.h"

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
    refreshDisplay();
}

SheetView::~SheetView()
{
    delete ui;
}

void SheetView::refreshDisplay() {
    ui->tableWidget->setRowCount(C->Count_Get());
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
        //ui->tableWidget->setItem(FilePos,0,new QTableWidgetItem(wstring2QString(C->Get(FilePos, Stream_General, 0, _T("CompleteName")))));
        for(int i=0;i<Sheet::getSheet()->getNbColumns();++i) {
            column c = Sheet::getSheet()->getColumn(i);
            ui->tableWidget->setItem(FilePos,i,new QTableWidgetItem(wstring2QString(C->Get(FilePos,c.stream,0,QString2wstring(c.key)))));
        }
    }
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
    if(ui->tableWidget->selectedItems().isEmpty())
        return;
    int filePos = ui->tableWidget->selectedItems().at(0)->row();
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
        return;
    }
    int filePos = ui->tableWidget->selectedItems().at(0)->row();
    stream_t kind = (stream_t)ui->comboBox->itemData(index).toPoint().x();
    int pos = ui->comboBox->itemData(index).toPoint().y();
    ui->label->setText(wstring2QString(C->Inform_Get(filePos,kind,pos)));
}
