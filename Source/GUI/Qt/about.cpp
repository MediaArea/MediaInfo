/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "translate.h"
#include "about.h"
#include "_Automated/ui_about.h"
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void About::on_okButton_clicked()
{
        this->accept();
}

void About::on_website_clicked()
{
    QDesktopServices::openUrl(QUrl("http://MediaArea.net/MediaInfo"));
}

void About::on_mail_clicked()
{
    QDesktopServices::openUrl(QUrl("mailto:info@mediaarea.net"));
}

void About::on_donate_clicked()
{
    QDesktopServices::openUrl(QUrl("http://MediaArea.net/MediaInfo/Donate"));
}

void About::on_checkversion_clicked()
{
    QDesktopServices::openUrl(QUrl("http://MediaArea.net/MediaInfo"));
    // copied from the VCL interface, not adapted for multi-plateform.
}
