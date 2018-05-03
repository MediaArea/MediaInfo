/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "export.h"
#include "translate.h"
#include "ui_export.h"
#include <QFileDialog>
#include <QMessageBox>
#include "configtreetext.h"
#include "mainwindow.h"

Export::Export(QString filename, int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export),
    path(filename)
{
    ui->setupUi(this);

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
    return path;
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
    case XML:
    case PBCORE:
    case PBCORE2:
    case MPEG7:
    case NISO_Z39_87:
    case EBUCORE_1_5:
    case EBUCORE_1_6:
    case EBUCORE_1_8_ps:
    case EBUCORE_1_8_sp: return "xml";
        break;
    case JSON:
    case EBUCORE_1_8_ps_JSON:
    case EBUCORE_1_8_sp_JSON: return "json";
        break;
    }
}

QString Export::extensionName(int mode) {
    switch(mode) {
    default:
    case TEXT: return tr("Text");
        break;
    case HTML: return "HTML";
        break;
    case XML:
    case PBCORE:
    case PBCORE2:
    case MPEG7:
    case NISO_Z39_87:
    case EBUCORE_1_5:
    case EBUCORE_1_6:
    case EBUCORE_1_8_ps:
    case EBUCORE_1_8_sp: return "XML";
        break;
    case JSON:
    case EBUCORE_1_8_ps_JSON:
    case EBUCORE_1_8_sp_JSON: return "JSON";
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
    case JSON: return Tr("JSON");
        break;
    case PBCORE: return Tr("PBCore");
        break;
    case PBCORE2: return Tr("PBCore 2");
        break;
    case MPEG7: return Tr("MPEG-7");
        break;
   case EBUCORE_1_5: return Tr("EBUCore 1.5");
       break;
   case EBUCORE_1_6: return Tr("EBUCore 1.6");
       break;
   case EBUCORE_1_8_ps: return Tr("EBUCore 1.8 parameter then segment");
       break;
   case EBUCORE_1_8_sp: return Tr("EBUCore 1.8 segment then parameter");
       break;
   case EBUCORE_1_8_ps_JSON: return Tr("EBUCore 1.8 parameter then segment (JSON output)");
       break;
   case EBUCORE_1_8_sp_JSON: return Tr("EBUCore 1.8 segment then parameter (JSON output)");
       break;
   case FIMS_1_1: return Tr("FIMS 1.1");
       break;
   case FIMS_1_2: return Tr("FIMS 1.2");
       break;
   case FIMS_1_3: return Tr("FIMS 1.3");
       break;
    case NISO_Z39_87: return Tr("NISO Z39.87");
        break;
    }
}

void Export::on_comboBoxMode_currentIndexChanged(int index)
{
    QStringList filename = path.split(".");
    filename[filename.size()-1] = extension(index);
    path = filename.join(".");

    switch(index) {
    default:
    case TEXT:
        ui->comboBoxConfig->setDisabled(ui->checkBoxAdvanced->isChecked());
        ui->checkBoxAdvanced->setEnabled(true);
        break;
    case XML:
    case JSON:
    case PBCORE:
    case PBCORE2:
    case MPEG7:
    case EBUCORE_1_5:
    case EBUCORE_1_6:
    case EBUCORE_1_8_ps:
    case EBUCORE_1_8_sp:
    case EBUCORE_1_8_ps_JSON:
    case EBUCORE_1_8_sp_JSON:
    case FIMS_1_1:
    case FIMS_1_2:
    case FIMS_1_3:
    case NISO_Z39_87:
        ui->comboBoxConfig->setEnabled(false);
        ui->checkBoxAdvanced->setEnabled(false);
        break;
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

void Export::on_buttonBox_accepted()
{
    path = QFileDialog::getSaveFileName(this,tr("Save File"),QDir(path).absolutePath(), QString(tr("%1 files (*.%2);;all files (*.*)")).arg(extensionName(getExportMode()), extension(getExportMode())));
}
