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
    QDesktopServices::openUrl(QUrl("mailto:mediainfo@yopmail.com"));
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
