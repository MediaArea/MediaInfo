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

#ifndef SHEET_H
#define SHEET_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include "Common/Core.h"
#include <QtCore/QDebug>

typedef struct {
    QString name;
    int width;
    stream_t stream;
    QString key;
} column;

class Sheet
{
public:
    static Sheet* getSheet();
    static Sheet* get(int i);
    static int getNbSheets();
    static int getIndex();
    static void setDefault(int i);
    static Sheet* add(QString name);
    static void load(QSettings* settings);
    static void save(QSettings* settings);
    static void remove(int i);
    static void removeLast();

    void addColumn(const char* name, const int width, const int stream, const char* keyw);
    void addColumn(column c);
    void resetColumns();
    int getNbColumns();
    column getColumn(int i);
    QString getName();
    void setName(QString n);
    bool getAdaptColumns();
    void setAdaptColumns(bool ac);

private:
    Sheet(QString name);
    QString name;
    QVector<column> columns;
    bool adaptColumns;

    static QVector<Sheet*> sheets;
    static QStringList names;
    static int indexDefault;
};

#endif // SHEET_H
