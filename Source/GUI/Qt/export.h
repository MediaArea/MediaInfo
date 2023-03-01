/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <QIODevice>
#include <QDebug>

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
        JSON,
        PBCORE,
        PBCORE2,
        EBUCORE_1_5,
        EBUCORE_1_6,
        EBUCORE_1_8_ps,
        EBUCORE_1_8_sp,
        EBUCORE_1_8_ps_JSON,
        EBUCORE_1_8_sp_JSON,
        FIMS_1_1,
        FIMS_1_2,
        FIMS_1_3,
        MPEG7_Strict,
        MPEG7_Relaxed,
        MPEG7_Extended,
        NISO_Z39_87,
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
    QString extensionName(int mode);
    QString name(int mode);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Export *ui;
    QString path;

private slots:
    void on_checkBoxAdvanced_toggled(bool checked);
    void on_comboBoxMode_currentIndexChanged(int index);
    void on_buttonBox_accepted();
};

#endif // EXPORT_H
