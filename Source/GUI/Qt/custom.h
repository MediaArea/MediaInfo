/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef CUSTOM_H
#define CUSTOM_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtGui/QComboBox>
#include "Common/Core.h"
#include <QtCore/QDebug>

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
    void setName(QString n);
    void setPage(QString p);
    void setFile(QString f);
    void setStream(int stream, QString s);

private:
    Custom(QString name);
    QString name;
    QString page;
    QString file;
    QStringList streams;

    static QVector<Custom*> customs;
    static QStringList names;
    static int indexDefault;
};

#endif // CUSTOM_H
