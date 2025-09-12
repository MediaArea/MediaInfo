/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "editconfigtreetext.h"
#include "ui_editconfigtreetext.h"

#include "configtreetext.h"
#include "Common/Core.h"
#include <QCheckBox>

#include <ZenLib/Ztring.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

EditConfigTreeText::EditConfigTreeText(ConfigTreeText* ctt, Core* C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editConfigTreeText)
{
    ui->setupUi(this);
    this->C = C;
    this->ctt = ctt;
    fillToolBox();
    ui->lineEdit->setText(ctt->getName());
}

EditConfigTreeText::~EditConfigTreeText()
{
    delete ui;
}

void EditConfigTreeText::fillToolBox() {
    for(;ui->toolBox->count()>0;ui->toolBox->removeItem(0));

    static const QRegularExpression RegEx(";(.*)");
    for (int streamKind=0; streamKind<(int)Stream_Max; streamKind++) {
        if(streamKind==Stream_Other)
            continue;
        QFrame* box = new QFrame();
        qDebug() << "adding " << wstring2QString(C->StreamName((stream_t)streamKind));
        ui->toolBox->addItem(box,wstring2QString(C->StreamName((stream_t)streamKind)));
        QString s = wstring2QString(C->Parameters());
        s.replace("\r\n","\n").replace("\r","\n");
        s.remove(0,s.indexOf(wstring2QString(C->StreamName((stream_t)streamKind))+"\n"));
        s.truncate((s.indexOf("\n\n")==-1?s.size():s.indexOf("\n\n")));
        QStringList sl = s.split("\n");
        sl.removeAt(0);
        sl.replaceInStrings(RegEx,"");
        box->setLayout(new QVBoxLayout());
        for (int i=0; i<sl.size(); ++i) {
            QCheckBox* check = new QCheckBox(sl.at(i));
            check->setChecked(ctt->getFields(streamKind).contains(sl.at(i)));
            box->layout()->addWidget(check);
        }
    }
}

void EditConfigTreeText::changeEvent(QEvent *e)
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

void EditConfigTreeText::apply() {
    ctt->setName(ui->lineEdit->text());

    for (int i=0; i<ui->toolBox->count(); i++) {
        for (int j=0; j<ui->toolBox->widget(i)->layout()->count(); ++j) {
            if(static_cast<QCheckBox*>(ui->toolBox->widget(i)->layout()->itemAt(j)->widget())->isChecked()) {
                ctt->addField(i, static_cast<QCheckBox*>(ui->toolBox->widget(i)->layout()->itemAt(j)->widget())->text());
                qDebug() << ctt->getFields(i).join(", ");
            } else {
                ctt->removeField(i, static_cast<QCheckBox*>(ui->toolBox->widget(i)->layout()->itemAt(j)->widget())->text());
            }
        }
    }

    qDebug() << "Applying settings";
}
