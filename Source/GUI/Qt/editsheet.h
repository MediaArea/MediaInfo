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

#ifndef EDITSHEET_H
#define EDITSHEET_H

#include <QtGui/QDialog>
#include "sheet.h"
#include <QtCore/QDebug>

namespace Ui {
    class EditSheet;
}

class EditSheet : public QDialog {
    Q_OBJECT
public:
    EditSheet(Sheet* s, Core* C, QWidget *parent = 0);
    ~EditSheet();
    void apply();

public slots:
    void upCol(int i);
    void downCol(int i);
    void delCol(int i);
    void addColumn();
    void refreshDisplay();

signals:
    void switchPos(int,int,int);
    void deletePos(int,int);
    void newPos(int);

protected:
    void changeEvent(QEvent *e);

private:
    void initDisplay();
    QLayout* createColumn(column c);

    Sheet* sheet;
    Core* C;
    Ui::EditSheet *ui;
};

#endif // EDITSHEET_H
