/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "export.h"
#include "ui_export.h"
#include <QFileDialog>
#include <QMessageBox>
#include "configtreetext.h"

Export::Export(const QString& filename, int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export),
    path(filename)
{
    ui->setupUi(this);

#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) // Workaround render bug
    QString style = "QComboBox QAbstractItemView { border: 1px solid gray }";
    ui->comboBoxConfig->setStyleSheet(style);
    ui->comboBoxMode->setStyleSheet(style);
#else
    setWindowTitle(tr("Export"));
#endif

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
    case GRAPH: return "svg";
        break;
    case XML:
    case PBCORE:
    case PBCORE2:
    case MPEG7_Strict:
    case MPEG7_Relaxed:
    case MPEG7_Extended:
    case NISO_Z39_87:
    case FIMS_1_1:
    case FIMS_1_2:
    case FIMS_1_3:
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
    case GRAPH: return tr("Graph");
        break;
    case XML:
    case PBCORE:
    case PBCORE2:
    case MPEG7_Strict:
    case MPEG7_Relaxed:
    case MPEG7_Extended:
    case NISO_Z39_87:
    case FIMS_1_1:
    case FIMS_1_2:
    case FIMS_1_3:
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
    case TEXT: return tr("Text");
        break;
    case HTML: return tr("HTML");
        break;
    case XML: return tr("XML");
        break;
    case JSON: return tr("JSON");
        break;
    case GRAPH: return tr("Graph");
        break;
    case PBCORE: return tr("PBCore");
        break;
    case PBCORE2: return tr("PBCore 2");
        break;
    case MPEG7_Strict: return tr("MPEG-7 (strict)");
        break;
    case MPEG7_Relaxed: return tr("MPEG-7 (relaxed)");
        break;
    case MPEG7_Extended: return tr("MPEG-7 (extended)");
        break;
   case EBUCORE_1_5: return tr("EBUCore 1.5");
       break;
   case EBUCORE_1_6: return tr("EBUCore 1.6");
       break;
   case EBUCORE_1_8_ps: return tr("EBUCore 1.8 parameter then segment");
       break;
   case EBUCORE_1_8_sp: return tr("EBUCore 1.8 segment then parameter");
       break;
   case EBUCORE_1_8_ps_JSON: return tr("EBUCore 1.8 parameter then segment (JSON output)");
       break;
   case EBUCORE_1_8_sp_JSON: return tr("EBUCore 1.8 segment then parameter (JSON output)");
       break;
   case FIMS_1_1: return tr("FIMS 1.1");
       break;
   case FIMS_1_2: return tr("FIMS 1.2");
       break;
   case FIMS_1_3: return tr("FIMS 1.3");
       break;
    case NISO_Z39_87: return tr("NISO Z39.87");
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
    case GRAPH:
    case PBCORE:
    case PBCORE2:
    case MPEG7_Strict:
    case MPEG7_Relaxed:
    case MPEG7_Extended:
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
    path = QFileDialog::getSaveFileName(this,tr("Save File"),QDir(path).absolutePath(), QString(tr("%1 files (*.%2)")).arg(extensionName(getExportMode()), extension(getExportMode())));
}
