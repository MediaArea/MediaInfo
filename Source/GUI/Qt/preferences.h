/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtGui/QDialog>
#include <QtCore/QSettings>
#include "Common/Core.h"
#include <QtCore/QDebug>

namespace Ui {
    class Preferences;
}

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QSettings* settings, Core* C, QWidget *parent = 0);
    ~Preferences();
    void saveSettings();

protected:
    void changeEvent(QEvent *e);

private:
    void refreshDisplay();
    QSettings* settings;
    Ui::Preferences *ui;
    Core* C;

private slots:
    void on_customComboBox_currentIndexChanged(int index);
    void on_pushButton_deleteCustom_clicked();
    void on_pushButton_editCustom_clicked();
    void on_pushButton_newCustom_clicked();
    void on_treeTextComboBox_currentIndexChanged(int index);
    void on_pushButton_deleteTreeText_clicked();
    void on_pushButton_newTreeText_clicked();
    void on_pushButton_editTreeText_clicked();
    void on_pushButton_deleteSheet_clicked();
    void on_pushButton_newSheet_clicked();
    void on_pushButton_editSheet_clicked();
    void on_treeWidget_itemSelectionChanged();
};

#endif // PREFERENCES_H
