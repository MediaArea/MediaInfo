/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef CUSTOM_H
#define CUSTOM_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QComboBox>
#include <QDebug>

class Custom
{
public:
    static Custom* getCustom();
    static Custom* get(int i);
    static int getNbCustoms();
    static int getIndex();
    static void setDefault(int i);
    static Custom* add(QString name);
    static void load(QSettings* settings);
    static void save(QSettings* settings);
    static void remove(int i);
    static void removeLast();

    static void fillComboBox(QComboBox* qcb);

    QString getName();
    QString getPage();
    QString getFile();
    QString getStream(int stream);
    void setName(const QString& n);
    void setPage(const QString& p);
    void setFile(const QString& f);
    void setStream(int stream, const QString& s);

private:
    Custom(const QString& name);
    QString name;
    QString page;
    QString file;
    QStringList streams;

    static QVector<Custom*> customs;
    static QStringList names;
    static int indexDefault;
};

#endif // CUSTOM_H
