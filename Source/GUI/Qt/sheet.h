#ifndef SHEET_H
#define SHEET_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include "Common/Core.h"

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
