/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
