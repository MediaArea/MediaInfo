#include "export.h"
#include "translate.h"
#include "_Automated/ui_export.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

Export::Export(QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export)
{
    ui->setupUi(this);
    ui->lineEdit->setText(filename);
}

Export::~Export()
{
    delete ui;
}

void Export::changeEvent(QEvent *e)
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

void Export::on_toolButton_pressed()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this,"",QDir::home().absolutePath(),Tr("All files (*.*)")));
}

int Export::getExportMode() {
    return ui->tabWidget->currentIndex();
}

QString Export::getFileName() {
    return ui->lineEdit->text();
}

QIODevice::OpenMode Export::getOpenMode() {
    if(ui->appendToFile->isChecked())
        return QIODevice::Append;
    else
        return QIODevice::WriteOnly;
}

bool Export::isAdvancedChecked() {
    switch(getExportMode()) {
    case HTML_MODE:
        return ui->htmlAdvanced->isChecked();
        break;
    default:
    case TEXT_MODE:
        return ui->textAdvanced->isChecked();
        break;
    }
}
