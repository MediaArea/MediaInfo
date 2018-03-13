/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef CONFIGTREETEXT_H
#define CONFIGTREETEXT_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QComboBox>
#include <QDebug>

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
