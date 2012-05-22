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

#ifndef COLUMNEDITSHEET_H
#define COLUMNEDITSHEET_H

#include "sheet.h"
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtCore/QDebug>

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
