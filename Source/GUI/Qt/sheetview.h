/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef SHEETVIEW_H
#define SHEETVIEW_H

#include <QFrame>
#include "Common/Core.h"
#include <QDebug>

namespace Ui {
    class SheetView;
}

class SheetView : public QFrame {
    Q_OBJECT
public:
    SheetView(Core* C, QWidget *parent = 0, QFont* monoFont = nullptr);
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
