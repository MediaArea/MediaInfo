/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "prefs.h"
#include "translate.h"
#include "ui_prefs.h"
#include <QLabel>
#include "views.h"
#include "sheet.h"
#include "configtreetext.h"
#include "custom.h"
#include "editsheet.h"
#include "editconfigtreetext.h"
#include "editcustom.h"

Preferences::Preferences(QSettings* settings, Core* C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    this->settings=settings;
    this->C = C;

    ui->setupUi(this);

#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) // Workaround render bug
    QString style = "QComboBox QAbstractItemView { border: 1px solid gray }";
    ui->comboBox_defaultview->setStyleSheet(style);
    ui->comboBoxSheet->setStyleSheet(style);
    ui->customComboBox->setStyleSheet(style);
    ui->treeTextComboBox->setStyleSheet(style);
#else
    setWindowTitle("Preferences");
#endif

    ui->treeWidget->setColumnHidden(1,true);
    ui->treeWidget->expandAll();

    for(int v=VIEW_EASY;v<NB_VIEW;v++) {
        ui->comboBox_defaultview->addItem(nameView((ViewMode)v),v);
    }

    ui->showMenu->setChecked(settings->value("showMenu",true).toBool());
    ui->showToolbar->setChecked(settings->value("showToolbar",true).toBool());
    ui->closeAllBeforeOpen->setChecked(settings->value("closeBeforeOpen",true).toBool());
    ui->comboBox_defaultview->setCurrentIndex(settings->value("defaultView",VIEW_EASY).toInt());
    ui->shellExtension->setChecked(settings->value("shellExtension",true).toBool());
    ui->informVersion->setChecked(settings->value("informVersion",false).toBool());
    ui->informTimestamp->setChecked(settings->value("informTimestamp",false).toBool());
    ui->displayCaptions->setCurrentIndex(settings->value("displayCaptions",1).toInt());
    ui->legacyStreamDisplay->setChecked(settings->value("legacyStreamDisplay",false).toBool());
    ui->enableFFmpeg->setChecked(settings->value("enableFFmpeg",false).toBool());
#ifdef NEW_VERSION
    ui->checkForNewVersion->setChecked(settings->value("checkForNewVersion",true).toBool());
#else
    ui->checkForNewVersion->setVisible(false);
#endif

#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) //Setup UI for winRT
    ui->rememberToolBarPosition->setVisible(false);
    ui->rememberGeometry->setVisible(false);
    ui->showToolbar->setVisible(false);
    ui->showMenu->setVisible(false);
    ui->shellExtension->setVisible(false);
    ui->shellInfoTip->setVisible(false);
#else
    ui->rememberToolBarPosition->setChecked(settings->value("rememberToolBarPosition",true).toBool());
    ui->rememberGeometry->setChecked(settings->value("rememberGeometry",false).toBool());

    ui->showToolbar->setEnabled(ui->showMenu->isChecked());
    ui->showMenu->setEnabled(ui->showToolbar->isChecked());
    ui->rememberToolBarPosition->setEnabled(ui->showToolbar->isChecked());
#endif

    refreshDisplay();

    ui->stackedWidget->setCurrentIndex(0);
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::refreshDisplay() {
    // Text&Tree
    ConfigTreeText::fillComboBox(ui->treeTextComboBox);
    ui->pushButton_editTreeText->setEnabled(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt()>0);
    ui->pushButton_deleteTreeText->setEnabled( (ConfigTreeText::getNbConfigTreeTexts()>1) && (ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt()>0) );

    // Custom
    Custom::fillComboBox(ui->customComboBox);
    ui->pushButton_editCustom->setEnabled(ui->customComboBox->itemData(ui->customComboBox->currentIndex()).toInt()>0);
    ui->pushButton_deleteCustom->setEnabled( (Custom::getNbCustoms()>1) && (ui->customComboBox->itemData(ui->customComboBox->currentIndex()).toInt()>0) );

    // Sheets
    ui->comboBoxSheet->clear();
    for(int i=0;i<Sheet::getNbSheets();i++) {
        ui->comboBoxSheet->addItem(Sheet::get(i)->getName(),i);
    }
    ui->comboBoxSheet->setCurrentIndex(Sheet::getIndex());
    ui->pushButton_deleteSheet->setEnabled(Sheet::getNbSheets()>1);
}

void Preferences::saveSettings() {
    settings->setValue("showMenu",ui->showMenu->isChecked());
    settings->setValue("showToolbar",ui->showToolbar->isChecked());
    settings->setValue("closeBeforeOpen",ui->closeAllBeforeOpen->isChecked());
    settings->setValue("checkForNewVersion",ui->checkForNewVersion->isChecked());
    settings->setValue("defaultView",ui->comboBox_defaultview->currentIndex());
    settings->setValue("rememberToolBarPosition",ui->rememberToolBarPosition->isChecked());
    settings->setValue("rememberGeometry",ui->rememberGeometry->isChecked());
    settings->setValue("shellExtension",ui->shellExtension->isChecked());
    settings->setValue("informVersion",ui->informVersion->isChecked());
    settings->setValue("informTimestamp",ui->informTimestamp->isChecked());
    settings->setValue("displayCaptions",ui->displayCaptions->currentIndex());
    settings->setValue("legacyStreamDisplay",ui->legacyStreamDisplay->isChecked());
    settings->setValue("enableFFmpeg",ui->enableFFmpeg->isChecked());
    Sheet::setDefault(ui->comboBoxSheet->itemData(ui->comboBoxSheet->currentIndex()).toInt());
    Sheet::save(settings);
    ConfigTreeText::setDefault(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt());
    ConfigTreeText::save(settings);
    Custom::setDefault(ui->customComboBox->itemData(ui->customComboBox->currentIndex()).toInt());
    Custom::save(settings);
}

void Preferences::changeEvent(QEvent *e)
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

void Preferences::on_treeWidget_itemSelectionChanged()
{
    if(!ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).isNull())
        ui->stackedWidget->setCurrentIndex(ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).toInt());
}

void Preferences::on_pushButton_editSheet_clicked()
{
    EditSheet es(Sheet::get(ui->comboBoxSheet->itemData(ui->comboBoxSheet->currentIndex()).toInt()), C, this);
    if(es.exec() == QDialog::Accepted) {
        es.apply();
        refreshDisplay();
    } else
        qDebug() << "sheet editing cancelled";
}

void Preferences::on_pushButton_newSheet_clicked()
{
    Sheet* s = Sheet::add("newsheet");
    s->addColumn(Tr("File Name").toStdString().c_str(),300,Stream_General,"CompleteName");
    EditSheet es(s, C, this);
    if(es.exec() == QDialog::Accepted) {
        es.apply();
        refreshDisplay();
    } else {
        Sheet::removeLast();
        qDebug() << "new sheet cancelled";
    }
}

void Preferences::on_pushButton_deleteSheet_clicked()
{
    Sheet::remove(ui->comboBoxSheet->itemData(ui->comboBoxSheet->currentIndex()).toInt());
    refreshDisplay();
}

void Preferences::on_pushButton_editTreeText_clicked()
{
    EditConfigTreeText ectt(ConfigTreeText::get(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt()), C, this);
    if(ectt.exec() == QDialog::Accepted) {
        ectt.apply();
        refreshDisplay();
    } else
        qDebug() << "config editing cancelled";
}

void Preferences::on_pushButton_newTreeText_clicked()
{
    ConfigTreeText* ctt = ConfigTreeText::add("newconfig");
    EditConfigTreeText ectt(ctt, C, this);
    if(ectt.exec() == QDialog::Accepted) {
        ectt.apply();
        refreshDisplay();
    } else {
        ConfigTreeText::removeLast();
        qDebug() << "new config cancelled";
    }
}

void Preferences::on_pushButton_deleteTreeText_clicked()
{
    ConfigTreeText::remove(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt());
    refreshDisplay();
}

void Preferences::on_treeTextComboBox_currentIndexChanged(int index)
{
    ui->pushButton_editTreeText->setEnabled(index>0);
    ui->pushButton_deleteTreeText->setEnabled( (ConfigTreeText::getNbConfigTreeTexts()>1) && (index>0) );
}

void Preferences::on_pushButton_newCustom_clicked()
{
    Custom* c = Custom::add("newconfig");
    EditCustom ec(c, C, this);
    if(ec.exec() == QDialog::Accepted) {
        ec.apply();
        refreshDisplay();
    } else {
        Custom::removeLast();
        qDebug() << "new config cancelled";
    }

}

void Preferences::on_pushButton_editCustom_clicked()
{
    EditCustom ec(Custom::get(ui->customComboBox->itemData(ui->customComboBox->currentIndex()).toInt()), C, this);
    if(ec.exec() == QDialog::Accepted) {
        ec.apply();
        refreshDisplay();
    } else
        qDebug() << "config editing cancelled";

}

void Preferences::on_pushButton_deleteCustom_clicked()
{
    Custom::remove(ui->customComboBox->itemData(ui->customComboBox->currentIndex()).toInt());
    refreshDisplay();

}

void Preferences::on_customComboBox_currentIndexChanged(int index)
{
    ui->pushButton_editCustom->setEnabled(index>0);
    ui->pushButton_deleteCustom->setEnabled( (Custom::getNbCustoms()>1) && (index>0) );
}
