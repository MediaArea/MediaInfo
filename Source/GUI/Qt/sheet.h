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
    static Sheet* get(int i) {
        return sheets[i];
    }

    static int getNbSheets() {
        return sheets.size();
    }
    static int getIndex() {
        return indexDefault;
    }
    static void setDefault(int i);
    static Sheet* add(const char* name);
    static void load(QSettings* settings);
    static void save(QSettings* settings);
    void addColumn(const char* name, const int width, const int stream, const char* keyw);
    void addColumn(column c);
    void resetColumns() {
        columns.clear();
    }
    int getNbColumns() {
        return columns.size();
    }
    column getColumn(int i);
    QString getName() {
        return name;
    }
    void setName(const char* n) {
        name = n;
    }
    static void remove(int i) {
        sheets.remove(i);
        if(getIndex()>=sheets.size())
            setDefault(sheets.size()-1);
    }
    static void removeLast() {
        remove(sheets.size()-1);
    }
    bool getAdaptColumns() {
        return adaptColumns;
    }
    void setAdaptColumns(bool ac) {
        adaptColumns = ac;
    }

private:
    Sheet(const char* name);
    QString name;
    QVector<column> columns;
    bool adaptColumns;

    static QVector<Sheet*> sheets;
    static int indexDefault;
};

#endif // SHEET_H
