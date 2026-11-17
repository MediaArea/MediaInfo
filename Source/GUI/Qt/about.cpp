/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include "about.h"
#include "ui_about.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QPalette>
#include <QPixmap>
#include <QUrl>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

#if !defined(_WIN32) || !defined(WINAPI_FAMILY) || (WINAPI_FAMILY!=WINAPI_FAMILY_APP) // Workaround render bug
    setWindowTitle(tr("About"));
#endif

#if defined(_WIN32) && defined(WINAPI_FAMILY) && (WINAPI_FAMILY==WINAPI_FAMILY_APP) // Remove donate button
    ui->donate->setVisible(false);
#endif

    updateLogo();
    ui->logoMail->setPixmap(QIcon(":/icon/aboutmail.svg").pixmap(ui->logoMail->maximumSize()));
    ui->aboutText->setText(ui->aboutText->text().arg(QCoreApplication::applicationVersion()));
}

// Black logo on a light background, inverted logo (white disc and crescents) on a dark one
void About::updateLogo()
{
    bool dark = palette().color(QPalette::Window).lightness() < 128;
    ui->logo->setPixmap(QPixmap(dark ? ":/icon/icon_inverted.png" : ":/icon/icon.png"));
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
    case QEvent::PaletteChange: // e.g. light/dark theme switch
        updateLogo();
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
    QDesktopServices::openUrl(QUrl("https://MediaArea.net/MediaInfo"));
}

void About::on_mail_clicked()
{
    QDesktopServices::openUrl(QUrl("mailto:info@mediaarea.net"));
}

void About::on_donate_clicked()
{
    QDesktopServices::openUrl(QUrl("https://MediaArea.net/MediaInfo/Donate"));
}

void About::on_checkversion_clicked()
{
    QDesktopServices::openUrl(QUrl("https://MediaArea.net/MediaInfo"));
    // copied from the VCL interface, not adapted for multi-plateform.
}
