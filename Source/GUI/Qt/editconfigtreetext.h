#ifndef EDITCONFIGTREETEXT_H
#define EDITCONFIGTREETEXT_H

#include "translate.h"
#include <QtGui/QDialog>
#include "Common/Core.h"
#include "configtreetext.h"
#include <QtCore/QDebug>

namespace Ui {
    class editConfigTreeText;
}

class EditConfigTreeText : public QDialog {
    Q_OBJECT
public:
    EditConfigTreeText(ConfigTreeText* ctt, Core* C, QWidget *parent = 0);
    ~EditConfigTreeText();
    void apply();

protected:
    void changeEvent(QEvent *e);
    void fillToolBox();

private:
    Ui::editConfigTreeText *ui;
    Core* C;
    ConfigTreeText* ctt;
};

#endif // EDITCONFIGTREETEXT_H
