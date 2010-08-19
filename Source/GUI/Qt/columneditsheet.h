#ifndef COLUMNEDITSHEET_H
#define COLUMNEDITSHEET_H

#include "sheet.h"
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

class ColumnEditSheet : public QHBoxLayout
{
Q_OBJECT
public:
    explicit ColumnEditSheet(column c, int pos, int nb, Core* C, QWidget *parent = 0);
    ~ColumnEditSheet();
    QSpinBox* widthBox() {
        return spinbox;
    }

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
    void posSwitched(int,int,int);
    void posRemoved(int,int);
    void posChanged(int);
    void fillCombobox();

private:
    int pos;
    column col;
    QToolButton* up;
    QToolButton* down;
    QComboBox* stream;
    QComboBox* combobox;
    QSpinBox* spinbox;
    Core* C;
};

#endif // COLUMNEDITSHEET_H
