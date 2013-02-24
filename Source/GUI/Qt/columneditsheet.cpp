// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "columneditsheet.h"
#include "sheet.h"
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QToolButton>
#include "translate.h"

#include <ZenLib/Ztring.h>
using namespace ZenLib;
#define wstring2QString(_DATA) \
    QString::fromUtf8(Ztring(_DATA).To_UTF8().c_str())
#define QString2wstring(_DATA) \
    Ztring().From_UTF8(_DATA.toUtf8())

ColumnEditSheet::ColumnEditSheet(column c, int pos, int nb, Core* C, QWidget *parent) :
    QHBoxLayout(parent)
{
    this->pos = pos;
    this->C = C;
    this->col = c;

    QLineEdit* lineedit = new QLineEdit(c.name);
    this->addWidget(lineedit);
    spinbox = new QSpinBox();
    spinbox->setMinimum(0);
    spinbox->setMaximum(500);
    spinbox->setValue(c.width);
    this->addWidget(spinbox);
    stream = new QComboBox();
    for (int streamKind=(int)Stream_General; streamKind<(int)Stream_Max; streamKind++)
    {
        if(streamKind==Stream_Other)
            continue;
        stream->addItem(wstring2QString(C->StreamName((stream_t)streamKind)),streamKind);
    }
    stream->setCurrentIndex(c.stream);
    this->addWidget(stream);
    combobox = new QComboBox();
    combobox->addItem(c.key,c.key);
    this->addWidget(combobox);
    up = new QToolButton();
    up->setArrowType(Qt::UpArrow);
    //up->setSizePolicy(QSizePolicy::Minimum);
    this->addWidget(up);
    down = new QToolButton();
    down->setArrowType(Qt::DownArrow);
    down->setBaseSize(480,480);
    down->setMaximumHeight(spinbox->height());
    this->addWidget(down);
    minus = new QToolButton();
    minus->setText("-");
    this->addWidget(minus);

    connect(lineedit,SIGNAL(textChanged(QString)),SLOT(contentChanged()));
    connect(spinbox,SIGNAL(valueChanged(int)),SLOT(contentChanged()));
    connect(up,SIGNAL(clicked()),SLOT(upButton()));
    connect(down,SIGNAL(clicked()),SLOT(downButton()));
    connect(minus,SIGNAL(clicked()),SLOT(minusButton()));
    connect(stream,SIGNAL(currentIndexChanged(int)),SLOT(fillCombobox()));

    fillCombobox();

    posChanged(nb);
}

ColumnEditSheet::~ColumnEditSheet() {
    while (QLayoutItem* i = takeAt(0)) {
        delete i->widget();
    }
}

void ColumnEditSheet::fillCombobox() {
    combobox->clear();
    QString s = wstring2QString(C->Parameters());
    s.replace("\r\n","\n").replace("\r","\n");
    s.remove(0,s.indexOf(stream->currentText()+"\n"));
    s.truncate((s.indexOf("\n\n")==-1?s.size():s.indexOf("\n\n")));
    QStringList sl = s.split("\n");
    sl.removeAt(0);
    sl.replaceInStrings(QRegExp(";(.*)"),"");
    for (int i=0; i<sl.size(); ++i)
        combobox->addItem(sl.at(i),sl.at(i));
    if(stream->itemData(stream->currentIndex()).toInt()==col.stream)
        combobox->setCurrentIndex(sl.indexOf(col.key));
}

void ColumnEditSheet::contentChanged() {
    emit somethingChanged();
}

void ColumnEditSheet::upButton() {
    emit moveMeUp(pos);
}

void ColumnEditSheet::downButton() {
    emit moveMeDown(pos);
}

void ColumnEditSheet::minusButton() {
    emit removeMe(pos);
}

void ColumnEditSheet::posSwitched(int p1, int p2, int nb) {
    if(pos==p1)
        pos=p2;
    else if(pos==p2)
        pos=p1;
    posChanged(nb);
}

void ColumnEditSheet::posRemoved(int p, int nb) {
    if(pos>p)
        pos--;
    posChanged(nb);
}

void ColumnEditSheet::posChanged(int nb) {
    up->setEnabled(pos!=0);
    down->setEnabled(pos!=(nb-1));
    minus->setEnabled(nb>1);
    emit somethingChanged();
}
