#ifndef SHEET_H
#define SHEET_H

#include <QtCore/QVector>
#include <QtCore/QString>
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
    static void setDefault(int i);
    static Sheet* add(const char* name);
    static void load(QSettings* settings);
    static void save(QSettings* settings);
    void addColumn(const char* name, const int width, const int stream, const char* keyw);
    int getNbColumns() {
        return columns.size();
    }
    column getColumn(int i);
    QString getName() {
        return name;
    }

private:
    Sheet(const char* name);
    QString name;
    QVector<column> columns;

    static QVector<Sheet*> sheets;
    static int indexDefault;
};

#endif // SHEET_H
