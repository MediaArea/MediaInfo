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

#ifndef SHEETVIEW_H
#define SHEETVIEW_H

#include <QtGui/QFrame>
#include "Common/Core.h"
#include <QtCore/QDebug>

namespace Ui {
    class SheetView;
}

class SheetView : public QFrame {
    Q_OBJECT
public:
    SheetView(Core* C, QWidget *parent = 0);
    ~SheetView();

protected:
    void changeEvent(QEvent *e);

public slots:
    void adaptColumnsToContent();
    void resetColumnsSizes();

private:
    Core* C;
    void refreshDisplay();
    Ui::SheetView *ui;
    QString url;

private slots:
    void on_toolButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_tableWidget_itemSelectionChanged();
};

#endif // SHEETVIEW_H
