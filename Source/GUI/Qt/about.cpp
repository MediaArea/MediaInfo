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
    QDesktopServices::openUrl(QUrl("http://mediainfo.sourceforge.net"));
}

void About::on_mail_clicked()
{
    QDesktopServices::openUrl(QUrl("mailto:info@mediaarea.net"));
}

void About::on_donate_clicked()
{
    QDesktopServices::openUrl(QUrl("http://mediainfo.sourceforge.net/Donate"));
}

void About::on_checkversion_clicked()
{
    QDesktopServices::openUrl(QUrl("http://sourceforge.net/project/showfiles.php?group_id=86862&package_id=90341"));
    // copied from the VCL interface, not adapted for multi-plateform.
}
