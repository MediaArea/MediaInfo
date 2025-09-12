/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QDebug>

namespace Ui {
    class About;
}

class About : public QDialog {
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);
    ~About();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::About *ui;

private slots:
    void on_checkversion_clicked();
    void on_donate_clicked();
    void on_mail_clicked();
    void on_website_clicked();
    void on_okButton_clicked();
};

#endif // ABOUT_H
