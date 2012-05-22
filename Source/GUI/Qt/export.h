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

#ifndef EXPORT_H
#define EXPORT_H

#include <QtGui/QDialog>
#include <QtCore/QIODevice>
#include <QtCore/QDebug>

namespace Ui {
    class Export;
}

class Export : public QDialog {
    Q_OBJECT
public:
    enum ExportMode {
        TEXT=0,
        HTML=1,
        XML,
        PBCORE,
        MPEG7,
        CSV,

        NB_EXPORT_MODE
    };

    Export(QString filename, int mode, QWidget *parent = 0);
    ~Export();
    int getExportMode();
    int getExportConfig();
    QString getFileName();
    QIODevice::OpenMode getOpenMode();
    bool isAdvancedChecked();
    QString extension(int mode);
    QString name(int mode);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Export *ui;

private slots:
    void on_checkBoxAdvanced_toggled(bool checked);
    void on_comboBoxMode_currentIndexChanged(int index);
    void on_toolButton_pressed();
};

#endif // EXPORT_H
