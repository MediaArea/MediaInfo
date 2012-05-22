// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef CONFIGTREETEXT_H
#define CONFIGTREETEXT_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtGui/QComboBox>
#include <QtCore/QDebug>

class ConfigTreeText
{
public:
    static ConfigTreeText* getConfigTreeText();
    static ConfigTreeText* get(int i);
    static int getNbConfigTreeTexts();
    static int getIndex();
    static void setDefault(int i);
    static ConfigTreeText* add(QString name);
    static void load(QSettings* settings);
    static void save(QSettings* settings);
    static void remove(int i);
    static void removeLast();

    static void fillComboBox(QComboBox* qcb);

    QString getName();
    void setName(QString n);
    QList<QStringList> getFields();
    QStringList getFields(int i);
    void setFields(int i, QStringList fs);
    void addField(int i, QString f);
    void removeField(int i, QString f);

private:
    ConfigTreeText(QString name);
    QString name;
    QList<QStringList> fields;

    static QVector<ConfigTreeText*> configs;
    static QStringList names;
    static int indexDefault;
};

#endif // CONFIGTREETEXT_H
