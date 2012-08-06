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
