#include "sheet.h"

int Sheet::indexDefault = 0;
QVector<Sheet*> Sheet::sheets = QVector<Sheet*>();

Sheet::Sheet(const char* name)
{
    this->name = QString(name);
}
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

void Sheet::load(QSettings* settings) {
    qDebug("loading sheets");
    int size = settings->beginReadArray("sheets");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        Sheet* s = new Sheet(settings->value("name").toString().toStdString().c_str());
        s->setAdaptColumns(settings->value("adaptColumns",false).toBool());
        qDebug(("...loading "+s->getName()).toStdString().c_str());
        int nbColumns = settings->value("nbColumns",0).toInt();
        for(int j=0;j<nbColumns;j++) {
            s->addColumn(settings->value("nameCol"+QString::number(j)).toString().toStdString().c_str(),settings->value("widthCol"+j).toInt(),settings->value("streamCol"+j).toInt(),settings->value("keywordCol"+j).toString().toStdString().c_str());
        }
        sheets.append(s);
    }
    settings->endArray();
    qDebug("end loading");
}

void Sheet::save(QSettings* settings) {
    qDebug("saving sheets");
    settings->beginWriteArray("sheets");
    for (int i = 0; i < sheets.size(); ++i) {
        settings->setArrayIndex(i);
        qDebug(("...saving "+sheets[i]->getName()).toStdString().c_str());
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
    qDebug("end saving");
}

column Sheet::getColumn(int i)  {
    return columns[i];
}

Sheet* Sheet::getSheet() {
    return sheets[indexDefault];
}

void Sheet::setDefault(int i) {
    indexDefault = i;
}

Sheet* Sheet::add(const char* name) {
    Sheet* s = new Sheet(name);
    sheets.append(s);
    setDefault(sheets.size()-1);
    return s;
}
