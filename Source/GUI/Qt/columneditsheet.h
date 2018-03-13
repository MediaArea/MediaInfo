/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef COLUMNEDITSHEET_H
#define COLUMNEDITSHEET_H

#include "sheet.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>

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
    QToolButton* minus;
    QComboBox* stream;
    QComboBox* combobox;
    QSpinBox* spinbox;
    Core* C;
};

#endif // COLUMNEDITSHEET_H
