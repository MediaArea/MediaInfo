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
