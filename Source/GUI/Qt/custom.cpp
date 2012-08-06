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

#include "custom.h"

int Custom::indexDefault = 0;
QVector<Custom*> Custom::customs = QVector<Custom*>();
QStringList Custom::names = QStringList();

Custom::Custom(QString name)
{
    this->name = name;
    this->page = "%Files%";
    this->file = "%General%";
    for(int i=0;i<Stream_Max;i++) {
        streams.append("");
    }
}

/* STATIC FUNCTIONS */

void Custom::load(QSettings* settings) {
    qDebug() << "loading customs";
    int size = settings->beginReadArray("customs");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        Custom* s = new Custom(settings->value("name").toString());
        qDebug() << "...loading " << s->getName();
        s->page = settings->value("page","%Files%").toString();
        s->file = settings->value("file","%General%").toString();
        s->streams = settings->value("streams","").toStringList();
        customs.append(s);
        names.append(s->getName());
    }
    settings->endArray();
    indexDefault = settings->value("CustomIndexDefault",0).toInt();
    qDebug() << "end loading";
}

void Custom::save(QSettings* settings) {
    qDebug() << "saving customs";
    settings->beginWriteArray("customs");
    for (int i = 0; i < customs.size(); ++i) {
        settings->setArrayIndex(i);
        qDebug() << "...saving " << customs[i]->getName();
        settings->setValue("name", customs[i]->getName());
        settings->setValue("page", customs[i]->page);
        settings->setValue("file", customs[i]->file);
        settings->setValue("streams", customs[i]->streams);
    }
    settings->endArray();
    settings->setValue("CustomIndexDefault",indexDefault);
    qDebug() << "end saving";
}

void Custom::remove(int i) {
    customs.remove(i);
    if(getIndex()>=customs.size())
        setDefault(customs.size()-1);
}

void Custom::removeLast() {
    remove(customs.size()-1);
}

Custom* Custom::get(int i) {
    return customs[i];
}

Custom* Custom::getCustom() {
    return get(indexDefault);
}

int Custom::getNbCustoms() {
    return customs.size();
}

void Custom::setDefault(int i) {
    indexDefault = i;
}

int Custom::getIndex() {
    return indexDefault;
}

Custom* Custom::add(QString name) {
    if(names.contains(name)) {
        int n;
        for(n=2;names.contains(name+QString::number(n));n++);
        name.append(QString::number(n));
    }
    Custom* s = new Custom(name);
    customs.append(s);
    names.append(s->getName());
    setDefault(customs.size()-1);
    return s;
}

void Custom::fillComboBox(QComboBox* qcb) {
    qcb->clear();
    for(int i=0;i<Custom::getNbCustoms();i++) {
        qcb->addItem(Custom::get(i)->getName(),i);
    }
    qcb->setCurrentIndex(Custom::getIndex());
}

/* METHODS */

QString Custom::getName() {
    return name;
}
QString Custom::getPage() {
    return page;
}
QString Custom::getFile() {
    return file;
}
QString Custom::getStream(int stream) {
    return streams[stream];
}

void Custom::setName(QString n) {
    name = n;
}
void Custom::setPage(QString p) {
    page = p;
}

void Custom::setFile(QString f) {
    file = f;
}

void Custom::setStream(int stream, QString s) {
    if(stream<streams.size())
       streams[stream] = s;
}
