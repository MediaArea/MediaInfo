#ifndef COLUMNEDITSHEET_H
#define COLUMNEDITSHEET_H

#include "sheet.h"
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>

class ColumnEditSheet : public QHBoxLayout
{
Q_OBJECT
public:
    explicit ColumnEditSheet(column c, int pos, Core* C, QWidget *parent = 0);
    ~ColumnEditSheet();

signals:
    void somethingChanged();
    void moveMeUp(int);
    void moveMeDown(int);
    void removeMe(int);

public slots:
    void contentChanged();
    void upButton();
    void downButton();
    void minusButton();
    void posSwitched(int,int);
    void posRemoved(int);
    void posChanged();
    void fillCombobox();

private:
    int pos;
    column col;
    QToolButton* up;
    QToolButton* down;
    QComboBox* stream;
    QComboBox* combobox;
    Core* C;
};

#endif // COLUMNEDITSHEET_H
