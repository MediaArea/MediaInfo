/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "sheet.h"

int Sheet::indexDefault = 0;
QVector<Sheet*> Sheet::sheets = QVector<Sheet*>();
QStringList Sheet::names = QStringList();

Sheet::Sheet(QString name)
{
    this->name = name;
}

/* STATIC FUNCTIONS */

void Sheet::load(QSettings* settings) {
    qDebug() << "loading sheets";
    int size = settings->beginReadArray("sheets");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        Sheet* s = new Sheet(settings->value("name").toString());
        s->setAdaptColumns(settings->value("adaptColumns",false).toBool());
        qDebug() << "...loading " << s->getName();
        int nbColumns = settings->value("nbColumns",0).toInt();
        for(int j=0;j<nbColumns;j++) {
            s->addColumn(settings->value("nameCol"+QString::number(j)).toString().toStdString().c_str(),settings->value("widthCol"+QString::number(j)).toInt(),settings->value("streamCol"+QString::number(j)).toInt(),settings->value("keywordCol"+QString::number(j)).toString().toStdString().c_str());
        }
        sheets.append(s);
        names.append(s->getName());
    }
    settings->endArray();
    indexDefault = settings->value("SheetIndexDefault",0).toInt();
    qDebug() << "end loading";
}

void Sheet::save(QSettings* settings) {
    qDebug() << "saving sheets";
    settings->beginWriteArray("sheets");
    for (int i = 0; i < sheets.size(); ++i) {
        settings->setArrayIndex(i);
        qDebug() << "...saving " << sheets[i]->getName();
        settings->setValue("name", sheets[i]->getName());
        settings->setValue("adaptColumns", sheets[i]->getAdaptColumns());
        settings->setValue("nbColumns", sheets[i]->getNbColumns());
        for(int j=0;j<sheets[i]->getNbColumns();++j) {
            settings->setValue("nameCol"+QString::number(j),sheets[i]->getColumn(j).name);
            settings->setValue("widthCol"+QString::number(j),sheets[i]->getColumn(j).width);
            settings->setValue("streamCol"+QString::number(j),(int)sheets[i]->getColumn(j).stream);
            settings->setValue("keywordCol"+QString::number(j),sheets[i]->getColumn(j).key);
        }
    }
    settings->endArray();
    settings->setValue("SheetIndexDefault",indexDefault);
    qDebug() << "end saving";
}

void Sheet::remove(int i) {
    sheets.remove(i);
    if(getIndex()>=sheets.size())
        setDefault(sheets.size()-1);
}

void Sheet::removeLast() {
    remove(sheets.size()-1);
}

Sheet* Sheet::get(int i) {
    return sheets[i];
}

Sheet* Sheet::getSheet() {
    return get(indexDefault);
}

int Sheet::getNbSheets() {
    return sheets.size();
}

void Sheet::setDefault(int i) {
    indexDefault = i;
}

int Sheet::getIndex() {
    return indexDefault;
}

Sheet* Sheet::add(QString name) {
    if(names.contains(name)) {
        int n;
        for(n=2;names.contains(name+QString::number(n));n++);
        name.append(QString::number(n));
    }
    Sheet* s = new Sheet(name);
    sheets.append(s);
    names.append(s->getName());
    setDefault(sheets.size()-1);
    return s;
}

/* METHODS */

void Sheet::addColumn(const char* name, const int width, const int stream, const char* keyw) {
    column c;
    c.key = QString(keyw);
    c.name = QString(name);
    c.stream = (stream_t)stream;
    c.width = width;
    addColumn(c);
}
void Sheet::addColumn(column c) {
    columns.append(c);
}

column Sheet::getColumn(int i)  {
    return columns[i];
}

void Sheet::resetColumns() {
    columns.clear();
}

int Sheet::getNbColumns() {
    return columns.size();
}

QString Sheet::getName() {
    return name;
}

void Sheet::setName(QString n) {
    name = n;
}

bool Sheet::getAdaptColumns() {
    return adaptColumns;
}

void Sheet::setAdaptColumns(bool ac) {
    adaptColumns = ac;
}
