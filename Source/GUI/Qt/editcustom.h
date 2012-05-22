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

#ifndef EDITCUSTOM_H
#define EDITCUSTOM_H

#include <QtGui/QDialog>
#include "Common/Core.h"
#include "custom.h"

namespace Ui {
    class EditCustom;
}

class EditCustom : public QDialog {
    Q_OBJECT
public:
    EditCustom(Custom* c, Core* C, QWidget *parent = 0);
    ~EditCustom();
    void apply();
    void refreshDisplay();
    void saveText();

protected:
    void changeEvent(QEvent *e);
    //void fillToolBox();

private:
    Ui::EditCustom *ui;
    Core* C;
    Custom* c;

private slots:
    void on_pushButton_clicked();
    void on_textEdit_textChanged();
    void on_treeWidget_itemSelectionChanged();
};

#endif // EDITCUSTOM_H
