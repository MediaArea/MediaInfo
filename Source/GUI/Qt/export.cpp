/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "export.h"
#include "translate.h"
#include "_Automated/ui_export.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include "configtreetext.h"
#include "mainwindow.h"

Export::Export(QString filename, int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export)
{
    ui->setupUi(this);
    ui->lineEdit->setText(QDir::toNativeSeparators(filename));
    for(int i=0;i<Export::NB_EXPORT_MODE;i++) {
        ui->comboBoxMode->addItem(name(i),i);
        if(i==mode)
            ui->comboBoxMode->setCurrentIndex(ui->comboBoxMode->count()-1);
    }
    ConfigTreeText::fillComboBox(ui->comboBoxConfig);
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
    QString path = QFileDialog::getOpenFileName(this,"",QDir(ui->lineEdit->text()).absolutePath());
    if(!path.isEmpty())
        ui->lineEdit->setText(path);
}

int Export::getExportMode() {
    return ui->comboBoxMode->itemData(ui->comboBoxMode->currentIndex()).toInt();
}

int Export::getExportConfig() {
    if(ui->comboBoxConfig->isEnabled())
        return ui->comboBoxConfig->itemData(ui->comboBoxConfig->currentIndex()).toInt();
    else
        return 0;
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
    return ui->checkBoxAdvanced->isChecked();
}

QString Export::extension(int mode) {
    switch(mode) {
    default:
    case TEXT: return "txt";
        break;
    case HTML: return "html";
        break;
    case XML: return "xml";
        break;
    case PBCORE: return "xml";
        break;
    case PBCORE2: return "xml";
        break;
    case MPEG7: return "xml";
        break;
    case CSV: return "csv";
        break;
    }
}

QString Export::name(int mode) {
    switch(mode) {
    default:
    case TEXT: return Tr("Text");
        break;
    case HTML: return Tr("HTML");
        break;
    case XML: return Tr("XML");
        break;
    case PBCORE: return Tr("PBCore");
        break;
    case PBCORE2: return Tr("PBCore 2");
        break;
    case MPEG7: return Tr("MPEG-7");
        break;
    case CSV: return Tr("CSV");
        break;
    }
}

void Export::on_comboBoxMode_currentIndexChanged(int index)
{
    QStringList filename = ui->lineEdit->text().split(".");
    filename[filename.size()-1] = extension(index);
    ui->lineEdit->setText(filename.join("."));

    switch(index) {
    default:
    case TEXT:
        ui->comboBoxConfig->setDisabled(ui->checkBoxAdvanced->isChecked());
        ui->checkBoxAdvanced->setEnabled(true);
        break;
    case XML:
    case PBCORE:
    case PBCORE2:
    case MPEG7:
        ui->comboBoxConfig->setEnabled(false);
        ui->checkBoxAdvanced->setEnabled(false);
        break;
    case CSV:
    case HTML:
        ui->comboBoxConfig->setEnabled(false);
        ui->checkBoxAdvanced->setEnabled(true);
        break;
    }
}

void Export::on_checkBoxAdvanced_toggled(bool checked)
{
    ui->comboBoxConfig->setEnabled( (!checked) && (ui->comboBoxMode->itemData(ui->comboBoxMode->currentIndex()).toInt()==TEXT) );
}
